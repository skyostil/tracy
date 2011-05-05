/**
 *  Generic Tracer API implementation.
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
#include "tracer_config.h"
#include <stdio.h>
#include <stdlib.h>

TREvent* trBeginEvent(TRenum api, const TRbyte* name)
{
    TRState*   state = triGetState();
    {
        TRFunction* func = name ? triLookupFunction(state, api, name) : 0;
        return triBeginEvent(state, api, func);
    }
}

TREvent* trBeginEventByIndex(TRenum api, TRuint index)
{
    TRState*   state = triGetState();
    TRFunction* func = triLookupFunctionByIndex(state, api, index);
    return triBeginEvent(state, api, func);
}

TREvent* trBeginTerminatorEventByIndex(TRenum api, TRuint index)
{
    TRState*   state = triGetStateWithoutUserInit();
    TRFunction* func = triLookupFunctionByIndex(state, api, index);
    return triBeginEvent(state, api, func);
}

void trEndEvent(TREvent* event)
{
    triEndEvent(event);
    if (event && event->state->synchronousOperation)
    {
        trFlush();
    }
}

/** 
 *  A macro that defines a value logger for a specific type.
 *
 *  @param NAME Name of the logger function
 *  @param TYPE Type of the value to log
 *  @param OP   Encoder operation to call
 */
#define TR_VALUE_LOGGER(NAME, TYPE, OP)                                    \
void NAME(TREvent* event, const TRbyte* name, TYPE value)                  \
{                                                                          \
    TR_ASSERT(event);                                                      \
    if (event->state->openEvents == 1)                                     \
    {                                                                      \
        TR_FOR_EACH(TRStreamEncoder*, enc, triGetEncoders(event->state))   \
        {                                                                  \
            enc->ops->OP(enc, name, value);                                \
        }                                                                  \
    }                                                                      \
}

/** 
 *  A macro that defines a value loader for a specific type.
 *
 *  @param NAME Name of the loader function
 *  @param TYPE Type of the value to load
 *  @param OP   Decoder operation to call
 */
#define TR_VALUE_LOADER(NAME, TYPE, OP)                                    \
TYPE NAME(TREvent* event, const TRbyte* name)                              \
{                                                                          \
    TR_ASSERT(event);                                                      \
    {                                                                      \
        TYPE value = 0;                                                    \
        TR_FOR_EACH(TRStreamDecoder*, dec, triGetDecoders(event->state))   \
        {                                                                  \
            if (dec->ops->OP(dec, name, &value) == TR_NO_ERROR)            \
            {                                                              \
                break;                                                     \
            }                                                              \
        }                                                                  \
        return value;                                                      \
    }                                                                      \
}

/** 
 *  A macro that defines an array value logger for a specific type.
 *
 *  @param NAME Name of the logger function
 *  @param TYPE Type of the value array to log
 *  @param OP   Encoder operation to call
 */
#define TR_VALUE_ARRAY_LOGGER(NAME, TYPE, TYPE_ID)                      \
void NAME(TREvent* event, const TRbyte* name, TRuint size, TYPE array)  \
{                                                                       \
    if (event && event->state->openEvents == 1)                         \
    {                                                                   \
        trDefinePointerData(event, array, size * sizeof(array[0]),      \
                           (const TRubyte*)array, TYPE_ID);             \
        trPointerValue(event, name, array);                             \
    }                                                                   \
}

