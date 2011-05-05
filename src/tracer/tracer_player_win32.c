/**
 *  Win32 trace player framework implementation
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

#include "tracer_player_win32.h"
#include "tracer_player.h"
#include "tracer_util.h"
#include "tracer_platform.h"
#include "tracer.h"

#define CONFIG_FILE "player.ini"

const int           defaultWindowWidth       = 640;
const int           defaultWindowHeight      = 480;
const TRColorFormat defaultWindowColorFormat = TR_COLOR_RGBX_8888;

void reportError(TracePlayerState* playerState, const TRbyte* message)
{
    TR_UNUSED(playerState);
    MessageBox(NULL, message, "Trace Player Error", MB_OK | MB_ICONERROR);
}

TRbool initialize(TracePlayerState* playerState)
{
#if 0
    /* Read the configuration */
    trLoadConfig(playerState->configFileName);
    
    /* If the config file couldn't be found, open the streams manually */
    if (trGetError() != TR_NO_ERROR)
#else
    if (1)
#endif
    {
        if (strlen(playerState->defaultTraceFileName))
        {
            strncpy(playerState->traceFileName, playerState->defaultTraceFileName, sizeof(playerState->traceFileName));
        }
        else
        {
            /* Ask the user for a file */
            OPENFILENAME ofn;
            
            playerState->traceFileName[0] = 0;

            memset(&ofn, 0, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.lpstrFilter = "*.bin";
            ofn.lpstrFile   = playerState->traceFileName;
            ofn.nMaxFile    = sizeof(playerState->traceFileName);
            ofn.lpstrTitle  = "Open Trace File";
            ofn.Flags       = OFN_ENABLESIZING | OFN_FILEMUSTEXIST;
            
            if (!GetOpenFileName(&ofn))
            {
                return TR_FALSE;
            }
        }
        trOpenInputStream("stdio", "binary", playerState->traceFileName, 0);
    }

    if (trGetError() != TR_NO_ERROR)
    {
        reportError(playerState, "Unable to open trace file.");
        return TR_FALSE;
    }
    
    /* Initialize the instrumentation */
    if (playerState->profile)
    {
        playerState->instrumentation        = trInitializeInstrumentation();
        playerState->instrumentationLogFile = 0;

        /* Prepare the instrumentation data directory */
        if (playerState->instrumentation && triStringLength(playerState->traceFileName))
        {
            TRbyte* fileName;
            TRint i;
            
            for (i = 0; i < triStringLength(playerState->traceFileName); i++)
            {
                if (playerState->traceFileName[i] == '.')
                {
                    if (i < sizeof(playerState->traceFileName) - 5)
                    {
                        triMemCopy(&playerState->instrumentationOutputPath[i], "_inst", 6);
                        break;
                    }
                    break;
                }
                playerState->instrumentationOutputPath[i] = playerState->traceFileName[i];
            }
            /* Create the instrumentation data directory */
            mkdir(playerState->instrumentationOutputPath);
            
            /* Open the instrumentation log file */
            fileName = triStringFormat(playerState->state, "%s\\instrumentation.log", playerState->instrumentationOutputPath);
            playerState->instrumentationLogFile = triOpenFile(playerState->state, fileName, "w");
            triFree(playerState->state, fileName);
            
            /* Dump the counter names and descriptions */
            {
                const TRArray* counters = trUpdatePerformanceCounters(playerState->instrumentation);
                
                TR_FOR_EACH(const TRPerformanceCounter*, counter, counters)
                {
                    TRbyte* msg = triStringFormat(playerState->state, "desc %s %s\n", counter->name, counter->description);
                    triWriteFile(playerState->instrumentationLogFile, msg, triStringLength(msg));
                    triFree(playerState->state, msg);
                }
            }
        }
    }
    
    return TR_TRUE;
}

void terminate(TracePlayerState* playerState)
{
    if (playerState->instrumentation)
    {
        triCloseFile(playerState->instrumentationLogFile);
        trDestroyInstrumentation(playerState->instrumentation);
    }
    trTerminate();
}

