/**
 *  Symbian file stream device
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
#include "tracer_internal.h"
#include "tracer_platform.h"
#include "tracer_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <f32file.h>
#include <e32std.h>

/**
 *  Buffer
 */
typedef struct
{
    TRbyte*   data;
    TRuint    size;
    TRuint    pos;
} Buffer;

/**
 *  Stream state
 */
typedef struct 
{
    TRState*        state;
    RFile*          file;
    TRbool          done;
    TRuint          bufferCount;
    Buffer*         buffers;
    TRuint          bufferIn, bufferOut;
    TRint           autoFlushTimeout;
    RThread*        worker;
    RSemaphore*     fullBuffers;
    RSemaphore*     emptyBuffers;
    /** Global mutex, only used while automatically flushing imcomplete buffers */
    RMutex*         lock;
} SymbianStream;

/**
 *  Device state
 */
typedef struct
{
    TRState*  state;
    RFs*      fileServer;
} SymbianDevice;

extern "C"
{
    
static void flushBuffer(Buffer* buffer, RFile& file)
{
    if (buffer->pos)
    {
        TPtrC8 data((TUint8*)buffer->data, buffer->pos);
        file.Write(data);
        buffer->pos = 0;
    }
}

static void fillBuffer(Buffer* buffer, RFile& file)
{
    TPtr8 data((TUint8*)buffer->data, buffer->size);
    file.Read(data);
    buffer->size = data.Length();
    buffer->pos  = 0;
}

/**
 *  Asynchronous writer thread.
 *
 *  This thread waits for filled buffers via the writeRequest condition
 *  variable and writes their contents to the output file.
 */
TInt writerThread(TAny* arg)
{
    SymbianStream* s = (SymbianStream*)arg;

    while (!s->done)
    {
        /* Wait for write request */
        if (s->autoFlushTimeout > 0 && s->fullBuffers->Wait(s->autoFlushTimeout) == KErrTimedOut)
        {
            /* If we had a time out, we should flush all the pending buffers */
            s->lock->Wait();
            while (s->buffers[s->bufferOut].pos)
            {
                flushBuffer(&s->buffers[s->bufferOut], *s->file);
                s->bufferOut = (s->bufferOut + 1) % s->bufferCount;
            }
            s->lock->Signal();
            continue;
        }
        else
        {
            s->fullBuffers->Wait();
        }
        
        if (s->done)
        {
            break;
        }
        
        /* Flush the filled buffer */
        ASSERT(s->buffers[s->bufferOut].pos == s->buffers[s->bufferOut].size);
        flushBuffer(&s->buffers[s->bufferOut], *s->file);
        
        /* Go for the next buffer */
        s->bufferOut = (s->bufferOut + 1) % s->bufferCount;
        
        /* Tell the writer that there is an empty buffer available */
        s->emptyBuffers->Signal();
    }
    
    /* Flush all the buffers in the correct order */
    while (s->buffers[s->bufferOut].pos)
    {
        flushBuffer(&s->buffers[s->bufferOut], *s->file);
        s->bufferOut = (s->bufferOut + 1) % s->bufferCount;
    }
    return KErrNone;
}

/**
 *  Asynchronous reader thread.
 *
 *  This thread waits for empty buffers via the readRequest condition
 *  variable and fills them by reading from the input file.
 */
TInt readerThread(TAny* arg)
{
    SymbianStream* s = (SymbianStream*)arg;

    while (!s->done)
    {
        /* Wait for read request */
        s->emptyBuffers->Wait();
        
        if (s->done)
        {
            break;
        }
        
        /* Fill the empty buffer */
        ASSERT(s->buffers[s->bufferIn].pos == s->buffers[s->bufferIn].size);
        fillBuffer(&s->buffers[s->bufferIn], *s->file);
        
        /* Go for the next buffer */
        s->bufferIn = (s->bufferIn + 1) % s->bufferCount;
        
        /* Tell the writer that there is a full buffer available */
        s->fullBuffers->Signal();
    }
    return KErrNone;
}

TRuint symbianStreamRead(TRStream* stream, TRbyte* data, TRuint length)
{
    SymbianStream* s = (SymbianStream*)stream->handle;
    TRuint bytes, read = 0;

    while (length && s->buffers[s->bufferOut].size)
    {
        Buffer* buffer = &s->buffers[s->bufferOut];
        bytes = TR_MIN(length, buffer->size - buffer->pos);
        
        triMemCopy(&data[read], &buffer->data[buffer->pos], bytes);
        buffer->pos  += bytes;
        length       -= bytes;
        read         += bytes;
        
        /* If the buffer was fully consumed, let the writer know and acquire a new buffer */
        if (buffer->pos == buffer->size)
        {
            s->emptyBuffers->Signal();
            s->bufferOut = (s->bufferOut + 1) % s->bufferCount;
            s->fullBuffers->Wait();
            TR_ASSERT(s->buffers[s->bufferOut].pos == 0);
        }
    }
    return read;
}

TRuint symbianStreamWrite(TRStream* stream, const TRbyte* data, TRuint length)
{
    SymbianStream* s = (SymbianStream*)stream->handle;
    TRuint bytes, written = 0;
    
    s->lock->Wait();
    while (length)
    {
        Buffer* buffer = &s->buffers[s->bufferIn];
        
        bytes = TR_MIN(length, buffer->size - buffer->pos);
        memcpy(&buffer->data[buffer->pos], &data[written], bytes);
        buffer->pos  += bytes;
        length       -= bytes;
        written      += bytes;
        
        /* If the buffer filled up, let the writer know and acquire a new buffer */
        if (buffer->pos == buffer->size)
        {
            s->fullBuffers->Signal();
            s->bufferIn = (s->bufferIn + 1) % s->bufferCount;
            s->emptyBuffers->Wait();
            TR_ASSERT(s->buffers[s->bufferIn].pos == 0);
        }
    }
    s->lock->Signal();

    return written;
}

/* Stream operations */
static const TRStreamOperations symbianStreamOps = 
{
    symbianStreamRead,
    symbianStreamWrite,
    NULL /* flush */
};

static TRint getIntegerOption(const TRStrDict* options, const TRbyte* key, TRint dfl)
{
    const TRbyte* value = options ? (const TRbyte*)triStrDictGet(options, key) : (const TRbyte*)0;
    return value ? atoi(value) : dfl;
}

TRenum symbianOpenStream(TRStreamDevice* dev, TRStream* stream, const TRbyte* name, const TRbyte* mode, const TRStrDict* options)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    {
        SymbianDevice* d = (SymbianDevice*)dev->handle;
        RFile          file;
        TRuint         i;
        TInt           err;
        
        /* Try opening the file first */
        TBuf<256>      fileName;
        fileName.Copy(TPtrC8((TUint8*)name, triStringLength(name)));
        
        if (mode[0] == 'w')
        {
            err = file.Replace(*d->fileServer, fileName, EFileStream | EFileWrite);
        }
        else
        {
            TR_ASSERT(mode[0] == 'r');
            err = file.Open(*d->fileServer, fileName, EFileStream | EFileRead);
        }
      
        if (err != KErrNone)
        {
            triLogMessage(d->state, "Unable to open stream: %d");
            return TR_NOT_FOUND;
        }
        
        SymbianStream* s = (SymbianStream*)triMalloc(d->state, sizeof(SymbianStream));
        TR_ASSERT_RETURN(s, TR_OUT_OF_MEMORY);
        
        s->state        = d->state;
        s->file         = new RFile();
        
        /* Duplicate the file handle for the thread. */
        s->file->Duplicate(file);

        /* Close the original file since we don't need it in this thread anymore */
        file.Close();
        
        s->bufferCount      = getIntegerOption(options, "buffer_count", 4);
        s->autoFlushTimeout = getIntegerOption(options, "auto_flush_timeout", 0) * 1000;
        s->buffers          = (Buffer*)triMalloc(d->state, s->bufferCount * sizeof(Buffer));
        s->bufferIn         = 0;
        s->bufferOut        = 0;
        s->done             = TR_FALSE;
        s->fullBuffers      = new RSemaphore();
        s->emptyBuffers     = new RSemaphore();
        s->lock             = new RMutex();
        s->worker           = 0;
        
        for (i = 0; i < s->bufferCount; i++)
        {
            s->buffers[i].size = getIntegerOption(options, "buffer_size", 512 * 1024);
            s->buffers[i].data = (TRbyte*)triMalloc(d->state, sizeof(TRbyte) * s->buffers[i].size);
            s->buffers[i].pos  = 0;
            if (!s->buffers[i].data)
            {
                do
                {
                    triFree(d->state, s->buffers[i].data);
                } while (--i);
                triFree(d->state, s->buffers);
                s->buffers = 0;
            }
        }
        if (!s->buffers)
        {
            delete s->file;
            s->fullBuffers->Close();;
            delete s->fullBuffers;
            s->emptyBuffers->Close();
            delete s->emptyBuffers;
            s->lock->Close();
            delete s->lock;
            triFree(d->state, s);
            return TR_OUT_OF_MEMORY;
        }

        s->lock->CreateLocal();
        
        if (mode[0] == 'w' || mode[0] == 'a')
        {
            _LIT(KTracerWriterThread, "TracerWriterThread");
            TBuf<32> threadName;
            threadName.Append(KTracerWriterThread);
            threadName.AppendNum((TUint)d->state);
            s->worker = new RThread();
            TInt err = s->worker->Create(threadName, writerThread, KDefaultStackSize, KMinHeapSize, 4 * 1024 * 1024, s);
            TR_ASSERT(err == KErrNone);
            
            /* Initialize the buffer semaphores */
            s->fullBuffers->CreateLocal(0);
            s->emptyBuffers->CreateLocal(s->bufferCount - 1);
            
            s->worker->Resume();
        }
        else if (mode[0] == 'r')
        {
            /* Fill all the buffers right away */
            for (i = 0; i < s->bufferCount; i++)
            {
                fillBuffer(&s->buffers[i], *s->file);
            }

            /* Initialize the buffer semaphores */
            s->fullBuffers->CreateLocal(s->bufferCount - 1);
            s->emptyBuffers->CreateLocal(0);
          
            _LIT(KReaderThread, "TracerReaderThread");
            s->worker = new RThread();
            TR_ASSERT(s->worker->Create(KReaderThread, readerThread, KDefaultStackSize, KMinHeapSize, 4 * 1024 * 1024, s) == KErrNone);
            s->worker->Resume();
        }
        
        stream->ops     = &symbianStreamOps;
        stream->handle  = s;
        return s->file ? TR_NO_ERROR : TR_NOT_FOUND;
    }
}

