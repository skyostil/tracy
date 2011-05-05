/*
 *  Stub EGL implementation for the instrumentation API.
 *  Copyright (c) 2011 Nokia
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

/* Type definitions */
typedef TRint   EGLint;
typedef TRint   EGLDisplay;
typedef TRbool  EGLBoolean;
typedef void*   NativeDisplayType;

/* Constants */
#define EGL_DEFAULT_DISPLAY            ((NativeDisplayType)0)
#define EGL_DEFAULT_DISPLAY_HANDLE     ((EGLDisplay)1)
#define EGL_FALSE                      0
#define EGL_TRUE                       1
#define EGL_SUCCESS                    0x3000
#define EGL_NOT_INITIALIZED            0x3001
#define EGL_NONE                       0x3038
#define EGL_EXTENSIONS                 0x3055

/* eglGetProcAddress needs to be supplied by the instrumented engine */
#ifdef __cplusplus
extern void (* eglGetProcAddress (const char *procname))(...);
#else
extern void (* eglGetProcAddress (const char *procname))();
#endif

EGLint eglGetError(void)
{
  return EGL_SUCCESS;
}

EGLDisplay eglGetDisplay(NativeDisplayType display)
{
    TR_UNUSED(display);
    return EGL_DEFAULT_DISPLAY_HANDLE;
}

EGLBoolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    TR_UNUSED(dpy);
    TR_UNUSED(major);
    TR_UNUSED(minor);
    return EGL_TRUE;
}

EGLBoolean eglTerminate(EGLDisplay dpy)
{
    TR_UNUSED(dpy);
    return EGL_TRUE;
}

const char* eglQueryString(EGLDisplay dpy, EGLint name)
{
    TR_UNUSED(dpy);
    if (name == EGL_EXTENSIONS)
    {
        return "EGL_TCY_instrumentation_api";
    }
    return 0;
}

