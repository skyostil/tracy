/**
 *  Binary log stream codec
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
#include "tracer_util.h"
#include "tracer_platform.h"
#include "tracer_config.h"
#include <stdio.h>
#include <stdarg.h>

#define FORMAT_VERSION     2

/**
 *  Stream tokens
 */
#define TOK_DEFINE         0x1
#define TOK_EVENT          0x2
#define TOK_VALUE          0x3
#define TOK_BEGIN_OBJECT   0x4
#define TOK_API            0x80
#define TOK_DURATION       0x81
#define TOK_END_OBJECT     0x82

#define TOK_TINY_TOKEN_BIT 0x80

/**
 *  Type flags
 */
#define TR_PHRASE_BIT     0x80

typedef struct
{
    TRbyte*     data;
    TRuint      size;
} Phrase;

/**
 *  Binary encoder state
 */
typedef struct 
{
    TRState*    state;
    TRIntDict*  phrasebook;
    TRStrDict*  stringPool;
    TRuint      stringPoolId;
    TRenum      currentApi;
} BinaryEncoder;

/**
 *  Binary decoder state
 */
typedef struct 
{
    TRState*    state;
    TRIntDict*  phrasebook;
    TREvent*    event;
} BinaryDecoder;

/**
 *  A macro for generating an basic type writer.
 */
#define WRITER(NAME, TYPE)                              \
static void NAME(TRStreamEncoder* encoder, TYPE value)  \
{                                                       \
    encoder->stream->ops->write(encoder->stream,        \
                                (const TRbyte*)&value,  \
                                sizeof(value));         \
}                                                       \

/**
 *  A macro for generating an basic type reader.
 */
#define READER(NAME, TYPE)                              \
static TYPE NAME(TRStreamDecoder* decoder)              \
{                                                       \
    TYPE value;                                         \
    decoder->stream->ops->read(decoder->stream,         \
                               (TRbyte*)&value,         \
                               sizeof(value));          \
    return value;                                       \
}

WRITER(writeByte,    TRbyte)
WRITER(writeShort,   TRshort)
WRITER(writeInt,     TRint)
WRITER(writeLong,    TRlong)
WRITER(writeFloat,   TRfloat)
WRITER(writeDouble,  TRdouble)
WRITER(writePointer, const void*)

READER(readByte,     TRubyte)
READER(readShort,    TRshort)
READER(readInt,      TRint)
READER(readLong,     TRlong)
READER(readFloat,    TRfloat)
READER(readDouble,   TRdouble)
READER(readPointer,  void*)

static TRbool freeStringPoolEntry(TRDict* dict, TRDictNode* node, void* arg)
{
    BinaryEncoder* enc = (BinaryEncoder*)arg;
    TRuint          id = (TRuint)node->value;
    Phrase*     phrase = (Phrase*)triIntDictGet(enc->phrasebook, id);
    TR_UNUSED(dict);
    
    if (phrase)
    {
        /* Delete this entry and stop the iteration */
        triFree(enc->state, phrase->data);
        triStrDictRemove(enc->stringPool, node->key);
        TR_ASSERT(triIntDictRemove(enc->phrasebook, id));
        return TR_FALSE;
    }
    return TR_TRUE;
}

TRuint defineString(TRStreamEncoder* encoder, const TRbyte* string)
{
    if (!string)
    {
        /* Phrase zero is reserved for NULL pointers */
        return 0;
    }
    else
    {
        BinaryEncoder* enc = (BinaryEncoder*)encoder->handle;
        TRuint          id = (TRuint)triStrDictGet(enc->stringPool, string);
        
        if (!id)
        {
            TRuint length  = triStringLength(string) + 1;
            Phrase* phrase = triMalloc(enc->state, sizeof(Phrase));

            if (!phrase)
            {
                TRuint i;
                
                /* No strings to deallocate? */
                if (triDictSize(enc->stringPool) < 4)
                {
                    TR_ASSERT(phrase);
                }
              
                /* Free a quarter of the allocated strings */
                for (i = 0; i < triDictSize(enc->stringPool) / 4; i++)
                {
                    triDictVisit(enc->stringPool, freeStringPoolEntry, enc);
                }
                
                /* Try the allocation again */
                return defineString(encoder, string);
            }
            
            /* Find a free slot in the phrasebook for the new string */
            while (triIntDictGet(enc->phrasebook, enc->stringPoolId) || enc->stringPoolId == 0)
            {
                enc->stringPoolId++;
            }
            
            /* Add the string to the phrasebook */
            id = enc->stringPoolId;
            phrase->data = triStringDuplicate(enc->state, string);
            phrase->size = length;
            triIntDictSet(enc->phrasebook, id, phrase);
            triStrDictSet(enc->stringPool, string, (void*)id);
            
            /* Serialize the string to the stream */
            writeByte(encoder, TOK_DEFINE);
            writeInt(encoder, 2 * sizeof(TRint) + sizeof(TRbyte) + length * sizeof(TRbyte));
            writeInt(encoder, id);
            writeInt(encoder, length);
            writeByte(encoder, TR_VOID);
            encoder->stream->ops->write(encoder->stream, string, length);
        }
        return id;
    }
}

