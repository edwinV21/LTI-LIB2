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
 * \file   ltiSymmetricMatrixDeterminant_template.h
 * \author Thomas Rusert
 * \author Gustavo Quiros
 * \date   02.06.99
 * 
 * $Id: ltiSymmetricMatrixDeterminant_template.h,v 1.8 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiMath.h"
#include <limits>

#include "ltiCholeskyDecomposition.h"
#include "ltiIncompatibleDimensionsException.h"

namespace lti {
  
  /*
   * Constructor
   */
  template<typename T>
  symmetricMatrixDeterminant<T>::symmetricMatrixDeterminant() 
    : linearAlgebraFunctor() {
  }

  /*
   * Destructor
   */
  template<typename T>
  symmetricMatrixDeterminant<T>::~symmetricMatrixDeterminant() {
  }

  /**
   * Returns a pointer to a clone of the functor.
   */
  template<typename T>
  symmetricMatrixDeterminant<T>* symmetricMatrixDeterminant<T>::clone() const {
    return new symmetricMatrixDeterminant<T>(*this);
  }

  // name
  template<typename T>
  const std::string& symmetricMatrixDeterminant<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /**
   * Returns a pointer to a new instance of the functor.
   */
  template<typename T>
  symmetricMatrixDeterminant<T>* 
  symmetricMatrixDeterminant<T>::newInstance() const {
    return new symmetricMatrixDeterminant<T>();
  }

  /**
   * apply, compute the determinant using Cholesky decomposition if possible,
   * or LU decomposition otherwise
   */
  template<typename T>
  bool symmetricMatrixDeterminant<T>::apply(const matrix<T>& theMatrix,
                                            T& det) const {
    if (theMatrix.rows() != theMatrix.columns()) {
      throw incompatibleDimensionsException();
    }
    matrix<T> u = theMatrix;
    choleskyDecomposition<T> chd;
    if (chd.apply(theMatrix,u)) {
      det = T(1);
      for (int j = 0; j < u.rows(); ++j) {
        const T& tmp=u.at(j,j);
	det *= tmp*tmp;
      }
      return true;
    }
    else {
      setStatusString("Error in choleskyDecomposition::apply in symmetricMatrixDeterminant:\n");
      appendStatusString(chd);
      return false;
    }
  }
  
  /**
   * Copy data of "other" functor.
   */
  template<typename T>
  symmetricMatrixDeterminant<T>& 
  symmetricMatrixDeterminant<T>
  ::copy(const symmetricMatrixDeterminant<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }
}
