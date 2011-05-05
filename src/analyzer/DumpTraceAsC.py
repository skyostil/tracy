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

import sys
import Trace
import BinaryCodec
from optparse import OptionParser

print "This script is obsolete. Please use the analyzer tool to export C files."
sys.exit(1)

def parseCommandLine(args):
  parser = OptionParser(usage = "Usage: %prog [options] [trace file]")
  parser.add_option("-v", "--verbose", action = "store_true", help = "show verbose status messages")
  parser.add_option("-s", "--seekable", action = "store_true", help = "generate a seekable trace")
  options, args = parser.parse_args(args)
  
  return options, args

# Parse the command line options
options, args = parseCommandLine(sys.argv[1:])

if not args:
  print "Trace file missing."
  sys.exit(1)

trace  = Trace.Trace()
reader = BinaryCodec.Reader(trace, open(args[0], "rb"))
reader.load()

# Map of types to native C types
typeMap = {
  Trace.ByteValue:     "char",
  Trace.ShortValue:    "short",
  Trace.IntegerValue:  "int",
  Trace.LongValue:     "long",
  Trace.FloatValue:    "float",
  Trace.DoubleValue:   "double",
}

# Map of array types to native C types
arrayTypeMap = {
  Trace.ByteArrayValue:     "char",
  Trace.ShortArrayValue:    "short",
  Trace.IntegerArrayValue:  "int",
  Trace.LongArrayValue:     "long",
  Trace.FloatArrayValue:    "float",
  Trace.DoubleArrayValue:   "double",
}

class CustomClass(object):
  @staticmethod
  def declare():
    pass

  @staticmethod
  def create(name):
    pass

  @staticmethod
  def destroy(name):
    pass
    
class CFbsBitmapClass(CustomClass):
  @staticmethod
  def declare():
    return "CFbsBitmap*"

  @staticmethod
  def create(name, width, height, mode):
    return "%s = new CFbsBitmap(); %s->Create(TSize(%d, %d), (TDisplayMode)%d);" % (name, name, width, height, mode)

  @staticmethod
  def destroy(name):
    return "delete %s;" % name

class TSizeClass(CustomClass):
  @staticmethod
  def declare():
    return "TSize"

  @staticmethod
  def create(name, width, height):
    return "%s = TSize(%d, %d);" % (name, width, height)

# Some very basic object classes
customClasses = {
  "TSize":      TSizeClass,
  "CFbsBitmap": CFbsBitmapClass,
}

# Print a header
print """/*
 *  Generated C code from a trace of %d events.
 */
""" % (len(trace.events))

print """/* Nasty hack: Since we don't know the actual types of the function parameters,
 * we generate a bunch of fake function declarations that only use basic types
 * as parameters. This can be circumvented by parsing the tracer configuration 
 * at this end as well.
 */"""
initializedEvents = []

def getParameterType(value):
  if isinstance(value, Trace.Object):
    if value.cls.name in customClasses:
      type = customClasses[value.cls.name].declare()
      if type: return type
    return "void*"
  elif isinstance(value, list):
    return arrayTypeMap[value.__class__] + "*"
  else:
    return typeMap[value.__class__]

for event in trace.events:
  if event.name in initializedEvents:
    continue
  initializedEvents.append(event.name)
  
  types = [getParameterType(v) for k, v in event.values.items() if k]

  # Check if we need to save the return type
  if None in event.values :
    returnType = getParameterType(event.values[None])
    sys.stdout.write(returnType + " ")
  else:
    sys.stdout.write("void ")
    
  sys.stdout.write(event.name + "(")
  sys.stdout.write(", ".join([str(t) for t in types]))
  sys.stdout.write(");")
  print
print

# First stage: create all the needed arrays and objects
classes = {}
arrays  = []

def getObjectId(obj):
  try:
    return classes[obj.cls][obj.id]
  except KeyError:
    pass

def getArrayId(array):
  try:
    #return arrays.index(hash(tuple(array))) + 1
    return arrays.index(id(array)) + 1
  except ValueError:
    pass

def allocateArray(array):
  #id = hash(tuple(array))
  i = id(array)
  assert not i in arrays
  arrays.append(i)
  return getArrayId(array)
    
def allocateObject(obj):
  if not obj.cls in classes:
    classes[obj.cls] = {}
  assert not obj.cls in classes[obj.cls]
  classes[obj.cls][obj.id] = len(classes[obj.cls]) + 1
  return getObjectId(obj)
    
for event in trace.events:
  for key, value in event.values.items():
    if isinstance(value, Trace.Object) and value.id:
      if not getObjectId(value):
        name = "%s%d" % (value.cls.name.lower(), allocateObject(value))
        if value.cls.name in customClasses:
          code = customClasses[value.cls.name].declare() + " %s;" % name
          if code:
            print code
        else:
          if value.attrs:
            print "/* Insert code to declare %s */" % (value)
          print "%s %s;" % (value.cls.name, name)
        print
    elif isinstance(value, list):
      if not getArrayId(value):
        name = "array%d" % allocateArray(value)
        type = arrayTypeMap[value.__class__]
        print "%s %s[%d] = {" % (type, name, len(value))
        print "    ",
        for i, v in enumerate(value):
          if not (i + 1) % 8:
            print
            print "    ",
          sys.stdout.write(str(v))
          if i != len(value) - 1:
            print ", ",
        print
        print "};"
        print

# Second stage: initialize all the objects
print "void init(void)"
print "{"
initializedObjects = []
for event in trace.events:
  for key, value in event.values.items():
    if isinstance(value, Trace.Object) and value.id and not value.id in initializedObjects:
      initializedObjects.append(value.id)
      name = "%s%d" % (value.cls.name.lower(), getObjectId(value))
      if value.cls.name in customClasses:
        code = customClasses[value.cls.name].create(name, *value.attrs.values())
        if code:
          print "    " + code
      else:
        if value.attrs:
          print "/* Insert code to create %s */" % (value)
      
print "}"
print

# Third stage: insert all the events
if options.seekable:
  print "void play(int eventNumber)"
  print "{"
  print "    switch (eventNumber %% %d)" % len(trace.events)
  print "    {"
else:
  print "void play(void)"
  print "{"
  
for i, event in enumerate(trace.events):
  args = []
  for key, value in event.values.items():
    if not key:
      continue
    if isinstance(value, Trace.Object):
      if not value.id:
        name = "0"
      else:
        name = "%s%d" % (value.cls.name.lower(), getObjectId(value))
      args.append(name)
    elif isinstance(value, list):
      name = "array%d" % getArrayId(value)
      args.append(name)
    else:
      args.append(value)
      
  # Indent
  if options.seekable:
    print "    case %5d: " % i,
  else:
    print "    ",
      
  # Check if we need to save the return type
  if None in event.values :
    returnValue = event.values[None]
    if isinstance(returnValue, Trace.Object):
      name = "%s%d" % (returnValue.cls.name.lower(), getObjectId(returnValue))
      sys.stdout.write("%s = " % name)
  sys.stdout.write(event.name + "(")
  sys.stdout.write(", ".join([str(a) for a in args]))
  sys.stdout.write(");")

  if options.seekable:
    print " break;"
  else:
    print

if options.seekable:
  print "    }"

print "}"