/* Define loggers for all supported types */
TR_VALUE_LOGGER(trByteValue,               TRubyte,         byteValue)
TR_VALUE_LOGGER(trShortValue,              TRshort,         shortValue)
TR_VALUE_LOGGER(trIntegerValue,            TRint,           integerValue)
TR_VALUE_LOGGER(trLongValue,               TRlong,          longValue)
TR_VALUE_LOGGER(trFloatValue,              TRfloat,         floatValue)
TR_VALUE_LOGGER(trDoubleValue,             TRdouble,        doubleValue)
TR_VALUE_LOGGER(trPointerValue,            const void*,     pointerValue)
TR_VALUE_ARRAY_LOGGER(trByteArrayValue,    const TRubyte*,  TR_BYTE)
TR_VALUE_ARRAY_LOGGER(trShortArrayValue,   const TRshort*,  TR_SHORT)
TR_VALUE_ARRAY_LOGGER(trIntegerArrayValue, const TRint*,    TR_INTEGER)
TR_VALUE_ARRAY_LOGGER(trLongArrayValue,    const TRlong*,   TR_LONG)
TR_VALUE_ARRAY_LOGGER(trFloatArrayValue,   const TRfloat*,  TR_FLOAT)
TR_VALUE_ARRAY_LOGGER(trDoubleArrayValue,  const TRdouble*, TR_DOUBLE)

/* Define loaders for all supported types */
TR_VALUE_LOADER(trGetByteValue,            TRubyte,         readByteValue)
TR_VALUE_LOADER(trGetShortValue,           TRshort,         readShortValue)
TR_VALUE_LOADER(trGetIntegerValue,         TRint,           readIntegerValue)
TR_VALUE_LOADER(trGetLongValue,            TRlong,          readLongValue)
TR_VALUE_LOADER(trGetFloatValue,           TRfloat,         readFloatValue)
TR_VALUE_LOADER(trGetDoubleValue,          TRdouble,        readDoubleValue)
TR_VALUE_LOADER(trGetPointerValue,         void*,           readPointerValue)

TRbool trDefinePointerData(TREvent* event, const void* pointer, TRuint length, const TRubyte* data, TRenum type)
{
    if (event)
    {
        return triDefineSimpleArrayData(event->state, pointer, length, data, type);
    }
    return TR_FALSE;
}

void trForgetPointerData(TREvent* event, const void* pointer)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        triDefineSimpleArrayData(event->state, pointer, 0, 0, TR_VOID);
    }
}

static void _trMaybeSerializeObject(TREvent* event, const TRClass* cls, TRhandle ns, TRhandle handle)
{
    TRhandle regHandle = 0;

    /* Null objects don't need to be serialized */
    if (!handle)
    {
        return;
    }

    /* See whether this object is already registered */
    regHandle = triGetObject(event->state, cls->id, ns, handle, 0);
    
    /* Serialize the object if we just saw it or it can be modified */
    if (!regHandle || cls->modify)
    {
        /* Eat the error state */
        if (!regHandle)
        {
            TR_ASSERT(trGetError() == TR_INVALID_HANDLE);
        }

        /* Serialize the object */
        TR_ASSERT(triSerializeObject(event->state, cls->id, ns, handle));
    }
}

void trObjectValue(TREvent* event, const TRbyte* name, TRenum cls, TRhandle handle)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        const TRClass* objectClass = triGetClass(event->state, cls);
        
        TR_ASSERT_ERROR(event->state, objectClass, TR_BAD_VALUE);
        
        if (event->state->openEvents == 1)
        {
            TRhandle ns = 0;
            TRObject object;

            /* Get the currently active namespace */
            if (objectClass->nsPath)
            {
                TRStateGroup* root = triGetApiState(event->state, event->api);
                ns = (TRhandle)triGetStateValue(event->state, root, objectClass->nsPath);
            }

            _trMaybeSerializeObject(event, objectClass, ns, handle);
            
            /* Write out the object handle along with the class specifier */
            object.cls    = objectClass;
            object.ns     = ns;
            object.handle = handle;
            TR_ASSERT(triObjectValue(event->state, name, &object));
        }
    }
}

