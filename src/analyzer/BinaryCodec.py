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
A codec for reading and writing binary-encoded trace files.
"""

import sys
import struct
import string
import Trace
import Common
import binascii
from common import Task
from common import Log

def cstr(s):
  if s is None or isinstance(s, Trace.UnknownPhrase):
    return s
  return s.rstrip("\x00")

FORMAT_VERSION = "TCY2"

TOK_DEFINE         = 0x1
TOK_EVENT          = 0x2
TOK_VALUE          = 0x3
TOK_BEGIN_OBJECT   = 0x4
TOK_API            = 0x80
TOK_DURATION       = 0x81
TOK_END_OBJECT     = 0x82
TOK_TINY_TOKEN_BIT = 0x80

TR_VOID           = 0x0
TR_BYTE           = 0x1
TR_SHORT          = 0x2
TR_INTEGER        = 0x3
TR_LONG           = 0x4
TR_FLOAT          = 0x5
TR_DOUBLE         = 0x6
TR_OBJECT         = 0x7
TR_PHRASE_BIT     = 0x80

# Provide verbose diagnostic information for some trace characteristics
verbose = False

# Pool of shared string constants
class StringCache(object):
  def __init__(self):
    self.strings = {}
    
  def get(self, string):
    try:
      return self.strings[string]
    except KeyError:
      self.strings[string] = string
      return string

class ClassNamespace(object):
  def __init__(self):
    self.objects       = {}

class Class(Trace.Class):
  def __init__(self, name):
    Trace.Class.__init__(self, name)
    self.namespaces    = {}
   
class Decoder(object):
  def __init__(self):
    self.phrasebook    = {}
    self.classes       = {}
    self.currentObject = None
    self.currentEvent  = None
    self.apiId         = 0
    self.typeMap = {
      TR_BYTE:       ("<B",   1,  Trace.ByteValue,),
      TR_SHORT:      ("<h",   2,  Trace.ShortValue,),
      TR_INTEGER:    ("<i",   4,  Trace.IntegerValue,),
      TR_LONG:       ("<q",   8,  Trace.LongValue),
      TR_FLOAT:      ("<f",   4,  Trace.FloatValue),
      TR_DOUBLE:     ("<d",   8,  Trace.DoubleValue),
      TR_OBJECT:     ("<III", 12, None),
    }
    self.arrayTypeMap = {
      TR_BYTE:       ("<B", 1, Trace.ByteArrayValue),
      TR_SHORT:      ("<h", 2, Trace.ShortArrayValue),
      TR_INTEGER:    ("<i", 4, Trace.IntegerArrayValue),
      TR_LONG:       ("<q", 8, Trace.LongArrayValue),
      TR_FLOAT:      ("<f", 4, Trace.FloatArrayValue),
      TR_DOUBLE:     ("<d", 8, Trace.DoubleArrayValue),
      TR_OBJECT:     ("<I", 4, Trace.ObjectArrayValue),
    }

  def getObject(self, className, ns, handle):
    # Create the class if it's new
    if not className in self.classes:
      self.classes[className] = Class(className)
    cls = self.classes[className]
    try:
      # Construct a null object
      try:
        if not ns in cls.namespaces:
          cls.namespaces[ns] = ClassNamespace()
        return cls.namespaces[ns].objects[handle]
      except KeyError:
        return Trace.Object(handle, ns, cls)
    except KeyError:
      raise ValueError("Unknown %s instance at 0x%x" % (className, handle))
  
  def decodeValue(self, type, data):
    try:
      # The void type doesn't require any translation
      if type == TR_VOID:
        return data
      
      format, size, cls = self.typeMap[type]
      value = struct.unpack(format, data)
      if type == TR_OBJECT:
        className, ns, handle = cstr(self.getPhrase(value[0])), value[1], value[2]
        return self.getObject(className, ns, handle)
      return cls(value[0])
    except KeyError:
      raise ValueError("Unsupported value type: 0x%x" % type)
    except struct.error, e:
      raise ValueError("Badly formatted data of type 0x%x: %s" % (type, data))

  def decodeValueArray(self, id, type, data):
    try:
      format, size, arrayClass = self.arrayTypeMap[type]
    except KeyError:
      return data
    items = len(data) / size
    return arrayClass(id, struct.unpack("%s%d%s" % (format[0], items, format[1:]), data))
      
  def getPhrase(self, id):
    try:
      if not id:
        return None
      return self.phrasebook[id]
    except:
      return Trace.UnknownPhrase(id)
      
  def setPhrase(self, id, data):
    # Check that this definition is really necessary
    if verbose and name in self.phrasebook:
      oldData = self.phrasebook[name]
      if data == oldData:
        Log.warn("%d bytes of redundant phrase data for phrase 0x%x" % (len(data), name))
      elif len(oldData) < len(data):
        for v1, v2 in zip(oldData, data):
          if v1 != v2:
            break
        else:
          Log.warn("%d bytes of partially redundant phrase data for phrase 0x%x" % (len(data), name))
      
    self.phrasebook[id] = data

class Encoder(object):
  def __init__(self, file):
    self.file          = file
    self.phrasebook    = {}
    self.classes       = {}
    self.stringPoolId  = 1
    self.stringPool    = {}
    self.apiId         = 0
    self.typeMap = {
      Trace.VoidValue:           (TR_VOID,    "I",    4),
      Trace.ByteValue:           (TR_BYTE,    "B",    1),
      Trace.ShortValue:          (TR_SHORT,   "h",    2),
      Trace.IntegerValue:        (TR_INTEGER, "i",    4),
      Trace.LongValue:           (TR_LONG,    "q",    8),
      Trace.FloatValue:          (TR_FLOAT,   "f",    4),
      Trace.DoubleValue:         (TR_DOUBLE,  "d",    8),
      Trace.Object:              (TR_OBJECT,  "III", 12),
      Trace.ByteArrayValue:      (TR_BYTE,    "B",    1),
      Trace.ShortArrayValue:     (TR_SHORT,   "h",    2),
      Trace.IntegerArrayValue:   (TR_INTEGER, "i",    4),
      Trace.LongArrayValue:      (TR_LONG,    "q",    8),
      Trace.FloatArrayValue:     (TR_FLOAT,   "f",    4),
      Trace.DoubleArrayValue:    (TR_DOUBLE,  "d",    8),
      Trace.ObjectArrayValue:    (TR_OBJECT,  "I",    4),
      # TODO: make sure this works always
      int:                       (TR_INTEGER, "i",    4),
      float:                     (TR_FLOAT,   "f",    4),
    }
    
  def encodeValue(self, value):
    """
    Encode a value into a token and return the token representing the value.
    
    @param value      Value to encode, i.e. None, UnknownPhrase, Trace.Array, Trace.Value, Trace.Object
    @returns Token
    """
    
    # A value of None and unknown phrases are coerced to null pointers
    if value is None or isinstance(value, Trace.UnknownPhrase):
      return TR_VOID | TR_PHRASE_BIT, struct.pack("<I", 0)
    
    type, format, size = self.typeMap[value.__class__]

    # Write value arrays through the phrasebook
    if isinstance(value, Trace.Array):
      # If this is a new value array, define it
      if not value.id in self.phrasebook or self.phrasebook[value.id] != value:
        token          = DefineToken()
        token.phraseId = value.id
        token.type     = type
        if type == TR_OBJECT and len(value):
          # FIXME: what to do when we have a zero-length object array?
          for obj in value:
            self._maybeSerializeObject(obj)
          token.classNameId = self.defineString(value[0].cls.name)
          token.ns = value[0].ns
          token.phrase = struct.pack("<%d%s" % (len(value), format), *[obj.id for obj in value])
        else:
          token.phrase = struct.pack("<%d%s" % (len(value), format), *value)
        token.size   = len(token.phrase)
        
        # If this is the first instance of this array, we may shave off any terminating zeros
        if not value.id in self.phrasebook:
          token.phrase = token.phrase.rstrip("\x00")
        
        self.file.write(token.save())
        self.phrasebook[value.id] = value[:]
      return type | TR_PHRASE_BIT, struct.pack("<I", value.id)
    elif isinstance(value, Trace.Object):
      classNameId = self.defineString(value.cls.name)
      self._maybeSerializeObject(value)
      return type, struct.pack("<" + format, classNameId, value.ns, value.id)
    else:
      # A plain value fits into the data field
      return type, struct.pack("<" + format, value)
  
  def _maybeSerializeObject(self, object):
    classNameId       = self.defineString(object.cls.name)
    newObject         = False

    # See if this is a new object
    if not object.cls in self.classes:
      self.classes[object.cls] = Class(object.cls.name)
      newObject = True
      
    if not object.ns in self.classes[object.cls].namespaces:
      self.classes[object.cls].namespaces[object.ns] = ClassNamespace()
      newObject = True

    if not object.id in self.classes[object.cls].namespaces[object.ns].objects:
      self.classes[object.cls].namespaces[object.ns].objects[object.id] = object
      newObject = True
      
    if object.attrs != self.classes[object.cls].namespaces[object.ns].objects[object.id].attrs:
      newObject = True
      
    # Objects without attributes don't need to be serialized
    #if not object.attrs:
    #  newObject = False
      
    if newObject:
      # Serialize all fields of objects
      token             = BeginObjectToken()
      token.classNameId = classNameId
      token.ns          = object.ns
      token.handle      = object.id
      self.file.write(token.save())
      
      for attr, val in object.attrs.items():
        token        = ValueToken()
        token.nameId = self.defineString(attr)
        token.type, token.valueData = self.encodeValue(val)
        self.file.write(token.save())

      token             = EndObjectToken()
      self.file.write(token.save())

  def encodeValueArray(self, type, data):
    try:
      format, cls, arrayClass = self.typeMap[type]
      return arrayClass(struct.pack("%s%d%s" % (format[0], len(data), format[1:]), *data))
    except KeyError:
      return data

  def defineString(self, s):
    # The id zero is reserved for null strings
    if not s:
      return 0
    
    # Append a zero terminator for C
    s += "\x00"
    
    # If the string is already defined, reuse the same id
    if s in self.stringPool:
      return self.stringPool[s]
      
    # Find a free id for this string
    while self.stringPoolId in self.phrasebook:
      self.stringPoolId += 1
      
    # Register the assignment
    self.stringPool[s] = self.stringPoolId
    self.phrasebook[self.stringPoolId] = s
    
    # Create a token for recording the mapping
    token          = DefineToken()
    token.phraseId = self.stringPoolId
    token.size     = len(s)
    token.type     = TR_VOID
    token.phrase   = s
    self.file.write(token.save())
    
    return self.stringPoolId
  
class Token(object):
  __slots__ = ["id", "data"]
  
  def __init__(self, id):
    self.id    = id
    self.data  = None

  @staticmethod
  def create(id):
    try:
      return tokenClasses[id]()
    except KeyError:
      raise ValueError("Unknown token identifier: 0x%x" % id)
  
  @staticmethod
  def read(file):
    header = file.read(4 + 1)
    
    if not header:
      return None
      
    if not len(header) == 4 + 1:
      raise ValueError("Truncated file.")
    
    id, length = Token.unserialize("BI", header)
    token = Token.create(id)
    
    if token.isTinyToken():
      data = header
    else:
      data = file.read(length)
      if not len(data) == length:
        raise ValueError("Truncated file.")
      data = header + data
      
    token.load(data)
    return token
    
  def isTinyToken(self):
    return self.id & TOK_TINY_TOKEN_BIT
  
  def load(self, data):
    if self.id & TOK_TINY_TOKEN_BIT:
      id, self.data = self.unserialize("BI", data)
      assert id == self.id
    else:
      id, length = self.unserialize("BI", data[:4 + 1])
      self.data = data[4 + 1:4 + 1 + length]
      assert id == self.id
      assert len(self.data) == length
    
  def save(self):
    if self.id & TOK_TINY_TOKEN_BIT:
      return self.serialize("BI", self.id, self.data is not None and self.data or 0)
    else:
      return self.serialize("BI%ds" % len(self.data), self.id, len(self.data), self.data)
    
  @staticmethod
  def unserialize(format, data):
    return struct.unpack("<" + format, data)
    
  @staticmethod
  def serialize(format, *args):
    return struct.pack("<" + format, *args)
    
  def __repr__(self):
    def printableString(s):
      if not type(s) == str:
        return s
        
      for c in s:
        if not c in string.printable + "\x00":
          return "[" + ", ".join(["%d" % ord(c) for c in s]) + "]"
      return s
      
    return "<%s: %s>" % (self.__class__.__name__, ", ".join("%s=%s" % (k, printableString(getattr(self, k))) for k in self.__slots__ if not k in ["id", "data"]))
  
class DefineToken(Token):
  __slots__ = ["phraseId", "size", "type", "phrase", "classNameId", "ns"]

  def __init__(self):
    Token.__init__(self, TOK_DEFINE)
    self.phraseId     = 0
    self.size         = 0
    self.type         = TR_VOID
    self.phrase       = None
    self.classNameId  = 0
    self.ns           = 0
  
  def load(self, data):
    Token.load(self, data)
    self.phraseId, self.size, self.type = self.unserialize("IIB", self.data[:4 * 2 + 1])
    if self.type == TR_OBJECT:
      self.classNameId, self.ns = self.unserialize("II", self.data[9:9 + 4 * 2])
      self.phrase = self.data[9 + 4 * 2:]
    else:
      self.phrase = self.data[9:]
    assert self.size >= len(self.phrase), "Define token has more data than it should have (%d bytes vs %d bytes)" % (self.size, len(self.phrase))
    self.phrase = self.phrase + "\x00" * (self.size - len(self.phrase))
    
  def save(self):
    if self.type == TR_OBJECT:
      self.data = self.serialize("IIBII%ds" % len(self.phrase), self.phraseId, self.size, self.type, self.classNameId, self.ns, self.phrase)
    else:
      self.data = self.serialize("IIB%ds" % len(self.phrase), self.phraseId, self.size, self.type, self.phrase)
    return Token.save(self)

class EventToken(Token):
  __slots__ = ["nameId", "seq", "time"]
  
  def __init__(self):
    Token.__init__(self, TOK_EVENT)
    self.nameId       = 0
    self.seq          = 0
    self.time         = 0
    
  def load(self, data):
    Token.load(self, data)
    self.nameId, self.seq, self.time = self.unserialize("III", self.data)
    
  def save(self):
    self.data = self.serialize("III", self.nameId, self.seq, self.time)
    return Token.save(self)
    
class ValueToken(Token):
  __slots__ = ["nameId", "type", "valueData"]

  def __init__(self):
    Token.__init__(self, TOK_VALUE)
    self.nameId       = 0
    self.type         = 0
    self.valueData    = None
    
  def load(self, data):
    Token.load(self, data)
    self.nameId, self.type = self.unserialize("IB", self.data[:4 + 1])
    self.valueData = self.data[4 + 1:]
      
  def save(self):
    self.data = self.serialize("IB", self.nameId, self.type) + self.valueData
    return Token.save(self)

class BeginObjectToken(Token):
  __slots__ = ["classNameId", "ns", "handle"]
  
  def __init__(self):
    Token.__init__(self, TOK_BEGIN_OBJECT)
    self.classNameId = 0
    self.ns          = 0
    self.handle      = 0

  def load(self, data):
    Token.load(self, data)
    self.classNameId, self.ns, self.handle = self.unserialize("III", self.data)
    
  def save(self):
    self.data = self.serialize("III", self.classNameId, self.ns, self.handle)
    return Token.save(self)

class EndObjectToken(Token):
  __slots__ = []
  
  def __init__(self):
    Token.__init__(self, TOK_END_OBJECT)
    
class ApiToken(Token):
  __slots__ = ["apiId"]

  def __init__(self):
    Token.__init__(self, TOK_API)
    self.apiId = 0
  
  def load(self, data):
    Token.load(self, data)
    self.apiId = self.data
    
  def save(self):
    self.data = self.apiId
    return Token.save(self)
    
class DurationToken(Token):
  __slots__ = ["duration"]
  
  def __init__(self):
    Token.__init__(self, TOK_DURATION)
    self.duration = 0
  
  def load(self, data):
    Token.load(self, data)
    self.duration = self.data

  def save(self):
    self.data = self.duration
    return Token.save(self)

# All defined tokens
tokenClasses = {
  TOK_DEFINE:       DefineToken,
  TOK_EVENT:        EventToken,
  TOK_VALUE:        ValueToken,
  TOK_BEGIN_OBJECT: BeginObjectToken,
  TOK_END_OBJECT:   EndObjectToken,
  TOK_API:          ApiToken,
  TOK_DURATION:     DurationToken,
}
    
class Reader(object):
  def __init__(self, trace, file):
    self.trace        = trace
    self.file         = file
    self.decoder      = Decoder()
    self.tokenClasses = {
      TOK_DEFINE:       DefineToken,
      TOK_EVENT:        EventToken,
      TOK_VALUE:        ValueToken,
      TOK_BEGIN_OBJECT: BeginObjectToken,
      TOK_END_OBJECT:   EndObjectToken,
      TOK_API:          ApiToken,
      TOK_DURATION:     DurationToken,
    }
    self.stringCache  = StringCache()
    
  def load(self):
    # Determine the trace file size
    try:
      self.file.seek(0, 2)
      fileSize = self.file.tell()
      self.file.seek(0)
    except:
      fileSize = 0
    
    task = Task.startTask("load", "Loading trace", steps = fileSize)

    # Read and verify version magic
    version = self.file.read(4)
    if version != FORMAT_VERSION:
      raise RuntimeError("Unsupported file format version: %s (%s)" % (version, binascii.hexlify(version)))

    # First pass: read all the events
    for i, event in enumerate(self.readEvents()):
      self.trace.events.append(event)
      if fileSize:
        task.updateProgress(self.file.tell())
      elif (i & 0xff) == 0:
        task.step()

    task.finish()
    task = Task.startTask("resolve", "Resolving data", len(self.trace.events))
      
    # Second pass: resolve any unknown phrases
    for event in self.trace.events:
      if isinstance(event.name, Trace.UnknownPhrase):
        Log.warn("Event %d name is undefined, trace is probably corrupted." % event.seq)
        self.trace.events.remove(event)
        continue

      for key, value in event.values.items():
        if isinstance(key, Trace.UnknownPhrase):
          Log.warn("Event %d parameter name is undefined, trace is probably corrupted." % event.seq)
          self.trace.events.remove(event)
          continue

        if isinstance(value, Trace.UnknownPhrase):
          v2 = value
          value = self.decoder.getPhrase(value.id)
          if isinstance(value, Trace.UnknownPhrase):
            #Log.warn("Unable to resolve unknown phrase %s.%s = %s." % (event.name, key, value))
            pass
          else:
            event.values[key] = value
            if not value in event.modifiedArrays:
              event.modifiedArrays.append(value)
      task.step()

  def isTracePlayable(self):
    """
    Determine whether this trace file is properly normalized and playable.
    """
    phraseSizes = {}
    self.file.seek(0)
    
    while 1:
      try:
        token = Token.read(self.file)
        if not token:
          return True
        if isinstance(token, DefineToken):
          if not token.phraseId in phraseSizes:
            phraseSizes[token.phraseId] = token.size
          elif phraseSizes[token.phraseId] < token.size:
            return False
        elif isinstance(token, ValueToken):
          if token.type & TR_PHRASE_BIT:
            phraseId, = Token.unserialize("I", token.valueData)
            if phraseId and not phraseId in phraseSizes:
              return False
      except ValueError, e:
        return True

  def _handleDefineToken(self, token):
    data = self.decoder.decodeValueArray(token.phraseId, token.type, token.phrase)

    # If the token contains a list of objects, translate their handles to live references
    if token.type == TR_OBJECT:
      className = self.stringCache.get(cstr(self.decoder.getPhrase(token.classNameId)))
      data = Trace.ObjectArrayValue(token.phraseId, [self.decoder.getObject(className, token.ns, handle) for handle in data])
   
    self.decoder.setPhrase(token.phraseId, data)
    # If this token is associated with an event and the phrase data is not opaque (TR_VOID),
    # add this phrase to the list of modified arrays for this event.
    if self.decoder.currentEvent and token.type != TR_VOID:
      self.decoder.currentEvent.modifiedArrays.append(self.decoder.getPhrase(token.phraseId))

  def _handleEventToken(self, token):
    currentEvent = self.decoder.currentEvent
    self.decoder.currentEvent       = Trace.Event()
    self.decoder.currentEvent.name  = self.stringCache.get(cstr(self.decoder.getPhrase(token.nameId)))
    self.decoder.currentEvent.seq   = token.seq
    self.decoder.currentEvent.time  = token.time
    self.decoder.currentEvent.apiId = self.decoder.apiId
    return currentEvent
    
  def _handleValueToken(self, token):
    name = self.stringCache.get(cstr(self.decoder.getPhrase(token.nameId)))

    if token.type & TR_PHRASE_BIT:
      valueId, = Token.unserialize("I", token.valueData)
      value    = self.decoder.getPhrase(valueId)
    else:
      value    = self.decoder.decodeValue(token.type, token.valueData)
      
    if self.decoder.currentObject:
      self.decoder.currentObject.attrs[name] = value
    elif self.decoder.currentEvent:
      self.decoder.currentEvent.values[name] = value
      # Register any object arrays as modified by the current event
      if isinstance(value, Trace.ObjectArrayValue):
        for obj in value:
          self.decoder.currentEvent.modifiedObjects.append(obj)
      # Register any regular arrays as modified by the current event
      elif isinstance(value, Trace.Array):
        self.decoder.currentEvent.modifiedArrays.append(value)
      # Register any objects as modified by the current event
      elif isinstance(value, Trace.Object):
        self.decoder.currentEvent.modifiedObjects.append(value)
    else:
      raise ValueError("Stray value token encountered.")
          
  def _handleDurationToken(self, token):
    if self.decoder.currentEvent:
      self.decoder.currentEvent.duration = token.duration
    else:
      raise ValueError("Stray duration token encountered.")

  def _handleApiToken(self, token):
    self.decoder.apiId = token.apiId

  def _handleBeginObjectToken(self, token):
    className = self.stringCache.get(cstr(self.decoder.getPhrase(token.classNameId)))

    # Create the class if it's new
    if not className in self.decoder.classes:
      self.decoder.classes[className] = Class(className)
      
    cls = self.decoder.classes[className]
    obj = Trace.Object(token.handle, token.ns, cls)

    if not self.decoder.currentObject:
      self.decoder.currentObject = obj
    else:
      raise ValueError("Nested object specification.")

  def _handleEndObjectToken(self, token):
    obj = self.decoder.currentObject
    if obj:
      # See if this is a duplicate object. If so, check if it's really different
      # and only then add it to the list of modified objects.
      cls = self.decoder.currentObject.cls
      if obj.ns in cls.namespaces and obj.id in cls.namespaces[obj.ns].objects and obj.attrs != cls.namespaces[obj.ns].objects[obj.id].attrs:
        if self.decoder.currentEvent:
          self.decoder.currentEvent.modifiedObjects.append(obj)
        else:
          raise ValueError("Stray end object token encountered.")

      # New namespace?     
      if not obj.ns in cls.namespaces:
        cls.namespaces[obj.ns] = ClassNamespace()
 
      # Add the object to our collection
      cls.namespaces[obj.ns].objects[obj.id] = obj
      self.decoder.currentObject = None
    else:
      raise ValueError("Stray object closing token.")
          
  def readEvents(self):
    tokenHandlers = {
      DefineToken:      self._handleDefineToken,
      EventToken:       self._handleEventToken,
      ValueToken:       self._handleValueToken,
      DurationToken:    self._handleDurationToken,
      ApiToken:         self._handleApiToken,
      BeginObjectToken: self._handleBeginObjectToken,
      EndObjectToken:   self._handleEndObjectToken,
    }
    
    while 1:
      try:
        token = Token.read(self.file)
      except ValueError, e:
        Log.warn("Error reading token: %s" % e)
        continue
      
      if not token:
        break
    
      try:
        event = tokenHandlers[token.__class__](token)
      except Exception, e:
        Log.warn("Error decoding token %s: %s" % (token.__class__.__name__, e))
        continue
      
      if event:
        yield event
          
    if self.decoder.currentEvent:
      yield self.decoder.currentEvent

class Writer(object):
  def __init__(self, trace, file):
    self.trace        = trace
    self.file         = file
    self.encoder      = Encoder(file)
    
  def save(self):
    task = Task.startTask("save", "Saving trace", len(self.trace.events))

    # Find out the maximum sizes of all arrays
    maxSize = {}
    for event in self.trace.events:
      for array in event.modifiedArrays:
        try:
          elementSize = self.encoder.typeMap[array.__class__][2]
        except KeyError:
          print event.name, event.seq, event.values
        maxSize[array.id] = max(maxSize.get(array.id, 0), elementSize * len(array))
   
    # Write version magic 
    self.file.write(FORMAT_VERSION)

    for event in self.trace.events:
      # Update the active API
      if event.apiId != self.encoder.apiId:
        self.encoder.apiId = event.apiId
        token       = ApiToken()
        token.apiId = event.apiId
        self.file.write(token.save())
        
      # Write a token for the event
      token        = EventToken()
      token.nameId = self.encoder.defineString(event.name)
      token.seq    = event.seq
      token.time   = event.time
      self.file.write(token.save())
      
      # Update any modified arrays
      for array in event.modifiedArrays:
        # If this is the first definition of this array, extend the array to cover its maximum size.
        if array.id in maxSize:
          elementSize = self.encoder.typeMap[array.__class__][2]
          assert maxSize[array.id] >= elementSize * len(array)
          array.extend([0] * (maxSize[array.id] / elementSize - len(array)))
          del maxSize[array.id]
        self.encoder.encodeValue(array)

      # Update any modified objects
      for obj in event.modifiedObjects:
        self.encoder.encodeValue(obj)
      
      # Write a duration token
      if event.duration:
        token          = DurationToken()
        token.duration = event.duration
        self.file.write(token.save())

      # Write the parameters
      for key, value in event.values.items():
        token        = ValueToken()
        token.nameId = self.encoder.defineString(key)
        token.type, token.valueData = self.encoder.encodeValue(value)
        self.file.write(token.save())
      task.step()
