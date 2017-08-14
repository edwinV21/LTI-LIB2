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
 * \file   ltiLuDecomposition_template.h
 *         Compute the LU decomposition of a given matrix.
 * \author Thomas Rusert
 * \author Gustavo Quiros
 * \date   23.06.1999
 *
 * $Id: ltiLuDecomposition_template.h,v 1.15 2012-11-17 14:49:05 alvarado Exp $
 */

#ifdef HAVE_LAPACK
#  include "clapack.h"
#endif


#undef min
#undef max

#include "ltiIncompatibleDimensionsException.h"
#include "ltiMath.h"
#include <limits>

#include "ltiDebug.h"

namespace lti {

//------------------------------------------------------------
// internal::luDecompositionLapackInterface
//------------------------------------------------------------

#ifdef HAVE_LAPACK
  namespace internal {
    template<>
    int luDecompositionLapackInterface<float>
    ::getrf(integer* rows, integer* cols,
            float* a, integer* lda,
            integer *ipiv, integer* info) const {
      lockInterface();
      int tmp=LA_SGETRF(rows,cols,a,lda,ipiv,info);
      unlockInterface();
      return tmp;
    }
    
    template<>
    int luDecompositionLapackInterface<double>
    ::getrf(integer* rows, integer* cols, 
            double* a, integer* lda, 
            integer *ipiv, integer *info) const {
      lockInterface();
      int tmp=LA_DGETRF(rows,cols,a,lda,ipiv,info);
      unlockInterface();
      return tmp;
    }
  }
#endif

  
  /* LU decomposition functor */
  
  /**
   * Use Sqrt(numeric_limits<double>::min()) as epsilon value
   */
  template<typename T>
  const T luDecomposition<T>::epsilon = sqrt(std::numeric_limits<T>::min());

  /**
   * Default constructor
   */
  template<typename T>
  luDecomposition<T>::parameters::parameters() 
    : linearAlgebraFunctor::parameters() {
  }
  
  /**
   * Copy constructor
   */
  template<typename T>
  luDecomposition<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  /**
   * Destructor
   */
  template<typename T>
  luDecomposition<T>::parameters::~parameters() {
  }
      
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename luDecomposition<T>::parameters& 
  luDecomposition<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    return *this;
  }
      
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename luDecomposition<T>::parameters& 
  luDecomposition<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }
      
  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& luDecomposition<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
      
  /**
   * Returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename luDecomposition<T>::parameters* 
  luDecomposition<T>::parameters::clone() const {
    return new parameters(*this);
  }

  /**
   * Returns a pointer to a new instance of the parameters
   */
  template<typename T>
  typename luDecomposition<T>::parameters* 
  luDecomposition<T>::parameters::newInstance() const {
    return new parameters();
  }
      
  /**
   * Write the parameters in the given ioHandler
   */
  template<typename T>
  bool luDecomposition<T>::parameters::write(ioHandler& handler, const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }
  
  /**
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template<typename T>
  bool luDecomposition<T>::parameters::read(ioHandler& handler, const bool complete){
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    if (complete) {
      b = b && handler.readEnd();
    }
        
    return b;
  }

  template<typename T>
  luDecomposition<T>::luDecomposition() 
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , internal::luDecompositionLapackInterface<T>()
#endif
  {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }
  
  /**
   * Constructor with parameters
   */
  template <typename T>
  luDecomposition<T>::luDecomposition(const parameters& par)
    : linearAlgebraFunctor() 
#ifdef HAVE_LAPACK
    , internal::luDecompositionLapackInterface<T>()
#endif
  { 
    // set the given parameters
    setParameters(par);
  }

  /**
   * Copy constructor
   */
  template <typename T>
  luDecomposition<T>::luDecomposition(const luDecomposition<T>& other)
    : linearAlgebraFunctor() 
#ifdef HAVE_LAPACK
    , internal::luDecompositionLapackInterface<T>()
