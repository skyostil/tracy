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
import Common
from common import Collections

class CollectionsTest(unittest.TestCase):
  def testDefaultDict(self):
    d = Collections.DefaultDict(lambda: 1)
    assert d["foo"] == 1
    d["foo"] = 2
    assert d["foo"] == 2

  def testDictProxy(self):
    d1 = {"hip": "hei"}
    d2 = Collections.DictProxy(d1)
    assert d2.hip == "hei"

  def testInverseDict(self):
    d1 = {"hip": "hei"}
    d2 = Collections.inverseDict(d1)
    assert d2["hei"] == "hip"
    
if __name__ == "__main__":
  unittest.main()