TRenum symbianCloseStream(TRStreamDevice* dev, TRStream* stream)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    
    if (stream->handle)
    {
        SymbianDevice* d = (SymbianDevice*)dev->handle;
        SymbianStream* s = (SymbianStream*)stream->handle;
        TRuint i;
        
        /* Tell the writer thread to exit */
        s->done = TR_TRUE;
        if (s->worker)
        {
            TRequestStatus req;
            s->worker->Rendezvous(req);
            s->fullBuffers->Signal();
            s->emptyBuffers->Signal();
            User::WaitForRequest(req);
            s->worker->Close();
            delete s->worker;
            s->worker = 0;
        }
        
        /* Free stream data */
        for (i = 0; i < s->bufferCount; i++)
        {
            triFree(d->state, s->buffers[i].data);
        }
        triFree(d->state, s->buffers);

        s->file->Close();
        delete s->file;
        s->fullBuffers->Close();;
        delete s->fullBuffers;
        s->emptyBuffers->Close();
        delete s->emptyBuffers;
        s->lock->Close();
        delete s->lock;
        triFree(s->state, s);
        
        stream->handle = 0;
        return TR_NO_ERROR;
    }
    return TR_INVALID_HANDLE;
}

TRenum symbianExit(TRStreamDevice* dev)
{
    SymbianDevice* d = (SymbianDevice*)dev->handle;
    
    if (d)
    {
        d->fileServer->Close();
        delete d->fileServer;
        triFree(d->state, d);
    }
    return TR_NO_ERROR;
}

/* Device operations */
static const TRStreamDeviceOperations symbianStreamDeviceOps = 
{
    NULL,        /* init */
    symbianExit,
    symbianOpenStream,
    symbianCloseStream
};

/* Device factory */
TRenum symbianCreateDevice(TRState* state, TRStreamDevice* dev)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(dev, TR_INVALID_HANDLE);
    
    SymbianDevice* d = (SymbianDevice*)triMalloc(state, sizeof(SymbianDevice));
    
    if (!d)
    {
        return TR_OUT_OF_MEMORY;
    }
    
    d->state         = state;
    d->fileServer    = new RFs();
    
    if (!d->fileServer)
    {
        return TR_OUT_OF_MEMORY;
    }
    
    TR_ASSERT(d->fileServer->Connect() == KErrNone);
    
    dev->handle     = (TRhandle)d;
    dev->ops        = &symbianStreamDeviceOps;
    return TR_NO_ERROR;
}

} /* extern "C" */
