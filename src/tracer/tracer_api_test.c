/**
 *  Tracer C API test suite
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
#include "CuTest.h"
#include <stdlib.h>
#include <unistd.h>

#define TR_TEST_API 0x0

#if defined(TR_PLATFORM_WIN32)
#  define TRACEFILE_BIN  "tracer_testsuite.exe.bin"
#  define TRACEFILE_LOG  "tracer_testsuite.exe.log"
#  define LIBRARYFILE    "testlibrary.dll"
#else
#  define TRACEFILE_BIN   "tracer_testsuite.bin"
#  define TRACEFILE_LOG   "tracer_testsuite.log"
#  define LIBRARYFILE     "./testlibrary.so"
#endif

static void testFunctionHandler(TREvent* event)
{
    TRhandle obj = trGetObjectValue(event, "obj");
    TR_UNUSED(obj);
}

const static TRFunction functions[] =
{
    {"testFunction", LIBRARYFILE, 0xf00d, 0, testFunctionHandler, 0},
};

void trInitialize()
{
    trRegisterApi(TR_TEST_API, sizeof(functions) / sizeof(functions[0]), functions);
    trLoadConfig("test");
}
 
void TestBasicEventLogging(CuTest* tc)
{
    /* Check that we can find our test function */
    TREvent* event = trBeginEvent(TR_TEST_API, "testFunction");
    const TRFunction* func = event->function;
    CuAssertPtrNotNull(tc, func);
    CuAssertIntEquals(tc, 0xf00d, func->ordinal);

    /* Try logging some values */    
    trIntegerValue(event, "foo", 1234);
    
    /* All done */
    trEndEvent(event);

    /* Shutdown */
    trTerminate();
}

void TestUnknownEvent(CuTest* tc)
{
    /* Bad function */
    CuAssertPtrEquals(tc, 0, (void*)trBeginEvent(TR_TEST_API, "nonexistant")->function);
    
    /* Bad value */
    trGetError();
    CuAssertPtrEquals(tc, 0, trBeginEvent(TR_TEST_API, 0)->function);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    /* Shutdown */
    trTerminate();
}
 
void TestStreamWriting(CuTest* tc)
{
    TREvent* event = 0;
    TREvent* event2 = 0;
    const TRFunction* func = 0;

    trOpenOutputStream("stdio", "ascii", "tracer", "log", 0);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    /* Check that we can find our test function */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    func = event->function;
    CuAssertPtrNotNull(tc, func);
    CuAssertIntEquals(tc, 0xf00d, func->ordinal);

    /* Try logging some values */    
    trIntegerValue(event, "foo", 1234);
    
    /* All done */
    trEndEvent(event);

    /* Test recursive events */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerValue(event, "foo1", 1);
    event2 = trBeginEvent(TR_TEST_API, "testFunction");
    trIntegerValue(event2, "foo2", 2);
    CuAssertPtrNotNull(tc, event2);
    trEndEvent(event2);
    trIntegerValue(event, "foo3", 3);
    trEndEvent(event);
    
    /* Shutdown */
    trTerminate();
}

/* Test state tree */
#define ROOT    "\x01"
#define LEVEL1  "\x01"
#define KEY1_1  "\x81"
#define LEVEL2  "\x02"
#define KEY2_1  "\x81"
#define KEY2_2  "\x82"
#define KEY2_3  "\x83"
#define READKEY "\xff"