#define LOG(TEXT) \
  msg = triStringFormat(playerState->state, "%s", TEXT); \
  triWriteFile(playerState->instrumentationLogFile, msg, triStringLength(msg)); \
  triFree(playerState->state, msg);

#define LOG1(FORMAT, P1) \
  msg = triStringFormat(playerState->state, FORMAT, P1); \
  triWriteFile(playerState->instrumentationLogFile, msg, triStringLength(msg)); \
  triFree(playerState->state, msg);

#define LOG2(FORMAT, P1, P2) \
    { \
        msg = triStringFormat(playerState->state, FORMAT, P1, P2); \
        triWriteFile(playerState->instrumentationLogFile, msg, triStringLength(msg)); \
        triFree(playerState->state, msg); \
        msg = 0; \
    }

void play(TracePlayerState* playerState)
{
    /* Run the player */
    while (!playerState->done)
    {
        TREvent* event = trPlaySingleEvent(playerState);
        
        if (!event)
        {
            playerState->done = TR_TRUE;
          
            /* If we were profiling the trace or there is no visible window, we can quit once we reach the end */
            if (playerState->profile || !playerState->mainWindow)
            {
                playerState->terminate = TR_TRUE;
            }
            break;
        }

#if 1
        /* Log the event */
        triLogMessage(playerState->state, "%010d %s", event->sequenceNumber, event->function->name);
#endif

        /* Log and update performance counters */        
        if (event->function->flags & (TR_FUNC_RENDER_CALL | TR_FUNC_FRAME_MARKER) && 
            playerState->instrumentation && playerState->instrumentationLogFile)
        {
            const TRArray* counters = trUpdatePerformanceCounters(playerState->instrumentation);
            TRbyte* msg;
            
            LOG2("event %s %d\n", event->function->name, event->sequenceNumber);
            {
                TR_FOR_EACH(const TRPerformanceCounter*, counter, counters)
                {
                    switch (counter->type)
                    {
                    case TR_INTEGER:
                        LOG2("value int %s %d\n", counter->name, counter->value.i);
                        break;
                    case TR_FLOAT:
                        LOG2("value float %s %f\n", counter->name, counter->value.f);
                        break;
                    default:
                        continue;
                    }
                }
            }
            
            /* Save the frame buffer */
            if (playerState->captureFrameBuffer)
            {
                const TRInstrumentationSurface* surf = trLockInstrumentationSurface(playerState->instrumentation);

                if (surf)
                {
                    LOG1("value int render_surface_width %u\n", surf->width);
                    LOG1("value int render_surface_height %u\n", surf->height);
                    LOG1("value int color_stride %u\n", surf->colorStride);
                    LOG1("value int depth_stride %u\n", surf->depthStride);
                    LOG1("value int stencil_stride %u\n", surf->stencilStride);
                    LOG1("value int red_mask %u\n", surf->colorMasks[0]);
                    LOG1("value int green_mask %u\n", surf->colorMasks[1]);
                    LOG1("value int blue_mask %u\n", surf->colorMasks[2]);
                    LOG1("value int alpha_mask %u\n", surf->colorMasks[3]);
                    LOG1("value int depth_mask %u\n", surf->depthMask);
                    LOG1("value int color_data_type %u\n", surf->colorDataType);
                    LOG1("value int depth_data_type %u\n", surf->depthDataType);
                    LOG1("value int stencil_data_type %u\n", surf->stencilDataType);
                    LOG1("value int stencil_mask %u\n", surf->stencilMask);
                    LOG1("value int is_linear %u\n", surf->isLinear);
                    LOG1("value int is_premultiplied %u\n", surf->isPremultiplied);
                    if (surf->colorPixels)
                    {
                        TRbyte* fileName = triStringFormat(playerState->state, "%s\\colorbuffer%08d.dat", playerState->instrumentationOutputPath, event->sequenceNumber);
                        TRFile f = triOpenFile(playerState->state, fileName, "wb");
                        triFree(playerState->state, fileName);
                        if (f)
                        {
                            TRbyte* fileName = triStringFormat(playerState->state, "colorbuffer%08d.dat", event->sequenceNumber);
                            LOG1("value str color_buffer %s\n", fileName);
                            triFree(playerState->state, fileName);
                            triWriteFile(f, (const TRbyte*)surf->colorPixels, surf->colorStride * surf->height);
                            triCloseFile(f);
                        }
                    }
                    if (surf->depthPixels)
                    {
                        TRbyte* fileName = triStringFormat(playerState->state, "%s\\depthbuffer%08d.dat", playerState->instrumentationOutputPath, event->sequenceNumber);
                        TRFile f = triOpenFile(playerState->state, fileName, "wb");
                        triFree(playerState->state, fileName);
                        if (f)
                        {
                            TRbyte* fileName = triStringFormat(playerState->state, "depthbuffer%08d.dat", event->sequenceNumber);
                            LOG1("value str depth_buffer %s\n", fileName);
                            triFree(playerState->state, fileName);
                            triWriteFile(f, (const TRbyte*)surf->depthPixels, surf->depthStride * surf->height);
                            triCloseFile(f);
                        }
                    }
                    if (surf->stencilPixels)
                    {
                        TRbyte* fileName = triStringFormat(playerState->state, "%s\\stencilbuffer%08d.dat", playerState->instrumentationOutputPath, event->sequenceNumber);
                        TRFile f = triOpenFile(playerState->state, fileName, "wb");
                        triFree(playerState->state, fileName);
                        if (f)
                        {
                            TRbyte* fileName = triStringFormat(playerState->state, "stencilbuffer%08d.dat", event->sequenceNumber);
                            LOG1("value str stencil_buffer %s\n", fileName);
                            triFree(playerState->state, fileName);
                            triWriteFile(f, (const TRbyte*)surf->stencilPixels, surf->stencilStride * surf->height);
                            triCloseFile(f);
                        }
                    }
                }
                trUnlockInstrumentationSurface(playerState->instrumentation);
            }
        }
        
        /* Leave this loop when the frame is complete */
        if (event->function->flags & TR_FUNC_FRAME_MARKER)
        {
            break;
        }
    }
}

