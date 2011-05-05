/**
 *  GTK trace player framework implementation
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

#include "tracer_player_gtk.h"
#include "tracer_player.h"
#include "tracer_util.h"
#include "tracer_platform.h"
#include "tracer.h"

#include <sys/stat.h>
#include <string.h>
#include <gdk/gdkx.h>

#define CONFIG_FILE "player.ini"

const int           defaultWindowWidth       = 640;
const int           defaultWindowHeight      = 480;
const TRColorFormat defaultWindowColorFormat = TR_COLOR_RGBX_8888;

/* Event handlers */
static gboolean deleteEvent(GtkWidget* widget, GdkEvent* event, gpointer data);
static void destroyEvent(GtkWidget* widget, gpointer data);
static gboolean exposeEvent(GtkWidget* widget, GdkEventExpose* event, gpointer data);


void reportError(TracePlayerState* playerState, const TRbyte* message)
{
    GtkWidget* dialog;

    dialog = gtk_message_dialog_new(playerState->mainWindow, 
                                    GTK_DIALOG_DESTROY_WITH_PARENT, 
                                    GTK_MESSAGE_ERROR, 
                                    GTK_BUTTONS_CLOSE,
                                    "Trace Player Error: %s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
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
            GtkWidget* dialog;

            dialog = gtk_file_chooser_dialog_new("Open Trace File", playerState->mainWindow,
                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                 GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, 
                                                 NULL);
            if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
            {
                char* fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
                strncpy(playerState->traceFileName, fileName, sizeof(playerState->traceFileName));
                g_free(fileName);
                gtk_widget_destroy(dialog);
            }
            else
            {
                gtk_widget_destroy(dialog);
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
            mkdir(playerState->instrumentationOutputPath, 0700);
            
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
    /* Record the base time stamp */
    if (!playerState->firstEventTime)
    {
        playerState->firstEventTime = trGetSystemTime();
    }

    /* Run the player */
    while (!playerState->done)
    {
        TREvent* event = trPlaySingleEvent(playerState);
        
        if (!event)
        {
            playerState->done = TR_TRUE;
            
            /* Display the FPS esimate */
            {
                TRtimestamp now  = trGetSystemTime();
                TRtimestamp diff = now - playerState->firstEventTime;
                if (playerState->frameCount && diff)
                {
                    TRfloat fps  = (1000000.0 * playerState->frameCount) / diff;
                    printf("Average FPS: %.02f\n", fps);
                }
            }

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
            playerState->frameCount++;
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
    GtkWindow* window = NULL;
    TR_UNUSED(colorFormat);
    
    window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    
    if (window)
    {
        gtk_window_set_title(window, playerState->appName);
        gtk_window_set_default_size(window, width, height);
        gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
        gtk_widget_set_double_buffered(GTK_WIDGET(window), FALSE);

        /* Connect the events */
        g_signal_connect(G_OBJECT(window), "destroy",      G_CALLBACK(destroyEvent), NULL);
        g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(deleteEvent), NULL);
        g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(exposeEvent), playerState);

        if (!playerState->mainWindow)
        {
            playerState->mainWindow = window;
        }

        gtk_widget_show_all(GTK_WIDGET(window));

        /* 
         * This is a hack to ensure the window is really created before 
         * we return it to the trace player
         */
        while (gtk_events_pending())
        {
            gtk_main_iteration();
        }

        /* Make sure the window was realized */
        TR_ASSERT(GTK_WIDGET(window)->window);
        
        /* Make sure we have the native X window for the widget */
        TR_ASSERT(GDK_WINDOW_XWINDOW(GTK_WIDGET(window)->window));
        
        return (TRNativeWindow)GDK_WINDOW_XWINDOW(GTK_WIDGET(window)->window);
    }
    return NULL;
}

void trPlayerDestroyWindow(TRPlayer player, TRNativeWindow window)
{
    TracePlayerState* playerState = (TracePlayerState*)player;

    /* TODO: Only the top level window can be destroyed */
    if (playerState->mainWindow)
    {
        gtk_widget_destroy(GTK_WIDGET(playerState->mainWindow));
        playerState->mainWindow = NULL;
    }
}

void trPlayerRefreshWindow(TRPlayer player, TRNativeWindow window)
{
    TR_UNUSED(player);
    TR_UNUSED(window);
}

TRNativePixmap trPlayerCreatePixmap(TRPlayer player, TRint width, TRint height, TRColorFormat colorFormat)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    GdkPixmap* pixmap = NULL;
    int depth = 0;

    /* Create a window if one doesn't exist */
    if (!playerState->mainWindow)
    {
        trPlayerCreateWindow(player, 0, 0, defaultWindowWidth, defaultWindowHeight, defaultWindowColorFormat);
    }

    switch (colorFormat)
    {
    case TR_COLOR_RGBX_8888:
    case TR_COLOR_RGBA_8888:
        depth = 32;
        break;
    case TR_COLOR_RGB_888:
        depth = 24;
        break;
    case TR_COLOR_RGB_565:
        depth = 16;
        break;
    case TR_COLOR_RGBA_5551:
        depth = 15;
        break;
    case TR_COLOR_RGBA_4444:
        depth = 16;
        break;
    case TR_COLOR_L_8:
        /* FIXME: allocate a palette here */
        depth = 8;
        break;
    case TR_COLOR_BW_1:
        depth = 1;
        break;
    default:
        TR_ASSERT(!"Invalid color mode");
    }

    pixmap = gdk_pixmap_new(NULL, width, height, depth);

    return (TRNativePixmap)GDK_PIXMAP_XID(pixmap);
}

void trPlayerDestroyPixmap(TRPlayer player, TRNativePixmap pixmap_)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    
    /* TODO: Only the top current pixmap can be destroyed */
    if (playerState->visibleBitmap)
    {
        gdk_pixmap_unref(playerState->visibleBitmap);
        playerState->visibleBitmap = 0;
    }
}

