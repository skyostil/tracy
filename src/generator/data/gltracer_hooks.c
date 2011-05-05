/*
 *  Hooks for the GLES tracer.
 */

/* State management helpers */
#define ENABLE(PATH)   trSetStateValue(event, PATH, (void*)1)
#define DISABLE(PATH)  trSetStateValue(event, PATH, (void*)0)
#define GET(PATH)      trGetStateValue(event, PATH)
#define GET_INT(PATH)  ((int)GET(PATH))
#define GET_BOOL(PATH) ((TRbool)GET(PATH))
#define SET(PATH, VALUE) trSetStateValue(event, PATH, (void*)VALUE)

void glDrawElements.indices()
{
    if (type == GL_UNSIGNED_BYTE)
        trByteArrayValue(event, "indices", count, (const TRubyte*)indices);
    else if (type == GL_UNSIGNED_SHORT)
        trShortArrayValue(event, "indices", count, (const TRshort*)indices);
    else
        trPointerValue(event, "indices", indices);
}

#define SAVE_ARRAY(ENABLE_FLAG, POINTER, STRIDE, TYPE, SIZE)   \
    if (ENABLE_FLAG)                                           \
    {                                                          \
        int      size = 0;                                     \
        int    stride = STRIDE;                                \
        TRenum   type = TR_VOID;                               \
        const TRubyte* pointer = (const TRubyte*)POINTER;      \
        switch (TYPE)                                          \
        {                                                      \
        case GL_BYTE:                                          \
        case GL_UNSIGNED_BYTE:                                 \
            size = 1;                                          \
            type = TR_BYTE;                                    \
            break;                                             \
        case GL_SHORT:                                         \
            size = 2;                                          \
            type = TR_SHORT;                                   \
            break;                                             \
        case GL_FIXED:                                         \
            size = 4;                                          \
            type = TR_INTEGER;                                 \
            break;                                             \
        case GL_FLOAT:                                         \
            size = 4;                                          \
            type = TR_FLOAT;                                   \
            break;                                             \
        }                                                      \
        if (stride > size) size = stride;                      \
        if (pointer)                                           \
          trDefinePointerData(event, pointer,                  \
                              (maxIndex + 1) * SIZE * size,    \
                              pointer, type);                  \
    }

void glDrawElements.@precall()
{
    int maxIndex = 0, i;
    const void *indexData = indices;
    int maxTextureUnit = GET_INT(TRS_CTX_CTEXUNIT_MAX);
    
    if (!indexData)
    {
        /* NULL indices, which usually means VBO usage */
        void* origBuffer = GET(TRS_CTX_BUFFER);
        
        SET(TRS_CTX_BUFFER, GL_ARRAY_BUFFER);
        if (GET(TRS_CTX_BUFFER_HANDLE))
        {
            indexData = GET(TRS_CTX_BUFFER_HANDLE_POINTER);
        }
        SET(TRS_CTX_BUFFER, origBuffer);
    }
    
    /* Find out the minimum and maximum indices for this draw call */
    if (indexData)
    {
        if (type == GL_UNSIGNED_BYTE)
        {
            const unsigned char* d = (const unsigned char*)indexData;
            for (i = 0; i < count; i++)
            {
                if (d[i] > maxIndex) maxIndex = d[i];
            }
        }
        else if (type == GL_UNSIGNED_SHORT)
        {
            const unsigned short* d = (const unsigned short*)indexData;
            for (i = 0; i < count; i++)
            {
                if (d[i] > maxIndex) maxIndex = d[i];
            }
        }
    }

    /* Save all the enabled arrays */
    SAVE_ARRAY(GET_BOOL(TRS_CTX_VERTEX_ENABLED),      GET(TRS_CTX_VERTEX_POINTER),      GET_INT(TRS_CTX_VERTEX_STRIDE),      GET_INT(TRS_CTX_VERTEX_TYPE),      GET_INT(TRS_CTX_VERTEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_NORMAL_ENABLED),      GET(TRS_CTX_NORMAL_POINTER),      GET_INT(TRS_CTX_NORMAL_STRIDE),      GET_INT(TRS_CTX_NORMAL_TYPE),      3);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_COLOR_ENABLED),       GET(TRS_CTX_COLOR_POINTER),       GET_INT(TRS_CTX_COLOR_STRIDE),       GET_INT(TRS_CTX_COLOR_TYPE),       GET_INT(TRS_CTX_COLOR_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_POINTSIZE_ENABLED),   GET(TRS_CTX_POINTSIZE_POINTER),   GET_INT(TRS_CTX_POINTSIZE_STRIDE),   GET_INT(TRS_CTX_POINTSIZE_TYPE),   1);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_MATRIXINDEX_ENABLED), GET(TRS_CTX_MATRIXINDEX_POINTER), GET_INT(TRS_CTX_MATRIXINDEX_STRIDE), GET_INT(TRS_CTX_MATRIXINDEX_TYPE), GET_INT(TRS_CTX_MATRIXINDEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_WEIGHT_ENABLED),      GET(TRS_CTX_WEIGHT_POINTER),      GET_INT(TRS_CTX_WEIGHT_STRIDE),      GET_INT(TRS_CTX_WEIGHT_TYPE),      GET_INT(TRS_CTX_WEIGHT_SIZE));
    
    maxTextureUnit = TR_MAX(GL_TEXTURE0, maxTextureUnit);
    for (i = GL_TEXTURE0; i < maxTextureUnit + 1; i++)
    {
        void* origUnit = GET(TRS_CTX_CTEXUNIT);
        
        SET(TRS_CTX_CTEXUNIT, i);
        SAVE_ARRAY(GET_BOOL(TRS_CTX_CTEXUNIT_TEXCOORD_ENABLED),
                   GET(TRS_CTX_CTEXUNIT_TEXCOORD_POINTER),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_STRIDE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_TYPE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_SIZE));
        SET(TRS_CTX_CTEXUNIT, origUnit);
    }
}

