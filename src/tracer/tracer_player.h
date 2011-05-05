/*
 *  Trace player framework functionality.
 *  Copyright (c) 2011 Nokia
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#ifndef TRACER_PLAYER_H
#define TRACER_PLAYER_H

#include "tracer.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 *  A handle to the platform-specific player instance.
 */
typedef TRhandle TRPlayer;

/**
 *  A native window handle.
 */
typedef TRhandle TRNativeWindow;

/**
 *  A native pixmap handle.
 */
typedef TRhandle TRNativePixmap;

/** Supported color formats */
typedef enum {
  TR_COLOR_RGBX_8888,
  TR_COLOR_RGBA_8888,
  TR_COLOR_RGB_888,
  TR_COLOR_RGB_565,
  TR_COLOR_RGBA_5551,
  TR_COLOR_RGBA_4444,
  TR_COLOR_L_8,
  TR_COLOR_BW_1
} TRColorFormat;

/**
 *  Returns a reference to the currently active trace player object.
 */
TRPlayer    trGetPlayer(void);

/**
 *  Create a native window suitable for e.g. passing to EGL.
 *
 *  @param player       Player instance
 *  @param x            Window X coordinate
 *  @param y            Window Y coordinate
 *  @param width        Window width
 *  @param height       Window height
 *  @param colorFormat  Window color format
 *
 *  @returns a window handle or NULL on error.
 */
TRNativeWindow trPlayerCreateWindow(TRPlayer player, TRint x, TRint y, TRint width, TRint height, TRColorFormat colorFormat);

/**
 *  Destroy a native window.
 *
 *  @param player       Player instance
 *  @param window       Window handle
 */
void           trPlayerDestroyWindow(TRPlayer player, TRNativeWindow window);

/**
 *  Refresh the contents of a window.
 *
 *  @param player       Player instance
 *  @param window       Window handle
 */
void           trPlayerRefreshWindow(TRPlayer player, TRNativeWindow window);

/**
 *  Create a native pixmap suitable for e.g. passing to EGL.
 *
 *  @param player       Player instance
 *  @param width        Pixmap width
 *  @param height       Pixmap height
 *  @param colorFormat  Pixmap color format
 *
 *  @returns a pixmap handle or NULL on error.
 */
TRNativePixmap trPlayerCreatePixmap(TRPlayer player, TRint width, TRint height, TRColorFormat colorFormat);

/**
 *  Destroy a native pixmap.
 *
 *  @param player       Player instance
 *  @param pixmap       Pixmap handle
 */
void           trPlayerDestroyPixmap(TRPlayer player, TRNativePixmap pixmap);

/**
 *  Displays a pixmap on the screen.
 *
 *  @param player       Player instance
 *  @param pixmap       Pixmap handle
 */
void           trPlayerDisplayPixmap(TRPlayer player, TRNativePixmap pixmap);

/**
 *  Play back a recorded trace file. Note that the trace file must have been
 *  previously opened with trOpenInputStream.
 *
 *  @param player             Player instance
 *
 *  @retval TR_END_OF_FILE    The end of the event stream was encountered.
 *  @retval TR_NOT_FOUND      An unknown event was read from the stream.
 *  @retval TR_NO_ERROR       The frame marker was encountered and playback was suspended.
 */
TRenum         trPlay(TRPlayer player);

/**
 *  Play back a single event from a recorded trace file.
 *
 *  @param player             Player instance
 *
 *  @returns the event that was played or NULL on error.
 */
TREvent*       trPlaySingleEvent(TRPlayer player);


#if defined(__cplusplus)
} // extern "C"
#endif

#endif // TRACER_PLAYER_H
