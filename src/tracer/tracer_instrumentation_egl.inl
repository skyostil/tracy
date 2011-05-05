/**
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
#include "tracer_instrumentation.h"
#include "tracer_internal.h"

/*
 *  Include the appropriate EGL implementation
 */
#if defined(TRACER_USE_GLES_EGL)
#  include <gles/egl.h>
#elif defined(TRACER_USE_SHARED_EGL)
#  include <egl/egl.h>
#elif defined(TRACER_USE_STUB_EGL)
#  include "tracer_egl_stub.inl"
#else
#  error EGL instrumentation was enabled, but an EGL implementation has not been selected.
#endif

/*
 * Instrumentation EGL extension functions 
 */

#define MAX_COUNTERS            1024

/* Counter types */
#define EGL_INTEGER_TCY         0x6001
#define EGL_FLOAT_TCY           0x6002

/* Surface types */
#define EGL_COLOR_BUFFER_TCY    0x6101
#define EGL_DEPTH_BUFFER_TCY    0x6102
#define EGL_STENCIL_BUFFER_TCY  0x6103

/* Performance counter metadata */
typedef EGLint        (*EglGetPerfCounterTypeFunc)(EGLint counter);
typedef const char*   (*EglGetPerfCounterNameFunc)(EGLint counter);
typedef const char*   (*EglGetPerfCounterDescriptionFunc)(EGLint counter);

/* Performance counter queries */
typedef EGLint        (*EglGetPerfCounterIntegerValueFunc)(EGLint counter);
typedef float         (*EglGetPerfCounterFloatValueFunc)(EGLint counter);
typedef void          (*EglResetPerfCountersFunc)(void);
  
/* Surface data readback */
typedef EGLint        (*EglLockSurfaceFunc)(EGLint type, void** pixels, 
                                            EGLint* width, EGLint* height, EGLint* stride,
                                            EGLint* componentMasks, EGLint* dataType,
                                            EGLint* isLinear, EGLint* isPremultiplied);
typedef void          (*EglUnlockSurfaceFunc)(EGLint surface);

/** Instrumentation API state */
typedef struct
{
    TRState*      state;
    TRArray*      counters;
    
    /* EGL functions */
    EglGetPerfCounterTypeFunc         eglGetPerfCounterType;
    EglGetPerfCounterNameFunc         eglGetPerfCounterName;
    EglGetPerfCounterDescriptionFunc  eglGetPerfCounterDescription;
    EglGetPerfCounterIntegerValueFunc eglGetPerfCounterIntegerValue;
    EglGetPerfCounterFloatValueFunc   eglGetPerfCounterFloatValue;
    EglResetPerfCountersFunc          eglResetPerfCounters;
    EglLockSurfaceFunc                eglLockSurface;
    EglUnlockSurfaceFunc              eglUnlockSurface;
    
    /* Surface */
    TRInstrumentationSurface          surface;
    EGLint                            colorSurfaceHandle;
    EGLint                            depthSurfaceHandle;
    EGLint                            stencilSurfaceHandle;
} TRInstrumentationState;

#define LOAD_EXT(NAME) s->egl##NAME = (Egl##NAME##Func)eglGetProcAddress("egl" #NAME "TCY"); TR_ASSERT(s->egl##NAME)

