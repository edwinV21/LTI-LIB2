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
 * \file   ltiSymmetricEigenSystem.h
 *         Contains the template class symmetricEigenSystem<T> that
 *         uses LAPACK if available.
 * \author Thomas Rusert
 * \author Peter Doerfler
 * \date   16.06.99
 *
 * $Id: ltiSymmetricEigenSystem.h,v 1.5 2009-07-26 16:34:24 alvarado Exp $
 */

#ifndef _LTI_SYMMETRIC_EIGEN_SYSTEM_H_
#define _LTI_SYMMETRIC_EIGEN_SYSTEM_H_

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiLinearAlgebraFunctor.h"

#ifdef HAVE_LAPACK
#  include "ltiLapackInterface.h"
#endif

namespace lti {
  /**
   * Computes the Eigenvectors and Eigenvalues of a symmetric,
   * positive definite, real matrix.
   *
   * The most common source of such matrices is a covariance matrix
   * (see lti::secondOrderStatistics). 
   *
   * This class uses the LAPACK functions by default if they are
   * installed on your system. Otherwise a simple Jacobi algorithm is
   * used. This can also be chosen deliberately by setting
   * parameters::useLapack to false.
   *
   * The eigenvectors can be sorted according to their eigenvalues by
   * setting parameters::sort. A fixed number of eigenvectors can be
   * set via parameters::dimension. For LAPACK this also reduces
   * computation time.
   *
   * Please note that the eigenvector matrices will contain the
   * eigenvectors in the COLUMNS and not in the rows, as could be
   * expected. This avoids the requirement of transposing matrices in
   * eigenvector-based transformations like PCA!
   *
   * @ingroup gLinearAlgebra, gStatistics
   */
  template <typename T>
  class symmetricEigenSystem : public linearAlgebraFunctor
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
       * number of dimensions calculated. The default is zero. In this
       * case, all eigenvectors and eigenvalues are calculated.
       *
       * This option is only provided for compatibility with the
       * fastEigenSystem functor based on LAPACK, but it does not make (yet)
       * the computation of the eigensolution any faster.  It just will cut
       * the already computed complete solution to the desired size!
       *
       * Default value: 0 (implying all eigenvalues will be computed)
       */
      int dimensions;
    };


    /**
     * Default constructor
     */
    symmetricEigenSystem();

    /**
     * Default constructor with parameters
     */
    symmetricEigenSystem(const parameters& params);


    /**
     * Calculate the Eigensystem with \a eigenvalues and \a eigenvalues
     * from the given data in \a theMatrix (Samples in rows). Due to the
     * symmetricity of the data it suffices if \a theMatrix is upper
     * triangular.
     *
     * @param theMatrix  Real symmetric matrix to be transformed.
     * Only the diagonal and above diagonal elements have
     * to be set.
     * @param eigenvalues  Elements will contain the eigenvalues.
     * @param eigenvectors Columns will contain the eigenvectors corresponding
     * to the eigenvalues.
     * returns true if successful, false otherwise.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector<T>& eigenvalues,
                       matrix<T>& eigenvectors) const;


    /**
     * Convenience function that calculates the Eigensystem like
     * apply, but uses the given argument \a dimensions instead of
     * those given in parameters::dimensions.
     *
     * @param theMatrix  Real symmetric matrix to be transformed.
     * Only the diagonal and above diagonal elements have
     * to be set.
     * @param eigenvalues  Elements will contain the eigenvalues.
     * @param eigenvectors Columns will contain the eigenvectors corresponding
     * to the eigenvalues.
     * @param dimensions number of dimensions in eigenspace
     * returns true if successful, false otherwise.
     */
    virtual bool reducedEigenSystem(const matrix<T>& theMatrix,
                                    vector<T>& eigenvalues,
                                    matrix<T>& eigenvectors,
                                    const int dimensions) const;
    
    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual symmetricEigenSystem* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual symmetricEigenSystem* newInstance() const;

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

  protected:
    inline void rotateT(double& g,double& h,matrix<T>& a,
                        const int i,const int j,const int k,const int l,
                        const double s,const double tau) const;

    inline void rotate(double& g,double& h,matrix<double>& a,
                       const int i,const int j,const int k,const int l,
                       const double s,const double tau) const;

#ifdef HAVE_LAPACK
  private:
    /**
     * apply the LAPACK function with the correct parameters
     */
    bool applyLapack(const matrix<T>& theMatrix,
                     vector<T>& eigenvalues,
                     matrix<T>& eigenvectors,
                     const int dimensions) const;

    /**
     * Calls the lapack function of eigensystem of symmetric matrix
     * specialized for float and double
     */ 
    int evr(char* jobz, char* range, char* uplo,
            integer* n, T* a, integer* lda,
            T* vl, T* vu,
            integer* il, integer* iu,
            T* abstol,
            integer* m, T* w,
            T* z, integer* ldz, integer* isuppz,
            T* work, integer* lwork,
            integer* iwork, integer* liwork,
            integer* info) const;

    /**
     * Find out which is the best abstol value for evr
     */
    void lamch(char* cmach,T& abstol) const;
#endif

  };
}


#endif

