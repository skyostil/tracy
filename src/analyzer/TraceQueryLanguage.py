# Based on the SimpleSQL example from PyParsing.
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

from common import Task
import Trace
import Common
from common import pyparsing as p
from common import Collections

#
# Trace query language syntax definition
#
p.ParserElement.setDefaultWhitespaceChars(" \t\n")
selectStmt     = p.Forward()
selectToken    = p.Keyword("select", caseless = True)
fromToken      = p.Keyword("from", caseless = True)
intoToken      = p.Keyword("into", caseless = True)
distinctToken  = p.Keyword("distinct", caseless = True)
orderToken     = p.Keyword("order by", caseless = True)
descToken      = p.Keyword("desc", caseless = True)
ascToken       = p.Keyword("asc", caseless = True)
limitToken     = p.Keyword("limit", caseless = True)
ident          = p.Word(p.alphas, p.alphanums + "_$").setName("identifier")
columnName     = p.Group(p.delimitedList(ident, ".", combine = False))
columnNameList = p.Group(p.delimitedList(columnName))
columnSortList = p.Group(p.delimitedList(p.Group(columnName + p.Optional(descToken | ascToken, "asc"))))
tableName      = p.delimitedList(ident, ".", combine = True)
tableNameList  = p.Group(p.delimitedList(tableName ))

binaryOperators = {
  "=":    lambda a, b: a == b,
  "!=":   lambda a, b: a != b,
  "<":    lambda a, b: a < b,
  ">":    lambda a, b: a > b,
  ">=":   lambda a, b: a >= b,
  "<=":   lambda a, b: a <= b,
  "eq":   lambda a, b: a == b,
  "ne":   lambda a, b: a != b,
  "lt":   lambda a, b: a < b,
  "le":   lambda a, b: a <= b,
  "gt":   lambda a, b: a > b,
  "ge":   lambda a, b: a >= b,
  "+":    lambda a, b: a + b,
  "-":    lambda a, b: a - b,
  "*":    lambda a, b: a * b,
  "/":    lambda a, b: a / b,
  "%":    lambda a, b: a % b,
  "&":    lambda a, b: a & b,
  "|":    lambda a, b: a | b,
  "^":    lambda a, b: a ^ b,
  ">>":   lambda a, b: a << b,
  "<<":   lambda a, b: a >> b,
  "and ": lambda a, b: a and b,
  "or ":  lambda a, b: a or b,
}

unaryOperators = {
  "-":    lambda a: -a,
  "!":    lambda a: not a,
  "~":    lambda a: ~a,
  "not ": lambda a: not a,
}

precedence = {
  "=":   5,
  "!=":  5,
  "<":   6,
  ">":   6,
  ">=":  6,
  "<=":  6,
  "eq":  5,
  "ne":  5,
  "lt":  6,
  "le":  6,
  "gt":  6,
  "ge":  6,
  ">>":  7,
  "<<":  7,
  "+":   8,
  "-":   8,
  "*":   9,
  "/":   9,
  "%":   9,
  "!":   10,
  "not": 10,
  "~":   10,
  "&":   4,
  "^":   3,
  "|":   2,
  "and": 1,
  "or":  0,
  "(":   1000,
  ")":   -1,
  # Any other token
  None:  100,
}

#and_ = p.Keyword("and", caseless = True)
#or_  = p.Keyword("or", caseless = True)
in_  = p.Keyword("in", caseless = True)

E         = p.CaselessLiteral("E")
#binop     = p.oneOf("= != < > >= <= eq ne lt le gt ge and or not", caseless = True)
binop     = p.oneOf(binaryOperators.keys(), caseless = True)
arithSign = p.Word("+-", exact = True)
realNum   = p.Combine(p.Optional(arithSign) + (p.Word(p.nums) + "." + p.Optional(p.Word(p.nums)) |
                                               ( "." + p.Word(p.nums) ) ) + 
            p.Optional(E + p.Optional(arithSign) + p.Word(p.nums)))
intNum    = p.Combine(p.Optional(arithSign) + p.Word(p.nums) + 
                      p.Optional(E + p.Optional("+") + p.Word(p.nums)))

