#
# Tracer C API makefile
#

# Tools
CC=gcc
RM=rm
GDB=gdb
VERBOSE?=0
ifeq ($(VERBOSE),1)
CFLAGS=-g -Wall -DTRACER_VERBOSE_LOGGING -DTRACER_VERBOSE_CONFIG_LOGGING
else
CFLAGS=-g -Wall
endif

ifeq ($(OS),Windows_NT)
LIBS=
else
LIBS=-ldl -lpthread
endif

# Objects
OBJECTS=tracer.o tracer_ascii_codec.o tracer_stdio_stream.o \
        tracer_platform_unix.o tracer_util.o tracer_internal.o \
        tracer_binary_codec.o tracer_logicanalyzer_codec.o \
	tracer_etm_stream.o
TEST_OBJECTS=CuTest.o TestAll.o tracer_api_test.o tracer_util_test.o \
             tracer_player_test.o testlibrary.o

all: test

testlibrary.so: $(TEST_OBJECTS)
	$(CC) -g -shared -Wl -o testlibrary.so testlibrary.o

testsuite: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LIBS) -o testsuite

test: testsuite testlibrary.so
	@./testsuite

leaktest: testsuite
	@valgrind --leak-check=full --show-reachable=yes ./testsuite

debug: testsuite
	@$(GDB) ./testsuite

clean:
	@$(RM) -f *.o testsuite testlibrary.so


.PHONY: clean test leaktest

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
