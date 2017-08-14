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
 * \file   ltiMatrixDeterminant.h
 *         Compute the determinant of a matrix
 * \author Thomas Rusert
 * \author Gustavo Quiros
 * \date   23.06.1999
 *
 * $Id: ltiMatrixDeterminant.h,v 1.5 2005-06-22 15:26:41 doerfler Exp $
 */

#ifndef _LTI_MATRIX_DETERMINANT_H_
#define _LTI_MATRIX_DETERMINANT_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

namespace lti {
  
  /**
   * Matrix determinant functor.
   * Computes the determinant of a square matrix using LU decomposition
   *
   * @ingroup gLinearAlgebra
   * @ingroup gLTILib2FormatRequired
   */
  template<typename T>
  class matrixDeterminant : public linearAlgebraFunctor {
    
  public:

    typedef linearAlgebraFunctor::parameters parameters;

    /**
     * Default constructor
     */
    matrixDeterminant();

    /** 
     * Destructor
     */
    virtual ~matrixDeterminant();

    /**
     * apply, compute the determinant using LU decomposition
     */
    virtual bool apply(const matrix<T>& theMatrix, T& det) const;

    /**
     * Copy data of "other" functor.
     */
    matrixDeterminant<T>& copy(const matrixDeterminant<T>& other);
    
    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual matrixDeterminant<T>* clone() const;
    
    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual matrixDeterminant<T>* newInstance() const;
  };
}

#endif
