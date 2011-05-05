#include "gl.h"
#include "tracer.h"
#include "tracer_util.h"

typedef void (*f_ptr)(void);

/* Function call macros. Up to 12 function arguments supported. */
#define TR_CALL0(RET, FUNC) \
         ((RET (*) ()) (FUNC)->pointer)()
#define TR_CALL1(RET, FUNC, T1, P1) \
         ((RET (*) (T1)) (FUNC)->pointer)(P1)
#define TR_CALL2(RET, FUNC, T1, P1, T2, P2) \
         ((RET (*) (T1, T2)) (FUNC)->pointer)(P1, P2)
#define TR_CALL3(RET, FUNC, T1, P1, T2, P2, T3, P3) \
         ((RET (*) (T1, T2, T3)) (FUNC)->pointer)(P1, P2, P3)
#define TR_CALL4(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4) \
         ((RET (*) (T1, T2, T3, T4)) (FUNC)->pointer)(P1, P2, P3, P4)
#define TR_CALL5(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
         ((RET (*) (T1, T2, T3, T4, T5)) (FUNC)->pointer)(P1, P2, P3, P4, P5)
#define TR_CALL6(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
         ((RET (*) (T1, T2, T3, T4, T5, T6)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6)
#define TR_CALL7(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7) \
         ((RET (*) (T1, T2, T3, T4, T5, T6, T7)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6, P7)
#define TR_CALL8(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8) \
         ((RET (*) (T1, T2, T3, T4, T5, T6, T7, T8)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6, P7, P8)
#define TR_CALL9(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8, T9, P9) \
         ((RET (*) (T1, T2, T3, T4, T5, T6, T7, T8, T9)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6, P7, P8, P9)
#define TR_CALL10(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8, T9, P9, T10, P10) \
         ((RET (*) (T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)
#define TR_CALL11(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8, T9, P9, T10, P10, T11, P11) \
         ((RET (*) (T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)
#define TR_CALL12(RET, FUNC, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8, T9, P9, T10, P10, T11, P11, T12, P12) \
         ((RET (*) (T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)) (FUNC)->pointer)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)

__declspec(dllexport) void (*eglGetProcAddress(const char *name))(void)
{
    TREvent* event = trBeginEvent(0, "eglGetProcAddress");

    void (*ret)(void) = ((void (* (*)(const char*)) (void))event->function->pointer)(name);
    
    trByteArrayValue(event, "name", triStringLength(name) + 1, (const TRubyte*)name);
    trEndEvent(event);
    return ret;
}
