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
A parser for converting configuration files to configuration objects.
"""

from pyparsing import *
import OrderedDict
import Common
from common import Log
from common import Config

ParserElement.enablePackrat()

def _collapse(s, l, t):
  return t[0]

def _createGroup(s, l, t):
  d = {}
  for key, value in t:
    if key in d:
      raise ValueError("Duplicate group item: %s" % key)
    d[key] = value
  return d

def _createList(s, l, t):
  l = []
  for item in t[0]:
    l.append(item)
  return [l]

def _createGroupItem(s, l, t):
  try:
    name, attrs, value = t[0]
  except ValueError:
    # An empty list is eaten by pyparsing
    name, attrs, value = t[0][0], t[0][1], []
  if type(value) == dict:
    g = Config.Group(attrs)
    g.update(value)
    return name, g
  elif type(value) == list:
    l = Config.List(attrs)
    l += value
    return name, l
  else:
    return name, Config.Item(value, attrs)

def _createListItem(s, l, t):
  value, attrs = t[0]
  if type(value) == dict:
    g = Config.Group(attrs)
    g.update(value)
    return g
  elif type(value) == list:
    l = Config.List(attrs)
    l += value
    return [l]
  else:
    return Config.Item(value, attrs)

# Configuration file grammar
configIdentifier  = Word(alphas + "_@.", alphanums + "_@.")
configInteger     = Word(nums)
configName        = configIdentifier | QuotedString('"', escChar = "\\")
configValue       = Forward()
configAttribute   = Group(configName + Suppress("=") + configValue)
configAttributes  = Suppress("(") + \
                    delimitedList(configAttribute).setParseAction(lambda s, l, t: dict(list(t))) + \
                    Suppress(")")
configInitializer = Suppress(":") + configValue
configGroup       = Forward()
configList        = Forward()
configGroupItem   = Group(configName.setResultsName("name") + \
                          Optional(configAttributes,  default = {}) + \
                          Optional(configInitializer, default = "")).setParseAction(_createGroupItem)
configListItem    = Group(configValue + Optional(configAttributes,  default = {})).setParseAction(_createListItem)
configGroup      << Suppress("{") + \
                        ZeroOrMore(configGroupItem).setParseAction(_createGroup) + \
                    Suppress("}")
configList       << Suppress("[") + \
                        Group(ZeroOrMore(configListItem)).setParseAction(_createList) + \
                    Suppress("]")
configValue      << (configName | configGroup | configList | configInteger)
configFile        = ZeroOrMore(configGroupItem)
configFile.ignore(cStyleComment)
configFile.ignore(cppStyleComment)

def parseConfig(config, template = None):
  """
  Parses the given tracer configuration and returns a tree of configuration
  symbols.
      
  @param config     Config file text
  @param template   Optional configuration to use a template.
  
  @return: A dictionary tree of configuration values
  """
  if not template:
    template = Config.Config()
  items = template

  for itemName, item in configFile.parseString(config):
    if not item:
      Log.warn("Empty top-level item '%s'." % itemName)
    if itemName in items and isinstance(item, Config.Group):
      items[itemName].update(item)
    elif itemName in items and isinstance(item, Config.List):
      items[itemName] += item
    else:
      items[itemName] = item

  return items
