/**
 *  Compiler definitions
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
#ifndef TRACER_COMPILER_H
#define TRACER_COMPILER_H

/* Platform selection */
#if defined(_MSC_VER)
#  define TR_PLATFORM_WIN32
#elif defined(__SYMBIAN32__)
#  define TR_PLATFORM_SYMBIAN
#elif defined(ANDROID)
#  define TR_PLATFORM_ANDROID
#else
#  define TR_PLATFORM_UNIX
#endif

/* Inline function hint */
#if defined (__GNUC__) || defined (__GCC32__)                         /* GNU GCC >= 3.0 */
#  if defined(__cplusplus)
#    define TR_INLINE inline __attribute__ ((always_inline))
#  else
#    define TR_INLINE static inline __attribute__ ((always_inline))
#  endif
#elif defined(__ARMCC_VERSION)                                        /* RVCT */
#  if defined(__cplusplus)
#    define TR_INLINE static __forceinline
#  else
#    define TR_INLINE static __forceinline
#  endif
#else                                                                 /* Unknown compiler */
#  if defined(__cplusplus)
#    define TR_INLINE inline
#  else
#    define TR_INLINE static
#  endif
#endif

/* Disable some warnings for MSVC */
#ifdef _MSC_VER
#   pragma warning (disable: 4127)
#   pragma warning (disable: 4505)
#endif /* _MSC_VER */

/* 64 bit long integer support */
#if defined(_MSC_VER) && _MSC_VER < 1310                              /* Visual Studio 6 */
#   define TR_INT64_TYPE        __int64
#else
#   define TR_INT64_TYPE        long long
#endif

/** A function with custom entry and exit code */
#if defined(_MSC_VER) || defined(__WINSCW__)
#  define TR_NAKED   __declspec(naked)
#elif defined(__GNUC__)
#  define TR_NAKED __attribute((naked))
#elif defined(__arm__)
#  define TR_NAKED   __asm
#endif

#if defined(WIN32) || defined(__WINSCW__)
   /*
    *  Handle a passthrough API call by jumping into the dispatcher
    *
    *  @param LIB_AND_INDEX high word: API index (zero based)
    *                        low word: function index (zero-based)
    */
#   define TR_GENSTUB(LINKAGE, NAME, LIB_AND_INDEX) \
    LINKAGE void NAME(void); \
    TR_NAKED void NAME(void) \
    { \
        __asm mov eax, LIB_AND_INDEX \
        __asm jmp trDispatchPassthrough \
    }
#elif defined(__GNUC__) && !defined(__arm__)
   /*
    *  Handle a passthrough API call by jumping into the dispatcher
    *
    *  @param LIB_AND_INDEX high word: API index (zero based)
    *                        low word: function index (zero-based)
    */
#   define TR_GENSTUB(LINKAGE, NAME, LIB_AND_INDEX) \
    LINKAGE void NAME(void); \
    TR_NAKED void NAME(void) \
    { \
        __asm__("movl $" #LIB_AND_INDEX", %eax\n\t" \
                "jmp trDispatchPassthrough\n\t"); \
    }
#elif defined(__arm__)
   /*
    *  Handle a passthrough API call by jumping into the dispatcher
    *
    *  @param LIB_AND_INDEX high word: API index (zero based)
    *                        low word: function index (zero-based)
    *
    *  Note: The implementation generates two different functions,
    *  since it seems to be impossible to correctly generate multiple 
    *  embedded assembly commands from the preprocessor. We can do this
    *  since RVCT is guaranteed not to reorder __asm functions.
    *
    *  Note #2: LIB_AND_INDEX must be less than 256!
    *
    *  The magic constant means: mov r12, #LIB_AND_INDEX
    */
#   define TR_GENSTUB(LINKAGE, NAME, LIB_AND_INDEX) \
    LINKAGE void NAME(void); \
    TR_NAKED void NAME(void) \
    { \
        dcdu __cpp(0xE3A0C000 | LIB_AND_INDEX) \
    } \
    TR_NAKED void NAME ## __jump (void) \
    { \
        b   __cpp(trDispatchPassthrough) \
    }
#endif

#define TR_GENERATE_PASSTHROUGH_FUNCTION(LINKAGE, NAME, LIB, INDEX) \
	TR_GENSTUB(LINKAGE, NAME, (LIB << 16) | INDEX)

/* Lint settings */
/*lint -efunc(774, trPanic)                   */ /* Boolean within 'if' always evaluates to True */
/*lint -emacro(506, TR_FOR_EACH)              */ /* Constant value Boolean */
/*lint -emacro(522, TR_FOR_EACH)              */ /* Expected void type, assignment, increment or decrement */
/*lint -emacro(717, TR_ASSERT)                */ /* do ... while(0); */
/*lint -emacro(717, TR_ASSERT_RETURN)         */ /* do ... while(0); */
/*lint -emacro(717, TR_ASSERT_ERROR)          */ /* do ... while(0); */
/*lint -emacro(717, TR_ASSERT_ERROR_RETURN)   */ /* do ... while(0); */
/*lint -e730                                  */ /* Boolean argument to function */
/*lint -e830                                  */ /* Unreferenced location */
/*lint -e768                                  */ /* Unreferenced struct member */
/*lint -e769                                  */ /* Unreferenced macro */
/*lint -e755                                  */ /* Unreferenced global macro */
/*lint -e757                                  */ /* Unreferenced declarator */
/*lint -e522                                  */ /* Expected void type, assignment, ... (for function pointers) */

#endif /* TRACER_COMPILER_H */
