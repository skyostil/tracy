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

"""Generation targets."""

import tempfile
import os
import shutil
import copy
import Common
from common import Resource
from common import Log
from common import Config
from common import Library
from common import Project
import Generator
import Tools
import Parser

class Target(Project.Object):
  dependencies = []
  
  def __init__(self, project, name, template = None, configName = None):
    """
    Constructor.
    
    @param       project   Project instance which this target is a part of.
    @param       name      Name of this target.
    @param       template  An optional target name to copy the configuration
                           and library from. If not given, the global project
                           configuration and library is used.
    @param     configName  Target configuration file name in the targets directory.
                           A value of None defaults to the target name
    """
    self.project = project
    
    if template:
      assert template in project.targets
      self.config  = project.targets[template].config
      self.library = project.targets[template].library
    else:
      self.config  = project.config
      self.library = project.library
      
    # Create local copies of things we're going to modify
    self.config   = copy.deepcopy(self.config)
    self.library  = copy.deepcopy(self.library)
    self.name     = name
    
    # Read the target-specific configuration
    if configName is None:
      configName = self.name
    
    conf = Resource.getPath("targets", "%s.conf" % configName, required = False)
    if conf:
      self.config.loadFile(conf)

    if self.project.platform.name in self.config:
      # First merge the target config into the platform config
      if self.name in self.config[self.project.platform.name]:
        self.config[self.project.platform.name].merge(self.config[self.project.platform.name][self.name])

      # Then merge the platform config to the global config
      self.config.merge(self.config[self.project.platform.name])

      # Remove the platform specific section since we won't need it anymore
      del self.config[self.project.platform.name]

    if self.name in self.config:
      # Finally merge the target config to the global config
      self.config.merge(self.config[self.name])

      # Remove the target specific section since we won't need it anymore
      del self.config[self.name]

  def generate(self, projectName, targetPath):
    raise RuntimeError("The '%s' target does not support generating anything." % self.name)

  def fail(self, reason):
    msg = "Target '%s' failed: %s" % (self.name, reason)
    Log.error(msg)
    raise RuntimeError(msg)
    
  def getLanguages(self):
    langs = [self.project.platform.language]
    for function in self.library.functions.values():
      if not function.language in langs:
        langs.append(function.language)
    return langs
    
  def addSourceFile(self, fileName):
    if not "sources" in self.config:
      self.config["sources"] = Config.List()
    self.config.sources.append(Config.Item(fileName))

  def serialize(self):
    data = {
      "name":     self.name,
      "config":   self.config,
      "library":  self.library,
    }
    return data
    
  def prepare(self):
    pass
    
  def validate(self):
    pass

  def loadSymbolMap(self):
    config = self.config
    lib    = self.library
    demangler = Tools.SymbolDecoder(config)

    # Set default library name
    if "library" in config:
      for function in lib.functions.values():
        function.libName = config.library

    # Read in the export ordinals
    if "deffiles" in config:
      for defFile in config.deffiles:
        for function, ordinal in Parser.parseDefFile(open(config.getRelativePath(defFile)).read()):
          if function in lib.functions:
            lib.functions[function].exportOrdinal = ordinal
            Log.debug("%s is exported at %d" % (function, ordinal))
          # The function was not found in the library, so let's check whether it is a C++ symbol
          elif demangler.isMangled(function):
            function = demangler.decode(function)
            if function in lib.functions:
              # Save the ordinal and mark the function as C++
              lib.functions[function].exportOrdinal  = ordinal
              lib.functions[function].language = "c++"
              Log.debug("%s is exported at %d" % (function, ordinal))

    # Read the link ordinals and DLL information
    if "symbol_map" in config:
      defaultLibName = None
      for fileName, libName in config.symbol_map.items():
        if fileName == "default":
          defaultLibName = libName
          continue

        # Is it a .DEF file
        if fileName.endswith(".def"):
          for function, ordinal in Parser.parseDefFile(open(config.getRelativePath(fileName)).read()):
            if function in lib.functions:
              lib.functions[function].ordinal = ordinal
              lib.functions[function].libName = libName
              Log.debug("%s is at %s:%d" % (function, libName, ordinal))
            # The function was not found in the library, so let's check whether it is a C++ symbol
            elif demangler.isMangled(function):
              function = demangler.decode(function)
              if function in lib.functions:
                # Save the ordinal and mark the function as C++
                lib.functions[function].ordinal  = ordinal
                lib.functions[function].libName  = libName
                lib.functions[function].language = "c++"
                Log.debug("%s is at %s:%d" % (function, libName, ordinal))
        else: # it's a header file
          assert fileName.endswith(".h")
          for function in lib.functions.values():
            if function.headerName == fileName:
              function.libName = libName
      # Generate passthrough functions for internal symbols
      if defaultLibName:
        assert self.project.platform.requireOrdinals, "Default symbol mapping can only be used in platforms that use symbol ordinals"
        assert "max_internal_ordinal" in config, "max_internal_ordinal must be defined when using a default symbol mapping"
        ordinals = range(1, int(config["max_internal_ordinal"]) + 1)
        for function in lib.functions.values():
          if function.exportOrdinal in ordinals:
            ordinals.remove(function.exportOrdinal)
        for ordinal in ordinals:
          name = "_trInternal%d" % ordinal
          f = Library.Function(name, Library.Type("void"))
          f.language = "c"
          f.ordinal = ordinal
          f.exportOrdinal = ordinal
          f.libName = defaultLibName
          lib.functions[name] = f
          Log.debug("%s is at %s:%d" % (name, defaultLibName, ordinal))
    
