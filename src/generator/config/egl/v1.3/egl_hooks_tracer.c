/*
 *  Hooks for the EGL tracer.
 */

/* State management helpers */
#define ENABLE(PATH)   trSetStateValue(event, PATH, (void*)1)
#define DISABLE(PATH)  trSetStateValue(event, PATH, (void*)0)
#define GET(PATH)      trGetStateValue(event, PATH)
#define GET_INT(PATH)  ((int)GET(PATH))
#define GET_BOOL(PATH) ((TRbool)GET(PATH))
#define SET(PATH, VALUE) trSetStateValue(event, PATH, (void*)VALUE)

/*
 *  EGL config support
 */

#define SAVE_CONFIG_ATTRIB(ENUM, NAME) \
    __eglGetConfigAttrib(display, config, ENUM, &value); \
    trIntegerValue(event, NAME, value);

TRhandle @EGLConfig.serialize(TREvent* event, TRhandle handle)
{
    /* Query all the needed parameters of the config */
    EGLNativeDisplayType nativeDisplay = trEglCreateNativeDisplay();
    EGLDisplay display = __eglGetDisplay(nativeDisplay);
    EGLint     value;
    EGLConfig  config  = (EGLConfig)handle;
    int major, minor;

    __eglInitialize(display, &major, &minor);

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
    SAVE_CONFIG_ATTRIB(EGL_COLOR_BUFFER_TYPE,       "color_buffer_type");
    SAVE_CONFIG_ATTRIB(EGL_CONFORMANT,              "conformant");
    SAVE_CONFIG_ATTRIB(EGL_RENDERABLE_TYPE,         "renderable_type");

    /* Display not terminated intentionally since we might kill
     * the app's display on some implementations
     */
    trEglDestroyNativeDisplay(nativeDisplay);
}

/* EGL hooks */

/**
 *  A macro for saving EGL_NONE terminated attribute lists
 */
#define SAVE_ATTRIB_LIST(attrib_list)                                 \
    int length = 0;                                                   \
    const EGLint* attr = attrib_list;                                 \
    if (attr && *attr == EGL_NONE)                                    \
    {                                                                 \
        length = 1;                                                   \
    }                                                                 \
    else while (attr && *attr != EGL_NONE)                            \
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

void eglCreatePbufferFromClientBuffer.attrib_list()
{
    SAVE_ATTRIB_LIST(attrib_list);
}

void eglCreatePbufferFromClientBuffer.buffer()
{
    TR_ASSERT(buftype == EGL_OPENVG_IMAGE);
    trObjectValue(event, "buffer", trGetClassId("VGImage"), (TRhandle)buffer);
}

void eglQueryString.@return()
{
    trByteArrayValue(event, 0, ret ? triStringLength(ret) + 1 : 0, (const TRubyte*)ret);
}

void eglMakeCurrent.@postlog()
{
    /* Set the current context */
    SET(TRS_EGL_CONTEXT_HANDLE, (void*)ctx);

    /* Set the root context */
    SET(TRS_CTX_ROOT_CTX, GET(TRS_EGL_CONTEXT_HANDLE_ROOT_CTX));
}

void eglCreateContext.@postlog()
{
    /* Figure out the root context */
    if (share_context)
    {
        void* rootCtx = (void*)share_context;
        void* savedCtx = GET(TRS_EGL_CONTEXT_HANDLE);
        while (1)
        {
            void* newRoot;
            SET(TRS_EGL_CONTEXT_HANDLE, rootCtx);
            newRoot = GET(TRS_EGL_CONTEXT_HANDLE_ROOT_CTX);
            if (!newRoot || newRoot == rootCtx)
            {
                /* Save the root context for the context we just created */
                SET(TRS_EGL_CONTEXT_HANDLE, (void*)ret);
                SET(TRS_EGL_CONTEXT_HANDLE_ROOT_CTX, rootCtx);
                break;
            }
            rootCtx = newRoot;
        }
        SET(TRS_EGL_CONTEXT_HANDLE, savedCtx);
    }
    else
    {
        void* savedCtx = GET(TRS_EGL_CONTEXT_HANDLE);
        SET(TRS_EGL_CONTEXT_HANDLE, (void*)ret);
        SET(TRS_EGL_CONTEXT_HANDLE_ROOT_CTX, (void*)ret);
        SET(TRS_EGL_CONTEXT_HANDLE, savedCtx);
    }
}