void trPlayerDisplayPixmap(TRPlayer player, TRNativePixmap pixmap_)
{
    TracePlayerState* playerState = (TracePlayerState*)player;
    GdkPixmap* pixmap = (GdkPixmap*)pixmap_;
    
    if (pixmap && playerState->mainWindow)
    {
        /* TODO: Set the visible bitmap */

        /* TODO: Resize window client area */

        /* TODO: Refresh the whole window */
    }
}

static gboolean deleteEvent(GtkWidget* widget, GdkEvent* event, gpointer data)
{
    /* Always allow the window to be destroyed */
    return FALSE;
}

static void destroyEvent(GtkWidget* widget, gpointer data)
{
    if (gtk_main_level() > 0)
    {
        gtk_main_quit();
    }
}

static gboolean exposeEvent(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    TracePlayerState* playerState = (TracePlayerState*)data;
    
    if (playerState->visibleBitmap)
    {
        /* TODO: Render the bitmap */
    }
    
    return TRUE;
}

static gboolean idleEvent(gpointer data)
{
    TracePlayerState* playerState = (TracePlayerState*)data;
    static TRfloat fpsEstimate = 0.0;

    play(playerState);

    /* Update the FPS estimate text */
    if (playerState->mainWindow && fpsEstimate != trGetFpsEstimate())
    {
        TRbyte title[256];
        fpsEstimate = trGetFpsEstimate();
        sprintf(title, "%s [%.2f FPS]", playerState->appName, fpsEstimate);
        gtk_window_set_title(playerState->mainWindow, title);
    }

    if (playerState->done)
    {
        gtk_main_quit();
    }

    return !playerState->done;
}

int main(int argc, char** argv)
{
    TracePlayerState* playerState;
    int i;
    
    /* Bring up GTK */
    gtk_init(&argc, &argv);
    
    playerState = (TracePlayerState*)triMalloc(triGetState(), sizeof(TracePlayerState));
    TR_ASSERT(playerState);

    /* Initialize the state */
    playerState->state                = triGetState();
    playerState->appName              = "Trace Player";
    playerState->mainWindow           = 0;
    playerState->visibleBitmap        = 0;
    playerState->instrumentation      = 0;
    playerState->done                 = TR_FALSE;
    playerState->terminate            = TR_FALSE;
    playerState->profile              = TR_FALSE;
    playerState->captureFrameBuffer   = TR_FALSE;
    playerState->firstEventTime       = 0;
    playerState->frameCount           = 0;
    
    strncpy(playerState->configFileName,       CONFIG_FILE, sizeof(playerState->configFileName));
    strncpy(playerState->defaultTraceFileName, "",          sizeof(playerState->defaultTraceFileName));
    
    /* Parse the command line arguments */
    for (i = 1; i < argc; i++)
    {
        const char* arg = argv[i];
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
    }
  
    /* Load the player */
    if (!initialize(playerState))
    {
        return 1;
    }

    /* Schedule the player */
    g_idle_add(idleEvent, playerState);

    /* Main loop */
    gtk_main();

    /* Tear down */
    terminate(playerState);
    triFree(triGetState(), playerState);

    return 0;
}
