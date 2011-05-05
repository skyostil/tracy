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

"""
A parser for converting C source code, DEF files and other source
files into an API DOM model.
"""

import Common
from common.pyparsing import *
from common import OrderedDict
from common import Log
from common import Library
from common import Task
from Library import Library, Function, Type, Parameter

# Enable packrat parsing
ParserElement.enablePackrat()

def splitVariable(s, pos, tokens):
  """
  Split a list of variable definition tokens (e.g. const char * foo) into
  a type declaration and the variable name.
  """
  if len(tokens) == 1:
    # Handle unnamed variables
    name, typeDec, arrayDec = None, [], None
  elif type(tokens[-1]) == ParseResults:
    # Handle array declarations
    name, typeDec, arrayDec = tokens[-2], tokens[:-2], list(tokens[-1])
  else:
    name, typeDec, arrayDec = tokens[-1], tokens[:-1], None
  
  return (name, Type(" ".join(typeDec), arrayDec))
  
def parseBlock(s, pos, tokens):
  # Find the matching closing brace
  depth = 0
  for i, c in enumerate(s[pos:]):
    if c == "{":
      depth += 1
    elif c == "}":
      depth -= 1
      if depth == 0:
        return s[pos:pos + i + 1]

def parseInteger(s, pos, tokens):
  text = tokens[0]
  if text.startswith("0x"):
    return int(text, 16)
  return int(text)

# Parses constants of the type (n << m) - p
def parseBitShiftConstant(s, pos, tokens):
  try:
    c1, c2 = tokens[0]
    return int(c1) << int(c2)
  except ValueError:
    c1, c2, c3 = tokens[0]
    return (int(c1) << int(c2)) - c3

#
# Rudimentary C source grammar
#
# Limitations:
#
#  - Macros are not processed.
#  - The function body parser eats newlines and whitespace.
#  - Only very basic function pointers are supported.
#  - Structure typedefs are not supported.
#

# Make line feeds white space. Note that this breaks any syntax based on LineStart(), etc.
ParserElement.setDefaultWhitespaceChars(" \t\n\r")

identifier      = Word(alphas + "_@.", alphanums + "_@.")
integer         = Word(alphanums + "-").setParseAction(parseInteger)
arraySpecifier  = Group(OneOrMore(Suppress("[") + Word(alphanums) + Suppress("]")))
variable        = OneOrMore(identifier | "*" | "&" | arraySpecifier).setParseAction(splitVariable)
parameterList   = Suppress("(") + Optional(delimitedList(variable)) + Suppress(")")
preprocessor    = "#" + SkipTo(LineEnd(), include = True)
cppStyleComment = Literal("//") + SkipTo(LineEnd(), include = True)
operator        = oneOf("+ - * / & % . , : ; = ( ) [ ] < > !")
block           = Literal("{").setParseAction(parseBlock)
storageClass    = oneOf("auto register static extern typedef")
function        = Optional(storageClass).setResultsName("storage") + \
                  Group(variable).setResultsName("name") + \
                  parameterList.setResultsName("args") + \
                  (";" | block.setResultsName("body"))
function.ignore(cStyleComment)
function.ignore(cppStyleComment)
function.ignore(preprocessor)
bitshiftConstant = Group(Suppress(Optional("(")) + \
                   integer + \
                   Suppress("<<") + \
                   integer + \
                   Suppress(Optional(")")) + \
                   Optional(Suppress("-")) + \
                   Optional(integer)).setParseAction(parseBitShiftConstant)
enumerator       = Group(identifier + \
                   Optional(Suppress("=") + (identifier | integer | bitshiftConstant), default = None))
enumSpecifier    = Literal("enum") + Optional(identifier) + Suppress("{") + Group(delimitedList(enumerator)) + Suppress(Optional(",")) + Suppress("}")
typedefDecl      = Suppress("typedef") + ((enumSpecifier + identifier) | variable) + Suppress(";")
typedefDecl.ignore(cStyleComment)
typedefDecl.ignore(cppStyleComment)
typedefDecl.ignore(preprocessor)

