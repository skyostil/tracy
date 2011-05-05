#
# Tracer makefile
#

test:
	$(MAKE) -C common test
	$(MAKE) -C generator test
	$(MAKE) -C analyzer test
	$(MAKE) -C tracer test

clean:
	$(MAKE) -C common clean
	$(MAKE) -C generator clean
	$(MAKE) -C analyzer clean
	$(MAKE) -C tracer clean


.PHONY: clean test
