/**
 *  Symbian trace player framework interface
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
 
#ifndef TRACE_PLAYER_SYMBIAN_H
#define TRACE_PLAYER_SYMBIAN_H

#include <aknapp.h>
#include <akndoc.h>
#include <aknappui.h>
#include <coecntrl.h>
#include <eikstart.h>

#include "tracer.h"
#include "tracer_config_symbian.h"

const TUid KUidTracePlayerApplication = { TRACER_SYMBIAN_UID };

/**
 *  Trace player application class.
 */
class TracePlayerApp: public CAknApplication
{
private:
    /**
     *  Creates and returns a new document instance.
     */
    CApaDocument* CreateDocumentL();
    
    /**
     *  Returns the application ID.
     */
    TUid AppDllUid() const;
};

/**
 *  A trace file document.
 */
class TracePlayerDocument: public CAknDocument
{
public:
    /**
     *  Static constructor.
     */
    static TracePlayerDocument* NewL(CEikApplication& aApp);
    
    /**
     *  Destructor.
     */
    virtual ~TracePlayerDocument();
private:
    /**
     *  Private constructor.
     */
    TracePlayerDocument(CEikApplication& aApp);
    
    /**
     *  Second phase constructor.
     */
    void ConstructL();
    
    /**
     *  Creates and returns a new application UI object.
     */
    CEikAppUi* CreateAppUiL();
};

/**
 *  The trace player user interface.
 */
class TracePlayerAppUi: public CAknAppUi
{
public:
    /**
     *  Second phase constructor.
     */
    void ConstructL();
    
    /**
     *  Destructor.
     */
    virtual ~TracePlayerAppUi();
    
    RWindow*      CreateWindow(TRect aRect, TDisplayMode aMode);
    void          DestroyWindow(RWindow* aWindow);
    void          RefreshWindow(RWindow* aWindow);
    
    CFbsBitmap*   CreateBitmap(TSize aSize, TDisplayMode aMode);
    void          DestroyBitmap(CFbsBitmap* aBitmap);
    void          DisplayBitmap(CFbsBitmap* aBitmap);
    
    void          ShowMessage(const TDesC& aMessage);
    void          Stop();
    void          Start();
    void          Pause();
    void          Resume();
    
    void          HandleCommandL(TInt aCommand);
private:
    /**
     *  Timer callback.
     */
    static TInt   AdvancePlayer(TAny* aInstance);
    
    enum State
    {
        Stopped,
        Running,
    };
    
    /** Player State */
    State                       iState;

    /** Created windows */
    RPointerArray<RWindow>      iWindows;

    /** Created pixmaps */
    RPointerArray<CFbsBitmap>   iPixmaps;
    
    /** Main application container */
    class TracePlayerContainer* iAppContainer;
    
    /** A timer for triggering the animation */
    CPeriodic* iPeriodic;
    
    /** Time of first played event */
    TRtimestamp                 iFirstEventTime;
    TBool                       iFirstEventTimestamped;
    TRint                       iFrameCount;
};

class TracePlayerContainer: public CCoeControl
{
public:
    /**
     *  Second phase constructor.
     */
    void ConstructL();
    
    /**
     *  Destructor.
     */
    virtual ~TracePlayerContainer();
    
    /**
     *  Make a specific bitmap visible on the tracer screen.
     *
     *  @param aBitmap Bitmap to show, or NULL to hide all bitmaps.
     */
    void SetVisibleBitmap(CFbsBitmap* aBitmap);
    
    RWindow& GetWindow() const;
    
private:
    /**
     *  Draw the contents of this control.
     */
    void Draw(const TRect& aRect) const;
    
    /** Currently shown bitmap */
    CFbsBitmap* iVisibleBitmap;
};

/**
 * Factory method used by the E32Main method to create a new application instance.
 */
LOCAL_C CApaApplication* NewApplication();

/**
 * Entry point to the EXE application. Creates new application instance and
 * runs it by giving it as parameter to EikStart::RunApplication() method.
 */
GLDEF_C TInt E32Main();

#endif // TRACE_PLAYER_SYMBIAN_H
