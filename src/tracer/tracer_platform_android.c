/**
 *  Tracer platform implementation for Android
 *  Copyright (c) 2013 Google
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
#include <android/log.h>
#include <sys/system_properties.h>

/* Global data pointer */
static void* globalData = 0;

/* Prefix for configuration files */
#ifndef PREFIX
#   define PREFIX ""
#endif /* PREFIX */

/* Platform data */
typedef struct
{
    volatile TRuint    seqCounter;
    int                verbose;
} AndroidPlatformData;

static int getIntegerProperty(const char* name, int defaultValue)
{
    char value[PROP_VALUE_MAX] = "0";
    if (__system_property_get(name, value) == 0)
    {
        return defaultValue;
    }
    return atoi(value);
}

void triPlatformInit(TRState* state)
{
    AndroidPlatformData* d = triMalloc(state, sizeof(AndroidPlatformData));
    d->seqCounter = 0;
    d->verbose = getIntegerProperty("tracy.verbose", 1);
    state->synchronousOperation = getIntegerProperty("tracy.sync", 0);
    state->platformData = d;

    /* Schedule a proper termination when the host application shuts down */
    atexit(trTerminate);
}

void triPlatformExit(TRState* state)
{
    AndroidPlatformData* d = (AndroidPlatformData*)state->platformData;
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
        __android_log_print(ANDROID_LOG_ERROR, "tracy", "Assertion failed: %s\n", desc);
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

static char messageBuffer[1024];

void triLogMessage(TRState* state, const TRbyte* format, ...)
{
    if (state)
    {
        AndroidPlatformData* d = (AndroidPlatformData*)state->platformData;
        if (d && !d->verbose)
        {
            return;
        }
    }

    va_list args;
    va_start(args, format);
    vsnprintf(messageBuffer, sizeof(messageBuffer), format, args);
    va_end(args);

    __android_log_print(ANDROID_LOG_INFO, "tracy", "%s", messageBuffer);
}

TRhandle triOpenLibrary(TRState* state, const TRbyte* name)
{
    return (TRhandle)dlopen(name, RTLD_NOW);
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
}

TRuint triGetNextSequenceNumber(TRState* state)
{
    AndroidPlatformData* d = (AndroidPlatformData*)state->platformData;
    TRuint c;
    c = __sync_fetch_and_add(&d->seqCounter, 1);
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

    /* Try looking for the config file in /system/etc */
    snprintf(path, PATH_MAX, PREFIX "/system/etc/%s", name);
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
    char path[PATH_MAX];
    if (!__system_property_get("tracy.path", path))
    {
        strncpy(path, "/sdcard/Download/", sizeof(path));
    }
    return triStringDuplicate(state, path);
}

TRbyte* triGetDefaultTraceOutputType(TRState* state)
{
    char type[PROP_VALUE_MAX];
    if (!__system_property_get("tracy.format", type))
    {
        strncpy(type, "binary", sizeof(type));
    }
    return triStringDuplicate(state, type);
}
