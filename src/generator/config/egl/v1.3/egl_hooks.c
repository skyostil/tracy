/*
 *  Common hooks for the EGL tracer and player.
 */

/*
 *  EGL config support
 */

#define LOAD_CONFIG_ATTRIB(ENUM, NAME) \
    attrs[attrCount++] = ENUM; \
    attrs[attrCount++] = trGetIntegerValue(event, NAME); \
    TR_ASSERT(attrCount < sizeof(attrs) / sizeof(attrs[0]));

#define SKIP_CONFIG_ATTRIB(ENUM, NAME) \
    trGetIntegerValue(event, NAME);

TRhandle @EGLConfig.create(TREvent* event)
{
    EGLNativeDisplayType nativeDisplay = trEglCreateNativeDisplay();
    EGLDisplay display = __eglGetDisplay(nativeDisplay);
    EGLint attrs[64], attrCount = 0, configCount = 0;
    EGLConfig config = 0, originalConfig;
    int terminate = 0;
    int major, minor;

    __eglInitialize(display, &major, &minor);
    originalConfig = (EGLConfig)trGetIntegerValue(event, "config_id");
    
    LOAD_CONFIG_ATTRIB(EGL_BUFFER_SIZE,             "buffer_size");
    LOAD_CONFIG_ATTRIB(EGL_RED_SIZE,                "red_size");
    LOAD_CONFIG_ATTRIB(EGL_GREEN_SIZE,              "green_size");
    LOAD_CONFIG_ATTRIB(EGL_BLUE_SIZE,               "blue_size");
    LOAD_CONFIG_ATTRIB(EGL_ALPHA_SIZE,              "alpha_size");
    LOAD_CONFIG_ATTRIB(EGL_BIND_TO_TEXTURE_RGB,     "bind_to_texture_rgb");
    LOAD_CONFIG_ATTRIB(EGL_BIND_TO_TEXTURE_RGBA,    "bind_to_texture_rgba");
    LOAD_CONFIG_ATTRIB(EGL_CONFIG_CAVEAT,           "config_caveat");
    LOAD_CONFIG_ATTRIB(EGL_DEPTH_SIZE,              "depth_size");
    LOAD_CONFIG_ATTRIB(EGL_LEVEL,                   "level");
    SKIP_CONFIG_ATTRIB(EGL_MAX_SWAP_INTERVAL,       "max_swap_interval");
    SKIP_CONFIG_ATTRIB(EGL_MIN_SWAP_INTERVAL,       "min_swap_interval");
    LOAD_CONFIG_ATTRIB(EGL_NATIVE_RENDERABLE,       "native_renderable");
    LOAD_CONFIG_ATTRIB(EGL_SAMPLE_BUFFERS,          "sample_buffers");
    LOAD_CONFIG_ATTRIB(EGL_SAMPLES,                 "samples");
    LOAD_CONFIG_ATTRIB(EGL_STENCIL_SIZE,            "stencil_size");
    LOAD_CONFIG_ATTRIB(EGL_SURFACE_TYPE,            "surface_type");
    LOAD_CONFIG_ATTRIB(EGL_TRANSPARENT_TYPE,        "transparent_type");
    LOAD_CONFIG_ATTRIB(EGL_TRANSPARENT_RED_VALUE,   "transparent_red");
    LOAD_CONFIG_ATTRIB(EGL_TRANSPARENT_GREEN_VALUE, "transparent_green");
    LOAD_CONFIG_ATTRIB(EGL_TRANSPARENT_BLUE_VALUE,  "transparent_blue");
    LOAD_CONFIG_ATTRIB(EGL_LUMINANCE_SIZE,          "luminance_size");
    LOAD_CONFIG_ATTRIB(EGL_ALPHA_MASK_SIZE,         "alpha_mask_size");
    LOAD_CONFIG_ATTRIB(EGL_COLOR_BUFFER_TYPE,       "color_buffer_type");
    LOAD_CONFIG_ATTRIB(EGL_CONFORMANT,              "conformant");
    LOAD_CONFIG_ATTRIB(EGL_RENDERABLE_TYPE,         "renderable_type");
    attrs[attrCount] = EGL_NONE;
    
    /* If the config ID was negative, i.e. forced, use the that specific configuration */
    if (((EGLint)originalConfig) < 0)
    {
        return (TRhandle)(-((EGLint)originalConfig));
    }

    __eglChooseConfig(display, attrs, 0, 0, &configCount);
    
    /* Bring up EGL if it hasn't been already */
    if (__eglGetError() == EGL_NOT_INITIALIZED)
    {
        __eglInitialize(display, 0, 0);
        terminate = 1;
    }
    
    /* Find a matching config from EGL */
    __eglChooseConfig(display, attrs, &config, 1, &configCount);

    TR_ASSERT(__eglGetError() == EGL_SUCCESS);
    
    if (!configCount)
    {
        /* Drop some attributes that are usually not strictly required */
        EGLint i;
        for (i = 0; i < attrCount; i += 2)
        {
            switch (attrs[i])
            {
            case EGL_LEVEL:
            case EGL_MAX_SWAP_INTERVAL:
            case EGL_MIN_SWAP_INTERVAL:
            case EGL_STENCIL_SIZE:
            case EGL_TRANSPARENT_RED_VALUE:
            case EGL_TRANSPARENT_GREEN_VALUE:
            case EGL_TRANSPARENT_BLUE_VALUE:
                attrs[i + 1] = 0;
                break;
            case EGL_TRANSPARENT_TYPE:
                attrs[i + 1] = EGL_NONE;
                break;
            case EGL_BIND_TO_TEXTURE_RGB:
            case EGL_BIND_TO_TEXTURE_RGBA:
            case EGL_CONFIG_CAVEAT:
            case EGL_NATIVE_RENDERABLE:
                attrs[i + 1] = EGL_DONT_CARE;
                break;
            }
        }
        
        __eglChooseConfig(display, attrs, &config, 1, &configCount);
        __eglGetError();
        
        /* Nada? Lose the antialiasing */
        if (!configCount)
        {
            for (i = 0; i < attrCount; i += 2)
            {
                switch (attrs[i])
                {
                case EGL_SAMPLE_BUFFERS:
                case EGL_SAMPLES:
                    attrs[i + 1] = 0;
                    break;
                }
            }
            __eglChooseConfig(display, attrs, &config, 1, &configCount);
            __eglGetError();
        
            /* No luck? Fall back to a window surface and lose the precision qualifiers */
            if (!configCount)
            {
                for (i = 0; i < attrCount; i += 2)
                {
                    switch (attrs[i])
                    {
                    case EGL_BUFFER_SIZE:
                    case EGL_RED_SIZE:
                    case EGL_GREEN_SIZE:
                    case EGL_BLUE_SIZE:
                    case EGL_ALPHA_SIZE:
                    case EGL_DEPTH_SIZE:
                    case EGL_LUMINANCE_SIZE:
                    case EGL_ALPHA_MASK_SIZE:
                        attrs[i + 1] = attrs[i + 1] ? 1 : EGL_DONT_CARE;
                        break;
                    case EGL_SURFACE_TYPE:
                        attrs[i + 1] = EGL_WINDOW_BIT;
                        break;
                    }
                }
                
                __eglChooseConfig(display, attrs, &config, 1, &configCount);
                
                /* No go? Accept just about anything then */
                if (!configCount)
                {
                    for (i = 0; i < attrCount; i += 2)
                    {
                        switch (attrs[i])
                        {
                        case EGL_SURFACE_TYPE:
                            break;
                        case EGL_RENDERABLE_TYPE:
                            break;
                        default:
                            attrs[i] = EGL_RED_SIZE;
                            attrs[i + 1] = EGL_DONT_CARE;
                        }
                    }
                    
                    __eglChooseConfig(display, attrs, &config, 1, &configCount);
                    
                    /* Still no luck? Can't do much about it, then. */
                    if (!configCount)
                    {
                        TR_ASSERT(!"Unable to find matching EGL config.");
                    }
                }
            }
        }
    }
    
    if (terminate)
    {
        __eglTerminate(display);
    }

    trEglDestroyNativeDisplay(nativeDisplay);
 
    return (TRhandle)config;
}

