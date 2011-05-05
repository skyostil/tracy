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

"""Trace player."""

import os
import subprocess
import tempfile
import Instrumentation
import BinaryCodec
import Common
import glob
from common import Resource

def createPlayer(analyzer):
  # Find a suitable player
  if os.name == "nt":
    return TracePlayerWin32(analyzer)
  elif os.name == "posix":
    return TracePlayerUnix(analyzer)
  else:
    analyzer.fail("No trace playback support on platform '%s'." % os.name)

class TracePlayer(object):
  def _makeTracePlayable(self, trace, traceFileName, checkCoherence = True):
    """Makes sure that the given trace is playable.
       @returns a tuple (trace_file_name, is_temporary), where the is_temporary 
                flag indicates that the trace file is temporary and should be 
                deleted after playing.
    """
    # Find a suitable exporter
    try:
      exporter = [e for e in self.analyzer.exportPlugins if e.formatName == "binary"][0]
    except IndexError:
      self.analyzer.fail("No binary format exporter found.")
      
    # Check that the trace file is playable -- if not, we need to resave it
    if traceFileName and checkCoherence:
      reader = BinaryCodec.Reader(trace, open(traceFileName, "rb"))
      if not reader.isTracePlayable():
        self.analyzer.reportWarning("The trace file will be normalized before playing. Save and reload the trace to avoid this step.")
        traceFileName = None
      
    # If there is no physical trace file, save the trace to a temporary file
    if not traceFileName:
      fd, traceFileName = tempfile.mkstemp(suffix = "_trace.bin")
      os.close(fd)
      traceFile = open(traceFileName, "wb")
      exporter.saveTrace(trace, traceFile)
      traceFile.close()
      removeTraceFile = True
    else:
      removeTraceFile = False
    return (traceFileName, removeTraceFile)
	  
class TracePlayerWin32(TracePlayer):
  """Win32 trace player."""
  def __init__(self, analyzer):
    self.analyzer = analyzer
    self.library  = self.analyzer.project.targets["code"].library
    apiName       = analyzer.project.config.name
    self.binary   = os.path.join(Resource.getPath("bin"), apiName + "-win32-player.exe")
    
  def play(self, trace, traceFileName = None, profile = False, saveFrames = False, checkCoherence = True, synchronize = False):
    # Check that the player is there
    if not os.path.isfile(self.binary):
      self.analyzer.fail("Player not found: %s" % self.binary)
      
    traceFileName, removeTraceFile = self._makeTracePlayable(trace, traceFileName, checkCoherence)

    # Play it
    try:
      args = [self.binary, traceFileName]
      if saveFrames:
        args += ["--save-frames"]
      if profile or saveFrames:
        args += ["--profile"]
      if synchronize:
        args += ["--synchronize"]
      subprocess.call(args)
    finally:
      if removeTraceFile:
        os.unlink(traceFileName)
      
    # Return the instrumentation log if it exists
    instLog = Instrumentation.instrumentationLogFileName(traceFileName)
    if os.path.exists(instLog):
      return instLog

class TracePlayerUnix(TracePlayer):
  """Unix trace player."""
  def __init__(self, analyzer):
    self.analyzer = analyzer
    self.library  = self.analyzer.project.targets["code"].library
    apiName       = analyzer.project.config.name
    self.binary   = os.path.join(Resource.getPath("bin"), apiName + "-unix-player")
    self.args     = []
    
  def play(self, trace, traceFileName = None, profile = False, saveFrames = False, checkCoherence = True, synchronize = False):
    # Check that the player is there
    if not os.path.isfile(self.binary):
      self.analyzer.fail("Player not found: %s" % self.binary)
    
    traceFileName, removeTraceFile = self._makeTracePlayable(trace, traceFileName, checkCoherence)

    # Play it
    try:
      args = [self.binary] + self.args + [traceFileName]
      if saveFrames:
        args += ["--save-frames"]
      if profile or saveFrames:
        args += ["--profile"]
      if synchronize:
        args += ["--synchronize"]
      subprocess.call(args)
    finally:
      if removeTraceFile:
        os.unlink(traceFileName)
      
    # Return the instrumentation log if it exists
    instLog = Instrumentation.instrumentationLogFileName(traceFileName)
    if os.path.exists(instLog):
      return instLog
