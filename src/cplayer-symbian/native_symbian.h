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
#ifndef NATIVE_SYMBIAN_H
#define NATIVE_SYMBIAN_H

#include <e32std.h>
#include <e32base.h>
#include <e32debug.h>
#include <w32std.h>
#include <stdio.h>
#include <stdlib.h>
#include <eikenv.h>
#include <EGL/egl.h>
#include <VG/openvg.h>

typedef NativeDisplayType NativeDisplay;
typedef NativeWindowType  NativeWindow;
typedef NativePixmapType  NativePixmap;

extern "C" {
    NativeDisplay nativeCreateDisplay  ( void );
    void          nativeDestroyDisplay ( NativeDisplay display );
    NativeWindow  nativeCreateWindow   ( const char *title, int width, int height, EGLint visualId );
    void          nativeDestroyWindow  ( NativeWindow window );
    NativePixmap  nativeCreatePixmap   ( int width, int height, EGLint visualId );
    void          nativeDestroyPixmap  ( NativePixmap pixmap );
}

#endif /* NATIVE_SYMBIAN_H */
