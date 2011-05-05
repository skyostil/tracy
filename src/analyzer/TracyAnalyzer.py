#!/usr/bin/python
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

"""Tracy analyzer shell."""

import os
import sys
import glob
import Options
import Common
import Analyzer
from common import Project
from common import Log
from common import Resource
from common import Console

def getAvailableProjectFiles():
  projectFiles = sorted(glob.glob(os.path.join(Resource.getPath("lib/tracy"), "*.tcy")))
  projectNames = [os.path.basename(p) for p in projectFiles]
  projectNames = [p.split(".")[0] for p in projectNames]
  return zip(projectFiles, projectNames)

def main(args):
  # Parse the command line options
  options, args = Options.parseCommandLine(args)
  
  if options.verbose:
    Log.quiet = False

  if options.quiet:
    Log.quiet = True
    
  # If a project file was not given, present the possibilities
  if not options.project:
    projectFiles = getAvailableProjectFiles()
    try:
      projectFile = projectFiles[Console.chooseIndex("Choose a project file", [f[1] for f in projectFiles])][0]
    except TypeError:
      return
  else:
    projectFile = options.project

  # Read the project file
  project = Project.Project(options = options, fileName = projectFile)

  # Create the interactive analyzer  
  analyzer = Analyzer.InteractiveAnalyzer(project, options)
  
  if options.execute:
    for command in options.execute:
      try:
        analyzer.execute(command)
      except Analyzer.ExecutionError:
        return 1

  analyzer.run()
  return 0
  
if __name__ == "__main__":
  sys.exit(main(sys.argv[1:]))

