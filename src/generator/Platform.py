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
import shutil
import Common
from common import Log
from common import Resource
from common import Config
from common import Project
import Generator
import Tools

class Platform(Project.Object):
  """A target platform for code generation."""
  def __init__(self, config, name):
    self.name            = name
    self.requireOrdinals = False
    self.exportLinkage   = None
    self.entryLinkage    = None
    self.language        = None

    # Read the platform-specific configuration
    conf = Resource.getPath("platforms", "%s.conf" % self.name, required = False)
    if conf:
      config.loadFile(conf)
 
  def createBuild(self, config, library, name, targetName):
    raise RuntimeError("The %s platform does not support creating builds." % self.name)
    
class Build(object):
  """A compilation project targeting a platform."""
  def __init__(self, config, library, platform, name, targetName):
    self.config     = config
    self.name       = name
    self.platform   = platform
    self.targetName = targetName
    self.library    = library

  def compile(self, targetPath):
    """
    Compile this build.
    
    @param  targetPath:   Target path for project files
    """
    pass

  def collectItem(self, targetPath, item):
    Tools.makePath(targetPath)
    path = self.config.getRelativePath(item)
    
    if not os.path.exists(path):
      Log.error("Project file '%s' not found." % path)
      return
    
    if not int(item.attrs.get("copy", 1)):
      return path
      
    name = os.path.basename(path)
    shutil.copyfile(path, os.path.join(targetPath, name))
    return name

class SymbianBuild(Build):
  def __init__(self, *args):
    Build.__init__(self, *args)

    # Migrate the target-specific settings
    #if "symbian.%s" % self.targetName in self.config:
    #  self.config.symbian.merge(self.config.symbian[self.targetName])
      
    # Merge the sources and headers
    #for section in ["includes", "systemincludes", "headers", "sources"]:
    #  if not section in self.config:
    #    self.config[section] = Config.Group()
    #  if section in self.config.symbian:
    #    self.config[section].merge(self.config.symbian[section])
    #  if self.targetName in self.config[section]:
    #    self.config[section].merge(self.config[section][self.targetName])
    #  # Delete any subgroups from this section that apply to other targets
    #  for item in self.config[section]:
    #    if isinstance(item, Config.Group):
    #      del self.config[section][item]
  
  def compile(self, targetPath):
    # Collect the source and header files
    sources = []
    for item in self.config.get("sources", []):
      sources.append(self.collectItem(os.path.join(targetPath, "src"), item))

    headers = []
    for item in self.config.get("headers", []):
      if not isinstance(item, Config.Group):
        headers.append(self.collectItem(os.path.join(targetPath, "inc"), item))

    resources = []
    for item in self.config.get("resources", []):
      if not isinstance(item, Config.Group):
        resources.append(self.collectItem(os.path.join(targetPath, "resource"), item))

    # Copy the def files over
    #deffiles = {}
    #for key, item in self.config.get("deffiles", {}).items():
    #  if key == "WINSCW":
    #    arch = "bwins"
    #  elif key == "MARM":
    #    arch = "eabi"
    #  elif key == "GCCE":
    #    arch = "bmarm"
    #  else:
    #    raise RuntimeError("Unknown DEF file architecture: %s" % key)
    #  self.collectItem(os.path.join(targetPath, arch), item)
    #  name = os.path.basename(item)
    #  # Yeah, search me.
    #  name = name.replace("u.def", ".def")
    #  deffiles[key] = name

    #Log.notice("%d source files and %d header files collected." % (len(sources), len(headers)))

    # Prepare the code generator and its namespace
    namespace = {
      "targetName":  self.targetName,
      "projectName": self.name,
      "config":      self.config,
      "library":     self.library,
      "sources":     sources,
      "headers":     headers,
    }
    
    def generate(templatePath, outputPath):
      Log.debug("Generating %s" % (outputPath[-1]))
      Generator.generate(templates = [Resource.getPath(*templatePath)],
                         namespace = namespace,
                         outputFile = open(os.path.join(*outputPath), "w"))
  
    
    # Create the project dirs
    projectPath = os.path.join(targetPath, "group")
    headerPath  = os.path.join(targetPath, "inc")
    Tools.makePath(projectPath)
    Tools.makePath(headerPath)

    # Create the configuration header file
    generate(["templates", "project", "symbian", "tracer_config_symbian.h.tmpl"],
             [headerPath, "tracer_config_symbian.h"])

    # EXE specific files
    if self.config.targettype.lower() == "exe":
      if not "resources" in self.config:
        self.config["resources"] = Config.List()
        
      # Add in the custom resources
      self.config.resources.append(Config.Item(self.name + ".rss"))
      self.config.resources.append(Config.Item(self.name + "_reg.rss", attrs = {"type": "reg"}))
        
      # Create the resource dir
      resourcePath = os.path.join(targetPath, "resource")
      Tools.makePath(resourcePath)
      
      # Create the resource file
      generate(["templates", "project", "symbian", "resource.rss.tmpl"],
               [resourcePath, self.name + ".rss"])
               
      # Create the registration resource file
      generate(["templates", "project", "symbian", "registration.rss.tmpl"],
               [resourcePath, self.name + "_reg.rss"])

      # Create the package file
      generate(["templates", "project", "symbian", "package.pkg.tmpl"],
               [projectPath, self.name + ".pkg"])
               
      # Create the backup registration file
      generate(["templates", "project", "symbian", "backup_registration.xml.tmpl"],
               [projectPath, "backup_registration.xml"])
    else:
      # Create the DEF files
      for arch in ["bwins", "eabi"]:
        defPath = os.path.join(targetPath, arch)
        Tools.makePath(defPath)
        generate(["templates", "project", "symbian", "project.def.tmpl"],
                 [defPath, self.name + "u.def"])

    # Create the bld.inf file
    generate(["templates", "project", "symbian", "bld.inf.tmpl"],
             [projectPath, "bld.inf"])

    # Create the MMP file
    generate(["templates", "project", "symbian", "project.mmp.tmpl"],
             [projectPath, self.name + ".mmp"])