TRInstrumentationHandle trInitializeInstrumentation()
{
    TRState* state = triGetState();
    TRInstrumentationState* s;
    {
        TRbool terminate = TR_FALSE;
        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        const TRbyte* extensions;
        TRint i;

        /* Bring up EGL if needed */
#if defined(EGL_VERSION_1_2)
        /* On EGL 1.2, eglGetError() returns success even if EGL hasn't been initialized for this thread */
        if (1)
#else        
        if (eglGetError() == EGL_NOT_INITIALIZED)
#endif
        {
            eglInitialize(display, 0, 0);
            terminate = TR_TRUE;
        }
        
        /* Check that the required instrumentation extension is there */
        extensions = eglQueryString(display, EGL_EXTENSIONS);
        if (!extensions || triStringIndex(extensions, "EGL_TCY_instrumentation_api") == -1)
        {
            triLogMessage(state, "The EGL_TCY_instrumentation_api extension was not found. The instrumentation statistics will not be available.");
            if (extensions)
            {
                triLogMessage(state, "Supported extensions: %s", extensions);
            }
            triSetError(state, TR_INVALID_OPERATION);
            return 0;
        }
        
        /* Initialize our state */
        s = (TRInstrumentationState*)triMalloc(state, sizeof(TRInstrumentationState));
        TR_ASSERT_ERROR_RETURN(state, s, TR_OUT_OF_MEMORY, 0);
        
        s->state    = state;
        s->counters = triCreateArray(state);
        
        /* Load all the extension functions */
        LOAD_EXT(GetPerfCounterType);
        LOAD_EXT(GetPerfCounterName);
        LOAD_EXT(GetPerfCounterDescription);
        LOAD_EXT(GetPerfCounterIntegerValue);
        LOAD_EXT(GetPerfCounterFloatValue);
        LOAD_EXT(GetPerfCounterType);
        LOAD_EXT(ResetPerfCounters);
        LOAD_EXT(LockSurface);
        LOAD_EXT(UnlockSurface);
        
        /* Load the performance counter data */
        for (i = 0; i < MAX_COUNTERS; i++)
        {
            if (s->eglGetPerfCounterType(i) != EGL_NONE)
            {
                TRPerformanceCounter* c = triMalloc(state, sizeof(TRPerformanceCounter));
                if (c)
                {
                    switch (s->eglGetPerfCounterType(i))
                    {
                    case EGL_INTEGER_TCY:
                        c->type = TR_INTEGER;
                        break;
                    case EGL_FLOAT_TCY:
                        c->type = TR_FLOAT;
                        break;
                    default:
                        c->type = TR_VOID;
                        break;
                    }
                    c->name        = s->eglGetPerfCounterName(i);
                    c->description = s->eglGetPerfCounterDescription(i);
                    triArrayAppend(s->counters, c);
                }
            }
            else
            {
                break;
            }
        }
        
        if (terminate)
        {
            eglTerminate(display);
        }
        
        return (TRInstrumentationHandle)s;
    }
}

const TRArray* trUpdatePerformanceCounters(TRInstrumentationHandle handle)
{
    TRint i = 0;
    TRInstrumentationState* s = (TRInstrumentationState*)handle;
    TR_ASSERT(s);
    {
        TR_FOR_EACH(TRPerformanceCounter*, counter, s->counters)
        {
            switch (counter->type)
            {
            case TR_INTEGER:
                counter->value.i = s->eglGetPerfCounterIntegerValue(i);
                break;
            case TR_FLOAT:
                counter->value.f = s->eglGetPerfCounterFloatValue(i);
                break;
            }
            i++;
        }
        s->eglResetPerfCounters();
        return s->counters;
    }
}

void trDestroyInstrumentation(TRInstrumentationHandle handle)
{
    TRInstrumentationState* s = (TRInstrumentationState*)handle;
    if (s)
    {
        TR_FOR_EACH(TRPerformanceCounter*, counter, s->counters)
        {
            triFree(s->state, counter);
        }
        triDestroyArray(s->counters);
        triFree(s->state, s);
    }
}

const TRInstrumentationSurface* trLockInstrumentationSurface(TRInstrumentationHandle handle)
{
    TRInstrumentationState* s = (TRInstrumentationState*)handle;
    
    if (s)
    {
        triMemSet(&s->surface, 0, sizeof(TRInstrumentationSurface));
        s->colorSurfaceHandle   = s->eglLockSurface(EGL_COLOR_BUFFER_TCY, &s->surface.colorPixels,
                                                    &s->surface.width, &s->surface.height, &s->surface.colorStride,
                                                    &s->surface.colorMasks[0], &s->surface.colorDataType,
                                                    &s->surface.isLinear, &s->surface.isPremultiplied);
        s->depthSurfaceHandle   = s->eglLockSurface(EGL_DEPTH_BUFFER_TCY, &s->surface.depthPixels,
                                                    0, 0, &s->surface.depthStride,
                                                    &s->surface.depthMask, &s->surface.depthDataType,
                                                    0, 0);
        s->stencilSurfaceHandle = s->eglLockSurface(EGL_STENCIL_BUFFER_TCY, &s->surface.stencilPixels,
                                                    0, 0, &s->surface.stencilStride,
                                                    &s->surface.stencilMask, &s->surface.stencilDataType,
                                                    0, 0);
        return &s->surface;
    }
    return 0;
}

void trUnlockInstrumentationSurface(TRInstrumentationHandle handle)
{
    TRInstrumentationState* s = (TRInstrumentationState*)handle;
    if (s)
    {
        s->eglUnlockSurface(s->stencilSurfaceHandle);
        s->eglUnlockSurface(s->depthSurfaceHandle);
        s->eglUnlockSurface(s->colorSurfaceHandle);
    }
}
