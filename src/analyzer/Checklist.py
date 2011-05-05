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

"""Checklist of performance problems."""

import TraceState
from common import Task
import Common
from common import Collections
import StringIO

class ChecklistItem(object):
  name        = ""
  description = ""
  
  def __init__(self, library, trace, state):
    self.library        = library
    self.trace          = trace
    self.state          = state
    self.stateModifiers = []
    self.verdict        = True
    self.comments       = []
    self.constants      = Collections.DictProxy(self.library.constants)
    self.constantNames  = Collections.inverseDict(self.library.constants)

  def initialize(self):
    pass
  
  def process(self, event):
    pass
  
  def finalize(self):
    pass
    
  def fail(self, event, comment):
    self.verdict = False
    self.comment(event, comment)

  def comment(self, event, comment):
    self.comments.append((event, comment))
    
  def __str__(self):
    s = StringIO.StringIO()
    print >>s, "[%s] %s" % (self.verdict and "PASS" or "FAIL", self.name)
    
    # Print unique comments only
    comments       = sorted(self.comments, key = lambda c: c[1])
    lastComment    = None
    repeatedEvents = []
      
    for event, comment in comments + [(None, None)]:
      if comment != lastComment:
        if lastComment and repeatedEvents:
          if len(repeatedEvents) < 10:
            print >>s, "       - Last comment repeated for events:", ", ".join(["%s #%d" % (e.name, e.seq) for e in repeatedEvents])
          else:
            print >>s, "       - Last comment repeated for %d events." % len(repeatedEvents)
        if comment:
          if event:
            print >>s, "       - %s (%d): %s" % (event.name, event.seq, comment)
          else:
            print >>s, "       - %s" % (comment)
          lastComment    = comment
          repeatedEvents = []
      elif event:
        repeatedEvents.append(event)
    return s.getvalue()

def compileChecklist(project, trace, items):
  """
  Runs the given trace through a number of checklist items.
  
  @param project  Tracy project
  @param trace    Trace to examine
  @param items    Checklist item classes
  
  @returns a list of processed checklist items (ChecklistItem).
  """
  state   = TraceState.TraceState(project)
  task    = Task.startTask("checklist", "Building checklist", len(trace.events))
  library = project.targets["code"].library
  
  # Initialize the checklist items
  items = [item(library, trace, state) for item in items]
  
  for item in items:
    item.initialize()
    
  for event in trace.events:
    stateModifiers = state.processEvent(event)
    for item in items:
      item.stateModifiers = stateModifiers
      item.process(event)
    task.step()

  for item in items:
    item.finalize()
    
  return items

class CPUTimeDivision(ChecklistItem):
  name        = "CPU time division"
  threshold   = 50
  description = "Verify that more than %d %% of CPU time is being spent inside the graphics engine." % threshold

  def initialize(self):
    total    = self.trace.events[-1].time - self.trace.events[0].time
    inEngine = sum([e.duration for e in self.trace.events])
    if total:
      p = 100.0 * inEngine / total
      if p < CPUTimeDivision.threshold:
        self.fail(self.trace.events[0], "Only %d %% of CPU time is spent inside the graphics engine." % p)

commonChecklistItems = [
    CPUTimeDivision
]
