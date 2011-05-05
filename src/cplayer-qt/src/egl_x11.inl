/**
 * Copyright (c) 2011 Nokia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef EGL_X11_INL
#define EGL_X11_INL

#include <X11/Xlib.h>

static Display* xdpy = 0;

static EGLNativeDisplayType createEGLNativeDisplayType1(void* context)
{
#if defined(WIN32)
    return EGL_DEFAULT_DISPLAY;
#else
    return (EGLNativeDisplayType)(xdpy = XOpenDisplay(NULL));
#endif
}

static void destroyEGLNativeDisplayType2(void* context, EGLNativeDisplayType dpy)
{
#if defined(WIN32)
    (void)dpy;
#else
    XCloseDisplay((Display*)dpy);
    xdpy = 0;
#endif
}

static EGLNativeDisplayType createNativeDisplayType1(void* context)
{
    return createEGLNativeDisplayType1(context);
}

static void destroyNativeDisplayType2(void* context, NativeDisplayType display)
{
    destroyEGLNativeDisplayType2(context, display);
}

EGLNativePixmapType createEGLNativePixmapType4(void* context, int width, int height, int mode)
{
    Window rootWindow = DefaultRootWindow(xdpy);
    Pixmap pixmap;
    int depth = 16;

    pixmap = XCreatePixmap(xdpy, rootWindow, width, height, depth);

    if (!pixmap)
    {
        fprintf(stderr, "XCreatePixmap failed\n");
        return 0;
    }

    XFlush(xdpy);

    return (EGLNativePixmapType)pixmap;
}

void destroyEGLNativePixmapType2(void* context, EGLNativePixmapType pixmap)
{
    XFreePixmap(xdpy, (Pixmap)pixmap);
}

#endif /* EGL_X11_INL */
