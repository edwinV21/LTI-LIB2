/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the Computer Vision and Robotics Library (LTI-Lib )
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiLinearLeastSquares.h
 *         Contains the class lti::linearLeastSquares,
 *         to solve a least squares linear problem.
 *
 * \author Pablo Alvarado
 * \date   16.01.2008
 *
 * revisions ..: $Id: ltiLinearLeastSquares.h,v 1.1 2008-04-04 20:39:36 alvarado Exp $
 */

#ifndef _LTI_LINEAR_LEAST_SQUARES_H_
#define _LTI_LINEAR_LEAST_SQUARES_H_

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiLinearAlgebraFunctor.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif


namespace lti {

  /**
   * Class linearLeastSquares
   *
   * This is a back-port from the CVR-Lib.
   *
   * Solving the linear least squares problem means to find a solution \f$x\f$
   * for
   * \f[
   * A x = b
   * \f]
   * by minimizing the Euclidean norm squared of the residual \f$Ax-b\f$.
   * Here \f$A\f$ is a m x n matrix, with m>n, \f$x\f$ is an n-dimensional
   * vector, and \f$b\f$ is a m-dimensional vector.
   *
   * This class is usually used to solve an over-determined system of linear
   * equations, where there is more equations than variables.  If the LAPACK
   * library is used, this method can also be employed to solve
   * under-determined system, choosing the solution of minimal norm.
   *
   * If LAPACK is enabled, the functor provides several algorithms for the same
   * task, which vary in speed and robustness against the ill-conditionness of
   * the matrix A.  Please refer to the LAPACK documentation for more details:
   *
   * - QR: uses the QR (or LQ) decomposition of the matrix A.
   *   This method can be employed if and only if the matrix A has full rank.
   * - CompleteFactorization: performs a complete orthogonal factorization
   * - SVD: uses a singular value decomposition.  This is the recomended method
   *        if the matrix A is ill-conditioned, and the only one used if LAPACK
   *        is not found.
   * - DCSVD: uses a divide-and-conquer singular value decomposition.
   *
   * @see linearLeastSquares::parameters.
   *
   * @ingroup gLinearAlgebra
   */
  class linearLeastSquares : public linearAlgebraFunctor {
  public:
    /**
     * Enumeration for the algorithms available
     */
    enum eAlgorithm {
      QR,       /**< QR decomposition: slower but more robust */
      CompleteFactorization, /**< Complete Orthogonal Factorization */
      SVD,      /**< Singular Value Decomposition: slowest but works even with
                     ill-conditioned matrices */
      DCSVD     /**< Divide-and-conquer singular value decomposition */
    };

    /**
     * The parameters for the class linearLeastSquares
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * Default constructor
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
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
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

      /**
       * Which algorithm should be used to solve the least squares problem.
       *
       * The available methods are:
       * - QR: uses the QR (or LQ) decomposition of the matrix A.
       *   This method can be employed if and only if the matrix A has full
       *   rank.
       * - CompleteFactorization: performs a complete orthogonal factorization
       * - SVD: uses a singular value decomposition.  This is the recomended
       *        method if the matrix A is ill-conditioned, and the only one 
       *        used if LAPACK is not found.
       * - DCSVD: uses a divide-and-conquer singular value decomposition.
       *
       * Default value: SVD
       */
      eAlgorithm algorithm;

      /**
       * The \c rCondition is used to determine the effective rank of the
       * matrix A, which is defined as the order of the largest leading
       * triangular submatrix R11 in the QR factorization with pivoting of A,
       * whose estimated condition number is less than 1.0/rCondition
       *
       * For intance, all singular values less than or equal to this value 
       * multiplied by the largest singular value are set to zero.
       *
       * Default value: 0.0
       */
      double rCondition;
    };

    /**
     * Default constructor
     */
    linearLeastSquares();

