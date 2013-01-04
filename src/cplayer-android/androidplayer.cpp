/**
 * Android C trace player
 * Copyright (c) 2013 Google
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
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <time.h>

#include <android_native_app_glue.h>

#include "util.inl"

// Include the C trace
#include TRACE

class AppContext
{
public:
    AppContext(android_app* app):
        app(app),
        frameCount(0),
        dpy(EGL_NO_DISPLAY)
    {
    }

    android_app* app;
    int frameCount;
    struct timespec startTime;

    EGLConfig config;
    EGLNativeWindowType win;
    EGLDisplay dpy;
    EGLContext context;
    EGLSurface surface;
};

static bool render(AppContext* appContext)
{
    if (appContext->dpy == EGL_NO_DISPLAY)
        return true;

    if (appContext->frameCount == 3)
        appContext->frameCount++;

    LOGI("Playing frame %d", appContext->frameCount);

    bool result = playFrame(appContext, appContext->frameCount++);
    eglSwapBuffers(appContext->dpy, appContext->surface);

    return result;
}

static bool initializeEgl(AppContext* appContext)
{
    const EGLint configAttrs[] =
    {
        EGL_BUFFER_SIZE, 32,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    const EGLint contextAttrs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLint configCount = 0;

    LOGI("Initializing EGL");
    appContext->dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    ASSERT_EGL();

    eglInitialize(appContext->dpy, NULL, NULL);
    eglChooseConfig(appContext->dpy, configAttrs, &appContext->config, 1, &configCount);
    ASSERT_EGL();

    if (!configCount)
    {
        LOGW("Config not found");
        goto out_error;
    }

    EGLint format;
    eglGetConfigAttrib(appContext->dpy, appContext->config, EGL_NATIVE_VISUAL_ID, &format);
    ASSERT_EGL();

    ANativeWindow_setBuffersGeometry(appContext->app->window, 0, 0, format);

    appContext->context = eglCreateContext(appContext->dpy, appContext->config, EGL_NO_CONTEXT, contextAttrs);
    ASSERT_EGL();
    if (!appContext->context)
    {
        LOGW("Unable to create a context");
        goto out_error;
    }

    appContext->surface = eglCreateWindowSurface(appContext->dpy, appContext->config, appContext->app->window, NULL);
    ASSERT_EGL();
    if (!appContext->surface)
    {
        LOGW("Unable to create a surface");
        goto out_error;
    }

    eglMakeCurrent(appContext->dpy, appContext->surface, appContext->surface, appContext->context);
    ASSERT_EGL();

    return true;

out_error:
    eglMakeCurrent(appContext->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(appContext->dpy, appContext->surface);
    eglDestroyContext(appContext->dpy, appContext->context);
    eglTerminate(appContext->dpy);
    return false;
}

void terminateEgl(AppContext* appContext)
{
    eglMakeCurrent(appContext->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(appContext->dpy, appContext->surface);
    eglDestroyContext(appContext->dpy, appContext->context);
    eglTerminate(appContext->dpy);
}

static void handleCommand(struct android_app* app, int32_t cmd)
{
    struct AppContext* appContext = (struct AppContext*)app->userData;

    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        if (appContext->app->window != NULL)
        {
            initializeEgl(appContext);
            render(appContext);
        }
        break;
    case APP_CMD_TERM_WINDOW:
        terminateEgl(appContext);
        break;
    }
}

extern "C" void android_main(struct android_app* state)
{
    struct AppContext appContext(state);

    // Make sure glue isn't stripped.
    app_dummy();

    state->userData = &appContext;
    state->onAppCmd = &handleCommand;

    appContext.app = state;

    while (true)
    {
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
        {
            if (source)
            {
                source->process(state, source);
            }
            if (state->destroyRequested != 0)
            {
                break;
            }
        }
        if (!render(&appContext))
        {
            break;
        }
    }

    LOGI("Playback done, exiting");
    exit(0);
}