void trObjectArrayValue(TREvent* event, const TRbyte* name, TRuint size, TRenum cls, const TRhandle* array)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        const TRClass* objectClass = triGetClass(event->state, cls);
        
        TR_ASSERT_ERROR(event->state, objectClass, TR_BAD_VALUE);
        
        if (event->state->openEvents == 1)
        {
            TRhandle ns = 0;
            int i;

            /* Get the currently active namespace */
            if (objectClass->nsPath)
            {
                TRStateGroup* root = triGetApiState(event->state, event->api);
                ns = (TRhandle)triGetStateValue(event->state, root, objectClass->nsPath);
            }

            for (i = 0; i < size; i++)
            {
                _trMaybeSerializeObject(event, objectClass, ns, array[i]);
            }

            /* Write out the array of object handles along with class and namespace information */
            triDefineObjectArrayData(event->state, size, objectClass, ns, array);
            trPointerValue(event, name, array);
        }
    }
}

static TRbool _trReadObjectValue(TREvent* event, const TRbyte* name, TRObject* object)
{
    TR_FOR_EACH(TRStreamDecoder*, dec, triGetDecoders(event->state))
    {
        if (dec->ops->readObjectValue(dec, name, object) == TR_NO_ERROR)
        {
            return TR_TRUE;
        }
    }
    return TR_FALSE;
}

TRhandle trGetObjectValue(TREvent* event, const TRbyte* name)
{
    TR_ASSERT(event);
    {
        TRObject object;
        if (_trReadObjectValue(event, name, &object))
        {
            TR_ASSERT(object.cls);
            return object.handle;
        }
#if defined(TRACER_PLAYER_VERBOSE_LOGGING)
        triLogMessage(event->state, "Unable to read object value.\n");
#endif
        return 0;
    }
}

void trRegisterApi(TRenum api, TRuint functionCount,
                   const TRFunction* functions)
{
    TRState* state = triGetState();
    triRegisterApi(state, api, functionCount, functions);
}

TRenum trGetError(void)
{
    TRState* state = triGetState();
    TRenum error = state->error;
    state->error = TR_NO_ERROR;
    return error;
}

static void _trOpenDefaultOutputStream(const TRbyte* apiConf)
{
    TRState* state = triGetState();

    /* If no streams were created, open the default stream */
    if (!triArraySize(state->streams))
    {
        TRbyte* device = triGetDefaultStreamDevice(state);
        TRbyte* outputPath = triGetDefaultTraceOutputPath(state);
        TRbyte* outputType = triGetDefaultTraceOutputType(state);
        TRbyte* traceFileName = 0;
        const TRbyte* extension = triStringsEqual(outputType, "binary") ? "bin" : "log";
        const TRbyte* format = triStringsEqual(outputType, "text") ? "ascii" : outputType;

        TR_ASSERT(device && outputPath);

        traceFileName = triStringFormat(state, "%s", outputPath);
        trOpenOutputStream(device, format, traceFileName, extension, 0);

        triFree(state, traceFileName);
        triFree(state, device);
        triFree(state, outputPath);
        triFree(state, outputType);
    }
}

void trOpenOutputStream(const TRbyte* device, const TRbyte* codec, const TRbyte* baseName, const TRbyte* extension,
                        const TRStrDict* options)
{
    TRState* state = triGetState();
    TR_ASSERT_ERROR(state, device,    TR_BAD_VALUE);
    TR_ASSERT_ERROR(state, codec,     TR_BAD_VALUE);
    TR_ASSERT_ERROR(state, baseName,  TR_BAD_VALUE);
    TR_ASSERT_ERROR(state, extension, TR_BAD_VALUE);
    {
        TRbyte*   path;
        TRbyte    procName[128];
        TRStream* stream;

        triGetProcessName(procName, sizeof(procName));
        
        path = triStringFormat(state, "%stracer_%s.%s", baseName, procName, extension);
        TR_ASSERT_ERROR(state, path, TR_OUT_OF_MEMORY);
        
        /* 
         *  If we aren't allowed to overwrite an existing file, check that this 
         *  file does not exist.
         */
        if (!trGetConfigInteger("tracer", "overwrite", 0))
        {
            TRint variant = 1;
            
            for (variant = 1; ; variant++)
            {
                TRFile file = triOpenFile(state, path, "r");
                if (!file)
                {
                    break;
                }
                triCloseFile(file);
                triFree(state, path);
                path = triStringFormat(state, "%stracer_%s_%03d.%s", baseName, procName, variant, extension);
                TR_ASSERT_ERROR(state, path, TR_OUT_OF_MEMORY);
            }
        }

        stream = triOpenStream(state, device, path, "wb", options);
        if (stream)
        {
            triAttachEncoder(state, stream, codec, options);
            triLogMessage(state, "Opened %s %s trace file %s", device, codec, path);
        }
        triFree(state, path);
    }
}

