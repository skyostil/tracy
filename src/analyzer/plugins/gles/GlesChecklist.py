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

"""Checklist of common GLES performance problems."""

from Checklist import ChecklistItem, commonChecklistItems

class MipmapUsage(ChecklistItem):
  name        = "Mipmap usage"
  description = "Test if textures are rendered with mipmap filtering, " + \
                "which reduces memory accesses and improves image quality. " + \
                "Usage of bilinear filtering is also checked, since it is " + \
                "a cheap way to improve image quality."
  
  def initialize(self):
    self.mipmapFilters = [
      self.constants.GL_NEAREST_MIPMAP_NEAREST,
      self.constants.GL_LINEAR_MIPMAP_NEAREST,
      self.constants.GL_NEAREST_MIPMAP_LINEAR,
      self.constants.GL_LINEAR_MIPMAP_LINEAR,
    ]
    self.nearestFilters = [
      self.constants.GL_NEAREST,
      self.constants.GL_NEAREST_MIPMAP_NEAREST,
      self.constants.GL_NEAREST_MIPMAP_LINEAR,
    ]
  
  def process(self, event):
    if event.name.startswith("glDraw"):
      # Check the filtering modes of all texture units
      for path in self.state.getValues(["ctx", "texunit*", "target*", "handle"]):
        texHandle = int(path[-1][1])
        path = self.state.getValues(["ctx", ("texture", texHandle), ("parameter", self.constants.GL_TEXTURE_MIN_FILTER), "value"])

        if not path:
          continue
          
        filterMode = int(path[0][-1][1])
        texUnit    = path[1][1] or "0"
        if filterMode not in self.mipmapFilters:
          filterName = self.constantNames.get(filterMode, hex(filterMode))
          self.fail(event, "Texture unit %s filtering mode %s does not use mipmaps." % (texUnit, filterName))
        if filterMode in self.nearestFilters:
          filterName = self.constantNames.get(filterMode, hex(filterMode))
          self.fail(event, "Texture unit %s filtering mode %s does not use bilinear filtering." % (texUnit, filterName))

class SynchronousFunctions(ChecklistItem):
  name        = "Synchronous functions"
  description = "Test whether functions that cause the CPU to wait " + \
                "for the GPU are used. Such functions may have a dramatic " + \
                "negative effect on performance."
  
  def initialize(self):
    self.synchronousFunctions = [
      "glReadPixels",
      "glCopyTexImage",
      "glCopyTexSubImage",
      "eglCopyBuffers"
      "glFlush"
      "glFinish"
    ]
    self.synchFunctionFoundInFrame = False
    self.synchThreshold            = 10
    self.synchFrameCount           = 0
    self.frameCount                = 0
  
  def process(self, event):
    func = self.library.functions[event.name]
    if event.name in self.synchronousFunctions:
      self.comment(event, "Function %s inhibits rendering parallelism." % event.name)
      self.synchFunctionFoundInFrame = True
    elif func.isFrameMarker:
      if self.synchFunctionFoundInFrame:
        self.synchFrameCount += 1
      self.synchFunctionFoundInFrame = False
      self.frameCount               += 1
      
  def finalize(self):
    if self.synchFunctionFoundInFrame:
      self.synchFrameCount += 1
    if self.frameCount and 100 * self.synchFrameCount > self.frameCount * self.synchThreshold:
      self.fail(None, "%d%% of frames have synchronous function calls" % (100.0 * self.synchFrameCount / self.frameCount))

class DepthClearing(ChecklistItem):
  name        = "Depth buffer clearing"
  description = "Test if the depth buffer is cleared before rendering. " + \
                "Failing to clear the depth buffer may have a significant " + \
                "performance penalty on some architectures."
  
  def initialize(self):
    self.depthCleared = True
  
  def process(self, event):
    func = self.library.functions[event.name]
    if func.isFrameMarker:
      self.depthCleared = False
    elif event.name == "glClear":
      if int(event.values["mask"]) & self.constants.GL_DEPTH_BUFFER_BIT:
        self.depthCleared = True
    elif func.isRenderCall and not self.depthCleared:
      self.fail(event, "Depth buffer not cleared before rendering.")

class VboUsage(ChecklistItem):
  name        = "Vertex buffer object usage"
  description = "Test if mesh data is stored in vertex buffer objects."
  
  def initialize(self):
    self.arrays = ["vertex", "normal", "color", "pointsize", "matrixindex", "weight"]
  
  def process(self, event):
    if event.name.startswith("glDraw"):
      # Check the various arrays
      for array in self.arrays:
        try:
          if self.state.getValue(["ctx", "%s_pointer" % array]) and not self.state.getValue(["ctx", "%s_binding" % array]):
            self.fail(event, "%s data not stored in VBO." % array.capitalize())
        except KeyError:
          pass
      
      # Check the mesh indices
      elementBuffers = self.state.getValues(["ctx", ("buffer", self.constants.GL_ELEMENT_ARRAY_BUFFER), "handle"])
      if elementBuffers and elementBuffers[0][-1][1] == 0:
        self.fail(event, "Index data not stored in VBO.")

