/*
 * GLES tracer hooks
 */

/* State management helpers */
#define ENABLE(PATH)   trSetStateValue(event, PATH, (void*)1)
#define DISABLE(PATH)  trSetStateValue(event, PATH, (void*)0)
#define GET(PATH)      trGetStateValue(event, PATH)
#define GET_INT(PATH)  ((int)GET(PATH))
#define GET_BOOL(PATH) ((TRbool)GET(PATH))
#define SET(PATH, VALUE) trSetStateValue(event, PATH, (void*)VALUE)

TRHandle @GLshadersourceline.serialize(TREvent* event, TRhandle handle)
{
    /* TODO: this only works with null-terminated strings */
    const char* s = (const char*)handle;

    if (s)
    {
        trByteArrayValue(event, "src", strlen(s) + 1, (const TRubyte*)s);
    }
}

void glShaderBinary.binary()
{
    trByteArrayValue(event, "binary", length, (const TRubyte*)binary);
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
    if (newData)
    {
        memcpy(newData, data, size);
        SET(TRS_CTX_BUFFER_HANDLE_DATA, newData);
    }
}

void glBufferSubData.@postlog()
{
    void* currentData = GET(TRS_CTX_BUFFER_HANDLE_DATA);

    if (currentData)
    {
        memcpy(((char*)currentData) + offset, data, size);
    }
}

void glEnableVertexAttribArray.@postlog()
{
    /* Keep track of the greatest vertex attrib index used */
    int n = GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_MAX_INDEX);
    SET(TRS_CTX_VERTEX_ATTRIB_ARRAY_MAX_INDEX, TR_MAX(n, index));
    
    ENABLE(TRS_CTX_VERTEX_ATTRIB_ARRAY_ENABLED);
}

void glDisableVertexAttribArray.@postlog()
{
    DISABLE(TRS_CTX_VERTEX_ATTRIB_ARRAY_ENABLED);
}

void glVertexAttribPointer.ptr()
{
    void* origBufferType = GET(TRS_CTX_BUFFER);
    void* binding;
    SET(TRS_CTX_BUFFER, GL_ARRAY_BUFFER);
    binding = GET(TRS_CTX_BUFFER_HANDLE);
    SET(TRS_CTX_BUFFER, origBufferType);

    /* Interpret pointer as offset into a VBO or a real pointer */
    if (binding)
    {
        /* FIXME not 64-bit safe */
        trIntegerValue(event, "ptr", (int)ptr);
    }
    else
    {
        trPointerValue(event, "ptr", ptr);
    }
}

void glVertexAttribPointer.@postlog()
{
    void* origBufferType = GET(TRS_CTX_BUFFER);
    void* binding;
    SET(TRS_CTX_BUFFER, GL_ARRAY_BUFFER);
    binding = GET(TRS_CTX_BUFFER_HANDLE);
    SET(TRS_CTX_BUFFER, origBufferType);

    SET(TRS_CTX_VERTEX_ATTRIB_ARRAY_BINDING, binding);
}

void glDrawElements.indices()
{
    void* origBufferType = GET(TRS_CTX_BUFFER);
    void* binding;
    SET(TRS_CTX_BUFFER, GL_ELEMENT_ARRAY_BUFFER);
    binding = GET(TRS_CTX_BUFFER_HANDLE);
    SET(TRS_CTX_BUFFER, origBufferType);

    /* Only save indices if there isn't a bound element array buffer */
    if (!binding)
    {
        if (type == GL_UNSIGNED_BYTE)
            trByteArrayValue(event, "indices", count, (const TRubyte*)indices);
        else if (type == GL_UNSIGNED_SHORT)
            trShortArrayValue(event, "indices", count, (const TRshort*)indices);
        else
            trPointerValue(event, "indices", indices);
    }
    else
    {
        trPointerValue(event, "indices", indices);
    }
}

#define SAVE_ARRAY(POINTER, STRIDE, TYPE, SIZE)                \
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
    const void* origBufferType = GET(TRS_CTX_BUFFER);
    const void* origIndex = GET(TRS_CTX_VERTEX_ATTRIB_ARRAY);
    const void* indexData = indices;
    int maxVertexAttribArrayIndex = GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_MAX_INDEX);

    /* If there is a bound element array buffer, use it as a source of index data */
    SET(TRS_CTX_BUFFER, GL_ELEMENT_ARRAY_BUFFER);
    if (GET(TRS_CTX_BUFFER_HANDLE))
    {
        const TRbyte* pointer = (const TRbyte*)GET(TRS_CTX_BUFFER_HANDLE_DATA);
        TRuint offset = (TRuint)indexData;
        indexData = (void*)(pointer + offset);
    }
    SET(TRS_CTX_BUFFER, origBufferType);

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

    /* Save all enabled vertex attrib arrays that don't point to a buffer object */
    for (i = 0; i < maxVertexAttribArrayIndex + 1; i++)
    {
        SET(TRS_CTX_VERTEX_ATTRIB_ARRAY, i);

        if (!GET_BOOL(TRS_CTX_VERTEX_ATTRIB_ARRAY_ENABLED))
        {
            continue;
        }

        if (GET(TRS_CTX_VERTEX_ATTRIB_ARRAY_BINDING))
        {
            continue;
        }
        SAVE_ARRAY(GET(TRS_CTX_VERTEX_ATTRIB_ARRAY_POINTER),
                   GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_STRIDE),
                   GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_TYPE),
                   GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_SIZE));
    }

    /* Restore the original index */
    SET(TRS_CTX_VERTEX_ATTRIB_ARRAY, origIndex);
}

void glDrawArrays.@precall()
{
    int maxIndex = first + count - 1, i;
    const void* origIndex = GET(TRS_CTX_VERTEX_ATTRIB_ARRAY);
    int maxVertexAttribArrayIndex = GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_MAX_INDEX);

    /* Save all enabled vertex attrib arrays that don't point to a buffer object */
    for (i = 0; i < maxVertexAttribArrayIndex + 1; i++)
    {
        SET(TRS_CTX_VERTEX_ATTRIB_ARRAY, i);

        if (!GET_BOOL(TRS_CTX_VERTEX_ATTRIB_ARRAY_ENABLED))
        {
            continue;
        }

        if (GET(TRS_CTX_VERTEX_ATTRIB_ARRAY_BINDING))
        {
            continue;
        }
        SAVE_ARRAY(GET(TRS_CTX_VERTEX_ATTRIB_ARRAY_POINTER),
                   GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_STRIDE),
                   GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_TYPE),
                   GET_INT(TRS_CTX_VERTEX_ATTRIB_ARRAY_SIZE));
    }

    /* Restore the original index */
    SET(TRS_CTX_VERTEX_ATTRIB_ARRAY, origIndex);
}

void glGetString.@return()
{
    if (ret)
    {
        trByteArrayValue(event, NULL, strlen((const char*)ret) + 1, (const TRubyte*)ret);
    }
}