# Need to add support for alg expressions
#columnRval = realNum | intNum | p.quotedString | columnName
#whereCondition = p.Group(
#    (columnName + binop + columnRval) |
#    (columnName + in_ + "(" + p.delimitedList(columnRval) + ")") |
#    (columnName + in_ + "(" + selectStmt + ")") |
#    ("(" + whereExpression + ")")
#    )

algebraicExpression = p.Forward()
lpar = p.Literal("(")
rpar = p.Literal(")")
atom = realNum | intNum | columnName | p.QuotedString('"') | p.QuotedString("'") | (lpar + algebraicExpression + rpar)
algebraicExpression << atom + p.ZeroOrMore(binop + atom)

#whereExpression << whereCondition + p.ZeroOrMore(( and_ | or_ ) + whereExpression) 
#whereExpression << p.OneOrMore(whereCondition)

# define the grammar
selectStmt      << (selectToken + 
                    p.Optional(distinctToken.setResultsName("distinct")) +
                    ('*' | columnNameList).setResultsName("columns") +
                    p.Optional(intoToken + tableNameList.setResultsName("intoTables")) +
                    fromToken + 
                    tableNameList.setResultsName( "tables" ) + 
                    p.Optional(p.Group(p.CaselessLiteral("where") + algebraicExpression), "").setResultsName("where") +
                    p.Optional(orderToken + columnSortList.setResultsName("orderBy")) +
                    p.Optional(limitToken + intNum.setResultsName("limit")))

traceQL = selectStmt

# define Oracle comment format, and ignore them
oracleSqlComment = "--" + p.restOfLine
traceQL.ignore(oracleSqlComment)

class Environment(object):
  def __init__(self, traces = {}, library = None):
    self.traces    = traces
    self.library   = library

class QueryException(Exception):
  pass
  
class Result(object):
  def __init__(self, columns):
    self.columns = columns
    self.rows    = []
    
  def __repr__(self):
    maxWidths = [len(n) for n in self.columns]
    for row in self.rows:
      for i, value in enumerate(row):
        maxWidths[i] = max(maxWidths[i], len(str(value)))
    
    formatStrings = []
    for width in maxWidths:
      formatStrings.append("%%-%ds" % width)

    header = " | ".join([formatStr % n for formatStr, n in zip(formatStrings, self.columns)])
    output = [header, "-" * len(header)]
    
    for row in self.rows:
      row = " | ".join([formatStr % value for formatStr, value in zip(formatStrings, row)])
      output.append(row)
      
    return "\n".join(output)

def _getAvailableColumns(environment):
  columns = set([
    "event.name",
    "event.seq",
    "event.time",
    "event.duration",
    "event.apiId",
  ])
  
  for trace in environment.traces.values():
    for event in trace.events:
      for value in event.values.keys():
        columns.add("event.values.%s" % value)
      for sensor in event.sensorData.keys():
        columns.add("event.sensor.%s" % sensor)

  return [c.split(".") for c in columns]

def _evaluateColumn(environment, events, eventIndices, column):
  # The first part of the column name specifies the object and the rest are attributes
  obj   = column[0]
  attrs = column[1:]
    
  # If the object name is actually a table name, dereference it
  if obj in eventIndices:
    event  = events[eventIndices[obj]]
    obj    = attrs[0]
    attrs  = attrs[1:]
  else:
    event  = events[0]
    
  if obj == "event":
    attrName = attrs[0]
    attrs    = attrs[1:]

    try:
      if attrName == "values":
        return event.values[attrs[0]]
      elif attrName == "sensors":
        return event.sensorData[attrs[0]]
      else:
        return getattr(event, attrName)
    except (KeyError, AttributeError):
      return None
  else:
    raise QueryException("Unknown column name: %s" % ".".join(column))

def _evaluateValue(environment, events, eventIndices, value):
  try:
    return int(value)
  except (TypeError, ValueError):
    if type(value) == str:
      return value
    return _evaluateColumn(environment, events, eventIndices, value)

