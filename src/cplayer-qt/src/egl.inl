/**
 * Copyright (c) 2011 Nokia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef EGL_INL
#define EGL_INL

static EGLConfig createEGLConfig28(void* context, EGLint config_id, EGLint
        buffer_size, EGLint red_size, EGLint green_size, EGLint blue_size,
        EGLint alpha_size, EGLint bind_to_texture_rgb, EGLint
        bind_to_texture_rgba, EGLint config_caveat, EGLint depth_size, EGLint
        level, EGLint max_swap_interval, EGLint min_swap_interval, EGLint
        native_renderable, EGLint sample_buffers, EGLint samples, EGLint
        stencil_size, EGLint surface_type, EGLint transparent_type, EGLint
        transparent_red, EGLint transparent_green, EGLint transparent_blue,
        EGLint luminance_size, EGLint alpha_mask_Size, EGLint
        color_buffer_type, EGLint conformant, EGLint renderable_type)
{
    EGLint attrs[64], n = 0, numConfigs = 0;
    EGLDisplay dpy = eglGetDisplay(EGLNativeDisplayType(xdpy));
    EGLConfig config;
    EGLint major, minor;
    int brokenConfig = 0;

#if defined(FORCE_CONFIG)
    printf("Forcing EGL config %d\n", FORCE_CONFIG);
    return FORCE_CONFIG;
#endif
    //surface_type = EGL_WINDOW_BIT;
    //renderable_type = EGL_OPENGL_ES2_BIT;
    printf("Surface type: 0x%x, renderable type: 0x%x\n", surface_type, renderable_type);

    /* Ignore config attributes that are definitely incorrect
     * TODO: see why this is happening
     */
    if (renderable_type > 0x3000)
    {
        renderable_type = EGL_OPENGL_ES2_BIT;
        surface_type = EGL_PIXMAP_BIT;
        brokenConfig = 1;
    }

    eglInitialize(dpy, &major, &minor);

    attrs[n++] = EGL_SURFACE_TYPE;
    attrs[n++] = surface_type;
#if defined(USE_GLES) || defined(USE_GLES2)
    attrs[n++] = EGL_RENDERABLE_TYPE;
    attrs[n++] = renderable_type;
#endif
    attrs[n++] = EGL_SAMPLE_BUFFERS;
    attrs[n++] = sample_buffers;
    attrs[n++] = EGL_SAMPLES;
    attrs[n++] = samples;

    if (!brokenConfig)
    {
        /*attrs[n++] = EGL_RED_SIZE;
        attrs[n++] = red_size;
        attrs[n++] = EGL_GREEN_SIZE;
        attrs[n++] = green_size;
        attrs[n++] = EGL_BLUE_SIZE;
        attrs[n++] = blue_size;
        attrs[n++] = EGL_ALPHA_SIZE;
        attrs[n++] = alpha_size;*/
        attrs[n++] = EGL_DEPTH_SIZE;
        attrs[n++] = depth_size;
        attrs[n++] = EGL_STENCIL_SIZE;
        attrs[n++] = stencil_size;
    }
    attrs[n++] = EGL_NONE;

    eglChooseConfig(dpy, attrs, &config, 1, &numConfigs);

    assert(numConfigs > 0);

    printf("Using EGL config: %d\n", int(config));

    return config;
}

static void destroyEGLConfig2(void* context, EGLConfig config)
{
    /* TODO */
}

#endif /* EGL_INL */
