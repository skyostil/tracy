# -*- encoding: iso-8859-1 -*-
# Copyright (c) 2011 Nokia
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

"""Analyzer engine."""

import sys
import traceback
import inspect
import BinaryCodec
import Trace
import TraceOperations
import Plugin
import Common
from common import Task
from common import Log
from common import Project
from common import Console
from common import pyparsing

# Try to import the readline module for nicer command editing capabilities
try:
  import readline
except ImportError:
  pass
  
# Try to enable runtime function specialization
try:
  import psyco
  psyco.full()
except:
  #Log.warn("Unable to enable psyco; expect somewhat poor performance.")
  pass

# Try to import profiling
try:
  import hotshot, hotshot.stats
except:
  hotshot = None
  pass

def getFunctionArguments(func):
  # See which arguments are available
  availableArgs, _, _, _ = inspect.getargspec(func)
  return availableArgs
  
def completeKeywordArguments(func, args):
  kwargs = {}
  availableArgs = getFunctionArguments(func)
  
  for arg, value in args.items():
    # If the argument is not a direct match, try to find the nearest match
    if not arg in availableArgs:
      for availableArg in reversed(availableArgs):
        if availableArg.lower().startswith(arg):
          arg = availableArg
          break
    kwargs[arg] = value
  return kwargs

class ExecutionError(Exception):
  pass

class Analyzer(object):
  def __init__(self, project):
    self.project       = project
    self.traces        = {}
    self.importPlugins = []
    self.exportPlugins = []

  def reportInfo(self, msg):
    pass

  def reportWarning(self, msg):
    pass

  def reportError(self, msg):
    pass
    
  def reportDebug(self, msg):
    pass

class ScriptParser(object):
  def __init__(self):
    # Script grammar
    p = pyparsing
    p.ParserElement.setDefaultWhitespaceChars(" \t")
    self.commandName     = p.Word(p.alphas, p.alphanums + "-")
    self.argName         = p.Word(p.alphas, p.alphanums + "-")
    self.argValue        = p.Word(p.alphanums + "_@.-+*/\\:[]{}$#") | p.QuotedString('"', escChar = "\\")
    self.commandArg      = p.Group(p.Optional(self.argName + p.Suppress("="), default = None) + self.argValue)
    self.command         = p.Optional(
                             p.Group(p.Literal("select") + p.SkipTo(p.lineEnd | p.Literal(";"))) |
                             p.Group(self.commandName + p.Optional(p.Group(p.ZeroOrMore(self.commandArg)), default = []))
                           )
    self.commandList     = p.delimitedList(self.command, ";")
    self.script          = p.delimitedList(self.commandList, "\n")
    
  def parse(self, script):
    try:
      return self.script.parseString(script)
    except pyparsing.ParseException:
      return []

