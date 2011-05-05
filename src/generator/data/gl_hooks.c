/*
 *  Common hooks for the GLES tracer and player.
 */

/*
 *  Window support
 */

TRhandle @NativeWindowType.create(TREvent* event)
{
    TPoint        pos;
    TSize         size;
    TRColorFormat colorFormat;
    TDisplayMode  mode;
    
    pos.iX       = trGetIntegerValue(event, "x");
    pos.iY       = trGetIntegerValue(event, "y");
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
    
    return trPlayerCreateWindow(trGetPlayer(), pos.iX, pos.iY, size.iWidth, size.iHeight, colorFormat);
}

void @NativeWindowType.destroy(TRhandle handle)
{
    trPlayerDestroyWindow(trGetPlayer(), handle);
}

void @NativeWindowType.serialize(TREvent* event TRhandle handle)
{
    RWindow* window = (RWindow*)handle;
    
    TSize size = window->Size();
    TPoint pos = window->Position();
    trIntegerValue(event, "x",      pos.iX);
    trIntegerValue(event, "y",      pos.iY);
    trIntegerValue(event, "width",  size.iWidth);
    trIntegerValue(event, "height", size.iHeight);
    trIntegerValue(event, "mode",   window->DisplayMode());
}

/*
 *  Pixmap support
 */

TRhandle @NativePixmapType.create(TREvent* event)
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

void @NativePixmapType.destroy(TRhandle handle)
{
    trPlayerDestroyPixmap(trGetPlayer(), handle);
}

void @NativePixmapType.serialize(TREvent* event TRhandle handle)
{
    CFbsBitmap* pixmap = (CFbsBitmap*)handle;
    
    TSize size = pixmap->SizeInPixels();
    trIntegerValue(event, "width",  size.iWidth);
    trIntegerValue(event, "height", size.iHeight);
    trIntegerValue(event, "mode",   pixmap->DisplayMode());
}
