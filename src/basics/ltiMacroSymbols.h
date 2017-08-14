/*
 * Copyright (C) 1999-2007
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * \file   ltiMacroSymbols.h 
 *         Contains several definitions of macros to determine which compiler
 *         is being used.  Since this file is included by ltiObject.h, their
 *         contents should be defined in every LTI-Lib file.
 * \author Pablo Alvarado
 * \date   01.04.99
 *
 * $Id: ltiMacroSymbols.h,v 1.9 2008-10-15 23:21:33 alvarado Exp $
 */

#ifndef _LTI_MACRO_SYMBOLS_H_
#define _LTI_MACRO_SYMBOLS_H_

#if defined(_WIN32) 
/**
 * \def _LTI_WIN32
 * Macro to recognize if the Windows 32-bit library is available
 */
#define _LTI_WIN32 _WIN32
#elif defined(WIN32) 
/**
 * \def _LTI_WIN32
 * Macro to recognize if the Windows 32-bit library is available
 */
#define _LTI_WIN32 WIN32
#endif

#if defined(_MSC_VER) 
/**
 * \def _LTI_MSC_VER
 * Macro to recognize if the MS Visual C++ compiler 6.0 or higher
 * (including the .NET family) is being used.
 */
#define _LTI_MSC_VER _MSC_VER
#elif defined(MSC_VER)
/**
 * \def _LTI_MSC_VER
 * Macro to recognize if the MS Visual C++ compiler 6.0 or higher
 * (including the .NET family) is being used.
 */
#define _LTI_MSC_VER MSC_VER
#endif

#if defined(_LTI_MSC_VER) && (_LTI_MSC_VER >= 1310)
/**
 * \def _LTI_MSC_DOT_NET
 * Macro to recognize if the MS Visual C++ compiler .NET 2003 is being used.
 */
#define _LTI_MSC_DOT_NET _LTI_MSC_VER
#endif

#if defined(_LTI_MSC_VER) && (_LTI_MSC_VER < 1310)
/**
 * \def _LTI_MSC_6
 * Macro to recognize if a previous version of MS Visual C++ compiler
 * .NET 2003 is being used.
 *
 * Note that this older version of the Visual C++ compiler is \b NOT supported
 * by the LTI-Lib, since it lacks many standard C++ issues with templates and
 * enclosed classes.
 */
#define _LTI_MSC_6 _LTI_MSC_VER
#endif


#if defined(_LTI_MSC_DOT_NET) && (_LTI_MSC_VER < 1400)
/**
 * \def _LTI_MSC_DOT_NET_2003
 * Macro to recognize if the MS Visual C++ compiler .NET 2003 is being
 * used.
 */
#define _LTI_MSC_DOT_NET_2003 _LTI_MSC_VER
#endif

#if defined(_LTI_MSC_DOT_NET) && (_LTI_MSC_VER >= 1400)
/**
 * \def _LTI_MSC_DOT_NET_2005
 * Macro to recognize if the MS Visual C++ compiler .NET 2005 is being
 * used.
 */
#define _LTI_MSC_DOT_NET_2005 _LTI_MSC_VER
#endif


#if defined(__GNUC__) 
/**
 * \def _LTI_GNUC
 * Macro to recognize if a GNU C++ compiler is being used
 */
#define _LTI_GNUC __GNUC__
#endif

#if defined(_LTI_GNUC) && (_LTI_GNUC <= 2)
/**
 * \def _LTI_GNUC_2
 * Macro to recognize if a GNU C++ compiler 2.95 is being used
 * 
 * Note that this older version of the GCC C++ compiler is not supported
 * anymore, since it lacks some standard issues in the STL and with the
 * keyword "using".
 */
#define _LTI_GNUC_2 _LTI_GNUC
#endif

#if defined(_LTI_GNUC) && (_LTI_GNUC >= 3)
/**
 * \def _LTI_GNUC_3
 * Macro to recognize if a GNU C++ compiler 3.0 or greater is being used
 */
#define _LTI_GNUC_3 _LTI_GNUC
#endif

#if defined(_LTI_GNUC) && (_LTI_GNUC >= 3) && (__GNUC_MINOR__ >= 4)
/**
 * \def _LTI_GNUC_3_4
 * Macro to recognize if a GNU C++ compiler 3.4.0 or greater is being used
 */
#define _LTI_GNUC_3_4 _LTI_GNUC
#endif

#if defined(_LTI_GNUC) && (_LTI_GNUC >= 4)
/**
 * \def _LTI_GNUC_4
 * Macro to recognize if a GNU C++ compiler 4.0.0 or greater is being used
 */
#define _LTI_GNUC_4 _LTI_GNUC
#endif

#if defined(_LTI_GNUC) && (_LTI_GNUC >= 4) && (__GNUC_MINOR__ >= 2)
/**
 * \def _LTI_GNUC_4_2
 * Macro to recognize if a GNU C++ compiler 4.2.x or greater is being used
 */
#define _LTI_GNUC_4_2 _LTI_GNUC
#endif

#if defined(_LTI_GNUC) && (_LTI_GNUC >= 4) && (__GNUC_MINOR__ >= 3)
/**
 * \def _LTI_GNUC_4_3
 * Macro to recognize if a GNU C++ compiler 4.3.x or greater is being used
 */
#define _LTI_GNUC_4_3 _LTI_GNUC
#endif


#if defined(__INTEL_COMPILER)
/**
 * \def _LTI_ICC 
 *
 * Macro to recognize if an Intel compiler is being used. Note that
 * _LTI_GNUC and some _LTI_GNUC_x are also defined in this case
 * (unless switched off manually). This is quite useful is most cases
 * since gcc and icc behave mostly equivalently.
 */
#define _LTI_ICC __INTEL_COMPILER
#endif

#if defined(__linux__) 
/**
 * \def _LTI_LINUX
 * Macro to recognize if a Linux system is being used
 */ 
#define _LTI_LINUX
#endif

#if defined(__CYGWIN__) || defined(__CYGWIN32) || defined(__MINGW32__)
/**
 * \def _LTI_CYGWIN
 * Macro to recognize if a CygWin system is being used
 */
#define _LTI_CYGWIN
#endif

#if defined(__APPLE__)
/**
 * \def _LTI_MACOSX
 * Macro to recognize if an Apple system is used
 */
#define _LTI_MACOSX
#endif

#endif
