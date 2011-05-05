/**
 *  Tracer platform implementation for Symbian
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <e32std.h>
#include <e32debug.h>
#include <e32property.h>
#include <e32const.h>
#include <hal.h>
#include <f32file.h>

_LIT(KTracerMutex, "TracerMutex");
_LIT(KTracerChunk, "TracerChunk");

#ifdef USE_WIN32_SHM
#define NO_CHUNK
#include <windows.h>
#endif // USE_WIN32_SHM

class SymbianPlatformData
{
public:
    SymbianPlatformData():
      heap(0),
      threadSeqCounter(0),
      seqCounter(0),
      seqCounterInitialized(EFalse)
    {
    }

    /* Memory allocation heap */
    RHeap*    heap;
    
    /* Lock for moderating access to this platform structure */
    RMutex    lock;
    
    /* Global chunk for storing the sequence counter value */ 
    RChunk    chunk;
    
    /* Timer period */
    TInt      tickMultiplier;
    TInt      tickDivisor;
    TUint32   lastTimerValue;
    TUint32   timerOverflow;
    
    /* Sequence counter data */
    TUint32   threadSeqCounter;
    TUint32*  seqCounter;
    TBool     seqCounterInitialized;
    
#ifdef USE_WIN32_SHM
    HANDLE    sharedMemory;
#endif

    /* File server session */
    RFs       fileServer;
};

class SymbianFile
{
public:
        TRState*    state;
        RFile       file;
};

#if !defined(SYMBIAN_DLL)
static void* globalData = 0;
#endif

const TInt microsecondsPerSecond = 1000000;

extern "C"
{

void triPlatformInit(TRState* state)
{
    SymbianPlatformData* plat = new SymbianPlatformData();
    TR_ASSERT(plat);
    
    /* Create the largest heap possible */
    TInt ramSize;
    HAL::Get(HALData::EMemoryRAM, ramSize);

    ramSize >>= 2;
    while (ramSize && (plat->heap = UserHeap::ChunkHeap(NULL, 0, ramSize)) == NULL)
    {
        ramSize >>= 1;
    }
    
    if (!plat->heap)
    {
        triLogMessage(state, "Unable to create tracer data heap.");
    }
    
    /* Initialize the global mutex lock */
    if (plat->lock.OpenGlobal(KTracerMutex) != KErrNone)
    {
        TInt err = plat->lock.CreateGlobal(KTracerMutex);
        TR_ASSERT(err == KErrNone);
    }

    /* Initialize the global chunk and the sequence counter */
#ifdef NO_CHUNK
#ifdef USE_WIN32_SHM
    triLogMessage(state, "Using win32 shared memory.");
    plat->sharedMemory = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, 4, "TracerMemory");
    TR_ASSERT(plat->sharedMemory);
    plat->seqCounter = (TUint32*)MapViewOfMemory(plat->sharedMemory, FILE_MAP_WRITE, 0, 0, 4);
    TR_ASSERT(plat->seqCounter);
    plat->seqCounterInitialized = ETrue;
#else
    plat->seqCounterInitialized = EFalse;
#endif // USE_WIN32_SHM

#else
    plat->seqCounterInitialized = ETrue;
    if (plat->chunk.OpenGlobal(KTracerChunk, EFalse) != KErrNone)
    {
        TInt err = plat->chunk.CreateGlobal(KTracerChunk, sizeof(TUint32), sizeof(TUint32));
        if (err == KErrNone)
        {
            triLogMessage(state, "Global sequence counter created and initialized.");
            plat->seqCounter = (TUint32*)plat->chunk.Base();
            TR_ASSERT(plat->seqCounter);
            *(plat->seqCounter) = 0;
        }
        else
        {
            triLogMessage(state, "Global sequence counter initialization failed with error code %d. "
                                 "Switching to thread-local counter.", err);
            plat->seqCounterInitialized = EFalse;
        }
    }
    else
    {
        plat->seqCounter = (TUint32*)plat->chunk.Base();
        TR_ASSERT(plat->seqCounter);
        triLogMessage(state, "Global sequence counter opened. Count is %d.", *(plat->seqCounter));
    }
#endif

    /* Store the nanokernel timer frequency */
    TInt tickFrequency;
    HAL::Get(HAL::EFastCounterFrequency, tickFrequency);
    triLogMessage(state, "System timer frequency is %d Hz.", tickFrequency);
    
    /* We aim for microsecond accuracy, so set up the multiplier and divisor accordingly */
    if (tickFrequency > microsecondsPerSecond)
    {
        plat->tickMultiplier = 1;
        plat->tickDivisor    = tickFrequency / microsecondsPerSecond;
    }
    else
    {
        plat->tickMultiplier = microsecondsPerSecond / tickFrequency;
        plat->tickDivisor    = 1;
    }
    plat->lastTimerValue = 0;
    plat->timerOverflow  = 0;
    TR_ASSERT(plat->tickDivisor > 0);
    
    /* Initialize the file server */
    TR_ASSERT(plat->fileServer.Connect() == KErrNone);

    triLogMessage(state, "Symbian platform initialized.");
    state->platformData = plat;
}