class InteractiveAnalyzer(Analyzer, Task.TaskMonitor):
  """Interactive command line analyzer."""
  def __init__(self, project, options):
    Analyzer.__init__(self, project)
    self.commands             = {}
    self.commandHelp          = {}
    self.options              = options
    self.parser               = ScriptParser()
    self.done                 = False
    self.colorizer            = Console.colorizer
    self.taskMonitor          = Task.RateLimitedTaskMonitor(self)
    self.profiling            = False
    self.plugins              = []
    self.traceFiles           = {}
    self.customSyntaxCommands = set()
    
    if not self.project.targets:
      Log.warn("Using empty project file.")
      
    if not self.projectHasApiDescription():
      Log.warn("Project file does not contain an API description.")
  
    # Register internal commands  
    self.registerCommand("quit", self.quit)

    # Start receiving task notifications
    Task.setMonitor(self.taskMonitor)

    # Load all plugins
    self._loadPlugins()

  def _loadPlugins(self):
    self.importPlugins = []
    self.exportPlugins = []
    self.plugins       = []
    
    def loadPlugins(pluginModule, collection, loader):
      plugins = loader(self, pluginModule)
      if self.options.verbose:
        for p in plugins:
          self.reportDebug("Loaded plugin: %s" % p)
      collection.extend(plugins)
    
    for plugin in Plugin.getAvailablePluginModules():
      loadPlugins(plugin, self.plugins,       Plugin.loadInteractivePlugins)
      loadPlugins(plugin, self.importPlugins, Plugin.loadImporterPlugins)
      loadPlugins(plugin, self.exportPlugins, Plugin.loadExporterPlugins)
      

  def projectHasApiDescription(self):
    if not self.project or not "code" in self.project.targets:
      return False
    return True

  def reportInfo(self, msg):
    for line in str(msg).rstrip().split("\n"):
      Log.notice(line)

  def reportWarning(self, msg):
    for line in str(msg).rstrip().split("\n"):
      Log.warn(line)

  def reportError(self, msg):
    for line in str(msg).rstrip().split("\n"):
      Log.error(line)
    
  def reportDebug(self, msg):
    for line in str(msg).rstrip().split("\n"):
      Log.debug(line)

  def fail(self, message, showException = False):
    if showException and sys.exc_info():
      self.reportDebug("".join(traceback.format_exception(*sys.exc_info())))
    self.reportError(message)
    raise ExecutionError(message)

  def registerCommand(self, command, function, helpFunction = None, customSyntax = False):
    # Note: A command with a custom syntax should also be added to the parser syntax 
    # defininition
    self.commands[command]    = function
    self.commandHelp[command] = helpFunction
    if customSyntax:
      self.customSyntaxCommands.add(command)

  def taskStarted(self, task):
    pass

  def taskProgress(self, task):
    Console.printProgressBar(task, self.colorizer)

  def taskFinished(self, task):
    Console.eraseProgressBar()
    
  def parseEventRange(self, trace, eventRange):
    """
    Convert an event range description string into a concrete event range for a trace.
    The string may be an integral event number or a frame number preceded with '#',
    or a range with ":" as the separator between start and end events.
    
    @param trace        Trace for event
    @param eventNumber  Event range string
    
    @returns the first and last event number specified by the range
    """
    if not eventRange:
      return (0, len(trace.events))
    
    def parse(number, base = 0, default = 0):
      if not number:
        return default
      
      if number[0] == '+':
        offset = base
        number = number[1:]
      else:
        offset = 0
        
      try:
        return int(number) + offset
      except ValueError:
        assert number[0] == "#"
        targetFrameNumber = int(number[1:])
        frameNumber       = 0
        
        if targetFrameNumber < 0:
          frames = len([e for e in trace.events if self.lookupFunction(e).isFrameMarker])
          targetFrameNumber += frames
        
        i = 0
        for i, event in enumerate(trace.events[offset:]):
          if frameNumber == targetFrameNumber:
            break
          if self.lookupFunction(event).isFrameMarker:
            frameNumber += 1
        return i + offset
    
    try:
      first, last = eventRange.split(":")
    except ValueError:
      first, last = eventRange, eventRange
      
    first = parse(first)
    last  = parse(last, first, len(trace.events))
    if first == last: last += 1
    return (first, last)
    
  def parseBoolean(self, boolean):
    """
    Parse a boolean descriptor and return True or False accordingly.
    """
    try:
      return bool(int(boolean))
    except ValueError:
      if boolean.lower() in ["yes", "on", "true", "enabled"]:
        return True
    return False
    
  def quit(self):
    """
    Exit the analyzer.
    """
    self.done = True
    
  def lookupFunction(self, event):
    """
    Fetch a function from the project's library for an event.
    
    @param event:       Event for which the function should be sought
    @returns a Function or None if it wasn't found
    """
    try:
      library = self.project.targets["code"].library
    except KeyError:
      return
    
    if event.name in library.functions:
      return library.functions[event.name]

  def completeCommand(self, command):
    if command not in self.commands:
      parts = command.split("-")
      for fullCommand in sorted(self.commands.keys()):
        fullParts = fullCommand.split("-")
        if not len(fullParts) == len(parts):
          continue
        for p1, p2 in zip(fullParts, parts):
          if not p1.startswith(p2):
            break
        else:
          command = fullCommand
          break
    return command
  
  def execute(self, script):
    commands = self.parser.parse(script)
    
    if not commands:
      self.fail("I can't decipher that.")
    
    for command, commandArgs in commands:
      command = self.completeCommand(command)
      
      if command not in self.commands:
        self.fail("Say what? Type 'help' to see the available commands.")
        
      # Separate the regular and keyword arguments for regular commands
      if command not in self.customSyntaxCommands:
        args   = []
        kwargs = {}
        for arg, value in commandArgs:
          if arg is None:
            args.append(value)
          else:
            kwargs[arg] = value
  
        # Complete the keyword arguments
        kwargs = completeKeywordArguments(self.commands[command], kwargs)
      else:
        # Custom syntax commands do their own parsing
        args   = [commandArgs]
        kwargs = {}

      if self.profiling and hotshot:
        prof = hotshot.Profile("tracy.prof")
      else:
        prof = None
        
      try:
        if prof:
          prof.runcall(self.commands[command], *args, **kwargs)
        else:
          self.commands[command](*args, **kwargs)
      except Exception, e:
        self.fail(e, showException = True)
        
      if prof:
        prof.close()
        stats = hotshot.stats.load("tracy.prof")
        stats.sort_stats("time", "calls")
        stats.print_stats(20)
  
  def run(self):
    while not self.done:
      try:
        self.colorizer.setColor(0xff, 0xff, 0xff)
        try:
          sys.stdout.write("%s> " % self.project.config.name)
        except AttributeError:
          sys.stdout.write("%s> " % "(no project)")
        self.colorizer.resetColor()
        command = raw_input()
      except KeyboardInterrupt:
        break
      except EOFError:
        break
      if not command:
        continue
      
      try:
        self.execute(command)
      except ExecutionError:
        pass
      except KeyboardInterrupt:
        print
        self.reportError("Interrupted.")

  def reload(self):
    task      = Task.startTask("reload", "Reloading plugins", len(sys.modules))
    for plugin in self.plugins:
      reload(sys.modules[plugin.__module__])
