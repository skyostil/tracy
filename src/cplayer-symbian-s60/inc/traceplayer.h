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
#ifndef TRACEPLAYER_H
#define TRACEPLAYER_H

//  INCLUDES
#include <e32base.h> // for CBase definition
#include <VG/openvg.h> // OpenVG header file
#include <EGL/egl.h> // EGL header file

// CLASS DECLARATION

/**
 * Class that does the actual OpenGL ES rendering.
 */
class CTracePlayer : public CBase
    {
    public:  // Constructors and destructor
  
        /**
         * Factory method for creating a new CTracePlayer object.
         */
        static CTracePlayer* NewL( TUint aWidth, TUint aHeight);

        /**
         * Destructor. Does nothing.
         */
        virtual ~CTracePlayer();

    public: // New functions
        
        /**
         * Initializes OpenGL ES, sets the vertex and color
         * arrays and pointers. Also selects the shading mode.
         */
        void AppInit( void );
        
        /**
         * Called upon application exit. Does nothing.
         */
        void AppExit( void );
                
        /**
         * Renders one frame.
         * @param aFrame Number of the frame to be rendered.
         */
        void AppCycle( TInt aFrame );
                
    protected: // New functions
        
        /**
         * Standard constructor that must never Leave.
         * Stores the given screen width and height.
         * @param aWidth Width of the screen.
         * @param aHeight Height of the screen.
         */
        CTracePlayer( TUint aWidth, TUint aHeight);
        
        /**
         * Second phase contructor. Does nothing.
         */
        void ConstructL( void );
        
    private: // Data
    };

EGLNativeWindowType createEGLNativeWindowType6(void* context, int x, int y, int width, int height, int mode);
EGLNativeDisplayType createEGLNativeDisplayType1(void* context);
void destroyEGLNativeWindowType2(void* context, EGLNativeWindowType window);
void destroyEGLNativeDisplayType2(void* context, EGLNativeDisplayType display);
EGLConfig createEGLConfig28(void* context, EGLint config_id, EGLint buffer_size, EGLint red_size, EGLint green_size, EGLint blue_size, EGLint alpha_size, EGLint bind_to_texture_rgb, EGLint bind_to_texture_rgba, EGLint config_caveat, EGLint depth_size, EGLint level, EGLint max_swap_interval, EGLint min_swap_interval, EGLint native_renderable, EGLint sample_buffers, EGLint samples, EGLint stencil_size, EGLint surface_type, EGLint transparent_type, EGLint transparent_red, EGLint transparent_green, EGLint transparent_blue, EGLint luminance_size, EGLint alpha_mask_Size, EGLint color_buffer_type, EGLint conformant, EGLint renderable_type);
void destroyEGLConfig2(void* context, EGLConfig config);
VGPaint createVGPaint1(void* context);
void destroyVGPaint2(void* context, VGPaint p);
VGPaint createVGImage1(void* context);
void destroyVGImage2(void* context, VGPaint p);
VGPath createVGPath1(void* context);
void destroyVGPath2(void* context, VGPath p);
VGFont createVGFont1(void* context);
void destroyVGFont2(void* context, VGFont p);

#endif // TRACEPLAYER_H

// End of File
