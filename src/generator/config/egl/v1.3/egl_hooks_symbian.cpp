/*
 *  Common Symbian hooks for the EGL tracer and player.
 */

/*
 *  Window support
 */

void @EGLNativeWindowType.serialize(TREvent* event, TRhandle handle)
{
    REglWindowBase* eglwindowbase = reinterpret_cast<REglWindowBase*>(handle);
        
    TInt isRwindow = eglwindowbase->iIsRWindow;
          
    if (isRwindow != -1)
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
    else
    {
      REglStandAloneWindow* window = (REglStandAloneWindow*)handle;
      TSize sizeinpixels = window->SizeInPixels();
        
      trIntegerValue(event, "x",      0);
      trIntegerValue(event, "y",      0);
      trIntegerValue(event, "width",  sizeinpixels.iWidth);
      trIntegerValue(event, "height", sizeinpixels.iHeight);
      trIntegerValue(event, "mode",   TR_COLOR_RGBX_8888);
    }
}

/*
 *  Pixmap support
 */

void @EGLNativePixmapType.serialize(TREvent* event TRhandle handle)
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
