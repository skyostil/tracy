/**
 * C source generated from 95 events (7 frames). 
 *
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

/** A macro for copying data into an array */
#define LOAD_ARRAY(TO, FROM, LENGTH) \
    { \
       int i; \
       for (i = 0; i < (LENGTH); i++) \
          (TO)[i] = (FROM)[i]; \
    }

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

/* Objects */ 
static EGLSurface eglsurface_0_7af80 = (EGLSurface)0x7af80;
static EGLDisplay egldisplay_0_1 = (EGLDisplay)0x1;
static GLuniform gluniform_33453_1 = (GLuniform)0x1;
static GLuniform gluniform_33453_2 = (GLuniform)0x2;
static EGLNativeDisplayType eglnativedisplaytype_0_3c768 = (EGLNativeDisplayType)0x3c768;
static EGLNativeWindowType eglnativewindowtype_0_1a00002 = (EGLNativeWindowType)0x1a00002;
static GLattribute glattribute_33453_1 = (GLattribute)0x1;
static GLprogram glprogram_6faf8_33453 = (GLprogram)0x33453;
static GLshader glshader_6faf8_11171 = (GLshader)0x11171;
static GLshader glshader_6faf8_222e2 = (GLshader)0x222e2;
static GLshadersourceline glshadersourceline_0_3ab8d4f0 = (GLshadersourceline)0x3ab8d4f0;
static GLshadersourceline glshadersourceline_0_f098 = (GLshadersourceline)0xf098;
static GLshadersourceline glshadersourceline_0_f3a8 = (GLshadersourceline)0xf3a8;
static EGLConfig eglconfig_0_c = (EGLConfig)0xc;
static EGLContext eglcontext_0_6faf8 = (EGLContext)0x6faf8;

/* 37 arrays */ 
static short short_array0[96];
static float float_array1[50];
static float float_array2[75];
static char char_array3[10];
static GLfloat glfloat_array4[16];
static char char_array5[30];
static GLint glint_array6[1];
static char char_array7[11];
static char char_array8[9];
static GLint glint_array9[1];
static GLint glint_array10[2];
static GLshadersourceline glshadersourceline_array11[2];
static char char_array12[22];
static char char_array13[64];
static GLuint gluint_array14[1];
static char char_array15[22];
static GLubyte glubyte_array16[16];
static EGLint eglint_array17[1];
static char char_array18[435];
static EGLint eglint_array19[1];
static EGLint eglint_array20[1];
static EGLint eglint_array21[1];
static EGLint eglint_array22[1];
static EGLint eglint_array23[1];
static EGLint eglint_array24[1];
static EGLint eglint_array25[1];
static EGLint eglint_array26[1];
static EGLint eglint_array27[1];
static EGLint eglint_array28[1];
static EGLint eglint_array29[1];
static EGLint eglint_array30[1];
static EGLint eglint_array31[3];
static EGLint eglint_array32[1];
static int int_array33[1];
static EGLint eglint_array34[21];
static char char_array35[19];
static char char_array36[18];