"""
expression              = Forward()
assignmentExpression    = Forward()
typeName                = Forward()
castExpression          = Forward()
declaration             = Forward()
statement               = Forward()
identifier              = Word(alphas + "_", alphanums + "_")
typedefName             = Word(alphas + "_", alphanums + "_")
constant                = Word(alphas + "_", alphanums + "_")
postfixExpression       = (identifier | constant | quotedString | "(" + expression + ")") + \
                          ZeroOrMore("[" + expression + "]" | \
                                     "(" + delimitedList(assignmentExpression) + ")" | \
                                     "." + identifier | \
                                     "->" + identifier | \
                                     "++" | \
                                     "--")
unaryExpression         = ZeroOrMore(oneOf("++ -- sizeof")) + \
                          Literal("sizeof") + "(" + typeName + ")" | \
                          oneOf("& * + -  ~ !") + castExpression    | \
                          postfixExpression
castExpression         << ZeroOrMore("(" + typeName + ")") + unaryExpression
multiplicativeExpression= castExpression + ZeroOrMore(oneOf("* / %") + castExpression)
additiveExpression      = multiplicativeExpression + ZeroOrMore(oneOf("+ -") + multiplicativeExpression)
shiftExpression         = additiveExpression + ZeroOrMore(oneOf("<< >>") + additiveExpression)
relationalExpression    = shiftExpression + ZeroOrMore(oneOf("< > <= >=") + shiftExpression)
equalityExpression      = relationalExpression + ZeroOrMore(oneOf("== !=") + relationalExpression)
andExpression           = equalityExpression + ZeroOrMore("&" + equalityExpression)
exclusiveOrExpression   = andExpression + ZeroOrMore("^" + andExpression)
inclusiveOrExpression   = exclusiveOrExpression + ZeroOrMore("|" + exclusiveOrExpression)
logicalAndExpression    = inclusiveOrExpression + ZeroOrMore("&&" + inclusiveOrExpression)
logicalOrExpression     = logicalAndExpression + ZeroOrMore("||" + logicalAndExpression)
conditionalExpression   = Forward()
conditionalExpression  << logicalOrExpression + Optional("?" + expression + ":" + conditionalExpression)
constantExpression      = conditionalExpression
assignmentExpression   << ZeroOrMore(unaryExpression + oneOf("= *= /= %= += -= <<= >>= &= ^= |=")) + conditionalExpression
expression             << delimitedList(assignmentExpression)
block                   = Literal("{") + ZeroOrMore(declaration) + ZeroOrMore(statement) + "}"
statement              << ZeroOrMore((identifier | "case" + constantExpression | "default") + ":") + \
                          (Optional(expression) + ";" | block | \
                          Literal("if") + "(" + expression + ")" + statement | \
                          Literal("if") + "(" + expression + ")" + statement + "else" + statement | \
                          Literal("switch") + "(" + expression + ")" + statement | \
                          Literal("while") + "(" + expression + ")" + statement | \
                          Literal("do") + statement + Literal("while") + "(" + expression + ")" + ";" | \
                          Literal("for") + "(" + Optional(expression) + ";" + Optional(expression) + ";" + Optional(expression) + ")" + statement | \
                          Literal("goto") + identifier + ";" | \
                          Literal("continue") + ";" | \
                          Literal("break") + ";" | \
                          Literal("return") + expression + ";")
typeQualifier           = oneOf("const volatile")
pointer                 = ZeroOrMore("*" + ZeroOrMore(typeQualifier))
parameterTypeList       = Forward()
abstractDeclarator      = Forward()
abstractDeclarator     << pointer + Optional("(" + abstractDeclarator + ")") + \
                          ZeroOrMore("[" + Optional(constantExpression) + "]" | "(" + Optional(parameterTypeList) + ")")
typeSpecifier           = Forward()
typeName               << OneOrMore(typeSpecifier | typeQualifier) + Optional(abstractDeclarator)
initializer             = Forward()
initializer            << assignmentExpression | "{" + delimitedList(initializer) + Optional(",") + "}"
declarationSpecifiers   = Forward()
declarator              = Forward()
declarator             << Optional(pointer) + (identifier | "(" + declarator + ")") + \
                          ZeroOrMore("[" + Optional(constantExpression) + "]" |
                                     "(" + parameterTypeList + ")" |
                                     "(" + Optional(delimitedList(identifier)) + ")")
parameterDeclaration    = declarationSpecifiers + Optional(declarator | abstractDeclarator)
parameterTypeList      << delimitedList(parameterDeclaration) + Optional(Literal(",") + "...")
enumerator              = identifier + Optional("=" + constantExpression)
enumSpecifier           = Literal("enum") + (identifier | Optional(identifier) + "{" + delimitedList(enumerator) + "}")
structDeclarator        = declarator | Optional(declarator) + ":" + constantExpression
structDeclaration       = OneOrMore(typeSpecifier | typeQualifier) + delimitedList(structDeclarator)
initDeclarator          = declarator + Optional("=" + initializer)
structOrUnionSpecifier  = oneOf("struct union") + (Optional(identifier) + "{" + OneOrMore(structDeclaration) + "}" | identifier)
typeSpecifier          << oneOf("void char short int long float double signed unsigned") | \
                          structOrUnionSpecifier | enumSpecifier | typedefName
storageClassSpecifier   = oneOf("auto register static extern typedef")
declarationSpecifiers  << OneOrMore(storageClassSpecifier | typeSpecifier | typeQualifier)
declaration            << declarationSpecifiers + delimitedList(initDeclarator) + ";"
functionDefinition      = Optional(declarationSpecifiers) + declarator + ZeroOrMore(declaration) + (block.setResultsName("body") | ";")
translationUnit         = OneOrMore(functionDefinition | declaration)
cppStyleComment         = Literal("//") + SkipTo(LineEnd(), include = True)
preprocessor            = LineStart() + "#" + SkipTo(LineEnd(), include = True)
functionDefinition.ignore(cStyleComment)
functionDefinition.ignore(cppStyleComment)
functionDefinition.ignore(preprocessor)
#functionDefinition.enablePackrat()
"""

