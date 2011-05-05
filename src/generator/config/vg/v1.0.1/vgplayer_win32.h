/**
 *  OpenVG player win32 support functionality
 */

#ifndef VGPLAYER_WIN32_H
#define VGPLAYER_WIN32_H

#include "tracer_player.h"

#if defined(TRACER_USE_STUB_EGL)
#include <vg/vgcontext.h>
#else
#include "vgplayer_vgi.h"
#endif

/* Mock implementation of Symbian data structures */

typedef int TInt;

class TSize
{
public:
    TInt iWidth, iHeight;
};

typedef enum
{
    KErrNone     = 0,
    KErrArgument = -6,
} EErrorCode;

typedef TRNativePixmap CFbsBitmap;

TInt VGISymbianInitialize(TSize aSize, VGIColorSpace aColorSpace);
TInt VGISymbianCopyToBitmap(CFbsBitmap *aBitmap, CFbsBitmap *aMaskBitmap, VGICopyToTargetHint aHint);
void VGISymbianTerminate();
TInt VGISymbianResize(TSize aSize);
TInt VGISymbianBindToImage(VGImage aImage);
TInt VGISymbianUnBindImage();

#endif
