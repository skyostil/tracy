/**
 * Copyright (c) 2011 Nokia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// INCLUDE FILES
#include "TracePlayerContainer.h"
#include "TracePlayer.h"
#include <aknnotewrappers.h>
// ================= MEMBER FUNCTIONS =======================

RWindow* window = 0;

TSize createTSize(int w, int h)
{
    return TSize(w, h);
}

NativeWindowType createEGLNativeWindowType6(void* context, int x, int y, int w, int h, int colorFormat)
{
    return window;
}

EGLNativeDisplayType createEGLNativeDisplayType1(void* context)
{
    return EGL_DEFAULT_DISPLAY;
}

void destroyEGLNativeWindowType2(void* context, EGLNativeWindowType window)
{
}

void destroyEGLNativeDisplayType2(void* context, EGLNativeDisplayType display)
{
}

#define SET_ATTR(ENUM, VALUE) attrs[attrCount++] = (ENUM); attrs[attrCount++] = (VALUE);

EGLConfig createEGLConfig28(void* context,
    EGLint config_id, EGLint buffer_size, EGLint red_size, EGLint green_size, EGLint blue_size, EGLint alpha_size, 
    EGLint bind_to_texture_rgb, EGLint bind_to_texture_rgba, EGLint config_caveat, EGLint depth_size, EGLint level, 
    EGLint max_swap_interval, EGLint min_swap_interval, EGLint native_renderable, EGLint sample_buffers, EGLint samples, 
    EGLint stencil_size, EGLint surface_type, EGLint transparent_type, EGLint transparent_red, EGLint transparent_green, 
    EGLint transparent_blue, EGLint luminance_size, EGLint alpha_mask_size, EGLint color_buffer_type, EGLint conformant, EGLint renderable_type)
{
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint attrs[64], attrCount = 0, configCount;
    EGLConfig config = 0;
    int terminate = 0;
    
    SET_ATTR(EGL_BUFFER_SIZE,             buffer_size);
    SET_ATTR(EGL_RED_SIZE,                red_size);
    SET_ATTR(EGL_GREEN_SIZE,              green_size);
    SET_ATTR(EGL_BLUE_SIZE,               blue_size);
    SET_ATTR(EGL_ALPHA_SIZE,              alpha_size);
    SET_ATTR(EGL_ALPHA_MASK_SIZE,         alpha_mask_size);
    SET_ATTR(EGL_LUMINANCE_SIZE,          luminance_size);
    SET_ATTR(EGL_BIND_TO_TEXTURE_RGB,     bind_to_texture_rgb);
    SET_ATTR(EGL_BIND_TO_TEXTURE_RGBA,    bind_to_texture_rgba);
    SET_ATTR(EGL_CONFIG_CAVEAT,           config_caveat);
    SET_ATTR(EGL_DEPTH_SIZE,              depth_size);
    SET_ATTR(EGL_LEVEL,                   level);
    /*SET_ATTR(EGL_MAX_SWAP_INTERVAL,       max_swap_interval);*/
    /*SET_ATTR(EGL_MIN_SWAP_INTERVAL,       min_swap_interval);*/
    SET_ATTR(EGL_NATIVE_RENDERABLE,       native_renderable);
    SET_ATTR(EGL_SAMPLE_BUFFERS,          sample_buffers);
    SET_ATTR(EGL_SAMPLES,                 samples);
    SET_ATTR(EGL_STENCIL_SIZE,            stencil_size);
    SET_ATTR(EGL_SURFACE_TYPE,            surface_type);
    SET_ATTR(EGL_TRANSPARENT_TYPE,        transparent_type);
    SET_ATTR(EGL_TRANSPARENT_RED_VALUE,   transparent_red);
    SET_ATTR(EGL_TRANSPARENT_GREEN_VALUE, transparent_green);
    SET_ATTR(EGL_TRANSPARENT_BLUE_VALUE,  transparent_blue);
    SET_ATTR(EGL_RENDERABLE_TYPE,         renderable_type);
    attrs[attrCount] = EGL_NONE;

    eglChooseConfig(display, attrs, 0, 0, &configCount);
    
    /* Bring up EGL if it hasn't been already */
    if (eglGetError() == EGL_NOT_INITIALIZED)
    {
        eglInitialize(display, 0, 0);
        terminate = 1;
    }
    
    eglChooseConfig(display, attrs, &config, 1, &configCount);

    if (eglGetError() != EGL_SUCCESS)
    {
    	/* Something went wrong -> use the original config */
        config      = (EGLConfig)config_id;
        configCount = 1;
    }
    
    if (!configCount)
    {
        /* Drop some attributes that are usually not strictly required */
        EGLint i;
        for (i = 0; i < attrCount; i += 2)
        {
            switch (attrs[i])
            {
            case EGL_LEVEL:
            case EGL_MAX_SWAP_INTERVAL:
            case EGL_MIN_SWAP_INTERVAL:
            case EGL_STENCIL_SIZE:
            case EGL_TRANSPARENT_RED_VALUE:
            case EGL_TRANSPARENT_GREEN_VALUE:
            case EGL_TRANSPARENT_BLUE_VALUE:
                attrs[i + 1] = 0;
                break;
            case EGL_TRANSPARENT_TYPE:
                attrs[i + 1] = EGL_NONE;
                break;
            case EGL_BIND_TO_TEXTURE_RGB:
            case EGL_BIND_TO_TEXTURE_RGBA:
            case EGL_CONFIG_CAVEAT:
            case EGL_NATIVE_RENDERABLE:
                attrs[i + 1] = EGL_DONT_CARE;
                break;
            }
        }
        
        eglChooseConfig(display, attrs, &config, 1, &configCount);
        eglGetError();
        
        /* Nada? Lose the antialiasing */
        if (!configCount)
        {
            for (i = 0; i < attrCount; i += 2)
            {
                switch (attrs[i])
                {
                case EGL_SAMPLE_BUFFERS:
                case EGL_SAMPLES:
                    attrs[i + 1] = 0;
                    break;
                }
            }
            eglChooseConfig(display, attrs, &config, 1, &configCount);
            eglGetError();
        
            /* No luck? Fall back to a window surface */
            if (!configCount)
            {
                for (i = 0; i < attrCount; i += 2)
                {
                    if (attrs[i] == EGL_SURFACE_TYPE)      attrs[i + 1] = EGL_PBUFFER_BIT;
                }
                
                eglChooseConfig(display, attrs, &config, 1, &configCount);
                
                /* Still no luck? Use the original config */
                if (!configCount)
                {
                    config = (EGLConfig)config_id;
                }
            }
        }
    }
