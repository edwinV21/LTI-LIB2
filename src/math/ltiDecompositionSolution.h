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
 * \file   ltiDecompositionSolution.h
 * \author Thomas Rusert
 * \date   02.06.1999
 *
 * $Id: ltiDecompositionSolution.h,v 1.1 2005-04-21 13:11:20 gquiros Exp $
 */

#ifndef _LTI_DECOMPOSITION_SOLUTION_H_
#define _LTI_DECOMPOSITION_SOLUTION_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * Abstract base class for all lti linear equation system solution functors
   * using matrix decomposition to solve Ax=b.
   *
   * @ingroup gLTILib2FormatRequired
   */
  template<class T>
  class decompositionSolution : public linearAlgebraFunctor {
  
  public:
    /**
     * Parameters of class decompositionSolution
     */
    class parameters : public linearAlgebraFunctor::parameters {
    
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * copy member.
       *
       * if the system-matrix of the other object is a normal matrix,
       * its contents will be copied to the system-matrix of this
       * parameters-object.  If the other system-matrix contains just
       * a reference to external data, then the system-matrix of this
       * object will contain the same reference to the data!  The
       * reason for this is to allow the use of this functor with huge
       * matrices without the need of duplication.  Use this data
       * referencing option carefully!
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      // -------------------------------------------------------
      // the parameters
      // -------------------------------------------------------

      /**
       * Matrix A.
       *
       * The linear system to be solved is Ax=b.
       *
       * Default value: empty matrix.
       */
      matrix<T> systemMatrix;

    };

    /**
     * default constructor
     */
    decompositionSolution();

    /**
     * constructor, sets the parameters
     */
    decompositionSolution(const parameters& theParams);

    /**
     * constructor, sets the matrix A
     */
    decompositionSolution(const matrix<T>& theMatrix);

    /**
     * destructor
     */
    virtual ~decompositionSolution() {};

    /**
     * sets the functor's parameters.
     * @return true if successful, false otherwise.
     */
    virtual bool updateParameters();

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     */
    decompositionSolution<T>& copy(const decompositionSolution<T>& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

  protected:
    bool decomposed_;
    matrix<T> dcmpMat_;
  };

}

#endif
