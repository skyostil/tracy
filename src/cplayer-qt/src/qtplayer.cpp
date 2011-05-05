/**
 * Qt C trace player
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
#include <stdlib.h>

#include <sys/time.h>

#include <QApplication>
#include <QMainWindow>

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
    bool done;
    int frame;
    int startTime;
} PlayerState;

static int windowWidth = 0, windowHeight = 0;

#include "egl_qt.inl"
#include "egl_x11.inl"
#include "egl.inl"
#include "hacks.inl"

/* Include the C trace */
#include TRACE

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

static void playTrace(PlayerState* state)
{
    /* Start timing after the first actually rendered frame */
    if (!state->startTime /*&& state->frame == 2*/)
    {
        state->startTime = getTimeStamp();
    }

    if (!state->done)
    {
        state->done = (playFrame(state, state->frame) == 0);

        printf("Frame %d\n", state->frame);
        ASSERT_EGL;
        ASSERT_GL;

        if (state->frame > 0)
        {
#if defined(TAKE_SCREENSHOTS)
            takeScreenshot(state->frame, 0);
#endif
        }
        state->frame++;

        if (state->done)
        {
            int diff = getTimeStamp() - state->startTime;
            if (diff)
            {
                float fps = (1000000.0f * state->frame) / diff;
                printf("Average FPS: %.02f\n", fps);
            }
        }
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    PlayerState state;

    /* Init the state */
    state.done      = false;
    state.frame     = 0;
    state.startTime = 0;

    /* Schedule the player */
#if defined(DONT_CLOSE_WINDOW)
    while (1)
#else
    while (!state.done)
#endif
    {
        app.processEvents();
        playTrace(&state);
    }

    return 0;
}
