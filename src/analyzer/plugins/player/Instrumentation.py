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

"""Instrumentation data processing."""

import os
from common import Task
import Trace
import Common
import StringIO
import Graphics
import Image
from common import Config
from common import Log

def instrumentationLogFileName(traceFileName):
  """
  @returns the corresponding instrumentation log file name for a trace file.
  """
  return os.path.join(traceFileName.split(".", 1)[0] + "_inst", "instrumentation.log")

def _processRenderSurface(logDir, attributes):
  def attr(name):
    return attributes[name]
    
  w, h            = attr("render_surface_width"), attr("render_surface_height")
  redMask         = attr("red_mask")
  greenMask       = attr("green_mask")
  blueMask        = attr("blue_mask")
  alphaMask       = attr("alpha_mask")
  depthMask       = attr("depth_mask")
  stencilMask     = attr("stencil_mask")
  isLinear        = attr("is_linear")
  isPremultiplied = attr("is_premultiplied")
  
  # Convert the color buffer
  if "color_buffer" in attributes:
    fileName    = attr("color_buffer")
    if not os.path.exists(fileName):
      fileName  = os.path.join(logDir, fileName)
    fileNameOut = fileName.rsplit(".", 1)[0] + ".png"
    
    # Only do the conversion if the image doesn't already exist
    # or if the source file is newer.
    if fileName.endswith(".dat") and \
       (not os.path.exists(fileNameOut) or \
        (os.path.exists(fileName) and os.path.getmtime(fileName) > os.path.getmtime(fileNameOut))
       ):
      stride      = attr("color_stride")
      
      f           = open(fileName, "rb")
      data        = f.read(stride * h)
      f.close()
      
      if len(data) != h * stride or not data:
        Log.error("Invalid color buffer data size: %d"  % len(data))
        return
      
      colorBuffer = Graphics.decodeImageData(data, (w, h), stride, redMask, greenMask, blueMask, alphaMask, isLinear, isPremultiplied)
      colorBuffer = colorBuffer.convert("RGBA")
      colorBuffer.save(fileNameOut)
      
      # We can remove the original file now
      os.unlink(fileName)
      
      # Replace the original file name with the decoded file
      attributes["color_buffer"] = fileNameOut
      
    # Eat the render surface attributes since they are of little use further down the road
    #for attrName in ["red_mask", "green_mask", "blue_mask", "alpha_mask",
    #                 "depth_mask", "stencil_mask", "color_stride",
    #                 "is_linear", "is_premultiplied", "color_data_type", 
    #                 "depth_data_type", "stencil_data_type"]:
    #  if attrName in attributes:
    #    del attributes[attrName]

  for bufferName in ["depth_buffer", "stencil_buffer"]:
    if bufferName in attributes and not os.path.exists(attributes[bufferName]):
      # Fill in the full buffer file name
      attributes[bufferName] = os.path.join(logDir, attr(bufferName))

def _postprocessEvent(event, logDir):
  # Process the render surface if there is one
  if "render_surface_width" in event.sensorData and "render_surface_height" in event.sensorData:
    try:
      _processRenderSurface(logDir, event.sensorData)
    except Exception, e:
      Log.error("Unable to load render surface for event %s(%d): %s" % (event.name, event.seq, e))

