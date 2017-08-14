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
 * \file   ltiLuDecomposition.h
 *         Compute the LU decomposition of a given matrix.
 * \author Thomas Rusert
 * \author Gustavo Quiros
 * \date   23.06.1999
 *
 * $Id: ltiLuDecomposition.h,v 1.8 2006-09-07 13:38:37 doerfler Exp $
 */

#ifndef _LTI_LU_DECOMPOSITON_H_
#define _LTI_LU_DECOMPOSITON_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif

namespace lti {

#ifdef HAVE_LAPACK
  namespace internal {
    
    /**
     * Offers a direct interface to the LAPACK function getrf for
     * computing an LU decomposition with partial pivoting.
     *
     * This function is needed by other classes that use it as part of
     * the LAPACK processing chain.
     */
    template <typename T>
    class luDecompositionLapackInterface : public lapackInterface {
    public:
      /**
       *  GETRF computes an LU factorization of a general M-by-N matrix A
       *  using partial pivoting with row interchanges.
       *
       *  The factorization has the form
       *  \code
       *     A = P * L * U
       *  \endcode
       *  where P is a permutation matrix, L is lower triangular with unit
       *  diagonal elements (lower trapezoidal if m > n), and U is upper
       *  triangular (upper trapezoidal if m < n).
       */
      int getrf(integer* rows, integer* cols, T* a, 
                integer* lda, integer *ipiv, integer* info) const;
    };
  }
#endif      

  
  /**
   * LU decomposition functor.
   * Computes the LU decomposition of a square matrix.
   *
   * This class uses LAPACK if it is available.  Note that if LAPACK
   * is not used or not available, A <b>must</b> be of full rank!
   * 
   * @ingroup gLinearAlgebra
   */
  template<typename T>
  class luDecomposition 
    : public linearAlgebraFunctor
#ifdef HAVE_LAPACK
      , protected internal::luDecompositionLapackInterface<T> 
#endif
  {
    
  public:
    
    /**
     * The parameters for the class luDecomposition
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
      virtual bool write(ioHandler& handler, const bool complete=true) const;

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
    luDecomposition();

    /**
     * Construct a functor using the given parameters
     */
    luDecomposition(const parameters& par);
    
    /**
     * Copy constructor
     * @param other the object to be copied
     */
    luDecomposition(const luDecomposition& other);

    /**
     * Destructor
     */
    virtual ~luDecomposition();

    /**
     * onPlace version of apply.
     * 
     * Given a matrix a[0..n-1][0..n-1], this routine replaces it by
     * the LU decomposition of a rowwise permutation of itself.
     * permutation[0..n-1] is an output vector that records the row
     * permutation effected by the partial pivoting. 'pivot' is
     * +/-1 depending on whether the number of row interchanges was even or
     * odd, respectively.
     * @return true, if the decomposition could be computed, false
     *          otherwise (typically, the matrix was singular).
     */
    bool apply(matrix<T>& a, vector<integer>& permutation, int& pivot) const;

    /**
     * onCopy version of apply.
     * 
     * Given a matrix a[0..n-1][0..n-1], this routine returns a matrix
     * decomposition[0..n-1][0..n-1] which contains the LU decomposition of a
     * rowwise permutation of theMatrix. permutation[0..n-1] is an output
     * vector that records the row permutation effected by the partial
     * pivoting. 'pivot' is +/-1 depending on whether the number of
     * row interchanges was even or odd, respectively. 
     * @return true, if the decomposition could be computed, false
     *         otherwise (typically, the matrix was singular).
     */
    bool apply(const matrix<T>& a, matrix<T>& lu, 
	       vector<integer>& permutation, int& pivot) const;

    /**
     * Copy data of "other" functor.
     */
    luDecomposition<T>& copy(const luDecomposition<T>& other);
    
    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual luDecomposition<T>* clone() const;

    /**
     * Returns a pointer to a new instance of the functor.
     */
    virtual luDecomposition<T>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
    
    /**
     * Returns a new Matrix which contains the "L" part of the given
     * LU decomposition
     */
    matrix<T> extractL(const matrix<T>& lu) const;

    /**
     * Returns a new Matrix which contains the "U" part of the given
     * LU decomposition
     */
    matrix<T> extractU(const matrix<T>& lu) const;

    /**
     * Returns a new Matrix which contains the "L" part of the given
     * LU decomposition.  This is much faster than the on copy
     * version.
     */
    void extractL(const matrix<T>& lu, matrix<T>& l) const;

    /**
     * Returns a new Matrix which contains the "U" part of the given
     * LU decomposition.  This is much faster than the on copy
     * version.
     */
    void extractU(const matrix<T>& lu, matrix<T>& u) const;

    /**
     * Builds the permutation matrix for the given permutation vector
     * 
     * @param pv The row permutation vector on input
     * @param pm The row permutation matrix on output
     */
    void buildPermutationMatrix(const vector<integer>& pv, 
                                matrix<T>& pm) const;

  protected:

    static const T epsilon;

#ifdef HAVE_LAPACK	 
    bool applyLapack(matrix<T>& theMatrix,
                     vector<integer>& permutation,
                     int& pivot) const;
#endif
    
  };
}

#endif

