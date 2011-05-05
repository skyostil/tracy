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

/*
 *  Extension loading
 */
#define LOAD_EXT(funcname) trLookupFunction(0, funcname)->pointer = lookup(funcname)

void @init()
{
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    /* Use the real EGL functions */
    typedef void* (*EglGetProcAddressFunc)(const char*);
    typedef void  (*EglTerminateFunc)(EGLDisplay);
    EglGetProcAddressFunc lookup = (EglGetProcAddressFunc)trLookupFunction(0, "eglGetProcAddress")->pointer;
    EglTerminateFunc   terminate = (EglTerminateFunc)trLookupFunction(0, "eglTerminate")->pointer;
    
    TR_ASSERT(lookup);
    TR_ASSERT(terminate);
    
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
    terminate(display);
}

void glDrawElements.indices()
{
    void* origBuffer = GET(TRS_CTX_BUFFER);
    void* boundElementBuffer = 0;

    SET(TRS_CTX_BUFFER, GL_ELEMENT_ARRAY_BUFFER);
    boundElementBuffer = GET(TRS_CTX_BUFFER_HANDLE);
    SET(TRS_CTX_BUFFER, origBuffer);

    if (boundElementBuffer)
        trPointerValue(event, "indices", indices);
    else if (type == GL_UNSIGNED_BYTE)
        trByteArrayValue(event, "indices", count, (const TRubyte*)indices);
    else if (type == GL_UNSIGNED_SHORT)
        trShortArrayValue(event, "indices", count, (const TRshort*)indices);
    else
        trPointerValue(event, "indices", indices);
}

#define SAVE_ARRAY(ENABLE_FLAG, POINTER, STRIDE, TYPE, SIZE)   \
    if (ENABLE_FLAG)                                           \
    {                                                          \
        int  bytesPerComponent = 0;                            \
        int             stride = STRIDE;                       \
        TRenum            type = TR_VOID;                      \
        int    bytesPerElement;                                \
        const TRubyte* pointer = (const TRubyte*)POINTER;      \
        switch (TYPE)                                          \
        {                                                      \
        case GL_BYTE:                                          \
        case GL_UNSIGNED_BYTE:                                 \
            bytesPerComponent = 1;                             \
            type = TR_BYTE;                                    \
            break;                                             \
        case GL_SHORT:                                         \
            bytesPerComponent = 2;                             \
            type = TR_SHORT;                                   \
            break;                                             \
        case GL_FIXED:                                         \
            bytesPerComponent = 4;                             \
            type = TR_INTEGER;                                 \
            break;                                             \
        case GL_FLOAT:                                         \
            bytesPerComponent = 4;                             \
            type = TR_FLOAT;                                   \
            break;                                             \
        }                                                      \
        bytesPerElement = SIZE * bytesPerComponent;            \
        if (stride > bytesPerElement)                          \
            bytesPerElement = stride;                          \
        trDefinePointerData(event, pointer,                    \
                            (maxIndex + 1) * bytesPerElement,  \
                            pointer, type);                    \
    }

void glDrawElements.@precall()
{
    int maxIndex = 0, i;
    const void *indexData = indices;
    int maxTextureUnit = GET_INT(TRS_CTX_CTEXUNIT_MAX);
    void* origUnit = GET(TRS_CTX_CTEXUNIT);
    void* origBuffer = GET(TRS_CTX_BUFFER);
        
    /* See if an element array buffer is bound */
    SET(TRS_CTX_BUFFER, GL_ELEMENT_ARRAY_BUFFER);
    if (GET(TRS_CTX_BUFFER_HANDLE))
    {
        const TRbyte* pointer = (const TRbyte*)GET(TRS_CTX_BUFFER_HANDLE_DATA);
        TRuint offset = (TRuint)indexData;
        indexData = (void*)(pointer + offset);
    }
    SET(TRS_CTX_BUFFER, origBuffer);
    
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
    SAVE_ARRAY(GET_BOOL(TRS_CTX_VERTEX_ENABLED)      && GET(TRS_CTX_VERTEX_BINDING) == 0,      GET(TRS_CTX_VERTEX_POINTER),      GET_INT(TRS_CTX_VERTEX_STRIDE),      GET_INT(TRS_CTX_VERTEX_TYPE),      GET_INT(TRS_CTX_VERTEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_NORMAL_ENABLED)      && GET(TRS_CTX_NORMAL_BINDING) == 0,      GET(TRS_CTX_NORMAL_POINTER),      GET_INT(TRS_CTX_NORMAL_STRIDE),      GET_INT(TRS_CTX_NORMAL_TYPE),      3);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_COLOR_ENABLED)       && GET(TRS_CTX_COLOR_BINDING) == 0,       GET(TRS_CTX_COLOR_POINTER),       GET_INT(TRS_CTX_COLOR_STRIDE),       GET_INT(TRS_CTX_COLOR_TYPE),       GET_INT(TRS_CTX_COLOR_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_POINTSIZE_ENABLED)   && GET(TRS_CTX_POINTSIZE_BINDING) == 0,   GET(TRS_CTX_POINTSIZE_POINTER),   GET_INT(TRS_CTX_POINTSIZE_STRIDE),   GET_INT(TRS_CTX_POINTSIZE_TYPE),   1);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_MATRIXINDEX_ENABLED) && GET(TRS_CTX_MATRIXINDEX_BINDING) == 0, GET(TRS_CTX_MATRIXINDEX_POINTER), GET_INT(TRS_CTX_MATRIXINDEX_STRIDE), GET_INT(TRS_CTX_MATRIXINDEX_TYPE), GET_INT(TRS_CTX_MATRIXINDEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_WEIGHT_ENABLED)      && GET(TRS_CTX_WEIGHT_BINDING) == 0,      GET(TRS_CTX_WEIGHT_POINTER),      GET_INT(TRS_CTX_WEIGHT_STRIDE),      GET_INT(TRS_CTX_WEIGHT_TYPE),      GET_INT(TRS_CTX_WEIGHT_SIZE));

    maxTextureUnit = TR_MAX(GL_TEXTURE0, maxTextureUnit);
    for (i = GL_TEXTURE0; i < maxTextureUnit + 1; i++)
    {
        SET(TRS_CTX_CTEXUNIT, i);
        SAVE_ARRAY(GET_BOOL(TRS_CTX_CTEXUNIT_TEXCOORD_ENABLED) && GET(TRS_CTX_CTEXUNIT_TEXCOORD_BINDING) == 0,
                   GET(TRS_CTX_CTEXUNIT_TEXCOORD_POINTER),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_STRIDE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_TYPE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_SIZE));
    }
    SET(TRS_CTX_CTEXUNIT, origUnit);
}

