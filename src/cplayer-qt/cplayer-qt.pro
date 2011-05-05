### Trace file to use
DEFINES += "TRACE=\\\"trace.inl\\\""

### API to use
#DEFINES += USE_GLES
DEFINES += USE_GLES2
#DEFINES += USE_VG

### Options
DEFINES += DONT_CLOSE_WINDOW
#DEFINES += TAKE_SCREENSHOTS
#DEFINES += TAKE_MORE_SCREENSHOTS
#DEFINES += FORCE_LANDSCAPE
#DEFINES += "FORCE_CONFIG=1"
#DEFINES += "FORCE_WINDOW_W=1"
#DEFINES += "FORCE_WINDOW_H=1"

CONFIG += debug

INCLUDEPATH += src

SOURCES += \
    src/qtplayer.cpp

HEADERS += \
    src/egl.inl \
    src/egl_qt.inl \
    src/egl_x11.inl \
    src/hacks.inl

LIBS += -lEGL

QT += opengl
