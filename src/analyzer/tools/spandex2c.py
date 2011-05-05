#
# Convert a set of Spandex OpenVG trace files to C
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
import glob
import os
import copy

#sourceMask = r"\epoc32\winscw\c\tracer_*Spandex*.bin"
sourceMask = r"data\ovg_rfs\tracer_*Spandex*.bin"
outputPath = r"data\ovg_rfs"
n          = 0
profile    = False

def countFrames(trace):
  n = 0
  for event in trace.events:
    if library.functions[event.name].isFrameMarker:
      n += 1
  return n

for fn in sorted(glob.glob(sourceMask)):
  trace = cmd.load(fn, "trace")
  
  # If this is a short trace, skip it
  if len(trace.events) <= 3:
    continue
  
  # See if there is a vgDrawPath command in the trace?
  for event in trace.events:
    if event.name == "vgDrawPath":
      break
  else:
    continue

  extraQualifier = ""
  isSteadystateBenchmark = False
  
  # Check the resolution
  for event in trace.events:
    if event.name == "VGIInitialize":
      extraQualifier = "rasterize"
      isSteadystateBenchmark = True
      break
    elif event.name == "VGISymbianInitialize":
      extraQualifier = "full"
      break
  
  outputFileName = "%s%03d.inl" % (extraQualifier, n)
    
  if isSteadystateBenchmark:
    frameMarkers = []

    # Split the loop at the first vgClear
    for i, event in enumerate(trace.events):
      if event.name == "vgClear":
        frameMarkers.append(trace.events[i - 1])
        break

    # ...and the last vgGetError or VGICopyToTarget
    for i, event in enumerate(reversed(trace.events)):
      if event.name in ["vgGetError", "VGICopyToTarget"]:
        frameMarkers.append(trace.events[-i - 1])
        break
        
    # Remove vgGetErrors
    [trace.events.remove(e) for e in trace.events if e.name == "vgGetError" and not e in frameMarkers]

    cmd.save(traceName = "trace", fileName = os.path.join(outputPath, outputFileName), format = "c", frameMarkers = frameMarkers)
  else:
    # Otherwise this is just a normal benchmark
    
    # Remove vgGetErrors
    [trace.events.remove(e) for e in trace.events if e.name == "vgGetError"]

    # Save the 'full' benchmark
    cmd.save(traceName = "trace", fileName = os.path.join(outputPath, outputFileName), format = "c")
    
    # Create a medium version of the benchmark where the first VGISymbianInitialize and the last 
    # VGISymbianTerminate are in their own frames
    frameMarkers = []
    outputFileName = "%s%03d.inl" % ("medium", n)

    # Split the loop at the first VGISymbianInitialize
    found = False
    for i, event in enumerate(trace.events):
      if event.name == "VGISymbianInitialize" and not found:
        frameMarkers.append(trace.events[i])
        found = True
      elif event.name == "VGISymbianCopyToBitmap":
        frameMarkers.append(trace.events[i])

    # ...and the VGISymbianTerminate
    for i, event in enumerate(reversed(trace.events)):
      if event.name == "VGISymbianTerminate":
        frameMarkers.append(trace.events[-i - 2])
        break
    
    if profile:
      cmd.play(traceName = "trace", profile = True, saveFrames = True)
    cmd.save(traceName = "trace", fileName = os.path.join(outputPath, outputFileName), format = "c", frameMarkers = frameMarkers)
  n += 1
