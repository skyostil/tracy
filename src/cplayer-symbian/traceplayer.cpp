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
#include <e32std.h>
#include <e32debug.h>
#include <w32std.h>
#include <coemain.h>

#include <EGL/egl.h>
#include <VG/openvg.h>
#include "native_symbian.h"

//
// functions needed for Tracy playback 
//
EGLNativeWindowType createEGLNativeWindowType6(void* context, int x, int y, int width, int height, int mode)
{
    return nativeCreateWindow(NULL, width, height, 0);
}
EGLNativeDisplayType createEGLNativeDisplayType1(void* context)
{
    return nativeCreateDisplay();
}

void destroyEGLNativeWindowType2(void* context, EGLNativeWindowType window)
{
    nativeDestroyWindow((NativeWindowType)window);
}
void destroyEGLNativeDisplayType2(void* context, EGLNativeDisplayType display)
{
    nativeDestroyDisplay(display);
}

EGLConfig createEGLConfig28(void* context, EGLint config_id, EGLint buffer_size, EGLint red_size, EGLint green_size, EGLint blue_size, EGLint alpha_size, EGLint bind_to_texture_rgb, EGLint bind_to_texture_rgba, EGLint config_caveat, EGLint depth_size, EGLint level, EGLint max_swap_interval, EGLint min_swap_interval, EGLint native_renderable, EGLint sample_buffers, EGLint samples, EGLint stencil_size, EGLint surface_type, EGLint transparent_type, EGLint transparent_red, EGLint transparent_green, EGLint transparent_blue, EGLint luminance_size, EGLint alpha_mask_Size, EGLint color_buffer_type, EGLint conformant, EGLint renderable_type)
{

    EGLConfig config;
    EGLint    numConfig;
    EGLint cfg_attribs[] = { EGL_RED_SIZE,     8,
                             EGL_GREEN_SIZE,   8,
                             EGL_BLUE_SIZE,    8,
                             EGL_ALPHA_SIZE,   8,
                             EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                             EGL_NONE };
    
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, NULL, NULL);
    eglBindAPI(EGL_OPENVG_API);

    eglChooseConfig(display, cfg_attribs, &config, 1, &numConfig);

    return config;
}
void destroyEGLConfig2(void* context, EGLConfig config)
{
    /* TODO */
}

/* These are needed due to a bug in Qt OpenVG backend */
VGPaint createVGPaint1(void* context)
{
    return 0;
}

void destroyVGPaint2(void* context, VGPaint p)
{
    (void)p;
}

VGPaint createVGImage1(void* context)
{
    return 0;
}

void destroyVGImage2(void* context, VGPaint p)
{
    (void)p;
}

VGPath createVGPath1(void* context)
{
    return 0;
}

void destroyVGPath2(void* context, VGPath p)
{
    (void)p;
}

VGFont createVGFont1(void* context)
{
    return 0;
}

void destroyVGFont2(void* context, VGFont p)
{
    (void)p;
}

// Trace file to play
#include "wiggly.inl"

LOCAL_C void DoStartL()
{
    int i = 0;

    while (1)
    {
        if (!playFrame(0, i++))
        {
            break;
        }
    }
}

GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TRAPD(mainError, DoStartL()); // Run application code inside TRAP harness
    
    delete cleanup;
    __UHEAP_MARKEND;
     
    return mainError;
}
