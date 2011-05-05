/*
 * GL2EXTPLATFORM.H
 *
 * Copyright (c) 2008 Symbian Software Ltd.  All rights reserved.
 */

#ifndef __GL2EXTPLATFORM_H__
#define __GL2EXTPLATFORM_H__

/*
 * The purpose of this header file is to allow a platform to control which
 * gl2ext.h extensions are seen, in order to enforce source-level backwards
 * compatiblity.
 * Consider the example of disabling GL_DEPTH_COMPONENT24_OES.  From gl2ext.h
 * we see this is guarded by the define GL_OES_depth24.  You need to define
 * GL_OES_depth24 in this file in order to disable this extension from being
 * seen by programs compiled against gl2.h and gl2ext.h headers.
 */

/**
@publishedPartner
@released
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Place #define statements as needed below to disable gl2ext.h extensions */

#ifdef __cplusplus
}
#endif

#endif