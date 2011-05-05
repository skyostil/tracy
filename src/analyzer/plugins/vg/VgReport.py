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

"""VG performance report generator."""

import Report
import ReportGenerator
import VgTraceOperations
from common import Task
import Trace
import Common
from common import Log
import os

def generateReport(project, trace, traceFileName, path, format):
  if traceFileName:
    title = "OpenVG performance report for %s" % os.path.basename(traceFileName)
  else:
    title = "OpenVG performance report"
  
  g = ReportGenerator.ReportGenerator(project, trace, title, path, format)

  # Calculate VG specific stats
  #VgTraceOperations.calculateStatistics(project, trace)

  # Calculate general stats
  g.calculateStatistics()

  # Add some general information first
  section = g.report.create(Report.Section, "General statistics")
  table   = g.createGeneralStatisticsTable()
  
  if traceFileName:
    table.addRow("File name", traceFileName)

  section.add(table)
  
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

  # Images
  imageLoaders = VgTraceOperations.getImageLoaders(project, trace)
  
  if imageLoaders:
    imageSection = overviewSection.create(Report.Section, "Loaded images")
    task         = Task.startTask("load-images", "Loading images", len(imageLoaders))
    for event, func in imageLoaders:
      task.step()
      image = func().convert("RGBA")
      fn = os.path.join(path, "image%03d.png" % event.seq)
      image.save(fn)
      imageSection.create(Report.Image, fn)

  # FPS
  data = [1.0 / f.duration for f in g.frames]
  plot = g.createPlot("Frames per second", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Render calls
  data = [len(f.events) for f in g.frames]
  plot = g.createPlot("Number of API calls per frame", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Fragment count
  data = [f.swapEvent.sensorData.get("rasterized_pixels", 0) for f in g.frames]
  plot = g.createPlot("Rasterized fragments per frame", range(len(g.frames)), data)
  overviewSection.add(plot)

  # Now go over each interesting frame
  task               = Task.startTask("frame-report", "Generating report", len(g.interestingFrames))
  frameDetailSection = g.report.create(Report.Section, "Detailed frame statistics")
  
  for frame in g.interestingFrames:
    task.step()
    frameSection = g.createFrameSection(frame)
    
    # Add some custom plots
    plot = g.createSensorPlot(frame, "rasterized_pixels")
    frameSection.add(plot)
    
    # Now go over the individual render calls + the swap event
    for event in frame.renderEvents + [frame.swapEvent]:
      eventSection = g.createEventSection(event)
      frameSection.add(eventSection)

    frameDetailSection.add(frameSection)

  # Add the checklist result
  #g.report.add(g.createChecklistSection("Performance Checklist", VgChecklist.checklistItems))

  # Finalize the report
  task.finish()
  g.generate()
