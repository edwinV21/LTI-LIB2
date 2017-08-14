/*
 * Copyright (C) 1998-2005
 * Peter Doerfler
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
 * \file   ltiMathMatrix.h
 *         Contains some useful vector and matrix functions that are
 *         too special for the actual classes.
 * \author Peter Doerfler
 * \date   10.07.2005
 *
 * $Id: ltiMathMatrix.h,v 1.2 2005-07-23 10:22:16 doerfler Exp $
 */

#ifndef _LTI_MATH_MATRIX_H_
#define _LTI_MATH_MATRIX_H_

#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * Used for indication whether a given matrix should be transposed
   * before a calculation.
   */
  enum eTransposeType {
    Transpose, /**< transpose the given matrix. */
    Normal     /**< don't transpose the given matrix */
  };

  /**
   * Calculates the square of the given matrix:
   * \f[ D = S\cdot S^T \f]
   * If \a trans is lti::Transpose then:
   * \f[ D = S^T\cdot S \f]
   *
   * @param src the source matrix (S)
   * @param trans specifies whether \a src should be transposed
   * before calculation.
   * @param dest the destination matrix (D)
   */
  template <typename T>
  inline void squareMatrix(const matrix<T>& src, 
                           const eTransposeType trans,
                           matrix<T>& dest);
  

}

#include "ltiMathMatrix_inline.h"

#endif 
