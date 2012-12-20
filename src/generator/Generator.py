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
Code generator for targets such as the tracer and trace player.
"""

from Cheetah.Template import Template
import sys
import os
import Common
from common import Log
from common import Config

class GeneratorException(Exception):
  pass

def generate(templates, namespace, outputFile = sys.stdout):
  """
  Process a list of templates and write the result to a file object.
  
  @param templates:       A list of templates to process
  @param namespace:       A dictionary of objects that the templates may access
  @param outputFile:      Output file object.
  """
  def processTemplate(inputFile, outputFile, searchList):
    t = Template(file = inputFile, searchList = searchList)
    outputFile.write(str(t))
    
  def process(templateName):
    processTemplate(open(templateName), outputFile, namespace)
    
  for template in templates:
    try:
      process(template)
    except Exception, e:
      Log.error("Exception while processing template '%s': %s" % (template, e))
      raise

class Generator(object):
  """
  Main code generator.
  """
  def __init__(self, config, platform, libraries, templatePath = "templates"):
    """
    Generator constructor.
    
    @param config:        Configuration object, see L{Parser.parseConfig}.
    @param platform:      Code generation target Platform
    @param libraries:     List of one or more Libraries to generate code from
    @param templatePath:  Path to template files
    """
    assert type(libraries) == list
    self.config       = config
    self.templatePath = templatePath
    self.platform     = platform
    self.libraries    = libraries

  def generate(self, outputFile = sys.stdout):
    raise RuntimeError("The abstract generator should not be used.")

class GeneratorTemplateInterface:
  nativeTypeLogger = {
    "int":        "trIntegerValue",
    "enum":       "trIntegerValue",
    "float":      "trFloatValue",
    "double":     "trDoubleValue",
    "short":      "trShortValue",
    "byte":       "trByteValue",
    "pointer":    "trPointerValue",
    "object":     "trObjectValue",
    None:         "trIntegerValue",
  }

  nativeArrayTypeLogger = {
    "object":     "trObjectArrayValue",
    "pointer":    "trIntegerArrayValue",
    "int":        "trIntegerArrayValue",
    "float":      "trFloatArrayValue",
    "double":     "trDoubleArrayValue",
    "short":      "trShortArrayValue",
    "byte":       "trByteArrayValue",
    "enum":       "trIntegerArrayValue",
  }

  cType = {
    "int":        "TRint",
    "enum":       "TRenum",
    "pointer":    "void",
    "object":     "TRhandle",
    "float":      "TRfloat",
    "double":     "TRdouble",
    "short":      "TRshort",
    "byte":       "TRubyte",
  }
  
  nativeTypeLoader = {
    "int":        "trGetIntegerValue",
    "enum":       "trGetIntegerValue",
    "float":      "trGetFloatValue",
    "double":     "trGetDoubleValue",
    "short":      "trGetShortValue",
    "byte":       "trGetByteValue",
    "short":      "trGetFloatValue",
    "pointer":    "trGetPointerValue",
    "object":     "trGetObjectValue",
    None:         "trGetIntegerValue",
  }
  
  """
  An interface that the tracer templates may use when generating code.
  """
  def __init__(self, platform, config, libraries):
    """
    @param libraries:   Libraries for code generation
    """
    self.libraries = libraries
    self.config    = config
    self.platform  = platform 
    
  def logMessage(self, msg):
    """
    Emit an informational message from a template.
    @param msg Message to log
    """
    #Log.debug(msg)
    pass
  
  def getNativeType(self, library, type):
    """
    Get the native equivalent of a Type instance.
    
    @param library:   Library from which the type is searched
    @param type:      Type instance to convert to native type
    @returns the name of the matching native type or None
    """
    t = library.getNativeType(type)
    
    if not t:
      Log.warn("Type '%s' has no mapping to a native equivalent; defaulting to 'int'." % type.declaration)
      t = "int"
    return t
  
  def isVoidType(self, library, type):
    """
    Determine whether a type corresponds to void.
    
    @param library:   Library from which the type is searched
    @param type:      Type to test
    """
    return self.getNativeType(library, type) == "void"

  def safeName(self, name):
    """
    @returns a C identifier compatible version of string 'name'.
    """
    return name.replace("-", "_").replace(" ", "_")

  def getStatePathIdentifier(self, path):
    """
    Get a unique identifier for a given state path.
    """
    return "TRS_" + "_".join([n.upper() for n in path])
    
  def getClassIdentifier(self, type):
    """
    Get a unique identifier for a given type.
    """
    return "TR_%s_CLSID" % type.name.upper()
    
  def getMangledFunctionName(self, function):
    """
    Get a mangled name for a function as determined by the calling convention
    """
    if self.platform.entryLinkage == "__stdcall" and \
       not int(self.config.get("disable_stdcall_mangling", 0)):
      # TODO: fix this for 64bit
      return "_%s@%d" % (function.name, len(function.parameters) * 4)

    return function.name

  def getMacroParameterList(self, function):
    """
    A comma-separated list of parameters for a given function suitable for passing
    to the TR_CALL macro
    """
    return ", ".join(["%s, %s" % (p.type, p.name) for p in function.parameters.values() if p.name is not None])

  def emitHook(self, name, collection, indent = 1):
    """
    Return code for a function hook from the given collection,
    or a comment describing the hook if no hook implementation was found.
    
    @param name:        Code hook name
    @param collection:  Dictionary of available hook implementations
    @param indent:      Indentation level, zero for no indendation
    """
    indent = " " * (indent * 4)
    if name in collection:
      return collection[name].replace("\n", "\n" + indent)
    return "/* Code hook: %s */" % name
  
  def fail(self, reason):
    raise GeneratorException(reason)
      
class TracerGenerator(Generator):
  """
  Tracer code generator.
  """
  def generate(self, outputFile = sys.stdout):
    templates = [
      "tracer/header.c.tmpl",
      "tracer/state.c.tmpl",
      "tracer/classes.c.tmpl",
      "tracer/functions.c.tmpl",
      "tracer/init.c.tmpl",
    ]
    templates = [os.path.join(self.templatePath, t) for t in templates]
    namespace = [
      GeneratorTemplateInterface(self.platform, self.config, self.libraries),
      {
        "libraries":  self.libraries,
        "platform":   self.platform,
        "config":     self.config,
      }
    ]
    generate(templates, namespace, outputFile)

class PassthroughTracerGenerator(Generator):
  """
  Tracer code generator.
  """
  def generate(self, outputFile = sys.stdout):
    templates = [
      "passthrough-tracer/header.c.tmpl",
      "passthrough-tracer/functions.c.tmpl",
      "passthrough-tracer/init.c.tmpl",
    ]
    templates = [os.path.join(self.templatePath, t) for t in templates]
    namespace = [
      GeneratorTemplateInterface(self.platform, self.config, self.libraries),
      {
        "libraries":  self.libraries,
        "platform":   self.platform,
        "config":     self.config,
      }
    ]
    generate(templates, namespace, outputFile)

class PlayerGenerator(Generator):
  """
  Player code generator.
  """
  def generate(self, outputFile = sys.stdout):
    templates = [
      "player/header.c.tmpl",
      "tracer/state.c.tmpl",
      "player/classes.c.tmpl",
      "player/functions.c.tmpl",
      "player/init.c.tmpl",
    ]
    templates = [os.path.join(self.templatePath, t) for t in templates]
    namespace = [
      GeneratorTemplateInterface(self.platform, self.config, self.libraries),
      {
        "libraries":  self.libraries,
        "platform":   self.platform,
        "config":     self.config,
      }
    ]
    generate(templates, namespace, outputFile)

class CSourcePlayerGenerator(Generator):
  """
  C source code player code generator.
  """
  def generate(self, outputFile = sys.stdout):
    templates = [
      "cplayer/cplayer-symbian.cpp.tmpl",
    ]
    templates = [os.path.join(self.templatePath, t) for t in templates]
    namespace = [
      GeneratorTemplateInterface(self.platform, self.config, self.libraries),
      {
        "libraries":  self.libraries,
        "platform":   self.platform,
        "config":     self.config,
      }
    ]
    generate(templates, namespace, outputFile)

def generateMakefile(outputFile = sys.stdout, templatePath = "templates"):
  templates = [
    "generator-makefile.tmpl"
  ]
  platforms  = ["android", "unix", "symbian", "win32"]
  genTargets = ["tracer", "passthrough_tracer", "player"]
  apis = {
    "egl13":                    "config/egl/v1.3/egl.conf",
    "gles11":                   "config/gles/v1.1/gles.conf",
    "gles20":                   "config/gles/v2.0/gles.conf",
    "vg11":                     "config/vg/v1.1/vg.conf",
    "gles11-egl11":             "config/gles/v1.1/gles-egl.conf",
    "vgu11":                    "config/vgu/v1.1/vgu.conf",
    "gles11-vg101-egl13":       "config/cross-api/gles-vg-egl/gles-vg-egl.conf",
    "gles11-vg11-egl13":        "config/cross-api/gles-vg-egl/gles11-vg11-egl13.conf",
    "gles20-vg11-egl13":        "config/cross-api/gles-vg-egl/gles20-vg11-egl13.conf",
    "gles20-egl13":             "config/cross-api/gles-egl/gles20-egl13.conf",
    "gles11-egl13":             "config/cross-api/gles-egl/gles11-egl13.conf",
    "vg11-vgu11-egl13":         "config/cross-api/vg-vgu-egl/vg11-vgu11-egl13.conf",
    "gles11-vg11-vgu11-egl13":  "config/cross-api/gles-vg-vgu-egl/gles11-vg11-vgu11-egl13.conf",
    "gles20-vg11-vgu11-egl13":  "config/cross-api/gles-vg-vgu-egl/gles20-vg11-vgu11-egl13.conf",
    "vg11-egl13":               "config/cross-api/vg-egl/vg11-egl13.conf",
  }  

  configs = []
  for platform in platforms:
    for api, conf in apis.items():
      configs.append({
        "name": "%s-%s" % (api, platform),
        "deps": os.path.dirname(conf) + "/*",
        "conf": conf,
        "platform": platform,
      })
  targets = []
  for platform in platforms:
    for target in genTargets:
      for api, conf in apis.items():
        targets.append({
          "name": "%s-%s-%s" % (api, target, platform),
          "target": target,
          "conf": conf,
          "api": api,
          "platform": platform,
        })

  templates = [os.path.join(templatePath, t) for t in templates]
  namespace = {
    "configs":    configs,
    "targets":    targets,
    "platforms":  platforms,
    "apis":       apis,
    "genTargets": genTargets,
  }
  generate(templates, namespace, outputFile)
