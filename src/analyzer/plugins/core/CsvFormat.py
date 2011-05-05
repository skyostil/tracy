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

import re
import Plugin
from common import Task
import Trace

class CsvExporterPlugin(Plugin.ExporterPlugin):
  """
  Comma separated value trace file writer.
  """
  formatName = "csv"

  def saveTrace(self, trace, traceFile):
    task       = Task.startTask("csv-export", "Formatting CSV text", len(trace.events))
    fields     = ["sequence_number", "time", "name", "duration"]
    instFields = []
    separator  = ","

    # Find out all the needed instrumentation data fields
    for event in trace.events:
      for key, value in event.sensorData.items():
        if value and not key in instFields:
          instFields.append(key)

    instFields = sorted(instFields)
          
    # Print out the field header line
    print >>traceFile, separator.join(fields + instFields)
        
    for event in trace.events:
      f = [str(event.seq), str(event.time), event.name, str(event.duration)]
      
      for key in instFields:
        if key in event.sensorData:
          f.append(str(event.sensorData[key]))
        else:
          f.append("0")
          
      print >>traceFile, separator.join(f)
      task.step()
 
