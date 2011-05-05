/*
 *  Common Unix hooks for the EGL tracer and player.
 */

/*
 *  Display support
 *
 *  Note: Only the default display is supported, since an X display 
 *  connection can't really be serialized meaningfully.
 */

TRHandle @NativeDisplayType.create(TREvent* event)
{
    return XOpenDisplay(NULL);
}

void @NativeDisplayType.destroy(TRhandle handle)
{
    if (handle)
    {
        XCloseDisplay((Display*)handle);
    }
}

/*
 *  Window support
 */

void @NativeWindowType.serialize(TREvent* event, TRhandle handle)
{
    Drawable drawable = (Drawable)handle;
    Display* display  = XOpenDisplay(NULL);
    Window rootWindow;
    int x, y;
    unsigned int width, height, border, depth;
    TRColorFormat colorFormat;

    TR_ASSERT(display);
    TR_ASSERT(XGetGeometry(display, drawable, &rootWindow, &x, &y, &width, &height, &border, &depth));

    switch (depth)
    {
    case 32:
        colorFormat = TR_COLOR_RGBA_8888;
        break;
    case 24:
        colorFormat = TR_COLOR_RGB_888;
        break;
    case 16:
        colorFormat = TR_COLOR_RGB_565;
        break;
    case 8:
        colorFormat = TR_COLOR_L_8;
        break;
    case 1:
        colorFormat = TR_COLOR_BW_1;
        break;
    default:
        TR_ASSERT(!"Bad window depth");
    }

    trIntegerValue(event, "x",      x);
    trIntegerValue(event, "y",      y);
    trIntegerValue(event, "width",  width);
    trIntegerValue(event, "height", height);
    trIntegerValue(event, "mode",   colorFormat);

    XCloseDisplay(display);
}

/*
 *  Pixmap support
 */

void @NativePixmapType.serialize(TREvent* event TRhandle handle)
{
    Drawable drawable = (Drawable)handle;
    Display* display  = XOpenDisplay(NULL);
    Window rootWindow;
    int x, y;
    unsigned int width, height, border, depth;
    TRColorFormat colorFormat;

    TR_ASSERT(display);
    TR_ASSERT(XGetGeometry(display, drawable, &rootWindow, &x, &y, &width, &height, &border, &depth));

    switch (depth)
    {
    case 32:
        colorFormat = TR_COLOR_RGBA_8888;
        break;
    case 24:
        colorFormat = TR_COLOR_RGB_888;
        break;
    case 16:
        colorFormat = TR_COLOR_RGB_565;
        break;
    case 8:
        colorFormat = TR_COLOR_L_8;
        break;
    case 1:
        colorFormat = TR_COLOR_BW_1;
        break;
    default:
        TR_ASSERT(!"Bad pixmap depth");
    }

    trIntegerValue(event, "width",  width);
    trIntegerValue(event, "height", height);
    trIntegerValue(event, "mode",   colorFormat);

    XCloseDisplay(display);
}
