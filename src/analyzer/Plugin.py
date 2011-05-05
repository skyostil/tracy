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

"""Analyzer plugin."""

import os
import sys
import Common
from common import Resource
from common import Log

class Plugin(object):
  """
  A plugin that provides extended functionality for the analyzer tool.
  """
  def __init__(self, analyzer):
    self.analyzer = analyzer

class ImporterPlugin(Plugin):
  """
  A plugin that can load trace files using a specific format.
  """
  formatName = None

  def recognizeTraceFile(self, traceFileName):
    """
    Determine whether a trace file is of a format this plugin can understand.
    
    @param traceFileName: Name of file to examine
    
    @returns True if the file is recognized, False otherwise
    """
    pass
  
  def loadTrace(self, traceFile):
    """
    Load a trace from a file object.
    
    @param traceFile:   File object that contains the trace
    
    @returns the loaded trace
    """
    pass

class ExporterPlugin(Plugin):
  """
  A plugin that can save trace files using a specific format.
  """
  formatName = None
  
  def saveTrace(self, trace, traceFile):
    """
    Save a trace to a file object.
    
    @param trace:       Trace to save
    @param traceFile:   Output file object
    """
    pass

class InteractivePlugin(Plugin):
  """
  A plugin that provides commands for the interactive analyzer.
  """
  
  # Helper functions for showing messages to the user
  def reportInfo(self, msg):
    self.analyzer.reportInfo(msg)

  def reportWarning(self, msg):
    self.analyzer.reportWarning(msg)

  def reportError(self, msg):
    self.analyzer.reportError(msg)
    
  def reportDebug(self, msg):
    self.analyzer.reportDebug(msg)

def getAvailablePluginModules(pluginPath = None):
  """
  Determine the available plugin modules on the system.
  
  @returns a list of found plugins
  """
  if pluginPath is None:
    pluginPath = Resource.getPath("plugins", required = True)
    
  if pluginPath is None:
    return []
    
  if not pluginPath in sys.path:
    sys.path.append(pluginPath)
    
  plugins = []
  for name in os.listdir(pluginPath):
    try:
      if os.path.isfile(os.path.join(pluginPath, name, "__init__.py")):
        plugins.append(__import__(name))
    except ImportError, e:
      Log.error("Unable to load plugin %s: %s" % (name, e))
  return plugins

def _loadPlugins(analyzer, plugin, pluginClass):
  assert type(plugin) == type(sys)
  
  # Check that we fullfil the plugin's project requirements
  try:
    if not analyzer.project.config.name.lower().startswith(plugin.requiredLibrary):
      return []
  except AttributeError:
    pass
  
  try:
    plugins = plugin.plugins
  except AttributeError:
    return []
  return [cls(analyzer) for cls in plugins if issubclass(cls, pluginClass)]
  
def loadInteractivePlugins(analyzer, plugin):
  """
  Load and register interactive plugins from a plugin module.
  
  @param analyzer TracyAnalyzer instance
  @param plugin   Plugin module to use
  
  @returns the list of loaded plugins
  """
  return _loadPlugins(analyzer, plugin, InteractivePlugin)

def loadImporterPlugins(analyzer, plugin):
  """
  Load and register importer plugins from a plugin module.
  
  @param analyzer TracyAnalyzer instance
  @param plugin   Plugin module to use
  
  @returns the list of loaded plugins
  """
  return _loadPlugins(analyzer, plugin, ImporterPlugin)

def loadExporterPlugins(analyzer, plugin):
  """
  Load and register exporter plugins from a plugin module.
  
  @param analyzer TracyAnalyzer instance
  @param plugin   Plugin module to use
  
  @returns the list of loaded plugins
  """
  return _loadPlugins(analyzer, plugin, ExporterPlugin)