def _infixToPostfix(expression):
  stack  = []
  result = []
  
  def getTokenPrecedence(token):
    return precedence.get(token, precedence[None])
    
  def flush(result, stack, tokenPrecedence = None):
    while stack and (tokenPrecedence is None or tokenPrecedence < getTokenPrecedence(stack[-1])):
      token = stack.pop()
      if token not in ["(", ")"]:
        result.append(token)
  
  for token in expression:
    tokenPrecedence = getTokenPrecedence(token)
    if not stack or tokenPrecedence > getTokenPrecedence(stack[-1]):
      stack.append(token)
    else:
      flush(result, stack, tokenPrecedence)
      stack.append(token)
  flush(result, stack)
  return result

def _evaluateInfixExpression(environment, events, eventIndices, expression):
  stack = []
  for token in expression:
    if token in binaryOperators:
      b = stack.pop()
      a = stack.pop()
      stack.append(binaryOperators[token](a, b))
    elif token in unaryOperators: # FIXME: unary minus?
      a = stack.pop()
      stack.append(unaryOperators[token](a))
    else:
      stack.append(_evaluateValue(environment, events, eventIndices, token))
  return stack[-1]

def _combinations(seq, base = []):
  position = len(base)
  if position == len(seq):
    yield base
  else:
    for element in seq[position]:
      for combination in _combinations(seq, base + [element]):
        yield combination

class _AntiString(str):
    def __lt__(self, other):
        return str.__gt__(self,other)
    def __gt__(self, other):
        return str.__lt__(self,other)

def query(environment, expression):
  try:
    tokens = traceQL.parseString(expression)
  except p.ParseException, e:
    raise QueryException('%s at "%s...".' % (e.msg, expression[e.loc:e.loc+10]))
    
  if not tokens.columns:
    raise QueryException("Nothing to select.")

  if not tokens.tables:
    raise QueryException("Nowhere to select.")
    
  for table in tokens.tables:
    if not table in environment.traces:
      raise QueryException("Trace does not exist: %s" % table)
    
  if tokens.columns == "*":
    columns = _getAvailableColumns(environment)
  else:
    columns = tokens.columns

  columnNames = []
  for column in columns:
    columnNames.append(".".join(column))

  result      = Result(columnNames)
  task        = Task.startTask("select", "Selecting events")
  
  # Precompile the where-expression
  if tokens.where[0]:
    whereExpression = tokens.where[0][1:]
    whereExpression = _infixToPostfix(whereExpression)
  else:
    whereExpression = None
    
  # Should we make new traces?
  for traceName in tokens.intoTables:
    if traceName in environment.traces:
      raise QueryException("Can't overwrite existing trace '%s'." % traceName)
    trace = Trace.Trace()
    environment.traces[traceName] = trace
  
  events       = [environment.traces[t].events for t in tokens.tables]
  eventIndices = dict([(t, i) for i, t in enumerate(tokens.tables)])
  
  for eventCombination in _combinations(events):
    task.step()
    if whereExpression:
      if not _evaluateInfixExpression(environment, eventCombination, eventIndices, whereExpression):
        continue
        
    # Add the selected event into the new traces
    event = eventCombination[0]
    for traceName in tokens.intoTables:
      if tokens.distinct and event in environment.traces[traceName].events:
        continue
      environment.traces[traceName].events.append(event)
    
    row     = []
    for column in columns:
      value = _evaluateColumn(environment, eventCombination, eventIndices, column)
      row.append(value)
    
    if tokens.distinct and row in result.rows:
      continue
    result.rows.append(row)
  
  # Sort the results
  if tokens.orderBy:
    columnSorters = []
    for i, rule in enumerate(tokens.orderBy):
      columnName, direction = rule
      columnName = ".".join(columnName)
      i          = columnNames.index(columnName)
      func       = lambda v: v
      
      if direction == "desc":
        for row in result.rows:
          if isinstance(row[i], str):
            func = lambda s: _AntiString(s)
            break
          elif isinstance(row[i], list) or row[i] is None:
            func = lambda s: s
            break
        else:
          func = lambda v: v * -1

      columnSorters.append((i, func))

    result.rows.sort(key = lambda row: [func(row[i]) for i, func in columnSorters])

  # Limit the resulting rows
  if tokens.limit:
    result.rows = result.rows[:int(tokens.limit)]

  task.finish()
  return result
