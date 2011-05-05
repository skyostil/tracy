/**
 *  ASCII log stream codec
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
#include <stdio.h>
#include <stdarg.h>

/**
 *  ASCII encoder state
 */
static void print(TRStream* stream, const char* format, ...)
{
    char str[256];
    int chars = 0;

    va_list args;
    va_start(args, format);
    chars = vsprintf(str, format, args);
    va_end(args);
    
    stream->ops->write(stream, str, chars);
}

TRenum asciiBeginEvent(TRStreamEncoder* encoder, const TREvent* event)
{
    print(encoder->stream, "%010d %010d %s ", event->sequenceNumber, (TRuint)(event->timestamp / 1000), event->function->name);
    return TR_NO_ERROR;
}

TRenum asciiEndEvent(TRStreamEncoder* encoder, const TREvent* event)
{
    print(encoder->stream, " +%d\n", (TRuint)event->callTime);
    return TR_NO_ERROR;
}

/**
 *  A macro for generating a value logger
 */
#define VALUE_LOGGER(NAME, TYPE, FORMAT) \
TRenum NAME(TRStreamEncoder* encoder, const TRbyte* name, TYPE value) \
{                                                                     \
    if (name)                                                         \
        print(encoder->stream, "%s=" FORMAT " ", name, value);        \
    else                                                              \
        print(encoder->stream, "-> " FORMAT, value);                  \
    return TR_NO_ERROR;                                               \
}

/* Generate loggers for all types */
VALUE_LOGGER(asciiByteValue, TRubyte,         "%02x")
VALUE_LOGGER(asciiShortValue, TRshort,        "%d")
VALUE_LOGGER(asciiIntegerValue, TRint,        "%d")
VALUE_LOGGER(asciiLongValue, TRlong,          "%l")
VALUE_LOGGER(asciiFloatValue, TRfloat,        "%f")
VALUE_LOGGER(asciiDoubleValue, TRdouble,      "%f")
VALUE_LOGGER(asciiPointerValue, const void*,  "%p")

#define LOGGER_LOOP(TYPE, FORMAT)                 \
{                                                 \
    const TYPE* d = (const TYPE*)data;            \
    TRuint s = size / sizeof(TYPE);               \
    for (i = 0; i < s; i++)                       \
    {                                             \
        print(encoder->stream, FORMAT, d[i]);     \
        if (i != s - 1)                           \
          print(encoder->stream, ", ");           \
    }                                             \
}

TRenum asciiDefinePointerData(TRStreamEncoder* encoder, const void* pointer, TRuint size, const TRubyte* data, 
                              TRenum type, const TRClass* cls, TRhandle ns)
{
    TRuint i;
    TR_UNUSED(cls);
    TR_UNUSED(ns);

    print(encoder->stream, "[%p = ", pointer);
    
    switch (type)
    {
    default:
    case TR_BYTE:
        LOGGER_LOOP(TRubyte, "%02x");
        break;
    case TR_SHORT:
        LOGGER_LOOP(TRshort, "%d");
        break;
    case TR_INTEGER:
        LOGGER_LOOP(TRint, "%d");
        break;
    case TR_LONG:
        LOGGER_LOOP(TRlong, "%l");
        break;
    case TR_FLOAT:
        LOGGER_LOOP(TRfloat, "%f");
        break;
    case TR_DOUBLE:
        LOGGER_LOOP(TRdouble, "%f");
        break;
    case TR_OBJECT:
        LOGGER_LOOP(TRhandle, "%p");
        break;
    }
    print(encoder->stream, "] ");
    return TR_NO_ERROR;
}

TRenum asciiObjectValue(TRStreamEncoder* encoder, const TRbyte* name, const TRObject* object)
{
    print(encoder->stream, "%s=<%s instance at %p:%p> ", name, object->cls->name, object->ns, object->handle);
    return TR_NO_ERROR;
}

TRenum asciiBeginObject(TRStreamEncoder* encoder, const TRClass* cls, TRhandle ns, TRhandle handle)
{
    print(encoder->stream, "[ <%s instance at %p:%p>: ", cls->name, ns, handle);
    return TR_NO_ERROR;
}

TRenum asciiEndObject(TRStreamEncoder* encoder)
{
    print(encoder->stream, "] ");
    return TR_NO_ERROR;
}

/* Encoder operations */
static const TRStreamEncoderOperations asciiEncoderOps =
{
    asciiBeginEvent,
    asciiEndEvent,
    asciiByteValue,
    asciiShortValue,
    asciiIntegerValue,
    asciiLongValue,
    asciiFloatValue,
    asciiDoubleValue,
    asciiPointerValue,
    asciiObjectValue,
    asciiDefinePointerData,
    asciiBeginObject,
    asciiEndObject,
};

TRenum asciiAttachEncoder(TRStreamCodec* codec, TRStreamEncoder* encoder, const TRStrDict* options)
{
    TR_UNUSED(codec);
    TR_UNUSED(options);
    
    /* Initialize state */
    encoder->handle = (TRhandle)0xc001babe;
    encoder->ops    = &asciiEncoderOps;
    return TR_NO_ERROR;
}

TRenum asciiDetachEncoder(TRStreamCodec* codec, TRStreamEncoder* encoder)
{
    TR_UNUSED(codec);
    
    encoder->handle = 0;
    encoder->ops    = 0;
    return TR_NO_ERROR;
}

/* Codec operations */
static const TRStreamCodecOperations asciiCodecOps =
{
    NULL, /* init */
    NULL, /* exit */
    asciiAttachEncoder,
    asciiDetachEncoder,
    NULL, /* attachDecoder */
    NULL, /* detachDecoder */
};

/* Codec factory */
TRenum asciiCreateCodec(TRState* state, TRStreamCodec* codec)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(codec, TR_INVALID_HANDLE);
    
    codec->handle     = (TRhandle)0xc001babe;
    codec->ops        = &asciiCodecOps;
    return TR_NO_ERROR;
}
