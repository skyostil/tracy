/**
 *  Generic Tracer internal implementation.
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

#include "tracer_internal.h"
#include "tracer_platform.h"
#include "tracer_util.h"
#include "tracer_config.h"
#include <stdio.h>              /* Config file reading */
#include <stdlib.h>

/* API state management functions */
TRStateTerminalValue* triCreateStateTerminalValue(TRState* state, void* data, TRbool allocated);
TRStateNode* triCreateStateNode(TRState* state, void* value);
TRStateGroup* triCreateStateGroup(TRState* state);
void triDestroyStateTerminalValue(TRState* state, TRStateTerminalValue* value);
void triDestroyStateGroup(TRState* state, TRStateGroup* root);
void triDestroyStateNode(TRState* state, TRStateNode* root);

/** A macro for initializing a new device */
#define TR_REGISTER_DEVICE(name, constructor)                              \
    {                                                                      \
        extern TRenum constructor(TRState*, TRStreamDevice*);              \
        TRStreamDevice* dev = triMalloc(state, sizeof(TRStreamDevice));    \
        if (constructor(state, dev) == TR_NO_ERROR)                        \
        {                                                                  \
            TR_ASSERT(triRegisterStreamDevice(state, name, dev));          \
        }                                                                  \
    }

/** A macro for initializing a new codec */
#define TR_REGISTER_CODEC(name, constructor)                               \
    {                                                                      \
        extern TRenum constructor(TRState*, TRStreamCodec*);               \
        TRStreamCodec* codec = triMalloc(state, sizeof(TRStreamCodec));    \
        if (constructor(state, codec) == TR_NO_ERROR)                      \
        {                                                                  \
            TR_ASSERT(triRegisterStreamCodec(state, name, codec));         \
        }                                                                  \
    }

static TRState* _triGetState(TRbool userInit)
{
    TRState* state = (TRState*)triGetGlobal();
    if (state)
    {
        return state;
    }
    
#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Initializing tracer.");
#endif

    state = triMalloc(TR_NO_STATE, sizeof(TRState));
    
    /* 
     * Perform a hard assert since no meaningful error management can be
     * done without the state structure.
     */
    TR_ASSERT(state);
    triMemSet(state, 0, sizeof(TRState));
    
    /*
     * Platform sanity checks
     */
    TR_ASSERT(sizeof(TRint) >= sizeof(void*));

    /* Boot up the platform */
    triPlatformInit(state);
    
    /* Initialize the state */
    state->error = TR_NO_ERROR;
    TR_ASSERT(state->streamDevices   = triCreateStrDict(state));
    TR_ASSERT(state->codecs          = triCreateStrDict(state));
    TR_ASSERT(state->encoders        = triCreateArray(state));
    TR_ASSERT(state->decoders        = triCreateArray(state));
    TR_ASSERT(state->apis            = triCreateArray(state));
    TR_ASSERT(state->streams         = triCreateArray(state));
    TR_ASSERT(state->pointers        = triCreateDict(state));
    TR_ASSERT(state->classes         = triCreateIntDict(state));
    TR_ASSERT(state->classesByName   = triCreateStrDict(state));
    state->fpsEstimate          = 0.0;
    state->pointerDataSizeLimit = ~0;
    
    /* Store it */
    triSetGlobal(state);
    
    /* Register configured codecs */
#if defined(TRACER_USE_ASCII_CODEC)
    TR_REGISTER_CODEC("ascii", asciiCreateCodec);
#endif

#if defined(TRACER_USE_BINARY_CODEC)
    TR_REGISTER_CODEC("binary", binaryCreateCodec);
#endif

#if defined(TRACER_USE_LOGICANALYZER_CODEC)
    TR_REGISTER_CODEC("logic", logicCreateCodec);
#endif

    /* Register configured stream devices */
#if defined(TRACER_USE_STDIO_DEVICE)
    TR_REGISTER_DEVICE("stdio", stdioCreateDevice);
#endif
    
#if defined(TRACER_USE_SYMBIAN_DEVICE)
    TR_REGISTER_DEVICE("symbian", symbianCreateDevice);
#endif

#if defined(TRACER_USE_STI_DEVICE)
    TR_REGISTER_DEVICE("sti", stiCreateDevice);
#endif

#if defined(TRACER_USE_ETM_DEVICE)
    TR_REGISTER_DEVICE("etm", etmCreateDevice);
#endif

    /* Call the user's initialization function if needed */
    if (userInit)
    {
        trInitialize();
    }
    
    /* Set the system base time at the very end */
    state->baseTime               = triGetSystemTime(state);
    state->fpsStartTime           = state->baseTime;
    state->measuredFrameStartTime = state->fpsStartTime;

#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Tracer initialized.");
#endif

    return state;
}

TRState* triGetState()
{
    return _triGetState(TR_TRUE);
}

TRState* triGetStateWithoutUserInit()
{
    return _triGetState(TR_FALSE);
}