/* Array data */ 
static const char arrayData0[435] = {
    69, 71, 76, 95, 75, 72, 82, 95, 
    105, 109, 97, 103, 101, 32, 69, 71, 
    76, 95, 75, 72, 82, 95, 105, 109, 
    97, 103, 101, 95, 98, 97, 115, 101, 
    32, 69, 71, 76, 95, 75, 72, 82, 
    95, 105, 109, 97, 103, 101, 95, 112, 
    105, 120, 109, 97, 112, 32, 69, 71, 
    76, 95, 78, 79, 75, 95, 105, 109, 
    97, 103, 101, 95, 115, 104, 97, 114, 
    101, 100, 32, 69, 71, 76, 95, 75, 
    72, 82, 95, 103, 108, 95, 116, 101, 
    120, 116, 117, 114, 101, 95, 50, 68, 
    95, 105, 109, 97, 103, 101, 32, 69, 
    71, 76, 95, 75, 72, 82, 95, 103, 
    108, 95, 116, 101, 120, 116, 117, 114, 
    101, 95, 99, 117, 98, 101, 109, 97, 
    112, 95, 105, 109, 97, 103, 101, 32, 
    69, 71, 76, 95, 75, 72, 82, 95, 
    103, 108, 95, 114, 101, 110, 100, 101, 
    114, 98, 117, 102, 102, 101, 114, 95, 
    105, 109, 97, 103, 101, 32, 69, 71, 
    76, 95, 75, 72, 82, 95, 118, 103, 
    95, 112, 97, 114, 101, 110, 116, 95, 
    105, 109, 97, 103, 101, 32, 69, 71, 
    76, 95, 78, 79, 75, 73, 65, 95, 
    116, 101, 120, 116, 117, 114, 101, 95, 
    102, 114, 111, 109, 95, 112, 105, 120, 
    109, 97, 112, 32, 69, 71, 76, 95, 
    78, 79, 75, 95, 116, 101, 120, 116, 
    117, 114, 101, 95, 102, 114, 111, 109, 
    95, 112, 105, 120, 109, 97, 112, 32, 
    69, 71, 76, 95, 75, 72, 82, 95, 
    102, 101, 110, 99, 101, 95, 115, 121, 
    110, 99, 32, 69, 71, 76, 95, 73, 
    77, 71, 95, 99, 111, 110, 116, 101, 
    120, 116, 95, 112, 114, 105, 111, 114, 
    105, 116, 121, 32, 69, 71, 76, 95, 
    75, 72, 82, 95, 108, 111, 99, 107, 
    95, 115, 117, 114, 102, 97, 99, 101, 
    32, 69, 71, 76, 95, 75, 72, 82, 
    95, 108, 111, 99, 107, 95, 115, 117, 
    114, 102, 97, 99, 101, 50, 32, 69, 
    71, 76, 95, 78, 79, 75, 95, 105, 
    109, 97, 103, 101, 95, 121, 117, 118, 
    32, 69, 71, 76, 95, 78, 79, 75, 
    95, 105, 109, 97, 103, 101, 95, 121, 
    117, 118, 95, 112, 105, 120, 109, 97, 
    112, 32, 69, 71, 76, 95, 78, 79, 
    75, 95, 105, 109, 97, 103, 101, 95, 
    121, 117, 118, 95, 102, 114, 97, 109, 
    101, 98, 117, 102, 102, 101, 114, 32, 
    69, 71, 76, 95, 78, 79, 75, 95, 
    105, 109, 97, 103, 101, 95, 102, 114, 
    97, 109, 101, 98, 117, 102, 102, 101, 
    114, 32, 0
};

static const char arrayData1[18] = {
    101, 103, 108, 67, 114, 101, 97, 116, 
    101, 73, 109, 97, 103, 101, 75, 72, 
    82, 0
};

static GLshadersourceline arrayData2[1];

static GLshadersourceline arrayData3[2];

static const EGLint arrayData4[1] = {
    0
};

static const GLubyte arrayData5[16] = {
    80, 111, 119, 101, 114, 86, 82, 32, 
    83, 71, 88, 32, 53, 51, 48, 0
};

static const GLint arrayData6[1] = {
    1
};

static const GLint arrayData7[1] = {
    10
};

static const GLuint arrayData8[1] = {
    70001
};

static const short arrayData9[96] = {
    0, 1, 6, 6, 5, 0, 1, 2, 
    7, 7, 6, 1, 2, 3, 8, 8, 
    7, 2, 3, 4, 9, 9, 8, 3, 
    5, 6, 11, 11, 10, 5, 6, 7, 
    12, 12, 11, 6, 7, 8, 13, 13, 
    12, 7, 8, 9, 14, 14, 13, 8, 
    10, 11, 16, 16, 15, 10, 11, 12, 
    17, 17, 16, 11, 12, 13, 18, 18, 
    17, 12, 13, 14, 19, 19, 18, 13, 
    15, 16, 21, 21, 20, 15, 16, 17, 
    22, 22, 21, 16, 17, 18, 23, 23, 
    22, 17, 18, 19, 24, 24, 23, 18
};

