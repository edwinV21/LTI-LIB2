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
 * \file   ltiLuSolution.h
 * \author Thomas Rusert
 * \date   02.06.1999
 *
 * $Id: ltiLuSolution.h,v 1.2 2006-09-07 13:38:37 doerfler Exp $
 */

#ifndef _LTI_LU_SOLUTION_H_
#define _LTI_LU_SOLUTION_H_

#include "ltiDecompositionSolution.h"

namespace lti {

  /**
   * LU solution functor.
   * Solves the linear equation Ax=b using LU decomposition.
   *
   * @ingroup gLTILib2FormatRequired
   */
  template<class T>
  class luSolution : public decompositionSolution<T> {
  public:
    typedef typename decompositionSolution<T>::parameters parameters;

    /**
     * default constructor
     */
    luSolution();

    /**
     * copy constructor
     */
    luSolution(const luSolution<T>& other);

    /**
     * constructor, sets the parameters
     * @see decompositionSolution::parameters
     */
    luSolution(const parameters& theParams);

    /**
     * constructor, sets the matrix A
     */
    luSolution(const matrix<T>& theMatrix);

    /**
     * onPlace version of apply.
     * Solves the set of n linear equations Ax=b. For use with multiple right
     * sides b of a set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.
     * After that the existing decomposition will be used until calling
     * setParameters.*/
    bool apply(vector<T>& b);

    /**
     * onCopy version of apply.
     * Solves the set of n linear equations Ax=b. For use with multiple right
     * sides b of a set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.
     * After that the existing decomposition will be used until calling
     * setParameters.
     */
    bool apply(const vector<T>& b,vector<T>& x);

    /**
     * onPlace version of apply.  Solves the set of n linear equations
     * A x=b where x is the i-th _column_ vector of X and b the i-th
     * _column_ vector of B. For use with multiple right sides b of a
     * set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.  After that the
     * existing decomposition will be used until calling
     * setParameters.
     */
    bool apply(matrix<T>& B);

    /**
     * onCopy version of apply.  Solves the set of n linear equations
     * A x=b where x is the i-th _column_ vector of X and b the i-th
     * _column_ vector of B. For use with multiple right sides b of a
     * set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.  After that the
     * existing decomposition will be used until calling
     * setParameters.
     */
    bool apply(const matrix<T>& B,matrix<T>& X);

    /**
     * copy data of "other" functor.
     */
    luSolution& copy(const luSolution& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual luSolution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual luSolution* newInstance() const;

  protected:
    //luSolution<T>& copy(const luSolution<T>& other);

    vector<integer> dcmpVec_;
  };

}

#endif
