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

import unittest
import Common
from Library import Library, Type

class LibraryTest(unittest.TestCase):
  def testNativeTypes(self):
    l = Library()
    
    # Basic C types
    typePairs = [
      ("void",             "void"),
      ("char",             "byte"),
      ("short",            "short"),
      ("int",              "int"),
      ("long",             "long"),
      ("float",            "float"),
      ("double",           "double"),
      ("enum",             "int"),
      ("unsigned char",    "byte"),
      ("unsigned short",   "short"),
      ("unsigned int",     "int"),
      ("unsigned long",    "long"),
      ("signed char",      "byte"),
      ("signed short",     "short"),
      ("signed int",       "int"),
      ("signed long",      "long"),
      ("const char",       "byte"),
      ("const short",      "short"),
      ("const int",        "int"),
      ("const long",       "long"),
      ("const unsigned char",  "byte"),
      ("const unsigned short", "short"),
      ("const unsigned int",   "int"),
      ("const unsigned long",  "long"),
      ("const signed char",  "byte"),
      ("const signed short", "short"),
      ("const signed int",   "int"),
      ("const signed long",  "long"),
    ]
    
    for cType, nativeType in typePairs:
      self.assertEquals(l.getNativeType(Type(cType)), nativeType)

  def testTypedefs(self):
    l = Library()
    l.typeDefs[Type("GLbyte")] = Type("char")
    self.assertEquals(l.getNativeType(Type("GLbyte")), "byte")
    self.assertEquals(l.getNativeType(Type("const GLbyte")), "byte")

  def testPointers(self):
    l = Library()
    
    # Basic C types
    pointerTypes = [
      "void*",
      "char*",
      "short*",
      "int*",
      "long*",
      "float*",
      "double*",
      "unsigned char*",
      "unsigned short*",
      "unsigned int*",
      "unsigned long*",
      "signed char*",
      "signed short*",
      "signed int*",
      "signed long*",
      "const void*",
      "const char*",
      "const short*",
      "const int*",
      "const long*",
      "const unsigned char*",
      "const unsigned short*",
      "const unsigned int*",
      "const unsigned long*",
      "const signed char*",
      "const signed short*",
      "const signed int*",
      "const signed long*",
    ]

    for cType in pointerTypes:
      self.assertEquals(l.getNativeType(Type(cType)), "pointer")

  def testObjects(self):
    l = Library()
    l.typeMap[Type("Window*")] = "object"
    l.typeMap[Type("const Window*")] = "object"
    self.assertEquals(l.getNativeType(Type("Window*")), "object")
    self.assertEquals(l.getNativeType(Type("const Window*")), "object")

if __name__ == "__main__":
  unittest.main()