/*    
    if (terminate)
    {
        eglTerminate(display);
    }
*/    
    return config;
}

void destroyEGLConfig2(void* context, EGLConfig config)
{
}

NativePixmapType createNativePixmapType(void* context, int width, int height, int displayMode)
{
    CTracePlayerContainer* instance = (CTracePlayerContainer*) context;
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
    
    if (bitmap)
    {
        bitmap->Create(TSize(width, height), EColor16MU);
    }
    instance->iBitmap = bitmap;
    return (NativePixmapType)bitmap;
}

void destroyNativePixmapType(void* context, NativePixmapType bitmap)
{
    CTracePlayerContainer* instance = (CTracePlayerContainer*) context;
    instance->iBitmap = 0;
    delete (CFbsBitmap*)bitmap;
}

#undef SET_ATTR

#include "wiggly.inl"
//#include "decoratorlist-new-arrays.inl"
//#include "griddemo3.inl"
//#include "hb_clock3.inl"

// ---------------------------------------------------------
// CTracePlayerContainer::ConstructL(const TRect& aRect)
// EPOC two phased constructor
// ---------------------------------------------------------
//
void CTracePlayerContainer::ConstructL(const TRect& /*aRect*/)
    {
    iOpenVGInitialized = EFalse;
    CreateWindowL();

    SetExtentToWholeScreen();                // Take the whole screen into use
    ActivateL();
    
    window = &Window();
    iFirstEventTime = 0;
    iBitmap = NULL;
    
    iPeriodic = CPeriodic::NewL( CActive::EPriorityIdle );         // Create an active object for
                                                                  // animating the scene
    iPeriodic->Start( 100, 100, 
                      TCallBack( CTracePlayerContainer::DrawCallBack, this ) );
    }

