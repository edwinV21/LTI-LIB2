/*
 * Copyright (C) 1998-2005
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2010
 * Pablo Alvarado
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
 * \file   ltiEigenSystem.h
 *         Contains the template class eigenSystem<T> that
 *         uses LAPACK to compute the eigenvectors and eigenvalues of
 *         of any square matrix (not necessarily symmetric).
 * \author Pablo Alvarado
 * \date   10.10.10
 *
 * $Id: ltiEigenSystem.h,v 1.6 2012-07-01 00:36:15 alvarado Exp $
 */

#ifndef _LTI_EIGEN_SYSTEM_H_
#define _LTI_EIGEN_SYSTEM_H_

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiComplex.h"

#ifdef HAVE_LAPACK
#  include "ltiLapackInterface.h"
#endif

namespace lti {
  /**
   * Eigen system class.
   * 
   * This functor computes the eigenvectors and eigenvalues of a matrix of real
   * values.  It is not required for the matrix to be symmetric, a case for
   * which the class lti::symmetricEigenSystem is best suited.
   * 
   * Since the matrix is not symmetric, complex eigenvalues and eigenvectors
   * should be expected.
   * 
   * If parameters::leftVector is set to false (default behaviour), then the
   * right-eigenvector problem is solved
   * \f[
   * A v_j = \lambda_j v_j
   * \f]
   * where \f$A\f$ is the matrix, \f$\lambda_j\f$ the eigenvalues and 
   * \f$v_j\f$ the eigenvectors.
   *
   * If parameters::leftVector is set to true then the left-eigenvector
   * problem is solved 
   * \f[
   * u_j^H A = \lambda_j u_j^H \f] where \f$A\f$ is the matrix, \f$\lambda_j\f$
   * the eigenvalues and \f$u_j^H\f$ the conjugate transpose of the 
   * eigenvectors\f$u_j\f$.
   *
   * This class requires the LAPACK library. Otherwise an error will be
   * returned when apply is called.
   *
   * The eigenvectors can be sorted according to the magnitude of their
   * eigenvalues by setting parameters::sort. A fixed number of eigenvectors
   * can be set via parameters::dimension.
   *
   * Please note that the eigenvector matrices will contain the
   * eigenvectors in the COLUMNS and not in the rows, as could be
   * expected. This avoids the requirement of transposing matrices in
   * eigenvector-based transformations.
   *
   * @ingroup gLinearAlgebra, gStatistics
   */
  template <typename T>
  class eigenSystem : public linearAlgebraFunctor
#ifdef HAVE_LAPACK
    , public lapackInterface
#endif
  {
  public:

    /**
     * eigenSystem parameter class
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
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * \param handler the ioHandler to be used
       * \param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * \return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;
      
      /**
       * read the parameters from the given ioHandler
       * \param handler the ioHandler to be used
       * \param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * \return true if write was successful
       */
      virtual bool read(ioHandler& handler, const bool complete=true);


      //****************************
      // the parameters
      //****************************

      /**
       * If true, the eigenvalues and corresponding eigenvectors will be
       * sorted in decreasing order of the eigenvalues.
       *
       * Default value: false
       */
      bool sort;

      /**
       * Number of dimensions calculated.
       *
       * The default value is zero, which indicates that all eigenvectors and
       * eigenvalues are calculated.
       *
       * This option is only provided for compatibility with other eigen-system
       * functors, but it does not make the computation of the eigensolution
       * any faster.  It just will cut the already computed complete solution
       * to the desired size!
       *
       * Default value: 0 (implying all eigenvalues will be computed)
       */
      int dimensions;

      /**
       * To compute left eigenvectors set to true.  Right eigenvectors are
       * computed by default (set to false).
       *
       * Default value: false
       */
      bool leftVector;
    };


    /**
     * Default constructor
     */
    eigenSystem();

    /**
     * Default constructor with parameters
     */
    eigenSystem(const parameters& params);


