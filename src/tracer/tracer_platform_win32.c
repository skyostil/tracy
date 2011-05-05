/**
 *  Tracer platform implementation for Win32
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

#include "tracer.h"
#include "tracer_platform.h"
#include "tracer_internal.h"
#include "tracer_util.h"
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct
{
    HANDLE heap;
    HANDLE lock;
    
    TRuint sequenceNumber;
} Win32PlatformData;

static void* globalData = 0;

void triPlatformInit(TRState* state)
{
    Win32PlatformData* plat = (Win32PlatformData*)malloc(sizeof(Win32PlatformData));
    TR_ASSERT(plat);
    
    /* Create the largest heap possible */
    plat->heap = HeapCreate(0, 0, 0);
    
    if (!plat->heap)
    {
        triLogMessage(state, "Unable to create tracer data heap.");
    }
    
    /* Initialize the global mutex lock */
    plat->lock = CreateMutex(NULL, FALSE, NULL);

    /* Initialize the sequence counter */
    plat->sequenceNumber = 0;

    /* Increase timer granularity to 1 ms */
    timeBeginPeriod(1);
    
    /* All done */
    state->platformData = plat;
}

void triPlatformExit(TRState* state)
{
    Win32PlatformData* plat = (Win32PlatformData*)state->platformData;

    /* Reset timer granularity */
    timeEndPeriod(1);

    /* Delete the platform data */
    if (plat->heap)
    {
        HeapDestroy(plat->heap);
        plat->heap = 0;
    }
    
    CloseHandle(plat->lock);
    
    free(plat);
    state->platformData = 0;
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
        triLogMessage(TR_NO_STATE, "Assertion failed: %s", desc);
        __debugbreak();
    }
}

void* triMalloc(TRState* state, TRuint bytes)
{
    if (state)
    {
        Win32PlatformData* plat = (Win32PlatformData*)state->platformData;
        if (plat && plat->heap)
        {
            return HeapAlloc(plat->heap, 0, bytes);
        }
    }
    return malloc(bytes);
}

void triFree(TRState* state, void* data)
{
    if (data)
    {    
        if (state)
        {
            Win32PlatformData* plat = (Win32PlatformData*)state->platformData;
            if (plat && plat->heap)
            {
                HeapFree(plat->heap, 0, data);
                return;
            }
        }
        free(data);
    }
}

void* triRealloc(TRState* state, void* data, TRuint bytes)
{
    if (!data)
    {
        return triMalloc(state, bytes);
    }
  
    if (state)
    {
        Win32PlatformData* plat = (Win32PlatformData*)state->platformData;
        if (plat && plat->heap)
        {
            return HeapReAlloc(plat->heap, 0, data, bytes);
        }
    }
    return realloc(data, bytes);
}

void triLogMessage(TRState* state, const TRbyte* format, ...)
{
    char str[256];

    va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);
    strncat(str, "\n", sizeof(str));
    
    OutputDebugString(str);
}

TRhandle triOpenLibrary(TRState* state, const TRbyte* name)
{
    HMODULE lib = LoadLibrary(name);
    return (TRhandle)lib;
}

void *triLookupSymbol(TRhandle handle, const TRbyte* name, TRint ordinal)
{
    if (handle && name)
    {
        HMODULE lib = (HMODULE)handle;
        return GetProcAddress(lib, name);
    }
    return 0;
}

void triCloseLibrary(TRhandle handle)
{
    if (handle)
    {
        HMODULE lib = (HMODULE)handle;
        FreeLibrary(lib);
    }
}

TRtimestamp triGetSystemTime(TRState* state)
{
    TR_UNUSED(state);
    return GetTickCount() * 1000;
}

void triGetProcessName(TRbyte* name, TRuint length)
{
    TCHAR path[MAX_PATH];
    HMODULE process = GetModuleHandle(NULL);
    LPSTR fileName;
    GetModuleFileName(process, path, sizeof(path));
    fileName = PathFindFileName(path);
    strncpy(name, fileName, length);
}

TRuint triGetNextSequenceNumber(TRState* state)
{
    Win32PlatformData* plat = (Win32PlatformData*)state->platformData;
    TRuint value;
    
    /* Increase the counter atomically */
    WaitForSingleObject(plat->lock, INFINITE);
    value = plat->sequenceNumber++;
    ReleaseMutex(plat->lock);
    return value;
}

void triMemSet(void* data, TRuint value, TRuint bytes)
{
    FillMemory(data, bytes, value);
}

void triMemCopy(void* dest, const void* src, TRuint bytes)
{
    CopyMemory(dest, src, bytes);
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
    Sleep(period / 1000);
}

TRbyte* triFindConfigFile(TRState* state, const TRbyte* name)
{
    /* Look for the file in the current directory only */
    FILE* f = fopen(name, "r");
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
    return triStringDuplicate(state, ".");
}

TRbyte* triGetDefaultTraceOutputType(TRState* state)
{
    return triStringDuplicate(state, "binary");
}
