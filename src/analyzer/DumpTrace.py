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

import sys
import binascii
import Trace
import BinaryCodec
from optparse import OptionParser

def parseCommandLine(args):
  parser = OptionParser(usage = "Usage: %prog [options] [trace file]")
  parser.add_option("-v", "--verbose", action = "store_true", help = "show verbose status messages")
  parser.add_option("-t", "--tokens", action = "store_true", help = "show raw trace file tokens")
  options, args = parser.parse_args(args)
  
  return options, args

# Parse the command line options
options, args = parseCommandLine(sys.argv[1:])

if not args:
  print "Trace file missing."
  sys.exit(1)

trace  = Trace.Trace()
reader = BinaryCodec.Reader(trace, open(args[0], "rb"))

if options.tokens:
  version = reader.file.read(4)
  print "Version:", version
  if not version.startswith("TCY"):
    reader.file.seek(0)
  while 1:
    tokenStart = reader.file.tell()
    token = BinaryCodec.Token.read(reader.file)
    tokenEnd = reader.file.tell()
    
    if not token:
      break

    reader.file.seek(tokenStart)
    tokenData = reader.file.read(tokenEnd - tokenStart)
    reader.file.seek(tokenEnd)

    tokenData = binascii.hexlify(tokenData)
    
    print "%08x:" % tokenStart,
    for i in range(0, len(tokenData), 2):
      print tokenData[i:i+2],
    print
    print "--------:", token
  sys.exit(0)
  
reader.load()

for event in trace.events:
  # Print out any modified arrays that are not actual parameters for the event.
  for array in event.modifiedArrays:
    for value in event.values.values():
      if isinstance(value, Trace.Array) and value.id == array.id:
        break
    else:
      print "%010d %010d @array 0x%x = %s" % (event.seq, event.time, array.id, array)
  print "%010d %010d %s (%s)" % (event.seq, event.time, event.name, ", ".join(["%s=%s" % (k, v) for k, v in event.values.items() if k])),
  if None in event.values:
    print "->", event.values[None]
  else:
    print
