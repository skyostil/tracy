/**
 *  Symbian trace player framework implementation
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

#include "tracer_player_symbian.h"
#include "tracer_player.h"
#include "tracer_util.h"
#include "tracer.h"
#include <aknnotewrappers.h>
/*
 *  Main entry point
 */

CApaApplication* NewApplication()
{
    return new TracePlayerApp;
}

TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

/*
 *  Trace player application
 */

CApaDocument* TracePlayerApp::CreateDocumentL()
{
    return TracePlayerDocument::NewL(*this);
}

TUid TracePlayerApp::AppDllUid() const
{
    return KUidTracePlayerApplication;
}

/*
 *  Trace player document
 */
 
TracePlayerDocument* TracePlayerDocument::NewL(CEikApplication& aApp)
{
    TracePlayerDocument* self = new (ELeave) TracePlayerDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    return self;
}

TracePlayerDocument::TracePlayerDocument(CEikApplication& aApp): CAknDocument(aApp)
{
}

TracePlayerDocument::~TracePlayerDocument()
{
}

#if defined(__WINS__)
#define INPUT_PATH  "c:\\player.bin"
#define CONFIG_FILE "c:\\player.ini"
#else
#define INPUT_PATH  "e:\\player.bin"
#define CONFIG_FILE "e:\\player.ini"
#endif

void TracePlayerDocument::ConstructL()
{
}

CEikAppUi* TracePlayerDocument::CreateAppUiL()
{
    return new (ELeave) TracePlayerAppUi;
}

/*
 *  Trace player UI
 */

void TracePlayerAppUi::ConstructL()
{
    BaseConstructL();
    iAppContainer = new (ELeave) TracePlayerContainer;
    iAppContainer->SetMopParent(this);
    iAppContainer->ConstructL();
    AddToStackL(iAppContainer);
    
    /* Create a timer for running the animation */
    iPeriodic              = CPeriodic::NewL(CActive::EPriorityIdle);
    iState                 = Stopped;
    iFirstEventTime        = 0;
    iFirstEventTimestamped = EFalse;
    iFrameCount            = 0;
    
    Start();
    
    iPeriodic->Start(100, 100, TCallBack(AdvancePlayer, this));
}

void TracePlayerAppUi::Stop()
{
    trTerminate();
    iState = Stopped;
}

void TracePlayerAppUi::Start()
{
    /* Read the configuration */
    trLoadConfig(CONFIG_FILE);
    
    /* If the config file couldn't be found, open the hardcoded streams */
    if (trGetError() != TR_NO_ERROR)
    {
        trOpenInputStream("symbian", "binary", INPUT_PATH, 0);
    }

    if (trGetError() == TR_NO_ERROR)
    {
        iState                 = Running;
        iFirstEventTimestamped = EFalse;
        iFrameCount            = 0;
    }
    else
    {
        ShowMessage(_L("Unable to open trace file."));
    }
    
}

void TracePlayerAppUi::Pause()
{
    iState = Stopped;
}

void TracePlayerAppUi::Resume()
{
    iState = Running;
}

void TracePlayerAppUi::ShowMessage(const TDesC& aMessage)
{
    CAknInformationNote* note = new CAknInformationNote(ETrue);
    note->ExecuteLD(aMessage);
}

TracePlayerAppUi::~TracePlayerAppUi()
{
    delete iPeriodic;
    if (iAppContainer)
    {
        RemoveFromStack(iAppContainer);
        delete iAppContainer;
    }
    trTerminate();
}

TInt TracePlayerAppUi::AdvancePlayer(TAny* aInstance)
{
    TracePlayerAppUi* appUi = (TracePlayerAppUi*)aInstance;
    TREvent* event;
  
    /* Run the player */
    while (appUi->iState == Running)
    {
        /* Record the time stamp of the first played event */
        if (!appUi->iFirstEventTimestamped)
        {
            appUi->iFirstEventTime        = trGetSystemTime();
            appUi->iFirstEventTimestamped = ETrue;
        }
      
        /* Play back the event */
        event = trPlaySingleEvent(aInstance);
        
        /* If this was the last event, display the measured FPS and stop */
        if (!event)
        {
            appUi->Stop();
            if (appUi->iFirstEventTimestamped && appUi->iFrameCount)
            {
                TRtimestamp diff = trGetSystemTime() - appUi->iFirstEventTime;
                if (diff)
                {
                    TBuf<128> msg;
                    TRulong fps = (10000000 * appUi->iFrameCount) / diff;
                    msg.Append(_L("Playback finished. Average FPS: "));
                    msg.AppendNum((TRint)(fps / 10));
                    msg.Append(_L("."));
                    msg.AppendNum((TRint)(fps % 10));
                    appUi->ShowMessage(msg);
                }
            }
            break;
        }
        
        if (event->function->flags & TR_FUNC_FRAME_MARKER)
        {
            appUi->iFrameCount++;
            break;
        }
    }
    
    return KErrNone;
}

