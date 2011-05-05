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

import BinaryCodec
import Plugin
import Trace
import binascii
import StringIO
import Task
import re
import Log

class StiImporterPlugin(Plugin.ImporterPlugin):
  """
  STI trace file reader.
  """
  formatName = "sti"

  def recognizeTraceFile(self, traceFileName):
    for line in open(traceFileName, "rb"):
      if "[TRACY:" in line:
        return True
    return False

  def loadTrace(self, traceFile):
    # Determine the trace file size
    try:
      traceFile.seek(0, 2)
      fileSize = traceFile.tell()
      traceFile.seek(0)
    except:
      fileSize = 0

    task = Task.startTask("load", "Decoding STI data", steps = fileSize)
    #s = StringIO.StringIO()
    r = re.compile(r"\[TRACY:(.*?)\] ([0-9a-f]+)")
    l = 1
    traces = {}
    #blockSize = None
    for line in traceFile:
      if "[TRACY:" in line:
        for match in r.finditer(line):
            #match = r.search(line)
            if not match:
                raise RuntimeError("Badly formatted line %d: %s" % l, line)
            proc = match.group(1)
            if not proc in traces:
                Log.notice("Found trace %s" % proc)
                traces[proc] = StringIO.StringIO() 
            data = match.group(2)
            #if not blockSize:
            #    blockSize = len(data)
            #elif len(data) != blockSize:
            #    print "Line %d, new block size: %d, %s, %s" % (l, blockSize, line, data)
            #    blockSize = len(data)
            traces[proc].write(binascii.unhexlify(data))
      if fileSize:
        task.updateProgress(traceFile.tell())
      elif (l & 0xff) == 0:
        task.step()
      l += 1

    t = []
    for proc, s in traces.items():
      Log.notice("Decoding trace %s" % proc)
      s.seek(0)
      trace      = Trace.Trace()
      reader     = BinaryCodec.Reader(trace, s)
      try:
        reader.load()
      except Exception, e:
        Log.error("Trace %s decoding failed: %s" % (proc, e))
      t.append(trace)
    return t
