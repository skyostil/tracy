/*
 *  Hooks for the OpenVG player
 */

void VGISymbianCopyToBitmap.@postcall()
{
    trPlayerDisplayPixmap(trGetPlayer(), aBitmap);
}

