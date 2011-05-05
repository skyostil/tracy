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

import Trace
import BinaryCodec
import BinaryCodec1
import sys

def usage():
    return """Program for upgrading binary trace files in to the latest revision 
of the binary format.

Supported output format: %s
Supported input formats: TCY1

Usage:
    TraceUpgrade.py [trace.bin] [output.bin]
""" % BinaryCodec.FORMAT_VERSION

def readTCY1(fileName):
    f = open(traceIn, "rb")
    trace = Trace.Trace()
    reader = BinaryCodec1.Reader(trace, open(fileName, "rb"))
    reader.load()
    return trace

if __name__ == "__main__":
    if not len(sys.argv) == 3:
        print usage()
        sys.exit(1)

    traceIn, traceOut = sys.argv[1:]
    
    # Just one legacy format supported so far
    trace = readTCY1(traceIn)

    # Save it with the latest format
    writer = BinaryCodec.Writer(trace, open(traceOut, "wb"))
    writer.save()

