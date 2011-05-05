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

"""GLES performance report generator."""

import Report
import ReportGenerator
from common import Task
import Trace
import GlesTraceOperations
import GlesChecklist
import Common
from common import Log
import os

def addRenderTargetSection(g):
  s = g.report.create(Report.Section, "Render targets")
  
  # Check the used EGL configurations and surfaces
  configs = set()
  for event in g.trace.events:
    for key, value in event.values.items():
      if isinstance(value, Trace.Object):
        if value.cls.name == "EGLConfig":
          id = value.attrs.get("config_id")
          if not id or id in configs:
            continue
          configs.add(id)
          s2 = s.create(Report.Section, "EGL configuration #%d" % id)
          t  = s2.create(Report.Table, ["Property", "Value"])
          for cfgKey, cfgValue in value.attrs.items():
            cfgKey = cfgKey.replace("_", " ").capitalize()
            t.addRow(cfgKey, cfgValue)
    if event.name == "eglCreateWindowSurface":
      window = event.values.get("window")
      
      if not window:
        continue
      
      s2 = s.create(Report.Section, "Window surface 0x%x" % event.values[None].id)
      t  = s2.create(Report.Table, ["Property", "Value"])
      t.addRow("Width",  window.attrs["width"])
      t.addRow("Height", window.attrs["height"])
      t.addRow("Config ID", event.values["config"].id)
    elif event.name == "eglCreatePixmapSurface":
      pixmap = event.values.get("pixmap")
      
      if not pixmap:
        continue
      
      s2 = s.create(Report.Section, "Pixmap surface 0x%x" % event.values[None].id)
      t  = s2.create(Report.Table, ["Property", "Value"])
      t.addRow("Width",  pixmap.attrs["width"])
      t.addRow("Height", pixmap.attrs["height"])
      t.addRow("Config ID", event.values["config"].id)
    elif event.name == "eglCreatePbufferSurface":      
      attrs = event.values.get("attrib_list")
      
      if not attrs:
        continue
        
      try:
        width  = attrs[attrs.index(g.constants.EGL_WIDTH) + 1]
        height = attrs[attrs.index(g.constants.EGL_HEIGHT) + 1]
      except ValueError:
        continue

      s2 = s.create(Report.Section, "Pbuffer surface 0x%x" % event.values[None].id)
      t  = s2.create(Report.Table, ["Property", "Value"])
      t.addRow("Width",  width)
      t.addRow("Height", height)
      t.addRow("Config ID", event.values["config"].id)
  
def generateReport(project, trace, traceFileName, path, format):
  if traceFileName:
    title = "OpenGL ES performance report for %s" % os.path.basename(traceFileName)
  else:
    title = "OpenGL ES performance report"
  
  g = ReportGenerator.ReportGenerator(project, trace, title, path, format)

  # Calculate GLES specific stats
  GlesTraceOperations.calculateStatistics(project, trace)

  # Calculate general stats
  g.calculateStatistics()

  # Add some general information first
  section = g.report.create(Report.Section, "General statistics")
  table   = g.createGeneralStatisticsTable()
  
  if traceFileName:
    table.addRow("File name", traceFileName)

  section.add(table)
  
  # Add a section about the used render targets
  addRenderTargetSection(g)
  
  # Add an overall timeline of all events
  g.report.add(g.createEventPlot("Event distribution", trace.events))

  # Add a graph about the event type distribution
  g.report.add(g.createEventFrequencyPlot("Operation distribution", trace.events))
  
  # Add overview section
  overviewSection = g.report.create(Report.Section, "Overview")
  
  # Frame thumbnails
  thumbnailSection = overviewSection.create(Report.Section, "Selected frames")
  thumbnails       = g.createEventThumbnails([f.swapEvent for f in g.interestingFrames])
  
  for frame, thumbnail in zip(g.interestingFrames, thumbnails):
    thumbnailSection.create(Report.Link, "#frame%d" % (frame.number + 1), thumbnail)

  # Textures
  textureLoaders = GlesTraceOperations.getTextureLoaders(project, trace)
  
  if textureLoaders:
    textureSection = overviewSection.create(Report.Section, "Loaded textures")
    task           = Task.startTask("load-textures", "Loading textures", len(textureLoaders))
    for event, func in textureLoaders:
      task.step()
      image = func().convert("RGBA")
      fn = os.path.join(path, "texture%03d.png" % event.seq)
      image.save(fn)
      textureSection.create(Report.Image, fn)

  # FPS
  data = [1.0 / f.duration for f in g.frames]
  plot = g.createPlot("Frames per second", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Render calls
  data = [len(f.events) for f in g.frames]
  plot = g.createPlot("Number of API calls per frame", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Overdraw
  data = [f.swapEvent.sensorData.get("draw_ratio", 0) for f in g.frames]
  plot = g.createPlot("Draw ratio", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Fragment count
  data = [f.swapEvent.sensorData.get("rasterizer_pixels", 0) for f in g.frames]
  plot = g.createPlot("Rasterized fragments per frame", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Texture reads
  data = [f.swapEvent.sensorData.get("rasterizer_texel_fetches", 0) for f in g.frames]
  plot = g.createPlot("Texel fetches per frame", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Texture uploads
  data = [f.swapEvent.sensorData.get("texel_uploads", 0) for f in g.frames]
  plot = g.createPlot("Texel uploads per frame", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Now go over each interesting frame
  task               = Task.startTask("frame-report", "Generating report", len(g.interestingFrames))
  frameDetailSection = g.report.create(Report.Section, "Detailed frame statistics")
  
  for frame in g.interestingFrames:
    task.step()
    frameSection = g.createFrameSection(frame)
    
    # Add some custom plots
    plot = g.createSensorPlot(frame, "rasterizer_pixels")
    frameSection.add(plot)
    plot = g.createSensorPlot(frame, "rasterizer_texel_fetches")
    frameSection.add(plot)
    plot = g.createSensorPlot(frame, "texel_uploads")
    frameSection.add(plot)
    
    # Now go over the individual render calls + the swap event
    for event in frame.renderEvents + [frame.swapEvent]:
      eventSection = g.createEventSection(event)
      frameSection.add(eventSection)

    frameDetailSection.add(frameSection)

  # Add the checklist result
  g.report.add(g.createChecklistSection("Performance Checklist", GlesChecklist.checklistItems))

  # Finalize the report
  task.finish()
  g.generate()
