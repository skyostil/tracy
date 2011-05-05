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

"""Tracy project file."""

import Common
import common
import new
from common import Config
from common import Resource
import pickle  

class Object(object):
  """Project file object that supports serialization using only common types."""
  def serialize(self):
    pass
    
  def __serialize__(self):
    return self.serialize() or self.__dict__

class ProxyObject(Object):
  pass

class ProjectUnpickler(pickle.Unpickler):
  def find_class(self, module, name):
    try:
      return pickle.Unpickler.find_class(self, module, name)
    except (ImportError, AttributeError):
      # Create a new proxy class to masquerade as the requested class
      c = new.classobj(name + "Proxy", (common.Project.Object, ), {})
      return c

class Project(object):
  VERSION = 1
  
  def __init__(self, options = None, fileName = None):
    self.config   = Config.Config()
    self.platform = None
    self.library  = None
    self.targets  = {}
    self.options  = options
    
    if fileName:
      self.load(fileName)
  
  def load(self, fileName):
    data = ProjectUnpickler(open(fileName, "rb")).load()
    
    assert isinstance(data, dict)
    assert data["version"] <= Project.VERSION, "Tracy project file created with a newer version of Tracy."
    
    self.config   = data.get("config",   self.config)
    self.library  = data.get("library",  self.library)
    self.platform = data.get("platform", self.platform)
    self.targets  = data.get("targets",  self.targets)

    for path in self.config.paths:
      Resource.paths.append(path)

  def save(self, fileName):
    data = {
      "version":  Project.VERSION,
      "config":   self.config,
      "library":  self.library,
      "platform": self.platform,
      "targets":  self.targets,
    }
    pickle.dump(data, open(fileName, "wb"), pickle.HIGHEST_PROTOCOL)

