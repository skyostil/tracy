/**
 *  Tracer platform implementation for Unix
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

#include <pthread.h>
#include "tracer.h"
#include "tracer_platform.h"
#include "tracer_internal.h"
#include "tracer_util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>

/* Global data pointer */
void* globalData = 0;

/* Prefix for configuration files */
#ifndef PREFIX
#   define PREFIX ""
#endif /* PREFIX */

/* Platform data */
typedef struct
{
    pthread_mutex_t    lock;
    TRuint             seqCounter;
    int                verbose;
} UnixPlatformData;

void triPlatformInit(TRState* state)
{
    UnixPlatformData* d = triMalloc(state, sizeof(UnixPlatformData));
    pthread_mutex_init(&d->lock, 0);
    d->seqCounter = 0;
    d->verbose = getenv("TRACY_VERBOSE") ? atoi(getenv("TRACY_VERBOSE")) : 0;
    state->synchronousOperation = getenv("TRACY_SYNC") ? atoi(getenv("TRACY_SYNC")) : 0;
    state->platformData = d;

    /* Schedule a proper termination when the host application shuts down */
    atexit(trTerminate);
}

void triPlatformExit(TRState* state)
{
    UnixPlatformData* d = (UnixPlatformData*)state->platformData;
    pthread_mutex_destroy(&d->lock);
    triFree(state, state->platformData);
}

void triSetGlobal(void* data)
{
    globalData = data;
}

void* triGetGlobal(void)
{
    return globalData;
}

void triAssert(const TRbyte* desc, TRbool value)
{
    if (!value)
    {
        fprintf(stderr, "Assertion failed: %s\n", desc);
        abort();
    }
}

void* triMalloc(TRState* state, TRuint bytes)
{
    return malloc(bytes);
}

void triFree(TRState* state, void* data)
{
    if (data)
    {    
        free(data);
    }
}

void* triRealloc(TRState* state, void* data, TRuint bytes)
{
    return realloc(data, bytes);
}

void triLogMessage(TRState* state, const TRbyte* format, ...)
{
    if (state)
    {
        UnixPlatformData* d = (UnixPlatformData*)state->platformData;
        if (d && !d->verbose)
        {
            return;
        }
    }

    if (isatty(2))
    {
        fprintf(stderr, "\033[1;32m(tracy)\033[0m ");
    }
    else
    {
        fprintf(stderr, "(tracy) ");
    }
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}

TRhandle triOpenLibrary(TRState* state, const TRbyte* name)
{
    return (TRhandle)dlopen(name, RTLD_LAZY);
}

void *triLookupSymbol(TRhandle handle, const TRbyte* name, TRint ordinal)
{
    return dlsym((void*)handle, name);
}

void triCloseLibrary(TRhandle handle)
{
    if (handle)
        dlclose((void*)handle);
}

TRtimestamp triGetSystemTime(TRState* state)
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return (TRtimestamp)(1000000 * tv.tv_sec + tv.tv_usec);
}

void triGetProcessName(TRbyte* name, TRuint length)
{
#if defined(linux) || defined(__CYGWIN32__)
    /* In Linux we use the proc file system */
    int bytes;
    char *s;
    TR_ASSERT((bytes = readlink("/proc/self/exe", name, length)) > 0);

    /* Append a terminating NULL */
    name[bytes] = 0;
    
    /* Separate the binary name */
    s = name + bytes;

    while (s > name && *s != '/')
    {
        s--;
    }
    if (s != name)
    {
        int i;
        for (i = 0; i < length; i++)
        {
            name[i] = s[i + 1];
            if (!s[i + 1]) break;
        }
    }
#else
    /* Just use the process id everywhere else */
    snprintf(name, length, "%d", getpid());
#endif
}

TRuint triGetNextSequenceNumber(TRState* state)
{
    UnixPlatformData* d = (UnixPlatformData*)state->platformData;
    TRuint c;
    pthread_mutex_lock(&d->lock);
    c = d->seqCounter++;
    pthread_mutex_unlock(&d->lock);
    return c;
}

void triMemSet(void* data, TRuint value, TRuint bytes)
{
    memset(data, value, bytes);
}

void triMemCopy(void* dest, const void* src, TRuint bytes)
{
    memcpy(dest, src, bytes);
}

TRint triMemCompare(const void* data1, const void* data2, TRuint bytes)
{
    return memcmp(data1, data2, bytes);
}

TRFile triOpenFile(TRState* state, const TRbyte* name, const TRbyte* mode)
{
    TR_UNUSED(state);
    return fopen(name, mode);
}

void triCloseFile(TRFile file)
{
    if (file)
    {
        FILE* f = (FILE*)file;
        fclose(f);
    }
}

TRuint triReadFile(TRFile file, TRbyte* data, TRuint length)
{
    TR_ASSERT(file);
    {
        FILE* f = (FILE*)file;
        return fread(data, 1, length, f);
    }
}

TRbyte triReadFileByte(TRFile file)
{
    TR_ASSERT(file);
    {
        FILE* f = (FILE*)file;
        return fgetc(f);
    }
}

TRuint triWriteFile(TRFile file, const TRbyte* data, TRuint length)
{
    TR_ASSERT(file);
    {
        FILE* f = (FILE*)file;
        return fwrite(data, 1, length, f);
    }
}

TRbool triEndOfFile(TRFile file)
{
    TR_ASSERT_RETURN(file, TR_TRUE);
    {
        FILE* f = (FILE*)file;
        return feof(f);
    }
}

void triSleep(TRtimestamp period)
{
    struct timespec ts;
    ts.tv_sec  = period / 1000000;
    ts.tv_nsec = (period % 1000000) * 1000;
    nanosleep(&ts, NULL);
}

TRbyte* triFindConfigFile(TRState* state, const TRbyte* name)
{
    char path[PATH_MAX];
    FILE* f;
    TR_UNUSED(state);

    /* Try looking for the config file in /etc */
    snprintf(path, PATH_MAX, PREFIX "/etc/%s", name);
    f = fopen(path, "r");
    if (f)
    {
        fclose(f);
        return triStringDuplicate(state, path);
    }

    /* Default to current directory */
    f = fopen(name, "r");
    if (f)
    {
        fclose(f);
        return triStringDuplicate(state, name);
    }

    return NULL;
}

TRbyte* triGetDefaultStreamDevice(TRState* state)
{
    return triStringDuplicate(state, "stdio");
}

TRbyte* triGetDefaultTraceOutputPath(TRState* state)
{
    const char* path = getenv("TRACY_OUTPUT_PATH");
    return triStringDuplicate(state, path ? path : "/tmp/");
}

TRbyte* triGetDefaultTraceOutputType(TRState* state)
{
    const char* type = getenv("TRACY_OUTPUT_FORMAT");
    return triStringDuplicate(state, type ? type : "binary");
}
