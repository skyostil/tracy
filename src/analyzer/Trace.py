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

"""
A class hierarchy that represents the contents of a tracer file.
"""

import Common
from common import OrderedDict

class VoidValue(int):
  __slots__ = []
  pass

class ByteValue(int):
  __slots__ = []
  pass

class ShortValue(int):
  __slots__ = []
  pass

class IntegerValue(int):
  __slots__ = []
  pass

class LongValue(long):
  __slots__ = []
  pass

class FloatValue(float):
  __slots__ = []
  pass

class DoubleValue(float):
  __slots__ = []
  pass

class Array(list):
  __slots__ = ["id"]
  
  def __init__(self, id, values):
    list.__init__(self, values)
    self.id = id
    
class ByteArrayValue(Array):
  __slots__ = []
  pass

class ShortArrayValue(Array):
  __slots__ = []
  pass

class IntegerArrayValue(Array):
  __slots__ = []
  pass

class LongArrayValue(Array):
  __slots__ = []
  pass

class FloatArrayValue(Array):
  __slots__ = []
  pass
  
class DoubleArrayValue(Array):
  __slots__ = []
  pass

class ObjectArrayValue(Array):
  __slots__ = []

class UnknownPhrase(object):
  __slots__ = ["id"]
  
  def __init__(self, id):
    self.id = id
    
  def __repr__(self):
    return "<Unknown phrase at 0x%x>" % self.id

class Class(object):
  __slots__ = ["name"]
  
  def __init__(self, name):
    self.name      = name
  
class Object(object):
  __slots__ = ["id", "ns", "cls", "attrs"]
  
  def __init__(self, id, ns, cls):
    self.id        = id
    self.ns        = ns
    self.cls       = cls
    self.attrs     = OrderedDict()
  
  def __repr__(self):
    attrs = ", ".join(["%s=%s" % (k, v) for k, v in self.attrs.items()])
    return "<%s instance at %s%x %s>" % (self.cls.name, ("%x:" % self.ns if self.ns else ""), self.id, attrs)
    
  def __eq__(self, o):
    try:
      return self.id == o.id and self.cls == o.cls and self.attrs == o.attrs
    except AttributeError:
      return False
      
  def __hash__(self):
    return hash(self.id) ^ hash(self.ns) ^ hash(self.cls) ^ hash(str(self.attrs.items()))

class InstrumentationSensor(object):
  def __init__(self, description = None, isAverage = False):
    self.description = description
    self.isAverage   = isAverage

class Event(object):
  __slots__ = ["name", "seq", "time", "apiId", "duration", "values", "modifiedArrays", "modifiedObjects", "sensorData"]
  
  def __init__(self):
    self.name            = None
    self.seq             = 0
    self.time            = 0
    self.duration        = 0
    self.apiId           = 0
    self.values          = OrderedDict()
    self.modifiedArrays  = []
    self.modifiedObjects = []
    self.sensorData      = {}

class Trace(object):
  def __init__(self):
    # List of events
    self.events  = []
    
    # Instrumentation sensors
    self.sensors = {}
