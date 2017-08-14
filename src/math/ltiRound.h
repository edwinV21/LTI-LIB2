/*
 * Copyright (C) 1998-2004
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
 * \file   ltiRound.h
 *         Definition of some usually used global functions for
 *         mathematical rounding (round, iround, lround and cround_cast)
 * \author Pablo Alvarado
 * \date   28.04.1999
 *
 * $Id: ltiRound.h,v 1.3 2004-07-27 08:22:42 alvarado Exp $
 */

#ifndef _LTI_ROUND_H_
#define _LTI_ROUND_H_

namespace lti {

  /**
   * Round any float type mathematicly
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T round(T x);

  /**
   * Round any float type mathematicly and return an integer
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline int iround(T x);

  /**
   * Round any float type mathematicly and return an long integer
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline long lround(T x);

  /**
   * Conditional round cast.
   *
   * This template functions cast the given value using rounding if necessary.
   * This means, if you cast between different floating point types, no
   * integer rounding will be done, but if you cast from a floating point
   * value to an integer value, the result will be rounded.  For example:
   *
   * \code
   * int i   = cround_cast<int>(3.6); // i will contain 4
   * float f = cround_cast<float>(3.6); // i will contain 3.6f
   * \endcode
   *
   * This sort of casting is used in the implementation of several template
   * functions where the output type depends on the current instantiation.
   */
  template<typename T,typename F>
  inline T cround_cast(const F from);

}

#include "ltiRound_inline.h"

#endif
