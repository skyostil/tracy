#!/bin/sh python
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

"""Main executable."""

import os
import sys
import copy
import Common
from common import Log
from common import Config
from common import Resource
from common import Library
from common import Project
from common import OrderedDict
from common import Task
from common import Console
import Parser
import Generator
import Platform
import Tools
import Options
import Target

# Try to enable runtime function specialization
try:
  import psyco
  psyco.full()
except:
  #Log.warn("Unable to enable psyco; expect somewhat poor performance.")
  pass

def fail(reason):
  Log.error(reason)
  sys.exit(1)

def heading(text):
  s = "[ %s ]" % text
  w = 78 - 6
  b = "-" * ((w / 2) - len(s) / 2)
  return b + s + b

class TracyTaskMonitor(Task.TaskMonitor):
  def __init__(self):
    Task.TaskMonitor.__init__(self)

  def taskStarted(self, task):
    pass

  def taskProgress(self, task):
    Console.printProgressBar(task, Console.colorizer)

  def taskFinished(self, task):
    Console.eraseProgressBar()
  
def create(project, options, args):
  """Create a new Tracy project. [config file] [targets]"""
  
  if not args or len(args) < 1:
    fail("Configuration file name missing.")

  configName  = args[0]
  targetNames = args[1:]

  # Load the configuration file
  project.config.loadFile(configName)
  for path in project.config.paths:
    Resource.paths.append(path)
  config = project.config

  try:
    # Create the platform and read it's configuration if it exists.
    platform = Platform.platforms[options.platform](config)
  except KeyError:
    fail("No such platform. Use one of %s" % ", ".join(Platform.platforms.keys()))

  projectName = config.name.lower()

  Log.notice(heading("Creating Tracy project '%s' for %s" % (projectName, platform.name)))
  
  # Initialize the project
  lib = Library.Library(name = config.name)
  project.library  = lib
  project.platform = platform
  
  def prepareTarget(targetName):
    if targetName in project.targets:
      return
      
    if not targetName in Target.targets:
      fail("Bad target name '%s'." % targetName)
      return
      
    targetClass = Target.targets[targetName]
    for dep in targetClass.dependencies:
      if not dep in project.targets:
        prepareTarget(dep)

    Log.notice("Preparing target %d of %d: %s." % (len(project.targets) + 1, len(targetNames), targetName))
    target = targetClass(project)
    target.prepare()
    target.validate()
    project.targets[targetName] = target
  
  # Prepare the targets
  if not targetNames:
    targetNames = Target.targets.keys()
    
  for targetName in targetNames:
    prepareTarget(targetName)

  # Save the project
  projectPath = options.output and options.output or ("%s.tcy" % projectName)
  project.save(projectPath)
  
  Log.notice("Tracy project '%s' saved to '%s'." % (projectName, projectPath))

def generate(project, options, args):
  """Generate a target from a project. [target]"""
  if not args:
    fail("Generation target missing. Use one of %s" % ", ".join(project.targets.keys()))
    
  if not project.targets:
    fail("Project file name missing.")

  config = project.config

  for targetName in args:
    if not targetName in project.targets:
      fail("No such target. Use one of %s" % ", ".join(project.targets.keys()))
      
    # Generate the target
    target      = project.targets[targetName]
    projectName = config.name.lower() + "-" + targetName
    targetPath  = options.output and options.output or projectName

    Log.notice(heading("Generating '%s' from project '%s'" % (targetName, config.name)))
    
    target.generate(projectName, targetPath)
    Log.notice("Target '%s' created at '%s'." % (projectName, targetPath))

def sync(options, target, args):
  """Synchronize the makefile"""
  f = open("Makefile", "w")
  Generator.generateMakefile(f)
  f.close()
  Log.notice("Makefile synchronized")

def show(project, options, args):
  """Show information about a project target. [target]"""
  commands = [
    "all",
    "config",
    "functions",
    "constants",
    "typedefs"
  ]
  
  if not args:
    fail("Show what? Use one of %s" % ", ".join(commands))
  
  command, args = args[0], args[1:]
    
  if not command in commands:
    fail("Show what? Use one of %s" % ", ".join(commands))
  
  if not project.targets:
    fail("Project file name missing.")

  config = project.config
  lib    = project.library
  
  if "code" in project.targets:
    lib = project.targets["code"].library
  
  if args:
    config = project.targets[args[0]].config
    lib    = project.targets[args[0]].library
    
  if command == "config" or command == "all":
    Log.notice("Configuration:")
    print config.save()
  if command == "functions" or command == "all":
    Log.notice("%d functions:" % len(lib.functions))
    for name, function in lib.functions.items():
      params = []
      for paramName, param in sorted(function.parameters.items()):
        params.append("%s %s" % (param.type, param.name))
      params = ", ".join(params)
      print "%s %s(%s)" % (function.type, function.name, params)
  if command == "constants" or command == "all":
    Log.notice("%d constants:" % len(lib.constants))
    for constName, constValue in sorted(lib.constants.items()):
      print "%-40s %-8s (0x%x)" % (constName, constValue, constValue)
  if command == "typedefs" or command == "all":
    Log.notice("%d typedefs:" % len(lib.typeDefs))
    for tdName, tdValue in sorted(lib.typeDefs.items()):
      print "%-40s %s" % (tdName, tdValue)

def getCommands():
  commands = [create, generate, show, sync]
  return dict([(cmd.__name__, (cmd, cmd.__doc__)) for cmd in commands])

def printCommands():
  print "Available commands:"
  for cmd, desc in getCommands().items():
    print "  %-16s %s" % (cmd, desc[1])

def main(args):
  # Parse the command line options
  options, args = Options.parseCommandLine(args)
  
  if options.help_commands:
    printCommands()
    return 0

  if not args:
    fail("Command missing. Use one of %s" % ", ".join(getCommands().keys()))
    
  command  = args[0]
  commands = getCommands()
  
  if options.verbose:
    Log.quiet = False
  else:
    Log.debug = lambda msg: None

  if options.quiet:
    Log.quiet = True
  
  if not command in commands:
    fail("Bad command. Use one of %s" % ", ".join(getCommands().keys()))
    
  # Install a task monitor
  taskMon = Task.RateLimitedTaskMonitor(TracyTaskMonitor())
  Task.setMonitor(taskMon)

  # Read the project file
  project = Project.Project(options = options, fileName = options.project)

  getCommands()[command][0](project, options, args[1:])
  return 0
  
if __name__ == "__main__":
  sys.exit(main(sys.argv[1:]))
