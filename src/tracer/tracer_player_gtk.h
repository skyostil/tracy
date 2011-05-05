/**
 *  GTK trace player framework interface
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
 
#ifndef TRACE_PLAYER_GTK_H
#define TRACE_PLAYER_GTK_H

#include "tracer.h"
#include "tracer_internal.h"
#include "tracer_instrumentation.h"

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

/**
 *  GTK trace player data
 */
typedef struct
{
    /** Tracer state */
    TRState*                state;
    
    /** Application name */
    const TRbyte*           appName;
    
    /** Configuration file path */
    TRbyte                  configFileName[PATH_MAX];
    
    /** Default trace file name */
    TRbyte                  defaultTraceFileName[PATH_MAX];
    
    /** Loaded trace file name */
    TRbyte                  traceFileName[PATH_MAX];
    
    /** Flag that indicates whether the trace has been played back to the end */
    TRbool                  done;

    /** Flag that indicates whether the player should be terminated */
    TRbool                  terminate;
    
    /** Main window */
    GtkWindow*              mainWindow;
    
    /** Currently visible bitmap */
    GdkPixmap*              visibleBitmap;
    
    /** Instrumentation handle */
    TRInstrumentationHandle instrumentation;
    
    /** Output file for instrumentation protocol */
    TRFile                  instrumentationLogFile;
    
    /** Output path for instrumentation data */
    TRbyte                  instrumentationOutputPath[PATH_MAX];

    /** Flag that indicates whether instrumentation data should be collected */
    TRbool                  profile;
    
    /** Flag that controls saving the frame buffer data */
    TRbool                  captureFrameBuffer;

    /** Time stamp of first played event */
    TRtimestamp             firstEventTime;

    /** Frame count */
    TRint                   frameCount;
} TracePlayerState;

/**
 *  Entry point to the application.
 */
int main(int argc, char** argv);

#endif // TRACE_PLAYER_GTK_H
