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
import re
import Common
from common import Library
import Parser

testHeader = """
 void foo(int a, int b, int c);
 int baz();
extern char *bar123_xyz(int* a, const float* b, char c[16], uint32_t d[C1][C2]);
int funcWithBody(int foo, int bar) {
  someStatement();
  for (;;)
  {
    loop;
  }
  /* Commented } */
}

int noparams(void);
//int commented1();
/*int commented2();*/

void (* funcPointer (const char *name))();  
"""

testDefFile = """
EXPORT
  foo @ 1 NONAME
  bar123_xyz @ 2 NONAME
  baz @ 3 NONAME
  noparams @ 4 NONAME
  funcWithBody @ 5 NONAME
  funcPointer @ 6 NONAME
"""

testBadDefFile = """
EXPORT
  foo @ 1 NONAME
  bar123_xyz @ 1 NONAME
"""

testBadDefFile2 = """
EXPORT
  nosuchname @ 1 NONAME
"""

class ParserTest(unittest.TestCase):
  def testHeaderParsing(self):
    l = Parser.parseSource(testHeader)
    
    # check that everything is there
    assert "foo" in l.functions
    assert "bar123_xyz" in l.functions
    assert "baz" in l.functions
    
    # check the functions
    self.assertEqual(len(l.functions.keys()), 5)
    assert "a" in l.functions["foo"].parameters
    assert "b" in l.functions["foo"].parameters
    assert "c" in l.functions["foo"].parameters
    self.assertEqual(l.functions["foo"].parameters["a"].type.declaration, "int")
    self.assertEqual(l.functions["foo"].parameters["b"].type.declaration, "int")
    self.assertEqual(l.functions["foo"].parameters["c"].type.declaration, "int")

    assert "a" in l.functions["bar123_xyz"].parameters
    assert "b" in l.functions["bar123_xyz"].parameters
    assert "c" in l.functions["bar123_xyz"].parameters
    assert "d" in l.functions["bar123_xyz"].parameters
    self.assertEqual(l.functions["bar123_xyz"].parameters["a"].type.declaration, "int *")
    self.assertEqual(l.functions["bar123_xyz"].parameters["b"].type.declaration, "const float *")
    self.assertEqual(l.functions["bar123_xyz"].parameters["c"].type.declaration, "char")
    self.assertEqual(l.functions["bar123_xyz"].parameters["c"].type.dimension, ['16'])
    self.assertEqual(l.functions["bar123_xyz"].parameters["d"].type.declaration, "uint32_t")
    self.assertEqual(l.functions["bar123_xyz"].parameters["d"].type.dimension, ['C1', 'C2'])
    assert len(l.functions["noparams"].parameters) == 0

    self.assertEqual(l.functions["funcWithBody"].parameters["foo"].type.declaration, "int")
    self.assertEqual(l.functions["funcWithBody"].parameters["bar"].type.declaration, "int")
    #self.assertEqual(l.functions["funcWithBody"].body, "{ someStatement ( ) ; for ( ; ; ) { loop ; } }")

    # the function pointer case is not yet supported
    #assert "funcPointer" in l.functions
    #assert "name" in l.functions["funcPointer"].parameters
    
  def testDefFile(self):
    l = Parser.parseSource(testHeader)
    
    # FIXME: hardcode the function pointer since we don't parse those yet
    l.functions["funcPointer"] = Library.Function("funcPointer", "void")

    for function, ordinal in Parser.parseDefFile(testDefFile):
      l.functions[function].ordinal = ordinal
    
    # check that all ordinals were read    
    for f in l.functions.values():
      assert f.ordinal is not None

    # check overlapping ordinals in input
    try:
      Parser.parseDefFile(testBadDefFile)
      self.fail("ValueError not raised.")
    except ValueError:
      pass
          
  def testGLHeaderParsing(self):
    header = open("data/gl-cpp.h").read()
    
    functions = re.findall(r".+\s(\w+)\s*\(.+\);", header)
    
    l = Parser.parseSource(header)
    assert len(l.functions) == len(functions)

    # check that all functions are there    
    for f in functions:
      assert f in l.functions
      
    # check that nothing extra is there
    for f in l.functions.keys():
      assert f in functions
      
    # read in the ordinals
    defFile = open("data/opengles11u.def").read()
    
    # allow missing functions since we don't have the EGL header
    for function, ordinal in Parser.parseDefFile(defFile):
      if function in l.functions:
        l.functions[function].ordinal = ordinal
    
    # make sure all functions have ordinals
    for f in l.functions.values():
      # extensions don't have ordinals
      if f.name.endswith("OES"): continue
      assert f.ordinal is not None
      
if __name__ == "__main__":
  unittest.main()
