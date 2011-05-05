/**
 *  OpenVG player win32 support for emulating VGISymbian functionality on VGI.
 *
 *  Since we're running on windows and the OpenVG trace files use the VGISymbian 
 *  binding API, we must wrap that binding API over the plain VGI API here.
 */

#include "vgplayer_win32.h"
#include <windows.h>

TInt VGISymbianInitialize(TSize aSize, VGIColorSpace aColorSpace)
{
    return VGIInitialize(aSize.iWidth, aSize.iHeight, aColorSpace);
}

static int lowestBit(int n)
{
    int m = 0;
    
    while (n && !(n & 1))
    {
        n >>= 1;
        m++;
    }
    return m;
}

static int log2(int n)
{
    int m = 0;
    
    while (n >>= 1)
    {
        m++;
    }
    return m;
}

TInt VGISymbianCopyToBitmap(CFbsBitmap *aBitmap, CFbsBitmap *aMaskBitmap, VGICopyToTargetHint aHint)
{
    DIBSECTION           dibSection;
    void*                bufferPixels    = 0;
    int                  bufferStride    = 0;
    void*                maskPixels      = 0;
    int                  maskStride      = 0;
    int                  convert565To555 = 0;
    VGIColorBufferFormat format          = (VGIColorBufferFormat)-1;

    if (aBitmap && GetObject(aBitmap, sizeof(dibSection), &dibSection))
    {
        int rSize, gSize, bSize, aSize;

        bufferPixels = (void*)dibSection.dsBm.bmBits;
        bufferStride = dibSection.dsBm.bmWidthBytes;

        switch (dibSection.dsBmih.biCompression)
        {
        case BI_BITFIELDS:
            rSize = dibSection.dsBitfields[0];
            gSize = dibSection.dsBitfields[1];
            bSize = dibSection.dsBitfields[2];
            aSize = 0;
            
            rSize = log2(rSize >> lowestBit(rSize)) + 1;
            gSize = log2(gSize >> lowestBit(gSize)) + 1;
            bSize = log2(bSize >> lowestBit(bSize)) + 1;
            break;
        case BI_RGB:
            switch (dibSection.dsBm.bmBitsPixel)
            {
            case 32:
                rSize = gSize = bSize = aSize = 8;
                break;
            case 16:
                /* FIXME: or is it 555? */
                rSize = 5;
                gSize = 6;
                bSize = 5;
                aSize = 0;
                break;
            }
            break;
        }
        
        switch (rSize + gSize + bSize + aSize)
        {
        case 32:
          format = VGI_COLOR_BUFFER_FORMAT_XRGB8888;
          break;
        case 24:
          format = VGI_COLOR_BUFFER_FORMAT_RGB888;
          break;
        case 16:
          format = VGI_COLOR_BUFFER_FORMAT_RGB565;
          break;
        case 15:
          format = VGI_COLOR_BUFFER_FORMAT_RGB565;
          convert565To555 = 1;
          break;
        default:
          break;
        }

        if (aMaskBitmap && GetObject(aMaskBitmap, sizeof(dibSection), &dibSection))
        {
            maskPixels = (void*)dibSection.dsBm.bmBits;
            maskStride = dibSection.dsBm.bmWidthBytes;
        }
        
        if (format != (VGIColorBufferFormat)-1)
        {
            int ret = VGICopyToTarget(format, bufferStride, bufferPixels, maskStride, maskPixels, aHint);
            
            /* Convert 565 pixels to 555 format if needed */
            if (ret == VGI_OK && convert565To555)
            {
                int             x, y;
                unsigned short* data = (unsigned short*)bufferPixels;
                unsigned int    r, g, b;
                
                for (y = 0; y < dibSection.dsBm.bmHeight; y++)
                {
                    for (x = 0; x < dibSection.dsBm.bmWidth; x++)
                    {
                        r = (*data) & 0xf800;
                        g = (*data) & 0x07e0;
                        b = (*data) & 0x001f;
                        
                        *data++ = (r >> 1) | ((g >> 1) & 0x03e0) | b;
                    }
                }
            }
            return ret;
        }
    }
    return KErrArgument;
}

void VGISymbianTerminate()
{
    VGITerminate();
}

TInt VGISymbianResize(TSize aSize)
{
    return VGIResize(aSize.iWidth, aSize.iHeight);
}

TInt VGISymbianBindToImage(VGImage aImage)
{
    return VGIBindToImage(aImage);
}

TInt VGISymbianUnBindImage()
{
    return VGIUnBindImage();
}

