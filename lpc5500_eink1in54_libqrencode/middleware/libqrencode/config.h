/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __QRENCODE_CONFIG_H__
#define __QRENCODE_CONFIG_H__

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 0

/* Major version number */
#define MAJOR_VERSION 3

/* Micro version number */
#define MICRO_VERSION 0

/* Minor version number */
#define MINOR_VERSION 9

/* Define to the version of this package. */
#define VERSION "0.1.0"
#define PACKAGE_VERSION "3.9.0"

/* Version number of package */

//#define VERSION "3.9.0"
#define LIBQRENCODE_VERSION "3.9.0"

#define STATIC_IN_RELEASE

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */
/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name. */

#ifndef __cplusplus

/* #undef inline */
#endif

/* Define to 'static' if no test programs will be compiled. */
#define __STATIC

#define WITH_TESTS 1

#endif

// end file __QRENCODE_CONFIG_H__