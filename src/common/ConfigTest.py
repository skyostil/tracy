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
from StringIO import StringIO
from TestUtil import TemporaryFile
import os

import Common
from common import Config

conf1 = """
item1
common: foo
group:
{
  subitem1
}
list:
[
  listitem1
]
"""

conf2 = """
item2
common: bar
group:
{
  subitem2
}
list:
[
  listitem2
]
"""

mergeConf1 = """
item: value1
group:
{
  item1
  subgroup:
  {
    subitem1
  }
}
"""

mergeConf2 = """
item: value2
group:
{
  item2
  subgroup:
  {
    subitem1
    subitem2
  }
}
group2:
{
  test
}
"""

collectconf = """
group1:
{
  foo1
  bar1
  baz1
  subgroup:
  {
    foo2
    bar2
    baz2
  }
}

"""

listConf = """
list:
[
    item1
    item2
    {
        foo: bar
    }
    [
        sublist1
        sublist2
    ]
    item5(attr=value)
]
"""

groupConf = """
group:
{
    list1:
    [
        sublist1
        sublist2
    ]
    list2:
    [
        sublist1
        sublist2
    ]
}
"""

emptyGroupAndList = """
emptygroup:
{
}

emptylist:
[
]
"""

class ConfigTest(unittest.TestCase):
  def testShadowing(self):
    c = Config.Config()
    c.load(conf1)
    c.load(conf2)
    
    assert "item1" in c
    assert "item2" in c
    assert "subitem1" in c.group
    assert "subitem2" in c.group
    assert c.list[0] == "listitem1"
    assert c.list[1] == "listitem2"
    assert "group.subitem1" in c
    self.assertEquals(c.common, "bar")
    
  def testRelativePath(self):
    f = TemporaryFile(conf1)
    c = Config.Config(f.name)
    assert os.path.dirname(f.name) == os.path.dirname(c.getRelativePath("foo.txt"))
    del f
    
  def testMerging(self):
    c1 = Config.Config()
    c2 = Config.Config()
    c1.load(mergeConf1)
    c2.load(mergeConf2)
    
    assert c1.item == "value1"
    c1.merge(c2)
    assert c1.item == "value2"
    assert "item1" in c1.group
    assert "item2" in c1.group
    assert "subitem1" in c1.group.subgroup
    assert "subitem2" in c1.group.subgroup
    assert "group2" in c1

  def testLists(self):
    c = Config.Config()
    c.load(listConf)
    
    assert "list" in c
    assert len(c.list) == 5
    assert c.list[0] == "item1"
    assert c.list[1] == "item2"
    assert c.list[2] == {"foo": "bar"}
    assert c.list[3] == ["sublist1", "sublist2"]
    assert c.list[4].attrs["attr"] == "value"

  def testGroups(self):
    c = Config.Config()
    c.load(groupConf)
    
    assert "group" in c
    assert len(c.group) == 2
    assert c.group["list1"] == ["sublist1", "sublist2"]
    assert c.group["list2"] == ["sublist1", "sublist2"]

  def testEmptyGroupAndList(self):
    c = Config.Config()
    c.load(emptyGroupAndList)
    
    assert "emptygroup" in c
    assert "emptylist" in c
    assert len(c.emptylist) == 0
    assert len(c.emptygroup) == 0
    
  def testDuplicateItems(self):
    c = Config.Config()
    try:
      c.load("foo: { dup dup }")
      raise RuntimeError("ValueError not raised.")
    except ValueError:
      pass

  def testMergeConflict(self):
    mc1 = """
    item:
    [
      subitem
    ]
    """
    mc2 = """
    item:
    {
      subitem
    }
    """
    c1 = Config.Config()
    c2 = Config.Config()
    c1.load(mc1)
    c2.load(mc2)
    
    try:
      c1.merge(c2)
      raise RuntimeError("ValueError not raised.")
    except ValueError:
      pass

if __name__ == "__main__":
  unittest.main()
