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
#ifndef EGL_QT_INL
#define EGL_QT_INL

#include <QMainWindow>

static QMainWindow* window = NULL;

static EGLNativeWindowType modifyEGLNativeWindowType7(void* context,
        EGLNativeWindowType win, int x, int y, int width, int height, int mode)
{
    assert(window);

#if defined(FORCE_LANDSCAPE)
    if (width < height)
    {
        int tmp = width;
        width = height;
        height = tmp;
    }
#endif

#if defined(FORCE_WINDOW_W)
    windowWidth = FORCE_WINDOW_W;
#else
    windowWidth = width;
#endif

#if defined(FORCE_WINDOW_H)
    windowHeight = FORCE_WINDOW_H;
#else
    windowHeight = height;
#endif

    window->setFixedSize(QSize(windowWidth, windowHeight));
    return win;
}

static EGLNativeWindowType createEGLNativeWindowType6(void* context,
        int x, int y, int width, int height, int mode)
{
#if defined(FORCE_LANDSCAPE)
    if (width < height)
    {
        int tmp = width;
        width = height;
        height = tmp;
    }
#endif

#if defined(FORCE_WINDOW_W)
    windowWidth = FORCE_WINDOW_W;
#else
    windowWidth = width;
#endif

#if defined(FORCE_WINDOW_H)
    windowHeight = FORCE_WINDOW_H;
#else
    windowHeight = height;
#endif
    if (!window)
    {
        window = new QMainWindow();
        window->setWindowTitle("C Trace Player");
        window->setFixedSize(QSize(windowWidth, windowHeight));
        window->show();
    }
    assert(window);

    return (EGLNativeWindowType)window->effectiveWinId();
}

static void destroyEGLNativeWindowType2(void* context, EGLNativeWindowType win)
{
#if defined(DONT_CLOSE_WINDOW)
    return;
#endif

    if (window && EGLNativeWindowType(window->effectiveWinId()) == win)
    {
        delete window;
        window = 0;
    }
}

#endif /* EGL_QT_INL */
