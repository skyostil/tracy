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
import Trace
import BinaryCodec
import StringIO

testTrace1 = "data/note.bin"
testTrace2 = "data/simplecube.bin"

class ToolsTest(unittest.TestCase):
  def testLoading(self):
    for traceFile in [testTrace1, testTrace2]:
      trace  = Trace.Trace()
      reader = BinaryCodec.Reader(trace, open(traceFile, "rb"))
      reader.load()
      
      for event in trace.events:
        for key, value in event.values.items():
          assert not isinstance(value, Trace.UnknownPhrase)
          
  def testSaving(self):
    trace = Trace.Trace()
    output = StringIO.StringIO()
    writer = BinaryCodec.Writer(trace, output)
    
    cls = Trace.Class("someClass")
    obj = Trace.Object(0xbadf00d, 0xdef, cls)
    obj.attrs["int"]       = Trace.IntegerValue(1234)
    obj.attrs["bytearray"] = Trace.ByteArrayValue(1001, [1,2,3,4])
    
    e1 = Trace.Event()
    e1.name     = "event1"
    e1.seq      = 0
    e1.time     = 0
    e1.apiId    = 0
    e1.duration = 0
    e1.values["void"] = Trace.VoidValue()
    e1.values["byte"] = Trace.ByteValue(1)
    e1.values["short"] = Trace.ShortValue(2)
    e1.values["int"] = Trace.IntegerValue(3)
    e1.values["long"] = Trace.LongValue(4)
    e1.values["float"] = Trace.FloatValue(5)
    e1.values["double"] = Trace.DoubleValue(6)
    e1.values["bytearray"] = Trace.ByteArrayValue(1002, [7,8])
    e1.values["shortarray"] = Trace.ShortArrayValue(1003, [9,10])
    e1.values["intarray"] = Trace.IntegerArrayValue(1004, [11,12])
    e1.values["longarray"] = Trace.LongArrayValue(1005, [13,14])
    e1.values["floatarray"] = Trace.FloatArrayValue(1006, [15,16])
    e1.values["doublearray"] = Trace.DoubleArrayValue(1007, [17,18])
    e1.values["object"] = obj
    e1.values[None] = Trace.IntegerValue(123)
    e2 = Trace.Event()
    e2.name     = "event2"
    e2.seq      = 1
    e2.time     = 2
    e2.apiId    = 3
    e2.duration = 4
    
    trace.events = [e1, e2]
    writer.save()
    
    output.seek(0)
    trace2 = Trace.Trace()
    reader = BinaryCodec.Reader(trace2, output)
    reader.load()
    
    f1, f2 = trace2.events
    
    self.assertEquals(e1.name,     f1.name)
    self.assertEquals(e1.seq,      f1.seq)
    self.assertEquals(e1.time,     f1.time)
    self.assertEquals(e1.apiId,    f1.apiId)
    self.assertEquals(e1.duration, f1.duration)
    self.assertEquals(len(e1.values),   len(f1.values))
    
    for v1, v2 in zip(e1.values, f1.values):
      self.assertEquals(v1, v2)

    for v1, v2 in zip(e1.values.values(), f1.values.values()):
      if isinstance(v1, Trace.VoidValue):
        continue
      elif isinstance(v1, Trace.Object):
        for a1, a2 in zip(v1.attrs.values(), v2.attrs.values()):
          self.assertEquals(a1, a2)
      else:
        self.assertEquals(v1, v2)

    self.assertEquals(e2.name,     f2.name)
    self.assertEquals(e2.seq,      f2.seq)
    self.assertEquals(e2.time,     f2.time)
    self.assertEquals(e2.apiId,    f2.apiId)
    self.assertEquals(e2.duration, f2.duration)
    
if __name__ == "__main__":
  unittest.main()
