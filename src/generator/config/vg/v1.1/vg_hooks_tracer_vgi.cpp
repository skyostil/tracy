/*
 *  VGI binding api functions
 */

/* State management helpers */
#define GET(PATH)        trGetStateValue(event, PATH)
#define GET_INT(PATH)    ((int)GET(PATH))
 
void VGICopyToTarget.buffer()
{
    TRint width    = GET_INT(TRS_CTX_RENDER_WIDTH);
    TRint height   = GET_INT(TRS_CTX_RENDER_HEIGHT);
    TRint saveData = trGetConfigInteger("vgtracer", "save_bitmaps", 0);
    
    if (width > 0 && height > 0)
    {
        /* Only save the pixel data if we're configured to do so */
        trDefinePointerData(event, buffer, height * bufferStride, (const TRubyte*)(saveData ? buffer : 0), TR_BYTE);
    }
    trPointerValue(event, "buffer", buffer);
}

void VGICopyToTarget.mask()
{
    TRint width    = GET_INT(TRS_CTX_RENDER_WIDTH);
    TRint height   = GET_INT(TRS_CTX_RENDER_HEIGHT);
    TRint saveData = trGetConfigInteger("vgtracer", "save_bitmaps", 0);
    
    if (width > 0 && height > 0)
    {
        /* Only save the pixel data if we're configured to do so */
        trDefinePointerData(event, mask, height * maskStride, (const TRubyte*)(saveData ? mask : 0), TR_BYTE);
    }
    trPointerValue(event, "mask", mask);
}
