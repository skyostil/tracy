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
import TraceOperations
import GlesTraceOperations
import GlesChecklist
import Checklist
import GlesReport
import StringUtils
import Trace
import os

class GlesPlugin(Plugin.InteractivePlugin):
  def __init__(self, analyzer):
    self.analyzer = analyzer
    self.analyzer.registerCommand("simplify",       self.simplifyTrace)
    self.analyzer.registerCommand("checklist",      self.runTraceChecklist)
    self.analyzer.registerCommand("report",         self.generateTraceReport)
    self.analyzer.registerCommand("calc-stats",     self.calculateTraceStatistics)

  def simplifyTrace(self, traceName, traceNameOut = None):
    """
    Remove redundant GLES commands from a trace.
    
    @param traceName:     Name of trace to simplify
    @param traceNameOut:  Result name, or the original trace name by default
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
    
    if not traceNameOut:
      traceNameOut = traceName
      
    trace    = self.analyzer.traces[traceName]
    newTrace = GlesTraceOperations.simplify(self.analyzer.project, trace)
    self.analyzer.traces[traceNameOut] = newTrace
    self.analyzer.reportInfo("%d events from trace %s simplified into %d events of trace %s." % (len(trace.events), traceName, len(newTrace.events), traceNameOut))

  def runTraceChecklist(self, traceName, fileName = None):
    """
    Run a GLES trace through a checklist of common performance issues.
    
    @param traceName:     Name of trace to check
    @param fileName:      File to which the checklist is saved
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
    
    trace    = self.analyzer.traces[traceName]

    if fileName:
      output = open(fileName, "w")
      print >>output, "GLES Checklist Report"
      print >>output, "====================="
      print >>output, ""
    else:
      output = None

    checklistItems = Checklist.compileChecklist(self.analyzer.project, trace, GlesChecklist.checklistItems)
    
    for item in checklistItems:
      if item.verdict:
        self.analyzer.reportInfo(str(item))
      else:
        self.analyzer.reportError(str(item))
      if output:
        title = "%s [%s]" % (item.name, item.verdict and "PASS" or "FAIL")
        print >>output, title
        print >>output, "-" * len(title)
        print >>output, ""
        print >>output, StringUtils.wordWrap(item.description)
        print >>output, ""
        
        # Print unique comments only
        comments       = sorted(item.comments, key = lambda c: c[1])
        lastComment    = None
        repeatedEvents = []
          
        for event, comment in comments + [(None, None)]:
          if comment != lastComment:
            if lastComment and repeatedEvents:
              if len(repeatedEvents) < 100:
                print >>output, StringUtils.wordWrap("* Last comment repeated for %s" % ", ".join(["%s (%d)" % (e.name, e.seq) for e in repeatedEvents]), indent = " " * 4, extraIndent = "  ")
              else:
                print >>output, StringUtils.wordWrap("* Last comment repeated for %d events." % len(repeatedEvents), indent = " " * 4, extraIndent = "  ")
            if comment:
              if event:
                print >>output, StringUtils.wordWrap("* %s (%d): %s" % (event.name, event.seq, comment), indent = " " * 4, extraIndent = "  ")
              else:
                print >>output, StringUtils.wordWrap("* %s" % (comment), indent = " " * 4, extraIndent = "  ")
              lastComment    = comment
              repeatedEvents = []
          elif event:
            repeatedEvents.append(event)
        print >>output, ""

  def generateTraceReport(self, traceName, path = None, format = "html"):
    """
    Generate a performance report of a trace.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace         = self.analyzer.traces[traceName]
    traceFileName = self.analyzer.traceFiles.get(traceName, None)
    
    if not path:
      path = traceFileName.split(".", 1)[0] + "_report"
    
    GlesReport.generateReport(self.analyzer.project, trace, traceFileName, path, format)
    self.analyzer.reportInfo("Performance report saved to '%s'." % (path))

  def generateTraceReport(self, traceName, path = None, format = "html"):
    """
    Generate a performance report of a trace.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace         = self.analyzer.traces[traceName]
    traceFileName = self.analyzer.traceFiles.get(traceName, None)
    
    if not path and traceFileName:
      path = traceFileName.split(".", 1)[0] + "_report"
    else:
      self.analyzer.fail("No output path specified.")
    
    GlesReport.generateReport(self.analyzer.project, trace, traceFileName, path, format)
    self.analyzer.reportInfo("Performance report saved to '%s'." % (path))

  def calculateTraceStatistics(self, traceName):
    """
    Calculate some derived OpenGL ES statistics based on instrumentation measurements.
    The following features are calculated from the trace:
    
    @param traceName:    Name of trace to process.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace = self.analyzer.traces[traceName]
    GlesTraceOperations.calculateStatistics(self.analyzer.project, trace)
    
    self.analyzer.reportInfo("Statistics calculated.")

  def postProcessInstrumentationData(self, trace):
    GlesTraceOperations.calculateStatistics(self.analyzer.project, trace)