    /**
     * Calculate the Eigensystem with \a eigenvalues and \a eigenvalues
     * from the given data in \a theMatrix (Samples in rows). Due to the
     * ity of the data it suffices if \a theMatrix is upper
     * triangular.
     *
     * @param theMatrix  Real  matrix to be transformed.
     * Only the diagonal and above diagonal elements have
     * to be set.
     * @param rEigenvalues  will contain the real part of the eigenvalues.
     * @param iEigenvalues will contain the imaginary part of the eigenvalues
     * @param rEigenvectors columns will contain the real part of the 
     *                     eigenvectors corresponding to the eigenvalues.
     * @param iEigenvectors columns will contain the imaginary part of the
     *                      eigenvectors corresponding to the eigenvalues.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector<T>& rEigenvalues,
                       vector<T>& iEigenvalues,
                       matrix<T>& rEigenvectors,
                       matrix<T>& iEigenvectors) const;

    /**
     * Calculate the Eigensystem with \a eigenvalues and \a eigenvalues
     * from the given data in \a theMatrix (Samples in rows). Due to the
     * ity of the data it suffices if \a theMatrix is upper
     * triangular.
     *
     * @param theMatrix  Real  matrix to be transformed.
     * Only the diagonal and above diagonal elements have
     * to be set.
     * @param eigenvalues  will contain the eigenvalues.
     * @param eigenvectors columns will contain the eigenvectors corresponding 
     *                     to the eigenvalues.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector< complex<T> >& eigenvalues,
                       matrix< complex<T> >& eigenvectors) const;

    /**
     * Computes left or right eigenvalues and eigenvectors of the
     * given matrix. Which kind will be computed depends on the
     * value of getParameters().leftVector. 
     *
     * \warning: This apply method just ignores imaginary parts of the
     *           eigenvalues and eigenvector.  So if the eigenvalues are in
     *           fact complex, you get wrong results.
     *
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     * @return false if \a theMatrix contains illegal values like NaN. 
     *         All return arguments are cleared. See lti::validator.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector<T>& eigenvalues,
                       matrix<T>& eigenvectors) const;

    /**
     * Computes left eigenvalues and eigenvectors of the
     * given matrix.
     *
     * This is just a wrapper for the LAPACK call, and the returned data
     * needs to be processed and presented in a more straightforward manner.
     *
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param iEigenvalues elements will contain the imaginary part of
     *                     the eigenvalues
     * @param eigenvectors rows will contain the eigenvectors
     *                     corresponding to the eigenvalues.  In case the
     *                     eigenvalues are complex, they will appear in 
     *                     conjugate pairs and so the eigenvectors, which
     *                     will have to be decoded by the calling function.
     * @return false if \a theMatrix contains illegal values like NaN. 
     *         All return arguments are cleared. See lti::validator.
     */
    bool solve(const matrix<T>& theMatrix,
               vector<T>& eigenvalues,
               vector<T>& iEigenvalues,
               matrix<T>& eigenvectors) const;
    
    /**
     * Method to split a complex vector into its two components
     */
    void split(const vector< complex <T> >& cplx,
               vector<T>& re,
               vector<T>& im) const;

    /**
     * Method to merge real and imag components into a complex one
     */
    void merge(const vector<T>& re,
               const vector<T>& im,
               vector< complex <T> >& cplx) const;

    /**
     * Method to split a complex vector into its two components
     */
    void split(const matrix< complex <T> >& cplx,
               matrix<T>& re,
               matrix<T>& im) const;

    /**
     * Method to merge real and imag components into a complex one
     */
    void merge(const matrix<T>& re,
               const matrix<T>& im,
               matrix< complex <T> >& cplx) const;

    /**
     * Convenience function that calculates the eigensystem like
     * apply, but uses the given argument \a dimensions instead of
     * those given in parameters::dimensions.
     *
     * @param theMatrix  Real  matrix to be transformed.
     *                   Only the diagonal and above diagonal elements have
     *                   to be set.
     * @param eigenvalues  Elements will contain the eigenvalues.
     * @param eigenvectors Columns will contain the eigenvectors corresponding
     *                     to the eigenvalues.
     * @param dimensions number of dimensions in eigenspace
     * @returns true if successful, false otherwise.
     */
    virtual bool reducedEigenSystem(const matrix<T>& theMatrix,
                                    vector< complex<T> >& eigenvalues,
                                    matrix< complex<T> >& eigenvectors,
                                    const int dimensions) const;
    
    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual eigenSystem* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual eigenSystem* newInstance() const;

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

#ifdef HAVE_LAPACK
  private:
    /**
     * Calls the lapack function of eigensystem of  matrix
     * specialized for float and double
     */ 
    // lapack routine in template form
    int geev(char *, char *, integer *, T *, integer *, T*, T*, T*,
             integer*,  T*, integer*, T*, integer*, integer*) const;

#endif

  };
}


#endif

