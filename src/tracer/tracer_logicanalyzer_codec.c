/**
 *  Logic Analyzer codec
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

TRenum logicBeginEvent(TRStreamEncoder* encoder, const TREvent* event)
{
    TRuint t = (TRuint)(event->timestamp / 1000);
    print(encoder->stream, "%d ms sm_library%d;%s\n", t, event->api, event->function->name);
    print(encoder->stream, "%d ms e_%s 1;", t, event->function->name);
    return TR_NO_ERROR;
}

TRenum logicEndEvent(TRStreamEncoder* encoder, const TREvent* event)
{
    TRuint t = (TRuint)((event->timestamp + event->callTime) / 1000);
    print(encoder->stream, "\n%d ms e_%s 0\n", t, event->function->name);
    print(encoder->stream, "%d ms sm_library%d;idle\n", t, event->api, event->function->name);
    return TR_NO_ERROR;
}

/**
 *  A macro for generating a value logger
 */
#define VALUE_LOGGER(NAME, TYPE, FORMAT) \
TRenum NAME(TRStreamEncoder* encoder, const TRbyte* name, TYPE value) \
{                                                                     \
    if (name)                                                         \
      print(encoder->stream, "%s=" FORMAT " ", name, value);          \
    else                                                              \
      print(encoder->stream, "-> " FORMAT, value);                    \
    return TR_NO_ERROR;                                               \
}

/* Generate loggers for all types */
VALUE_LOGGER(logicByteValue, TRubyte,         "%02x")
VALUE_LOGGER(logicShortValue, TRshort,        "%d")
VALUE_LOGGER(logicIntegerValue, TRint,        "%d")
VALUE_LOGGER(logicLongValue, TRlong,          "%l")
VALUE_LOGGER(logicFloatValue, TRfloat,        "%f")
VALUE_LOGGER(logicDoubleValue, TRdouble,      "%f")
VALUE_LOGGER(logicPointerValue, const void*,  "%p")

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

TRenum logicDefinePointerData(TRStreamEncoder* encoder, const void* pointer, TRuint size, const TRubyte* data, TRenum type, const TRClass* cls, TRhandle ns)
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
        LOGGER_LOOP(TRint, "%p");
        break;
    }
    print(encoder->stream, "] ");
    return TR_NO_ERROR;
}

TRenum logicObjectValue(TRStreamEncoder* encoder, const TRbyte* name, const TRObject* object)
{
    print(encoder->stream, "%s=<%s instance at %p:%p> ", name, object->cls->name, object->ns, object->handle);
    return TR_NO_ERROR;
}

TRenum logicBeginObject(TRStreamEncoder* encoder, const TRClass* cls, TRhandle ns, TRhandle handle)
{
    print(encoder->stream, "[ <%s instance at %p:%p>: ", cls->name, ns, handle);
    return TR_NO_ERROR;
}

TRenum logicEndObject(TRStreamEncoder* encoder)
{
    print(encoder->stream, "] ");
    return TR_NO_ERROR;
}


/* Encoder operations */
static const TRStreamEncoderOperations logicEncoderOps =
{
    logicBeginEvent,
    logicEndEvent,
    logicByteValue,
    logicShortValue,
    logicIntegerValue,
    logicLongValue,
    logicFloatValue,
    logicDoubleValue,
    logicPointerValue,
    logicObjectValue,
    logicDefinePointerData,
    logicBeginObject,
    logicEndObject,
};

TRenum logicAttachEncoder(TRStreamCodec* codec, TRStreamEncoder* encoder, const TRStrDict* options)
{
    TR_UNUSED(codec);
    TR_UNUSED(options);
    
    /* Initialize state */
    encoder->handle = (TRhandle)0x10616;
    encoder->ops    = &logicEncoderOps;
    return TR_NO_ERROR;
}

TRenum logicDetachEncoder(TRStreamCodec* codec, TRStreamEncoder* encoder)
{
    TR_UNUSED(codec);
    
    encoder->handle = 0;
    encoder->ops    = 0;
    return TR_NO_ERROR;
}

/* Codec operations */
static const TRStreamCodecOperations logicCodecOps =
{
    NULL, /* init */
    NULL, /* exit */
    logicAttachEncoder,
    logicDetachEncoder,
    NULL, /* attachDecoder */
    NULL, /* detachDecoder */
};

/* Codec factory */
TRenum logicCreateCodec(TRState* state, TRStreamCodec* codec)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(codec, TR_INVALID_HANDLE);
    
    codec->handle     = (TRhandle)0x10616;
    codec->ops        = &logicCodecOps;
    return TR_NO_ERROR;
}
