#
# Graphics operations
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
#

import Image
import ImageMath

debug = False

# Lookup table for converting from sRGB ("standard" RGB) color space to lRGB (linear RGB) color space.
sRGB_to_lRGB = [
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 
	0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 
	0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 
	0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x07, 
	0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 
	0x0a, 0x0a, 0x0a, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 
	0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0f, 0x0f, 0x10, 
	0x10, 0x11, 0x11, 0x11, 0x12, 0x12, 0x13, 0x13, 
	0x14, 0x15, 0x15, 0x16, 0x16, 0x17, 0x17, 0x18, 
	0x18, 0x19, 0x1a, 0x1a, 0x1b, 0x1b, 0x1c, 0x1d, 
	0x1d, 0x1e, 0x1f, 0x1f, 0x20, 0x21, 0x21, 0x22, 
	0x23, 0x24, 0x24, 0x25, 0x26, 0x27, 0x27, 0x28, 
	0x29, 0x2a, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 
	0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 
	0x37, 0x38, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 
	0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x45, 0x46, 
	0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 
	0x50, 0x51, 0x52, 0x53, 0x54, 0x56, 0x57, 0x58, 
	0x59, 0x5a, 0x5c, 0x5d, 0x5e, 0x60, 0x61, 0x62, 
	0x63, 0x65, 0x66, 0x67, 0x69, 0x6a, 0x6b, 0x6d, 
	0x6e, 0x70, 0x71, 0x73, 0x74, 0x75, 0x77, 0x78, 
	0x7a, 0x7b, 0x7d, 0x7e, 0x80, 0x81, 0x83, 0x84, 
	0x86, 0x88, 0x89, 0x8b, 0x8c, 0x8e, 0x90, 0x91, 
	0x93, 0x94, 0x96, 0x98, 0x9a, 0x9b, 0x9d, 0x9f, 
	0xa0, 0xa2, 0xa4, 0xa6, 0xa7, 0xa9, 0xab, 0xad, 
	0xaf, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 
	0xbe, 0xc0, 0xc1, 0xc3, 0xc5, 0xc7, 0xc9, 0xcb, 
	0xcd, 0xcf, 0xd1, 0xd3, 0xd5, 0xd7, 0xda, 0xdc, 
	0xde, 0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xeb, 0xed, 
	0xef, 0xf1, 0xf3, 0xf6, 0xf8, 0xfa, 0xfc, 0xff, 
]

def log2(n):
  m = -1
  while n > 0:
    n >>= 1
    m += 1
  return m

