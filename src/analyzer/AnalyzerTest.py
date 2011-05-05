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

import unittest
import Options
import Analyzer
from common import Task
import Common
import re
import os
from common import Project
from common import Log

Log.quiet = True

coreScript = [
  ("load data/simplecube.bin",  ["Loaded trace from 'data/simplecube.bin' as t0"]),
  ("info",                      ["Trace t0:", "Events: [0-9]+", "Frames: 0", "Render calls: 0", "Array data: .+", "Duration: [0-9.]+ s", "Filename: .+"]),
  ("help",                      []),
  ("help help",                 ["Show available commands or help on a specific command.", ""]),
  ("list t0 0:1",               ["eglGetDisplay.+(display=<.+>)"]),
  ("list t0 -2:+1",             ["eglDestroyContext.+(dpy=<.+>, ctx=<.+>)"]),
  ("extract t0 16:+1 t1",       ["1 events extracted from trace t0 to trace t1"]),
  ("list t1",                   ["glVertexPointer"]),
  ("extract t0 32:+1 t2",       ["1 events extracted from trace t0 to trace t2"]),
  ("join t1 t2 t3",             ["Traces t1 and t2 joined to trace t3"]),
  ("list t3",                   ["glVertexPointer", "glTranslatex"]),
  ("save t3 coretest.bin",      ["Saved trace t3 to 'coretest.bin'"]),
  ("export t3 coretest.txt text", ["Exported trace t3 to 'coretest.txt'"]),
]

glScript = [
  ("load data/simplecube.bin",   ["Loaded trace from 'data/simplecube.bin' as t0"]),
  ("info",                       ["Trace t0:", "Events: [0-9]+", "Frames: 149", "Render calls: 149", "Array data: .+", "Duration: [0-9.]+ s", "Filename: .+"]),
  ("show-state t0 #10",           "vertex_pointer = \[255, 1, 1, 1, 1, 1, 1, 255, 1, 255, 255, 1, 255, 1, 255, 1, 1, 255, 1, 255, 255, 255, 255, 255\]"),
  ("extract-state t0 #10:+1 t1", ["27 events extracted from trace t0 to trace t1"]),
]

class AnalyzerTest(unittest.TestCase):
  def logInfo(self, msg):
    for m in msg.strip().split("\n"):
      self.messages.append(m)

  def createAnalyzer(self, args = []):
    # Parse the command line options
    options, args = Options.parseCommandLine(args)
  
    # Read the project file
    project = Project.Project(options = options, fileName = options.project)

    # Create the interactive analyzer  
    analyzer = Analyzer.InteractiveAnalyzer(project, options)
    
    # Divert the output
    analyzer.reportInfo = self.logInfo
    Task.setMonitor(None)
    
    return analyzer

  def verifyScript(self, analyzer, script):
    for command, expectedMessages in script:
      self.messages = []
      analyzer.execute(command)
      
      # Collapse the messages if we're interested in all of the lines
      if type(expectedMessages) != type([]):
        expectedMessages = [expectedMessages]
        self.messages    = [" ".join(self.messages)]
      
      if len(self.messages) < len(expectedMessages):
        raise RuntimeError("%s: Expected %d messages, got only %d: %s" % (command, len(expectedMessages), len(self.messages), self.messages))
        
      for msg, expected in zip(self.messages[-len(expectedMessages):], expectedMessages):
        if not re.search(expected, msg):
          print self.messages
          raise RuntimeError("%s: Expected: '%s', got: '%s'" % (command, expected, msg))
  
  def testCoreOperations(self):
    analyzer = self.createAnalyzer()
    self.verifyScript(analyzer, coreScript)
    os.unlink("coretest.bin")
    os.unlink("coretest.txt")

  def testGlOperations(self):
    analyzer = self.createAnalyzer(["-c", "../../lib/tracy/gles11-egl11-symbian.tcy"])
    self.verifyScript(analyzer, glScript)
    
if __name__ == "__main__":
  unittest.main()
