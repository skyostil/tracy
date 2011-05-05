/**
 *  Internal Tracer API interface.
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
#ifndef TRACER_INTERNAL_H
#define TRACER_INTERNAL_H

#include "tracer.h"
#include "tracer_player.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Macros */
#define TR_NO_STATE           ((TRState*)0)
#define TR_MAX_OPEN_EVENTS	  4

/* Type definitions to ease structure referencing */
typedef struct TRStateStruct                    TRState;
typedef struct TRStreamOperationsStruct         TRStreamOperations;
typedef struct TRStreamDeviceOperationsStruct   TRStreamDeviceOperations;
typedef struct TRStreamEncoderOperationsStruct  TRStreamEncoderOperations;
typedef struct TRStreamDecoderOperationsStruct  TRStreamDecoderOperations;
typedef struct TRStreamCodecOperationsStruct    TRStreamCodecOperations;
typedef struct TRApiStruct                      TRApi;
typedef struct TRPointerInfoStruct              TRPointerInfo;
typedef struct TRClassStruct                    TRClass;
typedef struct TRClassNamespaceStruct           TRClassNamespace;
typedef struct TRObjectStruct                   TRObject;

/** Stream device handle */
typedef struct
{
    TRhandle                            handle;
    const TRStreamDeviceOperations*     ops;
} TRStreamDevice;

/** Stream handle */
typedef struct
{
    TRhandle                            handle;
    TRStreamDevice*                     device;
    const TRStreamOperations*           ops;
} TRStream;

/** Stream operations */
struct TRStreamOperationsStruct
{
    TRuint      (*read) (TRStream* stream, TRbyte*       data, TRuint length);
    TRuint      (*write)(TRStream* stream, const TRbyte* data, TRuint length);
    void        (*flush)(TRStream* stream);
};

/** Stream device operations */
struct TRStreamDeviceOperationsStruct
{
    TRenum      (*init) (TRStreamDevice* dev);
    TRenum      (*exit) (TRStreamDevice* dev);
    TRenum      (*open) (TRStreamDevice* dev, TRStream* stream, const TRbyte* name, const TRbyte* mode, const TRStrDict* options);
    TRenum      (*close)(TRStreamDevice* dev, TRStream* stream);
};

/** Stream codec handle */
typedef struct
{
    TRhandle                            handle;
    const TRStreamCodecOperations*      ops;
} TRStreamCodec;

/** Stream encoder handle */
typedef struct
{
    TRhandle                            handle;
    TRStream*                           stream;
    TRStreamCodec*                      codec;
    const TRStreamEncoderOperations*    ops;
} TRStreamEncoder;

/** Stream decoder handle */
typedef struct
{
    TRhandle                            handle;
    TRStream*                           stream;
    TRStreamCodec*                      codec;
    const TRStreamDecoderOperations*    ops;
} TRStreamDecoder;

/** Stream codec operations */
struct TRStreamEncoderOperationsStruct
{
    TRenum      (*beginEvent)       (TRStreamEncoder* encoder, const TREvent* event);
    TRenum      (*endEvent)         (TRStreamEncoder* encoder, const TREvent* event);
    TRenum      (*byteValue)        (TRStreamEncoder* encoder, const TRbyte* name, TRubyte value);
    TRenum      (*shortValue)       (TRStreamEncoder* encoder, const TRbyte* name, TRshort value);
    TRenum      (*integerValue)     (TRStreamEncoder* encoder, const TRbyte* name, TRint value);
    TRenum      (*longValue)        (TRStreamEncoder* encoder, const TRbyte* name, TRlong value);
    TRenum      (*floatValue)       (TRStreamEncoder* encoder, const TRbyte* name, TRfloat value);
    TRenum      (*doubleValue)      (TRStreamEncoder* encoder, const TRbyte* name, TRdouble value);
    TRenum      (*pointerValue)     (TRStreamEncoder* encoder, const TRbyte* name, const void* value);
    TRenum      (*objectValue)      (TRStreamEncoder* encoder, const TRbyte* name, const TRObject* value);
    TRenum      (*definePointerData)(TRStreamEncoder* encoder, const void* pointer, TRuint size, const TRubyte* data,
                                     TRenum type, const TRClass* cls, TRhandle ns);
    TRenum      (*beginObject)      (TRStreamEncoder* encoder, const TRClass* cls, TRhandle ns, TRhandle handle);
    TRenum      (*endObject)        (TRStreamEncoder* encoder);
};