class ParserTool(object):
  def __init__(self, target):
    self.cpp    = Tools.Preprocessor(target.project.options, target.config, target.project.platform)
    self.target = target

  def readSource(self, configItem):
    fileName = configItem
    if int(configItem.attrs.get("preprocess", "1")):
      source = self.cpp.process(self.target.config.getRelativePath(fileName), includeMacros = True)
    else:
      source = open(self.target.config.getRelativePath(fileName)).read()
    return source
    
  def loadHooks(self):
    target   = self.target
    config   = self.target.config
    lib      = self.target.library
    platform = self.target.project.platform
    
    # Collect hooks in various locations
    for fileName in config.get("hooks", []):
      Log.notice("Parsing hooks from '%s'." % fileName)
      source = self.readSource(fileName)
      functions = Parser.parseSource(source).functions.values()
      if not functions:
        Log.warn("No hooks found.")
      for f in functions:
        Log.debug("%s %s(%s)" % (f.type, f.name, ", ".join(["%s %s" % (t.type, p) for p, t in f.parameters.items()])))
      for function in functions:
        if not function.body:
          Log.warn("Hook function '%s' has no body." % function.name)
          continue
        if function.name.startswith("@"):
          lib.hooks[function.name] = function.body
          continue
        else:
          try:
            name, hookName = function.name.split(".", 1)
            if not name in lib.functions:
              target.fail("Function '%s' referred by hook function '%s' does not exist." % (name, function.name))
            if not hookName.startswith("@") and not hookName in lib.functions[name].parameters:
              target.fail("Parameter '%s' referred by hook function '%s' does not exist." % (hookName, function.name))
            lib.functions[name].hooks[hookName] = function.body
          except ValueError:
            target.fail("Hook function name '%s' is not valid." % function.name)

def traverseStatePath(config, path):
  node = config.state
  for p in path:
    if not p in node:
      return None
    node = node[p]
  return node
  
