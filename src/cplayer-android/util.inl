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
#ifndef UTIL_INL
#define UTIL_INL

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "cplayer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "cplayer", __VA_ARGS__))

#if !defined(NDEBUG)
#define ASSERT_EGL() do { \
    EGLint err = eglGetError(); \
    if (err != EGL_SUCCESS) \
        printf("EGL error at %s:%d: %04x\n", __FILE__, __LINE__, err); \
    } while (0)
#else
#define ASSERT_EGL()
#endif

#if defined(USE_GLES) || defined(USE_GLES2)
#if !defined(NDEBUG)
#define ASSERT_GL() do { \
    GLint err = glGetError(); \
    if (err != 0) \
        LOGI("GL error at %s:%d: %04x\n", __FILE__, __LINE__, err); \
    } while (0)
#else
#define ASSERT_GL()
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

GLuniform createGLuniform1(void* context)
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

void destroyGLuniform2(void*context, GLuniform u)
{
}
#endif /* USE_GLES2 */

#endif /* UTIL_INL */
