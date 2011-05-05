/**
 *  Tracer platform specific functionality
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
#ifndef TRACER_PLATFORM_H
#define TRACER_PLATFORM_H

#include "tracer.h"
#include "tracer_internal.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Macros */
#define TRI_TOKEN_TO_STRING(token)          #token
#define TRI_STR(value)                      TRI_TOKEN_TO_STRING(value)
#define TRI_AT                              __FILE__ "(" TRI_STR(__LINE__) ")"
#define TR_ASSERT(expr) \
    do \
    { \
        TRint cond = (TRint)(expr); \
        if (!cond) \
            triAssert("Assertion failed at " TRI_AT ": " #expr, TR_FALSE); \
    } while (0)

#define TR_ASSERT_RETURN(expr, value) \
    do \
    { \
        TRint cond = (TRint)(expr); \
        if (!cond) \
            return value; \
    } while (0)
    
#define TR_ASSERT_ERROR(state, expr, err) \
    do \
    { \
        TRint cond = (TRint)(expr); \
        if (!cond) \
        { \
            triSetError((state), (err)); \
            return; \
        } \
    } while (0)
    
#define TR_ASSERT_ERROR_RETURN(state, expr, err, ret) \
    do \
    { \
        TRint cond = (TRint)(expr); \
        if (!cond) \
        { \
            triSetError((state), (err)); \
            return ret; \
        } \
    } while (0)

/** File handle */
typedef     TRhandle  TRFile;

/*
 * File manipulation functions
 */
TRbyte*     triFindConfigFile(TRState* state, const TRbyte* name);
TRFile      triOpenFile(TRState* state, const TRbyte* name, const TRbyte* mode);
void        triCloseFile(TRFile file);
TRuint      triReadFile(TRFile file, TRbyte* data, TRuint length);
TRbyte      triReadFileByte(TRFile file);
TRuint      triWriteFile(TRFile file, const TRbyte* data, TRuint length);
TRbool      triEndOfFile(TRFile file);

/*
 * Tracer defaults
 */
TRbyte*     triGetDefaultStreamDevice(TRState* state);
TRbyte*     triGetDefaultTraceOutputPath(TRState* state);
TRbyte*     triGetDefaultTraceOutputType(TRState* state);

void        triPlatformInit(TRState* state);
void        triPlatformExit(TRState* state);
void        triAssert(const TRbyte* desc, TRbool value);
void*       triMalloc(TRState* state, TRuint bytes);
void        triFree(TRState* state, void* data);
void*       triRealloc(TRState* state, void* data, TRuint bytes);
void        triSetGlobal(void* data);
void        triMemSet(void* data, TRuint value, TRuint bytes);
void        triMemCopy(void* dest, const void* src, TRuint bytes);
TRint       triMemCompare(const void* data1, const void* data2, TRuint bytes);
void*       triGetGlobal(void);
void        triLogMessage(TRState* state, const TRbyte* format, ...);
TRhandle    triOpenLibrary(TRState* state, const TRbyte* name);
void        *triLookupSymbol(TRhandle handle, const TRbyte* name, TRint ordinal);
void        triCloseLibrary(TRhandle handle);
void        triGetProcessName(TRbyte* name, TRuint length);
TRtimestamp triGetSystemTime(TRState* state);
TRuint      triGetNextSequenceNumber(TRState* state);
void        triSleep(TRtimestamp period);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // TRACER_PLATFORM_H