void triDestroyState(TRState* state)
{
    TRuint i;

    if (!state)
    {
        return;
    }

#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Tracer terminated.");
#endif

    /* Destroy the encoders */
    while (triArraySize(state->encoders))
    {
        triDetachEncoder(state, (TRStreamEncoder*)triArrayGet(state->encoders, 0));
    }
    triDestroyArray(state->encoders);
    
    /* Destroy the decoders */
    while (triArraySize(state->decoders))
    {
        triDetachDecoder(state, (TRStreamDecoder*)triArrayGet(state->decoders, 0));
    }
    triDestroyArray(state->decoders);
    
    /* Destroy the APIs */
    for (i = 0; i < triArraySize(state->apis); i++)
    {
        TRApi* api = (TRApi*)triArrayGet(state->apis, i);
        TRArray* handles = triDictGetValues(api->libraryHandles);
        if (handles)
        {
            TR_FOR_EACH(TRhandle, handle, handles)
            {
                triCloseLibrary(handle);
            }
            triFree(state, handles);
        }
        triFree(state, api->functions);
        triDestroyStateGroup(state, api->state);
        triDestroyDict(api->functionsByName);
        triDestroyDict(api->libraryHandles);
        triFree(state, api);
    }
    triDestroyArray(state->apis);
    
    /* Destroy the streams */
    while (triArraySize(state->streams))
    {
        triCloseStream(state, (TRStream*)triArrayGet(state->streams, 0));
    }
    triDestroyArray(state->streams);

    /* Destroy all devices */
    {
        TRArray* values = triDictGetValues(state->streamDevices);
        if (values)
        {
            TR_FOR_EACH(TRStreamDevice*, dev, values)
            {
                 if (dev && dev->ops && dev->ops->exit)
                     dev->ops->exit(dev);
                 triFree(state, dev);
            }
            triDestroyArray(values);
        }
        triDestroyDict(state->streamDevices);
    }

    /* Destroy all codecs */
    {
        TRArray* values = triDictGetValues(state->codecs);
        if (values)
        {
            TR_FOR_EACH(TRStreamCodec*, codec, values)
            {
                if (codec && codec->ops && codec->ops->exit)
                    codec->ops->exit(codec);
                triFree(state, codec);
            }
        }
        triDestroyArray(values);
        triDestroyDict(state->codecs);
    }
    
    /* Destroy pointer data */
    {
        TRArray* values = triDictGetValues(state->pointers);
        if (values)
        {
            TR_FOR_EACH(TRPointerInfo*, info, values)
            {
                triFree(state, info->data);
                triFree(state, info);
            }
        }
        triDestroyArray(values);
        triDestroyDict(state->pointers);
    }
    
    /* Destroy config data */
    if (state->config)
    {
        TRArray* sections = triDictGetValues(state->config);
        if (sections)
        {
            TR_FOR_EACH(TRStrDict*, section, sections)
            {
                TRArray* values = triDictGetValues(section);
                if (values)
                {
                    TR_FOR_EACH(TRbyte*, value, values)
                    {
                        triFree(state, value);
                    }
                }
                triDestroyArray(values);
                triDestroyDict(section);
            }
        }
        triDestroyArray(sections);
        triDestroyDict(state->config);
    }
    
    /* Destroy the CRC table */
    triFree(state, state->crcTable);
    
    /* Destroy the classes and objects */
    {
        TRArray* classes = triDictGetValues(state->classes);
        if (classes)
        {
            TR_FOR_EACH(TRClass*, cls, classes)
            {
                TRArray* namespaces = triDictGetValues(cls->namespaces);
                
                if (namespaces)
                {
                    TR_FOR_EACH(TRClassNamespace*, ns, namespaces)
                    {
                        TRArray* objects = triDictGetValues(ns->objects); 
                        
                        if (objects && cls->destroy)
                        {      
                            TR_FOR_EACH(TRClass*, obj, objects)
                            {
                                if (obj)
                                {
                                    cls->destroy(obj);
                                }
                            }
                        }
                        triDestroyArray(objects);
                        triDestroyDict(ns->objects);
                        triFree(state, ns);
                    }
                    triDestroyArray(namespaces);
                }
                triFree(state, cls->name);
                triFree(state, cls->nsPath);
                triDestroyDict(cls->namespaces);
                triFree(state, cls);
            }
        }
        triDestroyArray(classes);
        triDestroyDict(state->classes);
        triDestroyDict(state->classesByName);
    }

    /* Power down the platform */    
    triPlatformExit(state);
    
    triFree(TR_NO_STATE, state);
    triSetGlobal(0);
}

void triSetError(TRState* state, TRenum error)
{
    /* Switched off error logging for now because it is too low level */
#if defined(TRACER_VERBOSE_LOGGING) && 0
    const TRbyte* msg = 0;
#endif
    
    TR_ASSERT(state);
    state->error = error;
    
#if defined(TRACER_VERBOSE_LOGGING) && 0
    switch (error)
    {
    case TR_OUT_OF_MEMORY:
        msg = "Out of memory";
        break;
    case TR_NOT_FOUND:
        msg = "Not found";
        break;
    case TR_INVALID_HANDLE:
        msg = "Invalid handle";
        break;
    case TR_BAD_VALUE:
        msg = "Bad value";
        break;
    case TR_BAD_NAME:
        msg = "Bad name";
        break;
    case TR_INVALID_OPERATION:
        msg = "Invalid operation";
        break;
    default:
        break;
    }
    
    if (msg)
    {
        triLogMessage(state, "Tracer error: %s.", msg);
    }
    else
    {
        triLogMessage(state, "Tracer error: 0x%X.", error);
    }
#endif    
}

TRbool triRegisterStreamDevice(TRState* state, const TRbyte* name, TRStreamDevice* dev)
{
#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Stream device '%s' registered.", name);
#endif
    triDictSet(state->streamDevices, name, dev);
    if (dev->ops->init)
    {
        dev->ops->init(dev);
    }
    return state->error == TR_NO_ERROR;
}

TRStreamDevice* triGetStreamDevice(TRState* state, const TRbyte* name)
{
    return (TRStreamDevice*)triStrDictGet(state->streamDevices, name);

}

TRbool triRegisterStreamCodec(TRState* state, const TRbyte* name, TRStreamCodec* codec)
{
#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Stream codec '%s' registered.", name);
#endif
    triDictSet(state->codecs, name, codec);
    return state->error == TR_NO_ERROR;
}

TRStreamCodec* triGetStreamCodec(TRState* state, const TRbyte* name)
{
    return (TRStreamCodec*)triStrDictGet(state->codecs, name);
}

