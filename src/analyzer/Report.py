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

"""Reporting tools."""

import os
import shutil
import cgi

class Report(object):
  def __init__(self, title):
    self.title    = title
    self.items    = []
  
  def add(self, item):
    if item:
      self.items.append(item)

  def create(self, cls, *args, **kwargs):
    item = cls(*args, **kwargs)
    self.add(item)
    return item
    
class Element(object):
  def __init__(self, elementClass = None):
    self.elementClass = elementClass

class Section(Element):
  def __init__(self, title, *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.title    = title
    self.items    = []

  def add(self, item):
    if item:
      self.items.append(item)

  def create(self, cls, *args, **kwargs):
    item = cls(*args, **kwargs)
    self.add(item)
    return item

class Paragraph(Element):
  def __init__(self, text, *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.text     = text

class Image(Element):
  def __init__(self, path, *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.path     = path

class Link(Element):
  def __init__(self, target, content, *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.target   = target
    self.content  = content

class LinkTarget(Element):
  def __init__(self, name, *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.name     = name

class Table(Element):
  def __init__(self, header = [], *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.header = header
    self.rows   = []
    
  def addColumn(self, text):
    self.header.append(text)
    self.rows = [r + [""] for r in self.rows]

  def addRow(self, *row):
    self.rows.append(row)

class UnorderedList(Element):
  def __init__(self, *args, **kwargs):
    Element.__init__(self, *args, **kwargs)
    self.items = []
    
  def addItem(self, item):
    self.items.append(item)

class Compiler(object):
  """Report compiler base"""
  def __init__(self, report):
    self.report = report
    
  def compile(self, outputPath):
    """
    Render report to a concrete format.
    
    @param outputPath: Output directory
    """
    pass

def makePath(path):
  # Try creating a directory
  try:
    os.makedirs(path)
  except OSError, err:
    if err.errno != 17: raise

class HtmlCompiler(Compiler):
  """HTML report generator"""
  
  def __init__(self, report, charset = "iso-8859-1"):
    Compiler.__init__(self, report)
    self.charset     = charset
    self.styles      = {}
    self.stylesheets = []
  
  def setStyle(self, selector, *style):
    self.styles[selector] = "\n".join(style)
    
  def addStylesheet(self, url):
    self.stylesheets.append(url)
  
  def compile(self, outputPath):
    makePath(outputPath)
    
    output = open(os.path.join(outputPath, "index.html"), "w")
    
    # Generation state
    self._depth          = 0
    self._sectionCounter = [0]
    self._outputPath     = os.path.abspath(outputPath)

    # Print the HTML header
    print >>output, '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'
    print >>output, '<html lang="en">'
    print >>output, '<head>'
    print >>output, '  <meta content="text/html; charset=%s" http-equiv="content-type"/>' % self.charset
    print >>output, '  <title>%s</title>' % self.report.title
    print >>output, '  <meta http-equiv="Content-Type" content="text/html; charset=%s"/>' % self.charset
    
    for stylesheet in self.stylesheets:
      print >>output, '  <link rel="Stylesheet" title="Normal" media="screen" type="text/css" href="%s"/>' % stylesheet
    
    print >>output, '  <style type="text/css">'
    for selector, style in self.styles.items():
      print >>output, '%s {' % selector
      print >>output, style
      print >>output, '}'
    print >>output, '  </style>'
    print >>output, '</head>'
    
    # Begin the content
    print >>output, '<body>'
    
    # Print the title
    print >>output, "<h1>",
    self._generateNode(output, self.report.title)
    print >>output, "</h1>"
    
    # Generate the table of contents
    def sectionHeaderFunc(name, counter, section):
      print >>output, '<li>'
      print >>output, '<a href="#%s">%s %s</a>' % (name, ".".join([str(s) for s in counter]), section.title)
      print >>output, '<ul>'
    
    def sectionFooterFunc(name, counter, section):
      print >>output, '</ul>'
      print >>output, '</li>'
    
    print >>output, '<div class="toc">'
    print >>output, "<h2>Table of Contents</h2>"
    print >>output, "<ul>"
    self._createTableOfContents(sectionHeaderFunc, sectionFooterFunc)
    print >>output, "</ul>"
    print >>output, '</div>'
    
    # Generate the contents
    for item in self.report.items:
      self._generateNode(output, item)
    
    print >>output, '</body>'
    print >>output, '</html>'
    output.close()
    
  def _normalizePath(self, path):
    return path.replace("\\", "/")
    
  def _createTableOfContents(self, sectionHeaderFunc, sectionFooterFunc):
    def collectSections(node, counter):
      name = "section-" + ".".join([str(s) for s in counter])
      if counter:
        sectionHeaderFunc(name, counter, node)
      i = 0
      for item in node.items:
        if isinstance(item, Section):
          collectSections(item, counter + [i + 1])
          i += 1
      if counter:
        sectionFooterFunc(name, counter, node)
    
    collectSections(self.report, counter = [])

  def _generateNode(self, output, node):
    dispatchTable = {
        Section:       self._generateSection,
        Paragraph:     self._generateParagraph,
        Image:         self._generateImage,
        Table:         self._generateTable,
        Link:          self._generateLink,
        LinkTarget:    self._generateLinkTarget,
        UnorderedList: self._generateUnorderedList,
    }
    self._depth += 1
    self._sectionCounter.append(0)
    try:
      dispatchTable[node.__class__](output, node)
    except KeyError:
      print >>output, cgi.escape(str(node)),
    self._sectionCounter.pop()
    self._depth -= 1

  def _generateSection(self, output, section):
    self._sectionCounter[-2] += 1
    d = min(6, self._depth + 1)
    print >>output, '<div class="%s">' % section.elementClass
    print >>output, '<a name="section-%s"></a>' % (".".join([str(s) for s in self._sectionCounter[:-1]])), 
    print >>output, "<h%d>%s" % (d, ".".join([str(s) for s in self._sectionCounter[:-1]])), 
    self._generateNode(output, section.title)
    print >>output, "</h%d>" % d
    
    for item in section.items:
      self._generateNode(output, item)
    print >>output, '</div'

  def _generateParagraph(self, output, paragraph):
    print >>output, '<p class="%s">' % paragraph.elementClass
    self._generateNode(output, paragraph.text)
    print >>output, "</p>"

  def _generateImage(self, output, image):
    path = os.path.abspath(image.path)
    if not path.startswith(self._outputPath):
      fn = os.path.basename(path)
      shutil.copyfile(path, os.path.join(self._outputPath, fn))
    else:
      fn = path[len(self._outputPath) + 1:]
    print >>output, '<img class="%s" src="%s"/>' % (image.elementClass, self._normalizePath(fn))

  def _generateTable(self, output, table):
    # Don't print out empty tables
    if not table.rows:
      return
      
    print >>output, '<table class="%s">' % table.elementClass
    print >>output, "<tr>"
    for header in table.header:
      print >>output, "<th>"
      self._generateNode(output, header)
      print >>output, "</th>"
    print >>output, "</tr>"

    for i, row in enumerate(table.rows):
      print >>output, '<tr class="%s">' % ((i % 2) and "odd" or "even")
      for item in row:
        print >>output, "<td>"
        self._generateNode(output, item)
        print >>output, "</td>"
    
    print >>output, "</table>"

  def _generateUnorderedList(self, output, list):
    print >>output, '<ul class="%s">' % list.elementClass
    for item in list.items:
      print >>output, "<li>"
      self._generateNode(output, item)
      print >>output, "</li>"
    print >>output, "</ul>"

  def _generateLink(self, output, link):
    print >>output, '<a class="%s" href="%s">' % (link.elementClass, link.target)
    self._generateNode(output, link.content)
    print >>output, '</a>'

  def _generateLinkTarget(self, output, linkTarget):
    print >>output, '<a class="%s" name="%s"></a>' % (linkTarget.elementClass, linkTarget.name)

def createCompiler(format, report):
  compilers = {
    "html": HtmlCompiler
  }
  return compilers[format](report)