TRHandle @EGLNativeDisplayType.create(TREvent* event)
{
    return (TRhandle)trEglCreateNativeDisplay();
}

void @EGLNativeDisplayType.destroy(TRhandle handle)
{
    if (handle)
    {
        EGLNativeDisplayType dpy = (EGLNativeDisplayType)handle;
        XCloseDisplay(handle);
    }
}
 
/*
 *  Window support
 */

TRhandle @EGLNativeWindowType.create(TREvent* event)
{
    TRint         x, y, width, height;
    TRColorFormat colorFormat;
    
    x           = trGetIntegerValue(event, "x");
    y           = trGetIntegerValue(event, "y");
    width       = trGetIntegerValue(event, "width");
    height      = trGetIntegerValue(event, "height");
    colorFormat = (TRColorFormat)trGetIntegerValue(event, "mode");
    
    return trPlayerCreateWindow(trGetPlayer(), x, y, width, height, colorFormat);
}

void @EGLNativeWindowType.destroy(TRhandle handle)
{
    trPlayerDestroyWindow(trGetPlayer(), handle);
}

/*
 *  Pixmap support
 */

TRhandle @EGLNativePixmapType.create(TREvent* event)
{
    TRint         width, height;
    TRColorFormat colorFormat;
    
    width       = trGetIntegerValue(event, "width");
    height      = trGetIntegerValue(event, "height");
    colorFormat = (TRColorFormat)trGetIntegerValue(event, "mode");
    
    return trPlayerCreatePixmap(trGetPlayer(), width, height, colorFormat);
}

void @EGLNativePixmapType.destroy(TRhandle handle)
{
    trPlayerDestroyPixmap(trGetPlayer(), handle);
}