TRbool triRegisterApi(TRState* state, TRenum id, 
                      TRuint functionCount, const TRFunction* functions)
{
    TRuint i;
    TRApi* api;

    /* The API id must be the next consecutive integer in the list of APIs */
    TR_ASSERT_ERROR_RETURN(state, id == triArraySize(state->apis), TR_BAD_NAME, TR_FALSE);

#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "API 0x%X registered with %d functions.", id, functionCount);
#endif

    api = triMalloc(state, sizeof(TRApi));
    TR_ASSERT_ERROR_RETURN(state, api, TR_OUT_OF_MEMORY, TR_FALSE);
    
    /* Initialize the API info */
    api->id                     = id;
    api->functions              = triMalloc(state, sizeof(TRFunction) * functionCount);
    api->functionCount          = functionCount;
    api->state                  = triCreateStateGroup(state);
    api->functionsByName        = triCreateStrDict(state);
    api->libraryHandles         = triCreateStrDict(state);
    api->dynamicFunctionName[0] = 0;
    
    if ((functionCount && !api->functions) || 
        !api->state || 
        !api->functionsByName)
    {
        triLogMessage(state, "Unable to register API 0x%X.", id);
        triFree(state, api->functions);
        triDestroyStateGroup(state, api->state);
        triDestroyDict(api->functionsByName);
        triDestroyDict(api->libraryHandles);
        triFree(state, api);
        triSetError(state, TR_OUT_OF_MEMORY);
        return TR_FALSE;
    }
    
    /* Copy the functions */
    for (i = 0; i < functionCount; i++)
    {
        api->functions[i] = functions[i];
        triStrDictSet(api->functionsByName, functions[i].name, &api->functions[i]);
    }
    
    /* Resolve the functions */
    for (i = 0; i < functionCount; i++)
    {
        TRhandle libHandle;
        if (!api->functions[i].libName)
        {
            continue;
        }

        libHandle = (TRhandle)triStrDictGet(api->libraryHandles, api->functions[i].libName);

        if (!libHandle)
        {
            libHandle = triOpenLibrary(state, api->functions[i].libName);
            if (!libHandle)
            {
                /* FIXME: memory leak */
                triLogMessage(state, "Failed to open library '%s'.", api->functions[i].libName);
                triSetError(state, TR_NOT_FOUND);
                return TR_FALSE;
            }
            triStrDictSet(api->libraryHandles, api->functions[i].libName, (void*)libHandle);
        }

        api->functions[i].pointer = triLookupSymbol(libHandle, functions[i].name, functions[i].ordinal);
        if (!api->functions[i].pointer)
        {
            triLogMessage(state, "Failed to look up symbol '%s' from library '%s'.", functions[i].name, functions[i].libName);
        }
    }

    /* Add it to the list of APIs */
    triArrayAppend(state->apis, api);

    return state->error == TR_NO_ERROR;
}

static TRApi* triGetApi(TRState* state, TRenum id)
{
    TR_ASSERT(id < triArraySize(state->apis));
    {
        return (TRApi*)triArrayGet(state->apis, id);
    }
}

static void triUpdateFpsEstimate(TRState* state, TREvent* event)
{
    if (event && event->function && event->function->flags & TR_FUNC_FRAME_MARKER)
    {
        TRtimestamp  now = triGetTimeStamp(state);
        TRtimestamp diff = now - state->fpsStartTime;
        
        state->fpsFrameCount++;
        
        if (diff > 500000)
        {
            state->fpsEstimate   = (1000000.0f * state->fpsFrameCount) / (TRint)diff;
            state->fpsStartTime  = now;
            state->fpsFrameCount = 0;
        }
    }
}

TRFunction* triLookupFunction(TRState* state, TRenum apiId, const TRbyte* name)
{
    TRApi* api = triGetApi(state, apiId);
    
    if (!api)
    {
        return 0;
    }
    
    /*
     *  If the API has no functions, dynamically create a new one
     */
    if (!api->functionCount)
    {
        /* Copy the function's name */
        triStringCopy(api->dynamicFunctionName, name, sizeof(api->dynamicFunctionName));
        
        /* Initialize the function */
        api->dynamicFunction.name         = api->dynamicFunctionName;
        api->dynamicFunction.ordinal      = -1;
        api->dynamicFunction.pointer      = 0;
        api->dynamicFunction.eventHandler = 0;
        api->dynamicFunction.flags        = 0;
        
        return &api->dynamicFunction;
    }
    
    return triStrDictGet(api->functionsByName, name);
}

TRFunction* triLookupFunctionByIndex(TRState* state, TRenum apiId, TRuint index)
{
    TRApi* api = triGetApi(state, apiId);
    
    if (!api)
    {
        return 0;
    }
    
    TR_ASSERT(index < api->functionCount);
    
    return &api->functions[index];
}

TRStream* triOpenStream(TRState* state, const TRbyte* device,
                        const TRbyte* name, const TRbyte* mode, const TRStrDict* options)
{
    TRStreamDevice* dev = triGetStreamDevice(state, device);
    TRStream* stream;

#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Opening stream '%s' from the '%s' device in mode '%s'.",
                  name, device, mode);
#endif

    TR_ASSERT(state);
    TR_ASSERT_ERROR_RETURN(state, dev, TR_BAD_NAME, 0);
    TR_ASSERT_ERROR_RETURN(state, dev->ops->open, TR_INVALID_OPERATION, 0);

    stream = triMalloc(state, sizeof(TRStream));
    stream->device = dev;
    
    if (dev->ops->open(dev, stream, name, mode, options) != TR_NO_ERROR)
    {
        triLogMessage(state, "Unable to open '%s' from the '%s' device in mode '%s'.", 
                      name, device, mode);
        triFree(state, stream);
        return 0;
    }
    
    /* Add it to the list of streams */
    triArrayAppend(state->streams, stream);
    return stream;
}

TRStreamEncoder* triAttachEncoder(TRState* state, TRStream* stream, const TRbyte* codec, const TRStrDict* options)
{
    TRStreamCodec*  cod = triGetStreamCodec(state, codec);
    TRStreamEncoder* encoder;

#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Attaching encoder '%s'.", codec);
#endif
    
    TR_ASSERT(state);
    TR_ASSERT_ERROR_RETURN(state, stream, TR_INVALID_HANDLE, 0);
    TR_ASSERT_ERROR_RETURN(state, cod, TR_BAD_NAME, 0);
    TR_ASSERT_ERROR_RETURN(state, cod->ops->attachEncoder, TR_INVALID_OPERATION, 0);

    encoder = triMalloc(state, sizeof(TRStreamEncoder));
    
    TR_ASSERT_ERROR_RETURN(state, encoder, TR_OUT_OF_MEMORY, 0);
    
    encoder->codec  = cod;
    encoder->stream = stream;
    encoder->ops    = 0;
        
    cod->ops->attachEncoder(cod, encoder, options);
    
    /* Verify the encoder */
    if (!encoder->ops->beginEvent         ||
        !encoder->ops->endEvent           ||
        !encoder->ops->byteValue          ||
        !encoder->ops->shortValue         ||
        !encoder->ops->integerValue       ||
        !encoder->ops->floatValue         ||
        !encoder->ops->doubleValue        ||
        !encoder->ops->objectValue        ||
        !encoder->ops->definePointerData  ||
        !encoder->ops->beginObject        ||
        !encoder->ops->endObject)
    {
        triLogMessage(state, "Unable to attach encoder '%s'.", codec);
        if (cod->ops->detachEncoder)
        {
            cod->ops->detachEncoder(cod, encoder);
        }
        triFree(state, encoder);
        TR_ASSERT_ERROR_RETURN(state, TR_FALSE, TR_OUT_OF_MEMORY, 0);
    }

    /* Add it to the list of encoders */
    triArrayAppend(state->encoders, encoder);
    return encoder;
}