class CodeTarget(Target):
  dependencies = []
  
  def __init__(self, project):
    Target.__init__(self, project, "code")
    self.parserTool = ParserTool(self)
    
  def prepare(self):
    # Shorthand for various objects
    config = self.config
    lib    = self.library
    
    # Parse the sources
    for fileName in config.get("apiheaders", []):
      Log.notice("Parsing functions from '%s'." % fileName)
      source = self.parserTool.readSource(fileName)
      newLib = Parser.parseSource(source)
      for f in newLib.functions.values():
        f.headerName = fileName
      for f in newLib.functions.values():
        Log.debug("%s %s(%s)" % (f.type, f.name, ", ".join(["%s %s" % (t.type, p) for p, t in f.parameters.items()])))
      if not newLib.functions:
        Log.warn("No new functions found.")
      else:
        Log.notice("%d functions found." % len(newLib.functions))
      lib.merge(newLib)

    # Load the hooks
    self.parserTool.loadHooks()

    def parseBool(s):
      return bool(int(s))

    # Read the typemap
    for typeDecl, mapping in self.config.types.items():
      attrs = self.config.types[typeDecl].attrs
      name, type = Parser.parseVariableDeclaration(typeDecl + " dummy")
      assert name == "dummy"
      # If this is a class mapping, create the class if it doesn't already exist
      if mapping == "object":
        if not mapping in self.library.classes:
          cls = Library.Class(type)
          if "namespace" in attrs:
            cls.namespacePath = attrs["namespace"].split(".")
          self.library.classes[type] = cls
      # Patch the default decoration hint into all matching types
      if "decorationhint" in attrs:
          for function in self.library.functions.values():
              for t in [p.type for p in function.parameters.values()] + [function.type]:
                  if t == type:
                      t.decorationHint = attrs["decorationhint"]
      self.library.typeMap[type] = str(mapping)

    # Patch in some function-specific attributes
    for function in config.functions.keys():
      if not function in lib.functions:
        self.fail("Attributes specified for non-existent function '%s'." % function)
      attrs = config.functions[function].attrs
      if "terminator" in attrs:
        lib.functions[function].isTerminator         = parseBool(attrs["terminator"])
      if "generate" in attrs:
        lib.functions[function].generate             = parseBool(attrs["generate"])
      if "runtimestate" in attrs:
        lib.functions[function].runtimeStateTracking = parseBool(attrs["runtimestate"])
      if "framemarker" in attrs:
        lib.functions[function].isFrameMarker        = parseBool(attrs["framemarker"])
      if "staticlinkage" in attrs:
        lib.functions[function].staticLinkage        = parseBool(attrs["staticlinkage"])
      if "rendercall" in attrs:
        lib.functions[function].isRenderCall         = parseBool(attrs["rendercall"])
      if "passthrough" in attrs:
        lib.functions[function].passthrough          = parseBool(attrs["passthrough"])
        
      if not isinstance(config.functions[function], Config.Group):
        self.fail("Syntax error: State map definition for function '%s' is missing braces." % function)
        
      # Argument to state mapping
      reservedNames = ["@return", "@modify", "@set", "@get", "@copy"]
      funcAttrs = attrs
      for arg, parameter in config.functions[function].items():
        # Check that this is a valid parameter
        if not arg in reservedNames and not arg in lib.functions[function].parameters:
          self.fail("State mapping for nonexistent parameter '%s' of function '%s' specified." % (arg, function))

        if arg in ["@copy"] and parseBool(funcAttrs.get("runtimestate", "0")):
          Log.warn("Function %s state relation %s not implemented for runtime state tracking." % (function, arg))
          
        # Read the parameter-specific attributes
        attrs = config.functions[function][arg].attrs
        if "decoration" in attrs:
          lib.functions[function].parameters[arg].decoration     = attrs["decoration"]
        if "decorationhint" in attrs:
          lib.functions[function].parameters[arg].decorationHint = attrs["decorationhint"]
        if "out" in attrs:
          lib.functions[function].parameters[arg].isOut          = parseBool(attrs["out"])
        if "object_class" in attrs:
          # Create a function-local type so that this parameter type is an object only for this function
          if arg == "@return":
            type = lib.functions[function].type
          else:
            type = lib.functions[function].parameters[arg].type
          # Override the type's name so that it will refer to the new object class
          # while still using the original C type under the hood
          newType = copy.deepcopy(type)
          newType.isObject = True
          newType.name = attrs["object_class"]
          if arg == "@return":
            lib.functions[function].type = newType
          else:
            lib.functions[function].parameters[arg].type = newType
          # Check that this class exists
          classType = Library.Type(attrs["object_class"])
          if not classType in self.library.classes:
            self.fail("Undefined object class '%s'." % classType)
          
        # Do we have a meta type?
        if "metatype" in config.functions[function][arg]:
          metaGroup = config.functions[function][arg].metatype
          
          try:
            metaType = Library.MetaType(metaGroup.attrs["class"])
          except KeyError:
            self.fail("Meta type for parameter '%s' does not define class." % arg)
            
          # Is this an array parameter?
          if metaType.name == "array":
            metaType.values["size"]       = Library.MetaValue("size",       metaGroup.attrs.get("size", 1))
            if "type" in metaGroup.attrs:
              metaType.values["type"]     = Library.MetaValue("type",       metaGroup.attrs["type"])
              if metaGroup.attrs["type"] == "object":
                if not "object_class" in metaGroup.attrs:
                  self.fail("Required metatype attribute object_class missing")
                metaType.values["object_class"] = Library.MetaValue("object_class", metaGroup.attrs["object_class"])
          # How about an image parameter?
          elif metaType.name == "image":
            metaType.values["stride"]     = Library.MetaValue("stride",     metaGroup.attrs.get("stride", "width"))
            metaType.values["height"]     = Library.MetaValue("height",     metaGroup.attrs.get("height", "height"))
            metaType.values["components"] = Library.MetaValue("components", metaGroup.attrs.get("components", "1"))
            metaType.values["type"]       = Library.MetaValue("type",       metaGroup.attrs.get("type", "byte"))
          else:
            self.fail("Unknown meta type class '%s'." % metaclass)
          
          Log.debug("Meta type: %s.%s: %s" % (function, arg, metaType.name))
            
          # Get the conditions for different meta values
          if isinstance(metaGroup, Config.List):
            for item in metaGroup:
              predicate      = item.attrs["condition"]
              predicateValue = item.attrs["value"]
              result         = item.attrs["result"]
              metaType.values[item].addPredicate(predicate, predicateValue, result)
              Log.debug("Meta type condition: If %s is %s, then %s = %s" % (predicate, predicateValue, item, result))
          elif isinstance(metaGroup, Config.Group):
            Log.error("Meta type variations for parameter '%s' represented in a group instead of a list." % arg)
              
          # Record the meta type
          lib.functions[function].parameters[arg].metaType = metaType
        
        # Is this a short-hand state mapping?
        try:
          path = parameter.split(".")
        except AttributeError:
          # Try the expanded form of a nested attribute set
          try:
            path = (config.functions[function][arg].state).split(".")
          except AttributeError:
            path = []

        # Check that we even have a state structure
        if path and not "state" in config:
          Log.warn("State structure not defined.")
          continue
            
        # Parse special state mapping relations
        relation   = None
        checkPaths = []
        if arg == "@copy":
          relation   = Library.StateRelationCopy(attrs["src"].split("."), attrs["dest"].split("."))
          checkPaths = [relation.sourcePath, relation.destPath]
        elif arg == "@get":
          relation   = Library.StateRelationGet(path)
          checkPaths = [relation.path]
        elif arg == "@set":
          relation   = Library.StateRelationSet(path)
          checkPaths = [relation.path]
        elif arg == "@modify":
          relation   = Library.StateRelationModify(path)
          checkPaths = [relation.path]
        # Empty mapping?
        elif not "".join(path):
          continue
          
        if relation:
          for path in checkPaths:
            if traverseStatePath(config, path) is None:
              self.fail("Relation state path '%s' for function '%s' does not exist." % (".".join(path), function))
          Log.debug("State relation: %s %s" % (function, relation))
          lib.functions[function].stateRelations.append(relation)
          continue
          
        Log.debug("State mapping: %s.%s -> %s" % (function, arg, ".".join(path)))

        # Determine the parameter type
        type = None

        if arg == "@return":
          type = lib.functions[function].type
        else:
          type = lib.functions[function].parameters[arg].type      

        # If this is a runtime mapping, check that the parameter is of a supported type
        if lib.functions[function].runtimeStateTracking and type and \
           lib.getNativeType(type) in ["float", "double"]:
          self.fail("Values of type '%s' can not be saved to the runtime state tree" % type)
          continue
          
        node = traverseStatePath(config, path)
        if node is None:
          self.fail("State path '%s' for function '%s' does not exist." % (".".join(path), function))
          
        # Save the mapping
        if arg == "@return":
          lib.functions[function].retStateRelation = Library.StateRelationSet(path)
        else:
          lib.functions[function].parameters[arg].stateRelation = Library.StateRelationSet(path)
    
