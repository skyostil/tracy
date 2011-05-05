/*
 *  Hooks for the OpenVG tracer
 */
 
/* State management helpers */
#define ENABLE(PATH)     trSetStateValue(event, PATH, (void*)1)
#define DISABLE(PATH)    trSetStateValue(event, PATH, (void*)0)
#define GET(PATH)        trGetStateValue(event, PATH)
#define SET(PATH, VALUE) trSetStateValue(event, PATH, (void*)VALUE)
#define GET_INT(PATH)    ((int)GET(PATH))
#define GET_BOOL(PATH)   ((TRbool)GET(PATH))
#define ABS(N)           ((N) < 0 ? (-(N)) : (N))

/*
 * The following is based on the upper byte of the type parameter 
 * being fixed to different ranges for different objects.
 */
#define SAVE_OBJECT_BY_PARAMETER_TYPE(object, paramType) \
switch (paramType & 0xff00) \
{ \
case VG_PATH_FORMAT: \
    trObjectValue(event, "object", TR_VGPATH_CLSID,  (TRhandle)object); \
    break; \
case VG_PAINT_TYPE: \
    trObjectValue(event, "object", TR_VGPAINT_CLSID, (TRhandle)object); \
    break; \
case VG_IMAGE_FORMAT: \
    trObjectValue(event, "object", TR_VGIMAGE_CLSID, (TRhandle)object); \
    break; \
default: \
    trIntegerValue(event, "object", object); \
    break; \
}

void vgSetParameteri.object()
{
    SAVE_OBJECT_BY_PARAMETER_TYPE(object, paramType);
}

void vgSetParameterf.object()
{
    SAVE_OBJECT_BY_PARAMETER_TYPE(object, paramType);
}

void vgSetParameteriv.object()
{
    SAVE_OBJECT_BY_PARAMETER_TYPE(object, paramType);
}

void vgSetParameterfv.object()
{
    SAVE_OBJECT_BY_PARAMETER_TYPE(object, paramType);
}

void vgClearPath.@postcall()
{
    /* Destroy any previously saved path commands */
    trSetStateValue(event, TRS_CTX_OBJECT, (void*)path);
    trSetStateValue(event, TRS_CTX_OBJECT_SEGMENTS, 0);
}

void vgCreatePath.@postcall()
{
    /* Destroy any previously saved path commands */
    trSetStateValue(event, TRS_CTX_OBJECT, (void*)ret);
    trSetStateValue(event, TRS_CTX_OBJECT_SEGMENTS, 0);
}

void vgAppendPathData.pathData()
{
    static const VGubyte commandCoords[] = 
    {
    /*  0  2  4  6  8 10 12 14 16 18 20 22 24 26 */
      	0, 2, 2, 1, 1, 4, 6, 2, 4, 5, 5, 5, 5, 0
    };
    VGint numCoords = 0, i;
    const VGubyte* previousPath;
    
    /* Count the number of coordinates */
    for (i = 0; i < numSegments; i++)
    {
        VGubyte cmd = pathSegments[i] >> 1;
        if (cmd < sizeof(commandCoords) / sizeof(commandCoords[0]))
        {
            numCoords += commandCoords[cmd];
        }
    }
    
    /* Save the path segments */
    previousPath = (const VGubyte*)GET(TRS_CTX_OBJECT_SEGMENTS);
    
    if (previousPath)
    {
        TRint    prevSegments = GET_INT(TRS_CTX_OBJECT_SEGMENT_COUNT);
        TRint        segments = numSegments + prevSegments;
        VGubyte* fullSegments = (VGubyte*)trMalloc(segments);
        
        if (fullSegments)
        {
            /* Append the previous path */
            for (i = 0; i < prevSegments; i++)
            {
                fullSegments[i] = previousPath[i];
            }
    
            /* Append the currently appended bit */
            for (i = 0; i < numSegments; i++)
            {
                fullSegments[prevSegments + i] = pathSegments[i];
            }
            trSetStateValueArray(event, TRS_CTX_OBJECT_SEGMENTS, segments, (const TRubyte*)fullSegments);
            trFree(fullSegments);
        }
        
        /* Save the segment count */
        SET(TRS_CTX_OBJECT_SEGMENT_COUNT, (void*)segments);
    }
    else
    {
        trSetStateValueArray(event, TRS_CTX_OBJECT_SEGMENTS, numSegments, (const TRubyte*)pathSegments);
        
        /* Save the segment count */
        SET(TRS_CTX_OBJECT_SEGMENT_COUNT, (void*)numSegments);
    }
    
    
    /* Save the path data */
    switch (GET_INT(TRS_CTX_OBJECT_DATATYPE))
    {
    case VG_PATH_DATATYPE_S_8:
        trByteArrayValue(event,    "pathData", numCoords, (const TRubyte*)pathData);
    break;
    case VG_PATH_DATATYPE_S_16:
        trShortArrayValue(event,   "pathData", numCoords, (const TRshort*)pathData);
    break;
    case VG_PATH_DATATYPE_S_32:
        trIntegerArrayValue(event, "pathData", numCoords, (const TRint*)pathData);
    break;
    case VG_PATH_DATATYPE_F:
        trFloatArrayValue(event,   "pathData", numCoords, (const TRfloat*)pathData);
    break;
    default:
        trPanic("Bad path data type");
        break;
    }
}

