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

"""Commonly used data collections."""

class DefaultDict(dict):
  def __init__(self, default = int):
    self.default = default
    dict.__init__(self)

  def __getitem__(self, name):
    try:
      return dict.__getitem__(self, name)
    except KeyError:
      item = self.default()
      self[name] = item
      return item
   
class DictProxy(object):
  """
  A proxy class for making dictionary values referable with attribute syntax.
  """
  def __init__(self, dictionary):
    self.dictionary = dictionary
  
  def __getattr__(self, name):
    try:
      return getattr(self.dictionary, name)
    except AttributeError:
      return self.dictionary[name]

  def __getitem__(self, name):
    return self.dictionary[name]
  
  def __repr__(self):
    return "[" + ", ".join(self.dictionary.keys()) + "]"

def inverseDict(d):
  """
  Construct a new dictionary that performs an inverse mapping of a given dictionary.
  Note that this only works for bijective dictionaries; the mapping of duplicate values
  to unique keys is undefined.
  """
  return dict(zip(d.values(), d.keys()))

def flatten(seq):
  """
  Recursively flatten subsequences in a sequence, e.g.
  >>> flatten([a, [b, c]])
  [a, b, c]
  """
  result = []
  for item in seq:
    if hasattr(item, "__iter__") and not isinstance(item, basestring):
      result.extend(flatten(item))
    else:
      result.append(item)
  return result
