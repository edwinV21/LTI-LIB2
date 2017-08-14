/*
 * Copyright (C) 1998-2005
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
 * \file   ltiQrDecomposition.h
 *         Compute the QR decomposition of a matrix.
 * \author Arnd Hannemann
 * \date   26.01.2004
 *
 * $Id: ltiQrDecomposition.h,v 1.12 2006-09-07 13:38:37 doerfler Exp $
 */

#ifndef _LTI_QR_DECOMPOSITION_H_
#define _LTI_QR_DECOMPOSITION_H_


#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiPerformanceConfig.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif

namespace lti {
  /**
   * This functor computes a QRDecomposition of a given rectangular 
   * m x n Matrix A of the Form:
   *    
   *  A = Q * R
   *
   * Where R is an upper triangular m x m Matrix and Q is a 
   * m x n orthogonal matrix.
   * Transpose of Q muliplied with Q itself is the identity Matrix.
   *
   * If LAPACK is not used or not available, A \b must be of full rank!
   *
   * \code
   * matrix<float> src(3,3);
   * float data[] =  {1,2,3,4,5,6,7,8,9};
   * src.fill(data);
   * matrix<float> q,r;
   *
   * qrDecomposition<float> qrd;
   * qrd.apply(src,q,r);
   *
   * matrix<float> result;
   * result.multiply(q,r);
   *
   * std::cout << "Q:\n" << q << "\n";
   * std::cout << "R:\n" << r << "\n";
   * // should be identical to src
   * std::cout << "A = Q * R:\n"<< result << "\n";
   *
   * \endcode
   *
   * \ingroup gLinearAlgebra
   */
  template<typename T>
  class qrDecomposition : public linearAlgebraFunctor  
#ifdef HAVE_LAPACK
    , public lapackInterface 
#endif
  {
    
  public:
    /**
     * The parameters for the class qrDecomposition
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
       * Returns the name of this class.
       */
      const std::string& name() const;
      
      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;
      
      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;
      
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler, const bool complete=true);
    
      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
      
      /**
       * Specifies above which matrix size, the matrix is internally
       * transposed.
       *
       * This parameter takes only effect if useLapack is false or LAPACK is
       * not available.
       *
       * Due to memory access isues, the algorithm works a lot faster 
       * especially for large matrices.
       *
       * Both, collumns and rows are checked against this value. So a
       * value of 50 means that the matrix must have at least 51 rows
       * <b>and</b> at least 51 columns to be transposed.
       *
       *
       * Default: _LTI_PERFORMANCE_QR_DECOMPOSITION in ltiPerformanceConfig.h
       */
      int performanceTweakThresholdForTranspose;
      
    };
      
    /**
     * Default constructor
     */
    qrDecomposition();
    
    /**
     * Construct a functor using the given parameters
     */
    qrDecomposition(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    qrDecomposition(const qrDecomposition& other);

    /**
     * Destructor
     */
    virtual ~qrDecomposition();

    /**
     * Apply QR using column pivoting (in place)
     * 
     * @param arh The source matrix on input, and on output
     *            the compact representation (R + elementary reflectors)
     * @param tau The beta components on output
     * @param p   The column permutation vector on output
     * @return true if the application was successful, false otherwise
     */
    bool apply(matrix<T>& arh, vector<T>& tau, vector<integer>& p) const;
    
    /**
     * Apply QR using column pivoting (on copy)
     * 
     * @param a   The source matrix on input
     * @param rh  The compact representation (R + elementary reflectors) on output
     * @param tau The beta components on output
     * @param p   The column permutation vector on output
     * @return true if the application was successful, false otherwise
     */
    bool apply(const matrix<T>& a,
	             matrix<T>& rh,
	             vector<T>& tau,
	             vector<integer>& p) const; 

    /**
     * Apply QR (in place)
     * 
     * @param arh The source matrix on input, and on output
     *            the compact representation (R + elementary reflectors)
     * @param tau The beta components on output
     * @return true if the application was successful, false otherwise
     */
    bool apply(matrix<T>& arh, vector<T>& tau) const;

    /**
     * Apply QR (on copy)
     * 
     * @param a   The source matrix on input
     * @param rh  The compact representation (R + elementary
     *            reflectors) on output
     * @param tau The beta components on output
     * @return true if the application was successful, false otherwise
     */
    bool apply(const matrix<T>& a, matrix<T>& rh, vector<T>& tau) const;
    
    /**
     * Extract R from RH.
     *
     * @param rh  The compact representation (R + elementary reflectors) 
     *            on input
     * @param r   R on output
     */
    void extractR(const matrix<T>& rh, matrix<T>& r) const;
    
    /**
     * Compute Q from RH and tau.
     *
     * @param rh  The compact representation (R + elementary reflectors)
     *            on input
     * @param tau The beta components on input
     * @param q   Q on output
     */
    bool computeQ(const matrix<T>& rh, vector<T>& tau, matrix<T>& q) const;
  
    /**
     * Builds the permutation matrix for the given permutation vector
     * 
     * @param pv The column permutation vector on input
     * @param pm The column permutation matrix on output
     */
    void buildPermutationMatrix(const vector<integer>& pv,
                                matrix<T>& pm) const;
    
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qrDecomposition& copy(const qrDecomposition& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qrDecomposition& operator=(const qrDecomposition& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual qrDecomposition<T>* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual qrDecomposition<T>* newInstance() const;
    
    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:

#ifdef HAVE_LAPACK	 
    
    // LAPACK routines in template form
    
    /*  *GEQRF computes a QR factorization of a real M-by-N matrix A:
     *  A = Q * R. */
    int geqrf(integer* rows, integer* cols, T* a, integer* lda, T* tau,
              T* work, integer* lwork, integer* info) const;
    
    /*  *ORGQR generates an M-by-N real matrix Q with orthonormal columns,
     *  which is defined as the first N columns of a product of K elementary
     *  reflectors of order M
     *
     *        Q  =  H(1) H(2) . . . H(k)
     *
     *  as returned by SGEQRF. */
    int orgqr(integer* rows, integer* cols, integer* k,T* a, integer* lda,
              T* tau, T* work, integer* lwork, integer* info) const;
    
    /*  *GEQP3 computes a QR factorization with column pivoting of a
     *  matrix A:  A*P = Q*R  using Level 3 BLAS. */
    int geqp3(integer* rows, integer* cols, T* a, integer* lda, integer *jpvt,
              T* tau, T* work, integer* lwork, integer* info) const;
    
#endif
    
    /**
     * Implementation of QR decomposition using Householder reflections.
     * The boolean parameter 'useColumnPivoting'specifies if column
     * pivoting should be used. If so, the permutation vector p will contain
     * the result of the columnwise pivoting.
     *
     * @param arh On input, the input matrix. 
     *            On output, the compact form representation
     *            of R and the essential parts of the Householder vectors.
     * @param tau On output, the beta components.
     * @param p   On output, the permutation vector if useColumnPivoting 
     *            was set to true.
     * @param useColumnPivoting Determines if column pivoting should be used
     * @return true if the computation was succesfull, false otherwise.
     */
    bool computeHouseholderQr(matrix<T>& arh, 
                              vector<T>& tau, 
                              vector<integer>& p, 
                              bool useColumnPivoting) const;
    
  };

}

#endif
