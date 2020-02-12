/* config.h.in.  Generated from configure.ac by autoheader.  */

/* If you use configure, this file provides #defines reflecting your
   configuration choices.  If you have not run configure, suitable
   conservative defaults will be used.

   Autoheader adds a number of items to this template file that are not
   used by HTSlib: STDC_HEADERS and most HAVE_*_H header file defines
   are immaterial, as we assume standard ISO C headers and facilities;
   the PACKAGE_* defines are unused and are overridden by the more
   accurate PACKAGE_VERSION as computed by the Makefile.  */

/* Define if HTSlib should enable plugins. */
#cmakedefine ENABLE_PLUGINS

/* Define if you have the Common Crypto library. */
#cmakedefine HAVE_COMMONCRYPTO

/* Define to 1 if you have the `fdatasync' function. */
#cmakedefine01 HAVE_FDATASYNC

/* Define to 1 if you have the `getpagesize' function. */
#cmakedefine01 HAVE_GETPAGESIZE

/* Define to 1 if you have the `gmtime_r' function. */
#cmakedefine01 HAVE_GMTIME_R

/* Define if you have libcrypto-style HMAC(). */
#cmakedefine HAVE_HMAC

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine01 HAVE_INTTYPES_H

/* Define if libcurl file access is enabled. */
#cmakedefine HAVE_LIBCURL

/* Define if you have the `deflate' library (-ldeflate). */
#cmakedefine HAVE_LIBDEFLATE

/* Define if you have the `z' library (-lz). */
#cmakedefine HAVE_LIBZ

/* Define to 1 if you have the <memory.h> header file. */
#cmakedefine01 HAVE_MEMORY_H

/* Define to 1 if you have a working `mmap' system call. */
#cmakedefine01 HAVE_MMAP

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine01 HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#cmakedefine01 HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#cmakedefine01 HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#cmakedefine01 HAVE_STRING_H

/* Define to 1 if you have the <sys/param.h> header file. */
#cmakedefine01 HAVE_SYS_PARAM_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#cmakedefine01 HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#cmakedefine01 HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#cmakedefine01 HAVE_UNISTD_H

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "j.park@dkfz-heidelberg.de"

/* Define to the full name of this package. */
#define PACKAGE_NAME "HTSlib-js"

/* Define to the full name and version of this package. */
#cmakedefine PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "htslib-js"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://eilslabs-phabricator.dkfz.de/source/htslib-js/"

/* Define to the version of this package. */
#cmakedefine PACKAGE_VERSION

/* Platform-dependent plugin filename extension. */
#cmakedefine PLUGIN_EXT "${PLUGIN_EXT}"

/* Define to 1 if you have the ANSI C header files. */
#cmakedefine STDC_HEADERS

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
#cmakedefine _FILE_OFFSET_BITS @_FILE_OFFSET_BITS@

/* Define for large files, on AIX-style hosts. */
#cmakedefine _LARGE_FILES
