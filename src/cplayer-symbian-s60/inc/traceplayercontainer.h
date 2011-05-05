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
#ifndef TRACEPLAYERCONTAINER_H
#define TRACEPLAYERCONTAINER_H

// INCLUDES
#include <coecntrl.h>
#include <EGL/egl.h>
#include "TracePlayer.h"
#include <akndef.h>

// CLASS DECLARATION

/**
 * Container control class that handles the OpenGL ES initialization and deinitializations.
 * Also uses the CTracePlayer class to do the actual OpenGL ES rendering.
 */
class CTracePlayerContainer : public CCoeControl, MCoeControlObserver
    {
    public: // Constructors and destructor
        
        /**
         * EPOC default constructor. Initializes the OpenGL ES for rendering to the window surface.
         * @param aRect Screen rectangle for container.
         */
        void ConstructL(const TRect& aRect);

        /**
         * Destructor. Destroys the CPeriodic, CTracePlayer and uninitializes OpenGL ES.
         */
        virtual ~CTracePlayerContainer();

    public: // New functions

        /**
         * Callback function for the CPeriodic. Calculates the current frame, keeps the background
         * light from turning off and orders the CTracePlayer to do the rendering for each frame.
         *@param aInstance Pointer to this instance of CTracePlayerContainer.
         */
        static TInt DrawCallBack( TAny* aInstance );

    private: // Functions from base classes

        /**
         * Method from CoeControl that gets called when the display size changes.
         * If OpenGL has been initialized, notifies the renderer class that the screen
         * size has changed.
         */
        void SizeChanged();

        /**
         * Handles a change to the control's resources. This method
         * reacts to the KEikDynamicLayoutVariantSwitch event (that notifies of
         * screen size change) by calling the SetExtentToWholeScreen() again so that
         * this control fills the new screen size. This will then trigger a call to the
         * SizeChanged() method.
         * @param aType Message UID value, only KEikDynamicLayoutVariantSwitch is handled by this method.
         */
        void HandleResourceChange(TInt aType); 

        /**
         * Method from CoeControl. Does nothing in this implementation.
         */
        TInt CountComponentControls() const;

        /**
         * Method from CCoeControl. Does nothing in this implementation.
         */
        CCoeControl* ComponentControl(TInt aIndex) const;

        /**
         * Method from CCoeControl. Does nothing in this implementation.
         * All rendering is done in the DrawCallBack() method.
         */
        void Draw(const TRect& aRect) const;

        /**
         * Method from MCoeControlObserver that handles an event from the observed control.
         * Does nothing in this implementation.
		 * @param aControl   Control changing its state.
		 * @param aEventType Type of the control event.
         */
        void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
        
    private: //data

        /** Display where the OpenGL ES window surface resides. */
        EGLDisplay  iEglDisplay;
        
        /** Window surface where the OpenGL ES rendering is blitted to. */
        EGLSurface  iEglSurface;  

        /** OpenGL ES rendering context. */
        EGLContext  iEglContext;
        
        /** Active object that is the timing source for the animation. */
        CPeriodic*  iPeriodic;
        
        /**
         * Flag that indicates if OpenGL ES has been initialized or not.
         * Used to check if SizeChanged() can react to incoming notifications.
         */
        TBool iOpenVGInitialized;

        /** Frame counter variable, used in the animation. */
        TInt iFrame;             
        
        
        TInt iFirstEventTime;

    public:  //data
        
        /** Used in DrawCallBack() method to do the actual OpenGL ES rendering.  */
        CTracePlayer* iTracePlayer; 
        CFbsBitmap* iBitmap;
    };

#endif

// End of File