TRStreamDecoder* triAttachDecoder(TRState* state, TRStream* stream, const TRbyte* codec, const TRStrDict* options)
{
    TRStreamCodec*  cod = triGetStreamCodec(state, codec);
    TRStreamDecoder* decoder;

#if defined(TRACER_VERBOSE_LOGGING)
    triLogMessage(state, "Attaching decoder '%s'.", codec);
#endif
    
    TR_ASSERT(state);
    TR_ASSERT_ERROR_RETURN(state, stream, TR_INVALID_HANDLE, 0);
    TR_ASSERT_ERROR_RETURN(state, cod, TR_BAD_NAME, 0);
    TR_ASSERT_ERROR_RETURN(state, cod->ops->attachDecoder, TR_INVALID_OPERATION, 0);

    decoder = triMalloc(state, sizeof(TRStreamDecoder));
    
    TR_ASSERT_ERROR_RETURN(state, decoder, TR_OUT_OF_MEMORY, 0);
    
    decoder->codec  = cod;
    decoder->stream = stream;
    decoder->ops    = 0;
        
    cod->ops->attachDecoder(cod, decoder, options);
    
    /* Verify the decoder */
    if (!decoder->ops->getEvent         ||
        !decoder->ops->readByteValue    ||
        !decoder->ops->readShortValue   ||
        !decoder->ops->readIntegerValue ||
        !decoder->ops->readLongValue    ||
        !decoder->ops->readFloatValue   ||
        !decoder->ops->readDoubleValue  ||
        !decoder->ops->readPointerValue)
    {
        triLogMessage(state, "Unable to attach decoder '%s'.", codec);
        if (cod->ops->detachDecoder)
        {
            cod->ops->detachDecoder(cod, decoder);
        }
        triFree(state, decoder);
        TR_ASSERT_ERROR_RETURN(state, TR_FALSE, TR_OUT_OF_MEMORY, 0);
    }

    /* Add it to the list of decoders */
    triArrayAppend(state->decoders, decoder);
    return decoder;
}

const TRArray* triGetStreams(TRState* state)
{
    TR_ASSERT(state);
    return state->streams;
}

const TRArray* triGetEncoders(TRState* state)
{
    TR_ASSERT(state);
    return state->encoders;
}

const TRArray* triGetDecoders(TRState* state)
{
    TR_ASSERT(state);
    return state->decoders;
}

TRbool triCloseStream(TRState* state, TRStream* stream)
{
    TR_ASSERT(state);
    TR_ASSERT(stream);
    TR_ASSERT(stream->device);
    
    if (stream->device->ops->close)
    {
        stream->device->ops->close(stream->device, stream);
    }
    TR_ASSERT(triArrayRemove(state->streams, stream));
    triFree(state, stream);
    return TR_TRUE;
}

TRbool triDetachEncoder(TRState* state, TRStreamEncoder* encoder)
{
    TR_ASSERT(state);
    TR_ASSERT(encoder);
    TR_ASSERT(encoder->codec);
    
    if (encoder->codec->ops->detachEncoder)
    {
        encoder->codec->ops->detachEncoder(encoder->codec, encoder);
    }
    TR_ASSERT(triArrayRemove(state->encoders, encoder));
    triFree(state, encoder);
    return TR_TRUE;
}

TRbool triDetachDecoder(TRState* state, TRStreamDecoder* decoder)
{
    TR_ASSERT(state);
    TR_ASSERT(decoder);
    TR_ASSERT(decoder->codec);
    
    if (decoder->codec->ops->detachDecoder)
    {
        decoder->codec->ops->detachDecoder(decoder->codec, decoder);
    }
    TR_ASSERT(triArrayRemove(state->decoders, decoder));
    triFree(state, decoder);
    return TR_TRUE;
}

TRtimestamp triGetTimeStamp(TRState* state)
{
    TR_ASSERT(state);
    return triGetSystemTime(state) - state->baseTime;
}

TRStateGroup* triGetApiState(TRState* state, TRenum apiId)
{
    TR_ASSERT(state);
    {
        TRApi* api = triGetApi(state, apiId);
        TR_ASSERT_ERROR_RETURN(state, api, TR_INVALID_HANDLE, 0);
        return api->state;
    }
}

TRStateTerminalValue* triCreateStateTerminalValue(TRState* state, void* data, TRbool allocated)
{
    TRStateTerminalValue* value = triMalloc(state, sizeof(TRStateTerminalValue));
    value->data                 = data;
    value->allocated            = allocated;
    return value;
}

TRStateNode* triCreateStateNode(TRState* state, void* value)
{
    TRStateNode* node = triMalloc(state, sizeof(TRStateNode));
    TR_ASSERT(node);
    node->keys        = triCreateIntDict(state);
    node->value       = value;
    node->current     = 0;
    return node;
}

TRStateGroup* triCreateStateGroup(TRState* state)
{
    TRStateGroup* group = triMalloc(state, sizeof(TRStateGroup));
    TR_ASSERT(group);
    group->nodes        = triCreateArray(state);
    group->values       = triCreateArray(state);
    return group;
}