void glDrawArrays.@precall()
{
    int maxIndex = first + count, i;
    int maxTextureUnit = GET_INT(TRS_CTX_CTEXUNIT_MAX);
    
    /* Save all the enabled arrays */
    SAVE_ARRAY(GET_BOOL(TRS_CTX_VERTEX_ENABLED),      GET(TRS_CTX_VERTEX_POINTER),      GET_INT(TRS_CTX_VERTEX_STRIDE),      GET_INT(TRS_CTX_VERTEX_TYPE),      GET_INT(TRS_CTX_VERTEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_NORMAL_ENABLED),      GET(TRS_CTX_NORMAL_POINTER),      GET_INT(TRS_CTX_NORMAL_STRIDE),      GET_INT(TRS_CTX_NORMAL_TYPE),      3);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_COLOR_ENABLED),       GET(TRS_CTX_COLOR_POINTER),       GET_INT(TRS_CTX_COLOR_STRIDE),       GET_INT(TRS_CTX_COLOR_TYPE),       GET_INT(TRS_CTX_COLOR_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_POINTSIZE_ENABLED),   GET(TRS_CTX_POINTSIZE_POINTER),   GET_INT(TRS_CTX_POINTSIZE_STRIDE),   GET_INT(TRS_CTX_POINTSIZE_TYPE),   1);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_MATRIXINDEX_ENABLED), GET(TRS_CTX_MATRIXINDEX_POINTER), GET_INT(TRS_CTX_MATRIXINDEX_STRIDE), GET_INT(TRS_CTX_MATRIXINDEX_TYPE), GET_INT(TRS_CTX_MATRIXINDEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_WEIGHT_ENABLED),      GET(TRS_CTX_WEIGHT_POINTER),      GET_INT(TRS_CTX_WEIGHT_STRIDE),      GET_INT(TRS_CTX_WEIGHT_TYPE),      GET_INT(TRS_CTX_WEIGHT_SIZE));

    maxTextureUnit = TR_MAX(GL_TEXTURE0, maxTextureUnit);
    for (i = GL_TEXTURE0; i < maxTextureUnit + 1; i++)
    {
        void* origUnit = GET(TRS_CTX_CTEXUNIT);
        
        SET(TRS_CTX_CTEXUNIT, i);
        SAVE_ARRAY(GET_BOOL(TRS_CTX_CTEXUNIT_TEXCOORD_ENABLED),
                   GET(TRS_CTX_CTEXUNIT_TEXCOORD_POINTER),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_STRIDE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_TYPE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_SIZE));
        SET(TRS_CTX_CTEXUNIT, origUnit);
    }
}

void glTexImage2D.pixels()
{
    /* The following assumes that the pixel scanlines are dword aligned */
    int components = 0;
    switch (format)
    {
    case GL_ALPHA:
    case GL_LUMINANCE:
        components = 1;
        break;
    case GL_LUMINANCE_ALPHA:
        components = 2;
        break;
    case GL_RGB:
        components = 3;
        break;
    case GL_RGBA:
        components = 4;
        break;
    }
    switch (type)
    {
    case GL_UNSIGNED_BYTE:
        trByteArrayValue(event, "pixels", width * height * components, (const TRubyte*)pixels);
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
        trShortArrayValue(event, "pixels", width * height, (const TRshort*)pixels);
        break;
    }
}