static const char arrayData10[22] = {
    103, 108, 71, 101, 116, 80, 114, 111, 
    103, 114, 97, 109, 66, 105, 110, 97, 
    114, 121, 79, 69, 83, 0
};

static const char arrayData11[19] = {
    103, 108, 80, 114, 111, 103, 114, 97, 
    109, 66, 105, 110, 97, 114, 121, 79, 
    69, 83, 0
};

static const char arrayData12[9] = {
    97, 95, 118, 101, 114, 116, 101, 120, 
    0
};

static const char arrayData13[11] = {
    97, 95, 116, 101, 120, 99, 111, 111, 
    114, 100, 0
};

static const GLint arrayData14[1] = {
    780
};

static const GLint arrayData15[2] = {
    64, 715
};

static const char arrayData16[10] = {
    117, 95, 115, 97, 109, 112, 108, 101, 
    114, 0
};

static const GLfloat arrayData17[16] = {
    1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 
    0.000000f, 0.000000f, -1.000000f, 0.000000f, -0.000000f, -0.000000f, 0.000000f, 1.000000f
};

static const char arrayData18[30] = {
    117, 95, 109, 111, 100, 101, 108, 118, 
    105, 101, 119, 95, 112, 114, 111, 106, 
    101, 99, 116, 105, 111, 110, 95, 109, 
    97, 116, 114, 105, 120, 0
};

static const float arrayData19[75] = {
    -0.750000f, -0.750000f, 0.250000f, -0.375000f, -0.750000f, 0.250000f, 0.000000f, -0.750000f, 
    0.250000f, 0.375000f, -0.750000f, 0.250000f, 0.750000f, -0.750000f, 0.250000f, -0.750000f, 
    -0.375000f, 0.250000f, -0.375000f, -0.375000f, 0.250000f, 0.000000f, -0.375000f, 0.250000f, 
    0.375000f, -0.375000f, 0.250000f, 0.750000f, -0.375000f, 0.250000f, -0.750000f, 0.000000f, 
    0.250000f, -0.375000f, 0.000000f, 0.250000f, 0.000000f, 0.000000f, 0.250000f, 0.375000f, 
    0.000000f, 0.250000f, 0.750000f, 0.000000f, 0.250000f, -0.750000f, 0.375000f, 0.250000f, 
    -0.375000f, 0.375000f, 0.250000f, 0.000000f, 0.375000f, 0.250000f, 0.375000f, 0.375000f, 
    0.250000f, 0.750000f, 0.375000f, 0.250000f, -0.750000f, 0.750000f, 0.250000f, -0.375000f, 
    0.750000f, 0.250000f, 0.000000f, 0.750000f, 0.250000f, 0.375000f, 0.750000f, 0.250000f, 
    0.750000f, 0.750000f, 0.250000f
};

static const EGLint arrayData20[1] = {
    5
};

static const EGLint arrayData21[1] = {
    6
};

static const EGLint arrayData22[1] = {
    12
};

static const EGLint arrayData23[1] = {
    33
};

static const EGLint arrayData24[3] = {
    12440, 2, 12344
};

static const char arrayData25[22] = {
    49, 46, 52, 32, 98, 117, 105, 108, 
    100, 32, 49, 46, 52, 46, 49, 52, 
    46, 50, 53, 49, 52, 0
};

static const EGLint arrayData26[21] = {
    12339, 4, 12352, 4, 12324, 0, 12323, 0, 
    12322, 0, 12321, 0, 12325, 1, 12326, 1, 
    12337, 4, 12338, 1, 12344
};

static const EGLint arrayData27[1] = {
    2
};

static const char arrayData28[19] = {
    101, 103, 108, 68, 101, 115, 116, 114, 
    111, 121, 73, 109, 97, 103, 101, 75, 
    72, 82, 0
};

static const EGLint arrayData29[1] = {
    8
};

static const EGLint arrayData30[1] = {
    4
};