def loadInstrumentationData(analyzer, trace, instLog):
  """
  Load precalculated instrumentation data for a trace from an instrumentation log file.
  
  @param trace:    Trace that should be augmented with the instrumentation data
  @param instLog:  Instrumentation log file name, see instrumentationLogFileName
  """
  
  logFile    = open(instLog)
  logDir     = os.path.dirname(instLog)

  logFile.seek(0, 2)
  task       = Task.startTask("load-instrumentation", "Loading instrumentation data", logFile.tell())
  logFile.seek(0)
  
  valueTypeMap = {
    "int":   int,
    "float": float,
    "str":   str
  }
  
  # Register common sensors
  trace.sensors["render_surface_width"]       = Trace.InstrumentationSensor("Render surface width", isAverage = True)
  trace.sensors["render_surface_height"]      = Trace.InstrumentationSensor("Render surface height", isAverage = True)
  
  event  = None
  events = dict([(event.seq, event) for event in trace.events])
    
  for line in logFile.xreadlines():
    msg, args = line.rstrip().split(" ", 1)
    if msg == "event":
      # Finish off the previous event
      if event:
        _postprocessEvent(event, logDir)
      
      eventName, seq = args.split(" ", 1)
      seq = int(seq)
      
      # Check that the data refers to a valid event
      if not seq in events:
        analyzer.fail("Bad event sequence number: %d" % seq)
      
      event = events[seq]
      
      if event.name != eventName:
        analyzer.fail("Event names do not match: %s != %s" % (event.name, eventName))
      task.updateProgress(logFile.tell())
    elif msg == "value":
      assert event
      # Convert and store the value
      try:
        valueType, valueName, value = args.split(" ", 2)
        event.sensorData[valueName] = valueTypeMap[valueType](value)
      except ValueError:
        analyzer.reportWarning("Badly formatted sensor value: %s" % line)
    elif msg == "desc":
      # Record the sensor description
      valueName, valueDesc = args.split(" ", 1)
      if not valueName in trace.sensors:
        isAverage = valueName in [
          "render_surface_width", "render_surface_height",
          "red_mask", "green_mask", "blue_mask", "alpha_mask",
          "depth_mask", "stencil_mask", "color_stride",
          "is_linear", "is_premultiplied", "color_data_type", 
          "depth_data_type", "stencil_data_type"
        ]
        trace.sensors[valueName] = Trace.InstrumentationSensor(valueDesc, isAverage = isAverage)
    else:
      analyzer.reportWarning("Unknown message received from the player: %s" % line)
  if event:
    _postprocessEvent(event, logDir)
  task.finish()
  
  # Call every instrumentation post processor
  for plugin in analyzer.plugins:
    try:
      process = plugin.postProcessInstrumentationData
    except AttributeError:
      continue
    process(trace)

def saveInstrumentationData(analyzer, trace, instLog):
  task       = Task.startTask("save-instrumentation", "Saving instrumentation data", len(trace.events))
  out        = open(instLog, "w")
  
  # Save the sensor descriptions
  for name, sensor in trace.sensors.items():
    print >>out, "desc", name, sensor.description
    
  valueTypeMap = {
    Trace.IntegerValue:   "int",
    Trace.ShortValue:     "int",
    Trace.ByteValue:      "int",
    Trace.LongValue:      "int",
    Trace.FloatValue:     "float",
    Trace.DoubleValue:    "float",
    int:   "int",
    long:  "int",
    float: "float",
    str:   "str"
  }
  
  # Save the sensor measurements
  for event in trace.events:
    task.step()
    if event.sensorData:
      print >>out, "event", event.name, event.seq
      for name, value in event.sensorData.items():
        print >>out, "value", valueTypeMap.get(type(value), "str"), name, value
    
  # All done
  out.close()

def loadBuffer(event, name = "color"):
  """
  Load a buffer associated with an event saved by an instrumented engine as an image.
  
  @param event: Event of interest
  @param name:  Buffer name, e.g. color, depth, stencil
  
  @returns the loaded buffer as an Image or None if the buffer was not found.
  """
  name = "%s_buffer" % name
  if name in event.sensorData and isinstance(event.sensorData[name], str):
    return Image.open(event.sensorData[name])

def getBufferFileName(event, name = "color"):
  """
  Returns the filename of a buffer associated with an event or None.
  
  @param event: Event of interest
  @param name:  Buffer name, e.g. color, depth, stencil
  
  @returns the buffer file name or None if the buffer was not found.
  """
  name = "%s_buffer" % name
  if name in event.sensorData and os.path.isfile(event.sensorData[name]):
    return event.sensorData[name]