void glTexSubImage2D.pixels()
{
    /* The following assumes that the pixel scanlines are dword aligned */
    int components = 0;
    switch (format)
    {
    case GL_ALPHA:
    case GL_LUMINANCE:
        components = 1;
        break;
    case GL_LUMINANCE_ALPHA:
        components = 2;
        break;
    case GL_RGB:
        components = 3;
        break;
    case GL_RGBA:
        components = 4;
        break;
    }
    switch (type)
    {
    case GL_UNSIGNED_BYTE:
        trByteArrayValue(event, "pixels", width * height * components, (const TRubyte*)pixels);
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
        trShortArrayValue(event, "pixels", width * height, (const TRshort*)pixels);
        break;
    }
}

void glReadPixels.pixels()
{
    /* The following assumes that the pixel scanlines are dword aligned */
    int components = 0;
    switch (format)
    {
    case GL_ALPHA:
    case GL_LUMINANCE:
        components = 1;
        break;
    case GL_LUMINANCE_ALPHA:
        components = 2;
        break;
    case GL_RGB:
        components = 3;
        break;
    case GL_RGBA:
        components = 4;
        break;
    }
    switch (type)
    {
    case GL_UNSIGNED_BYTE:
        trByteArrayValue(event, "pixels", width * height * components, (const TRubyte*)pixels);
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
        trShortArrayValue(event, "pixels", width * height, (const TRshort*)pixels);
        break;
    }
}

void glTexImage2D.@postcall()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, pixels);
}

void glTexSubImage2D.@postcall()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, pixels);
}

void glCompressedTexImage2D.@postcall()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, data);
}

void glCompressedTexSubImage2D.@postcall()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, data);
}

void glCompressedTexImage2D.data()
{
    trByteArrayValue(event, "data", imageSize, (const TRubyte*)data);
}

void glCompressedTexSubImage2D.data()
{
    trByteArrayValue(event, "data", imageSize, (const TRubyte*)data);
}

void glMultMatrixf.m()
{
    trFloatArrayValue(event, "m", 16, m);
}

void glLoadMatrixf.m()
{
    trFloatArrayValue(event, "m", 16, m);
}

void glMultMatrixf.m()
{
    trFloatArrayValue(event, "m", 16, m);
}

void glMultMatrixx.m()
{
    trIntegerArrayValue(event, "m", 16, m);
}

void glLoadMatrixx.m()
{
    trIntegerArrayValue(event, "m", 16, m);
}

void glClipPlanef.equation()
{
    trFloatArrayValue(event, "equation", 4, equation);
}

void glClipPlanex.equation()
{
    trIntegerArrayValue(event, "equation", 4, equation);
}

void glFogfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_FOG_COLOR ? 4 : 1, params);
}

void glFogxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_FOG_COLOR ? 4 : 1, params);
}

void glEnableClientState.@precall()
{
    switch (array)
    {
    case GL_VERTEX_ARRAY:
        ENABLE(TRS_CTX_VERTEX_ENABLED);
        break;
    case GL_NORMAL_ARRAY:
        ENABLE(TRS_CTX_NORMAL_ENABLED);
        break;
    case GL_COLOR_ARRAY:
        ENABLE(TRS_CTX_COLOR_ENABLED);
        break;
    case GL_MATRIX_INDEX_ARRAY_OES:
        ENABLE(TRS_CTX_MATRIXINDEX_ENABLED);
        break;
    case GL_POINT_SIZE_ARRAY_OES:
        ENABLE(TRS_CTX_POINTSIZE_ENABLED);
        break;
    case GL_TEXTURE_COORD_ARRAY:
        ENABLE(TRS_CTX_CTEXUNIT_TEXCOORD_ENABLED);
        break;
    case GL_WEIGHT_ARRAY_OES:
        ENABLE(TRS_CTX_WEIGHT_ENABLED);
        break;
    }
}

