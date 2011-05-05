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
import Common
from common import Task
import TraceOperations
import Instrumentation
import Player
import os

class PlayerPlugin(Plugin.InteractivePlugin):
  def __init__(self, analyzer):
    self.analyzer = analyzer
    self.analyzer.registerCommand("play",             self.playTrace)
    self.analyzer.registerCommand("load-inst",        self.loadInstrumentationData)
    self.analyzer.registerCommand("save-inst",        self.saveInstrumentationData)
    self.analyzer.registerCommand("calc-frame-stats", self.calculateFrameStatistics)

  def playTrace(self, traceName, eventRange = None, profile = False, saveFrames = False, checkCoherence = True,
                synchronize = False):
    """
    Play back a trace file using a generated trace player.
    
    @param traceName:       Trace to play.
    @param eventRange:      Range of events to play, or all events by default.
    @param profile:         Should instrumentation data be collected or not.
    @param saveFrames:      Should frame buffer snapshots be saved or not.
    @param checkCoherence:  Verify that the trace is properly normalized before playing.
    @param sychronize:      Attempt to match the original trace timings.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
    
    trace          = self.analyzer.traces[traceName]
    traceFileName  = self.analyzer.traceFiles.get(traceName, None)
    profile        = self.analyzer.parseBoolean(profile)
    saveFrames     = self.analyzer.parseBoolean(saveFrames)
    checkCoherence = self.analyzer.parseBoolean(checkCoherence)
    synchronize    = self.analyzer.parseBoolean(synchronize)
    
    # Create a temporary trace that only contains the needed events
    if eventRange is not None:
      firstEvent, lastEvent = self.analyzer.parseEventRange(trace, eventRange)
      traceFileName = None
      trace = TraceOperations.extract(trace, firstEvent, lastEvent)
      
    # Create a player
    player = Player.createPlayer(self.analyzer)

    # Play it
    self.analyzer.reportInfo("Playing trace.")
    logFile = player.play(trace, traceFileName, profile = profile, saveFrames = saveFrames, 
                          checkCoherence = checkCoherence, synchronize = synchronize)
    
    # Load the instrumentation data if it exists
    if profile and logFile:
      Instrumentation.loadInstrumentationData(self.analyzer, trace, logFile)
      self.analyzer.reportInfo("Instrumentation data loaded for trace %s." % traceName)

  def loadInstrumentationData(self, traceName):
    """
    Load previously computed instrumentation data for a trace file.
    
    @param traceName:   Trace for which data should be loaded.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    if not traceName in self.analyzer.traceFiles:
      self.analyzer.fail("The trace file for trace %s is not known." % traceName)
    
    trace         = self.analyzer.traces[traceName]
    traceFileName = self.analyzer.traceFiles[traceName]
    
    logFile = Instrumentation.instrumentationLogFileName(traceFileName)
    if not os.path.exists(logFile):
      self.analyzer.fail("Instrumentation log file not found (try the 'play' command): %s" % logFile)
      
    Instrumentation.loadInstrumentationData(self.analyzer, trace, logFile)
    self.analyzer.reportInfo("Instrumentation data loaded for trace %s." % traceName)

  def saveInstrumentationData(self, traceName):
    """
    Save the current instrumentation data for a trace file.
    
    @param traceName:   Trace for which data should be saved.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    if not traceName in self.analyzer.traceFiles:
      self.analyzer.fail("The trace file for trace %s is not known." % traceName)
    
    trace         = self.analyzer.traces[traceName]
    traceFileName = self.analyzer.traceFiles[traceName]
    
    logFile = Instrumentation.instrumentationLogFileName(traceFileName)
    Instrumentation.saveInstrumentationData(self.analyzer, trace, logFile)
    self.analyzer.reportInfo("Instrumentation data saved for trace %s." % traceName)

  def calculateFrameStatistics(self, traceName):
    """
    For each frame marker event, calculate the sum of the instrumentation data
    for the intermediate events. This operation will help to highlight frames
    that are especially resource intensive.
    
    @param traceName:   Trace for which frame data should be calculated.
    """
    if not traceName in self.analyzer.traces:
      self.analyzer.fail("Trace not found: %s" % traceName)
      
    trace =  self.analyzer.traces[traceName]
    events = TraceOperations.calculateFrameStatistics(self.analyzer.project, trace)
    self.analyzer.reportInfo("Data for %d frames collected."% len(events))