    /**
     * Construct a functor using the given parameters
     */
    linearLeastSquares(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    linearLeastSquares(const linearLeastSquares& other);

    /**
     * Destructor
     */
    virtual ~linearLeastSquares();

    /**
     * Find the solution \f$x\f$ for
     * \f[
     * A x = b
     * \f]
     *
     * @param A matrix A
     * @param b vector b
     * @param x vector x where the solution is stored.
     *
     * The dimension of \c b and the number of rows of A must be identical.
     * The resulting dimension of \c x will be identical to the number of
     * columns of A.
     *
     * The internal state of the class will store the decomposition of the
     * matrix A, so that the other apply methods with other \c b vectors can
     * be used.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const fmatrix& A,
               const fvector& b,
                     fvector& x) const;

    /**
     * Find the solution \f$x\f$ for
     * \f[
     * A x = b
     * \f]
     *
     * @param A matrix A
     * @param b vector b
     * @param x vector x where the solution is stored.
     *
     * The dimension of \c b and the number of rows of A must be identical.
     * The resulting dimension of \c x will be identical to the number of
     * columns of A.
     *
     * The internal state of the class will store the decomposition of the
     * matrix A, so that the other apply methods with other \c b vectors can
     * be used.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& A,
               const dvector& b,
                     dvector& x) const;


    /**
     * Find the solution \f$x\f$ for
     * \f[
     * A x = b
     * \f]
     *
     * @param A matrix A
     * @param b each column is a vector b
     * @param x each column is a vector x where the solution is stored for
     *          the corresponding column in b.
     *
     * The dimension of \c b and the number of rows of A must be identical.
     * The resulting dimension of \c x will be identical to the number of
     * columns of A.
     *
     * The internal state of the class will store the decomposition of the
     * matrix A, so that the other apply methods with other \c b vectors can
     * be used.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const fmatrix& A,
               const fmatrix& b,
                     fmatrix& x) const;

    /**
     * Find the solution \f$x\f$ for
     * \f[
     * A x = b
     * \f]
     *
     * @param A matrix A
     * @param b each column is a vector b
     * @param x each column is a vector x where the solution is stored for
     *          the corresponding column in b.
     *
     * The dimension of \c b and the number of rows of A must be identical.
     * The resulting dimension of \c x will be identical to the number of
     * columns of A.
     *
     * The internal state of the class will store the decomposition of the
     * matrix A, so that the other apply methods with other \c b vectors can
     * be used.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& A,
               const dmatrix& b,
                     dmatrix& x) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearLeastSquares& copy(const linearLeastSquares& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearLeastSquares& operator=(const linearLeastSquares& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual linearLeastSquares* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual linearLeastSquares* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     *
     */
    template<typename T>
    class helper 
#ifdef HAVE_LAPACK
    : public lapackInterface
#endif
    {
    public:
      /**
       * Find the solution \f$x\f$ for
       * \f[
       * A x = b
       * \f]
       *
       * where the matrix A was given in a previous apply or by means of the
       * use() methods.
       *
       * @param b vector b
       * @param x vector x where the solution is stored.
       *
       * The dimension of \c b and the number of rows of A must be identical.
       * The resulting dimension of \c x will be identical to the number of
       * columns of A.
       *
       * @return true if apply successful or false otherwise.
       */
      bool solve(const parameters& pars,
                 const matrix<T>& A,
                 const vector<T>& b,
                       vector<T>& x) const;

      /**
       * Find the solution \f$x\f$ for
       * \f[
       * A x = b
       * \f]
       *
       * where the matrix A was given in a previous apply or by means of the
       * use() methods.
       *
       * @param b vector b
       * @param x vector x where the solution is stored.
       *
       * The dimension of \c b and the number of rows of A must be identical.
       * The resulting dimension of \c x will be identical to the number of
       * columns of A.
       *
       * @return true if apply successful or false otherwise.
       */
      bool solve(const parameters& pars,
                 const matrix<T>& A,
                 const matrix<T>& b,
                       matrix<T>& x) const;

    protected:
#ifdef HAVE_LAPACK

      /**
       * GELS - solve overdetermined or underdetermined real linear systems
       * involving an M-by-N matrix A, or its transpose, using a QR or LQ
       * factorization of A };
       */
      int gels(char *trans, integer *m, integer *n, integer *nrhs, T* a,
               integer *lda, T* b, integer* ldb, T* work,
               integer *lwork, integer *info) const;
      /**
       * SGELSY - compute the minimum-norm solution to a real linear least
       * squares problem.
       *
       * Does the complete orthogonal decomposition
       */
      int gelsy(integer *m, integer *n, integer *nrhs, T* a,
                integer *lda, T* b, integer *ldb, integer *jpvt, 
                T* rcond,integer *rank, T* work, integer *lwork,
                integer *info) const;

      /**
       * SGELSS - compute the minimum norm solution to a real linear
       * least squares problem using SVD
       */
      int gelss(integer *m, integer *n, integer *nrhs, T* a,
                integer *lda, T* b, integer *ldb, T* s, T* rcond,
                integer *rank, T* work, integer *lwork, integer *info) const;

      /**
       * SGELSD - compute the minimum-norm solution to a real linear least
       * squares problem.  Uses the divide and conquer SVD method.
       */
      int gelsd(integer *m, integer *n, integer *nrhs, T *a,
                integer *lda, T *b, integer *ldb, T *s, T *rcond,
                integer *rank, T *work, integer *lwork, integer *iwork, 
                integer *info) const;
#endif
    };
        
    /**
     * Helper class for float methods.
     */
    helper<float> fhelper_;

    /**
     * Helper class for double methods.
     */
    helper<double> dhelper_;
  };

  /**
   * Read a linearLeastSquares::eAlgorithm
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            linearLeastSquares::eAlgorithm& data);

  /**
   * Write a linearLeastSquares::eAlgorithm
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const linearLeastSquares::eAlgorithm& data);

}

#endif