void trOpenInputStream(const TRbyte* device, const TRbyte* codec, const TRbyte* name,
                       const TRStrDict* options)
{
    TRState* state = triGetState();
    TR_ASSERT_ERROR(state, device,    TR_BAD_VALUE);
    TR_ASSERT_ERROR(state, codec,     TR_BAD_VALUE);
    TR_ASSERT_ERROR(state, name,      TR_BAD_VALUE);
    {
        TRStream* stream;

        stream = triOpenStream(state, device, name, "rb", options);
        if (stream)
        {
            triAttachDecoder(state, stream, codec, options);
        }
    }
}

void trTerminate(void)
{
    /* Don't trigger a reinitialization when we only need to terminate */
    if (triGetGlobal())
    {
        TRState* state = triGetState();
        const TRArray* encoders = triGetEncoders(state);
        const TRArray* decoders = triGetDecoders(state);
        const TRArray* streams  = triGetStreams(state);
        
        while (triArraySize(encoders))
        {
            TR_ASSERT(triDetachEncoder(state, triArrayGet(encoders, 0)));
        }
    
        while (triArraySize(decoders))
        {
            TR_ASSERT(triDetachDecoder(state, triArrayGet(decoders, 0)));
        }
    
        while (triArraySize(streams))
        {
            TR_ASSERT(triCloseStream(state, triArrayGet(streams, 0)));
        }
        triDestroyState(state);
    }
}

void trSetStateValue(TREvent* event, const TRbyte* path, void* value)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        TRState*     state = event->state;
        TRStateGroup* root = triGetApiState(state, event->api);
        triSetStateValue(state, root, path, value, TR_FALSE);
    }
}

void trSetStateValueArray(TREvent* event, const TRbyte* path, TRuint length, const TRubyte* data)
{
    TR_ASSERT_ERROR(triGetState(), event,  TR_BAD_VALUE);
    {
        TRState*     state  = event->state;
        TRStateGroup* root  = triGetApiState(state, event->api);
        TRubyte* copiedData = 0;
        
        /* Make a copy of the data */
        if (length)
        {
            TRuint i;
            copiedData = triMalloc(state, length * sizeof(TRubyte));
            
            TR_ASSERT_ERROR(triGetState(), copiedData, TR_OUT_OF_MEMORY);
            
            for (i = 0; i < length; i++)
            {
                copiedData[i] = data[i];
            }
        }
        triSetStateValue(state, root, path, copiedData, TR_TRUE);
    }
}

void* trGetStateValue(TREvent* event, const TRbyte* path)
{
    TR_ASSERT_ERROR_RETURN(triGetState(), event, TR_BAD_VALUE, 0);
    {
        TRState*     state = event->state;
        TRStateGroup* root = triGetApiState(state, event->api);
        return triGetStateValue(state, root, path);
    }
}

void* trGetSpecificStateValue(TREvent* event, const TRbyte* path, const TRint* keys)
{
    TR_ASSERT_ERROR_RETURN(triGetState(), event, TR_BAD_VALUE, 0);
    {
        TRState*     state = event->state;
        TRStateGroup* root = triGetApiState(state, event->api);
        return triGetSpecificStateValue(state, root, path, keys);
    }
}

