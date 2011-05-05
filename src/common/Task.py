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

"""Task monitoring."""

import time
import sys

monitor = None

class Task(object):
  def __init__(self, monitor, name, description, steps = None):
    self.monitor     = monitor
    self.name        = name
    self.description = description
    self.steps       = steps
    self.progress    = 0
    self.finished    = False
    if self.monitor:
      self.monitor.taskStarted(self)
      
  def setDescription(self, desc):
    self.description = desc
    
  def updateProgress(self, progress):
    if self.progress == self.steps:
      return
    
    self.progress = progress
    
    if self.monitor and self.progress is not None:
      self.monitor.taskProgress(self)
      
    if self.progress == self.steps:
      self.finish()
      
  def step(self):
    self.updateProgress(self.progress + 1)

  def finish(self):
    if self.finished:
      return

    self.finished = True
    
    if self.progress != self.steps:
      self.updateProgress(self.steps)
      
    if self.monitor:
      self.monitor.taskFinished(self)

class TaskMonitor(object):
  def taskStarted(self, task):
    pass

  def taskProgress(self, task):
    pass

  def taskFinished(self, task):
    pass

class RateLimitedTaskMonitor(TaskMonitor):
  def __init__(self, clientMonitor):
    self.client            = clientMonitor
    self.updateLimit       = 0.1
    self.runLimit          = 0.25
    self.taskStartTimes    = {}
    self.taskProgressTimes = {}
    
  def taskStarted(self, task):
    self.taskStartTimes[task]    = time.time()
    self.taskProgressTimes[task] = -1

  def taskProgress(self, task):
    t   = self.taskProgressTimes[task]
    now = time.time()
    
    if now - self.taskStartTimes[task] < self.runLimit:
      return
      
    if self.taskProgressTimes[task] == -1:
      self.client.taskStarted(task)
    
    if now - self.taskProgressTimes[task] >= self.updateLimit:
      self.taskProgressTimes[task] = now
      self.client.taskProgress(task)

  def taskFinished(self, task):
    if self.taskProgressTimes[task] != -1:
      self.client.taskFinished(task)
      
    del self.taskStartTimes[task]
    del self.taskProgressTimes[task]

def startTask(name, description, steps = None):
  global monitor
  return Task(monitor, name, description, steps)

def setMonitor(m):
  global monitor
  monitor = m
