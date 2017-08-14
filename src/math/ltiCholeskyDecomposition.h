/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiCholeskyDecomposition.h
 *         Defines the class lti::choleskyDecomposition which decomposes a 
 *         matrix in a upper/lower triangular matrix.
 * \author Peter Doerfler
 * \date   07.07.2003
 * 
 * $Id: ltiCholeskyDecomposition.h,v 1.6 2005-06-22 15:26:40 doerfler Exp $
 */

#ifndef _LTI_CHOLESKY_DECOMPOSITION_H_
#define _LTI_CHOLESKY_DECOMPOSITION_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiTriangularMatrixType.h"

namespace lti {
  /**
   * This functor calculates the Cholesky decomposition of a
   * symmetric, positive-definite matrix A. The Cholesky decomposition
   * is defined as A = L * L' (with L' the transpose of L) or A = U' *
   * U. The cholesky factor L or U are lower or upper triangular
   * matrices, respectively, where L=U'. The nature of the Cholesky
   * factor can be determined by setting
   * parameters::triangularMatrixType or using the appropriate apply
   * method. The default is Upper due to shorter calculation time.
   *
   * The Cholesky decomposition can be used to solve a linear equation
   * system A*x=b by first solving L*y=b and then L'*x=y using the
   * forwardSubstitution and backSubstitution functors, respectively.
   *
   * This class cannot use LAPACK (yet).
   */
  template<typename T>
  class choleskyDecomposition : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class choleskyDecomposition
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * default constructor. The triangularMatrixType is Upper by default.
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);
      
      /**
       * destructor
       */
      ~parameters();

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;
      
      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      eTriangularMatrixType triangularMatrixType;
    };

    /**
     * default constructor
     */
    choleskyDecomposition();

    /**
     * Construct a functor using the given parameters
     */
    choleskyDecomposition(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    choleskyDecomposition(const choleskyDecomposition& other);

    /**
     * destructor
     */
    virtual ~choleskyDecomposition();

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of parameters::triangularMatrixType. The default Upper is faster.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of parameters::triangularMatrixType. The default Upper is faster.
     * @param src matrix<T> with the source data.
     * @param l matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& l) const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of \a tType.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @param tType type of triangular matrix
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest, const eTriangularMatrixType& tType) const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of \a tType.
     * @param src matrix<T> with the source data.
     * @param l matrix<T> where the result will be left.
     * @param tType type of triangular matrix
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src, matrix<T>& l,
               const eTriangularMatrixType& tType) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    choleskyDecomposition& copy(const choleskyDecomposition& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    choleskyDecomposition& operator=(const choleskyDecomposition& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual choleskyDecomposition* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual choleskyDecomposition* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