TRStateGroup* triTraverseStatePath(TRState* state, TRStateGroup* root, const TRbyte* path, const TRint* keys, \
                                   TRbool pathMustExist, TRbyte* keyOut, TRbool* terminalOut)
{
    TRStateNode* node;

    TR_ASSERT(path);
    TR_ASSERT(keyOut);
    TR_ASSERT(terminalOut);

    /* Traverse the path up to the next-to-last component */
    while (root && *path && *(path + 1))
    {
        TRbyte key = ((*path++) - 1) & 0x7f;

        /* Create the node if it doesn't exist */
        if (triArraySize(root->nodes) > key)
        {
            node = triArrayGet(root->nodes, key);
        }
        else
        {
            node = 0;
        }
        
        if (!node)
        {
            if (!pathMustExist)
            {
                node = triCreateStateNode(state, 0);
                triArraySet(root->nodes, key, node);
            }
            else
            {
                *keyOut      = 0;
                *terminalOut = 0;
                return 0;
            }
        }

        /* 0xff indicates that a specific key must be read at this point */
        if (*((unsigned char*)path) == 0xff)
        {
            root = (TRStateGroup*)triIntDictGet(node->keys, *keys);
            
            if (!root)
            {
                *keyOut      = 0;
                *terminalOut = 0;
                return 0;
            }
            
            path++;
            keys++;
        }
        else
        {
            root = (TRStateGroup*)node->current;
            
            if (!root)
            {
                if (!pathMustExist)
                {
                    /* Create a new default group */
                    root = triCreateStateGroup(state);
                    triIntDictSet(node->keys, 0, root);
                    node->current = root;
                }
                else
                {
                    *keyOut      = 0;
                    *terminalOut = 0;
                    return 0;
                }
            }
        }
    }
    *keyOut      = (*path - 1) & 0x7f;
    *terminalOut = (*path - 1) & 0x80;
    return root;
}

#if defined(TRACER_VERBOSE_STATE_LOGGING)
void triDescribePath(const TRbyte* path, TRbyte* descOut)
{
    const char hexDigits[] = "0123456789abcdef";
    TRint i;
    
    for (i = 0; i < triStringLength(path); i++)
    {
        descOut[i * 3 + 0] = hexDigits[(path[i] & 0xf0) >> 4];
        descOut[i * 3 + 1] = hexDigits[(path[i] & 0xf)];
        descOut[i * 3 + 2] = path[i + 1] ? '/' : 0;
    }
}
#endif

void triSetStateValue(TRState* state, TRStateGroup* root, const TRbyte* path, void* value, TRbool allocated)
{
    TRbyte        key;
    TRbool        terminal;
    TRStateGroup* group = triTraverseStatePath(state, root, path, 0, TR_FALSE, &key, &terminal);
    TR_ASSERT_ERROR(state, group, TR_NOT_FOUND);
    TR_ASSERT_ERROR(state, (allocated && terminal) || !allocated, TR_BAD_VALUE);
    {
        TRStateNode*  valueNode = 0;
        TRStateGroup* subGroup;

#if defined(TRACER_VERBOSE_STATE_LOGGING)
        TRbyte pathDesc[256];
        triDescribePath(path, pathDesc);
        triLogMessage(state, "Setting state value: %s = %p", pathDesc, value);
#endif
        
        /* If this is a terminal, then the value is directly in the value array */
        if (terminal)
        {
            TRStateTerminalValue* valueInfo = 0;

            if (triArraySize(group->values) > key)
            {
                valueInfo = (TRStateTerminalValue*)triArrayGet(group->values, key);
            }

            if (!valueInfo)
            {
                valueInfo = triCreateStateTerminalValue(state, value, allocated);
            }
            else
            {
                if (valueInfo->allocated)
                {
                    triFree(state, valueInfo->data);
                }
                valueInfo->data      = value;
                valueInfo->allocated = allocated;
            }
            triArraySet(group->values, key, valueInfo);
            return;
        }

        if (triArraySize(group->nodes) > key)
        {
            valueNode = triArrayGet(group->nodes, key);
        }

        /* Create a new node for the value if it doesn't exist yet */
        if (!valueNode)
        {
            valueNode = triCreateStateNode(state, value);
            triArraySet(group->nodes, key, valueNode);
        }

        /* Make sure the subgroup for the given value exists */
        subGroup = triIntDictGet(valueNode->keys, (TRint)value);
        if (!subGroup)
        {
            subGroup = triCreateStateGroup(state);
            triIntDictSet(valueNode->keys, (TRint)value, subGroup);
        }
        
        /* Point the node at the new value */
        valueNode->current = subGroup;
        valueNode->value   = value;
    }
}

TR_INLINE void* triGetStateGroupValue(TRStateGroup* group, TRbyte key, TRbool terminal)
{
    if (terminal)
    {
        if (triArraySize(group->values) > key)
        {
            TRStateTerminalValue* value = triArrayGet(group->values, key);
            return value ? value->data : 0;
        }
        return 0;
    }
    else
    {
        if (triArraySize(group->nodes) > key)
        {
            TRStateNode* node = (TRStateNode*)triArrayGet(group->nodes, key);
            return node ? node->value : 0;
        }
        return 0;
    }
}

void* triGetStateValue(TRState* state, TRStateGroup* root, const TRbyte* path)
{
    TRbyte        key;
    TRbool        terminal;
    TRStateGroup* group = triTraverseStatePath(state, root, path, 0, TR_TRUE, &key, &terminal);
    TR_ASSERT_ERROR_RETURN(state, group, TR_NOT_FOUND, 0);
    {
#if defined(TRACER_VERBOSE_STATE_LOGGING)
        TRbyte pathDesc[256];
        void* value;

        triDescribePath(path, pathDesc);
        triLogMessage(state, "Getting state value: %s = %p", pathDesc, triGetStateGroupValue(group, key, terminal));
#endif
        return triGetStateGroupValue(group, key, terminal);
    }
}

void* triGetSpecificStateValue(TRState* state, TRStateGroup* root, const TRbyte* path, const TRint* keys)
{
    TRbyte        key;
    TRbool        terminal;
    TRStateGroup* group = triTraverseStatePath(state, root, path, keys, TR_TRUE, &key, &terminal);
    TR_ASSERT_ERROR_RETURN(state, group, TR_NOT_FOUND, 0);
    {
#if defined(TRACER_VERBOSE_STATE_LOGGING)
        TRbyte pathDesc[256];
        void* value;

        triDescribePath(path, pathDesc);
        triLogMessage(state, "Getting specific state value: %s = %p", pathDesc, triGetStateGroupValue(group, key, terminal));
#endif
        return triGetStateGroupValue(group, key, terminal);
    }
}

void triDestroyStateTerminalValue(TRState* state, TRStateTerminalValue* value)
{
    if (value->allocated)
    {
        triFree(state, value->data);
    }
    triFree(state, value);
}