RWindow* TracePlayerAppUi::CreateWindow(TRect aRect, TDisplayMode aMode)
{
#if 1
    TR_UNUSED(aRect);
    TR_UNUSED(aMode);
    return &iAppContainer->GetWindow();
#else
    /* The following panics for some reason -- just reuse the main application window for now */
    RWindow* window = new (ELeave) RWindow(iAppContainer->ControlEnv()->WsSession());
    
    if (window)
    {
        window->Construct(*iAppContainer->DrawableWindow(), (TUint32)window);
        //window->Construct(CCoeEnv::Static()->RootWin(), (TUint32)window);
        window->SetRequiredDisplayMode(aMode);
        window->SetSize(aRect.Size());
        window->SetPosition(aRect.iTl);
        window->SetVisible(ETrue);
        window->Activate();
    }
    
    return window;
#endif    
}

void TracePlayerAppUi::DestroyWindow(RWindow* aWindow)
{
#if 1
    TR_UNUSED(aWindow);
#else
    delete aWindow;
#endif    
}

void TracePlayerAppUi::RefreshWindow(RWindow* aWindow)
{
    TR_UNUSED(aWindow);
}

CFbsBitmap* TracePlayerAppUi::CreateBitmap(TSize aSize, TDisplayMode aMode)
{
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
    
    if (bitmap)
    {
        bitmap->Create(aSize, aMode);
    }
    return bitmap;
}

void TracePlayerAppUi::DestroyBitmap(CFbsBitmap* aBitmap)
{
    delete aBitmap;
}

void TracePlayerAppUi::DisplayBitmap(CFbsBitmap* aBitmap)
{
    iAppContainer->SetVisibleBitmap(aBitmap);
}

void TracePlayerAppUi::HandleCommandL(TInt aCommand)
{
    switch (aCommand)
    {
    case EAknSoftkeyBack:
    case EEikCmdExit:
    case 1:
        Exit();
        break;
    case 2:
        Stop();
        Start();
        break;
    default:
        break;      
    }
}

/*
 *  Trace player container
 */

void TracePlayerContainer::ConstructL()
{
    iVisibleBitmap = 0;
  
    CreateWindowL();
    SetExtentToWholeScreen();
    ActivateL();
}

TracePlayerContainer::~TracePlayerContainer()
{
}

void TracePlayerContainer::SetVisibleBitmap(CFbsBitmap* aBitmap)
{
    iVisibleBitmap = aBitmap;
    DrawNow();
}

void TracePlayerContainer::Draw(const TRect& aRect) const
{
    CWindowGc& gc = SystemGc();
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.SetBrushColor(0x000000);
    gc.DrawRect(aRect);
    
    if (iVisibleBitmap)
    {
        gc.BitBlt(TPoint(0, 0), iVisibleBitmap);
    }
}

RWindow& TracePlayerContainer::GetWindow() const
{
    return Window();
}

/*
 *  Trace player framework API
 */
 
static TDisplayMode colorFormatToDisplayMode(TRColorFormat colorFormat)
{
    switch (colorFormat)
    {
    case TR_COLOR_RGBX_8888:
        return EColor16MU;
    case TR_COLOR_RGBA_8888:
        return EColor16MA;
    case TR_COLOR_RGB_888:
        return EColor16M;
    case TR_COLOR_RGB_565:
        return EColor64K;
    case TR_COLOR_RGBA_5551:
        return ENone;
    case TR_COLOR_RGBA_4444:
        return EColor4K;
    case TR_COLOR_L_8:
        return EColor256;
    case TR_COLOR_BW_1:
        return EGray2;
    }
    return ENone;
}

TRNativeWindow trPlayerCreateWindow(TRPlayer player, TRint x, TRint y, TRint width, TRint height, TRColorFormat colorFormat)
{
    TracePlayerAppUi* app = (TracePlayerAppUi*)player;
    return app->CreateWindow(TRect(x, y, x + width, y + height), colorFormatToDisplayMode(colorFormat));
}

void trPlayerDestroyWindow(TRPlayer player, TRNativeWindow window)
{
    TracePlayerAppUi* app = (TracePlayerAppUi*)player;
    RWindow*          win = (RWindow*)window;
    return app->DestroyWindow(win);
}

void trPlayerRefreshWindow(TRPlayer player, TRNativeWindow window)
{
    TracePlayerAppUi* app = (TracePlayerAppUi*)player;
    RWindow*          win = (RWindow*)window;
    return app->RefreshWindow(win);
}

TRNativePixmap trPlayerCreatePixmap(TRPlayer player, TRint width, TRint height, TRColorFormat colorFormat)
{
    TracePlayerAppUi* app = (TracePlayerAppUi*)player;
    return app->CreateBitmap(TSize(width, height), colorFormatToDisplayMode(colorFormat));
}

void trPlayerDestroyPixmap(TRPlayer player, TRNativePixmap pixmap)
{
    TracePlayerAppUi* app = (TracePlayerAppUi*)player;
    CFbsBitmap*       bmp = (CFbsBitmap*)pixmap;
    return app->DestroyBitmap(bmp);
}

void trPlayerDisplayPixmap(TRPlayer player, TRNativePixmap pixmap)
{
    TracePlayerAppUi* app = (TracePlayerAppUi*)player;
    CFbsBitmap*       bmp = (CFbsBitmap*)pixmap;
    return app->DisplayBitmap(bmp);
}

