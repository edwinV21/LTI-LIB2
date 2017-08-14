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
 * \file   ltiMathLA.h
 *         Definition of some useful global functions which are often needed
 *         in Linear Algebra Algorithms. These include:
 *         - householder: calculates householder vector and beta
 *         - givens: calculates cos and sin values for Givens rotations
 * \author Peter Doerfler
 * \date   27.04.2004
 *
 * $Id: ltiMathLA.h,v 1.4 2005-01-21 17:05:36 alvarado Exp $
 */

#ifndef _LTI_MATH_LA_H_
#define _LTI_MATH_LA_H_

#include "ltiVector.h"


namespace lti {

  /**
   * Calculates the Householder vector \c v and the factor \c beta for
   * a %vector \c x (given as input in \c v).
   *
   * The resulting n-dimensional %vector \c v has the following properties:
   * - v[0] = 1,
   * - \f$ P = I_n - \beta v v^T\f$ is orthogonal, and
   * - /f$ Px=||x||_2e_1
   * , where I_n is the n-by-n identity matrix and e_1 the first
   * canonical vector.
   *
   * For more details see:<br>
   * Gene H. Golub and Charles F. Van Loan, "Matrix Computations",
   * 1996, The John Hopkins University Press, Baltimore and London
   *
   * @param v input: vector \c x; output: householder vector \c v
   * @param beta factor needed for householder transform
   *
   * @ingroup gUnassigned
   */
  template<typename T>
  inline void householder(vector<T>& v, T& beta);

  /**
   * Calculates the Householder vector \c v and the factor \c beta for
   * a %vector \c src. 
   *
   * The resulting n-dimensional %vector \c v has the following properties:
   * - v[0] = 1,
   * - \f$ P = I_n - \beta v v^T\f$ is orthogonal, and
   * - /f$ Px=||x||_2e_1
   * , where I_n is the n-by-n identity matrix and e_1 the first
   * canonical vector.
   *
   * \b Note: The on-place version of householder(vector<T>&, T&) is
   * faster since the input vector is not copied first.
   *
   * For more details see:<br>
   * Gene H. Golub and Charles F. Van Loan, "Matrix Computations",
   * 1996, The John Hopkins University Press, Baltimore and London
   *
   * @param src vector \c x used to form the householder vector
   * @param v householder vector \c v
   * @param beta factor needed for householder transform
   *
   * @ingroup gUnassigned
   */
  template<typename T>
  inline void householder(const vector<T>& src, vector<T>& v, T& beta);

  /**
   * Calculates the cos (\c c) and sin (\c s) values needed for Givens
   * Rotations.
   *
   * The values \c c and \c s have the following property:
   * \f[
   * \begin{bmatrix} c & s \\ -s & c \end{bmatrix}^T
   * \begin{bmatrix} a \\ b \end{bmatrix} =
   * \begin{bmatrix} r \\ 0 \end{bmatrix}.
   * \f]
   *
   * For more details see:<br>
   * Gene H. Golub and Charles F. Van Loan, "Matrix Computations",
   * 1996, The John Hopkins University Press, Baltimore and London
   *
   * @param a first scalar
   * @param b second scalar
   * @param c cos value for Givens Rotation
   * @param s sin value for Givens Rotation
   *
   * @ingroup gUnassigned
   */
  template<typename T>
  inline void givens(const T& a, const T& b, T& c, T& s);

}

#include "ltiMathLA_inline.h"

#endif
