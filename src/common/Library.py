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
A class hierarchy that represents the functions in an API.
"""

import common

class Type(object):
  """A type declaration for a Variable or a Function."""
  def __init__(self, declaration, dimension = None):
    self.declaration       = declaration
    self.dimension         = dimension
    self.modifiers         = []
    self.qualifiers        = []
    self.symbolicConstants = {}
    self.name              = None
    self.isObject          = False          # Does this type refer to an object?
    if declaration:
      for part in declaration.split(" "):
        if part in ["signed", "unsigned"]:
          self.modifiers.append(part)
        elif part in ["const", "volatile"]:
          self.qualifiers.append(part)
        elif part not in ["*", "&"]:
          self.name = part
    self._hash             = hash(self.name) ^ hash(tuple(self.dimension or []))

    # Type of decoration this variable receives when shown to the user, 
    # e.g. "enum", "bitfield"
    self.decoration     = None

    # Hint for the decoration, e.g. a regular expression that limits the allowed
    # bitfield constants
    self.decorationHint = None
    
  def addSymbolicConstant(self, name, value):
    """
    Add a symbolic constant (enumeration) for this type.
    
    @param name:    Name of symbolic constant
    @param value:   Constant  value
    """
    self.symbolicConstants[name] = value
    
  def __hash__(self):
    #return hash(self.name) ^ hash(tuple(self.dimension or []))
    return self._hash
    
  def __eq__(self, t):
    return self.declaration == t.declaration and self.dimension == t.dimension

  def __repr__(self):
    if self.dimension is not None:
      #return self.declaration + "".join(["[%s]" % d for d in self.dimension])
      return self.declaration + "*" * len(self.dimension)
    return self.declaration
    
  def isPointer(self):
    """
    Indicates whether or not this Type is a pointer instead of an integral type
    """
    return "*" in self.declaration or self.dimension is not None

  def getDereferencedType(self):
    """
    @returns the dereferenced Type of this pointer. This Type must be a pointer.
    """
    assert self.isPointer()
    return Type(" ".join(self.qualifiers + self.modifiers + [self.name]))

  def isConstant(self):
    return self.declaration.startswith("const ")

class Variable(object):
  """A single variable."""
  def __init__(self, name, type):
    # Variable name
    self.name       = name

    # Variable Type association
    self.type       = type

class MetaType(object):
  """A special type for a parameter, such as an array or an image."""
  def __init__(self, name):
    self.name   = name
    self.values = {}

class MetaValue(object):
  """A special attribute of a Parameter, e.g. an image width or height for image parameters."""
  def __init__(self, name, default = None):
    self.name       = name
    
    # The default value
    self.default    = default
    
    # Conditional values are listed as (predicate_name, predicate_value, this_meta_value) tuples
    self.predicates = []
  
  def addPredicate(self, predicate, predicateValue, value):
    self.predicates.append((predicate, predicateValue, value))

class StateRelation(object):
  """A state relation describes how a function or a parameter for a 
     function affects the library state structure.
  """
  pass
  
class StateRelationModify(StateRelation):
  def __init__(self, path):
    self.path = path

class StateRelationGet(StateRelation):
  def __init__(self, path):
    self.path = path

class StateRelationSet(StateRelation):
  def __init__(self, path):
    self.path = path

class StateRelationCopy(StateRelation):
  def __init__(self, sourcePath, destPath):
    self.destPath   = destPath
    self.sourcePath = sourcePath

class Parameter(Variable):
  """A parameter for a Function."""
  def __init__(self, name, type):
    Variable.__init__(self, name, type)
    self.metaType      = None                         # MetaType instance or None
    self.stateRelation = None                         # StateRelation instance or None
    self.isTerminal    = False                        # Does the state path point to a terminal?
    self.isOut         = False                        # Is this parameter modified by the function?

class Function(object):
  """A single function in a Library."""
  def __init__(self, name, type):
    self.name                 = name                  # Function name
    self.type                 = type                  # Function return type
    self.parameters           = common.OrderedDict()  # Function parameters
    self.libName              = None                  # Link library name (DLL) or None
    self.headerName           = None                  # C header name or None
    self.exportOrdinal        = None                  # Ordinal to export at
    self.ordinal              = None                  # Ordinal to link to
    self.body                 = None                  # Code for the function body
    self.linkage              = None                  # C linkage specification
    self.isTerminator         = False                 # Does the function uninitialize the API?
    self.isFrameMarker        = False                 # Does the function cause a frame swap?
    self.isRenderCall         = False                 # Does the function do rendering?
    self.passthrough          = False                 # Should event logging be skipped for this function?
    self.generate             = True                  # Should any code be generated for this function?
    self.language             = None                  # Function interface language
    self.runtimeStateTracking = False                 # Should the state table be updated during runtime?
    self.staticLinkage        = True                  # Is the function statically linked, i.e. has a valid symbol/ordinal
    self.hooks                = {}                    # Code hooks for this function
    self.statePath            = None                  # Function return value state path
    self.isTerminal           = False                 # Does the state path point to a terminal?
    self.retStateRelation     = None                  # State relation for the return value
    self.stateRelations       = []                    # A list of StateRelation mappings for this function

class Class(object):
  """A class of objects."""
  def __init__(self, name):
    self.name          = name
    self.namespacePath = None
    self.overridable   = False

class Library(object):
  """A collection of functions."""
  def __init__(self, name = None, id = 0):
    self.name         = name
    self.id           = id
    self.functions    = common.OrderedDict()
    self.classes      = common.OrderedDict()
    self.typeDefs     = common.OrderedDict()
    self.hooks        = {}
    self.language     = None
    self.constants    = {}
    
    # Map all C types to native types
    self.typeMap    = {
      Type("void"):             "void",
      Type("char"):             "byte",
      Type("short"):            "short",
      Type("int"):              "int",
      Type("long"):             "long",
      Type("float"):            "float",
      Type("double"):           "double",
      Type("enum"):             "int",
    }
    
  def resolveType(self, type, ignoreModifiersAndQualifiers = True):
    """
    Determine the final type of a possibly intermediate type by
    processing type definitions.
    
    @param type:    Type instance to resolve.
    @returns the final Type.
    """
    
    if not ignoreModifiersAndQualifiers:
      while type in self.typeDefs:
        type = self.typeDefs[type]
    else:
      while True:
        for t, n in self.typeDefs.items():
          if t.name == type.name or t.declaration == type.declaration:
            type = n
            break
        else:
          break
      
    return type
    
  def isPointerType(self, type):
    """
    Determine whether a given type is compiled to a pointer type.
    
    @param type:    Type instance examine.
    @retval True type is a pointer
    @retval True type is not a pointer
    """
    return type.isPointer() or self.resolveType(type).isPointer()
    
  def isObjectType(self, type):
    """
    Determine whether a given type is compiled to an object type.
    
    @param type:    Type instance examine.
    @retval True type is an object
    @retval True type is not an object
    """
    return type.isObject or self.resolveType(type).isObject

  def isIntegralType(self, type):
    """
    Determine whether a given type is compiled to an integral type
    
    @param type:    Type instance examine.
    @retval True type is an integral type
    @retval True type is not an integral type
    """
    return self.resolveType(type).declaration.split(" ")[-1] in ["char", "short", "int", "long"]

  def getNativeType(self, type, ignoreModifiersAndQualifiers = True):
    """
    Get the native equivalent of a Type instance.
    
    @param type:      Type instance to convert to native type
    @returns the name of the matching native type or None
    """
    
    def lookup(type):
      if not ignoreModifiersAndQualifiers:
        return self.typeMap[type]
      else:
        for t, n in self.typeMap.items():
          if t.name == type.name:
            return n
        raise KeyError(type)

    # Is it an object?
    if self.isObjectType(type):
      return "object"

    # Check whether there's an exact mapping for this type
    try:
      return self.typeMap[type]
    except KeyError:
      pass

    # Is it a pointer?
    if self.isPointerType(type):
      return "pointer"
    
    # Resolve any type definitions
    type = self.resolveType(type, ignoreModifiersAndQualifiers)

    # Try again with the resolved type
    try:
      return lookup(type)
    except KeyError:
      return None

  def merge(self, library):
    """
    Merge the functions and other definitions found in another library into this library.
    
    @param library: Source library for new functions and definitions
    """
    
    # Update constants
    for name, value in library.constants.items():
      if name in self.constants and value != self.constants[name]:
        raise ValueError("Constant '%s' value '%s' does not match previously defined value '%s'" % (name, value, self.constants[name]))
      self.constants[name] = value

    # Update typedefs
    for name, value in library.typeDefs.items():
      if name in self.typeDefs and value.declaration != self.typeDefs[name].declaration:
        raise ValueError("Type definition of '%s' as '%s' does not match previously defined '%s'" % (name, value, self.typeDefs[name]))
      self.typeDefs[name] = value

    # Update functions
    for f in library.functions.values():
      # If the function is already in the library, make sure the signature matches
      if f.name in self.functions:
        f2 = self.functions[f.name]
        if f.type.declaration != f2.type.declaration:
          raise ValueError("%s: Return type '%s' of function '%s' does not match previously declared '%s'." % \
                          (where(pos), f.type.declaration, f.name, f2.type.declaration))
        for p1, p2 in zip(f.parameters.values(), f2.parameters.values()):
          if p1.type.declaration != p2.type.declaration:
            raise ValueError("%s: Type '%s' of function '%s' parameter '%s' does not match previously declared '%s'." % \
                            (where(pos), p1.type.declaration, f.name, p1.name, p2.type.declaration))
      self.functions[f.name] = f
