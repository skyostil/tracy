/**
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
#include "CuTest.h"
#include "tracer.h"
#include "tracer_player.h"
#include <stdlib.h>

#define TR_TEST_API 0x0

#if defined(TR_PLATFORM_WIN32)
#  define TRACEFILE   "tracer_testsuite.exe.bin"
#else
#  define TRACEFILE   "tracer_testsuite.bin"
#endif

typedef struct
{
    TRint pixmapWidth, pixmapHeight, pixmapMode;
} Player;

TRNativeWindow trPlayerCreateWindow(TRPlayer player, TRint x, TRint y, TRint width, TRint height, TRColorFormat colorFormat)
{
    TR_UNUSED(player);
    return 0;
}

void trPlayerDestroyWindow(TRPlayer player, TRNativeWindow window)
{
    TR_UNUSED(player);
    TR_UNUSED(window);
}

void trPlayerRefreshWindow(TRPlayer player, TRNativeWindow window)
{
    TR_UNUSED(player);
    TR_UNUSED(window);
}

TRNativePixmap trPlayerCreatePixmap(TRPlayer player, TRint width, TRint height, TRColorFormat colorFormat)
{
    Player* p = (Player*)player;
    p->pixmapWidth  = width;
    p->pixmapHeight = height;
    p->pixmapMode   = colorFormat;
    return p;
}

void trPlayerDestroyPixmap(TRPlayer player, TRNativePixmap pixmap)
{
    TR_UNUSED(player);
    TR_UNUSED(pixmap);
}

void trPlayerDisplayPixmap(TRPlayer player, TRNativePixmap pixmap)
{
    TR_UNUSED(player);
    TR_UNUSED(pixmap);
}

const TRint PixmapClassId = 0x1;

TRhandle createPixmap(TREvent* event)
{
    TRint width, height, mode;
    width  = trGetIntegerValue(event, "width");
    height = trGetIntegerValue(event, "height");
    mode   = trGetIntegerValue(event, "mode");
    return trPlayerCreatePixmap(trGetPlayer(), width, height, mode);
}

void destroyPixmap(TRhandle handle)
{
    trPlayerDestroyPixmap(trGetPlayer(), handle);
}

void serializePixmap(TREvent* event, TRhandle handle)
{
    /* Just write some dummy values */
    trIntegerValue(event, "width",  320);
    trIntegerValue(event, "height", 240);
    trIntegerValue(event, "mode",   TR_COLOR_RGBX_8888);
}

void TestPlayerFramework(CuTest* tc)
{
    TREvent* event;
    Player   player;

    /* Register our pixmap class */
    trRegisterClass(PixmapClassId, "Pixmap", 0, createPixmap, destroyPixmap, serializePixmap, 0);
    trOpenOutputStream("stdio", "ascii",  "tracer", "log", 0);
    trOpenOutputStream("stdio", "binary", "tracer", "bin", 0);

    /* Write it out */
    event = trBeginEvent(TR_TEST_API, "testFunction");
    CuAssertPtrNotNull(tc, event);
    trObjectValue(event, "obj", PixmapClassId, (TRhandle)0xbadf00d);
    trEndEvent(event);

    /* Tear down and open the same stream for reading */
    trTerminate();
    trRegisterClass(PixmapClassId, "Pixmap", 0, createPixmap, destroyPixmap, serializePixmap, 0);
    trOpenInputStream("stdio", "binary", TRACEFILE, 0);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    trPlay(&player);
    CuAssertIntEquals(tc, TR_NO_ERROR, trGetError());

    /* Make sure the bitmap was created correctly */
    CuAssertIntEquals(tc, 320,                player.pixmapWidth);
    CuAssertIntEquals(tc, 240,                player.pixmapHeight);
    CuAssertIntEquals(tc, TR_COLOR_RGBX_8888, player.pixmapMode);

    trTerminate();
}

CuSuite* CuPlayerGetSuite(void)
{
    CuSuite* suite = CuSuiteNew();
  
    SUITE_ADD_TEST(suite, TestPlayerFramework);
  
    return suite;
}
