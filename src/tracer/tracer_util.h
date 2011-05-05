/**
 *  Tracer utility functionality
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
#ifndef TRACER_UTIL_H
#define TRACER_UTIL_H

#include "tracer.h"
#include "tracer_internal.h"
#include "tracer_platform.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* String utilities */
TRbyte* triStringFormat(TRState* state, const TRbyte* format, ...);
TRbyte* triStringDuplicate(TRState* state, const TRbyte* string);
void    triStringCopy(TRbyte* dest, const TRbyte* source, TRuint maxLength);
TRint   triStringIndex(const TRbyte* string, const TRbyte* match);
TR_INLINE TRuint  triStringLength(const TRbyte* string);
TR_INLINE TRbool  triStringsEqual(const TRbyte* string1, const TRbyte* string2);

/**
 * Dictionary node
 */
typedef struct TRDictNodeStruct TRDictNode;

/**
 * Dictionary node visitor function 
 */
typedef TRbool (*TRDictVisitor)(TRDict* dict, TRDictNode* node, void* arg);

/** 
 * Dictionary
 */
struct TRDictStruct
{
    /* Node operations */
    TRint           (*compareKeys)(const void* key1, const void* key2);
    TRDictNode*     (*createNode)(TRDict* dict, const void* key, void* value);
    void            (*destroyNode)(TRDict* dict, TRDictNode* node);
    
    /* Dictionary data */
    TRuint          size;
    TRDictNode*     root;
    TRState*        state;
};

struct TRDictNodeStruct
{
    const void*                key;
    void*                      value;
#if defined(DICT_DEBUG)
    TRint                      magic1;
#endif
    struct TRDictNodeStruct*   parent;
#if defined(DICT_DEBUG)
    TRint                      magic2;
#endif
    struct TRDictNodeStruct*   left;
    struct TRDictNodeStruct*   right;
};

TRDict*  triCreateDict(TRState* state);
void     triDestroyDict(TRDict* dict);
void     triDictSet(TRDict* dict, const void* key, void* value);
void*    triDictGet(const TRDict* dict, const void* key);
TRuint   triDictSize(const TRDict* dict);
TRbool   triDictRemove(TRDict* dict, const void* key);
TRArray* triDictGetKeys(TRDict* dict);
TRArray* triDictGetValues(TRDict* dict);
void     triDictVisit(TRDict* dict, TRDictVisitor visitor, void *arg);
int      triDictContains(const TRDict* dict, const void* key);

/* 
 * Dictionary with strings as keys and pointers as values 
 */
TRStrDict* triCreateStrDict(TRState* state);
void       triStrDictSet(TRStrDict* dict, const TRbyte* key, void* value);
void*      triStrDictGet(const TRStrDict* dict, const TRbyte* key);
TRbool     triStrDictRemove(TRStrDict* dict, const TRbyte* key);

/* 
 * Dictionary with integers as keys and pointers as values 
 */
TRIntDict* triCreateIntDict(TRState* state);
void       triIntDictSet(TRIntDict* dict, TRint key, void* value);
void*      triIntDictGet(const TRIntDict* dict, TRint key);
int        triIntDictContains(const TRIntDict* dict, const void* key);
TRbool     triIntDictRemove(TRIntDict* dict, TRint key);

/**
 *  A linear array containing pointers
 */
struct TRArrayStruct
{
    TRuint      size;
    void**      values;
    TRState*    state;
};

TRArray*  triCreateArray(TRState* state);
void      triDestroyArray(TRArray* array);
void      triArrayAppend(TRArray* array, void* value);
TRbool    triArrayRemove(TRArray* array, const void* value);
void      triArraySet(TRArray* array, TRuint index, void* value);
TR_INLINE TRuint    triArraySize(const TRArray* array);
TR_INLINE void*     triArrayGet(const TRArray* array, TRuint index);

/**
 * A macro for iterating over each value in an array.
 *
 *  @param type       Array item type declaration
 *  @param variable   Name of loop variable
 *  @param array      TRArray pointer
 */
#define TR_FOR_EACH(TYPE, VARIABLE, ARRAY) \
    TRuint __i ## __LINE__; \
    const TRArray* __array ## __LINE__ = (ARRAY); \
    TYPE VARIABLE; \
    for (__i ## __LINE__ = 0; \
        (__i ## __LINE__ < (__array ## __LINE__)->size) && ((0 != (VARIABLE = (TYPE)(__array ## __LINE__)->values[__i ## __LINE__])) || 1); \
        (__i ## __LINE__)++)

/**
 *  Calculates the CRC32 checksum of a block of data.
 *
 *  @param state      State structure
 *  @param length     Length of data
 *  @param data       Pointer to data
 */
TRuint    triCalculateCRC32(TRState* state, TRuint length, const TRbyte* data);


/* Bring in the inline functions */
#include "tracer_util.inl"

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // TRACER_PLATFORM_H
