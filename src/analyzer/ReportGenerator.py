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

import Report
import StringUtils
import Checklist
import Image
import TraceOperations
import Common
import player.Instrumentation
from common import Log
from common import Collections
from common import Task
import os
import math

# Minimum reasonable frame length in seconds
MIN_FRAME_DURATION = 0.001

class Frame(object):
  def __init__(self):
    self.number       = 0
    self.duration     = 0.0
    self.startTime    = 0.0
    self.endTime      = 0.0
    self.events       = []
    self.renderEvents = []
    self.swapEvent    = None
    
  def __repr__(self):
    return "<Frame #%d>" % self.number

class ReportGenerator(object):
  def __init__(self, project, trace, title, path, reportFormat):
    self.project           = project
    self.trace             = trace
    self.path              = path
    self.reportFormat      = reportFormat
    self.library           = project.targets["code"].library
    self.constants         = Collections.DictProxy(self.library.constants)
    self.report            = Report.Report(title)
    self.frames            = []
    self.interestingFrames = []

    # Lazy loading of charting support
    global pylab, Charting
    import Charting
    from Charting import pylab

    try:
      self.compiler = Report.createCompiler(reportFormat, self.report)
    except KeyError:
      raise RuntimeError("Unsupported format: %s" % format)
    
    if not trace.events:
      raise RuntimeError("Trace is empty.")

    # Make sure the output directory exists
    Report.makePath(self.path)
      
    # Report appearance
    self.primaryColor   = "orange"
    self.secondaryColor = "lightgrey"
    self.highlightColor = "red"  
    self.thumbnailSize  = (64, 64)
    
    # Chart appearance
    if pylab:
      pylab.subplots_adjust(hspace = .5)
      pylab.rc("text", fontsize = 7)
      pylab.rc("xtick", labelsize= 7)
      pylab.rc("ytick", labelsize= 7)
      pylab.rc("axes", labelsize= 7)
      pylab.rc("legend", fontsize = 9)

    # HTML report appearance
    if reportFormat == "html":
      self.compiler.setStyle("h1,h2,h3,h4,h5,h6", 
                             "clear: both;")
      self.compiler.setStyle("a", 
                             "color: %s;" % self.primaryColor,
                             "text-decoration: none;")
      self.compiler.setStyle("a:hover", 
                             "color: %s;" % self.highlightColor,
                             "text-decoration: underline;")
      self.compiler.setStyle("a img", 
                             "border: none;")
      self.compiler.setStyle("p.code", 
                             "background-color: #eee;",
                             "border: solid 1px #ddd;",
                             "width: 66%;",
                             "padding: 1em;",
                             "margin-left: 4em;")
      self.compiler.setStyle("p.checkpass", 
                             "float: right;",
                             "padding: 1em;",
                             "border: solid 1px black;",
                             "background-color: %s;" % self.primaryColor)
      self.compiler.setStyle("p.checkfail", 
                             "float: right;",
                             "padding: 1em;",
                             "border: solid 1px black;",
                             "background-color: %s;" % self.highlightColor)
      self.compiler.setStyle("div.toc li", 
                             "list-style-type: none")
      self.compiler.setStyle("div.toc a", 
                             "color: black;")
      self.compiler.setStyle("table", 
                             "border: solid thin #aaa;",
                             "margin: 1em;",
                             "border-collapse: collapse;",
                             "margin-left: 4em;")
      self.compiler.setStyle("table th", 
                             "text-align: left;",
                             "padding: .1em 1em .1em 1em;",
                             "background-color: %s;" % (self.primaryColor))
      self.compiler.setStyle("tr.odd", 
                             "background-color: #eee;")
      self.compiler.setStyle("table td", 
                             "padding: .1em 1em .1em 1em;")
      self.compiler.setStyle("img.screenshot", 
                             "float: right;")

  def createEventThumbnails(self, events):
    # Frame thumbnails
    task = Task.startTask("create-thumbnails", "Creating thumbnails", len(events))
    thumbnails = []
    
    for i, event in enumerate(events):
      task.step()
      try:
        image = player.Instrumentation.loadBuffer(event)
        if image:
          image.thumbnail(self.thumbnailSize, resample = True)
          fn = os.path.join(self.path, "frame_thumbnail%03d.png" % i)
          image.save(fn)
          thumbnails.append(Report.Image(fn))
      except IOError:
        continue
    return thumbnails
      
  def createGeneralStatisticsTable(self):
    t = Report.Table(["Property", "Value"])
      
    duration = self.frames[-1].endTime - self.frames[0].startTime
    t.addRow("Events", len(self.trace.events))
    t.addRow("Frames", len(self.frames))
    t.addRow("Duration", StringUtils.normalizeTimeValue(duration))
    t.addRow("Render calls", sum([len(f.renderEvents) for f in self.frames]))
    
    averageFps   = len(self.frames) / duration
    averageCalls = sum([len(f.events) for f in self.frames]) / float(len(self.frames))
  
    t.addRow("Average FPS", "%.02f" % averageFps)
    t.addRow("Average calls per frame", "%.02f" % averageCalls)
    return t
      
  def createPlot(self, title, xAxis, yAxis, id = 0, style = "line"):
    # See if there's anything to plot
    for value in yAxis:
      if value:
        break
    else:
      return
    
    pylab.clf()
    Charting.slicePlot(xAxis, yAxis, color = self.primaryColor, style = style)
    fn = os.path.join(self.path, title.lower().replace(" ", "_") + "%03d.png" % id)
    pylab.savefig(fn)
    pylab.close()

    section = Report.Section(title)
    section.create(Report.Image, fn)
    return section

  def createEventPlot(self, title, events, sliceStart = 0.0, sliceLength = 10.0, id = 0):
    # See if there's anything to plot
    if not len(events) >= 2:
      return
  
    segmentDuration = (events[-1].time - events[0].time) / 1e6
    
    if segmentDuration < sliceLength:
      sliceLength = events[0].time / 1e6 + segmentDuration
    
    segmentStart    = sliceStart
    slices          = int(segmentDuration / sliceLength) + 2
    
    pylab.figure(figsize = (8, slices * 1))
    pylab.subplot(slices, 1, 1)
    pylab.xlim(sliceStart, sliceStart + sliceLength)
    pylab.yticks([])
    
    legends = {}
    for event in events:
      time     = event.time / 1e6
      duration = event.duration / 1e6
      
      if time + duration > sliceStart + sliceLength:
        pylab.subplot(slices, 1, int((time - segmentStart) / sliceLength) + 1)
        pylab.xticks(size = 7)
        pylab.yticks([])
        sliceStart += sliceLength
        pylab.xlim(sliceStart, sliceStart + sliceLength)
      
      func = self.library.functions[event.name]
      if func.isFrameMarker:
        color = self.highlightColor
        label = "frameswap"
      elif func.isRenderCall:
        color = self.primaryColor
        label = "render"
      else:
        continue
      legends[label] = pylab.axvspan(time, time + max(sliceLength / 100.0, duration), facecolor = color, linewidth = .1, label = label)
  
    # Describe the chart elements
    l1 = []
    l2 = []
    if "render" in legends:
      l1.append(legends["render"])
      l2.append("Render call")
    if "frameswap" in legends:
      l1.append(legends["frameswap"])
      l2.append("Frame swap")
      
    pylab.legend(l1, l2)
    fn = os.path.join(self.path, title.lower().replace(" ", "_") + "%03d.png" % id)
    
    pylab.savefig(fn)
    pylab.close()
  
    section = Report.Section(title)
    section.create(Report.Image, fn)
    return section

  def createEventFrequencyPlot(self, title, events, id = 0):
    # Count the number of each event
    eventFrequency = Collections.DefaultDict(lambda: 0)
    for event in events:
      eventFrequency[event.name] += 1
      
    items = eventFrequency.items()
    items.sort(key = lambda f: -f[1])
      
    funcNames = [f[0] for f in items]
    funcFreq  = [f[1] for f in items]
    
    # Create a bar charts and add a text describing the event to each bar
    pylab.figure()
    pylab.yticks([])
    pylab.ylim(len(funcNames), 0)
    rects = pylab.barh(range(len(funcNames)), funcFreq, color = self.primaryColor)
    
    for name, rect in zip(funcNames, rects):
      pylab.text(rect.get_x() + rect.get_width(), rect.get_y() + rect.get_height(), "  " + name, fontsize = 8)
    
    fn = os.path.join(self.path, title.lower().replace(" ", "_") + "%03d.png" % id)
    pylab.savefig(fn)
    pylab.close()

    section = Report.Section(title)
    section.create(Report.Image, fn)
    return section

  def createSensorPlot(self, frame, sensorName):
    if sensorName in self.trace.sensors:
      return self.createPlot(self.trace.sensors[sensorName].description, range(len(frame.renderEvents)), 
                             [e.sensorData.get(sensorName, 0) for e in frame.renderEvents],
                             id = frame.number, style = "bar")

  def createFrameSection(self, frame):
    frameSection = Report.Section("Frame #%d" % (frame.number + 1))
    frameSection.create(Report.LinkTarget, "frame%d" % (frame.number + 1))

    # Add a framebuffer snapshot if we have one
    colorBuffer = player.Instrumentation.getBufferFileName(frame.swapEvent)
    if colorBuffer:
      frameSection.create(Report.Image, colorBuffer, elementClass = "screenshot")
    
    table = frameSection.create(Report.Table, ["Property", "Value"])
    table.addRow("API calls", len(frame.events))
    table.addRow("Render calls", len(frame.renderEvents))
    table.addRow("FPS", "%.02f" % (1.0 / frame.duration))

    # Add some frame-level graphs
    frameSection.add(self.createEventPlot("Event distribution", frame.events, sliceStart = frame.startTime, sliceLength = frame.duration, id = frame.number + 1))
    frameSection.add(self.createEventFrequencyPlot("Operation distribution", frame.events, id = frame.number + 1))
    
    return frameSection
    
  def createEventSection(self, event):
    func = self.library.functions[event.name]
    
    if func.isRenderCall:
      title = "Render call %s (#%d)" % (event.name, event.seq)
    elif func.isFrameMarker:
      title = "Frame swap %s (#%d)" % (event.name, event.seq)
    else:
      title = "Event %s (#%d)" % (event.name, event.seq)
      
    section = Report.Section(title)

    # Add a framebuffer snapshot if we have one
    colorBuffer = player.Instrumentation.getBufferFileName(event)
    if func.isRenderCall and colorBuffer:
      section.create(Report.Image, colorBuffer, elementClass = "screenshot")
    
    # Describe the event
    section.create(Report.Paragraph, self.getEventDescription(event), elementClass = "code")

    # Insert sensor measurements
    statsTable = section.create(Report.Table, ["Sensor", "Value"])
    
    for name in sorted(event.sensorData.keys()):
      if name in self.trace.sensors and name in event.sensorData and event.sensorData[name]:
        statsTable.addRow(self.trace.sensors[name].description, event.sensorData[name])

    return section

  def createChecklistSection(self, title, checklistItems):
    checklistSection = Report.Section(title)
    checklistItems   = Checklist.compileChecklist(self.project, self.trace, checklistItems)
   
    for item in checklistItems:
      title = "%s [%s]" % (item.name, item.verdict and "PASS" or "FAIL")
      itemSection = checklistSection.create(Report.Section, title)
      itemSection.create(Report.Paragraph, item.verdict and "PASS" or "FAIL", elementClass = item.verdict and "checkpass" or "checkfail")
      itemSection.create(Report.Paragraph, item.description)
      commentList = itemSection.create(Report.UnorderedList)
      
      # List unique comments only
      comments       = sorted(item.comments, key = lambda c: c[1])
      lastComment    = None
      repeatedEvents = []
        
      for event, comment in comments + [(None, None)]:
        if comment != lastComment:
          if lastComment and repeatedEvents:
            if len(repeatedEvents) < 100:
              commentList.addItem("Previous comment repeated for %s." % ", ".join(["%s (%d)" % (e.name, e.seq) for e in repeatedEvents]))
            else:
              commentList.addItem("Previous comment repeated for %d events." % len(repeatedEvents))
          if comment:
            if event:
              commentList.addItem("%s (%d): %s" % (self.getEventDescription(event), event.seq, comment))
            else:
              commentList.addItem(comment)
            lastComment    = comment
            repeatedEvents = []
        elif event:
          repeatedEvents.append(event)
    return checklistSection

  def getEventDescription(self, event):
    # Create a textual representation an event
    args = []
    func = self.library.functions[event.name]
    
    for name, value in event.values.items():
      if not name: continue
      value = StringUtils.ellipsis(StringUtils.decorateValue(self.library, func, name, value))
      args.append((name, value))
    return "%s(%s)" % (event.name, ", ".join(["%s = %s" % (k, v) for k, v in args]))

  def generate(self):
    self.compiler.compile(self.path)

  def calculateStatistics(self, frameSimilarityFunc = None, timeThreshold = .25, histogramThreshold = .96, frameCountThreshold = 20,
                          monochromaticThreshold = .99):
    # Reset the list of frames
    self.frames            = []
    self.interestingFrames = []
    
    # Combine frame level statistics
    TraceOperations.calculateFrameStatistics(self.project, self.trace)
    
    # Calculate some high level statistics
    events     = []
    
    for event in self.trace.events:
      func = self.library.functions[event.name]
      events.append(event)
      if func.isFrameMarker:
        frame = Frame()
        frame.startTime    = events[0].time / 1e6
        frame.endTime      = (events[-1].time + events[-1].duration) / 1e6
        frame.events       = events
        frame.renderEvents = [e for e in events if self.library.functions[e.name].isRenderCall]
        frame.swapEvent    = event
        frame.number       = len(self.frames)

        # Calculate frame duration
        frame.duration     = frame.endTime - frame.startTime
        if frame.duration < MIN_FRAME_DURATION:
          # If the frame has an essentially zero duration, the following event should give a proper time reading
          try:
            frame.duration = max(self.trace.events[self.trace.events.index(event) + 1].time / 1e6 - frame.startTime, MIN_FRAME_DURATION)
          except IndexError:
            frame.duration = MIN_FRAME_DURATION

        self.frames.append(frame)
        events             = []
          
    # Now prune the list of frames down to frames of special interest
    lastFrame  = None
    histograms = {}
    
    def getFrameHistogram(frame):
      if not frame.swapEvent in histograms:
        try:
          image = player.Instrumentation.loadBuffer(frame.swapEvent)
        except:
          return
        if image:
          # Calculate a normalized histogram
          hist = image.histogram()
          f    = 1.0 / (image.size[0] * image.size[1] * len(image.getbands()))
          hist = [h * f for h in hist]
          histograms[frame.swapEvent] = hist
      return histograms.get(frame.swapEvent)
    
    task = Task.startTask("choose-frames", "Choosing interesting frames", len(self.frames))
    for frame in self.frames:
      task.step()
      if lastFrame and len(self.frames) > frameCountThreshold:
        # If this frame's duration is pretty much the same as the last one, skip it
        if abs(frame.duration - lastFrame.duration) <= timeThreshold * lastFrame.duration:
          continue
          
        # Get color buffer histograms for both images
        hist1 = getFrameHistogram(frame)
        hist2 = getFrameHistogram(lastFrame)
        
        if hist1 and hist2:
          # Calculate the Bhattacharyya distance, i.e. the cosine of the angle
          # between the two histograms
          dist = sum([math.sqrt(h2 * h1) for h1, h2 in zip(hist1, hist2)])
          
          # If the distance is close to one, i.e. the frames are nearly identical,
          # skip this frame.
          if dist > histogramThreshold:
            continue
            
          # If the frame is mostly just one color, skip it
          if max(hist1) >= monochromaticThreshold:
            continue
        
        # If we have a similarity function, discard similar frames
        if frameSimilarityFunc and frameSimilarityFunc(lastFrame, frame):
          continue
          
      # Mark the new frame as interesting
      self.interestingFrames.append(frame)
      lastFrame = frame
    