/** Stream codec operations */
struct TRStreamDecoderOperationsStruct
{
    TRenum      (*getEvent)         (TRStreamDecoder* encoder, TREvent* event);
    TRenum      (*readByteValue)    (TRStreamDecoder* encoder, const TRbyte* name, TRubyte* value);
    TRenum      (*readShortValue)   (TRStreamDecoder* encoder, const TRbyte* name, TRshort* value);
    TRenum      (*readIntegerValue) (TRStreamDecoder* encoder, const TRbyte* name, TRint* value);
    TRenum      (*readLongValue)    (TRStreamDecoder* encoder, const TRbyte* name, TRlong* value);
    TRenum      (*readFloatValue)   (TRStreamDecoder* encoder, const TRbyte* name, TRfloat* value);
    TRenum      (*readDoubleValue)  (TRStreamDecoder* encoder, const TRbyte* name, TRdouble* value);
    TRenum      (*readPointerValue) (TRStreamDecoder* encoder, const TRbyte* name, void** value);
    TRenum      (*readObjectValue)  (TRStreamDecoder* encoder, const TRbyte* name, TRObject* value);
};

/** Stream codec operations */
struct TRStreamCodecOperationsStruct
{
    TRenum      (*init)         (TRStreamCodec* codec);
    TRenum      (*exit)         (TRStreamCodec* codec);
    TRenum      (*attachEncoder)(TRStreamCodec* codec, TRStreamEncoder* encoder, const TRStrDict* options);
    TRenum      (*detachEncoder)(TRStreamCodec* codec, TRStreamEncoder* encoder);
    TRenum      (*attachDecoder)(TRStreamCodec* codec, TRStreamDecoder* decoder, const TRStrDict* options);
    TRenum      (*detachDecoder)(TRStreamCodec* codec, TRStreamDecoder* decoder);
};

/*
 *  The API state tracking data structure is a hierarchical structure composed of
 *  groups and nodes. Each group contains a list of nodes, while each node contains
 *  a dictionary of values with integer keys, and the values pointing to either
 *  groups or concrete state values. Additionally, each node has a default value which
 *  points to one of its values.
 *
 *  The state nodes are addressed with paths. Each path is a zero terminated string of
 *  bytes at least one byte long, where each byte encodes one level of the state tree
 *  as follows:
 *
 *      bit 87654321 (hi -> lo)
 *          --------
 *          TIIIIIII
 *          |      |
 *          |       '-- (index + 1) of the node in the corresponding group
 *           '--------- terminator flag (1 if component is a terminator, 0 otherwise)
 *          
 *
 *  There are two kinds of state values: normal values and terminal values. Normal values
 *  are plain integers that are used in nodes. Terminal values are found at the outermost
 *  leaves of the state tree and may be either integral values or arbitrary allocated
 *  buffers.
 * 
 */

/** API state tracking terminal value */
typedef struct
{
    TRbool        allocated;
    void*         data;
} TRStateTerminalValue;

/** API state tracking group */
typedef struct
{
    TRArray*      nodes;
    TRArray*      values;
} TRStateGroup;

/** API state tracking node */
typedef struct
{
    TRIntDict*    keys;
    void*         current;
    void*         value;
} TRStateNode;

/** API information */
struct TRApiStruct
{
    TRenum        id;
    TRuint        functionCount;
    TRFunction*   functions;
    TRStrDict*    libraryHandles;
    TRStateGroup* state;
    
    /** Fast function lookup by name */
    TRStrDict*    functionsByName;
    
    /** A dynamically generated function */
    TRFunction    dynamicFunction;
    