void TestStateTracking(CuTest* tc)
{
    const TRint keys[] = {0x1000};
    const TRubyte array[] = {0xa, 0xb, 0xc, 0xd};
    const TRubyte* array2;
  
    TREvent* e = trBeginEvent(TR_TEST_API, 0);
    CuAssertPtrNotNull(tc, e);
    
    trSetStateValue(e, LEVEL1,               (void*)0x1000);
    trSetStateValue(e, LEVEL1 KEY1_1,        (void*)0x1001);
    trSetStateValue(e, LEVEL1 LEVEL2,        (void*)0x2000);
    trSetStateValue(e, LEVEL1 LEVEL2 KEY2_1, (void*)0x2001);
    trSetStateValue(e, LEVEL1 LEVEL2 KEY2_2, (void*)0x2002);
    trSetStateValue(e, LEVEL1 LEVEL2 KEY2_3, (void*)0x2003);
    trSetStateValue(e, LEVEL1 LEVEL2 KEY2_1, (void*)0x2004);
    CuAssertIntEquals(tc, 0x1000, (TRint)trGetStateValue(e, LEVEL1));
    CuAssertIntEquals(tc, 0x1001, (TRint)trGetStateValue(e, LEVEL1 KEY1_1));
    CuAssertIntEquals(tc, 0x2000, (TRint)trGetStateValue(e, LEVEL1 LEVEL2));
    CuAssertIntEquals(tc, 0x2004, (TRint)trGetStateValue(e, LEVEL1 LEVEL2 KEY2_1));
    CuAssertIntEquals(tc, 0x2002, (TRint)trGetStateValue(e, LEVEL1 LEVEL2 KEY2_2));
    CuAssertIntEquals(tc, 0x2003, (TRint)trGetStateValue(e, LEVEL1 LEVEL2 KEY2_3));

    /* Bad path */
    CuAssertIntEquals(tc, 0x0,    (TRint)trGetStateValue(e, "foo"));
    CuAssertIntEquals(tc, TR_NOT_FOUND, trGetError());
    
    /* Specific paths */
    trSetStateValue(e, LEVEL1,               (void*)0x3000);
    trSetStateValue(e, LEVEL1 KEY1_1,        (void*)0x3001);
    CuAssertIntEquals(tc, 0x3001, (TRint)trGetStateValue(e, LEVEL1 KEY1_1));
    CuAssertIntEquals(tc, 0x1001, (TRint)trGetSpecificStateValue(e, LEVEL1 READKEY KEY1_1, keys));

    /* Value arrays */
    trSetStateValueArray(e, LEVEL1 KEY1_1,   4, array);
    array2 = (const TRubyte*)trGetStateValue(e, LEVEL1 KEY1_1);
    CuAssertIntEquals(tc, array[0], array2[0]);
    trSetStateValue(e, LEVEL1 KEY1_1,        (void*)0x1001);
    CuAssertIntEquals(tc, 0x1001, (TRint)trGetStateValue(e, LEVEL1 KEY1_1));
    trSetStateValueArray(e, LEVEL1 KEY1_1,   4, array);
    array2 = (const TRubyte*)trGetStateValue(e, LEVEL1 KEY1_1);
    CuAssertIntEquals(tc, array[0], array2[0]);
    trSetStateValueArray(e, LEVEL1 KEY1_1,   0, 0);
    trSetStateValue(e, LEVEL1 KEY1_1,        (void*)0x0);
    
    /* Shutdown */
    trEndEvent(e);
    trTerminate();
}

void TestPointerTracking(CuTest* tc)
{
    TREvent* e;

    e = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, e);

    /* Log a pointer and define its data */
    trPointerValue(e, "pointer", (void*)0xdadacafe);
    CuAssertIntEquals(tc, TR_TRUE,  trDefinePointerData(e, (void*)0xdadacafe, 3, (TRubyte*)"fubu", TR_BYTE));
    CuAssertIntEquals(tc, TR_TRUE,  trDefinePointerData(e, (void*)0xdadacafe, 3, (TRubyte*)"bar",  TR_BYTE));
    CuAssertIntEquals(tc, TR_TRUE,  trDefinePointerData(e, (void*)0xdadacafe, 4, (TRubyte*)"quux", TR_BYTE));
    CuAssertIntEquals(tc, TR_TRUE,  trDefinePointerData(e, (void*)0xdadacafe, 3, (TRubyte*)"foo",  TR_BYTE));
    
    /* Get rid of the tracking data */
    CuAssertIntEquals(tc, TR_TRUE,   trDefinePointerData(e, (void*)0xdadacafe, 0, 0, TR_VOID));
    CuAssertIntEquals(tc, TR_FALSE,  trDefinePointerData(e, (void*)0xdadacafe, 0, 0, TR_VOID));
    
    /* All done */
    trEndEvent(e);

    /* Shutdown */
    trTerminate();
}