class Win32Build(Build):
  def __init__(self, *args):
    Build.__init__(self, *args)

    # Migrate the target-specific settings
    #if "win32.%s" % self.targetName in self.config:
    #  self.config.win32.merge(self.config.win32[self.targetName])
      
    # Merge the sources and headers
    #for section in ["includes", "systemincludes", "headers", "sources"]:
    #  if not section in self.config:
    #    self.config[section] = Config.Group()
    #  if section in self.config.win32:
    #    self.config[section].merge(self.config.win32[section])
    #  if self.targetName in self.config[section]:
    #    self.config[section].merge(self.config[section][self.targetName])
    #  # Delete any subgroups from this section that apply to other targets
    #  for item in self.config[section]:
    #    if isinstance(item, Config.Group):
    #      del self.config[section][item]
  
  def compile(self, targetPath):
    # Collect the source and header files
    sources = []
    for item in self.config.get("sources", []):
      sources.append(self.collectItem(os.path.join(targetPath, "src"), item))
        
    headers = []
    for item in self.config.get("headers", []):
      if not isinstance(item, Config.Group):
        headers.append(self.collectItem(os.path.join(targetPath, "include"), item))

    # Prepare the code generator and its namespace
    namespace = {
      "targetName":  self.targetName,
      "projectName": self.name,
      "config":      self.config,
      "sources":     sources,
      "headers":     headers,
    }
    
    def generate(templatePath, outputPath):
      Generator.generate(templates = [Resource.getPath(*templatePath)],
                         namespace = namespace,
                         outputFile = open(os.path.join(*outputPath), "w"))
  

    # Create the makefile
    generate(["templates", "project", "win32", "Makefile.tmpl"],
             [targetPath, "Makefile"])