void glDrawArrays.@precall()
{
    int maxIndex = first + count - 1, i;
    int maxTextureUnit = GET_INT(TRS_CTX_CTEXUNIT_MAX);
    void* origUnit = GET(TRS_CTX_CTEXUNIT);
    
    /* Save all the enabled arrays that are not bound to any buffers */
    SAVE_ARRAY(GET_BOOL(TRS_CTX_VERTEX_ENABLED)      && GET(TRS_CTX_VERTEX_BINDING) == 0,      GET(TRS_CTX_VERTEX_POINTER),      GET_INT(TRS_CTX_VERTEX_STRIDE),      GET_INT(TRS_CTX_VERTEX_TYPE),      GET_INT(TRS_CTX_VERTEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_NORMAL_ENABLED)      && GET(TRS_CTX_NORMAL_BINDING) == 0,      GET(TRS_CTX_NORMAL_POINTER),      GET_INT(TRS_CTX_NORMAL_STRIDE),      GET_INT(TRS_CTX_NORMAL_TYPE),      3);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_COLOR_ENABLED)       && GET(TRS_CTX_COLOR_BINDING) == 0,       GET(TRS_CTX_COLOR_POINTER),       GET_INT(TRS_CTX_COLOR_STRIDE),       GET_INT(TRS_CTX_COLOR_TYPE),       GET_INT(TRS_CTX_COLOR_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_POINTSIZE_ENABLED)   && GET(TRS_CTX_POINTSIZE_BINDING) == 0,   GET(TRS_CTX_POINTSIZE_POINTER),   GET_INT(TRS_CTX_POINTSIZE_STRIDE),   GET_INT(TRS_CTX_POINTSIZE_TYPE),   1);
    SAVE_ARRAY(GET_BOOL(TRS_CTX_MATRIXINDEX_ENABLED) && GET(TRS_CTX_MATRIXINDEX_BINDING) == 0, GET(TRS_CTX_MATRIXINDEX_POINTER), GET_INT(TRS_CTX_MATRIXINDEX_STRIDE), GET_INT(TRS_CTX_MATRIXINDEX_TYPE), GET_INT(TRS_CTX_MATRIXINDEX_SIZE));
    SAVE_ARRAY(GET_BOOL(TRS_CTX_WEIGHT_ENABLED)      && GET(TRS_CTX_WEIGHT_BINDING) == 0,      GET(TRS_CTX_WEIGHT_POINTER),      GET_INT(TRS_CTX_WEIGHT_STRIDE),      GET_INT(TRS_CTX_WEIGHT_TYPE),      GET_INT(TRS_CTX_WEIGHT_SIZE));

    maxTextureUnit = TR_MAX(GL_TEXTURE0, maxTextureUnit);
    for (i = GL_TEXTURE0; i < maxTextureUnit + 1; i++)
    {
        SET(TRS_CTX_CTEXUNIT, i);
        SAVE_ARRAY(GET_BOOL(TRS_CTX_CTEXUNIT_TEXCOORD_ENABLED) && GET(TRS_CTX_CTEXUNIT_TEXCOORD_BINDING) == 0,
                   GET(TRS_CTX_CTEXUNIT_TEXCOORD_POINTER),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_STRIDE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_TYPE),
                   GET_INT(TRS_CTX_CTEXUNIT_TEXCOORD_SIZE));
    }
    SET(TRS_CTX_CTEXUNIT, origUnit);
}

