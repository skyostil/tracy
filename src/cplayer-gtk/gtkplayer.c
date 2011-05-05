/****************************************************************************
 * GTK C trace player
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
 *
 * Configuration options 
 ****************************************************************************/

/* Use OpenGL ES */
/*#define USE_GLES*/

/* Use OpenGL ES 2.0 */
/*#define USE_GLES2*/

/* Use OpenVG */
/*#define USE_VG*/

/* Trace file name */
#if !defined(TRACE_SOURCE_FILE)
#    define TRACE_SOURCE_FILE   "wiggly.inl"
#endif

/* Take screenshots */
//#define TAKE_SCREENSHOTS
//#define TAKE_MORE_SCREENSHOTS

/* Force a landscape resolution */
//#define FORCE_LANDSCAPE

/* Force the use of particular EGL config */
//#define FORCE_CONFIG    9

/* Force window dimensions */
//#define FORCE_WINDOW_W    864
//#define FORCE_WINDOW_H    480

/****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#if defined(USE_GLES)
#    include <GLES/gl.h>
#    include <GLES/egl.h>
#endif

#if defined(USE_GLES2)
#    include <GLES2/gl2.h>
#    include <EGL/egl.h>
#endif

#if defined(USE_VG)
#    include <EGL/egl.h>
#    include <VG/openvg.h>
#endif

void takeScreenshot(int frame, int line);

/** Player state */
typedef struct
{
    int frame;
    int startTime;
} PlayerState;

static int windowWidth = 0, windowHeight = 0;
static GtkWindow* window = NULL;

static EGLNativeWindowType modifyEGLNativeWindowType7(void* context, EGLNativeWindowType win, int x, int y, int width, int height, int mode)
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

    gtk_window_resize(window, windowWidth, windowHeight);
    return win;
}

static EGLNativeWindowType createEGLNativeWindowType6(void* context, int x, int y, int width, int height, int mode)
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

    if (window)
    {
        return (NativeWindowType)GDK_WINDOW_XWINDOW(GTK_WIDGET(window)->window);
    }

    window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    
    assert (window);
    {
        gtk_window_set_title(window, "C Trace Player");
        gtk_window_set_default_size(window, windowWidth, windowHeight);
        gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
        gtk_widget_set_double_buffered(GTK_WIDGET(window), FALSE);

        gtk_widget_show_all(GTK_WIDGET(window));

        /*
         * This is a hack to ensure the window is really created before 
         * we return it to the trace player
         */
        while (gtk_events_pending())
        {
            gtk_main_iteration();
        }

        /* Make sure the window was realized */
        assert(GTK_WIDGET(window)->window);
        
        /* Make sure we have the native X window for the widget */
        assert(GDK_WINDOW_XWINDOW(GTK_WIDGET(window)->window));
        
        return (NativeWindowType)GDK_WINDOW_XWINDOW(GTK_WIDGET(window)->window);
    }
}

static void destroyEGLNativeWindowType2(void* context, EGLNativeWindowType window)
{
    /* TODO */
}

static EGLNativeDisplayType createEGLNativeDisplayType1(void* context)
{
#if defined(WIN32)
    return EGL_DEFAULT_DISPLAY;
#else
    return (EGLNativeDisplayType)XOpenDisplay(NULL);
#endif
}

static void destroyEGLNativeDisplayType2(void* context, EGLNativeDisplayType dpy)
{
#if defined(WIN32)
    (void)dpy;
#else
    XCloseDisplay((Display*)dpy);
#endif
}

#if !defined(NDEBUG)
#define ASSERT_EGL do { EGLint err = eglGetError(); if (err != EGL_SUCCESS) printf("EGL error at %s:%d: %04x\n", __FILE__, __LINE__, err); } while (0)
#else
#define ASSERT_EGL
#endif

#if defined(USE_VG)
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

#if defined(TAKE_MORE_SCREENSHOTS)
#  define vgDrawPath(A, B) \
    do { \
        vgDrawPath(A, B); \
        PlayerState* s = (PlayerState*)context; \
        takeScreenshot(s->frame, __LINE__); \
    } while (0)
#  define vgDrawImage(A) \
    do { \
        vgDrawImage(A); \
        PlayerState* s = (PlayerState*)context; \
        takeScreenshot(s->frame, __LINE__); \
    } while (0)
