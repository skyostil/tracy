/**
 *  Generic Tracer API interface.
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
#ifndef TRACER_H
#define TRACER_H

#include "tracer_compiler.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Type definitions to ease structure referencing */
typedef struct TRDictStruct                     TRDict;
typedef struct TRDictStruct                     TRStrDict;
typedef struct TRDictStruct                     TRIntDict;
typedef struct TRArrayStruct                    TRArray;
typedef struct TRFunctionStruct                 TRFunction;
typedef struct TREventStruct                    TREvent;

/* TODO: This is not supposed to be here */
#if defined(__SYMBIAN32__)
typedef int    NativeDisplayType;
typedef NativeDisplayType EGLNativeDisplayType;
#endif /* __SYMBIAN32__ */

/* Type declarations */
typedef int                     TRenum;
typedef int                     TRint;
typedef unsigned int            TRuint;
typedef char                    TRbyte;
typedef unsigned char           TRubyte;
typedef short                   TRshort;
typedef float                   TRfloat;
typedef unsigned int            TRbool;
typedef TR_INT64_TYPE           TRlong;
typedef unsigned TR_INT64_TYPE  TRulong;

typedef TRlong                  TRtimestamp;    /* time value in microseconds */
typedef double                  TRdouble;
typedef void*                   TRhandle;

/* Type enumerations */
#define TR_VOID                 0x0
#define TR_BYTE                 0x1
#define TR_SHORT                0x2
#define TR_INTEGER              0x3
#define TR_LONG                 0x4
#define TR_FLOAT                0x5
#define TR_DOUBLE               0x6
#define TR_OBJECT               0x7

/* Constants */
#define TR_TRUE                 (1==1)
#define TR_FALSE                (1==0)
#define TR_NO_ERROR             0x0
#define TR_OUT_OF_MEMORY        0xf000
#define TR_NOT_FOUND            0xf001
#define TR_INVALID_HANDLE       0xf002
#define TR_BAD_VALUE            0xf003
#define TR_BAD_NAME             0xf004
#define TR_INVALID_OPERATION    0xf005
#define TR_END_OF_FILE          0xf006

/* Unreference a function parameter to silence compiler warnings */
#define TR_UNUSED(x) ((void)(x))

/* Macros */
#define TR_NO_EVENT  ((TREvent*)0)

#define TR_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TR_MAX(a, b) ((a) > (b) ? (a) : (b))

/** User-supplied startup function */
extern void trInitialize(void);

/** Event handler function signature */
typedef void (*TREventHandler)(TREvent* event);

/** Object constructor function signature */
typedef TRhandle (*TRObjectConstructor)(TREvent* event);

/** Object destuctor function signature */
typedef void (*TRObjectDestructor)(TRhandle handle);

/** Object serializer function signature */
typedef void (*TRObjectSerializer)(TREvent* event, TRhandle handle);

/** Object modifier function signature */
typedef void (*TRObjectModifier)(TREvent* event, TRhandle handle);

typedef enum
{
    TR_FUNC_FRAME_MARKER    = 0x1,
    TR_FUNC_RENDER_CALL     = 0x2,
    TR_FUNC_TERMINATOR      = 0x4,
} TRFunctionFlags;

/** A structure that holds function specific attributes */
struct TRFunctionStruct
{
    /** Name of this function */
    const TRbyte*           name;

    /** Name of the library implementing this function */
    const TRbyte*           libName;
    
    /** Function ordinal in the DLL file */
    TRint                   ordinal;
    
    /** A pointer to the function itself */
    void*                   pointer;
    
    /** A pointer to the function event handler */
    TREventHandler          eventHandler;
    
    /** Boolean flags (see TRFunctionFlags) */
    TRuint                  flags;
};

/** Event structure */
struct TREventStruct
{
    /** State reference */
    struct TRStateStruct*   state;
    
    /** API identifier */
    TRenum                  api;
    
    /** Function referenced by this event */
    const TRFunction*       function;
    
    /** Call time for the function */
    TRtimestamp             callTime;
    
    /** Event timestamp */
    TRtimestamp             timestamp;
    
    /** Event sequence number */
    TRuint                  sequenceNumber;
};

/* API functions */
void        trRegisterApi(TRenum api, TRuint functionCount,
                          const TRFunction* functions);
void        trOpenDefaultOutputStream(const TRbyte* apiConf);
TRenum      trGetError(void);

/**
 *  Indicate a new event for a specific API. This function must be called before
 *  the other event related data such as parameter values are logged.
 *
 *  @param api  Event API identifier
 *  @param name Event name, or NULL for an anonymous event.
 *
 *  @returns a handle for the new event
 */
TREvent*    trBeginEvent(TRenum api, const TRbyte* name);

/**
 *  Indicate a new event for a specific API. This function must be called before
 *  the other event related data such as parameter values are logged.
 *
 *  @param api   Event API identifier
 *  @param index Index of the event in the function table passed to trRegisterApi.
 *
 *  @returns a handle for the new event
 */
TREvent*    trBeginEventByIndex(TRenum api, TRuint index);
TREvent*    trBeginTerminatorEventByIndex(TRenum api, TRuint index);

