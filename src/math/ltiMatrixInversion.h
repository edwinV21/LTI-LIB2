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
 * \file   ltiMatrixInversion.h
 *         Compute the inverse of a given matrix.
 * \author Jochen Wickel
 * \date   07.10.2001
 *
 * $Id: ltiMatrixInversion.h,v 1.11 2010-10-13 00:48:25 alvarado Exp $
 */

#ifndef _LTI_MATRIX_INVERSION_H_
#define _LTI_MATRIX_INVERSION_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"

#ifdef HAVE_LAPACK
#include "ltiLuDecomposition.h"
#endif

namespace lti {
  /**
   * Matrix inversion functor.
   *
   * Computes the inverse of a matrix using LU decomposition.
   *
   * Only use this functor if the inverse of a matrix is needed explicity!

   * To solve an equation system Ax=b or a set of equation systems AX=B it is
   * more efficient to use the LU solution method (solve Ax=b resp. AX[i]=B[i])
   * directly than inverting A and multiplying the result by b resp. B! (see 
   * also lti::linearLeastSquares)
   *
   * If the parameter value method is set to SVD, a singular value
   * decomposition is used to invert the matrix, instead of the LU
   * decomposition method.
   *
   * For small (2x2,3x3 or 4x4) symmetric matrices you can also use
   * lti::symmetricMatrixInversion.
   *
   * This class uses LAPACK if it is available, but LAPACK's method is used 
   * iff the method used is LU decomposition
   *
   * @ingroup gLinearAlgebra
   * @ingroup gLTILib2FormatRequired
   */
  template <typename T>
  class matrixInversion 
    : public linearAlgebraFunctor
#ifdef HAVE_LAPACK
    , protected internal::luDecompositionLapackInterface<T>
#endif
  {
    
  public:

    /**
     * Available algorithms for matrix inversion
     */
    enum eAlgorithmType {
      LUD = 0, //!< LU-Decomposition
      SVD = 1  //!< Singular Value Decomposition
    };

    /**
     * %parameters class for lti::matrixInversion %functor
     */
    class parameters: public linearAlgebraFunctor::parameters {
      
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
       * copy member
       *
       * @param other the parameters object to be copied
       * @return a reference to this object
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * Return a cloned instance of this one.
       */
      virtual parameters* clone() const;

      /**
       * Return a cloned instance of this one.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;
      
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      
      virtual bool read(ioHandler& handler,const bool complete = true);
     
      
      // ------------------------------------------------------------------
      // The Parameters
      // ------------------------------------------------------------------
            
      /**
       * Algorithm to be used in the matrix inversion
       *
       * LUD is implemented with and without LAPACK.
       *
       * SVD is implemented without LAPACK.
       *
       * Default value: LUD
       */
      eAlgorithmType method;
    };
    
    /**
     * Default constructor
     */
    matrixInversion();
    
    /**
     * Default constructor
     */
    matrixInversion(const parameters& par);

    /**
     * Destructor
     */
    virtual ~matrixInversion();
    
    /**
     * onCopy version of apply.
     *
     * @param theMatrix matrix to be inverted
     * @param theInverse inverted matrix
     * @return true if inversion was possible, false otherwise.
     */
    bool apply(const matrix<T>& theMatrix,
               matrix<T>& theInverse) const;
    
    /**
     * onPlace version of apply.
     *
     * @param theMatrix matrix to be inverted.  The result will be left here
     *                  too.
     * @return true if inversion was possible, false otherwise.
     */
    bool apply(matrix<T>& theMatrix) const;
    
    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual matrixInversion<T>* clone() const;
    
    /**
     * Returns a pointer to a new instance of the functor.
     */
    virtual matrixInversion<T>* newInstance() const;
    
    /**
     * change the used parameters in order to use the
     * LU-decomposition method for matrix inversion.
     */
    void useLUD();
    
    /**
     * Change the used parameters in order to use the singular value
     * decomposition method for matrix inversion.
     */
    void useSVD();
    
    /**
     * get a constant reference to the actual used parameters.
     */
    const parameters& getParameters() const;
    
    
  protected:
    /**
     * Smallest value of type T, which can be added to 0 in order to produce
     * a number different than zero.
     */
    static const T epsilon;
    
    /**
     * onCopy version of apply.
     *
     * @param theMatrix matrix to be inverted
     * @param theInverse inverted matrix
     * @return true if inversion was possible, false otherwise.
     */
    bool svdMethod(const matrix<T>& theMatrix,
                   matrix<T>& theInverse) const;

    /**
     * onCopy version of apply.
     *
     * @param theMatrix matrix to be inverted
     * @param theInverse inverted matrix
     * @return true if inversion was possible, false otherwise.
     */
    bool ludMethod(const matrix<T>& theMatrix,
                   matrix<T>& theInverse) const;



#ifdef HAVE_LAPACK	 
    /**
     *  GETRI computes the inverse of a matrix using the LU factorization
     *  computed by *GETRF.
     *
     *  This method inverts U and then computes inv(A) by solving the system
     *  inv(A)*L = inv(U) for inv(A).
     */
    int getri(integer* n, T* a, integer* lda, integer *ipiv,
              T* work, integer* lwork, integer* info) const;
#endif
  };
}

#endif
