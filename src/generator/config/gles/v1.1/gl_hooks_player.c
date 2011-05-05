/*
 *  Hooks for the GLES player
 */

/*
 *  Extension loading
 */
#define LOAD_EXT(funcname) trLookupFunction(0, funcname)->pointer = (void*)eglGetProcAddress(funcname)

void @init()
{
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);
    LOAD_EXT("glDrawTexsOES");
    LOAD_EXT("glDrawTexiOES");
    LOAD_EXT("glDrawTexfOES");
    LOAD_EXT("glDrawTexxOES");
    LOAD_EXT("glDrawTexsvOES");
    LOAD_EXT("glDrawTexivOES");
    LOAD_EXT("glDrawTexfvOES");
    LOAD_EXT("glDrawTexxvOES");
    LOAD_EXT("glMatrixIndexPointerOES");
    LOAD_EXT("glWeightPointerOES");
    LOAD_EXT("glPointSizePointerOES");
    LOAD_EXT("glLoadPaletteFromModelViewMatrixOES");
    eglTerminate(display);
}
 
void glCompressedTexImage2D.@postcall()
{
    /*
     *  If the format was not supported, generate a dummy texture.
     */
    if (glGetError() == GL_INVALID_ENUM)
    {
        /* Make a smaller texture since we're will probably run out of memory otherwise */
        TRuint w = width  / 8;
        TRuint h = height / 8;
        TRuint* pixels = (TRuint*)trMalloc(w * h * sizeof(TRint));
        
        if (pixels)
        {
            TRint x, y;
            
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    TRuint color = ~(x ^ y) & 0xff;
                    pixels[y * w + x] = (color + (color << 8) + (color << 16));
                }
            }
            
            glTexImage2D(target, level, GL_RGBA, w, h, border, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            trFree(pixels);
        }
    }
}

void glCompressedTexSubImage2D.@postcall()
{
    /*
     *  If the format was not supported, generate a dummy texture.
     */
    if (glGetError() == GL_INVALID_ENUM)
    {
        /* Make a smaller texture since we're will probably run out of memory otherwise */
        TRuint w = width  / 8;
        TRuint h = height / 8;
        TRuint* pixels = (TRuint*)trMalloc(w * h * sizeof(TRint));
        
        if (pixels)
        {
            TRint x, y;
            
            for (y = 0; y < h; y++)
            {
                for (x = 0; x < w; x++)
                {
                    TRuint color = ~(x ^ y) & 0xff;
                    pixels[y * w + x] = (color + (color << 8) + (color << 16));
                }
            }
            
            glTexSubImage2D(target, level, xoffset, yoffset, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            trFree(pixels);
        }
    }
}