/*
 *  Trace player framework API
 */
 
TRNativeWindow trPlayerCreateWindow(TRPlayer player, TRint x, TRint y, TRint width, TRint height, TRColorFormat colorFormat)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    HWND hWnd;
    TR_UNUSED(colorFormat);
    
    hWnd = CreateWindow(playerState->appName, playerState->appName, 
                        WS_OVERLAPPED | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
                        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                        NULL, NULL, playerState->instance, NULL);

    if (hWnd)
    {
        if (!playerState->mainWindow)
        {
            playerState->mainWindow = hWnd;
        }
        
        SetWindowLong(hWnd, GWL_USERDATA, (LONG)player);
        MoveWindow(hWnd, x, y, width, height, FALSE);
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
    return (TRNativeWindow)hWnd;
}

void trPlayerDestroyWindow(TRPlayer player, TRNativeWindow window)
{
    HWND hWnd = (HWND)window;
    TR_UNUSED(player);
    DestroyWindow(window);
}

void trPlayerRefreshWindow(TRPlayer player, TRNativeWindow window)
{
    TR_UNUSED(player);
    TR_UNUSED(window);
}

TRNativePixmap trPlayerCreatePixmap(TRPlayer player, TRint width, TRint height, TRColorFormat colorFormat)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    HDC        dc;
    HBITMAP    bitmap;
	BITMAPINFO bmi;
	DWORD*     pixels = 0;
	DWORD*     mask   = 0;
	DWORD*     bits   = 0;

    struct
    {
        BITMAPINFO  bmi;
        RGBQUAD     quads[256];
    } bitmapHeader;
    
    /* Create a window if one doesn't exist */
    if (!playerState->mainWindow)
    {
        playerState->mainWindow = (HWND)trPlayerCreateWindow(player, 0, 0, defaultWindowWidth, defaultWindowHeight, defaultWindowColorFormat);
    }
    
    dc = GetDC(playerState->mainWindow);
	
	memset(&bitmapHeader, 0, sizeof(bitmapHeader));
    bitmapHeader.bmi.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
    bitmapHeader.bmi.bmiHeader.biWidth           = width;
    bitmapHeader.bmi.bmiHeader.biHeight          = -height;
    bitmapHeader.bmi.bmiHeader.biPlanes          = 1;
    bitmapHeader.bmi.bmiHeader.biCompression     = BI_BITFIELDS;
    bitmapHeader.bmi.bmiHeader.biSizeImage       = 0;
    bitmapHeader.bmi.bmiHeader.biXPelsPerMeter   = 0;
    bitmapHeader.bmi.bmiHeader.biYPelsPerMeter   = 0;
    bitmapHeader.bmi.bmiHeader.biClrUsed         = 3;
    bitmapHeader.bmi.bmiHeader.biClrImportant    = 0;
    
    mask = (DWORD*)bitmapHeader.bmi.bmiColors;
    bits = (DWORD*)&bitmapHeader.bmi.bmiHeader.biBitCount;

	switch (colorFormat)
	{
    case TR_COLOR_RGBX_8888:
    case TR_COLOR_RGBA_8888:
        *bits = 32;
        mask[0] = 0x00ff0000;
        mask[1] = 0x0000ff00;
        mask[2] = 0x000000ff;
        mask[3] = 0xff000000;
        break;
    case TR_COLOR_RGB_888:
        *bits = 24;
        mask[0] = 0x00ff0000;
        mask[1] = 0x0000ff00;
        mask[2] = 0x000000ff;
        break;
    case TR_COLOR_RGB_565:
        *bits = 16;
        mask[0] = 0xf800;
        mask[1] = 0x07e0;
        mask[2] = 0x001f;
        break;
    case TR_COLOR_RGBA_5551:
        *bits = 15;
        mask[0] = 0x7c00;
        mask[1] = 0x03e0;
        mask[2] = 0x001f;
        break;
    case TR_COLOR_RGBA_4444:
        *bits = 16;
        mask[0] = 0xf800;
        mask[1] = 0x07e0;
        mask[2] = 0x001f;
        break;
    case TR_COLOR_L_8:
        /* FIXME: allocate a palette here */
        *bits = 8;
        break;
    case TR_COLOR_BW_1:
        *bits = 1;
        break;
    default:
        TR_ASSERT(!"Invalid color mode");
	}

    bitmap = CreateDIBSection(dc, &bitmapHeader.bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
    ReleaseDC(playerState->mainWindow, dc);
    
    return (TRNativePixmap)bitmap;
}