#endif /* TAKE_MORE_SCREENSHOTS */

#endif /* USE_VG */

#if defined(USE_GLES) || defined(USE_GLES2)
#if defined(TAKE_MORE_SCREENSHOTS)
#  define glDrawArrays(A, B, C) \
    do { \
        glDrawArrays(A, B, C); \
        PlayerState* s = (PlayerState*)context; \
        takeScreenshot(s->frame, __LINE__); \
    } while (0)
#  define glDrawElements(A, B, C, D) \
    do { \
        glDrawArrays(A, B, C, D); \
        PlayerState* s = (PlayerState*)context; \
        takeScreenshot(s->frame, __LINE__); \
    } while (0)
#endif /* TAKE_MORE_SCREENSHOTS */

#if !defined(NDEBUG)
#define ASSERT_GL do { GLint err = glGetError(); if (err != 0) printf("GL error at %s:%d: %04x\n", __FILE__, __LINE__, err); } while (0)
#else
#define ASSERT_GL
#endif
#endif /* USE_GLES || USE_GLES2 */

#if defined(USE_GLES2)
typedef GLuint GLshader;
typedef GLuint GLprogram;
typedef GLuint GLattribute;
typedef GLuint GLuniform;
typedef GLuint GLframebuffer;
typedef GLuint GLrenderbuffer;
typedef void* GLshadertype;
typedef void* GLprecisiontype;

/* TODO: End this insanity */
GLrenderbuffer createGLrenderbuffer1(void* context)
{
}

GLrenderbuffer createGLframebuffer1(void* context)
{
}

GLattribute createGLattribute1(void* context)
{
}

void destroyGLframebuffer2(void* context, GLframebuffer buffer)
{
}

void destroyGLrenderbuffer2(void* context, GLrenderbuffer buffer)
{
}

void destroyGLattribute2(void*context, GLattribute a)
{
}
#endif /* USE_GLES2 */

static EGLNativeDisplayType createNativeDisplayType1(void* context)
{
    return (EGLNativeDisplayType)XOpenDisplay(NULL);
}

static void destroyNativeDisplayType2(void* context, NativeDisplayType display)
{
    XFree((Display*)display);
}

static EGLConfig createEGLConfig28(void* context, EGLint config_id, EGLint buffer_size, EGLint red_size, EGLint green_size, EGLint blue_size, EGLint alpha_size, EGLint bind_to_texture_rgb, EGLint bind_to_texture_rgba, EGLint config_caveat, EGLint depth_size, EGLint level, EGLint max_swap_interval, EGLint min_swap_interval, EGLint native_renderable, EGLint sample_buffers, EGLint samples, EGLint stencil_size, EGLint surface_type, EGLint transparent_type, EGLint transparent_red, EGLint transparent_green, EGLint transparent_blue, EGLint luminance_size, EGLint alpha_mask_Size, EGLint color_buffer_type, EGLint conformant, EGLint renderable_type)
{
    EGLint attrs[64], n = 0, numConfigs = 0;
    EGLDisplay dpy = eglGetDisplay((EGLNativeDisplayType)GDK_DISPLAY());
    EGLConfig config;
    EGLint major, minor;
    int brokenConfig = 0;
    
#if defined(FORCE_CONFIG)
    printf("Forcing EGL config %d\n", FORCE_CONFIG);
    return FORCE_CONFIG;
#endif
    //surface_type = EGL_WINDOW_BIT;
    //renderable_type = EGL_OPENGL_ES2_BIT;
    printf("Surface type: 0x%x, renderable type: 0x%x\n", surface_type, renderable_type);

    /* Ignore config attributes that are definitely incorrect
     * TODO: see why this is happening
     */
    if (renderable_type > 0x3000)
    {
        renderable_type = EGL_OPENGL_ES2_BIT;
        surface_type = EGL_PIXMAP_BIT;
        brokenConfig = 1;
    }

    eglInitialize(dpy, &major, &minor);
    
    attrs[n++] = EGL_SURFACE_TYPE;
    attrs[n++] = surface_type;
#if defined(USE_GLES) || defined(USE_GLES2)
    attrs[n++] = EGL_RENDERABLE_TYPE;
    attrs[n++] = renderable_type;
#endif

    if (!brokenConfig)
    {
        /*attrs[n++] = EGL_RED_SIZE;
        attrs[n++] = red_size;
        attrs[n++] = EGL_GREEN_SIZE;
        attrs[n++] = green_size;
        attrs[n++] = EGL_BLUE_SIZE;
        attrs[n++] = blue_size;
        attrs[n++] = EGL_ALPHA_SIZE;
        attrs[n++] = alpha_size;*/
        attrs[n++] = EGL_DEPTH_SIZE;
        attrs[n++] = depth_size;
        attrs[n++] = EGL_STENCIL_SIZE;
        attrs[n++] = stencil_size;
    }
    attrs[n++] = EGL_NONE;
    
    eglChooseConfig(dpy, attrs, &config, 1, &numConfigs);

    assert(numConfigs > 0);

    printf("Using EGL config: %d\n", config);
    
    return config;
}

