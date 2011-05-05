# -*- encoding: iso-8859-1 -*-
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

def normalizeSiValue(v, unit, factor = 1000):
  """
  Normalize an SI unit to a practical range.
  """
  if v > factor:
    prefixes = ["", "k", "M", "G", "T"]
    for prefix in prefixes:
      if v > factor:
        v /= factor
      else:
        return "%.4f %s%s" % (v, prefix, unit)
  elif v < 1:
    prefixes = ["", "m", "µ", "n", "p"]
    for prefix in prefixes:
      if v < 1:
        v *= factor
      else:
        return "%.4f %s%s" % (v, prefix, unit)
  return "%.4f %s" % (v, unit)

def normalizeTimeValue(v, unit = "s"):
  """
  Normalize a time value in seconds to a meaningful time duration.
  """
  if v > 60:
    minutes = int(v / (60)) % 60
    hours   = int(v / (60 * 60)) % 24
    days    = int(v / (60 * 60 * 24))
    
    result = "%d min %.4f %s" % (minutes, v % 60, unit)
    
    if hours or days:
      result = "%d h " % (hours) + result
    if days:
      result = "%d days " % (days) + result

    return result
  elif v < 1:
    prefixes = ["", "m", "µ", "n", "p"]
    for prefix in prefixes:
      if v < 1:
        v *= 1000
      else:
        return "%.4f %s%s" % (v, prefix, unit)
  return "%.4f %s" % (v, unit)

def wordWrap(text, columns = 80, indent = "", extraIndent = ""):
  c      = 0
  output = []
  for word in text.split(" "):
    c += len(word) + 1
    if c >= columns - len(indent):
      output.append("\n" + indent + extraIndent + word)
      c = 0
    else:
      output.append(word)
  return indent + " ".join(output)

def ellipsis(text, maxLength = 32, trail = 5, ellipsis = "..."):
  text = str(text)
  if len(text) > maxLength:
    return text[:maxLength - trail - len(ellipsis)] + ellipsis + text[-trail:]
  return text

def decorateValue(library, function, name, value):
  if name is None:
    type           = function.type
    decoration     = None
    decorationHint = None
  else:
    type           = function.parameters[name].type
    decoration     = type.decoration
    decorationHint = type.decorationHint
  
  realType   = library.resolveType(type)
  nativeType = library.getNativeType(type)
  
  # Get a nice symbolic constant for this value if possible
  constants = {}
  if realType.symbolicConstants:
    constants = realType.symbolicConstants
  elif nativeType == "enum" or decoration == "enum":
    constants = library.constants
  elif decoration == "bitfield":
    flags = []
    bits  = value
    
    if decorationHint in library.typeDefs:
      constants      = library.typeDefs[decorationHint].symbolicConstants
      decorationHint = None
    else:
      constants = library.constants
    
    for k, v in constants.items():
      # If there is a decoration hint at this point, we require that the names of the bitfield 
      # members start with it.
      if decorationHint is not None and not re.match(decorationHint, k):
        continue
      if v > 0 and (v & (v - 1)) == 0 and (bits & v) == v:
        flags.append(k)
        bits &= ~v
    if flags:
      value = "|".join(flags)

  for k, v in constants.items():
    if decorationHint is not None and not re.match(decorationHint, k):
      continue
    if v == value:
      value = k
      break

  return value