# DEF file grammar
symbol          = Word(alphas + "_@.?", alphanums + "_@.?")
functionOrdinal = symbol.setResultsName("name") + Literal("@") + integer.setResultsName("ordinal")

# Preprocessor macro definition
preprocessorCommand = LineStart() + Suppress("#") + \
                      identifier.setResultsName("command") + \
                      identifier.setResultsName("name") + \
                      SkipTo(LineEnd(), include = True).setResultsName("value")

class ParserError(Exception):
  """Base class for parser exceptions."""
  pass

def parseSource(source):
  """
  Parses the given source string and adds all found functions
  to the library.
    
  @param header:    Source text
    
  @returns: A library containing the parsed functions
  """
  task    = Task.startTask("parser", "Parsing type declarations", len(source))
  library = Library()

  # Read the type definitions
  typedefs = []
  for match, pos, l in typedefDecl.scanString(source):
    task.updateProgress(pos)
    # Map enumerations to integers
    if match[0] == "enum":
      name = match[-1]
      type = Type("int")
      for i, e in enumerate(match[1]):
        enumeration, value = e
        if value is None:
          value = i
        type.addSymbolicConstant(enumeration, value)
        library.constants[enumeration] = value
    else:
      name, type = match[0]
    library.typeDefs[Type(name)] = type
  task.finish()

  # Read preprocessor constants
  task = Task.startTask("parser", "Parsing preprocessor constants", len(source))
  for match, pos, l in preprocessorCommand.scanString(source):
    task.updateProgress(pos)
    # Only look for #define
    if match.command != "define":
      continue
    
    # Only accept integral constants
    try:
      value = parseInteger(None, None, [match.value])
    except ValueError:
      continue
    
    library.constants[match.name] = value
  task.finish()

  def where(pos):
    return "Line %d, column %s" % (lineno(pos, source), col(pos, source))
  
  task = Task.startTask("parser", "Parsing functions", len(source))
  for match, pos, l in function.scanString(source):
    task.updateProgress(pos)
    name, type = match.name[0]
    if not name: continue
    f = Function(name, type)
    f.language = "c"
    for name, type in match.args:
      if not name or not type:
        continue
      f.parameters[name] = Parameter(name, type)

    # Grab the code if any
    if match.body:
      f.body = match.body

    library.functions[f.name] = f
  task.finish()
  return library

def parseVariableDeclaration(declaration):
  """
  Parse a single variable declaration and return a (name, Type instance) tuple.
  """
  return variable.parseString(declaration)[0]

def parseDefFile(defFile):
  """
  Parses the given DEF file string and returns all found ordinals.
    
  @param defFile:                 DEF file text
    
  @return: A list of found function and ordinal pairs
  """
  functions = []
  ordinals = []
  for match, pos, l in functionOrdinal.scanString(defFile):
    if match.ordinal in ordinals:
      raise ValueError("The ordinal %d is assigned to multiple functions." % match.ordinal)
    ordinals.append(match.ordinal)
    functions.append((match.name, match.ordinal))
  return functions