    /** Name of the dynamically generated function */
    TRbyte        dynamicFunctionName[128];
};

/** Pointer array tracking information */
struct TRPointerInfoStruct
{
    TRuint        length;
    TRuint        allocLength;
    TRuint        updateCount;
    TRbyte*       data;
};

/** A structure for an object instance */
struct TRObjectStruct
{
    /** Object handle */
    TRhandle            handle;

    /** Namespace to which this instance belongs to */
    TRhandle            ns;

    /** Class to which this object belongs */
    const TRClass*      cls;
};


/** A class namespace */
struct TRClassNamespaceStruct
{
    /** All registered objects of this namespace */
    TRIntDict*          objects;
};

/** A class structure for object serialized to the trace stream */
struct TRClassStruct
{
    /** Class name */
    TRbyte*             name;

    /** Class id */
    TRenum              id;

    /** Namespace path */
    TRbyte*             nsPath;
  
    /** 
     *  The object constructor creates an object from scratch, 
     *  initializing it with the serialized data read from the
     *  event stream.
     */
    TRObjectConstructor create;
    
    /**
     *  The destructor releases all resources associated with
     *  an object.
     */
    TRObjectDestructor  destroy;
    
    /**
     *  The serializer saves all relevant object attributes to the
     *  event stream. The serialized format is later consumed by
     *  the object constructor and modifier.
     */
    TRObjectSerializer  serialize;
    
    /**
     *  The object modifier changes the attributes of a live object
     *  to match those read from the event stream. Not all objects
     *  support live modification.
     */
    TRObjectModifier    modify;
    
    /** All registered namespaces for this class */
    TRIntDict*          namespaces;
};

/** Tracer state */
struct TRStateStruct
{
    /** Registered stream devices */
    TRStrDict*  streamDevices;

    /** Open streams */
    TRArray*    streams;
    
    /** Registered stream codecs */
    TRStrDict*  codecs;
    
    /** Attached encoders and decoders */
    TRArray*    encoders;
    TRArray*    decoders;
    
    /** Registered APIs */
    TRArray*    apis;

    /** Error code */
    TRenum      error;

    /** Number of open events */
    TRuint      openEvents;

    /** Timestamp of the first event */
    TRtimestamp baseTime;
    
    /** Pointer info storage */
    TRDict*     pointers;
    
    /** Number of bytes allocated by the array cache */
    TRuint      pointerDataSize;

    /** How many bytes to allow for the array data cache */
    TRuint      pointerDataSizeLimit;
    
    /** CRC32 lookup table */
    TRint*      crcTable;
    
    /** Platform-dependant data */
    TRhandle    platformData;
    
    /** Currently active events */
    TREvent     eventStack[TR_MAX_OPEN_EVENTS];
    
    /** Runtime configuration */
    TRStrDict*  config;
    
    /** Synchronous operation flag, causes a stream flush before each call and after each event */
    TRbool      synchronousOperation;
    
    /** Registered object classes by id */
    TRIntDict*  classes;
    
    /** Registered object classes by name */
    TRStrDict*  classesByName;
    
    /** Trace player instance */
    TRPlayer    player;
    
    /** FPS estimatation starting time */
    TRtimestamp fpsStartTime;
    
    /** FPS estimation frame count */
    TRuint      fpsFrameCount;
    
    /** Most recent FPS estimate */
    TRfloat     fpsEstimate;
    
    /** Start time for currently played frame as measured */
    TRtimestamp measuredFrameStartTime;

    /** Start time for currently played frame as indicated in the trace file */
    TRtimestamp indicatedFrameStartTime;
    
    /** 
     *  Should playback event frequency be synchronized 
     *  as specified in the trace file?
     */
    TRbool      synchronousPlayback;
};

/** A variant union that can hold any supported atomic type */
typedef union
{
    TRbyte    b;
    TRshort   s;
    TRint     i;
    TRlong    l;
    TRfloat   f;
    TRdouble  d;
    void*     p;
    TRObject  o;
} TRVariant;