def decodeImageData(data, size, stride, redMask, greenMask, blueMask, alphaMask, 
                    isLinear = True, isPremultiplied = False):
  w, h  = size[0], size[1]

  if debug:
    print "Decoding image data:"
    print "    Red mask: %08x" % redMask
    print "  Green mask: %08x" % greenMask
    print "   Blue mask: %08x" % blueMask
    print "  Alpha mask: %08x" % alphaMask
  
  # Imaging library fast paths
  if   redMask   == 0xff000000 and \
       greenMask == 0x00ff0000 and \
       blueMask  == 0x0000ff00 and \
       alphaMask == 0x000000ff:
    image = Image.fromstring("RGBA", size, data, "raw", "ABGR", stride)
  elif redMask   == 0x000000ff and \
       greenMask == 0x0000ff00 and \
       blueMask  == 0x00ff0000 and \
       alphaMask == 0xff000000:
    image = Image.fromstring("RGBA", size, data, "raw", "RGBA", stride)
  elif redMask   == 0x0000ff00 and \
       greenMask == 0x00ff0000 and \
       blueMask  == 0xff000000 and \
       alphaMask == 0x000000ff:
    image = Image.fromstring("RGBA", size, data, "raw", "ARGB", stride)
  elif redMask   == 0x00ff0000 and \
       greenMask == 0x0000ff00 and \
       blueMask  == 0x000000ff and \
       alphaMask == 0xff000000:
    image = Image.fromstring("RGBA", size, data, "raw", "BGRA", stride)
  elif redMask   == 0x00ff0000 and \
       greenMask == 0x0000ff00 and \
       blueMask  == 0x000000ff and \
       alphaMask == 0x00000000:
    image = Image.fromstring("RGB", size, data, "raw", "BGR", stride)
  elif redMask   == 0x000000ff and \
       greenMask == 0x0000ff00 and \
       blueMask  == 0x00ff00ff and \
       alphaMask == 0x00000000:
    image = Image.fromstring("RGB", size, data, "raw", "RGB", stride)
  else:
    redOffset   = log2((1 << (log2(redMask)   + 1)) - redMask)
    greenOffset = log2((1 << (log2(greenMask) + 1)) - greenMask)
    blueOffset  = log2((1 << (log2(blueMask)  + 1)) - blueMask)
    alphaOffset = log2((1 << (log2(alphaMask) + 1)) - alphaMask)
    
    redBits     = log2(redMask   >> redOffset)   + 1
    greenBits   = log2(greenMask >> greenOffset) + 1
    blueBits    = log2(blueMask  >> blueOffset)  + 1
    alphaBits   = log2(alphaMask >> alphaOffset) + 1
    bpp         = redBits + greenBits + blueBits + alphaBits
    
    if debug:
      print "Warning: No fast path for image format"

    image   = Image.new(alphaMask and "RGBA" or "RGBX", (w, h))
    dataOut = range(w * h)
    dataPos = 0
    
    for y in range(h):
      scanline = data[y * stride : (y + 1) * stride]
      for x in range(w):
        pixel = scanline[x * bpp : (x + 1) * bpp]
        pixel = sum([ord(p) * (1 << (i * 8)) for i, p in enumerate(pixel)])
        if redBits:
          red   = (255 * ((pixel >> redOffset)   & redMask))   / ((1 << redBits)   - 1)
        else:
          red   = 0
        if greenBits:
          green = (255 * ((pixel >> greenOffset) & greenMask)) / ((1 << greenBits) - 1)
        else:
          green = 0
        if blueBits:
          blue  = (255 * ((pixel >> blueOffset)  & blueMask))  / ((1 << blueBits)  - 1)
        else:
          blue  = 0
        if alphaBits:
          alpha = (255 * ((pixel >> alphaOffset) & alphaMask)) / ((1 << alphaBits) - 1)
        else:
          alpha = 255
          
        dataOut[dataPos] = (red, green, blue, alpha)
        dataPos += 1
    image.putdata(dataOut)

  if not isLinear:
    if alphaMask:
      r, g, b, a = image.split()
      r = r.point(sRGB_to_lRGB)
      g = g.point(sRGB_to_lRGB)
      b = b.point(sRGB_to_lRGB)
      image = Image.merge(image.mode, (r, g, b, a))
    else:
      r, g, b = image.split()
      r = r.point(sRGB_to_lRGB)
      g = g.point(sRGB_to_lRGB)
      b = b.point(sRGB_to_lRGB)
      image = Image.merge(image.mode, (r, g, b))
    
  if isPremultiplied and alphaMask:
    r, g, b, a = image.split()
    r = ImageMath.eval("convert(255 * color / alpha, 'L')", color = r, alpha = a)
    g = ImageMath.eval("convert(255 * color / alpha, 'L')", color = g, alpha = a)
    b = ImageMath.eval("convert(255 * color / alpha, 'L')", color = b, alpha = a)
    image = Image.merge(image.mode, (r, g, b, a))
  
  return image

def compareDepthBuffers(fileName1, fileName2, stride, mask):
  if not mask:
    return 0

  f1   = open(fileName1, "rb")
  f2   = open(fileName2, "rb")
  bpp  = (log2(mask) + 1) / 16
  diff = 0
  
  while True:
    scanline1 = f1.read(stride)
    scanline2 = f2.read(stride)
    
    if not scanline1 or not scanline2:
      break

    for i in range(0, stride, bpp):
      z1 = scanline1[i:i + bpp]
      z2 = scanline2[i:i + bpp]
      if z1 != z2:
        diff += 1
  return diff
