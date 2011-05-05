/*
 *  GLES 2.0 specific code for the C source generator
 */

typedef const char* GLshadersourceline;

static GLshadersourceline createGLshadersourceline2(void* context, const char* src)
{
    return src;
}

static GLshadersourceline modifyGLshadersourceline3(void* context, GLshadersourceline s, const char* src)
{
    return src;
}

static void destroyGLshadersourceline2(void* context, GLshadersourceline s)
{
    (void)s;
}
