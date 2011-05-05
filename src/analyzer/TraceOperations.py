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

"""
Trace manipulation operations.
"""

import Trace
import TraceState
from common import Task
from common import Console
import common
from common.Collections import DefaultDict
import copy

def extract(trace, firstEvent, lastEvent):
  """
  Extract a portion of a trace much like the slice operations
  extracts a portion of a list.
  
  @param trace        Source trace
  @param firstEvent   Index of the first event to extract
  @param lastEvent    Index of the last event to extract (non-inclusive)
  
  @returns a new trace that contains just the extracted events.
  """
  newTrace = Trace.Trace()
  newTrace.sensors = trace.sensors
  
  if not trace.events:
    raise IndexError("Event index out of range")
  
  baseTime = trace.events[firstEvent].time
  task     = Task.startTask("extract", "Extracting", len(trace.events[firstEvent:lastEvent]))
  
  for event in trace.events[firstEvent:lastEvent]:
    event       = copy.copy(event)
    event.seq   = len(newTrace.events)
    event.time -= baseTime
    newTrace.events.append(event)
    task.step()
  
  return newTrace

def join(trace1, trace2):
  """
  Joins two traces together.
  
  @param trace1       First trace
  @param trace2       Second trace
  
  @returns a new trace that is composed by joining trace1 and trace2
           together in this order.
  """
  newTrace = Trace.Trace()
  newTrace.sensors = trace1.sensors
  newTrace.sensors.update(trace2.sensors)
  baseTime = 0
  task     = Task.startTask("join", "Joining traces", len(trace1.events) + len(trace2.events))
  
  # Process trace 1 events
  if trace1.events:
    baseTime = trace1.events[0].time
    for event in trace1.events:
      event       = copy.copy(event)
      event.seq   = len(newTrace.events)
      event.time -= baseTime
      newTrace.events.append(event)
      task.step()

  # Process trace 2 events
  if trace2.events:
    baseTime      = trace2.events[0].time
    if newTrace.events:
      baseTime -= newTrace.events[-1].time + newTrace.events[-1].duration
    
    for event in trace2.events:
      event       = copy.deepcopy(event)
      event.seq   = len(newTrace.events)
      event.time -= baseTime
      newTrace.events.append(event)
      task.step()
        
  return newTrace

def merge(trace1, trace2, useTimeStamps = False):
  """
  Merge two traces together to produce a single temporally coherent trace.
  
  @param trace1       First trace
  @param trace2       Second trace
  
  @returns a new trace that is composed by interleaving the events in trace1 and trace2
           sorted by their sequence numbers.
  """
  newTrace = Trace.Trace()
  newTrace.sensors = trace1.sensors
  newTrace.sensors.update(trace2.sensors)
  task     = Task.startTask("merge", "Merging traces", len(trace1.events) + len(trace2.events))
  
  # Process trace 1 events
  for event in trace1.events:
    event       = copy.copy(event)
    newTrace.events.append(event)
    task.step()

  # Process trace 2 events
  for event in trace2.events:
    event       = copy.copy(event)
    newTrace.events.append(event)
    task.step()
    
  # Sort the resulting events by their sequence numbers or timestamps
  if useTimeStamps:
    newTrace.events.sort(key = lambda event: event.time)
  else:
    newTrace.events.sort(key = lambda event: event.seq)
        
  return newTrace

def extractWithState(project, trace, firstEvent, lastEvent):
  """
  Extract a portion of a trace much like the slice operations
  extracts a portion of a list. In addition to the plain
  extract operation, this operation also extracts events prior
  to the first event to guarantee that the API state is 
  preserved for the first event.
  
  @param project      Tracy project.
  @param trace        Source trace
  @param firstEvent   Index of the first event to extract
  @param lastEvent    Index of the last event to extract (non-inclusive)
  
  @returns a new trace that contains the extracted events preceeded
           by collected state setup events.
  """
  
  newTrace = Trace.Trace()
  newTrace.sensors = trace.sensors
  
  if not trace.events:
    raise IndexError("Event index out of range")
    
  if not "code" in project.targets:
    raise ValueError("No code information in project file")

  state = TraceState.TraceState(project)
  task  = Task.startTask("extract-state", "Computing state", len(trace.events[0:firstEvent]))
    
  # Extract state setup
  state.beginCollectingEffectiveEvents()
  for event in trace.events[0:firstEvent]:
    state.processEvent(event)
    task.step()
  state.endCollectingEffectiveEvents()

  events = state.getEffectiveEvents() + trace.events[firstEvent:lastEvent]
  task   = Task.startTask("extract-state", "Creating trace", len(events))
  baseTime = 0
  
  if events:
    baseTime = events[0].time
  
  # Add the state setup and the extracted events to the new trace
  for event in events:
    event       = copy.copy(event)
    event.seq   = len(newTrace.events)
    event.time -= baseTime
    newTrace.events.append(event)
    task.step()
  
  return newTrace

