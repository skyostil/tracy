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

import Common
import TracyGenerator
import Options
import Target
import os
import shutil
from common import Project
from common import Log

class TracyTest(unittest.TestCase):
  def setUp(self):
    self.options, args = Options.parseCommandLine(["-o", "tracytest.tcy"])
    self.project = Project.Project(self.options, self.options.project)
    Log.quiet = True
    
  def tearDown(self):
    os.unlink("tracytest.tcy")
    shutil.rmtree("tracytest")
  
  def testGlesProjectCreation(self):
    # Create the project
    args = ["config/gles/v1.1/gles-egl.conf"] + Target.targets.keys()
    TracyGenerator.create(self.project, self.options, args)
    self.generateTargets()

  def testVgProjectCreation(self):
    args = ["config/vg/v1.1/vg.conf"] + Target.targets.keys()
    TracyGenerator.create(self.project, self.options, args)
    self.generateTargets()

  def generateTargets(self):
    # Load the created project and generate each target
    options, args = Options.parseCommandLine(["-c", "tracytest.tcy", "-o", "tracytest"])
    proj = Project.Project(options, "tracytest.tcy")
    for target in Target.targets:
      assert target in proj.targets
      try:
        TracyGenerator.generate(proj, options, [target])
        assert os.path.exists("tracytest")
      except RuntimeError:
        if target not in ["code"]:
          raise

if __name__ == "__main__":
  unittest.main()
