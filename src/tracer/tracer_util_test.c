/**
 *  Tracer utility test suite
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
#include "tracer_util.h"
#include "tracer_internal.h"
#include "CuTest.h"
 
void TestDict(CuTest* tc)
{
    TRState* state = triGetState();
    TRStrDict* dict = triCreateStrDict(state);
    TRArray* keys;
    TRArray* values;
    
    CuAssertPtrNotNull(tc, dict);
    
    CuAssertIntEquals(tc, 0, triDictSize(dict));
    triStrDictSet(dict, "foo", (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triDictSize(dict));
    CuAssertIntEquals(tc, 1234, (int)triStrDictGet(dict, "foo"));

    triStrDictSet(dict, "foo", (void*)31337);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triDictSize(dict));
    CuAssertIntEquals(tc, 31337, (int)triStrDictGet(dict, "foo"));

    triStrDictSet(dict, "bar", (void*)715517);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triDictSize(dict));
    CuAssertIntEquals(tc, 715517, (int)triDictGet(dict, "bar"));

    keys = triDictGetKeys(dict);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triArraySize(keys));
    triDestroyArray(keys);

    values = triDictGetKeys(dict);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triArraySize(values));
    triDestroyArray(values);

    CuAssertIntEquals(tc, 0, (int)triStrDictGet(dict, "none"));
    triStrDictRemove(dict, "none");
    triStrDictRemove(dict, "foo");
    triStrDictRemove(dict, "bar");
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 0, triDictSize(dict));
    
    triStrDictSet(dict, "foo", (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triDictSize(dict));

    triDestroyDict(dict);
    triDestroyState(state);
}

static TRint pseudoRand(TRint seed)
{
    return (seed ^ ((seed << 3) + 0x213214) * (seed ^ 0x9853));
}

void StressTestDict(CuTest* tc)
{
    TRState* state = triGetState();
    TRStrDict* dict = triCreateStrDict(state);
    TRint i, n = 1000;
    TRbyte name[32];
    
    CuAssertPtrNotNull(tc, dict);

    for (i = 0; i < n; i++)
    {
        TRint k = pseudoRand(i);
        sprintf(name, "%d", k);
        triStrDictSet(dict, name, (void*)(i + 1));
    }
    CuAssertIntEquals(tc, n, triDictSize(dict));

    /* Remove every other key */
    for (i = 0; i < n; i += 2)
    {
        TRint k = pseudoRand(i);
        sprintf(name, "%d", k);
        CuAssertPtrNotNull(tc, triStrDictGet(dict, name));
        CuAssertIntEquals(tc, TR_TRUE, triStrDictRemove(dict, name));
        CuAssertIntEquals(tc, 0, (int)triStrDictGet(dict, name));
        CuAssertIntEquals(tc, n - i / 2 - 1, triDictSize(dict));
    }

    /* Remove the remaining keys */
    for (i = 1; i < n; i += 2)
    {
        TRint k = pseudoRand(i);
        sprintf(name, "%d", k);
        CuAssertPtrNotNull(tc, triStrDictGet(dict, name));
        CuAssertIntEquals(tc, TR_TRUE, triStrDictRemove(dict, name));
        CuAssertIntEquals(tc, 0, (int)triStrDictGet(dict, name));
        CuAssertIntEquals(tc, n / 2 - i / 2 - 1, triDictSize(dict));
    }

    CuAssertIntEquals(tc, 0, triDictSize(dict));
    triDestroyDict(dict);
    triDestroyState(state);
}

void TestIntDict(CuTest* tc)
{
    TRState* state = triGetState();
    TRIntDict* dict = triCreateIntDict(state);
    TRArray* keys;
    TRArray* values;
    
    CuAssertPtrNotNull(tc, dict);
    
    CuAssertIntEquals(tc, 0, triDictSize(dict));
    triIntDictSet(dict, 0x1, (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triDictSize(dict));
    CuAssertIntEquals(tc, 1234, (int)triIntDictGet(dict, 0x1));

    triIntDictSet(dict, 0x1, (void*)31337);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triDictSize(dict));
    CuAssertIntEquals(tc, 31337, (int)triIntDictGet(dict, 0x1));

    triIntDictSet(dict, 0x2, (void*)715517);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triDictSize(dict));
    CuAssertIntEquals(tc, 715517, (int)triIntDictGet(dict, 0x2));

    keys = triDictGetKeys(dict);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triArraySize(keys));
    triDestroyArray(keys);

    values = triDictGetKeys(dict);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triArraySize(values));
    triDestroyArray(values);

    CuAssertIntEquals(tc, 0, (int)triIntDictGet(dict, 0x3));
    triIntDictRemove(dict, 0x3);
    triIntDictRemove(dict, 0x1);
    triIntDictRemove(dict, 0x2);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 0, triDictSize(dict));
    
    triIntDictSet(dict, 0x1, (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triDictSize(dict));
    
    triDestroyDict(dict);
    triDestroyState(state);
}
 
void TestArray(CuTest* tc)
{
    TRState* state = triGetState();
    TRArray* array = triCreateArray(state);
    
    CuAssertPtrNotNull(tc, array);
    
    CuAssertIntEquals(tc, 0, triArraySize(array));
    triArrayAppend(array, (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triArraySize(array));
    CuAssertIntEquals(tc, 1234, (int)triArrayGet(array, 0));

    triArrayAppend(array, (void*)31337);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 2, triArraySize(array));
    CuAssertIntEquals(tc, 31337, (int)triArrayGet(array, 1));

    triArrayRemove(array, (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triArraySize(array));
    CuAssertIntEquals(tc, 31337, (int)triArrayGet(array, 0));

    /* Nonexistent value */
    CuAssertIntEquals(tc, TR_FALSE, triArrayRemove(array, (void*)715517));
    CuAssertIntEquals(tc, TR_TRUE,  triArrayRemove(array, (void*)31337));
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 0, triArraySize(array));
    
    triArrayAppend(array, (void*)1234);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());
    CuAssertIntEquals(tc, 1, triArraySize(array));
    
    triDestroyArray(array);
    triDestroyState(state);
}

void TestCRC(CuTest* tc)
{
    TRState* state = triGetState();
    const TRbyte data1[] = "Apples";
    const TRbyte data2[] = "Oranges";
    const TRbyte data3[] = "";

    TRuint crc1 = triCalculateCRC32(state, triStringLength(data1), data1);
    TRuint crc2 = triCalculateCRC32(state, triStringLength(data2), data2);
    TRuint crc3 = triCalculateCRC32(state, triStringLength(data3), data3);

    CuAssertIntEquals(tc, 0x6614e5a,  crc1);
    CuAssertIntEquals(tc, 0x8ef260b4, crc2);
    CuAssertIntEquals(tc, 0x0,        crc3);
    
    triDestroyState(state);
}

CuSuite* CuUtilGetSuite(void)
{
  CuSuite* suite = CuSuiteNew();

  SUITE_ADD_TEST(suite, TestArray);
  SUITE_ADD_TEST(suite, TestDict);
  SUITE_ADD_TEST(suite, StressTestDict);
  SUITE_ADD_TEST(suite, TestIntDict);
  SUITE_ADD_TEST(suite, TestCRC);
  
  return suite;
}