void trRemoveStateValue(TREvent* event, const TRbyte* path)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        TRState*     state = event->state;
        TRStateGroup* root = triGetApiState(state, event->api);
        triRemoveStateValue(state, root, path);
    }
}

void trBeginCall(TREvent* event)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        if (event->state->synchronousOperation)
        {
            trFlush();
        }
        event->callTime = triGetTimeStamp(event->state);
    }
}

void trEndCall(TREvent* event)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        event->callTime = triGetTimeStamp(event->state) - event->callTime;
    }
}

static TRbool _trLoadConfig(const TRbyte* configFileName)
{
    TRState*  state = triGetState();
    TRint i;
    TRbyte* configFilePath = triFindConfigFile(state, configFileName);

    if (!configFilePath)
    {
        triLogMessage(state, "Unable to find configuration file '%s'.", configFileName);
        return TR_FALSE;
    }
    
    if (!triLoadConfig(state, configFilePath))
    {
        triLogMessage(state, "Unable to read configuration file '%s'.", configFilePath);
        triFree(state, configFilePath);
        return TR_FALSE;
    }
    triFree(state, configFilePath);

    /* Load the global tracer settings */
    state->synchronousOperation = trGetConfigInteger("tracer", "sync", state->synchronousOperation);
    state->pointerDataSizeLimit = trGetConfigInteger("tracer", "array_cache_limit", 8 * 1024) * 1024;
    state->synchronousPlayback  = trGetConfigInteger("tracer", "sync_playback", 0);
    
    /* Open the configured streams */
    for (i = 1; i <= 8; i++)
    {
        TRbyte* streamName = triStringFormat(state, "stream%d", i);
        const TRbyte* type = trGetConfigString(streamName, "type", 0);

        TR_ASSERT_ERROR_RETURN(state, streamName, TR_OUT_OF_MEMORY, TR_FALSE);
                
        if (type)
        {
            if (triStringsEqual(type, "output"))
            {
                const TRbyte* device    = trGetConfigString(streamName, "device",    "stdio");
                const TRbyte* codec     = trGetConfigString(streamName, "codec",     "ascii");
                const TRbyte* path      = trGetConfigString(streamName, "path",      "./");
                const TRbyte* extension = trGetConfigString(streamName, "extension", "log");
            
                if (device && codec && path && extension)
                {
                    trOpenOutputStream(device, codec, path, extension, triGetConfigSection(state, streamName));
                }
                else
                {
                    triLogMessage(state, "Incomplete stream specification for stream %d.", i);
                }
            }
            else if (triStringsEqual(type, "input"))
            {
                const TRbyte* device    = trGetConfigString(streamName, "device",    "stdio");
                const TRbyte* codec     = trGetConfigString(streamName, "codec",     "binary");
                const TRbyte* path      = trGetConfigString(streamName, "path",      0);
            
                if (device && codec && path)
                {
                    trOpenInputStream(device, codec, path, triGetConfigSection(state, streamName));
                }
                else
                {
                    triLogMessage(state, "Incomplete stream specification for stream %d.", i);
                }
            }
            else
            {
                triLogMessage(state, "Stream type '%s' not supported.", type);
            }
        }
        triFree(state, streamName);
    }
    /* Eat the possible TR_NOT_FOUND error status from dict */
    trGetError();
    return TR_TRUE;
}

void trLoadConfig(const TRbyte* apiConf)
{
    TRState* state = triGetState();
    TRbyte* configFile = triStringFormat(state, "tracer_%s.ini", apiConf);

    if (!_trLoadConfig(configFile))
    {
       _trLoadConfig("tracer.ini"); 
    }
    triFree(state, configFile);
    _trOpenDefaultOutputStream(apiConf);
}

const TRbyte* trGetConfigString(const TRbyte* section, const TRbyte* option, const TRbyte* dfl)
{
    TRState*      state = triGetState();
    const TRbyte* value = triGetConfigString(state, section, option, dfl);
#if defined(TRACER_VERBOSE_CONFIG_LOGGING)
    triLogMessage(state, "Tracer config: %s.%s = %s", section, option, value);
#endif
    return value;
}

