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

from AnalyzerEnvironment import *
import struct
import Graphics

for traceName, trace in traces.items():
  analyzer.reportInfo("Processing %s" % traceName)
  
  for event in trace.events:
    if event.name != "vgReadPixels": continue
    if len(event.values) != 7: continue
    fn = "frame%05d-%dx%d.png" % (event.seq, event.values["width"], event.values["height"])
    d = event.values["data"]
    d = struct.pack("%dB" % len(d), *d)
    d = Graphics.decodeImageData(d, (event.values["width"], event.values["height"]), event.values["dataStride"], 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000)
    #open(fn, "wb").write(d)
    d.save(fn)
    print event.seq
