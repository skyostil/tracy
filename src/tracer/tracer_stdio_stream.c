/**
 *  STDIO file stream device
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
#include <string.h>
#include <stdlib.h>

/**
 *  Stream state
 */
typedef struct 
{
    TRbyte*   buffer;
    TRuint    bufferSize;
    TRuint    bufferPos;
    TRState*  state;
    TRFile    file;
} StdioStream;

static void fillBuffer(TRStream* stream)
{
    /* 
     * Note: The buffer size shrinks once we reach the end of the file. The
     *       file needs to be reopened to restore the original buffer size.
     */
    StdioStream* s = (StdioStream*)stream->handle;
    s->bufferSize  = triReadFile(s->file, s->buffer, s->bufferSize);
    s->bufferPos   = 0;
}

TRuint stdioStreamRead(TRStream* stream, TRbyte* data, TRuint length)
{
    StdioStream* s = (StdioStream*)stream->handle;
    TRuint    read = 0;
    
    while (length && s->bufferSize)
    {
        TRuint bytes = TR_MIN(length, s->bufferSize - s->bufferPos);
        
        triMemCopy(&data[read], &s->buffer[s->bufferPos], bytes);
        s->bufferPos += bytes;
        length       -= bytes;
        read         += bytes;
        
        if (s->bufferPos == s->bufferSize)
        {
            fillBuffer(stream);
        }
    }
    return read;
}

static void flushBuffer(TRStream* stream)
{
    StdioStream* s = (StdioStream*)stream->handle;
    if (s->bufferPos)
    {
        triWriteFile(s->file, s->buffer, s->bufferPos);
        s->bufferPos = 0;
    }
}

TRuint stdioStreamWrite(TRStream* stream, const TRbyte* data, TRuint length)
{
    StdioStream* s = (StdioStream*)stream->handle;
    TRuint written = 0;
    
    while (length)
    {
        TRuint bytes = TR_MIN(length, s->bufferSize - s->bufferPos);
        memcpy(&s->buffer[s->bufferPos], &data[written], bytes);
        s->bufferPos += bytes;
        length       -= bytes;
        written      += bytes;
        if (s->bufferPos == s->bufferSize)
        {
            flushBuffer(stream);
        }
    }
    return written;
}

/* Stream operations */
static const TRStreamOperations stdioStreamOps = 
{
    stdioStreamRead,
    stdioStreamWrite,
    NULL /* flush */
};

static TRint getIntegerOption(const TRStrDict* options, const TRbyte* key, TRint dfl)
{
    const TRbyte* value = options ? triStrDictGet(options, key) : (void*)0;
    return value ? atoi(value) : dfl;
}

TRenum stdioOpenStream(TRStreamDevice* dev, TRStream* stream, const TRbyte* name, const TRbyte* mode, const TRStrDict* options)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    {
        TRState*  state = triGetState();
        StdioStream* s  = triMalloc(state, sizeof(StdioStream));
        
        TR_ASSERT_RETURN(s, TR_OUT_OF_MEMORY);
        
        s->state        = state;
        s->file         = triOpenFile(state, name, mode);

        if (!s->file)
        {
            triFree(state, s);
            return TR_NOT_FOUND;
        }

        s->bufferSize   = getIntegerOption(options, "buffer_size", 512);
        s->buffer       = triMalloc(state, s->bufferSize * sizeof(TRbyte));
        s->bufferPos    = 0;
        
        if (!s->buffer)
        {
            triFree(state, s);
            triCloseFile(s->file);
            return TR_OUT_OF_MEMORY;
        }
        
        stream->ops     = &stdioStreamOps;
        stream->handle  = s;
        
        /* Fill the read buffer right away */
        if (mode[0] == 'r')
        {
            fillBuffer(stream);
        }
        
        return s->file ? TR_NO_ERROR : TR_NOT_FOUND;
    }
}

TRenum stdioCloseStream(TRStreamDevice* dev, TRStream* stream)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    
    if (stream->handle)
    {
        StdioStream* s = (StdioStream*)stream->handle;
        flushBuffer(stream);
        triCloseFile(s->file);
        triFree(s->state, s->buffer);
        triFree(s->state, s);
        stream->handle = 0;
        return TR_NO_ERROR;
    }
    return TR_INVALID_HANDLE;
}

/* Device operations */
static const TRStreamDeviceOperations stdioStreamDeviceOps = 
{
    NULL, /* init */
    NULL, /* exit */
    stdioOpenStream,
    stdioCloseStream
};

/* Device factory */
TRenum stdioCreateDevice(TRState* state, TRStreamDevice* dev)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(dev, TR_INVALID_HANDLE);
    
    dev->handle     = (TRhandle)0xdadacafe;
    dev->ops        = &stdioStreamDeviceOps;
    return TR_NO_ERROR;
}
