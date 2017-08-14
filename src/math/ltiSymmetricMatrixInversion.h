/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * \file   ltiSymmetricMatrixInversion.h
 *         Inversion of symmetric matrices.
 * \author Peter Doerfler
 * \date   02.08.2003
 *
 * $Id: ltiSymmetricMatrixInversion.h,v 1.9 2006-09-07 13:38:37 doerfler Exp $
 */

#ifndef _LTI_SYMMETRIC_MATRIX_INVERSION_H_
#define _LTI_SYMMETRIC_MATRIX_INVERSION_H_


#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiCholeskyDecomposition.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif

namespace lti {
  
  /**
   * Functor for inversion of symmetric, positive definite
   * matrices. The functor uses the choleskyDecomposition for
   * inversion unless the size of the matrix is smaller than or equal
   * to four. In that case the cofactor method is used which can
   * handle matrices that are not positiv definite but requires them
   * to be non-singular.
   *
   * This class uses LAPACK if it is available.
   *
   * @see lti::matrixInversion
   *
   * @ingroup gLinearAlgebra
   */
  template<typename T>
  class symmetricMatrixInversion : public linearAlgebraFunctor
#ifdef HAVE_LAPACK
    , public lapackInterface
#endif
  {
    
  public:

    /**
     * The parameters for the class symmetricMatrixInversion
     */
    class parameters : public linearAlgebraFunctor::parameters {

    public:
      
      /**
       * default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);
      
      /**
       * Destructor
       */
      ~parameters();
      
      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Return the name of the class 
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);
      
      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
    };

    /**
     * Default constructor
     */
    symmetricMatrixInversion();

    /**
     * Construct a functor using the given parameters
     */
    symmetricMatrixInversion(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    symmetricMatrixInversion(const symmetricMatrixInversion& other);

    /**
     * Destructor
     */
    virtual ~symmetricMatrixInversion();

    /**
     * Inverts the symmetric matrix \a srcdest. If the dimension of \a
     * srcdest ist greater than 4 it must be positive definite as
     * well.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Inverts the symmetric matrix \a src and leaves the result in \a
     * dest. If the dimension of \a src ist greater than 4 it must
     * be positive definite as well.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    symmetricMatrixInversion& copy(const symmetricMatrixInversion& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    symmetricMatrixInversion& operator=(const symmetricMatrixInversion& other);

    /**
     * Return the name of the class 
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual linearAlgebraFunctor* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual linearAlgebraFunctor* newInstance() const;
    
    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

#ifdef HAVE_LAPACK
    
    /* LAPACK routines in template form */
    
    /**
     *  POTRI computes the inverse of a real symmetric positive definite
     *  matrix A using the Cholesky factorization A = U**T*U or A = L*L**T
     *  computed by POTRF.
     */
    int potri(char *uplo, integer* n, T* a, integer* lda, integer* info) const;

#endif

  protected:

    /**
     * Invert the matrix \a m using the cholesky decomposition
     */
    bool choleskyInversion(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * Inverts a 2x2 matrix
     */
    bool invert2(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * Inverts a 3x3 matrix
     */
    bool invert3(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * Inverts a 4x4 matrix
     */
    bool invert4(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * Cholesky Decomposition functor.
     */
    choleskyDecomposition<T> chol;

  };
}

#endif
