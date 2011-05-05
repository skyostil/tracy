/*
 *  Hooks for the OpenVG EGL player
 */

#define LOAD_CONFIG_ATTRIB(ENUM, NAME) \
    attrs[attrCount++] = ENUM; \
    attrs[attrCount++] = trGetIntegerValue(event, NAME); \
    TR_ASSERT(attrCount < sizeof(attrs) / sizeof(attrs[0]));

#define SKIP_CONFIG_ATTRIB(ENUM, NAME) \
    trGetIntegerValue(event, NAME);

TRhandle @EGLConfig.create(TREvent* event)
{
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint attrs[64], attrCount = 0, configCount = 0;
    EGLConfig config = 0, originalConfig;
    int terminate = 0;

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
    LOAD_CONFIG_ATTRIB(EGL_RENDERABLE_TYPE,         "renderable_type");
    attrs[attrCount] = EGL_NONE;

    /* If the config ID was negative, i.e. forced, use the that specific configuration */
    if (originalConfig < 0)
    {
        return (TRhandle)-originalConfig;
    }

    eglChooseConfig(display, attrs, 0, 0, &configCount);
    
    /* Bring up EGL if it hasn't been already */
    if (eglGetError() == EGL_NOT_INITIALIZED)
    {
        eglInitialize(display, 0, 0);
        terminate = 1;
    }
    
    /* Find a matching config from EGL */
    eglChooseConfig(display, attrs, &config, 1, &configCount);

    TR_ASSERT(eglGetError() == EGL_SUCCESS);
    
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
        
        eglChooseConfig(display, attrs, &config, 1, &configCount);
        eglGetError();
        
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
            eglChooseConfig(display, attrs, &config, 1, &configCount);
            eglGetError();
        
            /* No luck? Fall back to a window surface */
            if (!configCount)
            {
                for (i = 0; i < attrCount; i += 2)
                {
                    if (attrs[i] == EGL_SURFACE_TYPE)      attrs[i + 1] = EGL_WINDOW_BIT;
                }
                
                eglChooseConfig(display, attrs, &config, 1, &configCount);
                
                /* Still no luck? Can't do much about it, then. */
                if (!configCount)
                {
                    TR_ASSERT(!"Unable to find matching EGL config.");
                }
            }
        }
    }
    
    if (terminate)
    {
        eglTerminate(display);
    }
    
    return (TRhandle)config;
}