#endif
  {
    copy(other);
  }

  /**
   * Destructor
   */
  template<typename T>
  luDecomposition<T>::~luDecomposition() {
  }

  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& luDecomposition<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /**
   * Returns a pointer to a clone of the functor.
   */
  template<typename T>
  luDecomposition<T>* luDecomposition<T>::clone() const {
    return (new luDecomposition<T>(*this));
  }
  
  /**
   * Returns a pointer to a new instance of the functor.
   */
  template<typename T>
  luDecomposition<T>* luDecomposition<T>::newInstance() const {
    return (new luDecomposition<T>());
  }
  
  /**
   * Return parameters
   */
  template <typename T>
  const typename luDecomposition<T>::parameters&
  luDecomposition<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  /**
   * Extract the "L" part of the LU decomposition
   */
  template<typename T>
  matrix<T> luDecomposition<T>::extractL(const matrix<T>& lu) const {
    matrix<T> res;
    extractL(lu,res);
    return res;
  }

  /**
   * Extract the "L" part of the LU decomposition
   */
  template<typename T>
  void luDecomposition<T>::extractL(const matrix<T>& lu, matrix<T>& l) const {
    const int rows = lu.rows();
    const int cols = lu.columns();
    l.resize(rows,cols,T(),AllocateOnly);
    
    int i, j;
    for (i = 0; i < rows; i++) {
      for (j = 0; j < i; j++) {
        l.at(i,j) = lu.at(i,j);
      }
      l.at(i,i) = T(1);
      for (j = i + 1; j < cols; j++) {
        l.at(i,j) = T(0);
      }
    }
  }


  /**
   * Extract the "U" part of the LU decomposition
   */
  template<typename T>
  void luDecomposition<T>::extractU(const matrix<T>& lu, matrix<T>& u) const {
    const int cols = lu.columns();
    u.resize(cols,cols,T(),AllocateOnly);
    
    int i, j;
    for (i = 0; i < cols; i++) {
      for (j = 0; j < i; j++) {
        u.at(i,j) = T(0);
      }
      for (j = i; j < cols; j++) {
        u.at(i,j) = lu.at(i,j);
      }
    }
  }
    

  /**
   * Extract the "U" part of the LU decomposition
   */
  template<typename T>
  matrix<T> luDecomposition<T>::extractU(const matrix<T>& lu) const {
    matrix<T> res;
    extractU(lu,res);
    return res;
  }

#ifdef HAVE_LAPACK
  template<typename T>
  bool luDecomposition<T>::applyLapack(matrix<T>& theMatrix,
                                       vector<integer>& permutation,
                                       int& pivot) const {
    const int m = theMatrix.columns();
    const int n = theMatrix.rows();
    
    if (theMatrix.getMode() != lti::matrix<T>::Connected) {
      // This resize connects the matrix data
      theMatrix.resize(n,m);
    }
    
    // transpose matrix for lapack use
    theMatrix.transpose();
    
    /*  M       (input) INTEGER
     *          The number of rows of the matrix A.  M >= 0. */
    integer rows = m;
    
    
    /*  N       (input) INTEGER
     *          The number of columns of the matrix A.  N >= 0. */
    integer cols = n;
    
    /*  LDA     (input) INTEGER
     *          The leading dimension of the array A.  LDA >= max(1,M). */
    integer lda = rows;
    
    /*  IPIV    (output) INTEGER array, dimension (min(M,N))
     *          The pivot indices; for 1 <= i <= min(M,N), row i of the
     *          matrix was interchanged with row IPIV(i). */
    
    permutation.resize(min(rows,cols));
    
    /*  INFO    (output) INTEGER
     *          = 0:  successful exit
     *          < 0:  if INFO = -i, the i-th argument had an illegal value
     *          > 0:  if INFO = i, U(i,i) is exactly zero. The factorization
     *                has been completed, but the factor U is exactly
     *                singular, and division by zero will occur if it is used
     *                to solve a system of equations. */
    integer info = 0;
    
    this->getrf(&rows,&cols,&theMatrix.at(0,0), &lda,
		permutation.data(), &info);
    
    pivot = 1;
    
    for(int i = 0; i < cols; ++i){
      // decrease indexes to convert from Fortran
      permutation.at(i)--;
      if(i != permutation.at(i)) pivot *= -1;
    }
    
    theMatrix.transpose();
    
    return info == 0;
  }