class TracerTarget(Target):
  dependencies = ["code"]
  
  def __init__(self, project):
    Target.__init__(self, project, "tracer", template = "code")
    self.parserTool = ParserTool(self)

  def prepare(self):
    # Shorthand for various objects
    lib    = self.library
    config = self.config
    
    # Load the hooks
    self.parserTool.loadHooks()
    
    # Read in the ordinals
    self.loadSymbolMap()
  
  def validate(self):
    lib = self.library

    # Validate the target
    for function in lib.functions.values():
      if function.ordinal is None and function.staticLinkage and self.project.platform.requireOrdinals:
        Log.warn("Function '%s' does not have a valid ordinal." % function.name)
      for parameter in function.parameters.values():
        if lib.getNativeType(parameter.type) == "pointer" \
           and not parameter.name in lib.functions[function.name].hooks \
           and not parameter.metaType \
           and lib.functions[function.name].generate:
          Log.warn("Saving naked pointer '%s' of type '%s' in function '%s'." % (parameter.name, parameter.type, function.name))

  def generate(self, projectName, targetPath):
    tempPath = tempfile.mkdtemp()
    try:
      # Create a build
      build = self.project.platform.createBuild(self.config, self.library, projectName, self.name)
        
      # If there is at least one C++ function, generate only C++ code
      if "c++" in self.getLanguages() or self.config.get("language", "c") == "c++":
        tracerSourceName       = "tracer_gen.cpp"
      else:
        tracerSourceName       = "tracer_gen.c"
        self.library.language  = "c"
        
      # Generate the tracer
      tracerSourcePath = os.path.join(tempPath, tracerSourceName)
      g = Generator.TracerGenerator(self.config, self.project.platform, [self.library],
                                    templatePath = Resource.getPath("templates"))
      output = open(tracerSourcePath, "w")
      g.generate(outputFile = output)
      output.close()
  
      # Add the generated source file to the configuration
      self.addSourceFile(tracerSourcePath)
      
      # Finalize the target
      build.compile(targetPath = targetPath)
    finally:
      try:
        shutil.rmtree(tempPath)
      except:
        Log.warn("Unable to clean up temporary directory '%s'" % tempPath)