#define FLIP_IMAGE_VERTICALLY(data, stride, height) \
{ \
    TRint x, y; \
    for (y = 0; y < height / 2; y++) \
    { \
        TRubyte* data1 = (TRubyte*)&data[y * stride]; \
        TRubyte* data2 = (TRubyte*)&data[(height - y - 1) * stride]; \
        TRubyte tmp; \
        for (x = 0; x < stride && data1 != data2; x++) \
        { \
            tmp      = *data1; \
            *data1++ = *data2; \
            *data2++ = tmp; \
        } \
    } \
}

#define SAVE_IMAGE_DATA(data, dataStride, dataFormat, height) \
{ \
    const TRubyte* data8 = (const TRubyte*)data; \
    TRint stride         = dataStride; \
    \
    /* 
     * If we have a negative stride, the image data must be flipped vertically 
     * and the pointer adjusted to point to the start of the data.
     */ \
    if (stride < 0) \
    { \
        data8 += stride * (height - 1); \
        stride = -stride; \
        FLIP_IMAGE_VERTICALLY(data8, stride, height); \
    } \
    \
    switch (dataFormat) \
    { \
    case VG_sRGBX_8888: \
    case VG_sRGBA_8888: \
    case VG_sRGBA_8888_PRE: \
    case VG_lRGBX_8888: \
    case VG_lRGBA_8888: \
    case VG_lRGBA_8888_PRE: \
    case VG_sXRGB_8888: \
    case VG_sARGB_8888: \
    case VG_sARGB_8888_PRE: \
    case VG_lXRGB_8888: \
    case VG_lARGB_8888: \
    case VG_lARGB_8888_PRE: \
    case VG_sBGRX_8888: \
    case VG_sBGRA_8888: \
    case VG_sBGRA_8888_PRE: \
    case VG_lBGRX_8888: \
    case VG_lBGRA_8888: \
    case VG_lBGRA_8888_PRE: \
    case VG_sXBGR_8888: \
    case VG_sABGR_8888: \
    case VG_sABGR_8888_PRE: \
    case VG_lXBGR_8888: \
    case VG_lABGR_8888: \
    case VG_lABGR_8888_PRE: \
        trIntegerArrayValue(event, "data",  stride * height / 4, (const TRint*)data8); \
        break; \
    case VG_sRGB_565: \
    case VG_sRGBA_5551: \
    case VG_sARGB_1555: \
    case VG_sBGRA_5551: \
    case VG_sABGR_1555: \
    case VG_sRGBA_4444: \
    case VG_sARGB_4444: \
    case VG_sBGRA_4444: \
    case VG_sABGR_4444: \
        trShortArrayValue(event, "data",  stride * height / 2, (const TRshort*)data8); \
        break; \
    case VG_sL_8: \
    case VG_lL_8: \
    case VG_A_8: \
        trByteArrayValue(event, "data",  stride * height, data8); \
        break; \
    case VG_BW_1: \
        trByteArrayValue(event, "data",  stride * height, data8); \
        break; \
    default: \
        trByteArrayValue(event, "data",  stride * height, data8); \
        break; \
    } \
    \
    /*
     * Flip the image back if we flipped it earlier.
     */ \
    if (dataStride < 0) \
    { \
        FLIP_IMAGE_VERTICALLY(data8, stride, height); \
    } \
    /* Hint that we don't need to store the image data in the array cache */ \
    trForgetPointerData(event, data8); \
}

void vgImageSubData.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat, height);
}

void vgImageSubData.dataStride()
{
    trIntegerValue(event, "dataStride", ABS(dataStride));
}

void vgGetImageSubData.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat, height);
}

void vgGetImageSubData.dataStride()
{
    trIntegerValue(event, "dataStride", ABS(dataStride));
}

void vgWritePixels.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat, height);
}

void vgWritePixels.dataStride()
{
    trIntegerValue(event, "dataStride", ABS(dataStride));
}

void vgReadPixels.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat, height);
}

void vgReadPixels.dataStride()
{
    trIntegerValue(event, "dataStride", ABS(dataStride));
}

void vgModifyPathCoords.pathData()
{
    const VGubyte* pathSegments = (const VGubyte*)GET(TRS_CTX_OBJECT_SEGMENTS);
    
    if (pathSegments)
    {
        static const VGubyte commandCoords[] = 
        {
          	0, 2, 2, 1, 1, 4, 6, 2, 4, 5, 5, 5, 5, 0
        };
        VGint numCoords = 0, i;
        
        /* Count the number of coordinates */
        for (i = startIndex; i < startIndex + numSegments; i++)
        {
            VGubyte cmd = pathSegments[i] >> 1;
            if (cmd < sizeof(commandCoords) / sizeof(commandCoords[0]))
            {
                numCoords += commandCoords[cmd];
            }
        }
        
        /* Save the path data */
        switch (GET_INT(TRS_CTX_OBJECT_DATATYPE))
        {
        case VG_PATH_DATATYPE_S_8:
            trByteArrayValue(event,    "pathData", numCoords, (const TRubyte*)pathData);
        break;
        case VG_PATH_DATATYPE_S_16:
            trShortArrayValue(event,   "pathData", numCoords, (const TRshort*)pathData);
        break;
        case VG_PATH_DATATYPE_S_32:
            trIntegerArrayValue(event, "pathData", numCoords, (const TRint*)pathData);
        break;
        case VG_PATH_DATATYPE_F:
            trFloatArrayValue(event,   "pathData", numCoords, (const TRfloat*)pathData);
        break;
        default:
            trPanic("Bad path data type");
            break;
        }
    }
}
