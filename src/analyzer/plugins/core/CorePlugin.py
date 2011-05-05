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

import Plugin
import Trace
import TraceOperations
import StringUtils
import Analyzer
import inspect
import StringIO
import copy
import code
import sys
import os
import re
import Common
from common import Collections
from common import Task

class CoreInteractivePlugin(Plugin.InteractivePlugin):
  def __init__(self, analyzer):
    Plugin.InteractivePlugin.__init__(self, analyzer)
    self.analyzer.registerCommand("help",          self.help)
    self.analyzer.registerCommand("load",          self.loadTrace)
    self.analyzer.registerCommand("new",           self.newTrace)
    self.analyzer.registerCommand("save",          self.saveTrace)
    self.analyzer.registerCommand("export",        self.exportTrace, self.exportTraceHelp)
    self.analyzer.registerCommand("close",         self.closeTrace)
    self.analyzer.registerCommand("list",          self.listTrace)
    self.analyzer.registerCommand("info",          self.showTraceInfo)
    self.analyzer.registerCommand("extract",       self.extractTrace)
    self.analyzer.registerCommand("extract-state", self.extractTraceWithState)
    self.analyzer.registerCommand("show-state",    self.showTraceState)
    self.analyzer.registerCommand("join",          self.joinTraces)
    self.analyzer.registerCommand("merge",         self.mergeTraces)
    self.analyzer.registerCommand("profiling",     self.enableProfiling)
    self.analyzer.registerCommand("python",        self.interactivePythonMode)
    self.analyzer.registerCommand("renumber",      self.renumberTrace)
    self.analyzer.registerCommand("reload",        self.reloadAnalyzer)
    self.analyzer.registerCommand("show-plugins",  self.showAnalyzerPlugins)
    self.analyzer.registerCommand("call-histogram",self.showCallHistogram)
    self.analyzer.registerCommand("grep",          self.grepTrace)
    self.analyzer.registerCommand("set-egl-config", self.setTraceEglConfig)
    self.analyzer.registerCommand("verify",        self.verifyTrace)

  def help(self, command = None):
    """
    Show available commands or help on a specific command.
    """
    if command:
      command = self.analyzer.completeCommand(command)
    
    if command in self.analyzer.commands:
      args, _, _, defaults = inspect.getargspec(self.analyzer.commands[command])
      
      if defaults is None:
        defaults = []

      helpString = "Usage: %s " % command
      for i, arg in enumerate(args):
        if arg == "self":
          continue
        if i >= len(args) - len(defaults):
          helpString += "[%s = %s] " % (arg, defaults[i - (len(args) - len(defaults))])
        else:
          helpString += "<%s> " % arg
      
      self.reportInfo(helpString)
      
      if self.analyzer.commands[command].__doc__:
        for line in self.analyzer.commands[command].__doc__.split("\n"):
          line = line.strip()
          if line.startswith("@param"):
            _, arg, text = line.split(" ", 2)
            line = "%-16s %s" % (arg.strip(), text.strip())
          self.reportInfo(line)

      if self.analyzer.commandHelp.get(command):
        self.reportInfo(self.analyzer.commandHelp[command]())
          
      return
    
    self.reportInfo("Available commands:")
    for command, func in sorted(self.analyzer.commands.items()):
      doc = func.__doc__ or ""
      if doc and "." in doc:
        doc = doc.strip().split(".", 1)[0] + "."
      doc = re.sub(" +", " ", doc)
      doc = re.sub("\n", " ", doc)
      doc = StringUtils.wordWrap(doc, columns = 55)
      doc = doc.replace("\n", "\n" + " " * 21)
      self.reportInfo("%-19s- %s" % (command, doc))
    self.reportInfo("""
Tips:
  * You may use abbreviated names for commands, e.g. "h" for "help" or "s-s" for "show-state".
  * Multiple commands may be separated with a semicolon ';'. 
  * Whenever an event range is required, you may pass a single event (e.g. 123), a range of 
    events (10:20 or 10:+10), a single frame (#512) e.g. or a range of frames (#15:#20 or 
    #15:+#5).
  * Named arguments can also be given for commands, e.g. list traceName=t0.
""")

  def loadTrace(self, fileName, traceName = None, format = None):
    """
    Open a trace file.

    @param fileName:   Trace file to open
    @param traceName:  Resulting trace name
    @param format:     Force a specific format to be used instead of autodetection.
    """
    trace     = Trace.Trace()

    for importer in self.analyzer.importPlugins:
      if format is not None and importer.formatName == format:
        break
      if importer.recognizeTraceFile(fileName):
        break
    else:
      if format is not None:
        self.analyzer.fail("No such format. Available formats: %s." % (", ".join([i.formatName for i in self.analyzer.importPlugins])))
      self.analyzer.fail("Trace file format not recognized.")

    try:
      f = open(fileName, "rb")
      traces = importer.loadTrace(f)
      f.close()

      # Some decoders can load multiple trace files from one source
      if not isinstance(traces, list):
          traces = [traces]
     
      for trace in traces:
          traceName = traceName or "t%d" % len(self.analyzer.traces)
          self.analyzer.traces[traceName]     = trace
          self.analyzer.traceFiles[traceName] = fileName
          if not TraceOperations.verify(self, self.analyzer.project, trace):
            self.reportWarning("The loaded project probably does not match the trace file.")
          
          self.reportInfo("Loaded trace from '%s' as %s." % (fileName, traceName))
          traceName = None
      return traces[0]
    except IOError, e:
      self.analyzer.fail(e)

  def verifyTrace(self, traceName):
    """
    Verify that a trace is compatible with the loaded project

    @param traceName:  Trace name
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
    
    trace = self.analyzer.traces[traceName]
    TraceOperations.verify(self, self.analyzer.project, trace)

  def newTrace(self, traceName = None):
    """
    Create a new empty trace file.

    @param traceName:  Resulting trace name
    """
    trace     = Trace.Trace()
    traceName = traceName or "t%d" % len(self.analyzer.traces)
    self.analyzer.traces[traceName]     = trace
    self.reportInfo("Created new trace as %s." % (traceName))
    return trace
      
  def saveTrace(self, traceName, fileName = None):
    """
    Save a trace to a file.

    @param traceName:   Trace to save
    @param fileName:    Output trace file or the original file name by default
    """
    if not fileName:
      try:
        fileName = self.analyzer.traceFiles[traceName]
      except KeyError:
        self.analyzer.fail("Original file name for trace %s not found." % traceName)
    
    self.exportTrace(traceName, fileName, format = "binary")
    self.analyzer.traceFiles[traceName] = fileName

  def exportTrace(self, traceName, fileName, format = "binary", **kwargs):
    """
    Export a trace to a file in a special format.

    @param traceName:   Trace to save
    @param fileName:    Output trace file
    @param format:      Output trace format
    @param **kwargs:    Keyword arguments for the exporter plugin
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace = self.analyzer.traces[traceName]
    
    for exporter in self.analyzer.exportPlugins:
      if exporter.formatName == format:
        break
    else:
      self.analyzer.fail("No such format. Available formats: %s." % (", ".join([e.formatName for e in self.analyzer.exportPlugins])))

    kwargs = Analyzer.completeKeywordArguments(exporter.saveTrace, kwargs)

    try:
      f = open(fileName, "wb")
      exporter.saveTrace(trace, f, **kwargs)
      f.close()
      # If the file is in binary format, record the output file
      if format == "binary":
        self.analyzer.traceFiles[traceName] = fileName
        self.reportInfo("Saved trace %s to '%s'." % (traceName, fileName))
      else:
        self.reportInfo("Exported trace %s to '%s' in %s format." % (traceName, fileName, format))
    except IOError, e:
      self.analyzer.fail(e)

  def exportTraceHelp(self):
    helpText = ["Output formats and options:"]
    for exporter in self.analyzer.exportPlugins:
      helpText.append("  %10s: %s" % (exporter.formatName, ", ".join(Analyzer.getFunctionArguments(exporter.saveTrace)[3:])))
    return "\n".join(helpText)

  def closeTrace(self, traceName):
    """
    Unload a trace file.

    @param traceName:  Name of trace to close
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)

    try:
      del self.analyzer.traces[traceName]
      del self.analyzer.traceFiles[traceName]
    except KeyError:
      pass
    
    self.reportInfo("Trace %s unloaded." % (traceName))

  def listTrace(self, traceName, eventRange = None):
    """
    List events contained in a trace.
    
    @param traceName:   Trace to examine.
    @param eventRange:  Range of events to list.
    """
    
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
    
    trace = self.analyzer.traces[traceName]
    
    try:    
      exporter = [e for e in self.analyzer.exportPlugins if e.formatName == "text"][0]
    except IndexError:
      self.analyzer.fail("No text format exporter found.")
    
    firstEvent, lastEvent = self.analyzer.parseEventRange(trace, eventRange)

    # Make a shallow copy of the trace that has only the events we're interested in
    trace = copy.copy(trace)
    trace.events = trace.events[firstEvent:lastEvent]
    
    text = StringIO.StringIO()
    exporter.saveTrace(trace, text, truncateValues = True, includeSensorData = False)
    self.reportInfo(text.getvalue())

  def showTraceInfo(self, traceName = None):
    """
    Show information about a trace.
    
    @param traceName:   Trace to examine or all opened traces by default.
    """
    if traceName is not None:
      if not traceName in self.analyzer.traces:
        self.analyzer.fail("Trace not found: %s" % traceName)
      traces = {traceName: self.analyzer.traces[traceName]}
    else:
      traces = self.analyzer.traces
      
    arrayElementSizes = {
        Trace.ByteArrayValue:     1,
        Trace.ShortArrayValue:    2,
        Trace.IntegerArrayValue:  4,
        Trace.LongArrayValue:     8,
        Trace.FloatArrayValue:    4,
        Trace.DoubleArrayValue:   8,
        Trace.ObjectArrayValue:   4,
    }
        
    for name, trace in traces.items():
      self.reportInfo("Trace %s:" % name)
      frames      = 0
      renderCalls = 0
      arrayBytes  = 0
      for event in trace.events:
        for valueName, value in event.values.items():
          if isinstance(value, Trace.Array):
            arrayBytes += len(value) * arrayElementSizes[value.__class__]
        try:
          if self.analyzer.lookupFunction(event).isFrameMarker:
            frames += 1
          if self.analyzer.lookupFunction(event).isRenderCall:
            renderCalls += 1
        except AttributeError:
          pass
      self.reportInfo("          Events: %d" % len(trace.events))
      self.reportInfo("          Frames: %d" % frames)
      self.reportInfo("    Render calls: %d" % renderCalls)
      self.reportInfo("      Array data: %s" % StringUtils.normalizeSiValue(arrayBytes, "B", 1024.0))
      if len(trace.events):
        self.reportInfo("        Duration: %s" % StringUtils.normalizeTimeValue((trace.events[-1].time - trace.events[0].time) / 1e6))
      if name in self.analyzer.traceFiles:
        self.reportInfo("        Filename: %s" % self.analyzer.traceFiles[name])

  def extractTrace(self, traceName, eventRange, traceNameOut = None):
    """
    Extract a portion of a trace to form a new trace.
    
    @param traceName:     Name of the source trace
    @param eventRange:    Event range to extract
    @param traceNameOut:  Name under which the resulting trace is saved, or the original trace by default.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    if traceNameOut is None:
      traceNameOut = traceName

    trace = self.analyzer.traces[traceName]
    firstEvent, lastEvent = self.analyzer.parseEventRange(trace, eventRange)
    trace = TraceOperations.extract(trace, firstEvent, lastEvent)
    self.analyzer.traces[traceNameOut] = trace
    
    self.reportInfo("%d events extracted from trace %s to trace %s." % (len(trace.events), traceName, traceNameOut))
    return trace

  def showTraceState(self, traceName, eventNumber = None):
    """
    Print out the computed API state at a particular trace event.
    
    @param traceName:     Name of trace to examine
    @param eventNumber:   Print out state at this event, or at the end of the trace by default
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace = self.analyzer.traces[traceName]
      
    if eventNumber is None:
      eventNumber = len(trace.events) - 1
    else:
      eventNumber = self.analyzer.parseEventRange(trace, eventNumber)[0]
    state = TraceOperations.computeStateAtEvent(self.analyzer.project, trace, eventNumber)
    
    self.reportInfo(str(state))

  def extractTraceWithState(self, traceName, eventRange, traceNameOut = None):
    """
    Extract a portion of a trace including preceding state to form a new trace. The command collects events
    prior to the first extracted event from the trace to set up the same state in the new trace.
    
    @param traceName:     Name of the source trace
    @param eventRange:    Event range to extract
    @param traceNameOut:  Name under which the resulting trace is saved, or the original trace by default.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    if traceNameOut is None:
      traceNameOut = traceName
      
    trace = self.analyzer.traces[traceName]
    firstEvent, lastEvent = self.analyzer.parseEventRange(trace, eventRange)
    trace = TraceOperations.extractWithState(self.analyzer.project, trace, firstEvent, lastEvent)
      
    self.analyzer.traces[traceNameOut] = trace
    
    self.reportInfo("%d events extracted from trace %s to trace %s." % (len(trace.events), traceName, traceNameOut))
    return trace
        
  def joinTraces(self, traceName1, traceName2, traceNameOut = None):
    """
    Join two traces together to produce a new third trace.
    
    @param traceName1:    Name of the first trace
    @param traceName2:    Name of the second trace
    @param traceNameOut:  Name under which resulting trace is saved, or the first trace by default.
    """
    if not traceName1 in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName1)

    if not traceName2 in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName2)
  
    if traceNameOut is None:
      traceNameOut = traceName1
  
    trace = TraceOperations.join(self.analyzer.traces[traceName1], self.analyzer.traces[traceName2])
    self.analyzer.traces[traceNameOut] = trace
    
    self.reportInfo("Traces %s and %s joined to trace %s." % (traceName1, traceName2, traceNameOut))
    return trace

  def mergeTraces(self, traceName1, traceName2, traceNameOut = None, useTimeStamps = False):
    """
    Merge two traces together to produce a third, sequentially coherent trace.
    
    @param traceName1:    Name of the first trace
    @param traceName2:    Name of the second trace
    @param traceNameOut:  Name under which resulting trace is saved, or the first trace by default.
    """
    if not traceName1 in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName1)

    if not traceName2 in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName2)
  
    if traceNameOut is None:
      traceNameOut = traceName1
    useTimeStamps = self.analyzer.parseBoolean(useTimeStamps)
  
    trace = TraceOperations.merge(self.analyzer.traces[traceName1], self.analyzer.traces[traceName2], useTimeStamps = useTimeStamps)
    self.analyzer.traces[traceNameOut] = trace
    
    self.reportInfo("Traces %s and %s merged to trace %s." % (traceName1, traceName2, traceNameOut))
    return trace

  def enableProfiling(self, state = "on"):
    """
    Enable or disable call profiling for all executed commands.
    
    @param state:     Profiling state (on or off)
    """
    self.analyzer.profiling = self.analyzer.parseBoolean(state)
    
    if self.analyzer.profiling:
      self.reportInfo("Profiling enabled.")
    else:
      self.reportInfo("Profiling disabled.")

  def interactivePythonMode(self, fileName = None, *args, **kwargs):
    """
    Run an interactive Python interpreter that can be used to manipulate the loaded traces.
    
    @param fileName:    Optional Python source file to run
    @param args:        Optional arguments for the source file
    @param kwargs:      Optional keyword arguments for the source file
    """
    
    # Refresh the analyzer environment module
    import AnalyzerEnvironment as env
    
    env.analyzer  = self.analyzer
    env.project   = self.analyzer.project
    env.args      = args
    env.kwargs    = kwargs
    env.cmd       = Collections.DictProxy(self.analyzer.commands)
    env.traces    = Collections.DictProxy(self.analyzer.traces)

    try:
      env.library   = self.analyzer.project.targets["code"].library
      env.constants = Collections.DictProxy(env.library.constants)
    except KeyError:
      env.library   = None
      env.constants = None

    try:
      if fileName:
        path       = os.path.dirname(fileName)
        moduleName = os.path.splitext(os.path.basename(fileName))[0]
        if not path in sys.path:
          sys.path.append(path)
        if not moduleName in sys.modules:
          __import__(moduleName)
        else:
          reload(sys.modules[moduleName])
      else:
        console = code.InteractiveConsole(locals = env.__dict__)
        console.interact(banner = "Entering Python mode.")
    except SystemExit:
      pass

  def renumberTrace(self, traceName):
    """
    Renumber a trace so that the events contained in it start at zero.
    
    @param traceName:   Name of the trace to renumber
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)

    trace = self.analyzer.traces[traceName]
    
    for i, event in enumerate(trace.events):
      event.seq = i
    
    self.reportInfo("%d events renumbered." % (len(trace.events)))

  def reloadAnalyzer(self):
    """
    Reload the analyzer modules while keeping all loaded data intact.
    """
    self.analyzer.reload()

  def showAnalyzerPlugins(self):
    """
    List the loaded analyzer plugin modules.
    """
    def show(plugin):
      self.reportInfo("  %-30s (from %s)" % (plugin.__class__.__name__, plugin.__module__))
    
    self.reportInfo("Command plugins:")
    for plugin in self.analyzer.plugins:
      show(plugin)
    self.reportInfo("Import plugins:")
    for plugin in self.analyzer.importPlugins:
      show(plugin)
    self.reportInfo("Export plugins:")
    for plugin in self.analyzer.exportPlugins:
      show(plugin)

  def showCallHistogram(self, traceName):
    """
    Show the function call frequency histogram for a trace.
    
    @param traceName:   Name of the trace to examine
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)

    trace = self.analyzer.traces[traceName]
    hist  = TraceOperations.calculateCallHistogram(trace)

    width   = 40
    largest = max(hist.values())
    values  = [(count, name) for name, count in hist.items()]
    
    for i, value in enumerate(sorted(values, reverse = True)):
      count, name = value
      self.reportInfo("%3d. %-25s %5d %s" % (i + 1, name, count, "#" * int(width * float(count) / largest)))

  def setTraceEglConfig(self, traceName, configId, eventRange = None):
    """
    Force the usage of a particular EGL config for a GLES trace. This command
    overrides the automatic EGL configuration selection and makes the trace
    player use a specific EGL configuration.
    
    @param traceName:    Name of trace to modify.
    @param configId:     ID of new EGL configuration. Pass 0 to use the original config.
    @param eventRange:   Range of events to modify, or the whole trace by default.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace                 = self.analyzer.traces[traceName]
    firstEvent, lastEvent = self.analyzer.parseEventRange(trace, eventRange)
    
    configId = int(configId)

    for event in trace.events[firstEvent:lastEvent]:
      for value in event.values.values():
        if isinstance(value, Trace.Object) and value.cls.name == "EGLConfig":
          # Found an EGL config -> replace the id
          if not configId:
            value.attrs["config_id"] = value.id
          else:
            value.attrs["config_id"] = -configId
          self.analyzer.reportInfo("EGL configuration ID of event %s(%d) set to %d." % (event.name, event.seq, abs(value.attrs["config_id"])))

  def grepTrace(self, traceName, expression, eventRange = None, fullText = True, context = 1, outputTrace = None):
    """
    Search for events matching a regular expression.
    
    @param traceName:     Trace to examine.
    @param expression:    Regular expression to search for.
    @param eventRange:    Event range to examine.
    @param fullText:      Search full trace text instead of only event names.
    @param context:       Number of events to list around matches.
    @param outputTrace:   Name of trace to save matched events to.
    """

    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
    
    trace = self.analyzer.traces[traceName]

    try:
      exporter = [e for e in self.analyzer.exportPlugins if e.formatName == "text"][0]
    except IndexError:
      self.analyzer.fail("No text format exporter found.")

    if outputTrace:
      newTrace = copy.copy(trace)
      newTrace.events = []
      self.analyzer.traces[outputTrace] = newTrace
    else:
      newTrace = None

    tempTrace = copy.copy(trace)
    def eventToText(event):
      out              = StringIO.StringIO()
      tempTrace.events = [event]
      exporter.saveTrace(tempTrace, out, truncateValues = True, includeSensorData = False)
      return out.getvalue()

    firstEvent, lastEvent = self.analyzer.parseEventRange(trace, eventRange)
    fullText   = self.analyzer.parseBoolean(fullText)
    expression = re.compile(expression)
    task       = Task.startTask("grep", "Searching", len(trace.events[firstEvent:lastEvent]))

    for event in trace.events[firstEvent:lastEvent]:
      task.step()
      if fullText:
        text = eventToText(event)
      else:
        text = event.name

      if expression.search(text):
        if newTrace:
          newTrace.events.append(event)
        else:
          if not fullText:
            text = eventToText(event)
          self.reportInfo(text)
