#
# Tracer makefile
#

test:
	$(MAKE) -C src test

clean:
	$(MAKE) -C src clean

.PHONY: clean test
