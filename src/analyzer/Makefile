#
# Trace Analyzer makefile
#

# Tools
PYTHON=python
RM=rm

test:
	@cd ../generator && $(MAKE) gles11-egl11-symbian && cd ../analyzer
	@$(PYTHON) TestAll.py

clean:
	@$(RM) -f *.pyc


.PHONY: clean test
