/*
 *  Common hooks for the OpenVG tracer and player
 */

/*
 *  Pixmap support
 */

TRhandle @CFbsBitmap.create(TREvent* event)
{
    TSize         size;
    TRColorFormat colorFormat;
    TDisplayMode  mode;
    
    size.iWidth  = trGetIntegerValue(event, "width");
    size.iHeight = trGetIntegerValue(event, "height");
    mode         = (TDisplayMode)trGetIntegerValue(event, "mode");
    
    switch (mode)
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
    
    return trPlayerCreatePixmap(trGetPlayer(), size.iWidth, size.iHeight, colorFormat);
}

void @CFbsBitmap.destroy(TRhandle handle)
{
    trPlayerDestroyPixmap(trGetPlayer(), handle);
}

void @CFbsBitmap.serialize(TREvent* event TRhandle handle)
{
    CFbsBitmap* pixmap = (CFbsBitmap*)handle;
    
    TSize size = pixmap->SizeInPixels();
    trIntegerValue(event, "width",  size.iWidth);
    trIntegerValue(event, "height", size.iHeight);
    trIntegerValue(event, "mode",   pixmap->DisplayMode());
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

void @TSize.serialize(TREvent* event TRhandle handle)
{
    TSize* size = (TSize*)handle;
    
    trIntegerValue(event, "width",  size->iWidth);
    trIntegerValue(event, "height", size->iHeight);
}