static void* lookupPhrase(TRStreamDecoder* decoder, TRint id)
{
    BinaryDecoder* dec = (BinaryDecoder*)decoder->handle;
    Phrase* phrase;
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
    /*triLogMessage(dec->state, "[binary] Phrase %p = %p", id, triIntDictGet(dec->phrasebook, id));*/
#endif
    phrase = (Phrase*)triIntDictGet(dec->phrasebook, id);
    return phrase ? phrase->data : 0;
}

static TRenum readToken(TRStreamDecoder* decoder, TRint expectedToken, TRVariant* value)
{
    BinaryDecoder* dec = (BinaryDecoder*)decoder->handle;
    TRubyte token;
    TRint   param;
    
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
    /*triLogMessage(dec->state, "[binary] Looking for token 0x%x.", expectedToken);*/
#endif

    do
    {
        if (decoder->stream->ops->read(decoder->stream, (TRbyte*)&token, sizeof(TRubyte)) != 1)
        {
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
            triLogMessage(dec->state, "[binary] End of stream encountered.");
#endif                
            return TR_NOT_FOUND;
        }
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
        /*triLogMessage(dec->state, "[binary] Reading token 0x%x.", token);*/
#endif                
        param = readInt(decoder);
        
        switch (token)
        {
        case TOK_DEFINE:
            {
                TRint id       = readInt(decoder);
                TRint size     = readInt(decoder);
                TRbyte type    = readByte(decoder);
                TRuint length  = param - (2 * sizeof(TRint) + sizeof(TRbyte));
                Phrase* phrase;
                TRhandle objectNs = 0;
                const TRClass* objectClass = 0;

                /* If this is an array of objects, read the class and namespace information */
                if (type == TR_OBJECT)
                {
                    const TRbyte* className = (const TRbyte*)lookupPhrase(decoder, readInt(decoder));
                    objectNs = readPointer(decoder);
                    objectClass = triGetClassByName(dec->state, className);
                    length -= sizeof(TRint) + sizeof(TRhandle);
                    TR_ASSERT(objectClass);
                }
                
                TR_ASSERT(size >= length);
                TR_UNUSED(type);

#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                triLogMessage(dec->state, "[binary] DEFINE %p = %d bytes (%d bytes defined)", id, size, length);
#endif                
                
                /* Replace any previous phrase under this id */
                phrase = triIntDictGet(dec->phrasebook, id);
                
                /* Clear the existing entry if this is a zero-length phrase */
                if (phrase && phrase->data && size == 0)
                {
                    triFree(dec->state, phrase->data);
                    triFree(dec->state, phrase);
                    triIntDictSet(dec->phrasebook, id, 0);
                    /* No need to read the rest of the data since there is none */
                    continue;
                }
                
                if (!phrase)
                {
                    /* Allocate the new phrase */
                    phrase       = triMalloc(dec->state, sizeof(Phrase));
                    TR_ASSERT(phrase);
                    phrase->data = triMalloc(dec->state, size);
                    phrase->size = size;
                    
                    /* Terminate the data with zeros if needed */
                    triMemSet(&phrase->data[length], 0, size - length);
                }
                else
                {
                    /* Make sure the allocated phrase is big enough */
                    TR_ASSERT(phrase->size >= size);
                }
                
                if (!phrase->data)
                {
                    return TR_OUT_OF_MEMORY;
                }
                
                /* Read the phrase contents */
                if (decoder->stream->ops->read(decoder->stream, phrase->data, length) != length)
                {
                    return TR_INVALID_OPERATION;
                }

                /* For object arrays, translate the references to live objects */
                if (type == TR_OBJECT)
                {
                    int i;
                    TRhandle* array = (TRhandle*)phrase->data;

                    for (i = 0; i < length / sizeof(TRhandle); i++)
                    {
                        array[i] = triGetObject(dec->state, objectClass->id, objectNs, array[i], array[i]);
                    }
                }
                
                /* Store it in the phrasebook */
                triIntDictSet(dec->phrasebook, id, phrase);
            }
        break;
        case TOK_EVENT:
            {
                /* TODO: dictionary lookup */
                const TRbyte* name         = (const TRbyte*)lookupPhrase(decoder, readInt(decoder));
                dec->event->function       = triLookupFunction(dec->state, dec->event->api, name);
                dec->event->sequenceNumber = readInt(decoder);
                dec->event->timestamp      = readInt(decoder);
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                triLogMessage(dec->state, "[binary] EVENT %s (%p), seq = %d, ts = %d", name, dec->event->function,
                              dec->event->sequenceNumber, (TRint)dec->event->timestamp);
#endif
            }
        break;
        case TOK_VALUE:
            {
                TRint nameId   = readInt(decoder);
                TRbyte type    = readByte(decoder);

                TR_UNUSED(nameId);
                
                if (type & TR_PHRASE_BIT)
                {
                    value->p = lookupPhrase(decoder, readInt(decoder));
                }
                else
                {
                    switch (type)
                    {
                    case TR_BYTE:
                        value->b = readByte(decoder);
                    break;
                    case TR_SHORT:
                        value->s = readShort(decoder);
                    break;
                    case TR_INTEGER:
                        value->i = readInt(decoder);
                    break;
                    case TR_LONG:
                        value->l = readLong(decoder);
                    break;
                    case TR_FLOAT:
                        value->f = readFloat(decoder);
                    break;
                    case TR_DOUBLE:
                        value->d = readDouble(decoder);
                    break;
                    case TR_OBJECT:
                    {
                        const TRbyte* className = (const TRbyte*)lookupPhrase(decoder, readInt(decoder));
                        const TRhandle       ns = readPointer(decoder);
                        const TRhandle   handle = readPointer(decoder);
                        const TRClass*        c = triGetClassByName(dec->state, className);

                        if (!c)
                        {
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                            triLogMessage(dec->state, "[binary] Bad class name: %s", className);
#endif
                        }
                        else
                        {
                            /* Translate the reference to a live pointer */
                            value->o.cls    = c;
                            value->o.ns     = ns;
                            value->o.handle = triGetObject(dec->state, c->id, ns, handle, handle);
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                            triLogMessage(dec->state, "[binary] %s instance at %p mapped to %p", c->name, handle, value->o.handle);
#endif
                        }
                    }
                    break;
                    default:
                        TR_ASSERT(!"Corrupted binary stream: Bad value type.");
                    break;
                    }
                }
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                {
                    const TRbyte* name = (const TRbyte*)lookupPhrase(decoder, nameId);
                    triLogMessage(dec->state, "[binary] VALUE %s = %d (%f, %p)", name, value->i, value->f, value->p);
                }
#endif
            }
        break;
        case TOK_API:
            dec->event->api = param;
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
            triLogMessage(dec->state, "[binary] API %d", dec->event->api);
#endif
        break;
        case TOK_DURATION:
            dec->event->callTime = param;
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
            triLogMessage(dec->state, "[binary] DURATION %d", (TRint)dec->event->callTime);
#endif
        break;
        case TOK_BEGIN_OBJECT:
            {
                const TRbyte*    className = (const TRbyte*)lookupPhrase(decoder, readInt(decoder));
                TRhandle                ns = readPointer(decoder);
                TRhandle            handle = readPointer(decoder);
                const TRClass* objectClass = triGetClassByName(dec->state, className);

#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                triLogMessage(dec->state, "[binary] BEGIN_OBJECT %s", className);
#endif

                if (!objectClass)
                {
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
                    triLogMessage(dec->state, "[binary] Bad class name: %s", className);
#endif
                }
                else if (handle)
                {
                    triUnserializeObject(dec->state, objectClass->id, ns, handle);
                }
                /* Skip to the end of the object data */
                readToken(decoder, TOK_END_OBJECT, value);
            }
            break;
        case TOK_END_OBJECT:
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
            triLogMessage(dec->state, "[binary] END_OBJECT");
#endif
            break;
        default:
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
            triLogMessage(dec->state, "[binary] Bad token: 0x%x", token);
#endif
            /* Skip it */
            while (!(token & TOK_TINY_TOKEN_BIT) && param--)
            {
                readByte(decoder);
            }
        break;
        }
    } while (token != expectedToken);

#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
    /*triLogMessage(dec->state, "[binary] Found our token.");*/
#endif
    return TR_NO_ERROR;
}

