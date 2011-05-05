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

#include "native_symbian.h"

static CCoeEnv* env = 0;
static RWindow* window = 0;

NativeDisplayType nativeCreateDisplay(void)
{
#if 1
    if (!CCoeEnv::Static()) {
        env = new (ELeave) CCoeEnv();
        TRAPD(err, env->ConstructL());
        User::LeaveIfError(err);
    }
#endif
    /* Return value is used in eglGetDisplay -> must be EGL_DEFAULT_DISPLAY in Symbian */
    return EGL_DEFAULT_DISPLAY;
}

void nativeDestroyDisplay(NativeDisplayType)
{
    if (env)
    {
        env->DestroyEnvironment();
        env = 0;
    }
}

NativeWindowType nativeCreateWindow(const char* title, int width, int height, EGLint visualId)
{
    static RWsSession ws;

    if (ws.Connect() != KErrNone) {
        RDebug::Printf("traceplayer: window server session failed!");
        return NULL;
    }
    static RWindowGroup grp(ws);

    RDebug::Printf("traceplayer: construct window group");
    if (grp.Construct(0xf00f00) != KErrNone) {
        RDebug::Printf("traceplayer: construct window group failed!");
        return NULL;
    }
	
    static RWindow win(ws);

    if (win.Construct(grp, 0xfeefee) != KErrNone) {
        RDebug::Printf("traceplayer: construct RWindow failed!");
        return NULL;
    }
    win.SetExtent(TPoint(0, 0), TSize(width, height));
    win.SetBackgroundColor();
    win.Activate();
    window = &win;

    return (NativeWindowType)(&win);
}

void nativeDestroyWindow(NativeWindowType window)
{
    RWindow* win = (RWindow*)window;
    if (win) {
        win->Close();
        delete win;
        window = 0;
    }
}

NativePixmapType nativeCreatePixmap(int width, int height, EGLint /*visualId*/)
{
    CFbsBitmap* aBmap = new CFbsBitmap();
    aBmap->Create(TSize(width, height), EColor16MU);
    return (NativePixmapType*)aBmap;
}

void nativeDestroyPixmap(NativePixmapType pixmap)
{
    delete (CFbsBitmap*)pixmap;
}

void writeframe(int frame) {
    if (!window)
    {
        return;
    }

    int windowWidth = window->Size().iWidth;
    int windowHeight = window->Size().iHeight;
    int i, temp;
    unsigned int* image = (unsigned int*)User::Alloc(4*windowWidth*windowHeight);
    char fn[512];
    FILE* f;
    unsigned char* RGBpixels = (unsigned char*)User::Alloc(3*windowWidth*windowHeight);

    sprintf(fn, "F:\\trace_%dx%d_%04d.raw", windowWidth, windowHeight, frame);
    f = fopen(fn, "wb");
    if (f == NULL)
        return;

    vgFinish();
    vgReadPixels(&image[windowWidth * (windowHeight - 1)], -windowWidth * 4, VG_sRGBA_8888, 0, 0, windowWidth, windowHeight);
    for (i = 0; i < windowWidth*windowHeight; i++) {
        temp = image[i];
        RGBpixels[3*i] = (temp>>24) & 0xff;
        RGBpixels[3*i+1] = (temp>>16) & 0xff;
        RGBpixels[3*i+2] = (temp>>8) & 0xff;
    }
    fwrite(RGBpixels, 3, windowWidth*windowHeight, f);

    fclose(f);
    User::Free(image);
    User::Free(RGBpixels);
}