void TestRuntimeConfig(CuTest* tc)
{
    trLoadConfig("test2");
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    CuAssertStrEquals(tc, "value",  trGetConfigString("section",     "option",    0));
    CuAssertStrEquals(tc, "value2", trGetConfigString("section2",    "option",    0));
    CuAssertPtrEquals(tc, 0,        (void*)trGetConfigString("nonexistent", "foo",       0));
    CuAssertPtrEquals(tc, 0,        (void*)trGetConfigString("section",     "nosection", 0));
    CuAssertIntEquals(tc, 3,        trGetConfigInteger("section",    "pi",        0));

    trTerminate();
}

typedef TRint TestObject;
const TRint TestObjectClassId = 0x1;

TRhandle createObject(TREvent* event)
{
    TestObject* obj = malloc(sizeof(TestObject));
    *obj = trGetIntegerValue(event, "shininess");
    return obj;
}

void destroyObject(TRhandle handle)
{
    TestObject* obj = (TestObject*)handle;
    free(obj);
}

void serializeObject(TREvent* event, TRhandle handle)
{
    TestObject* obj = (TestObject*)handle;
    trIntegerValue(event, "shininess", *obj);
}

void modifyObject(TREvent* event, TRhandle handle)
{
    TestObject* obj = (TestObject*)handle;
    *obj = trGetIntegerValue(event, "shininess");
}

void TestObjectSerialization(CuTest* tc)
{
    TREvent* event;
    TestObject  obj     = 31337;
    TestObject  obj2    = 8008135;
    TestObject* objPtr  = 0;
    TestObject* objPtr2 = 0;
    TestObject* objPtr3 = 0;
    TestObject** objArrayPtr = 0;
    const TestObject* objArray[] = {
        &obj,
        0,
        &obj2,
    };

    /* Register our test class */
    trRegisterClass(TestObjectClassId, "TestObject", LEVEL1, createObject, destroyObject, serializeObject, modifyObject);
    trOpenOutputStream("stdio", "ascii",  "tracer", "log", 0);
    trOpenOutputStream("stdio", "binary", "tracer", "bin", 0);

    /* Write it out */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trSetStateValue(event, LEVEL1, (void*)0); /* Reset the namespace */
    trObjectValue(event, "obj", TestObjectClassId, &obj);
    trEndEvent(event);
    
    /* Change the object */
    obj = 715517;

    /* Write it out again */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trObjectValue(event, "obj", TestObjectClassId, &obj);
    trEndEvent(event);

    /* Change the namespace */
    trSetStateValue(event, LEVEL1, (void*)0xdadacafe);

    /* Write it out again */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trObjectValue(event, "obj", TestObjectClassId, &obj);
    trEndEvent(event);

    /* Write it out as a part of an array */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trObjectArrayValue(event, "objArray", 3, TestObjectClassId, (const TRhandle*)&objArray);
    trEndEvent(event);

    /* Tear down and open the same stream for reading */
    trTerminate();
    trRegisterClass(TestObjectClassId, "TestObject", 0, createObject, destroyObject, serializeObject, modifyObject);
    trOpenInputStream("stdio", "binary", TRACEFILE_BIN, 0);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    /* Read the first event */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);

    objPtr = trGetObjectValue(event, "obj");
    
    /* Make sure we got a corresponding object */
    CuAssertIntEquals(tc, *objPtr, 31337);

    /* Read the second event */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);

    objPtr2 = trGetObjectValue(event, "obj");
    
    /* Make sure it is the same object */
    CuAssertIntEquals(tc, (int)objPtr2, (int)objPtr);

    /* Make sure we got a corresponding object */
    CuAssertIntEquals(tc, *objPtr2, 715517);

    /* Read the third event */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);

    objPtr3 = trGetObjectValue(event, "obj");
    
    /* Make sure it is a different object (different namespace) */
    CuAssertTrue(tc, objPtr3 != objPtr);

    /* Make sure we got a corresponding object */
    CuAssertIntEquals(tc, *objPtr3, 715517);

    /* Read the fourth event */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);

    objArrayPtr = trGetObjectValue(event, "objArray");
    CuAssertPtrNotNull(tc, objArrayPtr);
    
    /* Check the contents of the array */
    CuAssertIntEquals(tc, *objArrayPtr[0], 715517);
    CuAssertIntEquals(tc, (int)objArrayPtr[1], 0);
    CuAssertIntEquals(tc, *objArrayPtr[2], 8008135);

    trTerminate();
}

