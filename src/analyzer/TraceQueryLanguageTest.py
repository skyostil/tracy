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
import TraceQueryLanguage
import BinaryCodec
import StringIO

testTrace1 = "data/note.bin"
testTrace2 = "data/simplecube.bin"

class TraceQueryLanguageTest(unittest.TestCase):
  def __init__(self, *args, **kwargs):
    unittest.TestCase.__init__(self, *args, **kwargs)
    self.trace  = self.loadTrace(testTrace1)
    self.trace2 = self.loadTrace(testTrace2)
    self.env    = TraceQueryLanguage.Environment({
      "t0": self.trace,
      "t1": self.trace2
    })
  
  def loadTrace(self, traceFile):
    trace  = Trace.Trace()
    reader = BinaryCodec.Reader(trace, open(traceFile, "rb"))
    reader.load()
    return trace
            
  def testBasicQuery(self):
    result = TraceQueryLanguage.query(self.env, "select event.seq, event.name from t0")
    self.assertEqual(len(result.rows), len(self.trace.events))
    self.assertEqual(len(result.columns), 2)

  def testBasicQuery2(self):
    result = TraceQueryLanguage.query(self.env, "select * from t0")
    self.assertEqual(len(result.rows), len(self.trace.events))

  def testWhereClause(self):
    result = TraceQueryLanguage.query(self.env, "select event.seq, event.name from t0 where event.seq > 1000")
    self.assertEqual(len(result.rows), 91)

  def testWhereClause2(self):
    result = TraceQueryLanguage.query(self.env, "select event.seq, event.name from t0 where event.name = 'vgDrawPath'")
    self.assertEqual(len(result.rows), 3)

  def testSelectInto(self):
    result = TraceQueryLanguage.query(self.env, "select * into new_trace from t0 where event.name = 'vgDrawPath'")
    assert "new_trace" in self.env.traces
    self.assertEqual(len(self.env.traces["new_trace"].events), 3)

  def testSelectMultipleTables(self):
    result = TraceQueryLanguage.query(self.env, "select distinct event.seq, event.name from t0, t1 where t0.event.duration > 1000")
    self.assertEqual(len(result.rows), 3)

  def testOrderBy(self):
    result = TraceQueryLanguage.query(self.env, "select event.seq, event.name from t0 order by event.name desc, event.seq")
    self.assertEqual(len(result.rows), len(self.trace.events))
    self.assertEqual(result.rows[0][0], 41998)
  
if __name__ == "__main__":
  unittest.main()
