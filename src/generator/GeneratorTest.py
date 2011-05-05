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
from StringIO import StringIO

import Parser
from common import Config
from common import Library
from Generator import TracerGenerator, PlayerGenerator
from Platform import SymbianPlatform
from Cheetah.Template import Template

class GeneratorTest(unittest.TestCase):
  def setUp(self):
    # Create a configuration
    self.config = Config.Config()
    self.config["library"] = "testlib"
    
    # Create a basic library for testing
    self.library = Library.Library("test")

    voidType = Library.Type("void")
    intType  = Library.Type("int")
        
    f = Library.Function("func1", voidType)
    p = Library.Parameter("parameter", intType)
    f.parameters[p.name] = p    
    self.library.functions[f.name] = f
    
    f = Library.Function("func2", intType)
    p1 = Library.Parameter("p1", intType)
    p2 = Library.Parameter("p2", intType)
    f.parameters[p1.name] = p1    
    f.parameters[p2.name] = p2    
    self.library.functions[f.name] = f
    
    # Register some types
    self.library.typeMap["int"]  = "int"
    self.library.typeMap["void"] = "void"

    # Define platform properties    
    self.platform = SymbianPlatform(self.config)
  
  def testCheetah(self):
    t = Template("Hello, $who!")
    t.who = "World"
    self.assertEquals(str(t), "Hello, World!")
    
  def testBasicTracerGeneration(self):
    g = TracerGenerator(self.config, self.platform, [self.library])
    o = StringIO()
    g.generate(outputFile = o)

  def testGlTracerGeneration(self):
    # Parse the preprocessed GL header
    l = Library.Library("GL", "libgles_cm_orig.dll")
    l.merge(Parser.parseSource(open("data/gl-cpp.h").read()))
    l.merge(Parser.parseSource(open("data/egl-cpp.h").read()))
    defFile = open("data/opengles11u.def").read()
    for function, ordinal in Parser.parseDefFile(defFile):
      if function in l.functions:
        l.functions[function].ordinal = ordinal
    
    # Add a custom function hook
    customSource = """
    GLenum xglGetError(void)
    {
      wtf();
    }
    """
    l.merge(Parser.parseSource(customSource))
    T = Library.Type

    # GL types
    l.typeMap[T("GLenum")]     = "int"
    l.typeMap[T("GLint")]      = "int"
    l.typeMap[T("GLintptr")]   = "int"
    l.typeMap[T("GLuint")]     = "int"
    l.typeMap[T("GLboolean")]  = "int"
    l.typeMap[T("GLbitfield")] = "int"
    l.typeMap[T("GLsizei")]    = "int"
    l.typeMap[T("GLsizeiptr")] = "int"
    l.typeMap[T("GLfixed")]    = "int"
    l.typeMap[T("GLclampx")]   = "int"
    l.typeMap[T("GLubyte")]    = "byte"
    l.typeMap[T("GLshort")]    = "short"
    l.typeMap[T("GLclampf")]   = "float"
    l.typeMap[T("GLfloat")]    = "float"
    l.typeMap[T("void")]       = "void"
    
    # EGL types
    l.typeMap[T("EGLboolean")]         = "int"
    l.typeMap[T("EGLint")]             = "int"
    l.typeMap[T("EGLDisplay")]         = "int"
    l.typeMap[T("EGLSurface")]         = "int"
    l.typeMap[T("EGLContext")]         = "int"
    l.typeMap[T("EGLConfig")]          = "int"
    l.typeMap[T("EGLBoolean")]         = "int"
    l.typeMap[T("NativeDisplayType")]  = "int"
    l.typeMap[T("NativeWindowType")]   = "pointer"
    l.typeMap[T("NativePixmapType")]   = "pointer"
    
    # Mark eglTerminate as a terminator
    l.functions["eglTerminate"].isTerminator = True
    
    g = TracerGenerator(self.config, self.platform, [l])
    g.generate(open("generator_test_output.txt", "w"))
    
  def testBasicPlayerGeneration(self):
    g = PlayerGenerator(self.config, self.platform, [self.library])
    o = StringIO()
    g.generate(outputFile = o)
    
if __name__ == "__main__":
  unittest.main()