void triDestroyStateNode(TRState* state, TRStateNode* root)
{
    TRArray* values = triDictGetValues(root->keys);

    if (values)
    {
        TR_FOR_EACH(TRStateGroup*, group, values)
        {
            if (!group) continue;
            triDestroyStateGroup(state, group);
        }
        triDestroyArray(values);
    }
    triDestroyDict(root->keys);
    triFree(state, root);
}

void triDestroyStateGroup(TRState* state, TRStateGroup* root)
{
    /* Destroy the nodes under this group */
    TR_FOR_EACH(TRStateNode*, node, root->nodes)
    {
        if (!node) continue;
        triDestroyStateNode(state, node);
    }

    /* Destroy the terminal values */
    if (root->values)
    {
        TR_FOR_EACH(TRStateTerminalValue*, value, root->values)
        {
            if (!value) continue;
            triDestroyStateTerminalValue(state, value);
        }
    }
    
    triDestroyArray(root->nodes);
    triDestroyArray(root->values);
    triFree(state, root);
}

void triRemoveStateValue(TRState* state, TRStateGroup* root, const TRbyte* path)
{
    TRbyte        key;
    TRbool        terminal;
    TRStateGroup* group = triTraverseStatePath(state, root, path, 0, TR_TRUE, &key, &terminal);
    TR_ASSERT_ERROR(state, group, TR_NOT_FOUND);
    {
        TRStateNode* node;
        
        /* If it's a terminal, destroy the associated value */
        if (terminal)
        {
            TRStateTerminalValue* value = (TRStateTerminalValue*)triArrayGet(group->values, key);
            if (value)
            {
                triDestroyStateTerminalValue(state, value);
            }
            return;
        }
        node = (TRStateNode*)triArrayGet(group->nodes, key);
        triDestroyStateNode(state, node);
        triArraySet(group->nodes, key, 0);
    }
}

TREvent* triBeginEvent(TRState* state, TRenum api, const TRFunction* function)
{
    TR_ASSERT(state->openEvents < TR_MAX_OPEN_EVENTS);
    {
        TREvent* event = &state->eventStack[state->openEvents++];
        
#if defined(TRACER_VERBOSE_LOGGING)
        triLogMessage(state, "Event: %s", function ? function->name : "(null)");
#endif
        event->state    = state;
        event->api      = api;
        event->function = function;
        event->callTime = 0;
        
        /* Only report the top-level event for recursive events */
        if (function && state->openEvents == 1)
        {
            event->timestamp      = triGetTimeStamp(state);
            event->sequenceNumber = triGetNextSequenceNumber(state);
            triUpdateFpsEstimate(state, event);
            {
                TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(state))
                {
                    enc->ops->beginEvent(enc, event);
                }
            }
        }
        return event;
    }
}

void triEndEvent(TREvent* event)
{
    if (event && --event->state->openEvents == 0 && event->function)
    {
        TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(event->state))
        {
            enc->ops->endEvent(enc, event);
        }
    }
}

static TRbool _triDefinePointerData(TRState* state, const void* pointer, TRuint length,
                                    const TRubyte* data, TRenum type, const TRClass* cls, TRhandle ns)
{
    TRbool changed = TR_TRUE;
    TRPointerInfo* info;
  
    /* Don't track NULL pointers */
    if (!pointer)
    {
        return TR_FALSE;
    }
    
    /* Empty contents -> delete the pointer tracking info */
    if (!length || !data)
    {
        info = (TRPointerInfo*)triDictGet(state->pointers, pointer);

        if (info)
        {
            state->pointerDataSize -= info->allocLength;
            triFree(state, info->data);
            triFree(state, info);
            triDictRemove(state->pointers, pointer);
        }
        
        /* Notify the encoders about the deletion */
        {        
            TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(state))
            {
                enc->ops->definePointerData(enc, pointer, 0, 0, type, cls, ns);
            }
        }
        return info ? TR_TRUE : TR_FALSE;
    }

    info = (TRPointerInfo*)triDictGet(state->pointers, pointer);
    
#if defined(TRACER_VERBOSE_POINTER_LOGGING)
    triLogMessage(state, "Defining pointer %p:", pointer);
    if (info)
    {
        triLogMessage(state, "---: Old length = %d, updates = %d", info->length, info->updateCount);
    }
    triLogMessage(state, "---: New length = %d", length);
    triLogMessage(state, "---: %d bytes cached in total", state->pointerDataSize);
#endif

    /* If this is a new pointer, allocate the info for it */    
    if (!info)
    {
        info              = triMalloc(state, sizeof(TRPointerInfo));
        if (info)
        {
            info->data        = 0;
            info->length      = 0;
            info->allocLength = 0;
            info->updateCount = 0;
            triDictSet(state->pointers, pointer, info);
        }
    }
    else if (length <= info->length && info->data)
    {
        /* If the data block shrunk, check whether the contents have changed. If they haven't,
         * keep the larger block around.
         */
        changed = (triMemCompare(data, info->data, length) != 0);
#if defined(TRACER_VERBOSE_POINTER_LOGGING)
        triLogMessage(state, "---: Block shrunk, changed = %d", changed);
#endif
    }

    /* If the data changed, communicate that to the encoders */
    if (changed)
    {
        /* If this is the first time we see this pointer, do not allocate a copy. */
        if (info && info->updateCount++ > 0)
        {
            /* 
             *  If the new data does not fit into the old block, allocate a new one.
             */
            if (!info->data || length > info->allocLength)
            {
                /* Free the old block */
                if (info->data)
                {
                    triFree(state, info->data);
                    state->pointerDataSize -= info->allocLength;
                }
                
                /*
                 *  If the pointer cache has reached its limit, free some old data.
                 *  Ideally, the cache should be shrunk in an LRU fashion, but since
                 *  this memory limit is usually not used, we will just delete random
                 *  cache entries until we're good to go.
                 */
                if (state->pointerDataSize >= state->pointerDataSizeLimit)
                {
                    TRArray* keys = triDictGetKeys(state->pointers);
#if defined(TRACER_VERBOSE_POINTER_LOGGING)
                    triLogMessage(state, "---: Cache limit %d reached, deleting some entries", state->pointerDataSizeLimit);
#endif
                    if (keys)
                    {
                        TRPointerInfo* info2;
                        TR_FOR_EACH(void*, ptr, keys)
                        {
                            /* Don't delete the new pointer */
                            if (ptr == pointer) 
                            {
                                continue;
                            }
                            
                            /* Destroy the pointer info */
                            info2 = (TRPointerInfo*)triDictGet(state->pointers, ptr);
                            if (info2)
                            {
                                state->pointerDataSize -= info2->allocLength;
                                triFree(state, info2->data);
                                triFree(state, info2);
                                triDictRemove(state->pointers, ptr);
                            }
                            
                            if (state->pointerDataSize < state->pointerDataSizeLimit)
                            {
                                break;
                            }
                        }
                        triDestroyArray(keys);
                    }
                    else
                    {
                        TR_ASSERT(!"Out of memory");
                    }
                }
    
                info->data              = triMalloc(state, length * sizeof(TRbyte));
                info->allocLength       = length;
                state->pointerDataSize += length;
            }
                
            /*
             *  No need to check for out of memory condition, since we just end up
             *  doing some extra work if we can't keep the data around.
             */
            if (info->data)
            {
                TR_ASSERT(info->allocLength >= length);
                triMemCopy(info->data, data, length);
            }
            info->length = length;
#if defined(TRACER_VERBOSE_POINTER_LOGGING)
            triLogMessage(state, "---: %d bytes cached in total (updated)", state->pointerDataSize);
#endif
        }
        
        {        
            TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(state))
            {
                enc->ops->definePointerData(enc, pointer, length, data, type, cls, ns);
            }
        }
    }
    return changed;
}

