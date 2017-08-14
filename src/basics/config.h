/* ../src/basics/config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
/* #undef F77_DUMMY_MAIN */

/* Define to a macro mangling the given C identifier (in lower and upper
   case), which must not contain underscores, for linking with Fortran. */
#define F77_FUNC(name,NAME) name ## _

/* As F77_FUNC, but for C identifiers containing underscores. */
#define F77_FUNC_(name,NAME) name ## _

/* Define if F77 and FC dummy `main' functions are identical. */
/* #undef FC_DUMMY_MAIN_EQ_F77 */

/* define if processor has AMD 3DNow */
/* #undef HAVE_3DNOW */

/* define if processor has Altivec */
/* #undef HAVE_ALTIVEC */

/* Define if you have a BLAS library. */
#define HAVE_BLAS 1

/* Define to 1 if you have the `finite' function. */
#define HAVE_FINITE 1

/* define if the compiler supports GCC C++ ABI name demangling */
#define HAVE_GCC_ABI_DEMANGLE 1

/* defines if gtk libraries were found */
#define HAVE_GTK 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isinf' function. */
#define HAVE_ISINF 1

/* Define to 1 if you have the `isnan' function. */
#define HAVE_ISNAN 1

/* Define if you have LAPACK library. */
#define HAVE_LAPACK 1

/* Define to 1 if you have the `dc1394' library (-ldc1394). */
#define HAVE_LIBDC1394 1

/* Define to 1 if you have the `dc1394_control' library (-ldc1394_control). */
/* #undef HAVE_LIBDC1394_CONTROL */

/* Define to 1 if you have the `fftw3f' library (-lfftw3f). */
/* #undef HAVE_LIBFFTW3F */

/* Define to 1 if you have the `jpeg' library (-ljpeg). */
#define HAVE_LIBJPEG 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `png' library (-lpng). */
#define HAVE_LIBPNG 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
#define HAVE_LIBPTHREAD 1

/* Define to 1 if you have the `raw1394' library (-lraw1394). */
#define HAVE_LIBRAW1394 1

/* Define to 1 if you have the `v4l2' library (-lv4l2). */
#define HAVE_LIBV4L2 1

/* Define to 1 if you have the <libv4l2.h> header file. */
#define HAVE_LIBV4L2_H 1

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <linux/videodev2.h> header file. */
#define HAVE_LINUX_VIDEODEV2_H 1

/* defines if the local implementation for png loader/savers was found */
/* #undef HAVE_LOCALJPEG */

/* defines if the local implementation for png loader/savers was found */
/* #undef HAVE_LOCALPNG */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* define if the processor has intel mmx */
#define HAVE_MMX 1

/* define if the compiler implements namespaces */
#define HAVE_NAMESPACES /**/

/* Define if OpenMP is enabled */
#define HAVE_OPENMP 1

/* define if the unix/linux system has a /proc directory structure */
#define HAVE_PROC 1

/* Define to 1 if you have the <pwc-ioctl.h> header file. */
/* #undef HAVE_PWC_IOCTL_H */

/* define if processor has intel SSE */
#define HAVE_SSE 1

/* define if processor has intel SSE2 */
#define HAVE_SSE2 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* define if the compiler supports Standard Template Library */
#define HAVE_STL /**/

/* define if the std::streambuf has member fd() */
/* #undef HAVE_STREAMBUF_FD */

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* defines if the susan algorithms are present in the library */
/* #undef HAVE_SUSAN */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* define if the Standard Template Library supports tr1 <random> */
#define HAVE_TR1_RANDOM /**/

/* define if the type __u64 is available for V4L */
#define HAVE_U64_V4L /**/

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `_finite' function. */
/* #undef HAVE__FINITE */

/* Define to 1 if you have the `_isinf' function. */
/* #undef HAVE__ISINF */

/* Define to 1 if you have the `_isnan' function. */
/* #undef HAVE__ISNAN */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://ltilib.sourceforge.net/"

/* Define to the full name of this package. */
#define PACKAGE_NAME "LTI-Lib-2"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "LTI-Lib-2 2.0.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "lti-lib-2"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.0.0"

/* define processor type */
#define PROCESSOR Intel(R) Core(TM) i5-3210M CPU @ 2.50GHz

/* define processor vendor */
#define PROCESSOR_VENDOR GenuineIntel

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* define type for byte */
#define _LTI_BYTE char

/* define type equivalent to integer in f2c.h */
#define _LTI_F2C_INTEGER int

/* define type for two-bytes */
#define _LTI_WORD16 short int

/* define type for four-bytes */
#define _LTI_WORD32 int

/* define type for eight-bytes */
#define _LTI_WORD64 long int
