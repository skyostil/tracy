/**
 *  ETM stream device
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

/**
 *  Stream state
 */
typedef struct 
{
    TRState*  state;
} EtmStream;

TRuint etmStreamWrite(TRStream* stream, const TRbyte* data, TRuint length)
{
    EtmStream* s = (EtmStream*)stream->handle;
    TRuint written = 0;
    TR_UNUSED(s);
    
    while (length--)
    {
        /* Insert ETM signaling magic here */

        written++;
        data++;
    }
    return written;
}

/* Stream operations */
static const TRStreamOperations etmStreamOps = 
{
    NULL, /* read */
    etmStreamWrite,
    NULL  /* flush */
};

TRenum etmOpenStream(TRStreamDevice* dev, TRStream* stream, const TRbyte* name, const TRbyte* mode, const TRStrDict* options)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    TR_UNUSED(name);
    TR_UNUSED(mode);
    {
        TRState*  state = triGetState();
        EtmStream*    s = (EtmStream*)triMalloc(state, sizeof(EtmStream));
        
        TR_ASSERT_RETURN(s, TR_OUT_OF_MEMORY);
        
        s->state        = state;
        stream->ops     = &etmStreamOps;
        stream->handle  = s;
        
        /* Note: the stream name could also be written to ETM here */
        
        return TR_NO_ERROR;
    }
}

TRenum etmCloseStream(TRStreamDevice* dev, TRStream* stream)
{
    TR_ASSERT_RETURN(dev,       TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(stream,    TR_INVALID_HANDLE);
    
    if (stream->handle)
    {
        EtmStream* s = (EtmStream*)stream->handle;
        triFree(s->state, s);
        stream->handle = 0;
        return TR_NO_ERROR;
    }
    return TR_INVALID_HANDLE;
}

/* Device operations */
static const TRStreamDeviceOperations etmStreamDeviceOps = 
{
    NULL, /* init */
    NULL, /* exit */
    etmOpenStream,
    etmCloseStream
};

/* Device factory */
TRenum etmCreateDevice(TRState* state, TRStreamDevice* dev)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(dev, TR_INVALID_HANDLE);
    
    dev->handle     = (TRhandle)0xfeedface;
    dev->ops        = &etmStreamDeviceOps;
    return TR_NO_ERROR;
}