TRbool triDefineSimpleArrayData(TRState* state, const void* pointer, TRuint length,
                                const TRubyte* data, TRenum type)
{
    TR_ASSERT(type != TR_OBJECT);
    return _triDefinePointerData(state, pointer, length, data, type, 0, 0);
}

TRbool triDefineObjectArrayData(TRState* state, TRuint length, const TRClass* cls, TRhandle ns,
                                const TRhandle* array)
{
    return _triDefinePointerData(state, (const void*)array, length * sizeof(TRhandle),
                                 (const TRubyte*)array, TR_OBJECT, cls, ns);
}

/* Runtime configuration */
TRbool triLoadConfig(TRState* state, const TRbyte* path)
{
    TRFile      f = triOpenFile(state, path, "r");
    TRbyte*     line = 0;
    const TRint lineSize = 256;
    TRint       length, i;
    TRStrDict*  config = 0;
    TRStrDict*  section = 0;
    
#if defined(TRACER_VERBOSE_CONFIG_LOGGING)
    triLogMessage(state, "Loading config file '%s'", path);
#endif

    if (!f)
    {
        triLogMessage(state, "Unable to open config file '%s'", path);
        triSetError(state, TR_NOT_FOUND);
        return TR_FALSE;
    }
    
    line   = triMalloc(state, lineSize);
    config = state->config ? state->config : triCreateStrDict(state);
    
    if (!line || !config)
    {
        triFree(state, line);
        if (!state->config)
        {
            triDestroyDict(config);
        }
        triCloseFile(f);
        triSetError(state, TR_OUT_OF_MEMORY);
        return TR_FALSE;
    }
    
    state->config = config;
    
    while (!triEndOfFile(f))
    {
        i = 0;
        while (i < lineSize - 1)
        {
            TRbyte c = triReadFileByte(f);
            if (triEndOfFile(f)) break;
            if (c == '\r')       continue;
            if (c == '\n')       break;
            line[i++] = c;
        }
        line[i] = 0;
        
        length = triStringLength(line);
        if (!length)
        {
            continue;
        }
            
        /* Strip comments */
        for (i = 0; i < length; i++)
        {
            if (line[i] == '#')
            {
                while (i >= 0 && line[--i] == ' ') { }
                line[i + 1] = 0;
                length  = i;
                break;
            }
        }
        
        /* Parse a section header */
        if (line[0] == '[' && line[length - 1] == ']')
        {
            line[length - 1] = 0;
            section = triCreateStrDict(state);
            if (!section)
            {
                triCloseFile(f);
                triFree(state, line);
                triSetError(state, TR_OUT_OF_MEMORY);
                return TR_FALSE;
            }            
            triStrDictSet(config, &line[1], section);
#if defined(TRACER_VERBOSE_CONFIG_LOGGING)
            triLogMessage(state, "Entered config section '%s'", &line[1]);
#endif
            continue;
        }
        
        if (!section)
        {
            continue;
        }
        
        /* Parse a value */
        for (i = 0; i < length; i++)
        {
            if (line[i] == '=')
            {
                TRint l = i - 1, r = i + 1;
                while (l >= 0     && line[l] == ' ') l--;
                while (r < length && line[r] == ' ') r++;
                line[l + 1] = 0;
                triStrDictSet(section, line, triStringDuplicate(state, &line[r]));
#if defined(TRACER_VERBOSE_CONFIG_LOGGING)
                triLogMessage(state, "Read value %s = %s", line, &line[r]);
#endif
            }
        }
    }
    triCloseFile(f);
    triFree(state, line);
    return TR_TRUE;
}

const TRbyte* triGetConfigString(TRState* state, const TRbyte* section, const TRbyte* option, const TRbyte* dfl)
{
    TR_ASSERT_ERROR_RETURN(state, section, TR_BAD_VALUE, 0);
    TR_ASSERT_ERROR_RETURN(state, option,  TR_BAD_VALUE, 0);

    if (state->config)
    {
        TRStrDict* values = triDictGet(state->config, section);
        if (values)
        {
            const TRbyte* value = triDictGet(values, option);
            if (value)
            {
                return value;
            }
        }
    }    
    return dfl;
}

const TRStrDict* triGetConfigSection(TRState* state, const TRbyte* section)
{
    return state->config ? triDictGet(state->config, section) : 0;
}

TRint triGetConfigInteger(TRState* state, const TRbyte* section, const TRbyte* option, TRint dfl)
{
    const TRbyte* value = triGetConfigString(state, section, option, 0);
    return value ? atoi(value) : dfl;
}