class PassthroughTracerTarget(Target):
  dependencies = ["code"]
  
  def __init__(self, project):
    Target.__init__(self, project, "passthrough_tracer", template = "code")
    self.parserTool = ParserTool(self)

  def prepare(self):
    # Shorthand for various objects
    lib    = self.library
    config = self.config
    
    # Load the hooks
    self.parserTool.loadHooks()
    
    # Read in the ordinals
    self.loadSymbolMap()

  def validate(self):
    lib = self.library

    # Validate the target
    for function in lib.functions.values():
      if function.ordinal is None and function.staticLinkage and self.project.platform.requireOrdinals:
        Log.warn("Function '%s' does not have a valid ordinal." % function.name)
  
  def generate(self, projectName, targetPath):
    tempPath = tempfile.mkdtemp()
    try:
      # Create a build
      build = self.project.platform.createBuild(self.config, self.library, projectName, self.name)
        
      # If there is at least one C++ function, generate only C++ code
      if "c++" in self.getLanguages() or self.config.get("language", "c") == "c++":
        tracerSourceName       = "tracer_gen.cpp"
      else:
        tracerSourceName       = "tracer_gen.c"
        self.library.language  = "c"
        
      # Generate the tracer
      tracerSourcePath = os.path.join(tempPath, tracerSourceName)
      g = Generator.PassthroughTracerGenerator(self.config, self.project.platform, [self.library],
                                               templatePath = Resource.getPath("templates"))
      output = open(tracerSourcePath, "w")
      g.generate(outputFile = output)
      output.close()
  
      # Add the generated source file to the configuration
      self.addSourceFile(tracerSourcePath)
      
      # Finalize the target
      build.compile(targetPath = targetPath)
    finally:
      try:
        shutil.rmtree(tempPath)
      except:
        Log.warn("Unable to clean up temporary directory '%s'" % tempPath)