TRint trGetConfigInteger(const TRbyte* section, const TRbyte* option, TRint dfl)
{
    TRState*      state = triGetState();
    TRint         value = triGetConfigInteger(state, section, option, dfl);
#if defined(TRACER_VERBOSE_CONFIG_LOGGING)
    triLogMessage(state, "Tracer config: %s.%s = %d", section, option, value);
#endif
    return value;
}

const TRStrDict* trGetConfigSection(const TRbyte* section)
{
    TRState*      state = triGetState();
    return triGetConfigSection(state, section);
}

void trFlush(void)
{
    TRState*      state = triGetState();
    TR_FOR_EACH(TRStream*, stream, triGetStreams(state))
    {
        if (stream->ops->flush)
        {
            stream->ops->flush(stream);
        }
    }
}

TREvent* trGetEvent(void)
{
    TRState* state = triGetState();
    return triGetEvent(state);
}

TREvent* trPlaySingleEvent(TRPlayer player)
{
    TRState* state = triGetState();
    state->player = player;
    
    TR_ASSERT_ERROR_RETURN(triGetState(), triArraySize(triGetDecoders(state)), TR_INVALID_OPERATION, 0);
    {
        TREvent* event = triGetEvent(state);
        
        if (!event)
        {
            triSetError(state, TR_END_OF_FILE);
            return 0;
        }
            
        if (!event->function || !event->function->eventHandler)
        {
#if defined(TRACER_VERBOSE_LOGGING)
            triLogMessage(state, "Unknown event encountered.");
#endif
            triSetError(state, TR_NOT_FOUND);
            return 0;
        }
            
        /* Call the handler */
        event->function->eventHandler(event);
        
        /* 
         *  If we should synchronize playback and this is a frame swap, sleep 
         *  until the frame takes enough time.
         */
        if (state->synchronousPlayback && 
            event->function->flags & TR_FUNC_FRAME_MARKER)
        {
            TRtimestamp now    = triGetTimeStamp(state);
            TRtimestamp target = event->timestamp + event->callTime;
            TRtimestamp diff   = (target - state->indicatedFrameStartTime) - (now - state->measuredFrameStartTime);
            
            /* Sleep to reset the difference */
            if (state->indicatedFrameStartTime && diff > 0)
            {
                triSleep(diff);
            }
            
            state->measuredFrameStartTime  = now;
            state->indicatedFrameStartTime = target;
        }
        
        return event;
    }
}

TRenum trPlay(TRPlayer player)
{
    TRState* state = triGetState();
    
    TR_ASSERT_ERROR_RETURN(state, triArraySize(triGetDecoders(state)), TR_INVALID_OPERATION, TR_INVALID_OPERATION);
    {
        TREvent* event;
        do
        {
            event = trPlaySingleEvent(player);
        } while (event);
        return trGetError();
    }
}

void trRegisterObject(TREvent* event, TRenum cls, TRhandle ns, TRhandle traceHandle, TRhandle handle)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        triRegisterObject(event->state, cls, ns, traceHandle, handle);
    }
}

void trRegisterObjectAsEventValue(TREvent* event, const TRbyte* name, TRhandle handle)
{
    TR_ASSERT_ERROR(triGetState(), event, TR_BAD_VALUE);
    {
        /* Read the event value that was referred to */
        TRObject object;
        
        if (_trReadObjectValue(event, name, &object))
        {
            TR_ASSERT(object.cls);
            trRegisterObject(event, object.cls->id, object.ns, object.handle, handle);
        }
    }
}