def computeStateAtEvent(project, trace, eventNumber):
  """
  Compute a trace state at the given event.
  
  @param project:     Tracy project
  @param trace:       Trace to examine
  @param eventNumber: Number of event up to which the state is computed.
  
  @returns a TraceState instance
  """
  
  if not trace.events or eventNumber >= len(trace.events):
    raise IndexError("Event index out of range")
    
  if not "code" in project.targets:
    raise ValueError("No code information in project file")
  
  state = TraceState.TraceState(project)
  task  = Task.startTask("compute-state", "Computing state", len(trace.events[:eventNumber]))
  
  for event in trace.events[:eventNumber]:
    state.processEvent(event) 
    task.step()
  
  return state

def calculateFrameStatistics(project, trace):
  """
  For each frame marker event, calculate the sum of the instrumentation data
  for the intermediate events. The instrumentation sensors for each frame marker
  will contain the aggregate data.
  
  @param project:   Tracy project
  @param trace:     Trace to examine
  
  @returns a list of frame marker events
  """
  if not "code" in project.targets:
    raise ValueError("No code information in project file")
    
  counters    = DefaultDict(lambda: 0)
  avgCounters = DefaultDict(lambda: [])
  library     = project.targets["code"].library
  task        = Task.startTask("calculate-stats", "Computing frame statistics", len(trace.events))
  events      = []
  frameEvents = []
  frameStart  = 0

  trace.sensors["frame_duration"] = Trace.InstrumentationSensor("Frame duration")
  trace.sensors["event_count"]    = Trace.InstrumentationSensor("Event count")

  for event in trace.events:
    func = library.functions[event.name]
    task.step()
    
    if func.isFrameMarker:
      event.sensorData.update(dict([(k, sum(v) / float(len(v))) for k, v in avgCounters.items()]))
      event.sensorData.update(counters)
      counters    = DefaultDict(lambda: 0)
      avgCounters = DefaultDict(lambda: [])
      event.sensorData["frame_duration"] = event.time + event.duration - frameStart
      event.sensorData["event_count"]    = len(frameEvents)
      frameStart  = event.time + event.duration
      frameEvents = []
      events.append(event)
    else:
      frameEvents.append(event)
      for key, value in event.sensorData.items():
        if key in trace.sensors and trace.sensors[key].isAverage:
          try:
            avgCounters[key].append(float(value))
          except ValueError:
            pass
        else:
          try:
            if isinstance(value, float):
              counters[key] += value
            else:
              counters[key] += int(value)
          except ValueError:
            pass
  return events

def calculateCallHistogram(trace):
  """
  Calculate a cumulative call histogram for a given trace.
  """
  hist = DefaultDict(lambda: 0)
  
  for event in trace.events:
    hist[event.name] += 1
  
  return hist

def verify(analyzer, project, trace):
  # Can't do much without API metadata
  if not "code" in project.targets:
    return

  library     = project.targets["code"].library
  task        = Task.startTask("verify", "Verifying trace", len(trace.events))
  result      = True
  paramMap    = {}
  
  for event in trace.events:
    if not event.name in library.functions:
      analyzer.reportWarning("Function '%s' not found in library." % event.name)
      result = False
      continue
    func = library.functions[event.name]

    for name in event.values:
      if name is None:
        continue
      if not name in func.parameters:
        analyzer.reportWarning("Function '%s' parameter '%s' not found in library." % (event.name, name))
        result = False
        tag = "%s.%s" % (event.name, name)
        if not tag in paramMap:
          paramMap[tag] = Console.choose("Please choose the correct parameter", func.parameters.keys())
        if paramMap[tag]:
          newName = paramMap[tag]
          event.values[newName] = event.values[name]
          del event.values[name]

  return result
