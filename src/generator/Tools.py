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
Interfaces to various external tools.
"""

import subprocess
import os
import Common
from common import Log

class Preprocessor(object):
  """
  The C preprocessor.
  """
  def __init__(self, options, config, platform):
    """
    Constructor.
    
    @param options:   Command line options, see L{optparse}
    @param config:    L{Config} object that holds the settings for the preprocessor.
    @param platform:  Target L{Platform} for the preprocessor.
    """
    self.options  = options
    self.config   = config
    self.platform = platform
    
    self.binary    = "cpp"
    self.arguments = ""
    
    if os.name == "nt":
      # See if we need to use cpp-3 instead of cpp
      try:
        open(".tracy_tmp", "w").close()
        self.process(".tracy_tmp")
      except:
        self.binary = "cpp-3"
      finally:
        os.unlink(".tracy_tmp")

    self.arguments = ""
    if "preprocessor" in self.config:
      self.binary    = self.config.preprocessor.get("binary",    self.binary)
      self.arguments = self.config.preprocessor.get("arguments", self.arguments)

  def process(self, fileName, includeMacros = False):
    """
    Process the macro declarations present in a file and return the resulting string.
    
    @param fileName:       Name of file to parse
    @param includeMacros:  Pass  True to include the preprocessor macros in the output
    """
    cmd = [self.binary]

    if self.arguments:
      cmd.append(self.arguments)
      
    if includeMacros:
      cmd.append("-dD")
      # Don't include predefined macros
      cmd.append("-undef")
    
    for macro, value in self.config.get("macros", {}).items():
      cmd.append("-D%s=%s" % (macro, value))
    if self.platform.name in self.config:
      plat = self.config[self.platform.name]
      for lib in plat.get("includedirs", []):
        cmd.append("-I")
        cmd.append(lib)
      for macro, value in plat.get("macros", {}).items():
        cmd.append("-D%s=%s" % (macro, value))
    cmd.append(fileName)

    if self.options.verbose:
      Log.debug("Running preprocessor: " + " ".join(cmd))
      
    p = subprocess.Popen(cmd, stdout = subprocess.PIPE)
    return p.stdout.read()

class SymbolDecoder(object):
  """
  C++ symbol demangler. This implementation uses the c++filt helper utility
  from GNU Binutils.
  """
  def __init__(self, config):
    """
    Constructor.
    
    @param config:  L{Config} object that holds the settings for the demangler.
    """
    self.config    = config
    self.arguments = ""

    # Do some trickery to find out where c++filt might be
    self.binary    = "c++filt"
    if os.name in ["nt", "cygwin"]:
      try:
        cygpath = subprocess.Popen("cygpath -w -p /", stdout = subprocess.PIPE).stdout.read()[:-1]
        self.binary = os.path.join(cygpath, "bin", "c++filt")
      except:
        pass
   
    if "demangler" in self.config:
      self.binary    = self.config.demangler.get("binary",    self.binary)
      self.arguments = self.config.demangler.get("arguments", self.arguments)

  def decode(self, symbol):
    """
    Demangle a C++ symbol.
    
    @param symbol: Symbol to demangle
    @returns the decoded symbol name
    """
    
    # If we're dealing with a Microsoft C++ mangled name, just extract the function name
    if symbol.startswith("?"):
      return symbol[1:].split("@", 1)[0]

    if os.name in ["nt", "cygwin"]:    
      cmd = [self.binary, self.arguments, "_" + symbol]
    else:
      cmd = [self.binary, self.arguments, symbol]

    try:
      p = subprocess.Popen(cmd, stdout = subprocess.PIPE)
      symbol = p.stdout.read()[:-1]
      if "(" in symbol:
        symbol = symbol.split("(", 1)[0]
      return symbol.strip()
    except:
      raise RuntimeError("The c++filt helper program was not found. Please install GNU Binutils and try again.")

  def isMangled(self, symbol):
    """Determines whether a symbol name is mangled with any of the supported mangling schemes."""
    return symbol.startswith("_Z") or symbol.startswith("?")

def makePath(path):
  # Try creating the directory
  try:
    os.makedirs(path)
  except OSError, err:
    if err.errno != 17: raise
