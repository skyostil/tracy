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

void @TSize.modify(TREvent* event, TRhandle handle)
{
    TSize* size = (TSize*)handle;
    
    size->iWidth  = trGetIntegerValue(event, "width");
    size->iHeight = trGetIntegerValue(event, "height");
}
