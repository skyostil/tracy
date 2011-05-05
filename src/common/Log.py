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

import sys
import os
import Console

quiet   = False
#logFile = open("tracy.log", "w")
logFile = None

labels = {
  "warn":   ((0xff, 0xff, 0x00), "(W)"),
  "debug":  ((0x00, 0x00, 0xff), "(D)"),
  "notice": ((0x00, 0xff, 0x00), "(N)"),
  "error":  ((0xff, 0x00, 0x00), "(E)"),
}

def _log(label, msg, file = sys.stdout):
  color, ident = labels[label]
  if not quiet:
    if file.isatty():
      # Clear the current line
      file.write("\r" + " " * 79 + "\r")
      Console.colorizer.setColor(*color)
    file.write(ident)
    if file.isatty():
      Console.colorizer.resetColor()
    file.write(" " + str(msg) + "\n")
  if logFile:
    print >>logFile, labels["warn"] + " " + str(msg)


warn   = lambda msg: _log("warn",   msg)
debug  = lambda msg: _log("debug",  msg)
notice = lambda msg: _log("notice", msg)
error  = lambda msg: _log("error",  msg)
