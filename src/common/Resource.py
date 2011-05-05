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

"""Support functions for dealing with shared resource files."""

import os
import fnmatch
from common import Log

# Resource search path
# TODO: Generate this based on installation parameters
paths = [".", "../../"]

def getPath(*name, **args):
  """
  Get a path to an existing resource.
  
  @param *name      Resource path segments
  @param required   If True, a warning is logged if the resource is not found.
  """
  for path in paths:
    p = os.path.join(path, *name)
    if os.path.exists(p):
      return p
  if args.get("required", True) == True:
    Log.warn("Resource '%s' not found." % os.sep.join(name))

def getFiles(*path, **args):
  filter = args.get("filter", "*")
  path = getPath(*path)
  results = []
  for file in os.listdir(path):
    if fnmatch.fnmatch(file, filter):
      results.append(os.path.join(path, file))
  return results

def getWritablePath(appName):
  """
  @returns a path for writable resource files.
  """
  path = "."
  if os.name == "posix":
    path = os.path.expanduser("~/." + appName)
  elif os.name == "nt":
    try:
      path = os.path.join(os.environ["APPDATA"], appName)
    except KeyError:
      pass
  try:
    os.mkdir(path)
  except:
    pass
  return path
