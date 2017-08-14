/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiDebug.h
 *         Defines many macros used when debugging LTI-Lib classes.
 *         The main macros are _lti_debug* and _lti_if_debug*,
 *         which are active only if the macro symbol _LTI_DEBUG is high enough.
 * \author Jochen Wickel
 * \author Pablo Alvarado
 * \date   24.02.2003
 *
 * $Id: ltiDebug.h,v 1.3 2005-07-09 04:20:13 alvarado Exp $
 */

#undef _lti_debug
#undef _lti_debug1
#undef _lti_debug2
#undef _lti_debug3
#undef _lti_debug4

#undef _lti_if_debug
#undef _lti_if_debug1
#undef _lti_if_debug2
#undef _lti_if_debug3
#undef _lti_if_debug4

#undef _lti_enterCTOR
#undef _lti_leaveCTOR
#undef _lti_enterDTOR
#undef _lti_leaveDTOR

#undef _lti_showVar

/**
 * \name Debugging Macros
 *
 * Developers of the LTI-Lib use following macros to activate debugging
 * code in their implementations.
 *
 * To activate them, the macro _LTI_DEBUG must be defined before including 
 * this file.  The value of _LTI_DEBUG must be an integer value between 1 and 4
 * Debug level i+1 includes all information of level i.  This means, you
 * should use the macros _lti_debug and _lti_debug1 to display critical 
 * information, that must always be displayed when debugging is activated and
 * less critical information with higher levels.
 *
 * Don't forget to include the "ltiUndebug.h" file at the end of your source
 * code, to undefine all the debug macros defined here, especially if it is
 * a header file (e.g. _template.h, _inline.h).
 *
 * The typical use of this file is:
 * \code
 * // somewhere at the beginning of your source file
 * #undef _LTI_DEBUG
 * #define _LTI_DEBUG 1 // comment out this line to remove all debug info.
 * #include "ltiDebug.h"
 *
 * // somewhere in your code you can use 
 * _lti_debug("I've been here with value" << val);
 * 
 * _lti_debug4("Only in level 4 this will be displayed");
 *
 * // at the end of your file you MUST undefine every debugging macro, to
 * // avoid confusion if someone includes your file!  Of course this is
 * // not necessary if your file is a .cpp file.  This is important for
 * // _template.h and _inline.h files.
 * #include "ltiUndebug.h"
 * \endcode
 */
//@{
#ifdef _LTI_DEBUG
#  include <iostream>

  /**
   * \def _lti_debug
   * Debug Information Level 0 (always displayed)
   */
#  define _lti_debug(a) std::cerr << a
#  define _lti_if_debug(a) a

  /**
   * \def _lti_debug1
   * Debug Information Level 1 (always displayed)
   */
#  define _lti_debug1(a) std::cerr << a
#  define _lti_if_debug1(a) a

#  if _LTI_DEBUG > 1
#    include "ltiClassName.h"
     /**
      * \def _lti_debug2
      * Debug Information Level 2 
      */
#    define _lti_debug2(a) std::cerr << a
#    define _lti_if_debug2(a) a

#    if _LTI_DEBUG > 2
       /**
        * \def _lti_debug3
        * Debug Information Level 3
        */
#      define _lti_debug3(a) std::cerr << a
#      define _lti_if_debug3(a) a

#      if _LTI_DEBUG > 3
         /**
          * \def _lti_debug4
          * Debug Information Level 4
          */
#        define _lti_debug4(a) std::cerr << a
#        define _lti_if_debug4(a) a

#      else
         /**
          * \def _lti_debug4
          * Debug Information Level 4
          */
#        define _lti_debug4(a)
#        define _lti_if_debug4(a)

#      endif
#    else
       /**
        * \def _lti_debug3
        * Debug Information Level 3
        */
#      define _lti_debug3(a)
#      define _lti_if_debug3(a)

#    endif
#  else
    /**
     * \def _lti_debug2
     * Debug Information Level 2
     */
#    define _lti_debug2(a)
#    define _lti_if_debug2(a)

#  endif
#endif

#ifndef _lti_debug
  /**
   * \def _lti_debug
   * Debug Information Level 0 (always displayed)
   */
#  define _lti_debug(a)
#  define _lti_if_debug(a)

#endif

#ifndef _lti_debug1
  /**
   * \def _lti_debug1
   * Debug Information Level 1 (always displayed)
   */
#  define _lti_debug1(a)
#  define _lti_if_debug1(a)
#endif


#ifndef _lti_debug2
  /**
   * \def _lti_debug2
   * Debug Information Level 2
   */
#  define _lti_debug2(a)
#  define _lti_if_debug2(a)
#endif

#ifndef _lti_debug3
  /**
   * \def _lti_debug3
   * Debug Information Level 3
   */
#  define _lti_debug3(a)
#  define _lti_if_debug3(a)
#endif

#ifndef _lti_debug4
  /**
   * \def _lti_debug4
   * Debug Information Level 4
   */
#  define _lti_debug4(a)
#  define _lti_if_debug4(a)
#endif

/**
 * \def _lti_enterCTOR
 * Debug macro to indicate "entering constructor".  It displays the class name
 * followed by the string "::ctor(".  It uses debug level 2.
 */
#define _lti_enterCTOR() \
  _lti_debug2(lti::className::demangle(typeid(*this).name()) << \
              "::ctor(" << std::endl)

/**
 * \def _lti_leaveCTOR
 * Debug macro to indicate "leaving constructor".  It displays the class name
 * followed by the string "::ctor)".  It uses debug level 2.
 */
#define _lti_leaveCTOR() \
  _lti_debug2(lti::className::demangle(typeid(*this).name()) << \
              "::ctor)" << std::endl)

/**
 * \def _lti_enterDTOR
 * Debug macro to indicate "entering destructor".  It displays the class name
 * followed by the string "::dtor(".  It uses debug level 2.
 */
#define _lti_enterDTOR() \
  _lti_debug2(lti::className::demangle(typeid(*this).name()) << \
              "::dtor(" << std::endl)

/**
 * \def _lti_leaveDTOR
 * Debug macro to indicate "leaving destructor".  It displays the class name
 * followed by the string "::ctor)".  It uses debug level 2.
 */
#define _lti_leaveDTOR() \
  _lti_debug2(lti::className::demangle(typeid(*this).name()) << \
              "::dtor)" << std::endl)

/**
 * \def _lti_showVar
 * Debug macro to display at debug level 0 the value of a given variable.
 * It displays the name of the variable followed by " = " and the contents
 * of it.  The type of the variable must be supported by the std::cerr stream.
 */
#define _lti_showVar(a) _lti_debug(#a " = " << a << "\n")

//@}
