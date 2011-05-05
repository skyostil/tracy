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

from AnalyzerEnvironment import *
import Common
from common import Collections
import glob

if not args or len(args) < 2:
  Log.notice("Usage: batch filemask1 [filemask2 ...] command")
  Log.notice("Repeat the same command sequence for a number of trace files.")
  Log.notice("")
  Log.notice("The following variables can be used in the command sequence:")
  Log.notice("    $t    The current trace file name")
  exit()
  
masks    = args[:-1]
commands = args[-1]
files    = Collections.flatten(map(glob.glob, masks))

for i, traceFile in enumerate(files):
  Log.notice("Processing trace file %d of %d: %s." % (i + 1, len(files), traceFile))
  # FIXME: quoting
  c = commands.replace("$t", "%s" % traceFile)
  analyzer.execute(c)
