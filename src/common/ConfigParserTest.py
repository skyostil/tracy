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
from common import ConfigParser

configFile = """
key1
key2: value2
key3(attr3=attrvalue3)
key4(attr4=attrvalue4): value4
key5(attr5="attribute value 5"): "value with spaces"

group:   /* test config file */
{
  foo
  subgroup(something = other):
  {
    sub1: what
    sub2
    sub3
  }
}

stringdict: { "foo"(key=value): "joo"  "bar": "baz" }

/*list: [1, 2, 3]*/
/*list2(spede = true): [1, 2, 3, "foo", "bar", baz]*/
"""

crlfFile = "crlf_key1: crlf_value1\r\ncrlf_key2: crlf_value2\r\n"

class ConfigParserTest(unittest.TestCase):
  def testConfigParsing(self):
    items = ConfigParser.parseConfig(configFile)
    self.assertEquals(items.key1, "")
    self.assertEquals(items.key2, "value2")
    self.assertEquals(items.key3, "")
    self.assertEquals(items.key3.attrs["attr3"], "attrvalue3")
    self.assertEquals(items.key4, "value4")
    self.assertEquals(items.key4.attrs["attr4"], "attrvalue4")
    self.assertEquals(items.key5, "value with spaces")
    self.assertEquals(items.key5.attrs["attr5"], "attribute value 5")
    self.assertEquals(items.group.subgroup.sub3, "")
    self.assertEquals(items.group.subgroup.attrs["something"], "other")
    #self.assertEquals(items.list, [1, 2, 3])
    #self.assertEquals(items.list2, [1, 2, 3, "foo", "bar", "baz"])
    
    assert "foo" in items.stringdict
    self.assertEquals(items.stringdict.foo.attrs["key"], "value")
    assert "bar" in items.stringdict

    # check bad name
    try:
      items.nonexistent
      self.fail("KeyError not raised.")
    except AttributeError:
      pass

  def testCRLF(self):
    items = ConfigParser.parseConfig(crlfFile)
    self.assertEquals(items.crlf_key1, "crlf_value1")
    self.assertEquals(items.crlf_key2, "crlf_value2")

if __name__ == "__main__":
  unittest.main()