void glDisableClientState.@precall()
{
    switch (array)
    {
    case GL_VERTEX_ARRAY:
        DISABLE(TRS_CTX_VERTEX_ENABLED);
        break;
    case GL_NORMAL_ARRAY:
        DISABLE(TRS_CTX_NORMAL_ENABLED);
        break;
    case GL_COLOR_ARRAY:
        DISABLE(TRS_CTX_COLOR_ENABLED);
        break;
    case GL_MATRIX_INDEX_ARRAY_OES:
        DISABLE(TRS_CTX_MATRIXINDEX_ENABLED);
        break;
    case GL_POINT_SIZE_ARRAY_OES:
        DISABLE(TRS_CTX_POINTSIZE_ENABLED);
        break;
    case GL_TEXTURE_COORD_ARRAY:
        DISABLE(TRS_CTX_CTEXUNIT_TEXCOORD_ENABLED);
        break;
    case GL_WEIGHT_ARRAY_OES:
        DISABLE(TRS_CTX_WEIGHT_ENABLED);
        break;
    }
}

void glBufferData.data()
{
    trByteArrayValue(event, "data", size, (const TRubyte*)data);
}

void glBufferSubData.data()
{
    trByteArrayValue(event, "data", size, (const TRubyte*)data);
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

void eglCreateContext.@postcall()
{
    /* Set the first texture unit as default */
    void* origCtx = GET(TRS_CTX);
    SET(TRS_CTX, ret);
    SET(TRS_CTX_CTEXUNIT, GL_TEXTURE0);
    SET(TRS_CTX_TEXUNIT,  GL_TEXTURE0);
    SET(TRS_CTX, origCtx);
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

void glGetClipPlanef.eqn()
{
    trFloatArrayValue(event, "eqn", 4, eqn);
}

void glGetClipPlanex.eqn()
{
    trIntegerArrayValue(event, "eqn", 4, eqn);
}

#define SAVE_QUERY_PARAMETERS(ARRAY_LOGGER, TYPE) \
    GLint elements = 1; \
    switch (pname) \
    {  \
    case GL_ALIASED_POINT_SIZE_RANGE: \
    case GL_ALIASED_LINE_WIDTH_RANGE: \
    case GL_SMOOTH_LINE_WIDTH_RANGE: \
    case GL_SMOOTH_POINT_SIZE_RANGE: \
    case GL_DEPTH_RANGE: \
    case GL_MAX_VIEWPORT_DIMS: \
        elements = 2; \
        break; \
    case GL_COLOR_CLEAR_VALUE: \
    case GL_COLOR_WRITEMASK: \
    case GL_FOG_COLOR: \
    case GL_LIGHT_MODEL_AMBIENT: \
    case GL_SCISSOR_BOX: \
    case GL_VIEWPORT: \
        elements = 4; \
        break; \
    case GL_COMPRESSED_TEXTURE_FORMATS: \
        elements = GL_NUM_COMPRESSED_TEXTURE_FORMATS; \
        break; \
    case GL_MODELVIEW_MATRIX: \
    case GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES: \
    case GL_PROJECTION_MATRIX: \
    case GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES: \
    case GL_TEXTURE_MATRIX: \
    case GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES: \
        elements = 16; \
        break; \
    } \
    ARRAY_LOGGER(event, "params", elements, (const TYPE*)params);

void glGetFloatv.params()
{
    SAVE_QUERY_PARAMETERS(trFloatArrayValue, TRfloat);
}

void glGetIntegerv.params()
{
    SAVE_QUERY_PARAMETERS(trIntegerArrayValue, TRint);
}

void glGetFixedv.params()
{
    SAVE_QUERY_PARAMETERS(trIntegerArrayValue, TRint);
}

void glGetBooleanv.params()
{
    SAVE_QUERY_PARAMETERS(trIntegerArrayValue, TRint);
}

#define SAVE_LIGHT_PARAMETERS(ARRAY_LOGGER) \
    GLint elements = 4; \
    switch (pname) \
    { \
    case GL_SPOT_DIRECTION: \
        elements = 3; \
        break; \
    case GL_SPOT_EXPONENT: \
    case GL_SPOT_CUTOFF: \
    case GL_CONSTANT_ATTENUATION: \
    case GL_LINEAR_ATTENUATION: \
    case GL_QUADRATIC_ATTENUATION: \
        elements = 1; \
        break; \
    } \
    ARRAY_LOGGER(event, "params", elements, params);

void glGetLightfv.params()
{
    SAVE_LIGHT_PARAMETERS(trFloatArrayValue);
}

void glLightfv.params()
{
    SAVE_LIGHT_PARAMETERS(trFloatArrayValue);
}

void glGetLightxv.params()
{
    SAVE_LIGHT_PARAMETERS(trIntegerArrayValue);
}

void glLightxv.params()
{
    SAVE_LIGHT_PARAMETERS(trIntegerArrayValue);
}

void glMaterialfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_SHININESS ? 1 : 4, params);
}

void glGetMaterialfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_SHININESS ? 1 : 4, params);
}

void glMaterialxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_SHININESS ? 1 : 4, params);
}

void glGetMaterialxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_SHININESS ? 1 : 4, params);
}

void glTexEnvfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_TEXTURE_ENV_COLOR ? 4 : 1, params);
}

void glGetTexEnvfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_TEXTURE_ENV_MODE ? 1 : 4, params);
}

void glTexEnvxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_TEXTURE_ENV_COLOR ? 4 : 1, params);
}

void glGetTexEnvxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_TEXTURE_ENV_MODE ? 1 : 4, params);
}

void glTexEnviv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_TEXTURE_ENV_COLOR ? 4 : 1, params);
}

void glGetTexEnviv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_TEXTURE_ENV_MODE ? 1 : 4, params);
}

void glTexParameterfv.params()
{
    trFloatArrayValue(event, "params", 1, params);
}

void glTexParameterxv.params()
{
    trIntegerArrayValue(event, "params", 1, params);
}

void glTexParameteriv.params()
{
    trIntegerArrayValue(event, "params", 1, params);
}

void glGetTexParameterfv.params()
{
    trFloatArrayValue(event, "params", 1, params);
}

void glGetTexParameterxv.params()
{
    trIntegerArrayValue(event, "params", 1, params);
}

void glGetTexParameteriv.params()
{
    trIntegerArrayValue(event, "params", 1, params);
}

void glLightModelfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_LIGHT_MODEL_AMBIENT ? 4 : 1, params);
}

void glLightModelxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_LIGHT_MODEL_AMBIENT ? 4 : 1, params);
}

void glPointParameterfv.params()
{
    trFloatArrayValue(event, "params", pname == GL_POINT_DISTANCE_ATTENUATION ? 3 : 1, params);
}

void glPointParameterxv.params()
{
    trIntegerArrayValue(event, "params", pname == GL_POINT_DISTANCE_ATTENUATION ? 3 : 1, params);
}

void glDeleteBuffers.buffers()
{
    trIntegerArrayValue(event, "buffers", n, (const TRint*)buffers);
}

void glDeleteBuffers.@postcall()
{
    /* Delete the buffers from the state tree */
    GLint i, orig = GET_INT(TRS_CTX_BUFFER_HANDLE);
    
    for (i = 0; i < n; i++)
    {
        trSetStateValue(event, TRS_CTX_BUFFER_HANDLE, (void*)buffers[i]);
        trRemoveStateValue(event, TRS_CTX_BUFFER_HANDLE);
    }
    trSetStateValue(event, TRS_CTX_BUFFER_HANDLE, (void*)orig);
}

void glDeleteTextures.textures()
{
    trIntegerArrayValue(event, "textures", n, (const TRint*)textures);
}

void glDeleteTextures.@postcall()
{
    /* Delete the textures from the state tree */
    GLint i, orig = GET_INT(TRS_CTX_TEXUNIT_TARGET_HANDLE);
    
    for (i = 0; i < n; i++)
    {
        trSetStateValue(event, TRS_CTX_TEXUNIT_TARGET_HANDLE, (void*)textures[i]);
        trRemoveStateValue(event, TRS_CTX_TEXUNIT_TARGET_HANDLE);
    }
    trSetStateValue(event, TRS_CTX_TEXUNIT_TARGET_HANDLE, (void*)orig);
}

void glGetBufferParameteriv.params()
{
    trIntegerArrayValue(event, "params", 1, params);
}

void glGenBuffers.buffers()
{
    trIntegerArrayValue(event, "buffers", n, (const TRint*)buffers);
}

void glGenTextures.textures()
{
    trIntegerArrayValue(event, "textures", n, (const TRint*)textures);
}

void glGetPointerv.params()
{
    trIntegerArrayValue(event, "params", 1, (const TRint*)params);
}

void glQueryMatrixxOES.mantissa()
{
    trIntegerArrayValue(event, "mantissa", 16, mantissa);
}

void glQueryMatrixxOES.exponent()
{
    trIntegerArrayValue(event, "exponent", 16, exponent);
}

void glClientActiveTexture.@postcall()
{
    /* Keep track of the greatest texture unit used */
    int n = GET_INT(TRS_CTX_CTEXUNIT_MAX);
    SET(TRS_CTX_CTEXUNIT_MAX, TR_MAX(n, texture));
}