TRenum binaryBeginEvent(TRStreamEncoder* encoder, const TREvent* event)
{
    BinaryEncoder* enc = (BinaryEncoder*)encoder->handle;
    TRuint      nameId = defineString(encoder, event->function->name);
    
    if (event->api != enc->currentApi)
    {
        writeByte(encoder, TOK_API);
        writeInt(encoder, event->api);
        enc->currentApi = event->api;
    }
    
    writeByte(encoder, TOK_EVENT);
    writeInt(encoder, 3 * sizeof(TRint));
    writeInt(encoder, nameId);
    writeInt(encoder, event->sequenceNumber);
    writeInt(encoder, (TRint)event->timestamp);
    
    return TR_NO_ERROR;
}

TRenum binaryEndEvent(TRStreamEncoder* encoder, const TREvent* event)
{
    writeByte(encoder, TOK_DURATION);
    writeInt(encoder, event->callTime);
    return TR_NO_ERROR;
}

/**
 *  A macro for generating a value logger
 */
#define VALUE_LOGGER(NAME, TYPE, TYPE_ID, TYPE_WRITER)                \
TRenum NAME(TRStreamEncoder* encoder, const TRbyte* name, TYPE value) \
{                                                                     \
    TRuint nameId = defineString(encoder, name);                      \
    writeByte(encoder, TOK_VALUE);                                    \
    writeInt(encoder, sizeof(TRint) + sizeof(TRbyte) + sizeof(TYPE)); \
    writeInt(encoder, nameId);                                        \
    writeByte(encoder, TYPE_ID);                                      \
    TYPE_WRITER(encoder, value);                                      \
    return TR_NO_ERROR;                                               \
}

