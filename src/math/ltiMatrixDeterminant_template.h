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

/*
 * \file   ltiMatrixDeterminant_template.h
 *         Compute the determinant of a matrix
 * \author Thomas Rusert
 * \author Gustavo Quiros
 * \date   23.06.1999
 *
 * $Id: ltiMatrixDeterminant_template.h,v 1.10 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiLuDecomposition.h"
#include "ltiIncompatibleDimensionsException.h"

namespace lti {
  
  /**
   * Constructor
   */
  template<typename T>
  matrixDeterminant<T>::matrixDeterminant() {
  }

  /**
   * Destructor
   */
  template<typename T>
  matrixDeterminant<T>::~matrixDeterminant() {
  }

  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& matrixDeterminant<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /**
   * Returns a pointer to a clone of the functor.
   */
  template<typename T>
  matrixDeterminant<T>* matrixDeterminant<T>::clone() const {
    return (new matrixDeterminant<T>(*this));
  }
  
  /**
   * Returns a pointer to a clone of the functor.
   */
  template<typename T>
  matrixDeterminant<T>* matrixDeterminant<T>::newInstance() const {
    return (new matrixDeterminant<T>());
  }

  /**
   * apply, compute the determinant using LU decomposition
   */
  template<typename T>
  bool matrixDeterminant<T>::apply(const matrix<T>& theMatrix,
                                   T& det) const {
    if (theMatrix.rows() != theMatrix.columns()) {
      throw incompatibleDimensionsException();
    }
    matrix<T> lu = theMatrix;
    vector<integer> perm(theMatrix.rows());
    typename luDecomposition<T>::parameters ludParam;
    ludParam.useLapack=getParameters().useLapack;
    luDecomposition<T> lud(ludParam);
    int pivot;
    if (lud.apply(lu,perm,pivot)) {
      det = T(pivot);
      for (int j = 0; j < lu.rows(); ++j) {
	det *= lu.at(j,j);
      }
      return true;
    }
    else {
      det = T(0);
      appendStatusString(lud);
      return false;
    }
  }

  /**
   * Copy data of "other" functor.
   */
  template<typename T>
  matrixDeterminant<T>& matrixDeterminant<T>::copy(const matrixDeterminant<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }
}
