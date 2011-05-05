/**
 *  Win32 C trace player
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

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <VG/openvg.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

#define TRACE_SOURCE_FILE   "wiggly.inl"
//#define USE_GLUT

#if defined(USE_GLUT)
#include <GL/glut.h>
#endif

/** Player state */
typedef struct
{
    int frame;
	int hasWindow;
} PlayerState;

static HINSTANCE instance;
static int       done = 0;
static PlayerState state;

static EGLNativeWindowType createEGLNativeWindowType6(void* context, int x, int y, int width, int height, int mode)
{
#if defined(USE_GLUT)
	int win;
	state.hasWindow = 1;
	glutInitWindowPosition(x, y);
	glutInitWindowSize(width, height);
	win = glutCreateWindow("Trace Player");
	return (NativeWindowType)win;
#else
    HWND hWnd;
    
    hWnd = CreateWindow("Trace Player", "Trace Player", 
                        WS_OVERLAPPED | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
                        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                        NULL, NULL, instance, NULL);

    if (hWnd)
    {
        MoveWindow(hWnd, x, y, width, height, FALSE);
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
    return (EGLNativeWindowType)hWnd;
#endif
}

static void destroyEGLNativeWindowType2(void* context, EGLNativeWindowType window)
{
#if !defined(USE_GLUT)
    DestroyWindow((HWND)window);
#endif
}

static EGLConfig createEGLConfig28(void* context, EGLint config_id, EGLint buffer_size, EGLint red_size, EGLint green_size, EGLint blue_size, EGLint alpha_size, EGLint bind_to_texture_rgb, EGLint bind_to_texture_rgba, EGLint config_caveat, EGLint depth_size, EGLint level, EGLint max_swap_interval, EGLint min_swap_interval, EGLint native_renderable, EGLint sample_buffers, EGLint samples, EGLint stencil_size, EGLint surface_type, EGLint transparent_type, EGLint transparent_red, EGLint transparent_green, EGLint transparent_blue, EGLint luminance_size, EGLint alpha_mask_Size, EGLint color_buffer_type, EGLint conformant, EGLint renderable_type)
{
    EGLint attrs[64], n = 0, numConfigs = 0;
    EGLDisplay dpy = eglGetDisplay(1);
    EGLConfig config;
    
    /* Gerbera assumes 0 for the default display */
    if (!dpy)
    {
	    	dpy = eglGetDisplay(0);
    }
    
    eglInitialize(dpy, NULL, NULL);
    
    surface_type = EGL_WINDOW_BIT;
    attrs[n++] = EGL_SURFACE_TYPE;
    attrs[n++] = surface_type;
    attrs[n++] = EGL_RED_SIZE;
    attrs[n++] = red_size;
    attrs[n++] = EGL_GREEN_SIZE;
    attrs[n++] = green_size;
    attrs[n++] = EGL_BLUE_SIZE;
    attrs[n++] = blue_size;
    attrs[n++] = EGL_ALPHA_SIZE;
    attrs[n++] = alpha_size;
    attrs[n++] = EGL_DEPTH_SIZE;
    attrs[n++] = depth_size;
    attrs[n++] = EGL_STENCIL_SIZE;
    attrs[n++] = stencil_size;
    attrs[n++] = EGL_NONE;
    
    eglChooseConfig(dpy, attrs, &config, 1, &numConfigs);
    
    assert(numConfigs > 0);
    
    return config;
}

static void destroyEGLConfig2(void* context, EGLConfig config)
{
    /* TODO */
}

static EGLNativeDisplayType createEGLNativeDisplayType1(void* context)
{
    return EGL_DEFAULT_DISPLAY;
}

static void destroyEGLNativeDisplayType2(void* context, EGLNativeDisplayType display)
{
}

/* These are needed due to a bug in Qt OpenVG backend */
static VGPaint createVGPaint1(void* context)
{
    return 0;
}

static void destroyVGPaint2(void* context, VGPaint p)
{
    (void)p;
}

static VGPaint createVGImage1(void* context)
{
    return 0;
}

static void destroyVGImage2(void* context, VGPaint p)
{
    (void)p;
}

static VGPath createVGPath1(void* context)
{
    return 0;
}

static void destroyVGPath2(void* context, VGPath p)
{
    (void)p;
}

static VGFont createVGFont1(void* context)
{
    return 0;
}

static void destroyVGFont2(void* context, VGFont p)
{
    (void)p;
}

#ifdef USE_GLUT
static void idle(void)
{
	if (!done)
	{
		static char title[256];
		sprintf(title, "Trace Player [frame %d]", state.frame);
		glutSetWindowTitle(title);
	}
	glutPostRedisplay();
}

static void display(void)
{
	playFrame(&state, state.frame++);
}
#endif

/* Include the C trace */
#include TRACE_SOURCE_FILE

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(USE_GLUT)
	glutInit(&__argc, __argv);
	state.frame = 0;
	state.hasWindow = 0;

	while (!state.hasWindow)
	{
		display();
	}
	glutDisplayFunc(&display);
	glutIdleFunc(&idle);
	glutMainLoop();
#else
    MSG msg;
    WNDCLASS wndClass;

    /* Register the window class */
    wndClass.style          = 0;
    wndClass.lpfnWndProc    = (WNDPROC)DefWindowProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = NULL;
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = 0;
    wndClass.lpszMenuName   = "Trace Player";
    wndClass.lpszClassName  = "Trace Player";
	instance = hInstance;

	state.frame = 0;
  
    if (!RegisterClass(&wndClass))
    {
        return FALSE;
    }

    /* Message loop */
    while (!done)
    {
        /*
         *  Switch off the busy loop once the trace is played to the end.
         */
        if (done || PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
        {
            if (GetMessage(&msg, NULL, 0, 0) )
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                done = 1;
            }
        }
		if (playFrame(&state, state.frame++) == 0)
		{
			done = 1;
		}
        //assert(vgGetError() == VG_NO_ERROR);
        //assert(eglGetError() == EGL_SUCCESS);
    }

    return 0;
#endif
}