/**
 *  A macro for generating a value loader
 */
#define VALUE_LOADER(NAME, TYPE, VARIANT_MEMBER)                       \
TRenum NAME(TRStreamDecoder* decoder, const TRbyte* name, TYPE* value) \
{                                                                      \
    TRVariant variant;                                                 \
    TRenum ret = readToken(decoder, TOK_VALUE, &variant);              \
    TR_UNUSED(name);                                                   \
    if (ret == TR_NO_ERROR)                                            \
    {                                                                  \
        *value = variant.VARIANT_MEMBER;                               \
    }                                                                  \
    return ret;                                                        \
}

/* Generate loggers for all types */
VALUE_LOGGER(binaryByteValue,    TRubyte,        TR_BYTE,       writeByte)
VALUE_LOGGER(binaryShortValue,   TRshort,        TR_SHORT,      writeShort)
VALUE_LOGGER(binaryIntegerValue, TRint,          TR_INTEGER,    writeInt)
VALUE_LOGGER(binaryLongValue,    TRlong,         TR_LONG,       writeLong)
VALUE_LOGGER(binaryFloatValue,   TRfloat,        TR_FLOAT,      writeFloat)
VALUE_LOGGER(binaryDoubleValue,  TRdouble,       TR_DOUBLE,     writeDouble)
VALUE_LOGGER(binaryPointerValue, const void*,    TR_VOID | TR_PHRASE_BIT, writePointer)

/* Generate loaders for all types */
VALUE_LOADER(binaryReadByteValue,    TRubyte,  b)
VALUE_LOADER(binaryReadShortValue,   TRshort,  s)
VALUE_LOADER(binaryReadIntegerValue, TRint,    i)
VALUE_LOADER(binaryReadLongValue,    TRlong,   l)
VALUE_LOADER(binaryReadFloatValue,   TRfloat,  f)
VALUE_LOADER(binaryReadDoubleValue,  TRdouble, d)
VALUE_LOADER(binaryReadPointerValue, TRhandle, p)
VALUE_LOADER(binaryReadObjectValue,  TRObject, o)