static const char arrayData31[64] = {
    127, 127, 127, 255, 191, 191, 191, 127, 
    127, 127, 127, 127, 191, 191, 191, 255, 
    191, 191, 191, 127, 127, 127, 127, 0, 
    191, 191, 191, 0, 127, 127, 127, 127, 
    127, 127, 127, 127, 191, 191, 191, 0, 
    127, 127, 127, 0, 191, 191, 191, 127, 
    191, 191, 191, 255, 127, 127, 127, 127, 
    191, 191, 191, 127, 127, 127, 127, 255
};

static const EGLint arrayData32[1] = {
    24
};

static const float arrayData33[50] = {
    0.000000f, 0.000000f, 0.250000f, 0.000000f, 0.500000f, 0.000000f, 0.750000f, 0.000000f, 
    1.000000f, 0.000000f, 0.000000f, 0.250000f, 0.250000f, 0.250000f, 0.500000f, 0.250000f, 
    0.750000f, 0.250000f, 1.000000f, 0.250000f, 0.000000f, 0.500000f, 0.250000f, 0.500000f, 
    0.500000f, 0.500000f, 0.750000f, 0.500000f, 1.000000f, 0.500000f, 0.000000f, 0.750000f, 
    0.250000f, 0.750000f, 0.500000f, 0.750000f, 0.750000f, 0.750000f, 1.000000f, 0.750000f, 
    0.000000f, 1.000000f, 0.250000f, 1.000000f, 0.500000f, 1.000000f, 0.750000f, 1.000000f, 
    1.000000f, 1.000000f
};

static void init(void* context)
{
    /* EGLNativeDisplayType attributes:  */
    eglnativedisplaytype_0_3c768 = createEGLNativeDisplayType1(context);
    /* EGLNativeWindowType attributes: x, y, width, height, mode */
    eglnativewindowtype_0_1a00002 = createEGLNativeWindowType6(context, 0, 0, 640, 480, 3);
    /* GLshadersourceline attributes: src */
    glshadersourceline_0_3ab8d4f0 = createGLshadersourceline2(context, "#ifndef GL_FRAGMENT_PRECISION_HIGH\n"
"#define highp mediump\n"
"#endif\n"
"\n");
    /* GLshadersourceline attributes: src */
    glshadersourceline_0_f098 = createGLshadersourceline2(context, "uniform highp mat4 u_modelview_projection_matrix;               \n"
"                                                                \n"
"attribute highp vec4 a_vertex;                                  \n"
"attribute mediump vec2 a_texcoord;                              \n"
"                                                                \n"
"varying mediump vec2 v_texcoord;                                \n"
"                                                                \n"
"void main(void)                                                 \n"
"{                                                               \n"
"    gl_Position = u_modelview_projection_matrix * a_vertex;     \n"
"    v_texcoord = a_texcoord;                                    \n"
"}                                                               \n"
"\n");
    /* GLshadersourceline attributes: src */
    glshadersourceline_0_f3a8 = createGLshadersourceline2(context, "uniform lowp sampler2D u_sampler;                               \n"
"                                                                \n"
"varying mediump vec2 v_texcoord;                                \n"
"                                                                \n"
"void main(void)                                                 \n"
"{                                                               \n"
"    lowp vec4 texel = texture2D(u_sampler, v_texcoord);         \n"
"                                                                \n"
"                                                                \n"
"    gl_FragColor = vec4(1.0, 1.0, 1.0, 0.75) * texel;           \n"
"}                                                               \n"
"\n");
    /* EGLConfig attributes: config_id, buffer_size, red_size, green_size, blue_size, alpha_size, bind_to_texture_rgb, bind_to_texture_rgba, config_caveat, depth_size, level, max_swap_interval, min_swap_interval, native_renderable, sample_buffers, samples, stencil_size, surface_type, transparent_type, transparent_red, transparent_green, transparent_blue, luminance_size, alpha_mask_size, color_buffer_type, conformant, renderable_type */
    eglconfig_0_c = createEGLConfig28(context, 12, 16, 5, 6, 5, 0, 1, 0, 12344, 24, 0, 1, 1, 1, 1, 4, 8, 486, 12344, 0, 0, 0, 0, 8, 12430, 4, 6);
}