static void destroyEGLConfig2(void* context, EGLConfig config)
{
    /* TODO */
}

EGLNativePixmapType createEGLNativePixmapType4(void* context, int width, int height, int mode)
{
    Window rootWindow = DefaultRootWindow(GDK_DISPLAY());
    Pixmap pixmap;
    int depth = 16;

    pixmap = XCreatePixmap(GDK_DISPLAY(), rootWindow, width, height, depth);

    if (!pixmap)
    {
        fprintf(stderr, "XCreatePixmap failed\n");
        return 0;
    }

    XFlush(GDK_DISPLAY());

    return (EGLNativePixmapType)pixmap;
}

void destroyEGLNativePixmapType2(void* context, EGLNativePixmapType pixmap)
{
    /* TODO */
}

/* Include the C trace */
#include TRACE_SOURCE_FILE

int getTimeStamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (int)(1000000 * tv.tv_sec + tv.tv_usec);
}

void takeScreenshot(int frame, int line)
{
    if (!windowWidth || !windowHeight)
    {
        return;
    }

#if defined(TAKE_SCREENSHOTS) && defined(USE_VG)
    char* image = (char*)malloc(windowWidth * windowHeight * 4);
    char fn[512];
    FILE* f;

    assert(image);
    sprintf(fn, "trace_%dx%d_%04d_%d.raw", windowWidth, windowHeight, frame, line);
    f = fopen(fn, "wb");

    assert(f);
    vgReadPixels(image + windowWidth * (windowHeight - 1) * 4, -windowWidth * 4, VG_sABGR_8888, 0, 0, windowWidth, windowHeight);
    fwrite(image, 1, windowWidth * windowHeight * 4, f);

    fclose(f);
    free(image);
#elif defined(TAKE_SCREENSHOTS) && (defined(USE_GLES2) || defined(USE_GLES))
    char* image = (char*)malloc(windowWidth * windowHeight * 4);
    char fn[512];
    FILE* f;

    assert(image);
    sprintf(fn, "trace_%dx%d_%04d_%d.raw", windowWidth, windowHeight, frame, line);
    f = fopen(fn, "wb");

    assert(f);
    glReadPixels(0, 0, windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, image);
    fwrite(image, 1, windowWidth * windowHeight * 4, f);

    fclose(f);
    free(image);
#endif
}

static gboolean idleEvent(gpointer data)
{
    PlayerState* state = (PlayerState*)data;
    int done = 0;
    
    /* Start timing after the first actually rendered frame */
    if (!state->startTime /*&& state->frame == 2*/)
    {
        state->startTime = getTimeStamp();
    }

    done = (playFrame(state, state->frame) == 0);

    printf("Frame %d\n", state->frame);
    ASSERT_EGL;
    ASSERT_GL;

    if (state->frame > 0)
    {
        takeScreenshot(state->frame, 0);
    }
    state->frame++;

#if defined(TAKE_SCREENSHOTS)
    if (done)
#else
    if (done)
#endif
    {
        int  diff = getTimeStamp() - state->startTime;
        if (diff)
        {
            float fps = (1000000.0f * state->frame) / diff;
            printf("Average FPS: %.02f\n", fps);
        }

        gtk_main_quit();
    }
    return 1;
}

extern int initLibrary();

int main(int argc, char** argv)
{
    PlayerState state;

    /* Bring up GTK */
    gtk_init(&argc, &argv);

    /* Init the state */
    state.frame     = 0;
    state.startTime = 0;

    /* Schedule the player */
    g_idle_add(idleEvent, &state);

    /* Main loop */
    gtk_main();

    return 0;
}