TRenum binaryObjectValue(TRStreamEncoder* encoder, const TRbyte* name, const TRObject* object)
{
    TRuint nameId      = defineString(encoder, name);
    TRuint classNameId = defineString(encoder, object->cls->name);
    writeByte(encoder, TOK_VALUE);
    writeInt(encoder, sizeof(TRint) + sizeof(TRbyte) + sizeof(TRint) + sizeof(TRhandle) + sizeof(TRhandle));
    writeInt(encoder, nameId);
    writeByte(encoder, TR_OBJECT);
    writeInt(encoder, classNameId);
    writePointer(encoder, object->ns);
    writePointer(encoder, object->handle);
    return TR_NO_ERROR;
}

TRenum binaryDefinePointerData(TRStreamEncoder* encoder, const void* pointer, TRuint size, 
                               const TRubyte* data, TRenum type, const TRClass* cls, TRhandle ns)
{
    BinaryEncoder* enc = (BinaryEncoder*)encoder->handle;
    int tokenSize = 2 * sizeof(TRint) + sizeof(TRbyte) + size * sizeof(TRbyte);
    
    if (data && size)
    {
        /* Mark the phrase as taken */
        triIntDictSet(enc->phrasebook, (TRint)pointer, 0);
    }
    else
    {
        /* Free the phrase */
        triIntDictRemove(enc->phrasebook, (TRint)pointer);
    }

    if (type == TR_OBJECT)
    {
        tokenSize += sizeof(TRint) + sizeof(TRhandle);
    }

    writeByte(encoder, TOK_DEFINE);
    writeInt(encoder, tokenSize);
    writeInt(encoder, (TRint)pointer);
    writeInt(encoder, size);
    writeByte(encoder, type & 0xff);

    /* Object arrays need additional class and namespace information */
    if (type == TR_OBJECT)
    {
        TRuint classId = defineString(encoder, cls->name);
        writeInt(encoder, classId);
        writePointer(encoder, ns);
    }
    encoder->stream->ops->write(encoder->stream, (const TRbyte*)data, size);
    return TR_NO_ERROR;
}

TRenum binaryGetEvent(TRStreamDecoder* decoder, TREvent* event)
{
    BinaryDecoder* dec = (BinaryDecoder*)decoder->handle;
    TRVariant value;
    
    dec->event = event;
    
    return readToken(decoder, TOK_EVENT, &value);
}

TRenum binaryBeginObject(TRStreamEncoder* encoder, const TRClass* cls, TRhandle ns, TRhandle handle)
{
    TRuint      classId = defineString(encoder, cls->name);
    
    writeByte(encoder, TOK_BEGIN_OBJECT);
    writeInt(encoder, sizeof(TRint) + sizeof(TRhandle) + sizeof(TRhandle));
    writeInt(encoder, classId);
    writePointer(encoder, ns);
    writePointer(encoder, handle);
    
    return TR_NO_ERROR;
}

TRenum binaryEndObject(TRStreamEncoder* encoder)
{
    writeByte(encoder, TOK_END_OBJECT);
    writeInt(encoder, 0);
    
    return TR_NO_ERROR;    
}

/* Encoder operations */
static const TRStreamEncoderOperations binaryEncoderOps =
{
    binaryBeginEvent,
    binaryEndEvent,
    binaryByteValue,
    binaryShortValue,
    binaryIntegerValue,
    binaryLongValue,
    binaryFloatValue,
    binaryDoubleValue,
    binaryPointerValue,
    binaryObjectValue,
    binaryDefinePointerData,
    binaryBeginObject,
    binaryEndObject,
};

/* Decoder operations */
static const TRStreamDecoderOperations binaryDecoderOps =
{
    binaryGetEvent,
    binaryReadByteValue,
    binaryReadShortValue,
    binaryReadIntegerValue,
    binaryReadLongValue,
    binaryReadFloatValue,
    binaryReadDoubleValue,
    binaryReadPointerValue,
    binaryReadObjectValue,
};

