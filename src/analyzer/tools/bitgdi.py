# BITGDI analyzer
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
import TraceOperations
import Common
import StringIO
import os
from common import Collections

displayModeNames = {
  0: "ENone",
  1: "EGray2",
  2: "EGray4",
  3: "EGray16",
  4: "EGray256",
  5: "EColor16",
  6: "EColor256",
  7: "EColor64K",
  8: "EColor16M",
  9: "ERgb",
  10: "EColor4K",
  11: "EColor16MU",
  12: "EColor16MA",
  13: "EColorLast",
}

compressionModeNames = {
  0: "ENoBitmapCompression",
  1: "EByteRLECompression",
  2: "ETwelveBitRLECompression",
  3: "ESixteenBitRLECompression",
  4: "ETwentyFourBitRLECompression",
  5: "EThirtyTwoUBitRLECompression",
  6: "EThirtyTwoABitRLECompression",
  7: "ERLECompressionLast",
}

def bitmapDescription(bitmap):
  a = bitmap.attrs
  return "%dx%d/%s/%s" % (a["width"], a["height"], displayModeNames[a["mode"]], compressionModeNames[a["compression"]])

class Histogram(object):
  def __init__(self, items = None):
    self.freq = Collections.DefaultDict(lambda: 0)
    if items:
      for item in items:
        self.add(item)
  
  def add(self, value):
    self.freq[value] += 1
    
  def __len__(self):
    return len(self.freq)
    
  def __repr__(self):
    values  = [(count, name) for name, count in self.freq.items()]
    s = []
    for i, value in enumerate(sorted(values, reverse = True)):
      count, name = value
      s += ["%4d. %8d - %s" % (i + 1, count, name)]
    return "\n".join(s)

class Context(object):
  def __init__(self, id):
    self.id               = id
    self.targets          = []
    self.blits            = []
    self.texts            = []
    self.events           = []
    self.userDisplayModes = []
    
  def __repr__(self):
    s = StringIO.StringIO()
    
    print >>s, box("CFbsBitGc context 0x%x" % self.id)
    print >>s
    print >>s, "Rendering targets (%d):" % len(self.targets)
    print >>s, Histogram(self.targets)
    print >>s
    print >>s, "Rendering operations (%d):" % len(self.events)
    print >>s, Histogram([event.name for event in self.events])
    print >>s
    if self.blits:
      print >>s, "Blits (%d):" % len(self.blits)
      print >>s, Histogram(self.blits)
      print >>s
    if self.texts:
      print >>s, "Text renders (%d):" % len(self.texts)
      print >>s, Histogram(self.texts)
      print >>s
    if self.userDisplayModes:
      print >>s, "User display modes (%d):" % len(self.userDisplayModes)
      print >>s, Histogram(self.userDisplayModes)
      print >>s
    return s.getvalue()

def box(text):
  s = StringIO.StringIO()
  print >>s, "." + "-" * 78 + "."
  print >>s, "| %-76s |" % text
  print >>s, "`" + "-" * 78 + "'",
  return s.getvalue()
    
def histogram(trace):
  s = StringIO.StringIO()
  print >>s, "Call histogram:"  
  hist = TraceOperations.calculateCallHistogram(trace)
  values  = [(count, name) for name, count in hist.items()]

  for i, value in enumerate(sorted(values, reverse = True)):
    count, name = value
    print >>s, "%4d. %8d - %s" % (i + 1, count, name)
  print >>s
  return s.getvalue()

for traceName, trace in traces.items():
  analyzer.reportInfo("Processing %s" % traceName)
  
  if traceName in analyzer.traceFiles:
    tracePath = os.path.dirname(analyzer.traceFiles[traceName])
    traceName = os.path.splitext(os.path.basename(analyzer.traceFiles[traceName]))[0]
  else:
    tracePath = "."

  output = open(os.path.join(tracePath, traceName + "-bitgdi-report.txt"), "w")

  print >>output, box('BITGDI report for "%s"' % traceName)
  print >>output
  print >>output, histogram(trace)
  
  contexts = {}
  for event in trace.events:
    if not "this" in event.values:
      continue
      
    id = event.values["this"].id
    if not id in contexts:
      contexts[id] = Context(id)
    context = contexts[id]

    # Render target
    if "Activate" in event.name:
      d = event.values["aDest"]
      s = "%dx%d/%s" % (d.attrs["width"], d.attrs["height"], displayModeNames[d.attrs["mode"]])
      context.targets.append(s)

    context.events.append(event)

    # Text strings
    if "aText" in event.values:
      try:
        text = "".join([chr(c) for c in event.values["aText"].attrs["data"]])
        context.texts.append('"%s"' % text)
      except ValueError:
        pass
    
    # BitBlt
    if "BitBlt" in event.name:
      # Figure out the source and possible mask bitmap dimensions
      if "aBitmap" in event.values and "aMaskBitmap" in event.values:
        s = "src %s, mask %s" % (bitmapDescription(event.values["aBitmap"]), bitmapDescription(event.values["aMaskBitmap"]))
        context.blits.append(s)
      elif "aBitmap" in event.values:
        s = "src %s" % (bitmapDescription(event.values["aBitmap"]))
        context.blits.append(s)
    elif "AlphaBlend" in event.name:
      print event
      
    # User display mode
    if "SetUserDisplayMode" in event.name:
      mode = displayModeNames[event.values["aDisplayMode"]]
      context.userDisplayModes.append(mode)

  for context in contexts.values():
    print >>output, context
  output.close()
