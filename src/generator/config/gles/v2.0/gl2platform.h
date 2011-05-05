#ifndef __gl2platform_h_
#define __gl2platform_h_

/*
 * Portions Copyright (c) Symbian Software Ltd 2008.  All rights reserved.
 * Differences from the actual sample implementation provided by Khronos:
 * 1. This comment text.
 * 2. Doxygen comment with tag 'publishedPartner', and tag 'released'
 * 3. Modified GL_APICALL.  The symbian specific check has been moved to the
 *    top because with a clean epoc/build/* starting point, makmake.pl wincsw
 *    would pick up the Win32 defines section.  For vendor implementations,
 *    use in your .MMP file
 *    MACRO SYMBIAN_OGLES_DLL_EXPORTS.
 *    The GCC32 case has been removed because this does not fit with the symbian
 *    build system which uses GCCE instead.
 * 4. A vendor implementation is allowed to overwrite/modify this file;
 *    see Khronos API Implementers' Guidelines, Revision 0.91.0.
 */

/**
@publishedPartner
@released
*/

/* $Id: gl2platform.h,v 1.1 2009-03-27 10:17:53 skyostil Exp $ */

#ifdef __cplusplus
extern "C" {
#endif

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.0 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
**
** http://oss.sgi.com/projects/FreeB
**
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
**
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
**
** Additional Notice Provisions: The application programming interfaces
** established by SGI in conjunction with the Original Code are The
** OpenGL(R) Graphics System: A Specification (Version 1.2.1), released
** April 1, 1999; The OpenGL(R) Graphics System Utility Library (Version
** 1.3), released November 4, 1998; and OpenGL(R) Graphics with the X
** Window System(R) (Version 1.3), released October 19, 1998. This software
** was created using the OpenGL(R) version 1.2.1 Sample Implementation
** published by SGI, but has not been independently verified as being
** compliant with the OpenGL(R) version 1.2.1 Specification.
*/

/* Note: modified for Tracy */
#define GL_APICALL
#define GL_APIENTRY
#define __SOFTFP

#if 0
/*
 * If the pre-included toolchain header does not specify the __SOFTFP macro,
 * this include defines it.
 */
#include <e32def.h>

/*-------------------------------------------------------------------------
 * Definition of GL_APICALL and GL_APIENTRY
 *-----------------------------------------------------------------------*/

#if defined (__SYMBIAN32__)
#   if defined (SYMBIAN_OGLES_DLL_EXPORTS)
#       define GL_APICALL __declspec(dllexport)
#   else
#       define GL_APICALL __declspec(dllimport)
#   endif
#elif defined(_WIN32) || defined(__VC32__)       /* Win32 */
#   if defined (_DLL_EXPORTS)
#       define GL_APICALL __declspec(dllexport)
#   else
#       define GL_APICALL __declspec(dllimport)
#   endif
#elif defined (__ARMCC_VERSION)                  /* ADS */
#   define GL_APICALL
#elif defined (__GNUC__)                         /* GCC dependencies (kludge) */
#   define GL_APICALL
#endif

#if !defined (GL_APICALL)
#   error Unsupported platform!
#endif

#define GL_APIENTRY

#ifdef __cplusplus
}
#endif
#endif /* 0 */

#endif /* __gl2platform_h_ */
