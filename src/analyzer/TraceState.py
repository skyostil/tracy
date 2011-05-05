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

"""Trace state tracking."""

import StringIO
import sys
import Common
import StringUtils
from common.Collections import DefaultDict
from common import Library

class StateGroup(object):
  __slots__ = ["nodes", "values"]
  
  def __init__(self):
    self.nodes  = {}
    self.values = {}
    
class StateNode(object):
  __slots__ = ["groups", "value"]
  
  def __init__(self):
    self.groups  = {}
    self.value   = None

class ImplicitStateValue(object):
  __slots__ = []
  
  def __repr__(self):
    return "<...>"
    
class TraceState(object):
  def __init__(self, project, decorateValues = True):
    assert "code" in project.targets, "No code information in project."
    self.library                = project.targets["code"].library
    self.config                 = project.config
    self.root                   = StateGroup()
    self.collectEffectiveEvents = False
    self.effectiveEvents        = None
    self.stateProducers         = None
    self.stateConsumers         = None
    self.decorateValues         = decorateValues

  def beginCollectingEffectiveEvents(self):
    self.effectiveEvents        = set()
    self.stateProducers         = {}
    self.stateConsumers         = DefaultDict(set)
    self.collectEffectiveEvents = True

  def endCollectingEffectiveEvents(self):
    self.collectEffectiveEvents = False

  def getValue(self, path, default = None):
    root = self.root
    
    for p in path[:-1]:
      if not p in root.nodes:
        if default is None:
          raise KeyError(p)
        return default
          
      node = root.nodes[p]
        
      if not node.value in node.groups:
        if default is None:
          raise KeyError(node.value)
        return default
        
      root = node.groups[node.value]
    
    p = path[-1]
    if p in root.values:
      return root.values[p]
    elif p in root.nodes:
      return root.nodes[p].value
    if default is None:
      raise KeyError(p)
    return default
    
  def getValues(self, path):
    def _getValues(root, prefix, path):
      for i, p in enumerate(path[:-1]):
        if isinstance(p, tuple):
          p, value = p
          try:
            node     = root.nodes[p]
            root     = node.groups[value]
          except KeyError:
            return []
          prefix  += [(p, value)]
        else:
          if p.endswith("*"):
            try:
              p      = p[:-1]
              node   = root.nodes[p]
            except KeyError:
              return []
            values = []
            for value, root in node.groups.items():
              try:
                values += _getValues(root, prefix + [(p, value)], path[i + 1:])
              except KeyError:
                pass
            return values
          else:
            try:
              node    = root.nodes[p]
              root    = node.groups[node.value]
            except KeyError:
              return []
            prefix += [(p, node.value)]
            
      # Read the terminating value
      p = path[-1]
      if p in root.values:
        value = root.values[p]
      elif p in root.nodes:
        value = root.nodes[p].value
      else:
        return []
      return [prefix + [(p, value)]]
    return _getValues(self.root, [], path)
  
  def isTerminalPath(self, path):
    return not isinstance(self._traversePath(path), dict)

  def getNodeAttributes(self, path):
    node = self._traversePath(path)
    if node is not None:
      return node.attrs

  def _traversePath(self, path):
    node = self.config.state
    for p in path:
      if not p in node:
        return None
      node = node[p]
    return node
  
  def getFrozenPath(self, path):
    frozenPath = []
    root = self.root
    
    for p in path[:-1]:
      if not p in root.nodes:
        raise KeyError(p)
          
      frozenPath += [p]
      node = root.nodes[p]
        
      if not node.value in node.groups:
        raise KeyError(node.value)
        
      frozenPath += [node.value]
      root = node.groups[node.value]
    frozenPath += [path[-1]]
    return ".".join([str(p) for p in frozenPath])
    
  def setValue(self, path, value):
    root       = self.root
    isTerminal = self.isTerminalPath(path)
    
    for p in path[:-1]:
      # Create the node if it doesn't exist
      if not p in root.nodes:
        root.nodes[p] = StateNode()
        
      node = root.nodes[p]
      
      # Create the group if it doesn't exist
      if not node.value in node.groups:
        node.groups[node.value] = StateGroup()
        
      root = node.groups[node.value]

    # Don't create the subgroup if this a terminal node
    p = path[-1]
    if isTerminal:
      root.values[p] = value
    else:
      if not p in root.nodes:
        root.nodes[p] = StateNode()
      root.nodes[p].value = value

  def _updateState(self, event, relation, value):
    """
    Writes a new value to the state tree.
    
    @param event:     Originating event instance
    @param relation:  StateRelation instance which specifies the nature of the update
    @param value:     New value to update the state with
    
    @returns True if the state really changed, False otherwise.
    """
    changed    = True
    
    if isinstance(relation, Library.StateRelationCopy):
      path = relation.destPath
      try:
        value = self.getValue(relation.sourcePath)
      except KeyError:
        value = None
    else:
      path = relation.path
      
    isTerminal = self.isTerminalPath(path)
    
    # Write the state value
    if not isinstance(relation, Library.StateRelationGet):
      try:
        if self.getValue(path) == value:
          changed = False
      except KeyError:
        pass
      
      self.setValue(path, value)
      # Sanity check
      assert self.getValue(path) == value
    
    if self.collectEffectiveEvents:
      # The "get" and "modify" relations depend on the full path, while a "set" relation
      # only depends on the path up to the second-to-last element, since the last path 
      # element is set to the new value. A "copy" relation depends on it's full source path.
      if relation.__class__ in (Library.StateRelationGet, Library.StateRelationModify):
        depPath = path
      elif isinstance(relation, Library.StateRelationCopy):
        depPath = relation.sourcePath
      else:
        depPath = path[:-1]
      
      # Consume all parts of the dependent state path
      for i in range(len(depPath)):
        try:
          frozenPath = self.getFrozenPath(depPath[:i + 1])
        except KeyError:
          # State not defined -> ignore
          continue
        if frozenPath in self.stateProducers:
          producingEvent = self.stateProducers[frozenPath]
          # Don't consume state from ourselves
          if producingEvent != event:
            self.stateConsumers[producingEvent].add(event)

      # Mark the production of a new value
      if not isinstance(relation, Library.StateRelationGet):
        frozenPath = self.getFrozenPath(path)
        
        prevEvent = self.stateProducers.get(frozenPath, None)
        self.stateProducers[frozenPath] = event

        # Writing over a previously written terminal makes the earlier write redundant
        if relation.__class__ in (Library.StateRelationSet, Library.StateRelationCopy) and prevEvent:
          #print >>sys.stderr, "*** %s #%d eclipses %s #%d by %s" % (event.name, event.seq, prevEvent.name, prevEvent.seq, frozenPath)
          self._stateProducerValues = self.stateProducers.values()
          self._tryToCullEvent(prevEvent)
    return changed 

  def _tryToCullEvent(self, event):
    # Don't cull events that contribute to the state or have depending events.
    #if event in self.stateProducers.values():
    if event in self._stateProducerValues:
      #print >>sys.stderr, "*** %s #%d contributes to the state and won't be culled." % (event.name, event.seq)
      return

    if self.stateConsumers[event]:
      #print >>sys.stderr, "*** %s #%d has consumers and won't be culled: %s" % (event.name, event.seq, ", ".join(["%s #%d" % (e.name, e.seq) for e in self.stateConsumers[event]]))
      return
  
    #print >>sys.stderr, "*** %s #%d culled." % (event.name, event.seq)
    
    try:
      del self.stateConsumers[event]
      self.effectiveEvents.remove(event)
    except ValueError:
      pass

    # Release all state values consumed by this event and see whether
    # we can cull any more events.      
    #print "cons:", len(self.stateConsumers), "prod:", len(self.stateProducers), "eff:", len(self.effectiveEvents)
    
    for producingEvent, consumerSet in self.stateConsumers.items():
      if event in consumerSet:
        consumerSet.remove(event)
        if not consumerSet:
          self._tryToCullEvent(producingEvent)

  def getEffectiveEvents(self):
    return sorted(self.effectiveEvents, key = lambda e: e.seq)
    
  def isStateAccessingEvent(self, event):
    """
    Determines whether an event explicitly accesses the state or not.
    
    @returns True, if the event accesses the state.
    """
    assert event.name in self.library.functions, "Function not found in library: %s" % event.name
    function = self.library.functions[event.name]
    
    if function.retStateRelation or function.stateRelations:
      return True

    for param in function.parameters.values():
      if param.stateRelation:
        return True
    return False
       
  def processEvent(self, event, implicitStateValue = None):
    assert event.name in self.library.functions, "Function not found in library: %s" % event.name
    
    function       = self.library.functions[event.name]
    stateModifiers = []
    
    def getStatePathForValue(name):
      if name is None:
        relation = function.retStateRelation
      else:
        relation = function.parameters[name].stateRelation
      if relation:
        try:
          return relation.path
        except AttributeError:
          # The copy relation does it this way
          return relation.destPath
    
    # Sort the values by their state paths (shortest first)
    values = event.values.items()
    values.sort(cmp = lambda v1, v2: cmp(getStatePathForValue(v1[0]), getStatePathForValue(v2[0])))
    
    for name, value in values:
      # Get the state path
      if name is None:
        relation = function.retStateRelation
      else:
        relation = function.parameters[name].stateRelation
      
      if self.decorateValues:
        value = StringUtils.decorateValue(self.library, function, name, value)
     
      # Write the state value
      if relation and self._updateState(event, relation, value):
        stateModifiers.append(name)
    
    for relation in function.stateRelations:
      if isinstance(relation, Library.StateRelationModify):
        if implicitStateValue is None:
          # Use a special placeholder value for completely unknown state values
          self._updateState(event, relation, ImplicitStateValue())
        else:
          # When we know the exact value, this relation becomes a "set" relation
          self._updateState(event, Library.StateRelationSet(relation.path), implicitStateValue)
      elif relation.__class__ in (Library.StateRelationSet, Library.StateRelationGet, Library.StateRelationCopy):
        self._updateState(event, relation, None)
      else:
        raise RuntimeError("Unknown state relation: %s" % relation)
    
    if self.collectEffectiveEvents and self.isStateAccessingEvent(event):
      self.effectiveEvents.add(event)
    return stateModifiers

  def __str__(self):
    s = StringIO.StringIO()

    def dumpGroup(root, depth = 0):
      for nodeName, node in sorted(root.nodes.items()):
        print >>s, "  " * depth + "%s:" % nodeName
        
        for groupName, group in sorted(node.groups.items()):
          if groupName == node.value:
            selected = "(*)"
          else:
            selected = ""
          print >>s, "  " * (depth + 1) + "%s:" % groupName, selected
          dumpGroup(group, depth + 2)
      for valueName, value in sorted(root.values.items()):
        if isinstance(value, list) and len(value) > 32:
          value = value[:32] + ["..."]
        print >>s, "  " * depth + "%s = %s" % (valueName, value)
    
    dumpGroup(self.root)
    return s.getvalue()
