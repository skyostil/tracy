/*
 *  Hooks for the OpenVG player
 */

#if 0
/*
 *  Set the image data pointer to the end of the image if
 *  the given stride is negative, i.e. the image will be
 *  written upside down.
 */
#define ADJUST_IMAGE_POINTER() \
    if (dataStride < 0) \
    { \
        const TRubyte* data8 = (const TRubyte*)data; \
        data8 -= dataStride * height; \
        data = (void*)data8; \
    }

void vgImageSubData.@precall()
{
    ADJUST_IMAGE_POINTER();
}

void vgGetImageSubData.@precall()
{
    ADJUST_IMAGE_POINTER();
}

void vgWritePixels.@precall()
{
    ADJUST_IMAGE_POINTER();
}

void vgReadPixels.@precall()
{
    ADJUST_IMAGE_POINTER();
}

void vgReadPixels.@call()
{
}
#endif
