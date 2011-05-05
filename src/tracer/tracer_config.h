/**
 *  Tracer configuration
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
#ifndef TRACER_CONFIG_H
#define TRACER_CONFIG_H

#include "tracer_compiler.h"

/* General logging */
#define TRACER_VERBOSE_LOGGING

/* State tracker logging */
/*#define TRACER_VERBOSE_STATE_LOGGING*/

/* Pointer tracker logging */
/*#define TRACER_VERBOSE_POINTER_LOGGING*/

/* Runtime configuration logging */
/*#define TRACER_VERBOSE_CONFIG_LOGGING*/

/* Player logging */
#define TRACER_PLAYER_VERBOSE_LOGGING

/* Codec support */
#define TRACER_USE_ASCII_CODEC
#define TRACER_USE_BINARY_CODEC
#define TRACER_USE_LOGICANALYZER_CODEC

/* Device support */
#define TRACER_USE_STDIO_DEVICE

#if defined(TR_PLATFORM_UNIX)
#    define TRACER_USE_ETM_DEVICE
#endif

#if defined(TR_PLATFORM_SYMBIAN)
#    define TRACER_USE_SYMBIAN_DEVICE
#    define TRACER_USE_STI_DEVICE
#endif

/*
 * Trace player features 
 */

/* Should the EGL instrumentation API be used */
/*#define TRACER_USE_EGL_INSTRUMENTATION*/

/* Use the EGL implementation offered by GLES */
/*#define TRACER_USE_GLES_EGL*/

/* Use an internal stub EGL that implements everything but eglGetProcAddress */
/*#define TRACER_USE_STUB_EGL*/

#endif /* TRACER_CONFIG_H */
