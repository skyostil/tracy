/**
 *  OpenVG player VGI over EGL emulation.
 */

#ifndef VGPLAYER_VGI_H
#define VGPLAYER_VGI_H

#include <vg/openvg.h>

#if defined(__cplusplus)
extern "C" {
#endif

enum {
	VGI_OK                              = 0,						
	VGI_ERROR_OUT_OF_MEMORY             = (-10000),
	VGI_ERROR_INVALID_ARGUMENTS         = (-10001),
	VGI_ERROR_ALREADY_EXISTS            = (-10002),
	VGI_ERROR_COLORSPACE_NOT_SUPPORTED  = (-10003),
    VGI_ERROR_NOT_SUPPORTED             = (-10004),
    VGI_ERROR_ILLEGAL_IMAGE_HANDLE      = (-10005),
    VGI_ERROR_IMAGE_IN_USE              = (-10006),
    VGI_ERROR_ILLEGAL_OPERATION         = (-10007),
};

typedef enum {
	VGI_COLORSPACE_LINEAR,
	VGI_COLORSPACE_SRGB
} VGIColorSpace;

typedef enum {
	VGI_COLOR_BUFFER_FORMAT_RGB565,
	VGI_COLOR_BUFFER_FORMAT_RGB888,
	VGI_COLOR_BUFFER_FORMAT_XRGB8888,
	VGI_COLOR_BUFFER_FORMAT_ARGB8888,
	VGI_COLOR_BUFFER_FORMAT_XRGB4444,
} VGIColorBufferFormat;

typedef enum {
   VGI_SKIP_TRANSPARENT_PIXELS,
   VGI_COPY_TRANSPARENT_PIXELS,
} VGICopyToTargetHint;

int VGIInitialize(int width, int height, VGIColorSpace colorSpace);
int VGICopyToTarget(VGIColorBufferFormat format, int bufferStride, void *buffer, int maskStride, void *mask, VGICopyToTargetHint hint);
void VGITerminate(void);
int VGIResize(int width, int height);
int VGIBindToImage(VGImage image);
int VGIUnBindImage(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* VGPLAYER_VGI_OVER_EGL_H */
