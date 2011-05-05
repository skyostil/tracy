/*
 *  Hooks for the GLES player
 */
 
void eglCopyBuffers.@postcall()
{
    trPlayerDisplayPixmap(trGetPlayer(), target);
}

void eglChooseConfig.@postcall()
{
    /*  If no configurations were found, we should try again with a new attribute
     *  list with commonly missing features pruned.
     */
    if (attrib_list && num_config && *num_config == 0)
    {
        const EGLint* attr = attrib_list;
        EGLint* newAttrList;
        EGLint attrCount = 0;
        
        /* Count how many attributes there were to begin with */
        while (*attr != EGL_NONE)
        {
            attr += 2;
            attrCount++;
        }
        
        newAttrList = (EGLint*)trMalloc((attrCount * 2 + 1) * sizeof(EGLint));
        
        if (newAttrList)
        {
            EGLint* attrOut = newAttrList;
            
            /* Copy the attribute list */
            attr = attrib_list;
            while (*attr != EGL_NONE)
            {
                /* Don't require multisampling */
                if (*attr == EGL_SAMPLES ||
                    *attr == EGL_SAMPLE_BUFFERS)
                {
                    attr += 2;
                    continue;
                }
                
                *attrOut++ = *attr++;
                *attrOut++ = *attr++;
            }
            *attrOut++ = *attr;
        
            /* Choose a new config */
            eglChooseConfig(dpy, newAttrList, configs, config_size, num_config);
            
            trFree(newAttrList);
        }
    }
    
    /* Save the best configuration */
    if (*num_config)
    {
        trSetStateValue(event, TRS_BEST_CONFIG, (void*)configs[0]);
    }
}

void eglCreateContext.@precall()
{
    /*
     *  Use any previously chosen best config.
     */
    EGLConfig bestConfig = (EGLConfig)trGetStateValue(event, TRS_BEST_CONFIG);
    if (bestConfig)
    {
        config = bestConfig;
    }
}

void eglCreateWindowSurface.@precall()
{
    /*
     *  Use any previously chosen best config.
     */
    EGLConfig bestConfig = (EGLConfig)trGetStateValue(event, TRS_BEST_CONFIG);
    if (bestConfig)
    {
        config = bestConfig;
    }
}

void eglCreatePbufferSurface.@precall()
{
    /*
     *  Use any previously chosen best config.
     */
    EGLConfig bestConfig = (EGLConfig)trGetStateValue(event, TRS_BEST_CONFIG);
    if (bestConfig)
    {
        config = bestConfig;
    }
}

void eglCreatePixmapSurface.@precall()
{
    /*
     *  Use any previously chosen best config.
     */
    EGLConfig bestConfig = (EGLConfig)trGetStateValue(event, TRS_BEST_CONFIG);
    if (bestConfig)
    {
        config = bestConfig;
    }
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