class VersionStringExamination(ChecklistItem):
  name        = "Renderer version string differentiation"
  description = "Test whether the GLES renderer version and extension strings are being examined. " + \
                "GLES extension availability differs from one renderer to another, so their " + \
                "presence should always be checked beforehand. Performance can also be improved " + \
                "by scaling graphics content complexity based on whether a software or hardware " +\
                "renderer is present."
  
  def initialize(self):
    self.stringsExamined = False
  
  def process(self, event):
    if event.name in ["eglQueryString", "glGetString"]:
      self.stringsExamined = True

  def finalize(self):
    if not self.stringsExamined:
      self.fail(None, "Renderer strings are not being examined with either glGetString() or eglQueryString().")

class TextureDataModification(ChecklistItem):
  name        = "Existing texture data modification"
  description = "Test whether the content of existing textures is modified during their lifetime. " + \
                "Modifying existing texture data is an expensive operation on most renderers."

  def initialize(self):
    # List of textures used within the current frame
    self.usedTextureHandles = set()
    
  def process(self, event):
    func = self.library.functions[event.name]
    
    if func.isFrameMarker:
      self.usedTextureHandles = set()
    elif func.isRenderCall:
      for path in self.state.getValues(["ctx", "texunit*", "target*", "handle"]):
        texHandle = int(path[-1][1])
        self.usedTextureHandles.add(texHandle)
    elif event.name in ["glTexImage2D", "glCompressedTexImage2D", "glCopyTexImage2D",
                        "glTexSubImage2D", "glCompressedTexSubImage2D", "glCopyTexSubImage2D"]:
      for path in self.state.getValues(["ctx", "texunit*", "target*", "handle"]):
        texHandle = int(path[-1][1])
        if texHandle in self.usedTextureHandles:
          self.fail(event, "Texture %d has already been used during this frame and modifying it might cause a pipeline stall." % texHandle)
    elif event.name in ["glTexParameteri", "glTexParameterx", "glTexParameteriv", "glTexParameterxv"]:
      # Check that we really change a texture parameter
      if "param" in self.stateModifiers:
        for path in self.state.getValues(["ctx", "texunit*", "target*", "handle"]):
          texHandle = int(path[-1][1])
          if texHandle in self.usedTextureHandles:
            self.fail(event, "Texture %d has already been used during this frame and " % texHandle + \
                             "modifying its parameters might cause a pipeline stall.")
          
    # Check texture data modification
    if event.name in ["glTexSubImage2D", "glCompressedTexSubImage2D", "glCopyTexSubImage2D"]:
      self.comment(event, "Modifying only a part of a texture might be an expensive operation.")

class TextureDataLoading(ChecklistItem):
  name        = "Loading texture data during frame rendering"
  description = "Check whether texture data is constantly being loaded while graphics are drawn. " + \
                "Generally texture data should be preloaded during the startup phase and only " + \
                "if needed during runtime."

  def initialize(self):
    self.texLoadInFrame          = False
    self.texLoadFrameThreshold   = 10
    self.texLoadFrameCount       = 0
    self.frameCount              = 0
    
  def process(self, event):
    func = self.library.functions[event.name]
    
    if func.isFrameMarker:
      if self.texLoadInFrame:
        self.texLoadFrameCount += 1
        self.texLoadInFrame     = False
      self.frameCount += 1
    elif event.name in ["glTexImage2D", "glCompressedTexImage2D", "glCopyTexImage2D",
                        "glTexSubImage2D", "glCompressedTexSubImage2D", "glCopyTexSubImage2D"]:
      self.texLoadInFrame = True

  def finalize(self):
    if self.texLoadInFrame:
      self.texLoadFrameCount += 1
    if 100 * self.texLoadFrameCount >= self.frameCount * self.texLoadFrameThreshold:
      self.fail(None, "%d%% of frames have texture loading traffic." % (100.0 * self.texLoadFrameCount / self.frameCount))

class TextureDataCompression(ChecklistItem):
  name        = "Texture data compression"
  description = "Test if texture data is compressed. When supported by the hardware, texture data " + \
                "compression decreases memory usage and improves rendering performance."

  def process(self, event):
    if event.name in ["glTexImage2D", "glTexSubImage2D"]:
      self.fail(event, "Texture data is not compressed.")

class TriangleStripGeometry(ChecklistItem):
  name        = "Triangle strip geometry"
  description = "Verify that complex geometry is stored in triangle strips. Triangle " + \
                "strips reduce the need to process the same vertices more than once " + \
                "and improve rendering performance for complex meshes."

  def process(self, event):
    if event.name in ["glDrawElements", "glDrawArrays"]:
      # Consider moderately complex meshes only
      try:
        if event.values["count"] >= 8 and event.values["mode"] != self.constants.GL_TRIANGLE_STRIP:
          self.fail(event, "Mesh geometry is not represented in triangle strips.")
      except KeyError:
        pass

class MultisamplingUsage(ChecklistItem):
  name        = "Multisample usage"
  description = "Check that antialiasing or multisampling is used. On hardware platforms " + \
                "multisampling is an easy way to improve image quality with a small " + \
                "performance cost."

  def process(self, event):
    if event.name.startswith("eglCreate") and "config" in event.values:
      config = event.values["config"]
      if config.attrs.get("samples", 1) <= 1:
        self.fail(event, "Specified EGL config does not use multisampling.")

# List of items to check
checklistItems = commonChecklistItems + [
  MipmapUsage,
  SynchronousFunctions,
  DepthClearing,
  VboUsage,
  VersionStringExamination,
  TextureDataModification,
  TextureDataLoading,
  TextureDataCompression,
  TriangleStripGeometry,
  MultisamplingUsage,
]