/* Internal functions */
TRState*         triGetState(void);
TRState*         triGetStateWithoutUserInit(void);
void             triDestroyState(TRState* state);
void             triSetError(TRState* state, TRenum error);
TRbool           triRegisterStreamDevice(TRState* state, const TRbyte* name, TRStreamDevice* dev);
TRbool           triRegisterStreamCodec(TRState* state, const TRbyte* name, TRStreamCodec* codec);
TRbool           triRegisterApi(TRState* state, TRenum id, TRuint functionCount,
                                const TRFunction* functions);
TRFunction*      triLookupFunction(TRState* state, TRenum api, const TRbyte* name);
TRFunction*      triLookupFunctionByIndex(TRState* state, TRenum api, TRuint index);
TRStream*        triOpenStream(TRState* state, const TRbyte* device, const TRbyte* name,
                               const TRbyte* mode, const TRStrDict* options);
TRbool           triCloseStream(TRState* state, TRStream* stream);
TRStreamEncoder* triAttachEncoder(TRState* state, TRStream* stream, const TRbyte* codec, const TRStrDict* options);
TRStreamDecoder* triAttachDecoder(TRState* state, TRStream* stream, const TRbyte* codec, const TRStrDict* options);
TRbool           triDetachEncoder(TRState* state, TRStreamEncoder* encoder);
TRbool           triDetachDecoder(TRState* state, TRStreamDecoder* decoder);
const TRArray*   triGetStreams(TRState* state);
const TRArray*   triGetEncoders(TRState* state);
const TRArray*   triGetDecoders(TRState* state);
TRtimestamp      triGetTimeStamp(TRState* state);
TREvent*         triBeginEvent(TRState* state, TRenum api, const TRFunction* function);
void             triEndEvent(TREvent* event);
TRbool           triDefineSimpleArrayData(TRState* state, const void* pointer, TRuint length,
                                          const TRubyte* data, TRenum type);
TRbool           triDefineObjectArrayData(TRState* state, TRuint length, const TRClass* cls, TRhandle ns,
                                          const TRhandle* array);
TREvent*         triGetEvent(TRState* state);

/* State tracking */
TRStateGroup*    triGetApiState(TRState* state, TRenum apiId);
void             triSetStateValue(TRState* state, TRStateGroup* root, const TRbyte* path, void* value, TRbool allocated);
void*            triGetStateValue(TRState* state, TRStateGroup* root, const TRbyte* path);
void*            triGetSpecificStateValue(TRState* state, TRStateGroup* root, const TRbyte* path, const TRint* keys);
void             triRemoveStateValue(TRState* state, TRStateGroup* root, const TRbyte* path);

/* Runtime configuration */
TRbool           triLoadConfig(TRState* state, const TRbyte* path);
const TRbyte*    triGetConfigString(TRState* state, const TRbyte* section, const TRbyte* option, const TRbyte* dfl);
TRint            triGetConfigInteger(TRState* state, const TRbyte* section, const TRbyte* option, TRint dfl);
const TRStrDict* triGetConfigSection(TRState* state, const TRbyte* section);

/* Object management */
TRbool           triRegisterObject(TRState* state, TRenum cls, TRhandle ns, TRhandle traceHandle, TRhandle handle);
TRbool           triRegisterClass(TRState* state, TRenum cls, const TRbyte* name, const TRbyte* namespacePath, TRObjectConstructor c, TRObjectDestructor d, TRObjectSerializer s, TRObjectModifier m);
TRhandle         triGetObject(TRState* state, TRenum cls, TRhandle ns, TRhandle handle, TRhandle defaultHandle);
TRbool           triSerializeObject(TRState* state, TRenum cls, TRhandle ns, TRhandle handle);
TRhandle         triUnserializeObject(TRState* state, TRenum cls, TRhandle ns, TRhandle handle);
TRbool           triObjectValue(TRState* state, const TRbyte* name, const TRObject* object);
TRClass*         triGetClassByName(TRState* state, const TRbyte* name);
TRClass*         triGetClass(TRState* state, TRenum cls);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // TRACER_INTERNAL_H
