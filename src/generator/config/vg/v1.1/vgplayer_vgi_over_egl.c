/**
 *  OpenVG player win32 support for emulating VGISymbian functionality on VGI.
 *
 *  Since we're running on windows and the OpenVG trace files use the VGISymbian 
 *  binding API, we must wrap that binding API over the plain VGI API here.
 */

#include "vgplayer_vgi.h"
#include "tracer.h"
#include <egl/egl.h>

/** EGL state structure */
typedef struct
{
    EGLDisplay      display;
    EGLSurface      surface;
    EGLSurface      imageSurface;
    EGLContext      context;
    EGLConfig       config;
    EGLint          width;
    EGLint          height;
    VGIColorSpace   colorSpace;
} EGLState;

/** EGL state pointer */
static EGLState* state = 0;

int VGIInitialize(int width, int height, VGIColorSpace colorSpace)
{
    if (!state)
    {
        const EGLint configAttrs[] =
        {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT|EGL_OPENVG_BIT,
            EGL_BUFFER_SIZE,  32,
            EGL_NONE
        };

        const EGLint surfaceAttrs[] = 
        {
            EGL_WIDTH,        width,
            EGL_HEIGHT,       height,
            EGL_COLORSPACE,   (colorSpace == VGI_COLORSPACE_LINEAR) ? EGL_COLORSPACE_LINEAR : EGL_COLORSPACE_sRGB,
            EGL_NONE
        };
        
        
        EGLConfig config;
        EGLint    configCount;
      
        state = trMalloc(sizeof(EGLState));
        
        if (!state)
        {
            return VGI_ERROR_OUT_OF_MEMORY;
        }
      
        state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        
        if (!state->display)
        {
            trFree(state);
            return VGI_ERROR_ILLEGAL_OPERATION;
        }
        
        if (eglInitialize(state->display, 0, 0) == EGL_FALSE)
        {
            trFree(state);
            return VGI_ERROR_ILLEGAL_OPERATION;
        }

        eglBindAPI(EGL_OPENVG_API);

        if (eglChooseConfig(state->display, configAttrs, &state->config, 1, &configCount) == EGL_FALSE || !configCount)
        {
            eglTerminate(state->display);
            trFree(state);
            return VGI_ERROR_ILLEGAL_OPERATION;
        }
        
        state->surface       = eglCreatePbufferSurface(state->display, state->config, surfaceAttrs);
        state->imageSurface  = 0;
        state->width         = width;
        state->height        = height;
        state->colorSpace    = colorSpace;

        if (!state->surface)
        {
            eglTerminate(state->display);
            trFree(state);
            return VGI_ERROR_ILLEGAL_OPERATION;
        }
        
        state->context = eglCreateContext(state->display, state->config, EGL_NO_CONTEXT, 0);

        if (!state->context)
        {
            eglTerminate(state->display);
            trFree(state);
            return VGI_ERROR_ILLEGAL_OPERATION;
        }
        
        if (eglMakeCurrent(state->display, state->surface, state->surface, state->context) == EGL_FALSE)
        {
            eglTerminate(state->display);
            trFree(state);
            return VGI_ERROR_ILLEGAL_OPERATION;
        }

        return VGI_OK;
    }
    return VGI_ERROR_ALREADY_EXISTS;
}

int VGICopyToTarget(VGIColorBufferFormat format, int bufferStride, void *buffer, int maskStride, void *mask, VGICopyToTargetHint hint)
{
    VGImageFormat vgFormat;
    
    if (!state || !state->width || !state->height)
    {
        return VGI_ERROR_ILLEGAL_OPERATION;
    }
    
    /**
     *  This does not work exactly as the real copy-to-target operation, but does trigger a data readback
     */
    
    switch (format)
    {
    case VGI_COLOR_BUFFER_FORMAT_RGB565:
        /* FIXME: What about linear? */
        vgFormat = VG_sRGB_565;
        break;
    case VGI_COLOR_BUFFER_FORMAT_RGB888:
        /* FIXME: No such format */
        return VGI_ERROR_ILLEGAL_OPERATION;
    case VGI_COLOR_BUFFER_FORMAT_XRGB8888:
        /* FIXME: Byte order? */
        vgFormat = (state->colorSpace == VGI_COLORSPACE_LINEAR) ? VG_lRGBX_8888 : VG_sRGBX_8888;
        break;
    case VGI_COLOR_BUFFER_FORMAT_ARGB8888:
        /* FIXME: Byte order? Premultiplication? */
        vgFormat = (state->colorSpace == VGI_COLORSPACE_LINEAR) ? VG_lRGBA_8888 : VG_sRGBA_8888;
        break;
    case VGI_COLOR_BUFFER_FORMAT_XRGB4444:
        /* FIXME: What about linear? */
        vgFormat = VG_sRGBA_4444;
        break;
    }
    
    if (buffer && bufferStride)
    {
        vgReadPixels(buffer, bufferStride, vgFormat, 0, 0, state->width, state->height);
    }

    if (mask && maskStride)
    {
        vgReadPixels(mask, maskStride, VG_A_8, 0, 0, state->width, state->height);
    }
    
    return VGI_OK;
}

void VGITerminate(void)
{
    if (!state)
    {
        return;
    }
    
    eglMakeCurrent(state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(state->display, state->surface); 
    eglDestroySurface(state->display, state->imageSurface); 
    eglDestroyContext(state->display, state->context);
    eglTerminate(state->display);
    trFree(state);
    state = 0;
}

int VGIResize(int width, int height)
{
    if (!state)
    {
        return VGI_ERROR_ILLEGAL_OPERATION;
    }
    
    if (state->width != width || state->height != height)
    {
        const EGLint surfaceAttrs[] = 
        {
            EGL_WIDTH,        width,
            EGL_HEIGHT,       height,
            EGL_NONE
        };
        
        eglMakeCurrent(state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(state->display, state->surface); 
        
        state->surface    = eglCreatePbufferSurface(state->display, state->config, surfaceAttrs);
        state->width      = width;
        state->height     = height;
        
        if (!state->surface)
        {
            VGITerminate();
            return VGI_ERROR_OUT_OF_MEMORY;
        }
        
        if (eglMakeCurrent(state->display, state->surface, state->surface, state->context) == EGL_FALSE)
        {
            VGITerminate();
            return VGI_ERROR_ILLEGAL_OPERATION;
        }
    }
    
    return VGI_OK;
}

int VGIBindToImage(VGImage image)
{
    if (!state)
    {
        return VGI_ERROR_ILLEGAL_OPERATION;
    }
#if 0    
    state->imageSurface = CreatePbufferFromClientBuffer(state->display, EGL_OPENVG_IMAGE, (EGLClientBuffer)image, state->config, 0);
    
    if (!state->imageSurface)
    {
        return VGI_ERROR_OUT_OF_MEMORY;
    }

    if (eglMakeCurrent(state->display, state->imageSurface, state->imageSurface, state->context) == EGL_FALSE)
    {
        return VGI_ERROR_ILLEGAL_OPERATION;
    }
#endif
    return VGI_OK;
}

int VGIUnBindImage(void)
{
    if (!state || !state->imageSurface)
    {
        return VGI_ERROR_ILLEGAL_OPERATION;
    }
    
    if (eglMakeCurrent(state->display, state->surface, state->surface, state->context) == EGL_FALSE)
    {
        return VGI_ERROR_ILLEGAL_OPERATION;
    }
    
    eglDestroySurface(state->display, state->imageSurface); 
    state->imageSurface = 0;
    
    return VGI_OK;
}