class PlayerTarget(Target):
  dependencies = ["code"]
  
  def __init__(self, project):
    Target.__init__(self, project, "player", template = "code")
    self.parserTool = ParserTool(self)
    
  def prepare(self):
    # Load the hooks
    self.parserTool.loadHooks()
  
  def generate(self, projectName, targetPath):
    tempPath = tempfile.mkdtemp()
    try:
      # Create a build
      build = self.project.platform.createBuild(self.config, self.library, projectName, self.name)
      
      # If there is at least one C++ function, generate only C++ code
      if "c++" in self.getLanguages() or self.config.get("language", "c") == "c++":
        playerSourceName       = "player_gen.cpp"
      else:
        playerSourceName       = "player_gen.c"
        self.library.language  = "c"
      
      # Generate the player
      playerSourcePath = os.path.join(tempPath, playerSourceName)
      g = Generator.PlayerGenerator(self.config, self.project.platform, [self.library],
                                    templatePath = Resource.getPath("templates"))
      output = open(playerSourcePath, "w")
      g.generate(outputFile = output)
      output.close()
  
      # Add the generated source file to the configuration
      self.addSourceFile(playerSourcePath)
      
      # Finalize the build
      build.compile(targetPath = targetPath)
    finally:
      try:
        shutil.rmtree(tempPath)
      except:
        Log.warn("Unable to clean up temporary directory '%s'" % tempPath)
  
class SpandexPlayerTarget(Target):
  dependencies = ["code"]

  def __init__(self, project):
    Target.__init__(self, project, "spandexplayer", template = "code", configName = "player")
    self.parserTool = ParserTool(self)
  
  def prepare(self):
    # Load the hooks
    self.parserTool.loadHooks()
  
  def generate(self, projectName, targetPath):
    # If there is at least one C++ function, generate only C++ code
    if "c++" in self.getLanguages() or self.config.get("language", "c") == "c++":
      playerSourceName       = "%s.cpp" % projectName
    else:
      playerSourceName       = "%s.c" % projectName
      self.library.language  = "c"

    Tools.makePath(targetPath)
    
    # Generate the player
    playerSourcePath = os.path.join(targetPath, playerSourceName)
    g = Generator.PlayerGenerator(self.config, self.project.platform, [self.library],
                                  templatePath = Resource.getPath("templates"))
    output = open(playerSourcePath, "w")
    g.generate(outputFile = output)
    output.close()

class CSourcePlayerTarget(Target):
  dependencies = ["code"]

  def __init__(self, project):
    Target.__init__(self, project, "player", template = "code")
    self.parserTool = ParserTool(self)
  
  def prepare(self):
    # Load the hooks
    self.parserTool.loadHooks()

  def generate(self, projectName, targetPath):
    tempPath = tempfile.mkdtemp()
    try:
      # Create a build
      build = self.project.platform.createBuild(self.config, self.library, projectName, self.name)
      
      # If there is at least one C++ function, generate only C++ code
      if "c++" in self.getLanguages() or self.config.get("language", "c") == "c++":
        playerSourceName       = "player_gen.cpp"
      else:
        playerSourceName       = "player_gen.c"
        self.library.language  = "c"
      
      # Generate the player
      playerSourcePath = os.path.join(tempPath, playerSourceName)
      g = Generator.CSourcePlayerGenerator(self.config, self.project.platform, [self.library],
                                           templatePath = Resource.getPath("templates"))
      output = open(playerSourcePath, "w")
      g.generate(outputFile = output)
      output.close()
  
      # Add the generated source file to the configuration
      self.addSourceFile(playerSourcePath)
      
      # Finalize the build
      build.compile(targetPath = targetPath)
    finally:
      try:
        shutil.rmtree(tempPath)
      except:
        Log.warn("Unable to clean up temporary directory '%s'" % tempPath)

# Build targets
targets = {
  "code":               CodeTarget,
  "tracer":             TracerTarget,
  "player":             PlayerTarget,
  "spandexplayer":      SpandexPlayerTarget,
  "cplayer":            CSourcePlayerTarget,
  "passthrough_tracer": PassthroughTracerTarget,
}
