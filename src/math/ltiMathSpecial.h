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
 * \file   ltiMathSpecial.h
 *         Definition of some global functions like factorial, gamma and the
 *         binomial coefficients.  In the LTI-Lib 2 they have been packed into
 *         an extra file outside math, since this functions are seldom used.
 *         The name of the file "special" was chosen in accordance to
 *         Numerical Recipes
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   28.04.1999
 *
 * $Id: ltiMathSpecial.h,v 1.1 2004-07-26 19:20:34 alvarado Exp $
 */

#ifndef _LTI_MATH_SPECIAL_H_
#define _LTI_MATH_SPECIAL_H_

#include "ltiObject.h"
#include "ltiMath.h" // for log() and NaN

namespace lti {

  /**
   * Returns the value ln( gamma(x) ) for xx>0
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T> 
  T lnGamma(const T& x);

  /**
   * Returns the factorial of x 
   *
   * Note that the returned type is always double, no matter what you use as
   * input type.
   *
   * @ingroup gBasicMathFunc
   */
  double factorial(const int x);

  /**
   * Returns the natural logarithm of the factorial of x 
   *
   * Note that the returned type is always double, no matter what you use as
   * input type.
   *
   * @ingroup gBasicMathFunc
   */
  double lnFactorial(const int x);

  /**
   * Returns the binomial coefficient defined by
   * \f[ \begin{pmatrix} n \\ k \end{pmatrix} \frac{n!}{k!(n-k)!} 
   * \quad 0 \leq k \leq n \f]
   *
   * @ingroup gBasicMathFunc
   */
  double binomial(const int n,const int k);
}


#include "ltiMathSpecial_template.h"

#endif