void trPlayerDestroyPixmap(TRPlayer player, TRNativePixmap pixmap)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    HBITMAP bitmap = (HBITMAP)pixmap;
    TR_UNUSED(player);
    
    if (playerState->visibleBitmap == bitmap)
    {
        playerState->visibleBitmap = 0;
    }
    
    DeleteObject(bitmap);
}

void trPlayerDisplayPixmap(TRPlayer player, TRNativePixmap pixmap)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    HBITMAP bitmap = (HBITMAP)pixmap;
    
    if (bitmap && playerState->mainWindow)
    {
        RECT windowRect, clientRect;
        DIBSECTION dibSection;
      
        /* Set the visible bitmap */
        playerState->visibleBitmap = bitmap;
        
        /* Resize window client area */
        if (GetObject(playerState->visibleBitmap, sizeof(dibSection), &dibSection))
        {
            SIZE size;
            size.cx = dibSection.dsBm.bmWidth;
            size.cy = dibSection.dsBm.bmHeight;
            GetWindowRect(playerState->mainWindow, &windowRect);
            GetClientRect(playerState->mainWindow, &clientRect);
            MoveWindow(playerState->mainWindow, 
                       windowRect.left, windowRect.top, 
                       (windowRect.right  - windowRect.left) - (clientRect.right)  + size.cx,
                       (windowRect.bottom - windowRect.top)  - (clientRect.bottom) + size.cy, TRUE);
        }

        /* Refresh the whole window */
        InvalidateRect(playerState->mainWindow, NULL, TRUE);
        UpdateWindow(playerState->mainWindow);
    }
}

static LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    TracePlayerState* playerState = (TracePlayerState*)GetWindowLong(hWnd, GWL_USERDATA);
  
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        if (hWnd == playerState->mainWindow)
        {
            playerState->terminate = TR_TRUE;
        }
        break;
    case WM_PAINT:
    {
        if (playerState->visibleBitmap)
        {
            PAINTSTRUCT ps;
            DIBSECTION dibSection;

            HDC dc = BeginPaint(hWnd, &ps);
            HDC bitmapDc = CreateCompatibleDC(dc);
            HDC prevDc = SelectObject(bitmapDc, playerState->visibleBitmap);
            
            if (GetObject(playerState->visibleBitmap, sizeof(dibSection), &dibSection))
            {
                SIZE size;
                size.cx = dibSection.dsBm.bmWidth;
                size.cy = dibSection.dsBm.bmHeight;
                BitBlt(dc, 0, 0, size.cx, size.cy, bitmapDc, 0, 0, SRCCOPY);
            }
            SelectObject(bitmapDc, prevDc);
            EndPaint(hWnd, &ps);
        }
    }
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
        /* fallthrough */
	default:
		return DefWindowProc (hWnd, uMsg, wParam, lParam);
		break;
    }
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndClass;
    TracePlayerState* playerState;
    TRfloat fpsEstimate = 0.0;
  
    TR_UNUSED(hPrevInstance);
    TR_UNUSED(lpCmdLine);
    
    playerState = (TracePlayerState*)triMalloc(triGetState(), sizeof(TracePlayerState));
    TR_ASSERT(playerState);

    /* Initialize the state */
    playerState->state                = triGetState();
    playerState->appName              = "Trace Player";
    playerState->instance             = hInstance;
    playerState->mainWindow           = 0;
    playerState->visibleBitmap        = 0;
    playerState->instrumentation      = 0;
    playerState->done                 = TR_FALSE;
    playerState->terminate            = TR_FALSE;
    playerState->profile              = TR_FALSE;
    playerState->captureFrameBuffer   = TR_FALSE;
    
    strncpy(playerState->configFileName,       CONFIG_FILE, sizeof(playerState->configFileName));
    strncpy(playerState->defaultTraceFileName, "",          sizeof(playerState->defaultTraceFileName));
    
    /* Parse the command line arguments */
    {
        const char* arg = strtok(lpCmdLine, " ");
        
        while (arg)
        {
            if (arg[0] == '-')
            {
                if (!strcmp(arg, "--save-frames"))
                {
                    playerState->captureFrameBuffer = TR_TRUE;
                }
                else if (!strcmp(arg, "--profile"))
                {
                    playerState->profile = TR_TRUE;
                }
                else if (!strcmp(arg, "--synchronize"))
                {
                    playerState->state->synchronousPlayback = TR_TRUE;
                }
            }
            else
            {
                strncpy(playerState->defaultTraceFileName, arg, sizeof(playerState->defaultTraceFileName));
            }
            arg = strtok(NULL, " ");
        }
    }
  
    /* Register the window class */
    wndClass.style          = 0;
    wndClass.lpfnWndProc    = (WNDPROC)WindowProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(hInstance, playerState->appName);
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = 0;
    wndClass.lpszMenuName   = playerState->appName;
    wndClass.lpszClassName  = playerState->appName;
  
    if (!RegisterClass(&wndClass))
    {
        return FALSE;
    }

    /* Load the player */
    if (!initialize(playerState))
    {
        return FALSE;
    }

    /* Message loop */
    while (!playerState->terminate)
    {
        /*
         *  Switch off the busy loop once the trace is played to the end.
         */
        if (playerState->done || PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
        {
            if (GetMessage(&msg, NULL, 0, 0) )
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                playerState->done = TR_TRUE;
            }
        }
        play(playerState);

        /* Update the FPS estimate text */
        if (playerState->mainWindow && fpsEstimate != trGetFpsEstimate())
        {
            TRbyte title[256];
            fpsEstimate = trGetFpsEstimate();
            sprintf(title, "Trace Player [%.2f FPS]", fpsEstimate);
            SetWindowText(playerState->mainWindow, title);
        }
    }

    /* Tear down */
    terminate(playerState);
    triFree(triGetState(), playerState);

    return TRUE;
}
