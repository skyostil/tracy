/*
 *  Common Symbian hooks for the OpenVG tracer and player
 */

/*
 *  Pixmap support
 */

void @CFbsBitmap.serialize(TREvent* event TRhandle handle)
{
    CFbsBitmap* pixmap = (CFbsBitmap*)handle;
    TSize size = pixmap->SizeInPixels();
    TRColorFormat colorFormat;
    
    switch (pixmap->DisplayMode())
    {
    case EColor16MU:
        colorFormat = TR_COLOR_RGBX_8888;
        break;
    case EColor16MA:
        colorFormat = TR_COLOR_RGBA_8888;
        break;
    case EColor16M:
        colorFormat = TR_COLOR_RGB_888;
        break;
    case EColor64K:
        colorFormat = TR_COLOR_RGB_565;
        break;
    case EColor4K:
        colorFormat = TR_COLOR_RGBA_4444;
        break;
    case EColor256:
    case EGray256:
        colorFormat = TR_COLOR_L_8;
        break;
    case EGray2:
        colorFormat = TR_COLOR_BW_1;
        break;
    default:
        colorFormat = TR_COLOR_RGBX_8888;
        break;
    }
    
    trIntegerValue(event, "width",  size.iWidth);
    trIntegerValue(event, "height", size.iHeight);
    trIntegerValue(event, "mode",   colorFormat);
}


/**
 *  TSize support
 */

TRhandle @TSize.create(TREvent* event)
{
    TSize*       size = new TSize();
    
    if (size)
    {
        size->iWidth  = trGetIntegerValue(event, "width");
        size->iHeight = trGetIntegerValue(event, "height");
    }
    
    return size;
}

void @TSize.destroy(TRhandle handle)
{
    TSize* size = (TSize*)handle;
    delete size;
}

void @TSize.serialize(TREvent* event, TRhandle handle)
{
    TSize* size = (TSize*)handle;
    
    trIntegerValue(event, "width",  size->iWidth);
    trIntegerValue(event, "height", size->iHeight);
}

void @TSize.modify(TREvent* event, TRhandle handle)
{
    TSize* size = (TSize*)handle;
    
    size->iWidth  = trGetIntegerValue(event, "width");
    size->iHeight = trGetIntegerValue(event, "height");
}

void VGISymbianCopyToBitmap.@postcall()
{
    /* Save the bitmaps to files if such an option is enabled */
    if (trGetConfigInteger("vgtracer", "save_bitmaps", 0))
    {
        if (aBitmap)
        {
            RFs fileServer;
            TRequestStatus status = 0;
            fileServer.Connect();
        
            _LIT8(KMimeType, "image/png");
            TBuf<64> fileName;
            
            fileName.Append(_L("c:\\vgbitmap"));
            fileName.AppendNum(event->sequenceNumber);
            fileName.Append(_L(".png"));
            
            CImageEncoder* encoder = CImageEncoder::FileNewL(fileServer, fileName, KMimeType, CImageEncoder::EOptionAlwaysThread);
        
            // Do the conversion
            TRAPD(err, encoder->Convert(&status, *aBitmap)); 
        
            if (err == KErrNone)
            {
                User::WaitForRequest(status);
            }
        
            delete encoder;
            fileServer.Close();
        }
        if (aMaskBitmap)
        {
            RFs fileServer;
            TRequestStatus status = 0;
            fileServer.Connect();
        
            _LIT8(KMimeType, "image/png");
            TBuf<64> fileName;
            
            fileName.Append(_L("c:\\vgmask"));
            fileName.AppendNum(event->sequenceNumber);
            fileName.Append(_L(".png"));
            
            CImageEncoder* encoder = CImageEncoder::FileNewL(fileServer, fileName, KMimeType, CImageEncoder::EOptionAlwaysThread);
        
            // Do the conversion
            TRAPD(err, encoder->Convert(&status, *aMaskBitmap)); 
        
            if (err == KErrNone)
            {
                User::WaitForRequest(status);
            }
        
            delete encoder;
            fileServer.Close();
        }
    }
}
