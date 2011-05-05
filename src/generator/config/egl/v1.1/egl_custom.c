/**
 *  Custom EGL tracer functions
 */

#include <EGL/egl.h>
#include "tracer.h"
#include "tracer_util.h"
#include "tracer_compiler.h"

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

TR_EXPORT void (* EGLAPIENTRY eglGetProcAddress(const char *procname))(void)
/*TR_EXPORT void (*eglGetProcAddress(const char *procname))(void)*/
{
    typedef void (*ProcType)(void);
    TREvent* event    = trBeginEvent(0, "eglGetProcAddress");
    void (*ret)(void) = ((void (* (*)(const char*)) (void))event->function->pointer)(procname);

    /* Redirect all supported extensions to our own code */
    /* TODO: How to handle GL extensions? */
    if (ret)
    {
#if EGL_some_extensions
        HIJACK_EXT(eglSomeExtension);
#endif
    }

    trByteArrayValue(event, "procname", triStringLength(procname) + 1, (const TRubyte*)procname);
    trPointerValue(event, 0, (void*)ret);
    trEndEvent(event);
    return ret;
}
