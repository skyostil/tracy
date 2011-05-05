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
import TraceOperations
import BinaryCodec
import StringIO

testTrace1 = "data/note.bin"
testTrace2 = "data/simplecube.bin"

class TraceOperationsTest(unittest.TestCase):
  def loadTrace(self, traceFile):
    trace  = Trace.Trace()
    reader = BinaryCodec.Reader(trace, open(traceFile, "rb"))
    reader.load()
    return trace
    
  def assertEqualEventLists(self, events1, events2):
    for e1, e2 in zip(events1, events2):
      self.assertEquals(e1.name, e2.name)
      self.assertEquals(len(e1.values), len(e2.values))
      
      for v1, v2 in zip(e1.values.values(), e2.values.values()):
        if isinstance(v1, Trace.VoidValue):
          continue
        elif isinstance(v1, Trace.Object):
          for a1, a2 in zip(v1.attrs.values(), v2.attrs.values()):
            self.assertEquals(a1, a2)
        else:
          self.assertEquals(v1, v2)
            
  def testExtraction(self):
    for traceFile in [testTrace1, testTrace2]:
      trace = self.loadTrace(traceFile)
      
      newTrace = TraceOperations.extract(trace, 10, 20)
      
      assert len(newTrace.events) == 10
      assert newTrace.events[0].time == 0
      assert newTrace.events[0].seq  == 0
      assert newTrace.events[9].seq  == 9
      
      self.assertEqualEventLists(newTrace.events, trace.events[10:20])
      
      newTrace.events[0].name = "foo"
      assert trace.events[0].name != "foo"
 
  def testJoining(self):
    for traceFile in [testTrace1, testTrace2]:
      trace = self.loadTrace(traceFile)
       
      l = len(trace.events)
      a = TraceOperations.extract(trace, 0, l / 2)
      b = TraceOperations.extract(trace, l / 2, l)
       
      assert len(a.events) + len(b.events) == l
       
      newTrace = TraceOperations.join(a, b)
      self.assertEqualEventLists(newTrace.events, trace.events)
       
      for i, event in enumerate(newTrace.events):
        self.assertEqual(event.seq, i)
  
if __name__ == "__main__":
  unittest.main()
