/*
 *  Common hooks for the OpenVG tracer and player
 */

/*
 *  Pixmap support
 */

TRhandle @CFbsBitmap.create(TREvent* event)
{
    TRint         width, height;
    TRColorFormat colorFormat;
    
    width        = trGetIntegerValue(event, "width");
    height       = trGetIntegerValue(event, "height");
    colorFormat  = (TRColorFormat)trGetIntegerValue(event, "mode");

    return trPlayerCreatePixmap(trGetPlayer(), width, height, colorFormat);
}

void @CFbsBitmap.destroy(TRhandle handle)
{
    trPlayerDestroyPixmap(trGetPlayer(), handle);
}
