 /*
 *  EGL support
 */

#define SAVE_CONFIG_ATTRIB(ENUM, NAME) \
    eglGetConfigAttrib(display, config, ENUM, &value); \
    trIntegerValue(event, NAME, value);

TRhandle @EGLConfig.serialize(TREvent* event, TRhandle handle)
{
    /* Query all the needed parameters of the config */
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint     value;
    EGLConfig  config  = (EGLConfig)handle;

    /* 
     * Discarded values: 
     *   NATIVE_VISUAL_ID, NATIVE_VISUAL_TYPE, EGL_MAX_PBUFFER_WIDTH,
     *   EGL_MAX_PBUFFER_HEIGHT, EGL_MAX_PBUFFER_PIXELS
     */
    SAVE_CONFIG_ATTRIB(EGL_CONFIG_ID,               "config_id");
    SAVE_CONFIG_ATTRIB(EGL_BUFFER_SIZE,             "buffer_size");
    SAVE_CONFIG_ATTRIB(EGL_RED_SIZE,                "red_size");
    SAVE_CONFIG_ATTRIB(EGL_GREEN_SIZE,              "green_size");
    SAVE_CONFIG_ATTRIB(EGL_BLUE_SIZE,               "blue_size");
    SAVE_CONFIG_ATTRIB(EGL_ALPHA_SIZE,              "alpha_size");
    SAVE_CONFIG_ATTRIB(EGL_BIND_TO_TEXTURE_RGB,     "bind_to_texture_rgb");
    SAVE_CONFIG_ATTRIB(EGL_BIND_TO_TEXTURE_RGBA,    "bind_to_texture_rgba");
    SAVE_CONFIG_ATTRIB(EGL_CONFIG_CAVEAT,           "config_caveat");
    SAVE_CONFIG_ATTRIB(EGL_DEPTH_SIZE,              "depth_size");
    SAVE_CONFIG_ATTRIB(EGL_LEVEL,                   "level");
    SAVE_CONFIG_ATTRIB(EGL_MAX_SWAP_INTERVAL,       "max_swap_interval");
    SAVE_CONFIG_ATTRIB(EGL_MIN_SWAP_INTERVAL,       "min_swap_interval");
    SAVE_CONFIG_ATTRIB(EGL_NATIVE_RENDERABLE,       "native_renderable");
    SAVE_CONFIG_ATTRIB(EGL_SAMPLE_BUFFERS,          "sample_buffers");
    SAVE_CONFIG_ATTRIB(EGL_SAMPLES,                 "samples");
    SAVE_CONFIG_ATTRIB(EGL_STENCIL_SIZE,            "stencil_size");
    SAVE_CONFIG_ATTRIB(EGL_SURFACE_TYPE,            "surface_type");
    SAVE_CONFIG_ATTRIB(EGL_TRANSPARENT_TYPE,        "transparent_type");
    SAVE_CONFIG_ATTRIB(EGL_TRANSPARENT_RED_VALUE,   "transparent_red");
    SAVE_CONFIG_ATTRIB(EGL_TRANSPARENT_GREEN_VALUE, "transparent_green");
    SAVE_CONFIG_ATTRIB(EGL_TRANSPARENT_BLUE_VALUE,  "transparent_blue");
    SAVE_CONFIG_ATTRIB(EGL_LUMINANCE_SIZE,          "luminance_size");
    SAVE_CONFIG_ATTRIB(EGL_ALPHA_MASK_SIZE,         "alpha_mask_size");
    SAVE_CONFIG_ATTRIB(EGL_RENDERABLE_TYPE,         "renderable_type");
}

/* EGL hooks */
void eglInitialize.major()
{
    trIntegerArrayValue(event, "major", 1, major);
}

void eglInitialize.minor()
{
    trIntegerArrayValue(event, "minor", 1, minor);
}

/**
 *  A macro for saving EGL_NONE terminated attribute lists
 */
#define SAVE_ATTRIB_LIST(attrib_list)                                 \
    int length = 0;                                                   \
    const EGLint* attr = attrib_list;                                 \
    while (attr && *attr != EGL_NONE)                                 \
    {                                                                 \
        attr   += 2;                                                  \
        length += 2;                                                  \
    }                                                                 \
    trIntegerArrayValue(event, #attrib_list, length ? length + 1 : 0, \
                        attrib_list);

void eglChooseConfig.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

void eglChooseConfig.configs()
{
    trIntegerArrayValue(event, "configs", config_size, configs);
}

void eglChooseConfig.num_config()
{
    trIntegerArrayValue(event, "num_config", 1, num_config);
}

void eglCreateContext.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

void eglCreatePbufferSurface.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

void eglCreatePbufferSurface.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

void eglCreatePixmapSurface.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

void eglCreateWindowSurface.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

/* No such function in the reference implementation */
/*
void eglCreatePbufferFromClientBuffer.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}
*/

void eglGetConfigAttrib.value()
{
    trIntegerArrayValue(event, "value", 1, value);
}

void eglGetConfigs.configs()
{
    trIntegerArrayValue(event, "configs", config_size, configs);
}

void eglGetConfigs.num_config()
{
    trIntegerArrayValue(event, "num_config", 1, num_config);
}

void eglQueryContext.value()
{
    trIntegerArrayValue(event, "value", 1, value);
}

void eglQuerySurface.value()
{
    trIntegerArrayValue(event, "value", 1, value);
}

void eglQueryString.@return()
{
    trByteArrayValue(event, 0, ret ? triStringLength(ret) + 1 : 0, (const TRubyte*)ret);
}