void TestMutableArrays(CuTest* tc)
{
    TREvent* event;
    int array[5] = {1, 2, 3, 4, 5};
    int* arrayPtr;

    /* Open some streams */
    trOpenOutputStream("stdio", "ascii",  "tracer", "log", 0);
    trOpenOutputStream("stdio", "binary", "tracer", "bin", 0);

    /* Write an event */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 5, array);
    trEndEvent(event);

    /* Write the same one again */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 5, array);
    trEndEvent(event);

    /* Change the array */
    array[0] = 32;
    array[1] = 16;
    array[2] = 8;
    array[3] = 4;

    /* Write another event */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 4, array);
    trEndEvent(event);

    /* Shrink the array */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 1, array);
    trEndEvent(event);

    /* Enlarge it again */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 4, array);
    trEndEvent(event);

    /* Change it back again */
    array[0] = 1;
    array[1] = 2;
    array[2] = 3;
    array[3] = 4;

    /* Write another event */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 4, array);
    trEndEvent(event);

    /* Write another event */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trIntegerArrayValue(event, "array", 5, array);
    trEndEvent(event);

    /* Tear down and open the same stream for reading */
    trTerminate();
    trOpenInputStream("stdio", "binary", TRACEFILE_BIN, 0);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    /* Read the first array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 1, arrayPtr[0]);
    CuAssertIntEquals(tc, 2, arrayPtr[1]);
    CuAssertIntEquals(tc, 3, arrayPtr[2]);
    CuAssertIntEquals(tc, 4, arrayPtr[3]);
    CuAssertIntEquals(tc, 5, arrayPtr[4]);

    /* Read the second array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 1, arrayPtr[0]);
    CuAssertIntEquals(tc, 2, arrayPtr[1]);
    CuAssertIntEquals(tc, 3, arrayPtr[2]);
    CuAssertIntEquals(tc, 4, arrayPtr[3]);
    CuAssertIntEquals(tc, 5, arrayPtr[4]);

    /* Read the third array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 32, arrayPtr[0]);
    CuAssertIntEquals(tc, 16, arrayPtr[1]);
    CuAssertIntEquals(tc, 8,  arrayPtr[2]);
    CuAssertIntEquals(tc, 4,  arrayPtr[3]);

    /* Read the fourth array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 32, arrayPtr[0]);

    /* Read the fifth array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 32, arrayPtr[0]);
    CuAssertIntEquals(tc, 16, arrayPtr[1]);
    CuAssertIntEquals(tc, 8,  arrayPtr[2]);
    CuAssertIntEquals(tc, 4,  arrayPtr[3]);

    /* Read the sixth array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 1, arrayPtr[0]);
    CuAssertIntEquals(tc, 2, arrayPtr[1]);
    CuAssertIntEquals(tc, 3, arrayPtr[2]);
    CuAssertIntEquals(tc, 4, arrayPtr[3]);

    /* Read the seventh array */
    event = trGetEvent();
    CuAssertPtrNotNull(tc, event);
    arrayPtr = trGetPointerValue(event, "array");
    CuAssertIntEquals(tc, 1, arrayPtr[0]);
    CuAssertIntEquals(tc, 2, arrayPtr[1]);
    CuAssertIntEquals(tc, 3, arrayPtr[2]);
    CuAssertIntEquals(tc, 4, arrayPtr[3]);
    CuAssertIntEquals(tc, 5, arrayPtr[4]);

    /* Make sure we reached the end */
    event = trGetEvent();
    CuAssertIntEquals(tc, 0, (int)event);

    trTerminate();
}

CuSuite* CuApiGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();
  
    SUITE_ADD_TEST(suite, TestBasicEventLogging);
    SUITE_ADD_TEST(suite, TestUnknownEvent);
    SUITE_ADD_TEST(suite, TestStreamWriting);
    SUITE_ADD_TEST(suite, TestStateTracking);
    SUITE_ADD_TEST(suite, TestPointerTracking);
    SUITE_ADD_TEST(suite, TestRuntimeConfig);
    SUITE_ADD_TEST(suite, TestMutableArrays);
    SUITE_ADD_TEST(suite, TestObjectSerialization);
  
    return suite;
}