void glTexImage2D.@postlog()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, pixels);
}

void glTexSubImage2D.@postlog()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, pixels);
}

void glCompressedTexImage2D.@postlog()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, data);
}

void glCompressedTexSubImage2D.@postlog()
{
    /* Hint that we don't need to store the texture data in the array cache */
    trForgetPointerData(event, data);
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

void glDeleteTextures.@postcall()
{
    /* Delete the textures from the state tree */
    GLint i, orig = GET_INT(TRS_CTX_TEXTURE);
    
    for (i = 0; i < n; i++)
    {
        trSetStateValue(event, TRS_CTX_TEXTURE, (void*)textures[i]);
        trRemoveStateValue(event, TRS_CTX_TEXTURE);
    }
    trSetStateValue(event, TRS_CTX_TEXTURE, (void*)orig);
}

void glClientActiveTexture.@postcall()
{
    /* Keep track of the greatest texture unit used */
    int n = GET_INT(TRS_CTX_CTEXUNIT_MAX);
    SET(TRS_CTX_CTEXUNIT_MAX, TR_MAX(n, texture));
}


void glBufferData.@postlog()
{
    void* currentData = GET(TRS_CTX_BUFFER_HANDLE_DATA);
    void* newData = 0;

    if (currentData)
    {
        trFree(currentData);
    }

    newData = trMalloc(size);
    if (newData && data)
    {
        memcpy(newData, data, size);
        SET(TRS_CTX_BUFFER_HANDLE_DATA, newData);
    }
}

void glBufferSubData.@postlog()
{
    void* currentData = GET(TRS_CTX_BUFFER_HANDLE_DATA);

    if (currentData && data)
    {
        memcpy(((char*)currentData) + offset, data, size);
    }
}


/* Set the first texture unit as default */
/*
void eglCreateContext.@postcall()
{
    void* origCtx = GET(TRS_CTX);
    SET(TRS_CTX, ret);
    SET(TRS_CTX_CTEXUNIT, GL_TEXTURE0);
    SET(TRS_CTX_TEXUNIT,  GL_TEXTURE0);
    SET(TRS_CTX, origCtx);
}
*/

/*
 *  Save the VBO bindings for each array
 */
#define COPY_BUFFER_BINDING(HOOK, BINDING) \
    void HOOK() \
    { \
        void* origBuffer = GET(TRS_CTX_BUFFER); \
        void* handle; \
        SET(TRS_CTX_BUFFER, GL_ARRAY_BUFFER); \
        handle = GET(TRS_CTX_BUFFER_HANDLE); \
        if (handle) \
        { \
            SET(BINDING, handle); \
        } \
        SET(TRS_CTX_BUFFER, origBuffer); \
    }

COPY_BUFFER_BINDING(glVertexPointer.@postcall,         TRS_CTX_VERTEX_BINDING);
COPY_BUFFER_BINDING(glNormalPointer.@postcall,         TRS_CTX_NORMAL_BINDING);
COPY_BUFFER_BINDING(glColorPointer.@postcall,          TRS_CTX_COLOR_BINDING);
COPY_BUFFER_BINDING(glPointSizePointerOES.@postcall,   TRS_CTX_POINTSIZE_BINDING);
COPY_BUFFER_BINDING(glMatrixIndexPointerOES.@postcall, TRS_CTX_MATRIXINDEX_BINDING);
COPY_BUFFER_BINDING(glWeightPointerOES.@postcall,      TRS_CTX_WEIGHT_BINDING);
COPY_BUFFER_BINDING(glTexCoordPointer.@postcall,       TRS_CTX_CTEXUNIT_TEXCOORD_BINDING);

/*
 *  Save the actual array pointer or an offset in
 *  the case of VBO data.
 */
#define SAVE_ARRAY_POINTER(HOOK, BINDING) \
    void HOOK() \
    { \
        if (GET(BINDING)) \
        { \
            trIntegerValue(event, "pointer", (TRint)pointer); \
        } \
        else\
        { \
            trPointerValue(event, "pointer", pointer); \
        } \
    }

SAVE_ARRAY_POINTER(glVertexPointer.pointer,         TRS_CTX_VERTEX_BINDING);
SAVE_ARRAY_POINTER(glNormalPointer.pointer,         TRS_CTX_NORMAL_BINDING);
SAVE_ARRAY_POINTER(glColorPointer.pointer,          TRS_CTX_COLOR_BINDING);
SAVE_ARRAY_POINTER(glPointSizePointerOES.pointer,   TRS_CTX_POINTSIZE_BINDING);
SAVE_ARRAY_POINTER(glMatrixIndexPointerOES.pointer, TRS_CTX_MATRIXINDEX_BINDING);
SAVE_ARRAY_POINTER(glWeightPointerOES.pointer,      TRS_CTX_WEIGHT_BINDING);
SAVE_ARRAY_POINTER(glTexCoordPointer.pointer,       TRS_CTX_CTEXUNIT_TEXCOORD_BINDING);