void triPlatformExit(TRState* state)
{
    SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;

    /* Delete the platform data */
    if (plat->heap)
    {
        plat->heap->Close();
        plat->heap = 0;
    }
#ifdef USE_WIN32_SHM
    UnmapViewOfMemory(plat->seqCounter);
    CloseHandle(plat->sharedMemory);
#endif
    
    plat->lock.Close();
#ifndef NO_CHUNK
    plat->chunk.Close();
#endif
    plat->seqCounter = 0;
    plat->fileServer.Close();
    delete plat;
    state->platformData = 0;
}

void triSetGlobal(void* data)
{
#if defined(SYMBIAN_DLL)
    Dll::SetTls(data);
#else
    globalData = data;
#endif
}

void* triGetGlobal(void)
{
#if defined(SYMBIAN_DLL)
    return Dll::Tls();
#else
    return globalData;
#endif
}

void triAssert(const TRbyte* desc, TRbool value)
{
    if (!value)
    {
        triLogMessage(TR_NO_STATE, "Assertion failed: %s", desc);
        User::RaiseException(EExcAbort);
    }
}

void* triMalloc(TRState* state, TRuint bytes)
{
    void *allocation = 0;
    
    if (state)
    {
        SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
        if (plat && plat->heap)
        {
            allocation = plat->heap->Alloc(bytes);
        }
    }
    else
    {
        allocation = User::Alloc(bytes);
    }
    
    if (!allocation)
    {
        triLogMessage(state, "Allocation of %d bytes failed. Using custom heap: %d", bytes, state ? 1 : 0);
    }
    
    return allocation;
}

void triFree(TRState* state, void* data)
{
    if (data)
    {    
        if (state)
        {
            SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
            if (plat && plat->heap)
            {
                plat->heap->Free(data);
                return;
            }
        }
        User::Free(data);
    }
}