// Destructor
CTracePlayerContainer::~CTracePlayerContainer()
    {
    delete iPeriodic;

    /* AppExit call is made to release 
       any allocations made in AppInit. */
    if ( iTracePlayer )
        {
        iTracePlayer->AppExit();
        delete iTracePlayer;
        }

    eglMakeCurrent( iEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglDestroySurface( iEglDisplay, iEglSurface ); 
    eglDestroyContext( iEglDisplay, iEglContext );
    eglTerminate( iEglDisplay );                   // Release resources associated
                                                   // with EGL and OpenGL ES
    }

// ---------------------------------------------------------
// CTracePlayerContainer::SizeChanged()
// Called by framework when the view size is changed
// ---------------------------------------------------------
//
void CTracePlayerContainer::SizeChanged()
    {
    if( iOpenVGInitialized && iTracePlayer )
        {
        TSize size;
        size = this->Size();
        }
    }


// ---------------------------------------------------------
// CTracePlayerContainer::HandleResourceChange(
//     TInt aType)
// Dynamic screen resize changes by calling the
// SetExtentToWholeScreen() method again.
// ---------------------------------------------------------
//
 void CTracePlayerContainer::HandleResourceChange(TInt aType)
    {
	switch( aType )
    	{
	    case KEikDynamicLayoutVariantSwitch:
		    SetExtentToWholeScreen();
		    break;
	    }
    }

// ---------------------------------------------------------
// CTracePlayerContainer::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CTracePlayerContainer::CountComponentControls() const
    {
    return 0;
    }

// ---------------------------------------------------------
// CTracePlayerContainer::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CTracePlayerContainer::ComponentControl(TInt /*aIndex*/ ) const
    {
    return NULL;
    }

// ---------------------------------------------------------
// CTracePlayerContainer::Draw(const TRect& aRect) const
// ---------------------------------------------------------
//
void CTracePlayerContainer::Draw(const TRect& /*aRect*/ ) const
    {
    if (iBitmap)
    {
        CWindowGc& gc = SystemGc();
        gc.BitBlt( TPoint( 0, 0 ), iBitmap );
    }
    }

// ---------------------------------------------------------
// CTracePlayerContainer::DrawCallBack( TAny* aInstance )
// Called by the CPeriodic in order to draw the graphics 
// ---------------------------------------------------------

void ShowMessage(const TDesC& aMessage)
{
    CAknInformationNote* note = new CAknInformationNote(ETrue);
    note->ExecuteLD(aMessage);
}


TInt CTracePlayerContainer::DrawCallBack( TAny* aInstance )
    {
    CTracePlayerContainer* instance = (CTracePlayerContainer*) aInstance;
    if (instance->iFrame < 0)
    {
        return 0;
    }
    
    if (!instance->iFirstEventTime)
    {
        instance->iFirstEventTime = User::NTickCount();
    }

    if (!playFrame(instance, instance->iFrame++))
    {
        TBuf<128> msg;
        TInt diff = User::NTickCount() - instance->iFirstEventTime;
        TInt fps = (100000 * instance->iFrame) / diff;
        msg.Append(_L("Playback finished. Average FPS: "));
        msg.AppendNum((TInt)(fps / 100));
        msg.Append(_L("."));
        msg.AppendNum((TInt)(fps % 100));
        instance->iFrame = -1;
        ShowMessage(msg);
    }
    instance->DrawNow( );
    return 0; 
    }

// ---------------------------------------------------------
// CTracePlayerContainer::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CTracePlayerContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    }

// End of File  
