/*
 *  Common Symbian hooks for the EGL tracer and player.
 */

/*
 *  Window support
 */

void @NativeWindowType.serialize(TREvent* event, TRhandle handle)
{
    RWindow* window = (RWindow*)handle;
    
    TSize size = window->Size();
    TPoint pos = window->Position();
    TRColorFormat colorFormat;
    
    switch (window->DisplayMode())
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
    
    trIntegerValue(event, "x",      pos.iX);
    trIntegerValue(event, "y",      pos.iY);
    trIntegerValue(event, "width",  size.iWidth);
    trIntegerValue(event, "height", size.iHeight);
    trIntegerValue(event, "mode",   colorFormat);
}

/*
 *  Pixmap support
 */

void @NativePixmapType.serialize(TREvent* event TRhandle handle)
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
