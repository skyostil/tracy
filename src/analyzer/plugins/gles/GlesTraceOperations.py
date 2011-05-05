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

"""GLES trace operations."""

import Trace
import TraceState
from common import Task
import Graphics
import Common
import struct
import player.Instrumentation
from common import Collections

def simplify(project, trace):
  """
  Simplify a trace by removing redundant GLES commands from it.
  
  @param project:       Tracy GLES project
  @param trace:         Trace to examine
  
  @returns a simplified trace
  """
  
  if not "code" in project.targets:
    raise ValueError("No code information in project file")
    
  state            = TraceState.TraceState(project)
  newTrace         = Trace.Trace()
  newTrace.sensors = trace.sensors
  task             = Task.startTask("simplify", "Simplifying trace", len(trace.events))
  
  state.beginCollectingEffectiveEvents()
  
  for event in trace.events:
    task.step()
    """
    implicitStateValue = None
    
    if event.name == "glPushMatrix":
      try:
        depth = state.getValue(["ctx", "matrix_mode", "unit", "stack"])
        if depth is None: depth = 0
      except KeyError:
        depth = 0
      implicitStateValue = depth + 1
    elif event.name == "glPopMatrix":
      try:
        depth = state.getValue(["ctx", "matrix_mode", "unit", "stack"])
        if depth is None: depth = 0
      except KeyError:
        depth = 0
      implicitStateValue = depth - 1
    elif event.name == "glActiveTexture":
      implicitStateValue = event.values["texture"]
    """
    
    #print ">>>", event.name
    #print state
    state.processEvent(event)
    
    # If the event doesn't access the state explicitly, it is considered to be a draw call
    if not state.isStateAccessingEvent(event):
      state.endCollectingEffectiveEvents()
      
      effectiveEvents = state.getEffectiveEvents()
    
      i = 0
      while i < len(effectiveEvents):
        # Remove adjacent glPushMatrix()/glPopMatrix() pairs, since they are
        # an artifact of the state tracker  
        if i < len(effectiveEvents) - 1:
          if effectiveEvents[i].name == "glPushMatrix" and effectiveEvents[i + 1].name == "glPopMatrix":
            i += 2
            continue
        newTrace.events.append(effectiveEvents[i])
        i += 1
          
      newTrace.events.append(event)
      state.beginCollectingEffectiveEvents()
      continue

  #print state
  return newTrace

def calculateStatistics(project, trace):
  """
  Calculate derived OpenGL ES statistics instrumentation sensor data and trace events.
  """
  
  if not "code" in project.targets:
    raise ValueError("No code information in project file")

  task      = Task.startTask("gles-stats", "Calculating OpenGL ES statistics", len(trace.events))
  library   = project.targets["code"].library
  constants = Collections.DictProxy(library.constants)
  
  # Create the derived sensors
  trace.sensors["average_triangle_size"]   = Trace.InstrumentationSensor("Average triangle size", isAverage = True)
  trace.sensors["texel_fetches_per_pixel"] = Trace.InstrumentationSensor("Texel fetches per pixel", isAverage = True)
  trace.sensors["texel_uploads"]           = Trace.InstrumentationSensor("Texel uploads")
  trace.sensors["vertices_in"]             = Trace.InstrumentationSensor("Vertices in")
  trace.sensors["triangles_in"]            = Trace.InstrumentationSensor("Triangles in")
  trace.sensors["render_calls"]            = Trace.InstrumentationSensor("Rendering calls")
  trace.sensors["rasterizer_discarded_pixels"] = Trace.InstrumentationSensor("Rasterizer discarded pixels")
  trace.sensors["draw_ratio"]              = Trace.InstrumentationSensor("Draw ratio")
  
  prevRenderEvent = None
  depthMask       = 1
  
  for event in trace.events:
    task.step()
    
    func = library.functions[event.name]

    if func.isRenderCall:
      event.sensorData["render_calls"] = 1

    if func.isRenderCall and "count" in event.values and "mode" in event.values:
      m = event.values["mode"]
      if m == constants.GL_TRIANGLES:
        event.sensorData["triangles_in"] = int(event.values["count"] / 3)
      elif m == constants.GL_TRIANGLE_STRIP:
        event.sensorData["triangles_in"] = int(event.values["count"] - 2)
      elif m == constants.GL_TRIANGLE_FAN:
        event.sensorData["triangles_in"] = int(event.values["count"] - 2)
      elif m == constants.GL_POINTS:
        event.sensorData["triangles_in"] = int(event.values["count"] * 2)
      elif m == constants.GL_LINES:
        event.sensorData["triangles_in"] = int(event.values["count"])
      elif m == constants.GL_LINE_STRIP:
        event.sensorData["triangles_in"] = int(event.values["count"] * 2)
      elif m == constants.GL_LINE_LOOP:
        event.sensorData["triangles_in"] = int(event.values["count"] * 2 + 2)

    fragments  = event.sensorData.get("rasterizer_pixels", 0)
    triangles  = event.sensorData.get("triangles_in", 0)
    texFetches = event.sensorData.get("rasterizer_texel_fetches", 0)
    
    if triangles and fragments:
      event.sensorData["average_triangle_size"] = fragments / float(triangles)
      
    if fragments and texFetches:
      event.sensorData["texel_fetches_per_pixel"] = fragments / float(texFetches)
      
    if event.name in ["glTexImage2D", "glTexSubImage2D"]:
      event.sensorData["texel_uploads"] = int(event.values["width"] * event.values["height"])

    if func.isRenderCall and "count" in event.values:
      event.sensorData["vertices_in"] = int(event.values["count"])
      
    if event.name == "glDepthMask":
      depthMask = event.values["flag"]
      
    # If we have the depth buffers for this event and the previous draw event, see how many pixels
    # actually changed and use that value to estimate overdraw
    if func.isRenderCall and not "rasterizer_discarded_pixels" in event.sensorData:
      if depthMask and prevRenderEvent and "depth_stride" in event.sensorData and "depth_mask" in event.sensorData:
        f1 = player.Instrumentation.getBufferFileName(prevRenderEvent, "depth")
        f2 = player.Instrumentation.getBufferFileName(event, "depth")
        if f1 and f2:
          diff = Graphics.compareDepthBuffers(f1, f2, event.sensorData["depth_stride"], event.sensorData["depth_mask"])
          event.sensorData["rasterizer_discarded_pixels"] = fragments - diff
      prevRenderEvent = event
    
    discFragments = event.sensorData.get("rasterizer_discarded_pixels", 0)
    width         = event.sensorData.get("render_surface_width", 0)
    height        = event.sensorData.get("render_surface_height", 0)

    if fragments and width and height:
      event.sensorData["draw_ratio"] = (fragments - discFragments) / float(width * height)

