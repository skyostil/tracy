/**
 *  Custom GLES tracer functions
 */

#include <GLES/gl.h>
#include "tracer.h"
#include "tracer_util.h"
#include "tracer_compiler.h"

/* Do this in EGL instead */
#if 0

/* Symbol exporting macros */
#if defined(TR_PLATFORM_WIN32) || defined(TR_PLATFORM_SYMBIAN)
#  define TR_EXPORT     __declspec(dllexport)
#else
#  define TR_EXPORT
#endif

#define HIJACK_EXT(extFunc)                  \
    if (triStringsEqual(procname, #extFunc)) \
    {                                        \
        ret = (ProcType)extFunc;             \
    }

TR_EXPORT void (*eglGetProcAddress(const char *procname))(void)
{
    typedef void (*ProcType)(void);
    TREvent* event    = trBeginEvent(0, "eglGetProcAddress");
    void (*ret)(void) = ((void (* (*)(const char*)) (void))event->function->pointer)(procname);

    /* Redirect all supported extensions to our own code */
    if (ret)
    {
#if GL_OES_draw_texture
        HIJACK_EXT(glDrawTexsOES);
        HIJACK_EXT(glDrawTexiOES);
        HIJACK_EXT(glDrawTexfOES);
        HIJACK_EXT(glDrawTexxOES);
        HIJACK_EXT(glDrawTexsvOES);
        HIJACK_EXT(glDrawTexivOES);
        HIJACK_EXT(glDrawTexfvOES);
        HIJACK_EXT(glDrawTexxvOES);
#endif
#if GL_OES_matrix_palette
        HIJACK_EXT(glMatrixIndexPointerOES);
        HIJACK_EXT(glWeightPointerOES);
#endif
#if GL_OES_point_size_array
        HIJACK_EXT(glPointSizePointerOES);
#endif
#if GL_OES_matrix_get
        HIJACK_EXT(glLoadPaletteFromModelViewMatrixOES);
#endif
    }

    trByteArrayValue(event, "procname", triStringLength(procname) + 1, (const TRubyte*)procname);
    trPointerValue(event, 0, (void*)ret);
    trEndEvent(event);
    return ret;
}
#endif /* if 0 */