class UnixBuild(Build):
  def __init__(self, *args):
    Build.__init__(self, *args)

    # Migrate the target-specific settings
    #if "unix.%s" % self.targetName in self.config:
    #  self.config.unix.merge(self.config.unix[self.targetName])
      
    # Merge the sources and headers
    #for section in ["includes", "systemincludes", "headers", "sources"]:
    #  if not section in self.config:
    #    self.config[section] = Config.Group()
    #  if section in self.config.unix:
    #    self.config[section].merge(self.config.unix[section])
    #  if self.targetName in self.config[section]:
    #    self.config[section].merge(self.config[section][self.targetName])
    #  # Delete any subgroups from this section that apply to other targets
    #  for item in self.config[section]:
    #    if isinstance(item, Config.Group):
    #      del self.config[section][item]
  
  def compile(self, targetPath):
    # Collect the source and header files
    sources = []
    for item in self.config.get("sources", []):
      sources.append(self.collectItem(os.path.join(targetPath, "src"), item))
        
    headers = []
    for item in self.config.get("headers", []):
      if not isinstance(item, Config.Group):
        headers.append(self.collectItem(os.path.join(targetPath, "include"), item))

    #Log.notice("%d source files and %d header files collected." % (len(sources), len(headers)))
    
    # Prepare the code generator and its namespace
    namespace = {
      "targetName":  self.targetName,
      "projectName": self.name,
      "config":      self.config,
      "sources":     sources,
      "headers":     headers,
    }
    
    def generate(templatePath, outputPath):
      Generator.generate(templates = [Resource.getPath(*templatePath)],
                         namespace = namespace,
                         outputFile = open(os.path.join(*outputPath), "w"))
  

    # Create the makefile
    generate(["templates", "project", "unix", "Makefile.tmpl"],
             [targetPath, "Makefile"])

class AndroidBuild(Build):
  def __init__(self, *args):
    Build.__init__(self, *args)

  def compile(self, targetPath):
    # Collect the source and header files
    sources = []
    for item in self.config.get("sources", []):
      sources.append(self.collectItem(os.path.join(targetPath, "src"), item))

    headers = []
    for item in self.config.get("headers", []):
      if not isinstance(item, Config.Group):
        headers.append(self.collectItem(os.path.join(targetPath, "include"), item))

    # Prepare the code generator and its namespace
    namespace = {
      "targetName":  self.targetName,
      "projectName": self.name,
      "config":      self.config,
      "sources":     sources,
      "headers":     headers,
    }

    def generate(templatePath, outputPath):
      Generator.generate(templates = [Resource.getPath(*templatePath)],
                         namespace = namespace,
                         outputFile = open(os.path.join(*outputPath), "w"))

    # Create build files
    Tools.makePath(os.path.join(targetPath, "jni"))
    generate(["templates", "project", "android", "Android.mk.tmpl"],
             [targetPath, "jni", "Android.mk"])
    generate(["templates", "project", "android", "default.properties.tmpl"],
             [targetPath, "default.properties"])

class SymbianPlatform(Platform):
  """Symbian C++ platform"""
  def __init__(self, config):
    Platform.__init__(self, config, "symbian")
    self.requireOrdinals  = True
    self.exportLinkage    = "__declspec(dllexport)"
    self.entryLinkage     = ""
    self.language         = "c++"

  def createBuild(self, config, library, name, targetName):
    return SymbianBuild(config, library, self, name, targetName)

class SpandexPlatform(Platform):
  def __init__(self, config):
    Platform.__init__(self, config, "spandex")
    self.requireOrdinals  = False
    self.exportLinkage    = "__declspec(dllexport)"
    self.entryLinkage     = ""
    self.language         = "c"
      
class Win32Platform(Platform):
  def __init__(self, config):
    Platform.__init__(self, config, "win32")
    self.requireOrdinals  = False
    self.exportLinkage    = "__declspec(dllexport)"
    self.entryLinkage     = "__stdcall"
    self.language         = "c"

  def createBuild(self, config, library, name, targetName):
    return Win32Build(config, library, self, name, targetName)
    
class UnixPlatform(Platform):
  def __init__(self, config):
    Platform.__init__(self, config, "unix")
    self.requireOrdinals  = False
    self.exportLinkage    = ""
    self.entryLinkage     = ""
    self.language         = "c"

  def createBuild(self, config, library, name, targetName):
    return UnixBuild(config, library, self, name, targetName)

class AndroidPlatform(Platform):
  def __init__(self, config):
    Platform.__init__(self, config, "android")
    self.requireOrdinals  = False
    self.exportLinkage    = ""
    self.entryLinkage     = ""
    self.language         = "c"

  def createBuild(self, config, library, name, targetName):
    return AndroidBuild(config, library, self, name, targetName)

defaultPlatform = "unix"

platforms = {
  "symbian":  SymbianPlatform,
  "spandex":  SpandexPlatform,
  "win32":    Win32Platform,
  "unix":     UnixPlatform,
  "android":  AndroidPlatform,
}
