#
# GTK C player Makefile
#

# Path to the OpenVG engine
VG_DIR?=../libraries/ri_package_1.1/ri
VG_INCLUDES=$(VG_DIR)/include
VG_LIBDIR=$(VG_DIR)
VG_LIBS=-lOpenVG

# Path to the OpenGL ES 1.1 engine
GLES_INCLUDES?=.
GLES_LIBDIR?=.
GLES_LIBS=-lGLES_CM

# Path to the OpenGL ES 2.0 engine
GLES2_INCLUDES?=.
GLES2_LIBDIR?=.
GLES2_LIBS=-lGLESv2 -lEGL

CC=gcc
LD=gcc
TRACE?=wiggly.inl
CFLAGS+=$(shell pkg-config gtk+-2.0 gdk-2.0 --cflags) -DTRACE_SOURCE_FILE=\"$(TRACE)\" -O0 -g
#LDFLAGS=$(shell pkg-config gtk+-2.0 gdk-2.0 --libs) -lGLES_CM
LDFLAGS+=$(shell pkg-config gtk+-2.0 gdk-2.0 --libs)

all: gtkplayer

gtkplayer: gtkplayer.c *.inl
	gcc gtkplayer.c -o $@ $(CFLAGS) -I$(VG_INCLUDES) $(LDFLAGS) -L$(VG_LIBDIR) $(VG_LIBS)

gtkplayer-gles2: gtkplayer.c *.inl
	gcc gtkplayer.c -o $@ $(CFLAGS) -I$(GLES2_INCLUDES) $(LDFLAGS) -DUSE_GLES2 -L$(GLES2_LIBDIR) $(GLES2_LIBS)

gtkplayer-gles: gtkplayer.c *.inl
	gcc gtkplayer.c -o $@ $(CFLAGS) -I$(GLES_INCLUDES) $(LDFLAGS) -DUSE_GLES -L$(GLES_LIBDIR) $(GLES_LIBS)

run: gtkplayer
	LD_LIBRARY_PATH=$(VG_DIR) ./gtkplayer

clean:
	rm -f gtkplayer
