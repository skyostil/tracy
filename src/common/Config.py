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

import os
import StringIO
import re
import copy
import common
import Common

class Group(dict):
  """A group of configuration items."""
  def __init__(self, attrs = {}):
    dict.__init__(self)
    self.attrs = attrs
    
  def __getattr__(self, name):
    try:
      return self[name]
    except KeyError:
      raise AttributeError(name)
    
  def  __contains__(self, item):
    if dict.__contains__(self, item):
      return True
    if "." in item:
      key, path = item.split(".", 1)
      if dict.__contains__(self, key):
        return path in self[key]
    return False
    
  def __getitem__(self, item):
    try:
      return dict.__getitem__(self, item)
    except KeyError:
      if "." in item:
        try:
          key, path = item.split(".", 1)
          return self[key][path]
        except KeyError:
          # Show the full path in the exception
          raise KeyError(item)
      raise
    
  def __iter__(self):
    for key in self.keys():
      yield self[key]
      
  def merge(self, group, mergeGroups = True, mergeLists = True):
    assert isinstance(group, common.Config.Group)
    def mergeBranch(root, branch):
      for key, value in branch.items():
        if mergeGroups and key in root and isinstance(value, common.Config.Group):
          if isinstance(root.get(key), common.Config.Group):
            mergeBranch(root[key], value)
          else:
            raise ValueError("Unable to merge group '%s' due to type mismatch (target is %s) " % (key, type(root.get(key))))
        elif mergeLists and key in root and isinstance(value, common.Config.List):
          if isinstance(root.get(key), common.Config.List):
            root[key] += value
          else:
            raise ValueError("Unable to merge list '%s' due to type mismatch (target is %s) " % (key, type(root.get(key))))
        else:
          root[key] = value
    mergeBranch(self, group)

class List(list):
  """A list of configuration items."""
  def __init__(self, attrs = {}):
    list.__init__(self)
    self.attrs      = attrs

class Item(str):
  """A single configuration item."""
  def __new__(cls, value = None, attrs = {}):
    if value is None:
      value = ""
    s = str.__new__(cls, value)
    s.attrs       = attrs
    return s
    
  def __deepcopy__(self, memo):
    s = Item(self, self.attrs)
    return s

class Config(Group):
  SYSTEM_NAMESPACE = "sys"
  
  """A configuration setting database."""
  def __init__(self, fileName = None):
    self.paths = []
    if fileName:
      self.loadFile(fileName)
    else:
      self.paths.append(os.path.abspath("."))

  def load(self, data):
    if type(data) == file:
      data = data.read()

    baseConfigs = []
    config      = common.ConfigParser.parseConfig(data)

    # Process all top-level commands in the system namespace    
    for itemName in config.keys():
      if "." in itemName:
        ns, command = itemName.split(".", 1)
        if ns == Config.SYSTEM_NAMESPACE:
          if command == "include":
            # Remove this config section and parse the included file
            for includeFile in config[itemName]:
              baseConfigs.append(Config(self.getRelativePath(includeFile)))
            del config[itemName]

    # Merge all the loaded configs in the correct order
    for c in baseConfigs + [config]:
      self.merge(c)

      # Sync the paths too
      for path in c.paths:
        if not path in self.paths:
          self.paths.append(path)

  def save(self):
    output = StringIO.StringIO()
    indent = "     "
    
    def safeString(s):
      s = str(s).replace("\\", "\\\\")
      if not re.match("^[a-zA-Z_][A-Za-z_0-9.]*$", s):
        return '"%s"' % s
      return s

    def saveGroup(output, group, depth = 0):
      for key, value in group.items():
        attrs = ", ".join(["%s=%s" % (safeString(k), safeString(v)) for k, v in group[key].attrs.items()])
        if attrs:
          attrs = "(%s)" % attrs
        if isinstance(value, common.Config.Group):
          print >>output, indent * depth + safeString(key) + attrs + ":"
          print >>output, indent * depth + "{"
          saveGroup(output, value, depth + 1)
          print >>output, indent * depth + "}"
        elif isinstance(value, common.Config.List):
          print >>output, indent * depth + safeString(key) + attrs + ":"
          print >>output, indent * depth + "["
          saveList(output, value, depth + 1)
          print >>output, indent * depth + "]"
        elif isinstance(value, common.Config.Item):
          if value:
            print >>output, indent * depth + '%-16s %s' % (safeString(key) + attrs + ":", safeString(value))
          else:
            print >>output, indent * depth + (safeString(key) + attrs)
        else:
          raise ValueError("Unknown config group item '%s'." % key)
    
    def saveList(output, list, depth = 0):
      for value in list:
        attrs = ", ".join(["%s=%s" % (safeString(k), safeString(v)) for k, v in value.attrs.items()])
        if attrs:
          attrs = "(%s)" % attrs
        if isinstance(value, common.Config.Group):
          print >>output, indent * depth + "{"
          saveGroup(output, value, depth + 1)
          print >>output, indent * depth + "}"
        elif isinstance(value, common.Config.List):
          print >>output, indent * depth + "["
          saveList(output, value, depth + 1)
          print >>output, indent * depth + "]"
        elif isinstance(value, common.Config.Item):
          print >>output, indent * depth + safeString(value) + attrs
        else:
          raise ValueError("Unknown config list item '%s'." % value)
    
    saveGroup(output, self)
    return output.getvalue()

  def loadFile(self, fileName):
    path = os.path.abspath(os.path.dirname(fileName))
    if not path in self.paths:
      self.paths.append(path)
    self.load(open(fileName))

  def getRelativePath(self, fileName):
    for path in self.paths:
      f = os.path.join(path, fileName)
      if os.path.exists(f): 
        return f
    return os.path.join(self.paths[0], fileName)
