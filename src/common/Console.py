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

"""Console utilities."""

import os
import sys

class Colorizer:
  def setColor(self, r, g, b):
    pass
    
  def resetColor(self):
    pass

colorizer = Colorizer()

if os.name == "nt":
  try:
    import win32console, pywintypes
    class Win32Colorizer(Colorizer):
      def __init__(self):
        self.out = win32console.GetStdHandle(win32console.STD_OUTPUT_HANDLE)
        
      def setColor(self, r, g, b):
        t = 0xff / 3
        c = 0
        if r >= t: c |= win32console.FOREGROUND_RED
        if g >= t: c |= win32console.FOREGROUND_GREEN
        if b >= t: c |= win32console.FOREGROUND_BLUE
        if r >= 2*t or g >= 2*t or b >= 2*t:
          c |= win32console.FOREGROUND_INTENSITY
        try:
          self.out.SetConsoleTextAttribute(c)
        except pywintypes.error:
          pass
          
      def resetColor(self):
        self.setColor(0x80, 0x80, 0x80)
    colorizer = Win32Colorizer()
  except ImportError:
    pass
elif os.name == "posix":
  class AnsiColorizer(Colorizer):
    def __init__(self):
      self.palette = [
        ("0;30", (0x00, 0x00, 0x00)),
        ("0;31", (0x80, 0x00, 0x00)),
        ("0;32", (0x00, 0x80, 0x00)),
        ("0;33", (0x80, 0x80, 0x00)),
        ("0;34", (0x00, 0x00, 0x80)),
        ("0;35", (0x80, 0x00, 0x80)),
        ("0;36", (0x00, 0x80, 0x80)),
        ("0;37", (0x80, 0x80, 0x80)),
        ("1;30", (0x00, 0x00, 0x00)),
        ("1;31", (0xFF, 0x00, 0x00)),
        ("1;32", (0x00, 0xFF, 0x00)),
        ("1;33", (0xFF, 0xFF, 0x00)),
        ("1;34", (0x00, 0x00, 0xFF)),
        ("1;35", (0xFF, 0x00, 0xFF)),
        ("1;36", (0x00, 0xFF, 0xFF)),
        ("1;37", (0xFF, 0xFF, 0xFF)),
      ]
    def setColor(self, r, g, b):
      minDist   = 0xff ** 2 * 3
      colorCode = ""
      for code, color in self.palette:
        dist = sum([(c[1] - c[0]) ** 2 for c in zip(color, [r, g, b])])
        if dist < minDist:
          minDist   = dist
          colorCode = code
      sys.stdout.write("\033[" + colorCode + "m")

    def resetColor(self):
      sys.stdout.write("\033[0m")
  colorizer = AnsiColorizer()

def printProgressBar(task, colorizer, file = sys.stdout, barLength = 15, barColor = (0x0, 0xff, 0x0)):
  if not file.isatty():
    return
  if task.steps:
    f   = float(task.progress) / task.steps
    n   = int(barLength * f)
    bar = ("/" * n) + (" " * (barLength - n))
    file.write("\r[")
    colorizer.setColor(*barColor)
    file.write(bar)
    colorizer.resetColor()
    file.write("] %3d%% - %s\r" % (100 * f, task.description))
  else:
    bar = "".join(["/ "[(((i - (task.progress >> 2)) / 4)) % 2] for i in range(barLength)])
    file.write("\r[")
    colorizer.setColor(*barColor)
    file.write(bar)
    colorizer.resetColor()
    file.write("] - %s\r" % (task.description))
  file.flush()

def eraseProgressBar(file = sys.stdout):
  if not file.isatty():
    return
  # Clear the current line
  file.write("\r" + " " * 79 + "\r")
  file.flush()


def printList( lst ):
    """Prints the items in the given list as a numbered list."""
    for i, c in enumerate( lst ):
        print "%4d. %s" % (i + 1, c)

def chooseIndex( prompt, choices ):
    """
    Shows the items in the choices list and waits for user to select one of
    them. Returns the index of the selected choice element.
    """
    if len( choices ) == 1:
        return 0
    try:
        printList( choices )
        while True:
            try:
                s = raw_input("%s: " % prompt)
                if not s: return
                c = int(s)
                if c > 0 and c <= len(choices):
                    return c - 1
            except ValueError:
                pass
    except KeyboardInterrupt:
        pass

def choose( prompt, choices ):
    """
    Shows the items in the choices list and waits for user to select one of
    them. Returns the selected choice element.
    """
    index = chooseIndex( prompt, choices )
    if index == None:
        return None
    return choices[index]
