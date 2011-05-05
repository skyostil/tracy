/**
 *  STI stream device
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
#include <stdlib.h>
#include <e32std.h>
#include <e32des16.h>
#include <e32debug.h>

/**
 *  Stream state
 */
typedef struct 
{
    TPtr16*   buffer;
    HBufC16*  bufferData;
    TRState*  state;
    HBufC16*  name;
    TBool     hexEncoding;
} StiStream;

extern "C"
{

static void flushBuffer(TRStream* stream)
{
    StiStream* s = (StiStream*)stream->handle;
    
    if (s->buffer->Length())
    {
        RDebug::Print(*s->buffer);
        s->buffer->Copy(s->name->Des());
    }
}

TRuint stiStreamWrite(TRStream* stream, const TRbyte* data, TRuint length)
{
    StiStream* s = (StiStream*)stream->handle;
    TRuint written = 0;
    
    while (length--)
    {
        /* Flush the buffer when we see an end of line or when the buffer is filled */
        if (*data == '\n' && !s->hexEncoding)
        {
            data++;
            flushBuffer(stream);
            continue;
        }
        else if (s->buffer->Length() >= s->buffer->MaxLength() - (s->hexEncoding ? 2 : 0))
        {
            flushBuffer(stream);
        }
        
        if (s->hexEncoding)
        {
            TRubyte d = (TRubyte)*data;
            if (d < 0x10)
            {
                s->buffer->Append('0');
            }
            s->buffer->AppendNum(d, EHex);
        }
        else
        {
            s->buffer->Append(*data);
        }
        
        written++;
        data++;
    }
    return written;
}

/* Stream operations */
static const TRStreamOperations stiStreamOps = 
{
    NULL, /* write */
    stiStreamWrite,
    NULL  /* flush */
};

static TRint getIntegerOption(const TRStrDict* options, const TRbyte* key, TRint dfl)
{
    const TRbyte* value = options ? (const TRbyte*)triStrDictGet(options, key) : (const TRbyte*)0;
    return value ? atoi(value) : dfl;
}

TRenum stiOpenStream(TRStreamDevice* dev, TRStream* stream, const TRbyte* name, const TRbyte* mode, const TRStrDict* options)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    TR_UNUSED(name);
    TR_UNUSED(mode);
    {
        TRState*  state = triGetState();
        StiStream*    s = (StiStream*)triMalloc(state, sizeof(StiStream));
        
        TR_ASSERT_RETURN(s, TR_OUT_OF_MEMORY);
        
        s->state        = state;
        s->bufferData   = HBufC16::NewL(getIntegerOption(options, "buffer_size", 256));
        s->name         = HBufC16::NewL(triStringLength(name) + 10);
        s->hexEncoding  = getIntegerOption(options, "hex_encoding", 1);
        
        if (!s->bufferData || !s->name)
        {
            delete s->bufferData;
            delete s->name;
            triFree(state, s);
            return TR_OUT_OF_MEMORY;
        }
        s->buffer       = new TPtr16(s->bufferData->Des());
        
        /* Prepare the stream identifier */
        TPtr16 n = s->name->Des();
        n.Append(_L("[TRACY:"));
        while (*name)
        {
            n.Append(*name++);
        }
        n.Append(_L("] "));
        
        stream->ops     = &stiStreamOps;
        stream->handle  = s;
        
        s->buffer->Copy(s->name->Des());
        return TR_NO_ERROR;
    }
}

TRenum stiCloseStream(TRStreamDevice* dev, TRStream* stream)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    
    if (stream->handle)
    {
        StiStream* s = (StiStream*)stream->handle;
        flushBuffer(stream);
        delete s->buffer;
        delete s->bufferData;
        delete s->name;
        triFree(s->state, s);
        stream->handle = 0;
        return TR_NO_ERROR;
    }
    return TR_INVALID_HANDLE;
}

/* Device operations */
static const TRStreamDeviceOperations stiStreamDeviceOps = 
{
    NULL, /* init */
    NULL, /* exit */
    stiOpenStream,
    stiCloseStream
};

/* Device factory */
TRenum stiCreateDevice(TRState* state, TRStreamDevice* dev)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(dev, TR_INVALID_HANDLE);
    
    dev->handle     = (TRhandle)0xdadacafe;
    dev->ops        = &stiStreamDeviceOps;
    return TR_NO_ERROR;
}

} /* extern "C" */