void trRegisterClass(TRenum cls, const TRbyte* name, const TRbyte* namespacePath, TRObjectConstructor c, 
                     TRObjectDestructor d, TRObjectSerializer s,
                     TRObjectModifier m)
{
    TR_ASSERT_ERROR(triGetState(), name, TR_BAD_VALUE);
    {
        triRegisterClass(triGetState(), cls, name, namespacePath, c, d, s, m);
    }
}

TRenum trGetClassId(const TRbyte* name)
{
    TRState* state = triGetState();
    TR_ASSERT_ERROR_RETURN(state, name, TR_BAD_VALUE, 0);
    {
        TRClass* c = triGetClassByName(state, name);
        if (c)
        {
            return c->id;
        }
        triSetError(state, TR_NOT_FOUND);
        return 0;
    }
}

void* trMalloc(TRuint bytes)
{
    return triMalloc(triGetState(), bytes);
}

void trFree(void* data)
{
    triFree(triGetState(), data);
}

TRPlayer trGetPlayer(void)
{
    return triGetState()->player;    
}

TRFunction* trLookupFunction(TRenum api, const TRbyte* name)
{
    TRState* state = triGetState();
    return triLookupFunction(state, api, name);
}

TRFunction* trLookupFunctionByIndex(TRenum api, TRuint index)
{
    TRState* state = triGetState();
    return triLookupFunctionByIndex(state, api, index);
}

TRfloat trGetFpsEstimate(void)
{
    TRState* state = triGetState();
    TR_ASSERT(state);
    return state->fpsEstimate;
}

void trPanic(const TRbyte* reason)
{
    triLogMessage(triGetState(), "Tracer Panic: %s", reason);
    TR_ASSERT(0);
}

TRtimestamp trGetSystemTime(void)
{
    TRState* state = triGetState();
    TR_ASSERT(state);
    return triGetSystemTime(state);
}

void* trLookupFunctionPointerByIndex(TRuint apiAndIndex)
{
    TRenum api = (apiAndIndex >> 16);
    TRint index = apiAndIndex & 0xffff;
    struct TRStateStruct* state = triGetState();
    TRFunction* function = triLookupFunctionByIndex(state, api, index);
    TR_ASSERT(function);
    TR_ASSERT(function->pointer);
    return function->pointer;
}

#if defined(_MSC_VER) || defined(__WINSCW__)
/*
 *  Main x86 passthrough dispatch routine.
 *
 *  @param eax   high word: API index (zero based)
 *                low word: function index (zero-based)
 */
TR_NAKED void trDispatchPassthrough(void)
{
    __asm
	{
            push eax
            call trLookupFunctionPointerByIndex         // Get the function pointer
            add esp, 0x4
            jmp eax                                     // Jump into the function
	}
}
#elif !defined(__arm__) && defined(__GNUC__)
/*
 *  Main x86 passthrough dispatch routine for GCC.
 *
 *  @param eax   high word: API index (zero based)
 *                low word: function index (zero-based)
 */
TR_NAKED void trDispatchPassthrough(void)
{
    __asm__ (
        "push %eax\n\t"
        "call trLookupFunctionPointerByIndex\n\t"    // Get the function pointer
        "add $0x4, %esp\n\t"
        "jmp *%eax\n\t"                              // Jump into the function
    ); 
}
#elif defined(__arm__) && defined(__ARMCC__)
/*
 *  Main ARM passthrough dispatch routine.
 *
 *  @param r12   high word: API index (zero based)
 *                low word: function index (zero-based)
 */
TR_NAKED void trDispatchPassthrough(void)
{
    // can use: r0-r3, r12, lr
    // must pass to real func: r0-r3, sp
    stmfd sp!, {r0-r3, r10, r14}                    // Dummy r10 to preserve aligment
    mov   r0, r12
    bl    __cpp(trLookupFunctionPointerByIndex)     // Get the function pointer
    mov   r12, r0
    ldmfd sp!, {r0-r3, r10, r14}
    bx    r12                                       // Jump to the function
}
#else
void trDispatchPassthrough(void)
{
    TR_ASSERT(0);
}
#endif
