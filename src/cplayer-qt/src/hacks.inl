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
#ifndef HACKS_INL
#define HACKS_INL

#if !defined(NDEBUG)
#define ASSERT_EGL do { \
    EGLint err = eglGetError(); \
    if (err != EGL_SUCCESS) \
        printf("EGL error at %s:%d: %04x\n", __FILE__, __LINE__, err); \
    } while (0)
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
#define ASSERT_GL do { \
    GLint err = glGetError(); \
    if (err != 0) \
        printf("GL error at %s:%d: %04x\n", __FILE__, __LINE__, err); \
    } while (0)
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
    return 0;
}

GLrenderbuffer createGLframebuffer1(void* context)
{
    return 0;
}

GLattribute createGLattribute1(void* context)
{
    return 0;
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

#endif /* HACKS_INL */
