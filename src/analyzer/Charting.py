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

import Common
from common import Log

# Try to import matplotlib for charting
try:
  import matplotlib
  matplotlib.use("Agg")
  import pylab
except ImportError, e:
  matplotlib = None
  pylab      = None
  Log.warn("Matplotlib or one of it's dependencies not found (%s). Charts will not be generated." % e)
  
def slicePlot(x, y, sliceLength = 100, style = "line", *args, **kwargs):
  assert len(x) == len(y)
  
  if style == "line":
    plotFunc = pylab.plot
  elif style == "bar":
    plotFunc = pylab.bar
  else:
    raise RuntimeError("Unknown plotting style: %s" % style)
  
  if len(x) < sliceLength:
    plotFunc(x, y, *args, **kwargs)
    return
    
  slices = int(len(x) / sliceLength)
  
  pylab.figure(figsize = (8, slices * 1))
  for i in range(slices):
    pylab.subplot(slices, 1, i + 1)
    plotFunc(x[i * sliceLength: (i + 1) * sliceLength], y[i * sliceLength: (i + 1) * sliceLength], *args, **kwargs)