static void uninit(void* context)
{
    destroyEGLNativeDisplayType2(context, eglnativedisplaytype_0_3c768);
    destroyEGLNativeWindowType2(context, eglnativewindowtype_0_1a00002);
    destroyGLshadersourceline2(context, glshadersourceline_0_3ab8d4f0);
    destroyGLshadersourceline2(context, glshadersourceline_0_f098);
    destroyGLshadersourceline2(context, glshadersourceline_0_f3a8);
    destroyEGLConfig2(context, eglconfig_0_c);
}

static void frame0(void* context)
{
    egldisplay_0_1 = eglGetDisplay(eglnativedisplaytype_0_3c768);
    eglInitialize(egldisplay_0_1, (EGLint *)0, (EGLint *)0);
    LOAD_ARRAY(char_array18, arrayData0, 435);
    eglQueryString(egldisplay_0_1, 12373);
    LOAD_ARRAY(char_array36, arrayData1, 18);
    eglGetProcAddress((const char *)char_array36);
    LOAD_ARRAY(char_array35, arrayData28, 19);
    eglGetProcAddress((const char *)char_array35);
    eglQueryString(egldisplay_0_1, 12373);
    LOAD_ARRAY(eglint_array34, arrayData26, 21);
    LOAD_ARRAY(eglint_array32, arrayData27, 1);
    eglChooseConfig(egldisplay_0_1, (const EGLint *)eglint_array34, (EGLConfig *)0, 0, (EGLint *)eglint_array32);
    LOAD_ARRAY(int_array33, arrayData22, 1);
    LOAD_ARRAY(eglint_array32, arrayData6, 1);
    eglChooseConfig(egldisplay_0_1, (const EGLint *)eglint_array34, (EGLConfig *)int_array33, 1, (EGLint *)eglint_array32);
    eglBindAPI(12448);
    LOAD_ARRAY(eglint_array31, arrayData24, 3);
    eglcontext_0_6faf8 = eglCreateContext(egldisplay_0_1, eglconfig_0_c, (EGLContext)NULL, (const EGLint *)eglint_array31);
    LOAD_ARRAY(eglint_array30, arrayData20, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12324, (EGLint *)eglint_array30);
    LOAD_ARRAY(eglint_array29, arrayData21, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12323, (EGLint *)eglint_array29);
    LOAD_ARRAY(eglint_array28, arrayData20, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12322, (EGLint *)eglint_array28);
    LOAD_ARRAY(eglint_array27, arrayData4, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12321, (EGLint *)eglint_array27);
    LOAD_ARRAY(eglint_array17, arrayData32, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12325, (EGLint *)eglint_array17);
    LOAD_ARRAY(eglint_array26, arrayData29, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12326, (EGLint *)eglint_array26);
    LOAD_ARRAY(eglint_array25, arrayData30, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12337, (EGLint *)eglint_array25);
    LOAD_ARRAY(eglint_array24, arrayData4, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12329, (EGLint *)eglint_array24);
    eglGetError();
    LOAD_ARRAY(eglint_array23, arrayData20, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12324, (EGLint *)eglint_array23);
    LOAD_ARRAY(eglint_array22, arrayData21, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12323, (EGLint *)eglint_array22);
    LOAD_ARRAY(eglint_array21, arrayData20, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12322, (EGLint *)eglint_array21);
    LOAD_ARRAY(eglint_array20, arrayData4, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12321, (EGLint *)eglint_array20);
    LOAD_ARRAY(eglint_array19, arrayData22, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12328, (EGLint *)eglint_array19);
    LOAD_ARRAY(eglint_array19, arrayData23, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12334, (EGLint *)eglint_array19);
    eglQueryString(egldisplay_0_1, 12373);
    LOAD_ARRAY(eglint_array17, arrayData4, 1);
    eglGetConfigAttrib(egldisplay_0_1, eglconfig_0_c, 12321, (EGLint *)eglint_array17);
    eglsurface_0_7af80 = eglCreateWindowSurface(egldisplay_0_1, eglconfig_0_c, eglnativewindowtype_0_1a00002, (const EGLint *)0);
    eglBindAPI(12448);
    eglMakeCurrent(egldisplay_0_1, eglsurface_0_7af80, eglsurface_0_7af80, eglcontext_0_6faf8);
    LOAD_ARRAY(glubyte_array16, arrayData5, 16);
    glGetString(GL_RENDERER);
    LOAD_ARRAY(char_array15, arrayData25, 22);
    eglQueryString(egldisplay_0_1, 12372);
    LOAD_ARRAY(gluint_array14, arrayData8, 1);
    glGenTextures(1, (GLuint *)gluint_array14);
    glBindTexture(GL_TEXTURE_2D, 70001);
    LOAD_ARRAY(char_array13, arrayData31, 64);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void *)char_array13);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 9728);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 9728);
    LOAD_ARRAY(char_array12, arrayData10, 22);
    eglGetProcAddress((const char *)char_array12);
    LOAD_ARRAY(char_array12, arrayData11, 19);
    eglGetProcAddress((const char *)char_array12);
    glshader_6faf8_11171 = glCreateShader(GL_VERTEX_SHADER);
    glshadersourceline_array11[0] = glshadersourceline_0_f098;
    LOAD_ARRAY(glint_array10, arrayData14, 1);
    glShaderSource(glshader_6faf8_11171, 1, (const char * *)glshadersourceline_array11, (const GLint *)glint_array10);
    glshadersourceline_0_f098 = glshadersourceline_array11[0];
    glCompileShader(glshader_6faf8_11171);
    LOAD_ARRAY(glint_array9, arrayData6, 1);
    glGetShaderiv(glshader_6faf8_11171, GL_COMPILE_STATUS, (GLint *)glint_array9);
    LOAD_ARRAY(glint_array9, arrayData7, 1);
    glGetShaderiv(glshader_6faf8_11171, GL_INFO_LOG_LENGTH, (GLint *)glint_array9);
    glshader_6faf8_222e2 = glCreateShader(GL_FRAGMENT_SHADER);
    glshadersourceline_array11[0] = glshadersourceline_0_3ab8d4f0;
    glshadersourceline_array11[1] = glshadersourceline_0_f3a8;
    LOAD_ARRAY(glint_array10, arrayData15, 2);
    glShaderSource(glshader_6faf8_222e2, 2, (const char * *)glshadersourceline_array11, (const GLint *)glint_array10);
    glshadersourceline_0_3ab8d4f0 = glshadersourceline_array11[0];
    glshadersourceline_0_f3a8 = glshadersourceline_array11[1];
    glCompileShader(glshader_6faf8_222e2);
    LOAD_ARRAY(glint_array9, arrayData6, 1);
    glGetShaderiv(glshader_6faf8_222e2, GL_COMPILE_STATUS, (GLint *)glint_array9);
    LOAD_ARRAY(glint_array9, arrayData7, 1);
    glGetShaderiv(glshader_6faf8_222e2, GL_INFO_LOG_LENGTH, (GLint *)glint_array9);
    glprogram_6faf8_33453 = glCreateProgram();
    glAttachShader(glprogram_6faf8_33453, glshader_6faf8_11171);
    glAttachShader(glprogram_6faf8_33453, glshader_6faf8_222e2);
    LOAD_ARRAY(char_array8, arrayData12, 9);
    glBindAttribLocation(glprogram_6faf8_33453, (GLuint)0, (const char *)char_array8);
    LOAD_ARRAY(char_array7, arrayData13, 11);
    glBindAttribLocation(glprogram_6faf8_33453, glattribute_33453_1, (const char *)char_array7);
    glLinkProgram(glprogram_6faf8_33453);
    LOAD_ARRAY(glint_array6, arrayData6, 1);
    glGetProgramiv(glprogram_6faf8_33453, GL_LINK_STATUS, (GLint *)glint_array6);
    LOAD_ARRAY(glint_array6, arrayData4, 1);
    glGetProgramiv(glprogram_6faf8_33453, GL_INFO_LOG_LENGTH, (GLint *)glint_array6);
    glUseProgram(glprogram_6faf8_33453);
    LOAD_ARRAY(char_array5, arrayData18, 30);
    gluniform_33453_1 = glGetUniformLocation(glprogram_6faf8_33453, (const char *)char_array5);
    LOAD_ARRAY(glfloat_array4, arrayData17, 16);
    glUniformMatrix4fv(gluniform_33453_1, 1, 0, (const GLfloat *)glfloat_array4);
    LOAD_ARRAY(char_array3, arrayData16, 10);
    gluniform_33453_2 = glGetUniformLocation(glprogram_6faf8_33453, (const char *)char_array3);
    glUniform1i(gluniform_33453_2, 0);
    glEnableVertexAttribArray((GLuint)0);
    glEnableVertexAttribArray(glattribute_33453_1);
    LOAD_ARRAY(float_array2, arrayData19, 75);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, 0, 0, (const void *)float_array2);
    LOAD_ARRAY(float_array1, arrayData33, 50);
    glVertexAttribPointer(glattribute_33453_1, 2, GL_FLOAT, 0, 0, (const void *)float_array1);
    glClearColor(0.0f, 0.0f, 0.75f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, 640, 480);
    glClear(16640);
    glBindTexture(GL_TEXTURE_2D, 70001);
    LOAD_ARRAY(short_array0, arrayData9, 96);
    glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_SHORT, (const void *)short_array0);
    eglSwapBuffers(egldisplay_0_1, eglsurface_0_7af80);
}

