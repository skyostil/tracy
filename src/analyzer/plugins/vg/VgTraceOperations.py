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

"""VG trace operations."""

import Trace
import TraceState
from common import Task
import Graphics
import Common
import struct
import player.Instrumentation
from common import Collections

def calculateStatistics(project, trace):
  """
  Calculate derived OpenVG statistics instrumentation sensor data and trace events.
  """
  
  if not "code" in project.targets:
    raise ValueError("No code information in project file")

  task      = Task.startTask("vg-stats", "Calculating OpenVG statistics", len(trace.events))
  library   = project.targets["code"].library
  constants = Collections.DictProxy(library.constants)
  
  # Create the derived sensors
  trace.sensors["average_path_size"]       = Trace.InstrumentationSensor("Average path size", isAverage = True)
  trace.sensors["image_uploads"]           = Trace.InstrumentationSensor("Image uploads")
  trace.sensors["render_calls"]            = Trace.InstrumentationSensor("Rendering calls")
  trace.sensors["draw_ratio"]              = Trace.InstrumentationSensor("Draw ratio")
  trace.sensors["path_segments"]           = Trace.InstrumentationSensor("Path segments")
  trace.sensors["gradient_stops"]          = Trace.InstrumentationSensor("Number of gradient stops defined")
  
  prevRenderEvent = None
  
  for event in trace.events:
    task.step()
    
    func = library.functions[event.name]

    if func.isRenderCall:
      event.sensorData["render_calls"] = 1

    if event.name in ["vgSetParameterfv", "vgSetParameteriv"]:
      if event.values["paramType"] == constants.VG_PAINT_COLOR_RAMP_STOPS:
        event.sensorData["gradient_stops"] = event.values["count"] / 5

    fragments   = event.sensorData.get("rasterized_pixels", 0)
    renderCalls = event.sensorData.get("render_calls", 0)
    
    if fragments and renderCalls:
      event.sensorData["average_path_size"] = fragments / float(renderCalls)

    if event.name == "vgImageSubData" and "width" in event.values and "height" in event.values:
      event.sensorData["image_uploads"] = int(event.values["width"] * event.values["height"])
      
    if event.name == "vgAppendPathData":
      event.sensorData["path_segments"] = int(event.values["numSegments"])

    width         = event.sensorData.get("render_surface_width", 0)
    height        = event.sensorData.get("render_surface_height", 0)

    if fragments and width and height:
      event.sensorData["draw_ratio"] = fragments / float(width * height)

def getImageLoaders(project, trace):
  """
  Return a list of (event, func) pairs, where event is a image upload event and
  func is a function that returns an Image containing the image data when called.
  """
  library   = project.targets["code"].library
  constants = Collections.DictProxy(library.constants)
  loaders   = []

  formats = {
    constants.VG_sRGBX_8888:           ("b", 4, False, False, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000),
    constants.VG_sRGBA_8888:           ("b", 4, False, False, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000),
    constants.VG_sRGBA_8888_PRE:       ("b", 4, False, True,  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000),
    constants.VG_sRGB_565:             ("h", 3, False, False,     0x001f,     0x07e0,     0xf800,        0x0),
    constants.VG_sRGBA_5551:           ("h", 4, False, False,     0x001f,     0x03e0,     0x7c00,     0x8000),
    constants.VG_sRGBA_4444:           ("h", 4, False, False,     0x000f,     0x00f0,     0x0f00,     0xf000),
    constants.VG_sL_8:                 ("b", 1, False, False,       0xff,        0x0,        0x0,        0x0),
    constants.VG_lRGBX_8888:           ("b", 4, True,  False, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000),
    constants.VG_lRGBA_8888:           ("b", 4, True,  False, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000),
    constants.VG_lRGBA_8888_PRE:       ("b", 4, True,  True,  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000),
    constants.VG_lL_8:                 ("b", 1, True,  False,       0xff,        0x0,        0x0,        0x0),
    constants.VG_A_8:                  ("b", 1, True,  False,       0xff,        0x0,        0x0,        0x0),
    constants.VG_BW_1:                 ("b", 1, True,  False,        0x1,        0x0,        0x0,        0x0),
  }
  
  task = Task.startTask("prepare-images", "Looking for images", len(trace.events))
  for event in trace.events:
    task.step()
    
    if event.name == "vgImageSubData" and event.values.get("data"):
      width  = event.values["width"]
      height = event.values["height"]
      stride = event.values["dataStride"]
      format = event.values["dataFormat"]
      
      if format in formats:
        unit, components, isLinear, isPremultiplied, redMask, greenMask, blueMask, alphaMask = formats[format]
      else:
        continue

      data = event.values["data"]
      data = struct.pack("<%d%s" % (len(data), unit), *data)
      size = (width, height)

      # Construct copies of the passed variables to make sure the proper data goes into the lambda when called
      func = lambda d=data, s=size, st=stride, rb=redMask, gb=greenMask, bb=blueMask, ab=alphaMask, il=isLinear, ip=isPremultiplied: \
             Graphics.decodeImageData(d, s, st, rb, gb, bb, ab, isLinear = il, isPremultiplied = ip)
      loaders.append((event, func))
      
  return loaders