#endif

  /**
   * onPlace version of apply
   */
  template<typename T>
  bool luDecomposition<T>::apply(matrix<T>& theMatrix,
                                 vector<integer>& permutation,
                                 int& pivot) const {
    

    
    
#ifdef HAVE_LAPACK
    const parameters& par = getParameters();
      
    if(par.useLapack) {
      return applyLapack(theMatrix, permutation, pivot);
    }
      
#endif
      
#if 0
    // Work-in-progress implementation, NOT functional
    // Currently falling back to previous implementation.
    
    const int n = theMatrix.rows();
    if(n != theMatrix.columns()) throw incompatibleDimensionsException();
    
    permutation.resize(n - 1);
    
    for(int k = 0; k < n - 1; ++k){
      int mu = 0;
      for(int i = k; i < n; ++i){
        if(abs(theMatrix.at(mu,k)) > mu) mu = i;
      }
      if(k != mu){
        //for(int i = k; i < n; ++i){
        for(int i = 1; i < n; ++i){
          swap(theMatrix.at(k,i),theMatrix.at(mu,i));
        }
      }
      permutation.at(k) = mu;
      if(theMatrix.at(k,k) != 0){
        for(int row = k + 1; row < n; ++row){
          theMatrix.at(row,k) /= theMatrix.at(k,k);
          theMatrix.at(row,row) -= theMatrix.at(row,k) * theMatrix.at(k,row);
        }
      } 
    }
    
    return true;
      
#else
    
    const int n=theMatrix.rows();
    if(n!=theMatrix.columns()) throw incompatibleDimensionsException();
    int i,imax,k,d;
    T big,dum,temp,sum;
    vector<T> vv(n);
    permutation.resize(n);
    
    d=1;
    _lti_debug("La");
    big=static_cast<T>(1);
    // loop over rows to get implicit scaling information
    for (i=0;i<n && big >= epsilon;i++) {
      _lti_debug("b");
      big=0;
      for (int j=0;j<n;j++) {
        if ((temp=abs(theMatrix[i][j])) > big) big=temp;
      }
      // if a row is all zero, there is nothing we can do...
      if (big >= epsilon) {
        // save scaling
        vv[i]=static_cast<T>(1)/big;
      }
    }
    if (big < epsilon) {
      setStatusString("Singular matrix, cannot decompose");
      return false;
    }
    _lti_debug("\nLc");
    // loop over columns (Crout's method)
    for (int j=0;j!=n;j++) {
      _lti_debug("d");
      for (i=0;i<j;i++) {
        sum=theMatrix[i][j];
        for (k=0;k<i;k++) {
          sum -= theMatrix[i][k]*theMatrix[k][j];
        }
        theMatrix[i][j]=sum;
      }
      // initialize for the search for largest pivot element
      _lti_debug("e");
      big=0.0;
      imax=j;
      for (i=j;i<n;i++) {
        sum=theMatrix[i][j];
        for (k=0;k<j;k++) {
          sum -= theMatrix[i][k]*theMatrix[k][j];
        }
        theMatrix[i][j]=sum;
        // is the figure of merit for the pivot better than the best so far?
        if ( (dum=vv[i]*abs(sum)) >= big) {
          big=dum;
          imax=i;
        }
      }
      _lti_debug("f");
      if (j != imax) {
        _lti_debug("1");
        // interchange rows, if necessary
        for (k=0;k<n;k++) {
          _lti_debug("2");
          dum=theMatrix[imax][k];
          theMatrix[imax][k]=theMatrix[j][k];
          theMatrix[j][k]=dum;
        }
        // change parity of d
        d = -d;
        _lti_debug("3");
        // interchange the scaling factor
        vv[imax]=vv[j];
      }
      permutation[j]=imax;
      _lti_debug("g");
      
      // if the pivot element is zero, the matrix is singular
      // However, we just set it to epsilon, so that we don't get
      // illegal results later
      if (fabs(theMatrix[j][j]) < std::numeric_limits<double>::epsilon()) {
        theMatrix[j][j]=(theMatrix[j][j] >= 0)
          ? (std::numeric_limits<T>::epsilon())
          : (-std::numeric_limits<T>::epsilon());
      }
      _lti_debug("h");
      if (j != n) {
        // divide by the pivot element
        dum=static_cast<T>(1)/(theMatrix[j][j]);
        for (i=j+1;i<n;i++) {
          theMatrix[i][j] *= dum;
        }
      }
    }
    _lti_debug("\nLT");
    pivot = d;
    return true;
    
#endif // if 0
    
  }
  
  /**
   * onCopy version of apply
   */
  template<typename T>
  bool luDecomposition<T>::apply(const matrix<T>& theMatrix,
                                 matrix<T>& decomposition,
                                 vector<integer>& permutation,
                                 int& pivot) const {
    decomposition.copy(theMatrix);
    return apply(decomposition,permutation,pivot);
  }

  /**
   * Builds the permutation matrix for the given permutation vector
   */
  template<typename T>
  void luDecomposition<T>::buildPermutationMatrix(const vector<integer>& pv,
                                                  matrix<T>& pm) const {
    const int n = pv.size();
    vector<integer> p(n);
    for(int i = 0; i < n; ++i){
      p.at(i) = i;
    }
    pm.resize(n,n,T(0),Init);
    for(int i = 0; i < n; ++i){
      swap(p.at(i),p.at(pv.at(i)));
    }
    for(int i = 0; i < n; ++i){
      pm.at(p.at(i),i) = T(1);
    }
  }
  
  /**
   * Copy data of "other" functor.
   */
  template<typename T>
  luDecomposition<T>& luDecomposition<T>::copy(const luDecomposition<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }
  
}
