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

void vgSetfv.values()
{
    trFloatArrayValue(event, "values", count, values);
}

void vgSetiv.values()
{
    trIntegerArrayValue(event, "values", count, values);
}

void vgGetfv.values()
{
    trFloatArrayValue(event, "values", count, values);
}

void vgGetiv.values()
{
    trIntegerArrayValue(event, "values", count, values);
}

void vgSetParameterfv.values()
{
    trFloatArrayValue(event, "values", count, values);
}

void vgSetParameteriv.values()
{
    trIntegerArrayValue(event, "values", count, values);
}

void vgSetParameterfv.values()
{
    trFloatArrayValue(event, "values", count, values);
}

void vgGetParameteriv.values()
{
    trIntegerArrayValue(event, "values", count, values);
}

void vgGetParameterfv.values()
{
    trFloatArrayValue(event, "values", count, values);
}

void vgSetParameteriv.values()
{
    trIntegerArrayValue(event, "values", count, values);
}

void vgLoadMatrix.m()
{
    trFloatArrayValue(event, "m", 9, m);
}

void vgMultMatrix.m()
{
    trFloatArrayValue(event, "m", 9, m);
}

void vgGetMatrix.m()
{
    trFloatArrayValue(event, "m", 9, m);
}

void vgAppendPathData.pathSegments()
{
    trByteArrayValue(event, "pathSegments", numSegments, pathSegments);
}

void vgClearPath.@postcall()
{
    /* Destroy any previously saved path commands */
    trSetStateValue(event, TRS_PATH, (void*)path);
    trSetStateValue(event, TRS_PATH_SEGMENTS, 0);
}

void vgCreatePath.@postcall()
{
    /* Destroy any previously saved path commands */
    trSetStateValue(event, TRS_PATH, (void*)ret);
    trSetStateValue(event, TRS_PATH_SEGMENTS, 0);
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
    previousPath = (const VGubyte*)GET(TRS_PATH_SEGMENTS);
    
    if (previousPath)
    {
        TRint    prevSegments = GET_INT(TRS_PATH_SEGMENT_COUNT);
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
            trSetStateValueArray(event, TRS_PATH_SEGMENTS, segments, (const TRubyte*)fullSegments);
            trFree(fullSegments);
        }
        
        /* Save the segment count */
        SET(TRS_PATH_SEGMENT_COUNT, (void*)segments);
    }
    else
    {
        trSetStateValueArray(event, TRS_PATH_SEGMENTS, numSegments, (const TRubyte*)pathSegments);
        
        /* Save the segment count */
        SET(TRS_PATH_SEGMENT_COUNT, (void*)numSegments);
    }
    
    
    /* Save the path data */
    switch (GET_INT(TRS_PATH_DATATYPE))
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
    }
}

void vgPointAlongPath.x()
{
    trFloatArrayValue(event, "x", 1, x);
}

void vgPointAlongPath.y()
{
    trFloatArrayValue(event, "y", 1, y);
}

void vgPointAlongPath.tangentX()
{
    trFloatArrayValue(event, "tangentX", 1, tangentX);
}

void vgPointAlongPath.tangentY()
{
    trFloatArrayValue(event, "tangentY", 1, tangentY);
}

void vgPathBounds.minX()
{
    trFloatArrayValue(event, "minX", 1, minX);
}

void vgPathBounds.minY()
{
    trFloatArrayValue(event, "minY", 1, minY);
}

void vgPathBounds.width()
{
    trFloatArrayValue(event, "width", 1, width);
}

void vgPathBounds.height()
{
    trFloatArrayValue(event, "height", 1, height);
}

void vgPathTransformedBounds.minX()
{
    trFloatArrayValue(event, "minX", 1, minX);
}

void vgPathTransformedBounds.minY()
{
    trFloatArrayValue(event, "minY", 1, minY);
}

void vgPathTransformedBounds.width()
{
    trFloatArrayValue(event, "width", 1, width);
}

void vgPathTransformedBounds.height()
{
    trFloatArrayValue(event, "height", 1, height);
}

#define SAVE_IMAGE_DATA(data, dataStride, dataFormat) \
{ \
    const TRubyte* data8 = (const TRubyte*)data; \
    TRint stride         = dataStride; \
    \
    if (stride < 0) \
    { \
        data8 += stride * height; \
        stride = -stride; \
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
        trIntegerArrayValue(event, "data",  stride * height, (const TRint*)data8); \
    case VG_sRGB_565: \
    case VG_sRGBA_5551: \
    case VG_sRGBA_4444: \
        trShortArrayValue(event, "data",  stride * height, (const TRshort*)data8); \
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
        trPointerValue(event, "data", data); \
        break; \
    } \
}


void vgImageSubData.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat);
}

void vgGetImageSubData.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat);
}

void vgWritePixels.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat);
}

void vgReadPixels.data()
{
    SAVE_IMAGE_DATA(data, dataStride, dataFormat);
}

void vgColorMatrix.matrix()
{
    trFloatArrayValue(event, "matrix", 16, matrix);
}

void vgConvolve.kernel()
{
    trShortArrayValue(event, "kernel", kernelWidth * kernelHeight, kernel);
}

void vgSeparableConvolve.kernelX()
{
    trShortArrayValue(event, "kernelX", kernelWidth * kernelHeight, kernelX);
}

void vgSeparableConvolve.kernelY()
{
    trShortArrayValue(event, "kernelY", kernelWidth * kernelHeight, kernelY);
}

void vgLookup.redLUT()
{
    trByteArrayValue(event, "redLUT", 256, redLUT);
}

void vgLookup.greenLUT()
{
    trByteArrayValue(event, "greenLUT", 256, greenLUT);
}

void vgLookup.blueLUT()
{
    trByteArrayValue(event, "blueLUT", 256, blueLUT);
}

void vgLookup.alphaLUT()
{
    trByteArrayValue(event, "alphaLUT", 256, alphaLUT);
}

void vgLookupSingle.lookupTable()
{
    trIntegerArrayValue(event, "lookupTable", 256, (const TRint*)lookupTable);
}

void vgModifyPathCoords.pathData()
{
    const VGubyte* pathSegments = (const VGubyte*)GET(TRS_PATH_SEGMENTS);
    
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
        switch (GET_INT(TRS_PATH_DATATYPE))
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
        }
    }
}

void vguPolygon.points()
{
    trFloatArrayValue(event, "points", count, points);
}

void vguComputeWarpQuadToSquare.matrix()
{
    trFloatArrayValue(event, "matrix", 9, matrix);
}

void vguComputeWarpSquareToQuad.matrix()
{
    trFloatArrayValue(event, "matrix", 9, matrix);
}

void vguComputeWarpQuadToQuad.matrix()
{
    trFloatArrayValue(event, "matrix", 9, matrix);
}