void        trIntegerValue(TREvent* event, const TRbyte* name, TRint value);
void        trLongValue(TREvent* event, const TRbyte* name, TRlong value);
void        trFloatValue(TREvent* event, const TRbyte* name, TRfloat value);
void        trDoubleValue(TREvent* event, const TRbyte* name, TRdouble value);
void        trShortValue(TREvent* event, const TRbyte* name, TRshort value);
void        trByteValue(TREvent* event, const TRbyte* name, TRubyte value);
void        trPointerValue(TREvent* event, const TRbyte* name, const void* value);
void        trByteArrayValue(TREvent* event, const TRbyte* name, TRuint length, const TRubyte* value);
void        trShortArrayValue(TREvent* event, const TRbyte* name, TRuint length, const TRshort* value);
void        trIntegerArrayValue(TREvent* event, const TRbyte* name, TRuint length, const TRint* value);
void        trLongArrayValue(TREvent* event, const TRbyte* name, TRuint length, const TRlong* value);
void        trFloatArrayValue(TREvent* event, const TRbyte* name, TRuint length, const TRfloat* value);
void        trDoubleArrayValue(TREvent* event, const TRbyte* name, TRuint length, const TRdouble* value);
void        trObjectArrayValue(TREvent* event, const TRbyte* name, TRuint length, TRenum cls, const TRhandle* value);
TRbool      trDefinePointerData(TREvent* event, const void* pointer, TRuint length, const TRubyte* data, TRenum type);
void        trForgetPointerData(TREvent* event, const void* pointer);
void        trEndEvent(TREvent* event);
void        trBeginCall(TREvent* event);
void        trEndCall(TREvent* event);
void        trTerminate(void);
void        trFlush(void);
void        trPanic(const TRbyte* reason);
TRenum	    trGetClassId(const TRbyte* name);
void        trDispatchPassthrough(void);

/* Stream management */
void        trOpenInputStream(const TRbyte* device, const TRbyte* codec, const TRbyte* name,
                              const TRStrDict* options);
void        trOpenOutputStream(const TRbyte* device, const TRbyte* codec, const TRbyte* baseName,
                               const TRbyte* extension, const TRStrDict* options);

/* State tracking */
void        trSetStateValue(TREvent* event, const TRbyte* path, void* value);
void        trSetStateValueArray(TREvent* event, const TRbyte* path, TRuint length, const TRubyte* data);
void*       trGetStateValue(TREvent* event, const TRbyte* path);
void*       trGetSpecificStateValue(TREvent* event, const TRbyte* path, const TRint* keys);
void        trRemoveStateValue(TREvent* event, const TRbyte* path);

/* Object tracking */

/**
 *  Serialize an arbitrary object to the trace stream.
 *
 *  @param event    Currently active event.
 *  @param name     Name for the object.
 *  @param cls      Class enumeration for the object. This will determine the used serialized.
 *  @param handle   A handle that will uniquely identify the object within a collection of
 *                  objects of the same type.
 */
void        trObjectValue(TREvent* event, const TRbyte* name, TRenum cls, TRhandle handle);

/**
 *  Register a newly constructed object corresponding to an object serialized in a trace file.
 *  All further calls to trGetObjectValue() with the traceHandle passed here will return the
 *  object registered for that handle.
 *
 *  @param event        Currently active event.
 *  @param cls          Class enumeration for the object.
 *  @param ns           Namespace under which the object was serialized.
 *  @param traceHandle  Handle under which the object was serialized, i.e. what was passed to
 *                      trObjectValue() in the tracer.
 *  @param handle       Handle to the newly constructed object.
 */
void        trRegisterObject(TREvent* event, TRenum cls, TRhandle ns, TRhandle traceHandle, TRhandle handle);

/**
 *  Register a newly constructed object corresponding to an object passed as a value in an 
 *  event in the trace file. All further calls to trGetObjectValue() with the event value
 *  passed here will return the object registered for that handle. 
 *
 *  @param event        Currently active event.
 *  @param cls          Class enumeration for the object.
 *  @param valueName    Name of event value which corresponds to this event. Pass 0 to refer
 *                      to the event return value.
 *  @param handle       Handle to the newly constructed object.
 */
void        trRegisterObjectAsEventValue(TREvent* event, const TRbyte* name, TRhandle handle);

/**
 *  Register a new class of objects.
 *
 *  @param cls           Class identifier enumeration.
 *  @param name          Class name.
 *  @param namespacePath Path to state value which defines the namespace for this class.
 *                       This would be the EGL context for OpenVG paths, for example.
 *                       Pass NULL to indicate that no namespaces are used.
 *  @param c             Object constructor function.
 *  @param d             Object destructor function.
 *  @param s             Object serializer function.
 */
void        trRegisterClass(TRenum cls, const TRbyte* name, const TRbyte* namespacePath,
                            TRObjectConstructor c, TRObjectDestructor d, 
                            TRObjectSerializer s, TRObjectModifier m);

/* Player functions */
TREvent*    trGetEvent(void);
TRint       trGetIntegerValue(TREvent* event, const TRbyte* name);
TRlong      trGetLongValue(TREvent* event, const TRbyte* name);
TRfloat     trGetFloatValue(TREvent* event, const TRbyte* name);
TRdouble    trGetDoubleValue(TREvent* event, const TRbyte* name);
TRshort     trGetShortValue(TREvent* event, const TRbyte* name);
TRubyte     trGetByteValue(TREvent* event, const TRbyte* name);
void*       trGetPointerValue(TREvent* event, const TRbyte* name);
TRhandle    trGetObjectValue(TREvent* event, const TRbyte* name);
TRfloat     trGetFpsEstimate(void);

/* Runtime configuration */
void        trLoadConfig(const TRbyte* apiConf);
TRbool      trLoadDefaultConfig();
const TRbyte* trGetConfigString(const TRbyte* section, const TRbyte* option, const TRbyte* dfl);
TRint       trGetConfigInteger(const TRbyte* section, const TRbyte* option, TRint dfl);
const TRStrDict* trGetConfigSection(const TRbyte* section);

/* Utility functions */
void*       trMalloc(TRuint bytes);
void        trFree(void* data);
TRFunction* trLookupFunction(TRenum api, const TRbyte* name);
TRFunction* trLookupFunctionByIndex(TRenum api, TRuint index);
TRtimestamp trGetSystemTime(void);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // TRACER_H
