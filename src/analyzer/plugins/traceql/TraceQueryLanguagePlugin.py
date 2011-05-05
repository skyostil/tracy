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
import TraceQueryLanguage

class TraceQueryLanguagePlugin(Plugin.InteractivePlugin):
  def __init__(self, analyzer):
    self.analyzer = analyzer
    self.analyzer.registerCommand("select", self.select, customSyntax = True)

  def select(self, expression):
    """
    Choose events from one or many traces based on a condition. The condition for
    choosing the events is expressed in an SQL-like language.
    
    Here are some example queries to get you started:
    
      - SELECT event.seq, event.name FROM t0
        List all events in trace 't0'.
        
      - SELECT event.seq, event.name FROM t0 WHERE event.duration > 10000
        List all events that took more than 10 milliseconds.

      - SELECT event.seq, event.name FROM t0 WHERE event.name = "glDrawElements"
        List all glDrawElements events.

      - SELECT event.seq, event.duration, event.name FROM t0 ORDER BY event.duration DESC LIMIT 10
        List the ten longest running events.

      - SELECT event.seq, event.name FROM t0 ORDER BY event.duration
        List all events that took more than 10 milliseconds.

      - SELECT * FROM t0
        List all possible attribute values for all events in trace 't0'.

      - SELECT * INTO t1 FROM t0 WHERE event.seq > 1000 AND event.seq < 2000
        Copy a 1000 events from trace 't0' into trace 't1'.

      - SELECT event.seq, event.name FROM t0, t1 WHERE t0.event.seq=t1.event.seq
        List corresponding events from traces 't0' and 't1'.

      - SELECT DISTINCT event.name FROM t0 WHERE event.values.path
        List all functions that have a path parameter.

    @param expression:   A trace query language expression
    """
    
    # Prepare the environment for the query
    env = TraceQueryLanguage.Environment(dict(self.analyzer.traces.items()))
    
    # Perform the query
    try:
      result = TraceQueryLanguage.query(env, "select " + expression)
      
      # Copy any new created traces over to the analyzer
      newTraces = []
      for traceName, trace in env.traces.items():
        if not traceName in self.analyzer.traces:
          self.analyzer.traces[traceName] = trace
          newTraces.append(traceName)

      resultDesc = "%d %s selected"% (len(result.rows), len(result.rows) > 1 and "events" or "event")

      if not newTraces:
        self.analyzer.reportInfo(str(result))
      else:
        resultDesc += " into " + ", ".join(newTraces)

      self.analyzer.reportInfo(resultDesc + ".")

    except TraceQueryLanguage.QueryException, e:
      self.analyzer.reportError(e)
