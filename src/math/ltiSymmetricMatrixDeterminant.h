/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiSymmetricMatrixDeterminant.h
 *         Contains the class symmetricMatrixDeterminant which
 *         calculates the determinant of a symmetric matrix
 *         efficiently
 * \author Thomas Rusert
 * \author Gustavo Quiros
 * \date   23.06.1999
 *
 * $Id: ltiSymmetricMatrixDeterminant.h,v 1.6 2005-06-22 15:26:41 doerfler Exp $
 */

#ifndef _LTI_SYMMETRIC_MATRIX_DETERMINANT_H_
#define _LTI_SYMMETRIC_MATRIX_DETERMINANT_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

namespace lti {
  /**
   * Symmetric matrix determinant functor.
   * Computes the determinant of a symmetric square matrix using Cholesky
   * decomposition.
   *
   * @ingroup gLinearAlgebra
   */
  template<typename T>
  class symmetricMatrixDeterminant : public linearAlgebraFunctor {
    
  public:

    /**
     * default constructor
     */
    symmetricMatrixDeterminant();

    /** 
     * destructor
     */
    virtual ~symmetricMatrixDeterminant();

    /**
     * apply, compute the determinant of the symmetric matrix \a mat
     * using Cholesky decomposition.
     *
     * @param mat symmetric matrix with the data
     * @param det determiant of \a mat
     */
    virtual bool apply(const matrix<T>& mat, T& det) const;

    /**
     * Copy data of "other" functor.
     */
    symmetricMatrixDeterminant<T>& 
    copy(const symmetricMatrixDeterminant<T>& other);
    
    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual symmetricMatrixDeterminant<T>* clone() const;

    /**
     * Returns a pointer to a new instance of the functor.
     */
    virtual symmetricMatrixDeterminant<T>* newInstance() const;
    
  };
}

#endif
