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

import Plugin
from common import Task
import Trace
import StringUtils

class TextExporterPlugin(Plugin.ExporterPlugin):
  """
  Text trace file writer.
  """
  formatName = "text"

  def saveTrace(self, trace, traceFile, truncateValues = True, includeSensorData = False):
    try:
      library = self.analyzer.project.targets["code"].library
    except (AttributeError, KeyError):
      library = None

    task              = Task.startTask("text-export", "Formatting text", len(trace.events))
    truncateValues    = self.analyzer.parseBoolean(truncateValues)
    includeSensorData = self.analyzer.parseBoolean(includeSensorData)
    maxValueLength    = 1024 * 1024
    
    # Describe instrumentation sensors
    if includeSensorData:
      for name, sensor in sorted(trace.sensors.items()):
        print >>traceFile, "%010d %010d @inst %s: %s" % (0, 0, name, sensor.description)
    
    for event in trace.events:
      try:
        function = self.analyzer.lookupFunction(event)
      except:
        function = None
        
      # Print out any associated instrumentation data
      if includeSensorData:
        for key, value in sorted(event.sensorData.items()):
          if value:
            print >>traceFile, "%010d %010d @inst %s = %s" % (event.seq, event.time, key, value)
      
      # Print out any modified arrays that are not actual parameters for the event.
      for array in event.modifiedArrays:
        for value in event.values.values():
          if isinstance(value, Trace.Array) and value.id == array.id:
            break
        else:
          if truncateValues:
            text = StringUtils.ellipsis(array, maxLength = maxValueLength)
          else:
            text = array
          print >>traceFile, "%010d %010d @array 0x%x = %s" % (event.seq, event.time, array.id, text)
          
      args = []

      # Print out the parameters
      for name, value in event.values.items():
        if not name:
          continue
          
        if function and library:
          value = StringUtils.decorateValue(library, function, name, value)
          if truncateValues:
            value = StringUtils.ellipsis(value, maxLength = maxValueLength)
          
        args += ["%s=%s" % (name, value)]

      print >>traceFile, "%010d %010d %s (%s)" % (event.seq, event.time, event.name, ", ".join(args)),
          
      if None in event.values:
        print >>traceFile, "-> %s" % event.values[None], "+%d" % event.duration
      else:
        print >>traceFile, "+%d" % event.duration
      task.step()
