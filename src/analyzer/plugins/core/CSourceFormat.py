# -*- encoding: iso-8859-1 -*-
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

import re
import Plugin
from common import Task
import Trace
import Common
import StringUtils
import time
from common import OrderedDict
from common import Library
from common.Collections import DefaultDict

# Map of array types to native C types
arrayTypeMap = {
  Trace.ByteArrayValue:     "char",
  Trace.ShortArrayValue:    "short",
  Trace.IntegerArrayValue:  "int",
  Trace.LongArrayValue:     "long",
  Trace.FloatArrayValue:    "float",
  Trace.DoubleArrayValue:   "double",
}

copyrightText = ""

class CSourceExporterPlugin(Plugin.ExporterPlugin):
  """
  C source trace file writer.
  """
  formatName = "c"
  
  def saveTrace(self, trace, traceFile, 
                dataFileName       = None,
                dataFileFormat     = "rvct",
                frameMarkers       = [], 
                initFuncName       = "init",
                uninitFuncName     = "uninit",
                playFuncName       = "play",
                playFrameFuncName  = "playFrame",
                frameFuncName      = "frame",
                arrayPrefix        = "array",
                playerArgument     = "context",
                insertCopyright    = True):
    try:
      library = self.analyzer.project.targets["code"].library
      config  = self.analyzer.project.config
    except (AttributeError, KeyError):
      raise RuntimeError("API configuration not found.")

    def arrayId(array):
      assert isinstance(array, Trace.Array)
      return (array.__class__, array.id)

    def objectId(obj):
      assert isinstance(obj, Trace.Object)
      return "%s_%x_%x" % (obj.cls.name.lower(), obj.ns, obj.id)
      
    task   = Task.startTask("c-export", "Formatting source", len(trace.events))
    indent = " " * 3
    
    # Collect all values for all events
    task = Task.startTask("c-export", "Collecting data", len(trace.events))
    values = []
    [values.extend(e.values.values()) for e in trace.events]

    # Collect arrays
    arrays        = OrderedDict([(arrayId(v), v) for v in reversed(values) if isinstance(v, Trace.Array)])
    
    # Check that the external data format is supported
    if dataFileName:
      assert dataFileName.endswith(".s"), "Unsupported external data file type. Use one of: s"
      assert dataFileFormat in ("gcc", "rvct"), "Unsupported external data format. Use one of: gcc, rvct"
      dataFile = open(dataFileName, "w")
    else:
      dataFile = None

    # Calculate maximum sizes of arrays
    arraySizes = dict([(a, 0) for a in arrays.keys()])
    for value in values:
      if isinstance(value, Trace.Array):
        a = arrayId(value)
        arraySizes[a] = max(len(value), arraySizes[a])
    
    # Figure out the C types of arrays and objects
    classes             = {}
    objectTypes         = {}
    arrayTypes          = {}
    outValueObjects     = set()     # Objects that are acquired through a function
    arrayVariants       = DefaultDict(list)
    usePersistentArrays = False     # Use arrays whose contents must be kept up to date
                                    # even after passing them to the API

    def registerObject(event, name, value):
      if not value.cls in classes:
        classes[value.cls] = {}
      function = self.analyzer.lookupFunction(event)
      # Object has already been registered
      if not name or function.parameters[name].isOut:
        outValueObjects.add(value)
      if objectId(value) in classes[value.cls]:
        return
      classes[value.cls][objectId(value)] = value
      for cType, nativeTypeName in library.typeMap.items():
        if cType.name == value.cls.name:
          objectTypes[value] = cType
          break
      else:
        self.analyzer.reportWarning("Unknown class: <%s>" % value.cls.name)
        # Create a fake type name for this class
        objectTypes[value] = value.cls.name

    def registerArray(event, name, value):
      function = self.analyzer.lookupFunction(event)
      
      if name:
        cType = function.parameters[name].type
      else:
        cType = function.type
      
      # Extract an essential type for the array discarding all qualifiers and modifiers
      cType = Library.Type(cType.name)
      
      # Get the real, non-typedef'd type as well
      realType = library.resolveType(cType)
      
      # If this is a void type, use the original type instead
      if realType.name == "void":
        cType = arrayTypeMap[value.__class__]
        
      arrayTypes[arrayId(value)] = cType

      # If this is an object array, register the objects too
      if isinstance(value, Trace.ObjectArrayValue):
        for obj in value:
          registerObject(event, name, obj)

    for event in trace.events:
      # OpenGL needs persistent arrays
      if event.name.startswith("gl"):
        usePersistentArrays = True

      for name, value in event.values.items():
        if isinstance(value, Trace.Object):
          registerObject(event, name, value)
        elif isinstance(value, Trace.Array):
          registerArray(event, name, value)
      
      # Collect the modified arrays for this event
      for array in event.modifiedArrays:
        a = arrayId(array)
        # Only consider the arrays we know about
        if a in arrayTypes:
          arrayVariants[a].append(array)
      task.step()
      
    # Count the number of frames
    if frameMarkers:
      frameCount = len(frameMarkers) + 3
    else:
      frameCount = len([1 for event in trace.events if self.analyzer.lookupFunction(event).isFrameMarker]) + 3
      
    # Add the header
    print >>traceFile, "/**"
    print >>traceFile, " *  C source generated from %d events (%d frames). " % (len(trace.events), frameCount)
    if insertCopyright:
        print >>traceFile, copyrightText
    print >>traceFile, " */"
    print >>traceFile, ""
    print >>traceFile, "/** A macro for copying data into an array */"
    print >>traceFile, "#define LOAD_ARRAY(TO, FROM, LENGTH) \\"
    print >>traceFile, indent, "{ \\"
    print >>traceFile, indent * 2, "int i; \\"
    print >>traceFile, indent * 2, "for (i = 0; i < (LENGTH); i++) \\"
    print >>traceFile, indent * 3, "(TO)[i] = (FROM)[i]; \\"
    print >>traceFile, indent, "}"
    print >>traceFile, ""
    
    # Insert any additional code specified in the configuration
    if "c_player_code" in config:
      for fileName in config["c_player_code"]:
        f = open(config.getRelativePath(fileName))
        print >>traceFile, f.read()
        f.close()

    # Add the header to the data file if we have one
    if dataFile:
      if dataFileFormat == "gcc":
        print >>dataFile, "#"
        print >>dataFile, "# GNU Assembler data file generated from %d events (%d frames). " % (len(trace.events), frameCount)
        print >>dataFile, "#"
        print >>dataFile, ""
        print >>dataFile, ".section .data"
      else: # rvct
        print >>dataFile, ";"
        print >>dataFile, "; RVCT Assembler data file generated from %d events (%d frames). " % (len(trace.events), frameCount)
        print >>dataFile, ";"
        print >>dataFile, ""
        print >>dataFile, "    AREA ||.constdata||, DATA, READONLY, ALIGN=2"

    # Introduce objects
    print >>traceFile, "/* Objects */ "
    for objects in classes.values():
      for obj in objects.values():
        if not obj.id:
          continue
        print >>traceFile, "static %s %s = (%s)0x%x;" % (objectTypes[obj], objectId(obj), objectTypes[obj], obj.id)
    print >>traceFile, ""
    task.step()
    
    # Introduce arrays
    print >>traceFile, "/* %d arrays */ " % len(arrays)
    for i, array in enumerate(arrays.values()):
      a = arrayId(array)
      if usePersistentArrays:
        l = arraySizes[a]
        if not l:
          self.analyzer.reportWarning("Empty array %s" % str(a))
          l = 1
        print >>traceFile, "static %s %s_%s%d[%d];" % (arrayTypes[a], str(arrayTypes[a]).lower(), arrayPrefix, i, l)
      else:
        print >>traceFile, "static %s* %s_%s%d;" % (arrayTypes[a], str(arrayTypes[a]).lower(), arrayPrefix, i)
    print >>traceFile, ""

    # Introduce unique array data
    print >>traceFile, "/* Array data */ "
    arrayData = []
    arrayMap  = {}
    for variants in arrayVariants.values():
      for array in variants:
        # See if an equivalent array is already created
        for j, existingArray in enumerate(arrayData):
          if existingArray == array and \
             existingArray.__class__ == array.__class__:
            arrayMap[id(array)] = j
            break
        else:
          arrayMap[id(array)] = len(arrayData)
          arrayData.append(array)

    if not dataFile:
      # Inline data
      for i, array in enumerate(arrayData):
        if not len(array):
          continue
        # Object arrays can't be initialized inline
        if isinstance(array, Trace.ObjectArrayValue):
          print >>traceFile, "static %s %sData%d[%d];" % (arrayTypes[arrayId(array)], arrayPrefix, i, len(array))
          print >>traceFile, ""
          continue
        elif usePersistentArrays:
          print >>traceFile, "static const %s %sData%d[%d] = {" % (arrayTypes[arrayId(array)], arrayPrefix, i, len(array))
        else:
          print >>traceFile, "static %s %sData%d[%d] = {" % (arrayTypes[arrayId(array)], arrayPrefix, i, len(array))
        print >>traceFile, indent,
        
        # Figure out the proper qualifier for the array elements
        qualifier = ""
        format    = "s"
        if len(array):
          if isinstance(array, Trace.FloatArrayValue):
            format = qualifier = "f"
          elif isinstance(array, Trace.DoubleArrayValue):
            format = qualifier = "d"
          elif isinstance(array, Trace.LongArrayValue):
            format = qualifier = "l"
      
        for k, value in enumerate(array):
          value = ("%%%s%s" % (format, qualifier)) % value
          if k != len(array) - 1:
            print >>traceFile, "%s," % value,
            if not (k + 1) % 8:
              print >>traceFile, ""
              print >>traceFile, indent,
          else:
            print >>traceFile, value
        print >>traceFile, "};"
        print >>traceFile, ""
    else: # External data
      for i, array in enumerate(arrayData):
        if not len(array):
          continue
        if usePersistentArrays and not isinstance(array, Trace.ObjectArrayValue):
          print >>traceFile, "extern const %s %sData%d[%d];" % (arrayTypes[arrayId(array)], arrayPrefix, i, len(array))
        else:
          print >>traceFile, "extern %s %sData%d[%d];" % (arrayTypes[arrayId(array)], arrayPrefix, i, len(array))
        
        # Object arrays can't be initialized inline
        if isinstance(array, Trace.ObjectArrayValue):
          continue

        # Figure out the proper type code for the array elements
        if dataFileFormat == "gcc":
          print >>dataFile,  ".global %sData%d" % (arrayPrefix, i)
          print >>dataFile,  "%sData%d:" % (arrayPrefix, i)
          if isinstance(array, Trace.FloatArrayValue):
            typeCode = ".float"
          elif isinstance(array, Trace.DoubleArrayValue):
            typeCode = ".double"
          elif isinstance(array, Trace.LongArrayValue):
            typeCode = ".quad"
          elif isinstance(array, Trace.ShortArrayValue):
            typeCode = ".short"
          elif isinstance(array, Trace.ByteArrayValue):
            typeCode = ".byte"
          elif isinstance(array, Trace.IntegerArrayValue):
            typeCode = ".int"
          else:
            raise RuntimeError("Unknown array type")

          # Write out the data
          print >>dataFile, "%s %s" % (typeCode, ", ".join(map(str, array)))
        else: # rvct
          print >>dataFile, "GLOBAL %sData%d" % (arrayPrefix, i)
          print >>dataFile, "%sData%d" % (arrayPrefix, i)
          if isinstance(array, Trace.FloatArrayValue):
            typeCode = "DCFS"
          elif isinstance(array, Trace.DoubleArrayValue):
            typeCode = "DCFD"
          elif isinstance(array, Trace.LongArrayValue):
            typeCode = "DCQ"
          elif isinstance(array, Trace.ShortArrayValue):
            typeCode = "DCW"
          elif isinstance(array, Trace.ByteArrayValue):
            typeCode = "DCB"
          elif isinstance(array, Trace.IntegerArrayValue):
            typeCode = "DCD"
          else:
            raise RuntimeError("Unknown array type")

          # Write out the data
          prefix = "    %s " % typeCode
          for j in xrange(0, len(array), 8):
            values = array[j:j + 8]
            print >>dataFile, prefix, ",".join(map(str, values))

    # Initialize the objects
    print >>traceFile, "static void %s(void* %s)" % (initFuncName, playerArgument)
    print >>traceFile, "{"

    def getObjectAttributeValue(attr):
      if isinstance(attr, Trace.Array):
        # Only strings are supported so far
        assert isinstance(attr, Trace.ByteArrayValue)
        s = "".join((chr(c) for c in attr))
        s = s.replace("\r", "\\r")
        s = s.replace("\t", "\\t")
        s = s.rstrip("\x00")
        lines = s.split("\n")
        return "\n".join(('"%s\\n"' % l for l in lines))
      return str(attr)

    for objects in classes.values():
      for obj in objects.values():
        if not obj.id:
          continue
        # If the object has attributes or it wasn't created from a return value, ask the user to create it
        if obj.attrs or not obj in outValueObjects:
          print >>traceFile, indent, "/* %s attributes: %s */" % (obj.cls.name, ", ".join(obj.attrs.keys()))
          if obj.attrs:
            attrs = ", ".join(map(getObjectAttributeValue, obj.attrs.values()))
            print >>traceFile, indent, "%s = create%s%d(%s, %s);" % (objectId(obj), obj.cls.name, len(obj.attrs) + 1, playerArgument, attrs)
          else:
            print >>traceFile, indent, "%s = create%s1(%s);" % (objectId(obj), obj.cls.name, playerArgument)
    print >>traceFile, "}"
    print >>traceFile, ""

    # Uninitialize the objects
    print >>traceFile, "static void %s(void* %s)" % (uninitFuncName, playerArgument)
    print >>traceFile, "{"
    for objects in classes.values():
      for obj in objects.values():
        if not obj.id:
          continue
        # If the object has attributes or it wasn't created from a return value, ask the user to destroy it
        if obj.attrs or not obj in outValueObjects:
          print >>traceFile, indent, "destroy%s2(%s, %s);" % (obj.cls.name, playerArgument, objectId(obj))
    print >>traceFile, "}"
    print >>traceFile, ""
            
    # Add the events
    task.finish()
    task = Task.startTask("c-export", "Generating source", len(trace.events))
    frameNumber = 0
    frameFunctions = ["%s0" % frameFuncName]
    activeArrays = dict([(a, None) for a in arrays.keys()])

    # Open the frame function
    print >>traceFile, "static void %s0(void* %s)" % (frameFuncName, playerArgument)
    print >>traceFile, "{"
    
    for event in trace.events:
      function = self.analyzer.lookupFunction(event)
      
      # Modify objects
      for obj in event.modifiedObjects:
        if not obj.id:
          continue
        # Check the the object was really modified
        if obj.attrs and obj.attrs != classes[obj.cls][objectId(obj)].attrs:
          attrs = ", ".join(map(getObjectAttributeValue, obj.attrs.values()))
          print >>traceFile, indent, "/* %s attributes: %s */" % (obj.cls.name, ", ".join(obj.attrs.keys()))
          print >>traceFile, indent, "%s = modify%s%d(%s, %s, %s);" % \
            (objectId(obj), obj.cls.name, len(obj.attrs) + 2, playerArgument, objectId(obj), attrs)
          classes[obj.cls][objectId(obj)].attrs = obj.attrs

      # Modify arrays
      for array in event.modifiedArrays:
        # Load the correct data into the array
        a            = arrayId(array)
        
        # If this array is not used anywhere, skip it
        if not id(array) in arrayMap:
          continue
        
        toArray      = arrays.index(a)
        fromArray    = arrayMap[id(array)]

        # Don't reload the same data        
        if activeArrays[a] == fromArray:
          continue
          
        # Ignore empty arrays
        if not len(array):
          continue
        
        activeArrays[a] = fromArray
        # Insert new objects directly into the array
        if isinstance(array, Trace.ObjectArrayValue):
          for i, obj in enumerate(array):
            print >>traceFile, indent, "%s_%s%d[%d] = %s;" % \
            (str(arrayTypes[a]).lower(), arrayPrefix, toArray, i, objectId(obj))
        elif usePersistentArrays:
          print >>traceFile, indent, "LOAD_ARRAY(%s_%s%d, %sData%d, %d);" % \
            (str(arrayTypes[a]).lower(), arrayPrefix, toArray, arrayPrefix, fromArray, len(array))
        else:
          print >>traceFile, indent, "%s_%s%d = %sData%d;" % \
            (str(arrayTypes[a]).lower(), arrayPrefix, toArray, arrayPrefix, fromArray)
      
      # Collect the arguments
      args        = []      
      returnValue = None
      for name, value in event.values.items():
        valueType = name and function.parameters[name].type or function.type
        if value is None:
          value = "(%s)0" % valueType
        elif isinstance(value, Trace.Array):
          # If this array can be modified by the function, mark it as lost
          if not valueType.isConstant() and value in event.modifiedArrays:
            a = arrayId(value)
            activeArrays[a] = None
          if not value.id:
            value = "(%s)0" % valueType
          else:
            a         = arrayId(value)
            value     = "(%s)%s_%s%d" % (valueType, str(arrayTypes[a]).lower(), arrayPrefix, arrays.index(a))
        elif isinstance(value, Trace.Object):
          if not value.id:
            # See whether the object really is a pointer or just a basic integer
            if name and library.isPointerType(function.parameters[name].type):
              value = "(%s)NULL" % valueType
            else:
              value = "(%s)0" % valueType
          else:
            value = str(objectId(value))
        elif isinstance(value, Trace.UnknownPhrase):
          value = "(%s)NULL" % valueType
        else:
          value = StringUtils.decorateValue(library, function, name, value)
          if isinstance(value, Trace.FloatValue):
            value = str(value) + "f"
          elif isinstance(value, Trace.DoubleValue):
            value = str(value) + "d"
          elif isinstance(value, Trace.LongValue):
            value = str(value) + "l"

          # Do a cast if this is actually a pointer parameter (e.g. 'ptr' in glVertexAttribPointer)
          if name and library.isPointerType(function.parameters[name].type):
            value = "(%s)%s" % (valueType, value)

          # If the real C type is unsigned and we have a negative value, do a cast
          try:
            if name and "unsigned" in str(library.resolveType(function.parameters[name].type)) and int(value) < 0:
              value = "(%s)%s" % (function.parameters[name].type, value)
          except ValueError:
            # Not an integer
            pass

        # HACK: eglGetDisplay(0) -> eglGetDisplay(EGL_DEFAULT_DISPLAY)
        if event.name == "eglGetDisplay" and name and str(value) == "0":
          value = "EGL_DEFAULT_DISPLAY"

        # Make sure we have a meaningful parameter value
        assert len(str(value))
        
        if name:
          args.append(str(value))
        else:
          returnValue = value
          
      # Truncated event stream?
      if not len(args) == len(function.parameters):
        self.analyzer.reportWarning("Truncated call to %s(%s)" % (event.name, ", ".join(args)))
        print >>traceFile, indent, "/* truncated call to %s(%s) */" % (event.name, ", ".join(args))
        continue

      # Save the return value if needed
      returnObject = event.values.get(None, None)
      if isinstance(returnObject, Trace.Object) and returnObject.id:
        print >>traceFile, indent, "%s =" % returnValue,
      else:
        print >>traceFile, indent,

      args = ", ".join(args)
      print >>traceFile, "%s(%s);" % (event.name, args)

      # Apply modifications to object arrays
      for array in event.modifiedArrays:
        if isinstance(array, Trace.ObjectArrayValue):
          for i, obj in enumerate(array):
            a = arrayId(array)
            fromArray = arrays.index(a)
            print >>traceFile, indent, "%s = %s_%s%d[%d];" % \
            (objectId(obj), str(arrayTypes[a]).lower(), arrayPrefix, fromArray, i)

      if (not frameMarkers and function.isFrameMarker) or event in frameMarkers:
        frameNumber += 1
        name = "%s%d" % (frameFuncName, frameNumber)
        print >>traceFile, "}"
        print >>traceFile, ""
        print >>traceFile, "/**"
        print >>traceFile, " *  Frame #%d" % frameNumber
        print >>traceFile, " */"
        print >>traceFile, "static void %s(void* %s)" % (name, playerArgument)
        print >>traceFile, "{"
        frameFunctions.append(name)
      
      task.step()
    print >>traceFile, "}"
    print >>traceFile, ""

    # Create the playback function
    print >>traceFile, "/**"
    print >>traceFile, " *  Play back all trace frames."
    print >>traceFile, " *  @param %s Optional user data pointer" % (playerArgument)
    print >>traceFile, " */"
    print >>traceFile, "static void %s(void* %s)" % (playFuncName, playerArgument)
    print >>traceFile, "{"
    print >>traceFile, indent, "%s(%s);" % (initFuncName, playerArgument)
    for name in frameFunctions:
      print >>traceFile, indent, "%s(%s);" % (name, playerArgument)
    print >>traceFile, indent, "%s(%s);" % (uninitFuncName, playerArgument)
    print >>traceFile, "}"
    print >>traceFile, ""

    # Create the playback function for single frame playback
    print >>traceFile, "/**"
    print >>traceFile, " *  Play back a single frame of the trace."
    print >>traceFile, " *  @param %s Optional user data pointer" % (playerArgument)
    print >>traceFile, " *  @param frame Zero-based number of frame to play"
    print >>traceFile, " *  @returns 1 if the frame number was valid, 0 otherwise"
    print >>traceFile, " */"
    print >>traceFile, "static int %s(void* %s, int frame)" % (playFrameFuncName, playerArgument)
    print >>traceFile, "{"
    print >>traceFile, indent, "switch (frame)"
    print >>traceFile, indent, "{"
    print >>traceFile, indent * 2, "case %6d: %s(%s); break;" % (0, initFuncName, playerArgument)
    for i, name in enumerate(frameFunctions):
      print >>traceFile, indent * 2, "case %6d: %s(%s); break;" % (i + 1, name, playerArgument)
    print >>traceFile, indent * 2, "case %6d: %s(%s); break;" % (len(frameFunctions) + 1, uninitFuncName, playerArgument)
    print >>traceFile, indent * 2, "default: return 0;"
    print >>traceFile, indent, "}"
    print >>traceFile, indent, "return 1;"
    print >>traceFile, "}"
    
    # Close the data file
    if dataFile:
      dataFile.close()
    
    # All done
    task.finish()
