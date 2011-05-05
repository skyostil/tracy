/*
 *  Hooks for the OpenVG VGI player
 */

void VGISymbianCopyToBitmap.@postcall()
{
    trPlayerDisplayPixmap(trGetPlayer(), aBitmap);
}