TREvent* triGetEvent(TRState* state)
{
    TR_ASSERT(state->openEvents < TR_MAX_OPEN_EVENTS);
    {
        TREvent* event   = &state->eventStack[state->openEvents];
        event->state     = state;
        
        triUpdateFpsEstimate(state, event);
        {
            TR_FOR_EACH(TRStreamDecoder*, dec, triGetDecoders(state))
            {
                if (dec->ops->getEvent(dec, event) == TR_NO_ERROR)
                {
                    return event;
                }
            }
        }
        return 0;
    }
}

TRbool triRegisterObject(TRState* state, TRenum cls, TRhandle ns, TRhandle traceHandle, TRhandle handle)
{
    TRClass* c = (TRClass*)triIntDictGet(state->classes, cls);
    
    TR_ASSERT_ERROR_RETURN(state, c, TR_BAD_NAME, TR_FALSE);
    {
        /* TODO: Why cast to int? */
        TRClassNamespace* classNs = (TRClassNamespace*)triIntDictGet(c->namespaces, (TRint)ns);

        /* Register new namespace if needed */
        if (!classNs)
        {
            classNs = (TRClassNamespace*)triMalloc(state, sizeof(TRClassNamespace));
            TR_ASSERT_ERROR_RETURN(state, classNs, TR_OUT_OF_MEMORY, TR_FALSE);
            classNs->objects = triCreateIntDict(state);
            if (!classNs->objects)
            {
                triFree(state, classNs);
                TR_ASSERT_ERROR_RETURN(state, classNs->objects, TR_OUT_OF_MEMORY, TR_FALSE);
            }
            triIntDictSet(c->namespaces, (TRint)ns, classNs);
        }
        triIntDictSet(classNs->objects, (TRint)traceHandle, handle);
    }
    
    return TR_TRUE;
}

TRbool triRegisterClass(TRState* state, TRenum cls, const TRbyte* name, const TRbyte* namespacePath, 
                        TRObjectConstructor con, TRObjectDestructor dec, TRObjectSerializer ser, TRObjectModifier mod)
{
    TRClass* c;
    
    TR_ASSERT_ERROR_RETURN(state, !triIntDictGet(state->classes, cls), TR_BAD_NAME, TR_FALSE);
    
    c = triMalloc(state, sizeof(TRClass));    
    
    TR_ASSERT_ERROR_RETURN(state, c, TR_OUT_OF_MEMORY, TR_FALSE);
    
    c->name       = triStringDuplicate(state, name);
    c->id         = cls;
    c->nsPath     = namespacePath ? triStringDuplicate(state, namespacePath) : NULL;
    c->create     = con;
    c->destroy    = dec;
    c->serialize  = ser;
    c->modify     = mod;
    c->namespaces = triCreateIntDict(state);
    
    TR_ASSERT_ERROR_RETURN(state, c->namespaces, TR_OUT_OF_MEMORY, TR_FALSE);
    
    triIntDictSet(state->classes,       cls,  c);
    triStrDictSet(state->classesByName, name, c);
    
    return TR_TRUE;
}

TRhandle triGetObject(TRState* state, TRenum cls, TRhandle ns, TRhandle handle, TRhandle defaultHandle)
{
    TRClass* c = (TRClass*)triIntDictGet(state->classes, cls);
    TRClassNamespace* classNs = 0;

    TR_ASSERT_ERROR_RETURN(state, c, TR_BAD_NAME, defaultHandle);
    classNs = (TRClassNamespace*)triIntDictGet(c->namespaces, (TRint)ns);
    TR_ASSERT_ERROR_RETURN(state, classNs, TR_INVALID_HANDLE, defaultHandle);

    if (!triIntDictContains(classNs->objects, (void*)handle))
    {
        return defaultHandle;
    }

    handle = (TRhandle)triIntDictGet(classNs->objects, (TRint)handle);
    return handle;
}

TRbool triSerializeObject(TRState* state, TRenum cls, TRhandle ns, TRhandle handle)
{
    TRClass* c     = (TRClass*)triIntDictGet(state->classes, cls);
    TREvent* event = &state->eventStack[state->openEvents];
    
    TR_ASSERT_ERROR_RETURN(state, c, TR_BAD_NAME, TR_FALSE);
    TR_ASSERT(state->openEvents < TR_MAX_OPEN_EVENTS);
    {
        /* Don't serialize NULL objects */
        if (handle)
        {
            event->state    = state;
            event->api      = 0;
            event->function = 0;
            event->callTime = 0;

            {
                TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(state))
                {
                    enc->ops->beginObject(enc, c, ns, handle);
                }
            }
            if (c->serialize)
            {
                c->serialize(event, handle);
            }
            {
                TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(state))
                {
                   enc->ops->endObject(enc);
                }
            }
        }
        return TR_TRUE;
    }
}

TRhandle triUnserializeObject(TRState* state, TRenum cls, TRhandle ns, TRhandle handle)
{
    TRClass* objectClass = (TRClass*)triIntDictGet(state->classes, cls);
    TREvent* event       = &state->eventStack[state->openEvents];
    
    TR_ASSERT_ERROR_RETURN(state, handle,              TR_INVALID_HANDLE,    0);
    TR_ASSERT_ERROR_RETURN(state, objectClass,         TR_BAD_NAME,          0);
    TR_ASSERT_ERROR_RETURN(state, objectClass->create, TR_INVALID_OPERATION, 0);
    {
        /* Does this object already exist? */
        TRhandle existingObject = triGetObject(state, objectClass->id, ns, handle, 0);
        TRhandle newObject;
        
        /* If this object exists and can be modified at runtime, refresh it with new data */
        if (existingObject && objectClass->modify)
        {
            objectClass->modify(event, existingObject);
            return existingObject;
        }
        else
        {
            if (existingObject)
            {
                if (objectClass->destroy)
                {
                    objectClass->destroy(existingObject);
                }
            }
            
            /* Create and register a new object */
            newObject = objectClass->create(event);
            triRegisterObject(state, cls, ns, handle, newObject);

            return newObject;
        }
    }
}

TRClass* triGetClassByName(TRState* state, const TRbyte* name)
{
    return triStrDictGet(state->classesByName, name);
}

TRClass* triGetClass(TRState* state, TRenum cls)
{
    return (TRClass*)triIntDictGet(state->classes, cls);
}

TRbool triObjectValue(TRState* state, const TRbyte* name, const TRObject* object)
{
    TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(state))
    {
        enc->ops->objectValue(enc, name, object);
    }
    return TR_TRUE;
}