/**
 *  Frame #1
 */
static void frame1(void* context)
{
    glViewport(0, 0, 864, 480);
    glClear(16640);
    glBindTexture(GL_TEXTURE_2D, 70001);
    glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_SHORT, (const void *)short_array0);
    eglSwapBuffers(egldisplay_0_1, eglsurface_0_7af80);
}

/**
 *  Frame #2
 */
static void frame2(void* context)
{
    glClear(16640);
    glBindTexture(GL_TEXTURE_2D, 70001);
    glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_SHORT, (const void *)short_array0);
    eglSwapBuffers(egldisplay_0_1, eglsurface_0_7af80);
}

/**
 *  Frame #3
 */
static void frame3(void* context)
{
    glClear(16640);
    glBindTexture(GL_TEXTURE_2D, 70001);
    glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_SHORT, (const void *)short_array0);
    eglSwapBuffers(egldisplay_0_1, eglsurface_0_7af80);
}

/**
 *  Frame #4
 */
static void frame4(void* context)
{
    glFinish();
    eglBindAPI(12448);
    eglMakeCurrent(egldisplay_0_1, (EGLSurface)NULL, (EGLSurface)NULL, (EGLContext)NULL);
    eglBindAPI(12448);
    eglMakeCurrent(egldisplay_0_1, (EGLSurface)NULL, (EGLSurface)NULL, (EGLContext)NULL);
    eglDestroySurface(egldisplay_0_1, eglsurface_0_7af80);
    eglDestroyContext(egldisplay_0_1, eglcontext_0_6faf8);
    eglTerminate(egldisplay_0_1);
}

/**
 *  Play back all trace frames.
 *  @param context Optional user data pointer
 */
static void play(void* context)
{
    init(context);
    frame0(context);
    frame1(context);
    frame2(context);
    frame3(context);
    frame4(context);
    uninit(context);
}

/**
 *  Play back a single frame of the trace.
 *  @param context Optional user data pointer
 *  @param frame Zero-based number of frame to play
 *  @returns 1 if the frame number was valid, 0 otherwise
 */
static int playFrame(void* context, int frame)
{
    switch (frame)
    {
       case      0: init(context); break;
       case      1: frame0(context); break;
       case      2: frame1(context); break;
       case      3: frame2(context); break;
       case      4: frame3(context); break;
       case      5: frame4(context); break;
       case      6: uninit(context); break;
       default: return 0;
    }
    return 1;
}