TRenum binaryAttachEncoder(TRStreamCodec* codec, TRStreamEncoder* encoder, const TRStrDict* options)
{
    BinaryEncoder* enc = triMalloc(triGetState(), sizeof(BinaryEncoder));
    TR_UNUSED(codec);
    TR_UNUSED(options);
    
    TR_ASSERT_RETURN(enc, TR_OUT_OF_MEMORY);
    enc->state             = triGetState();
    enc->phrasebook        = triCreateIntDict(enc->state);
    enc->stringPool        = triCreateStrDict(enc->state);
    enc->stringPoolId      = 1;
    enc->currentApi        = 0;
    TR_ASSERT_RETURN(enc->phrasebook, TR_OUT_OF_MEMORY);
    TR_ASSERT_RETURN(enc->stringPool, TR_OUT_OF_MEMORY);
    
    /* Initialize state */
    encoder->handle = (TRhandle)enc;
    encoder->ops    = &binaryEncoderOps;

    /* Write version magic */
    writeByte(encoder, 'T');
    writeByte(encoder, 'C');
    writeByte(encoder, 'Y');
    writeByte(encoder, '0' + FORMAT_VERSION);

    return TR_NO_ERROR;
}

TRenum binaryAttachDecoder(TRStreamCodec* codec, TRStreamDecoder* decoder, const TRStrDict* options)
{
    BinaryDecoder* dec = triMalloc(triGetState(), sizeof(BinaryDecoder));
    TR_UNUSED(codec);
    TR_UNUSED(options);
    
    TR_ASSERT_RETURN(dec, TR_OUT_OF_MEMORY);
    dec->state             = triGetState();
    dec->phrasebook        = triCreateIntDict(dec->state);
    dec->event             = 0;
    TR_ASSERT_RETURN(dec->phrasebook, TR_OUT_OF_MEMORY);
    
    /* Initialize state */
    decoder->handle = (TRhandle)dec;
    decoder->ops    = &binaryDecoderOps;

    /* Read version magic */
    if (readByte(decoder) != 'T') return TR_BAD_VALUE;
    if (readByte(decoder) != 'C') return TR_BAD_VALUE;
    if (readByte(decoder) != 'Y') return TR_BAD_VALUE;
    if (readByte(decoder) != '0' + FORMAT_VERSION) return TR_BAD_VALUE;

    return TR_NO_ERROR;
}

TRenum binaryDetachEncoder(TRStreamCodec* codec, TRStreamEncoder* encoder)
{
    TR_UNUSED(codec);
    
    if (encoder->handle)
    {
        BinaryEncoder* enc = (BinaryEncoder*)encoder->handle;
        {
            TRArray* values = triDictGetValues(enc->phrasebook);

            TR_FOR_EACH(Phrase*, p, values)
            {
                if (p)
                {
                    triFree(enc->state, p->data);
                    triFree(enc->state, p);
                }
            }
            triDestroyArray(values);
            triDestroyDict(enc->phrasebook);
        }
        triDestroyDict(enc->stringPool);
        triFree(triGetState(), enc);
    }
    
    encoder->handle = 0;
    encoder->ops    = 0;
    return TR_NO_ERROR;
}

TRenum binaryDetachDecoder(TRStreamCodec* codec, TRStreamDecoder* decoder)
{
    TR_UNUSED(codec);
    
    if (decoder->handle)
    {
        BinaryDecoder* dec = (BinaryDecoder*)decoder->handle;
        {
            TRArray* values = triDictGetValues(dec->phrasebook);

            TR_FOR_EACH(Phrase*, p, values)
            {
                if (p)
                {
                    triFree(dec->state, p->data);
                    triFree(dec->state, p);
                }
            }
            triDestroyArray(values);
            triDestroyDict(dec->phrasebook);
        }
        triFree(triGetState(), dec);
    }
    
    decoder->handle = 0;
    decoder->ops    = 0;
    return TR_NO_ERROR;
}


/* Codec operations */
static const TRStreamCodecOperations binaryCodecOps =
{
    NULL, /* init */
    NULL, /* exit */
    binaryAttachEncoder,
    binaryDetachEncoder,
    binaryAttachDecoder,
    binaryDetachDecoder,
};

/* Codec factory */
TRenum binaryCreateCodec(TRState* state, TRStreamCodec* codec)
{
    TR_ASSERT_RETURN(state, TR_INVALID_HANDLE);
    TR_ASSERT_RETURN(codec, TR_INVALID_HANDLE);
    TR_ASSERT(sizeof(TRint) >= sizeof(void*));
    
    codec->handle     = (TRhandle)0xc001babe;
    codec->ops        = &binaryCodecOps;
    return TR_NO_ERROR;
}