void* triRealloc(TRState* state, void* data, TRuint bytes)
{
    if (state)
    {
        SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
        if (plat && plat->heap)
        {
            return plat->heap->ReAlloc(data, bytes);
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
    
    /* TODO: Don't do this if we were called inside the file functions */
#if 0
        if (state)
        {
                #if defined(TR_PLATFORM_UNIX)
                    TRFile f = triOpenFile(state, "f://tracer_debug.log", "a");
                #endif
                
                #if !defined(TR_PLATFORM_UNIX)
                    TRFile f = triOpenFile(state, "f:\\tracer_debug.log", "a");
                #endif
        
            if (f)
            {
                    triWriteFile(f, str, triStringLength(str));
                    triWriteFile(f, "\n", 1);
                triCloseFile(f);
            }
        }
#else
    TR_UNUSED(state);
#endif

    TBuf8<256> buf8((const TUint8*)str);
    TBuf16<256> buf16;
    TBuf16<256> msg;
    
    buf16.Copy(buf8);
    msg.Append(_L("[Tracy] "));
    msg.Append(buf16);
    
    RDebug::Print(msg);
}

TRhandle triOpenLibrary(TRState* /*state*/, const TRbyte* name)
{
    RLibrary* lib = new RLibrary();
    TBuf8<256> name8((const TUint8*)name);
    TBuf16<256> name16;
    
    name16.Copy(name8);
    
    TInt err = lib->Load(name16);
    
    if (err == KErrNone)
    {
        return (TRhandle)lib;
    }
    delete lib;
    return 0;
}

void *triLookupSymbol(TRhandle handle, const TRbyte* /*name*/, TRint ordinal)
{
    if (handle && ordinal >= 0)
    {
        RLibrary* lib = (RLibrary*)handle;
        return (void*)lib->Lookup(ordinal);
    }
    return 0;
}

void triCloseLibrary(TRhandle handle)
{
    if (handle)
    {
        RLibrary* lib = (RLibrary*)handle;
        lib->Close();
        delete lib;
    }
}

TRtimestamp triGetSystemTime(TRState* state)
{
    SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
    TUint32 t = User::FastCounter();
    
    /* Handle wrap-around -- a maximum of one cycle */
    if (plat->lastTimerValue > t)
    {
        plat->timerOverflow += 1;
    }
    plat->lastTimerValue = t;

    /* Convert the time into microseconds. */
    TRtimestamp ts = (plat->timerOverflow * 0x100000000) + t;
    return (plat->tickMultiplier * ts) / plat->tickDivisor;
}

void triGetProcessName(TRbyte* name, TRuint length)
{
    RProcess process;
    TName procName = process.Name();
    TInt i;
    
    length = Min(length - 1, procName.Length());
    
    for (i = 0; i < length; i++)
    {
        name[i] = procName[i];
    }
    name[i] = 0;
}

TRuint triGetNextSequenceNumber(TRState* state)
{
    SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
    TInt value;
    
    /* Increase the counter atomically */
    plat->lock.Wait();
    if (plat->seqCounterInitialized)
    {
        value = *plat->seqCounter;
        (*plat->seqCounter)++;
    }
    else
    {
        value = plat->threadSeqCounter++;
    }
    plat->lock.Signal();
    return value;
}

void triMemSet(void* data, TRuint value, TRuint bytes)
{
    Mem::Fill(data, bytes, value);
}

void triMemCopy(void* dest, const void* src, TRuint bytes)
{
    Mem::Copy(dest, src, bytes);
}

TRint triMemCompare(const void* data1, const void* data2, TRuint bytes)
{
    return Mem::Compare((const TUint8*)data1, bytes, (const TUint8*)data2, bytes);
}

TRFile triOpenFile(TRState* state, const TRbyte* name, const TRbyte* mode)
{
    TR_ASSERT(mode);
    TInt err;
    SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
    
    if (!plat)
    {
        return 0;
    }

    SymbianFile* f = new SymbianFile();
        
    if (!f)
    {
        triLogMessage(state, "Out of memory while opening a file");
        return 0;
    }
    
    /* Initialize the file */
    f->state = state;
    
    /* Make a copy of the file name */
    TBuf<256> fileName;
    fileName.Copy(TPtrC8((TUint8*)name, triStringLength(name)));
    
    TUint flags = 0;
    
    if (mode[1])
    {
        if (mode[1] == 'b')
        {
            flags |= EFileStream;
        }
        if (mode[1] == '+' || mode[2] == '+')
        {
            flags |= EFileRead | EFileWrite;
        }
    }

        /* Open the file */     
    if (mode[0] == 'w')
    {
        err = f->file.Replace(plat->fileServer, fileName, flags | EFileWrite);
    }
    else if (mode[0] == 'r')
    {
        err = f->file.Open(plat->fileServer, fileName, flags | EFileRead);
    }
    else if (mode[0] == 'a')
    {
        err = f->file.Open(plat->fileServer, fileName, flags | EFileWrite);
        if (err == KErrNone)
        {
            TInt pos = 0;
            f->file.Seek(ESeekEnd, pos);
        }
    }
    else
    {
        triLogMessage(state, "Bad file mode: %s", mode);
        delete f;
        return 0;
    }
    
    if (err != KErrNone)
    {
        triLogMessage(state, "Opening file %s failed with error %d", name, err);
        delete f;
        return 0;
    }
    
    return f;
}

void triCloseFile(TRFile file)
{
    if (file)
    {
            SymbianFile* f = (SymbianFile*)file;
            f->file.Close();
            delete f;
    }
}

TRuint triReadFile(TRFile file, TRbyte* data, TRuint length)
{
    TR_ASSERT(file);
    {
        SymbianFile* f = (SymbianFile*)file;
        TPtr8 d((TUint8*)data, length);
        f->file.Read(d);
        return d.Length();
    }
}

TRbyte triReadFileByte(TRFile file)
{
    TR_ASSERT(file);
    {
        SymbianFile* f = (SymbianFile*)file;
        TBuf8<1> d;
        f->file.Read(d);
        return d[0];
    }
}

TRuint triWriteFile(TRFile file, const TRbyte* data, TRuint length)
{
    TR_ASSERT(file);
    {
        SymbianFile* f = (SymbianFile*)file;
        TPtrC8 d((TUint8*)data, length);
        f->file.Write(d);
        return d.Length();
    }
}

TRbool triEndOfFile(TRFile file)
{
    TR_ASSERT_RETURN(file, TR_TRUE);
    {
        SymbianFile* f = (SymbianFile*)file;
        TInt pos = 0, size = 0;
        
        f->file.Seek(ESeekCurrent, pos);
        f->file.Size(size);
        
        return pos >= size;
    }
}

void triSleep(TRtimestamp period)
{
    User::AfterHighRes(period);
}

TRbyte* triFindConfigFile(TRState* state, const TRbyte* name)
{
    SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
    char path[256];
    TBuf<256> fileName;

    /* Look for the file in the root of every drive */
    for (TInt drive = 'Z'; drive >= 'A'; drive--)
    {
        RFile file;
        
        #if defined(TR_PLATFORM_UNIX)
            sprintf(path, "%c://%s", drive, name);
        #endif
        
        #if !defined(TR_PLATFORM_UNIX)
            sprintf(path, "%c:\\%s", drive, name);
        #endif
        
        fileName.Copy(TPtrC8((TUint8*)path, triStringLength(path)));
        if (file.Open(plat->fileServer, fileName, EFileRead) == KErrNone)
        {
            file.Close();
            return triStringDuplicate(state, path);
        }
    }
    return NULL;
}

TRbyte* triGetDefaultStreamDevice(TRState* state)
{
    return triStringDuplicate(state, "symbian");
}

TRbyte* triGetDefaultTraceOutputPath(TRState* state)
{
    SymbianPlatformData* plat = (SymbianPlatformData*)state->platformData;
    char path[256];
    TBuf<256> fileName;

    /* Try to create a file to the root of every drive until it succeeds */
    for (TInt drive = 'Z'; drive >= 'A'; drive--)
    {
        RFile file;

        #if defined(TR_PLATFORM_UNIX)
            sprintf(path, "%c://__tracy__.dat", drive);
        #endif

        #if !defined(TR_PLATFORM_UNIX)
            sprintf(path, "%c:\\__tracy__.dat", drive);
        #endif

        fileName.Copy(TPtrC8((TUint8*)path, triStringLength(path)));
        if (file.Replace(plat->fileServer, fileName, EFileWrite) == KErrNone)
        {
            file.Close();
            plat->fileServer.Delete(fileName);
            path[3] = 0;
            return triStringDuplicate(state, path);
        }
    }
    return triStringDuplicate(state, ".");
}

TRbyte* triGetDefaultTraceOutputType(TRState* state)
{
    const char* type = getenv("TRACY_OUTPUT_TYPE");
    return triStringDuplicate(state, type ? type : "binary");
}

} // extern "C"