def getTextureLoaders(project, trace):
  """
  Return a list of (event, func) pairs, where event is a texture upload event and
  func is a function that returns an Image containing the texture data when called.
  """
  library   = project.targets["code"].library
  constants = Collections.DictProxy(library.constants)
  loaders   = []

  componentCount = {
    constants.GL_ALPHA:           1,
    constants.GL_RGB:             3,
    constants.GL_RGBA:            4,
    constants.GL_LUMINANCE:       1,
    constants.GL_LUMINANCE_ALPHA: 2,
  }
  
  task = Task.startTask("prepare-textures", "Looking for textures", len(trace.events))
  for event in trace.events:
    task.step()
    
    # We don't handle compressed texture formats
    if event.name in ["glTexImage2D", "glTexSubImage2D"] and event.values.get("pixels"):
      width  = event.values["width"]
      height = event.values["height"]
      format = event.values["format"]
      type   = event.values["type"]
      
      if format in componentCount:
        components = componentCount[format]
      else:
        continue

      if type == constants.GL_UNSIGNED_BYTE:
        bytesPerPixel = components
        format    = "b"
        redMask   = 0x00ff0000
        greenMask = 0x0000ff00
        blueMask  = 0x000000ff
        alphaMask = 0xff000000
      elif type == constants.GL_UNSIGNED_SHORT_5_6_5:
        bytesPerPixel = 2
        format    = "h"
        redMask   = 0x001f
        greenMask = 0x07e0
        blueMask  = 0xf800
        alphaMask = 0x0000
      elif type == constants.GL_UNSIGNED_SHORT_5_5_5_1:
        bytesPerPixel = 2
        format    = "h"
        redMask   = 0x001f
        greenMask = 0x03e0
        blueMask  = 0x7c00
        alphaMask = 0x8000
      elif type == constants.GL_UNSIGNED_SHORT_4_4_4_4:
        bytesPerPixel = 2
        format    = "h"
        redMask   = 0x000f
        greenMask = 0x00f0
        blueMask  = 0x0f00
        alphaMask = 0xf000
      else:
        continue

      pixels = event.values["pixels"]
      data   = struct.pack("<%d%s" % (len(pixels), format), *pixels)
        
      if components < 4:
        alphaMask = 0
      if components < 3:
        blueMask = 0
      if components < 2:
        greenMask = 0
        
      size   = (width, height)
      stride = width * bytesPerPixel

      # Construct copies of the passed variables to make sure the proper data goes into the lambda when called
      func = lambda d=data, s=size, st=stride, rb=redMask, gb=greenMask, bb=blueMask, ab=alphaMask: \
             Graphics.decodeImageData(d, s, st, rb, gb, bb, ab)
      loaders.append((event, func))
      
  return loaders
