/**
 *  Internal Tracer API interface.
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
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This file is intended for use by clients of EGL. It provides
// the window type that clients must use in order to create EGL
// window surfaces that have no dependency on the window server.


/**
 @file
 @publishedPartner
 @prototype
*/


#ifndef __EGLWINDOW_H__
#define __EGLWINDOW_H__


#include <graphics/eglwindowcommon.h>


/**
Base class for all EGL window types except for RWindow. If an 
RWindow pointer is cast to this type, IsRWindow() will return true.
*/
NONSHARABLE_CLASS(REglWindowBase)
    {
public:
    virtual TBool IsValid() const = 0;
    virtual TSize SizeInPixels() const = 0;
    virtual TSize SizeInTwips() const = 0;
    virtual TInt ScreenNumber() const = 0;
    virtual void SetBackgroundColor(TRgb aColor, TBool aTriggerRedraw) = 0;
    virtual TInt SetBackgroundSurface(const TSurfaceConfiguration &aConfiguration, TBool aTriggerRedraw) = 0;
    virtual void RemoveBackgroundSurface(TBool aTriggerRedraw) = 0;

    TInt iIsRWindow;
    TBool IsRWindow() const;
protected:
    REglWindowBase();

private:
    friend class TEglWindowInterface;
    //TBool IsRWindow() const;
    
private:
    //This is aligned with RWindow::iBuffer, which is at offset 4 in RWindow.
    //Note that this class has virtual functions to the vptr occupies bytes 0-3.
    //TInt iIsRWindow;
    };


/**
Forward Declarations.
*/
class MEglStandAloneWindowObserver;


/**
Concrete EGL window type that has mutable size. It informs its 
delegate whenever a surface is attached and removed from it. It is
intended that render stages requiring an EGL window surface create a
window of this type, passing itself as the delegate. When creating 
the EGL window surface, the render stage passes this window as the 
EGLNativeWindowType. Whenever EGL attaches and removes surfaces, the 
render stage is notified via the delegate and can act accordingly by 
modifying the compositor's scene. The symbian composition surface
can be retrieved by calling GetBackgroundSurface(). This surface
is considered unmappable. Users must not call RSurfaceManager::
MapSurface() for this surface.
*/
NONSHARABLE_CLASS(REglStandAloneWindow) : public REglWindowBase
    {
public:
    IMPORT_C REglStandAloneWindow();
    IMPORT_C TInt Create(const TSize& aSizeInPixels, const TSize& aSizeInTwips, TInt aScreenNumber, MEglStandAloneWindowObserver& aObserver);
    IMPORT_C void Close();
    
    IMPORT_C void SetSizeInPixels(const TSize& aSize);
    IMPORT_C void SetSizeInTwips(const TSize& aSize);
    IMPORT_C TRgb BackgroundColor() const;
    IMPORT_C TBool GetBackgroundSurface(TSurfaceConfiguration& aConfiguration) const;
    
public: //From REglWindowBase.
    IMPORT_C TBool IsValid() const;
    IMPORT_C TSize SizeInPixels() const;
    IMPORT_C TSize SizeInTwips() const;
    IMPORT_C TInt ScreenNumber() const;
    IMPORT_C void SetBackgroundColor(TRgb aColor, TBool aTriggerRedraw);
    IMPORT_C TInt SetBackgroundSurface(const TSurfaceConfiguration& aConfiguration, TBool aTriggerRedraw);
    IMPORT_C void RemoveBackgroundSurface(TBool aTriggerRedraw);
    
private:
    class TEglStandAloneWindowData;
    TEglStandAloneWindowData* iData;
    };


/**
Delegate class intended for implementation by a render stage that 
requires an EGL window surface. See REglStandAloneWindow.
*/
class MEglStandAloneWindowObserver
    {
public:
    /**
    This method is called by REglStandAloneWindow just after a new 
    background color was set for the REglStandAloneWindow instance. 
    It provides the observer with an oportunity to adjust the 
    compositor's scene to reflect this change.
    
    @param  aWindow         The instance of REglStandAloneWindow that 
                            the background color was set upon.

    @param  aTriggerRedraw  If set, the client that set the 
                            background surface expects that it will
                            appear on screen immediately.
    */
    virtual void ColorWasSetForWindow(const REglStandAloneWindow& aWindow, TBool aTriggerRedraw) = 0;
    
    /**
    This method is called by REglStandAloneWindow just after a new 
    background surface was set on the REglStandAloneWindow instance. 
    It provides the observer with an oportunity to adjust the 
    compositor's scene to include this new surface.
    
    @param  aWindow         The instance of REglStandAloneWindow that 
                            the background surface was set upon.

    @param  aTriggerRedraw  If set, the client that set the 
                            background surface expects that it will
                            appear on screen immediately.
                            
    @return KErrNone on success or any system-wide error code. This
            is reported back to the caller of REglStandAloneWindow::
            SetBackgroundSurface().
    */
    virtual TInt SurfaceWasSetForWindow(const REglStandAloneWindow& aWindow, TBool aTriggerRedraw) = 0;
    
    /**
    This method is called by REglStandAloneWindow just before the 
    current background surface will be removed from the 
    REglStandAloneWindow instance. It provides the observer with an 
    oportunity to adjust the compositor's scene to exclude this 
    surface and unregister the surface with the compositor.
    
    @param  aWindow         The instance of REglStandAloneWindow that
                            the background surface was removed from.

    @param  aTriggerRedraw  If set, the client that removed the
                            background surface expects that the 
                            changes will appear on screen 
                            immediately.
    */
    virtual void SurfaceWillBeRemovedForWindow(const REglStandAloneWindow& aWindow, TBool aTriggerRedraw) = 0;
    };


#endif //__EGLWINDOW_H__
