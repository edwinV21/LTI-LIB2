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

/*
 * \file   ltiQrDecomposition_template.h
 *         Compute the QR decomposition of a matrix.
 * \author Arnd Hannemann
 * \date   26.01.2004
 *
 * $Id: ltiQrDecomposition_template.h,v 1.12 2006-09-07 13:38:37 doerfler Exp $
 */

#ifdef HAVE_LAPACK
#include "clapack.h"
#endif

#include "ltiMathLA.h"
#include "ltiClassName.h"
#include "ltiMath.h"

namespace lti {
  
  /**
   * Default constructor
   */
  template<typename T>
  qrDecomposition<T>::parameters::parameters() 
    : linearAlgebraFunctor::parameters() {
    performanceTweakThresholdForTranspose = 
      _LTI_PERFORMANCE_QR_DECOMPOSITION;
  }
  
  /**
   * Copy constructor
   */
  template<typename T>
  qrDecomposition<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }
  
  /**
   * Destructor
   */
  template<typename T>
  qrDecomposition<T>::parameters::~parameters() {
  }
  
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename qrDecomposition<T>::parameters& 
  qrDecomposition<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    performanceTweakThresholdForTranspose = 
      other.performanceTweakThresholdForTranspose;
    return *this;
  }
  
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename qrDecomposition<T>::parameters& 
  qrDecomposition<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }
  
  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& qrDecomposition<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /**
   * Returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename qrDecomposition<T>::parameters* 
  qrDecomposition<T>::parameters::clone() const {
    return new parameters(*this);
  }
  
  /**
   * Returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename qrDecomposition<T>::parameters* 
  qrDecomposition<T>::parameters::newInstance() const {
    return new parameters();
  }
  
  /**
   * write the parameters in the given ioHandler
   */
  template<typename T>
  bool qrDecomposition<T>::parameters::write(ioHandler& handler, 
                                             const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    b = b && lti::write(handler, "performanceTweakThresholdForTranspose",
                        performanceTweakThresholdForTranspose);
    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the parameters from the given ioHandler
   */
  template<typename T>
  bool qrDecomposition<T>::parameters::read(ioHandler& handler, 
                                            const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    b = b && lti::read(handler, "performanceTweakThresholdForTranspose",
                       performanceTweakThresholdForTranspose);
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  
  /*
   * Default constructor
   */
  template <typename T>
  qrDecomposition<T>::qrDecomposition() 
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , lapackInterface()
#endif
  {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  /*
   * Constructor with parameters
   */
  template <typename T>
  qrDecomposition<T>::qrDecomposition(const parameters& par)
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , lapackInterface()
#endif
  { 
    // set the given parameters
    setParameters(par);
  }

  /*
   * Copy constructor
   */
  template <typename T>
  qrDecomposition<T>::qrDecomposition(const qrDecomposition<T>& other)
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , lapackInterface()
#endif
  {
    copy(other);
  }
  
  /*
   * Destructor
   */
  template <typename T>
  qrDecomposition<T>::~qrDecomposition() {
  }

  /*
   * Copy
   */
  template <typename T>
  qrDecomposition<T>&
  qrDecomposition<T>::copy(const qrDecomposition<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  /*
   * Alias for copy
   */
  template <typename T>
  qrDecomposition<T>&
  qrDecomposition<T>::operator=(const qrDecomposition<T>& other) {
    return (copy(other));
  }
  
  /* 
   * Class name
   */
  template <typename T>
  const std::string& qrDecomposition<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /* 
   * Clone member
   */
  template <typename T>
  qrDecomposition<T>* qrDecomposition<T>::clone() const {
    return new qrDecomposition<T>(*this);
  }

  /*
   * Create new instance
   */
  template <typename T>
  qrDecomposition<T>* qrDecomposition<T>::newInstance() const {
    return new qrDecomposition<T>(*this);
  }

  /*
   * Return parameters
   */
  template <typename T>
  const typename qrDecomposition<T>::parameters&
  qrDecomposition<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(className::get(this));
    }
    return *par;
  }

  /*
   * Apply QR using column pivoting (in place)
   */
  template <typename T>
  bool qrDecomposition<T>::apply(matrix<T>& arh, 
                                 vector<T>& tau, 
                                 vector<integer>& p) const {
    
    const parameters& par = getParameters();

    if(par.useLapack){
      
#ifdef HAVE_LAPACK

      // transpose matrix for lapack use
      arh.transpose();
      
      /*  M       (input) INTEGER
       *          The number of rows of the matrix A.  M >= 0. */
      integer m = arh.columns();;
      
      /*  N       (input) INTEGER
       *          The number of columns of the matrix A.  N >= 0. */
      integer n = arh.rows();
      
      /*  LDA     (input) INTEGER
       *          The leading dimension of the array A.  LDA >= max(1,M). */
      integer lda = m;
      
      /*  JPVT    (input/output) INTEGER array, dimension (N)
       *          On entry, if JPVT(J).ne.0, the J-th column of A is permuted
       *          to the front of A*P (a leading column); if JPVT(J)=0,
       *          the J-th column of A is a free column.
       *          On exit, if JPVT(J)=K, then the J-th column of A*P was the
       *          the K-th column of A. */
      p.resize(n,0,Init);
      
      /*          (output) REAL array, dimension (min(M,N))
       *          The scalar factors of the elementary reflectors (see Further
       *          Details). */
      tau.resize(min(m,n),T(0),Init);
      
      // workspace variables
      T twork[1];
      
      /*  WORK    (workspace/output) REAL array, dimension (LWORK)
       *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */
      T* work;        
      
      /*  LWORK   (input) INTEGER
       *          The dimension of the array WORK.  LWORK >= max(1,N).
       *          For optimum performance LWORK >= N*NB, where NB is 
       *          the optimal blocksize.
       *
       *          If LWORK = -1, then a workspace query is assumed; the routine
       *          only calculates the optimal size of the WORK array, returns
       *          this value as the first entry of the WORK array, and no error
       *          message related to LWORK is issued by XERBLA. */
      integer lwork = -1;

      integer info = 0;
     
      if (arh.getMode() != lti::matrix<T>::Connected) {
        // This resize connects the matrix data
        arh.resize(n,m,Copy);
      }
      
      // ask for optimal workspace size
      geqp3(&m,&n,&arh.at(0,0),&lda,p.data(),
            tau.data(),&twork[0],&lwork,&info);
      
      lwork=static_cast<integer>(twork[0]);
      work = new T[lwork];
      // now compute qr decomposition
      geqp3(&m,&n,&arh.at(0,0),&lda,p.data(),tau.data(),&work[0],&lwork,&info);
      delete[] work;
      
      // decrease indexes to convert from Fortran
      for(int i = 0; i < n; ++i){
        p.at(i)--;
      }
    
      arh.transpose();
      
      return (info == 0);
      
#else
      
      setStatusString("LAPACK not available");
      return false;
      
#endif

    }
    else {
      // Don't use LAPACK
      return computeHouseholderQr(arh,tau,p,true);
    }

  }

  /*
   * Apply QR using column pivoting (on copy)
   */
  template <typename T>
  bool qrDecomposition<T>::apply(const matrix<T>& a, 
                                 matrix<T>& rh, 
                                 vector<T>& tau, 
                                 vector<integer>& p) const {
    rh = a;
    return apply(rh,tau,p);
  }

  /*
   * Apply QR (in place)
   */
  template <typename T>
  bool qrDecomposition<T>::apply(matrix<T>& arh, vector<T>& tau) const {
    const parameters& par = getParameters();
    if(par.useLapack){

#ifdef HAVE_LAPACK

      // transpose matrix for lapack use
      arh.transpose();
      
      /*  M       (input) INTEGER
       *          The number of rows of the matrix A.  M >= 0. */
      integer m = arh.columns();
      
      /*  N       (input) INTEGER
       *          The number of columns of the matrix A.  N >= 0. */
      integer n = arh.rows();
      
      /*  LDA     (input) INTEGER
       *          The leading dimension of the array A.  LDA >= max(1,M). */
      
      integer lda = m;
      /*  TAU     (output) REAL array, dimension (min(M,N))
       *          The scalar factors of the elementary reflectors (see Further
       *          Details). */
      tau.resize(min(m,n),T(0),Init);
      
      integer info = 0;
      
      // workspace variables
      T twork[1];
      
      /*  WORK    (workspace/output) REAL array, dimension (LWORK)
       *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */
      T* work;        
      
      /*  LWORK   (input) INTEGER
       *          The dimension of the array WORK.  LWORK >= max(1,N).
       *          For optimum performance LWORK >= N*NB, where NB is 
       *          the optimal blocksize.
       *
       *          If LWORK = -1, then a workspace query is assumed; the routine
       *          only calculates the optimal size of the WORK array, returns
       *          this value as the first entry of the WORK array, and no error
       *          message related to LWORK is issued by XERBLA. */
      integer lwork = -1;
      
      if (arh.getMode() != lti::matrix<T>::Connected) {
        // This resize connects the matrix data
        arh.resize(n,m,Copy);
      }
      
      // ask for optimal workspace size
      geqrf(&m,&n,&arh.at(0,0),&lda,tau.data(),&twork[0],&lwork,&info);
      
      lwork=static_cast<integer>(twork[0]);
      work = new T[lwork];
      // now compute qr decomposition
      geqrf(&m,&n,&arh.at(0,0),&lda,tau.data(),&work[0],&lwork,&info);
      delete[] work;
      arh.transpose();
      
      return (info == 0);
      
#else
      
      setStatusString("LAPACK not available");
      return false;
      
#endif

    }
    else {
      // Don't use LAPACK
      vector<integer> dummyPermutation(0);
      return computeHouseholderQr(arh,tau,dummyPermutation,false);
    }
    
  }

  /*
   * Apply QR (on copy)
   */
  template <typename T>
  bool qrDecomposition<T>::apply(const matrix<T>& a, 
                                 matrix<T>& rh, 
                                 vector<T>& tau) const {
    rh = a;
    return apply(rh,tau);
  }

  /*
   * Extract R from RH.
   */
  template <typename T>
  void qrDecomposition<T>::extractR(const matrix<T>& rh, matrix<T>& r) const {
    const int rows = rh.rows();
    const int cols = rh.columns();
    r.resize(rows,cols,T(0),Init);
    // construct r matrix 
    int i,j;
    for(i = 0; i < rows; ++i) {
      for(j = i; j < cols; ++j) {
        r.at(i,j) = rh.at(i,j);
      }
    } 
  }
  
  /*
   * Compute Q from RH and tau.
   */
  template <typename T>
  bool qrDecomposition<T>::computeQ(const matrix<T>& rh, 
                                    vector<T>& tau, 
                                    matrix<T>& q) const {
     
    const parameters& par = getParameters();
    
    if(par.useLapack){
      
#ifdef HAVE_LAPACK
      
      q = rh;

      // transpose matrix for lapack use
      q.transpose();

      /*  M       (input) INTEGER
       *          The number of rows of the matrix Q. M >= 0. */
      integer m = q.columns();

      /*  N       (input) INTEGER
       *          The number of columns of the matrix Q. M >= N >= 0. */
      integer n = q.rows();

      /*  K       (input) INTEGER
       *          The number of elementary reflectors whose product defines the
       *          matrix Q. N >= K >= 0. */
      integer k = n;

      /*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
       *          On entry, the i-th column must contain the vector which
       *          defines the elementary reflector H(i), for i = 1,2,...,k, as
       *          returned by DGEQRF in the first k columns of its array
       *          argument A.
       *          On exit, the M-by-N matrix Q. */
      
      /*  LDA     (input) INTEGER
       *          The first dimension of the array A. LDA >= max(1,M). */
      integer lda = m;

      /*  TAU     (input) DOUBLE PRECISION array, dimension (K)
       *          TAU(i) must contain the scalar factor of the elementary
       *          reflector H(i), as returned by DGEQRF. */
      tau.resize(k,Copy);

      /*  WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
       *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */
      T twork[1];
      T *work;

      /*  LWORK   (input) INTEGER
       *          The dimension of the array WORK. LWORK >= max(1,N).
       *          For optimum performance LWORK >= N*NB, where NB is the
       *          optimal blocksize.
       *
       *          If LWORK = -1, then a workspace query is assumed; the routine
       *          only calculates the optimal size of the WORK array, returns
       *          this value as the first entry of the WORK array, and no error
       *          message related to LWORK is issued by XERBLA. */
      integer lwork = -1;
      

      /*  INFO    (output) INTEGER
       *          = 0:  successful exit
       *          < 0:  if INFO = -i, the i-th argument has an illegal value */
      integer info = 0;

      if (q.getMode() != lti::matrix<T>::Connected) {
        // This resize connects the matrix data
        q.resize(n,m,Copy);
      }
      
      // construct q matrix
      // aks for optimal workspace size
            
      orgqr(&m,&n,&k,&q.at(0,0),&lda,tau.data(),&twork[0],&lwork,&info);
      lwork = static_cast<integer>(twork[0]);
      work = new T[lwork];
      
      // now do the work          
      orgqr(&m,&n,&k,&q.at(0,0),&lda,tau.data(),&work[0],&lwork,&info);
      delete[] work;

      q.transpose();
      
      return (info == 0);
      
#else
      
      setStatusString("LAPACK not available");
      return false;
      
#endif
      
    }
    else {
      // Don't use LAPACK

      /*
       * Computes Q from a factored form representation
       * For more details see:
       * Gene H. Golub and Charles F. Van Loan, "Matrix Computations",
       * 1996, The John Hopkins University Press, Baltimore and London
       */
      
      const int n = rh.columns();
      
      q.resize(n,n,T(0),Init);
      
      for(int i = 0; i < n; ++i){
        q.at(i,i) = T(1);
      }

      vector<T> v(n,T(0));
      matrix<T> nn(n,n,T(0));
      matrix<T> nn2(n,n,T(0));

      for(int j = n - 1; j >= 0; --j){
        
        v.at(j) = T(1);
        for(int i = j + 1; i < n; ++i){
          v.at(i) = rh.at(i,j);
        }

        for(int i = j; i < n; ++i){
          for(int k = i; k < n; ++k){
            nn.at(k,i) = nn.at(i,k) = 
              (((i == k) ? 1 : 0 ) - (tau.at(j) * v.at(i) * v.at(k))); 
          }
        }
        
        for(int r = j; r < n; ++r){
          for(int c = j; c < n; ++c){
            nn2.at(r,c) = T(0);
            for(int i = j; i < n; ++i){
              nn2.at(r,c) += (nn.at(r,i) * q.at(i,c));
            }
          }
        }
        
        for(int r = j; r < n; ++r){ 
          for(int c = j; c < n; ++c){
            q.at(r,c) = nn2.at(r,c);
          }
        }
        
      }
      
      return true;

    }
    
  }
  
  /*
   * Computes the Householder QR decomposition.
   * For more details see:
   * Gene H. Golub and Charles F. Van Loan, "Matrix Computations",
   * 1996, The John Hopkins University Press, Baltimore and London
   */
  template <typename T>
  bool qrDecomposition<T>::computeHouseholderQr(matrix<T>& arh,
                                                vector<T>& tau,
                                                vector<integer>& p,
                                                bool useColumnPivoting) const {
    const int n = arh.columns();
    const int m = arh.rows();
    matrix<T> mm(m,m);
    matrix<T> mn(m,n);
    vector<T> v(m);
    
    tau.resize(min(m,n),T(0),Init);

    vector<T> colnorms(useColumnPivoting ? n : 0,T(0));
    
    if(useColumnPivoting){
      
      p.resize(n, AllocateOnly);
      
      for(int j = 0; j < n; ++j){
        p.at(j) = j;
      }
    
      for(int j = 0; j < n; ++j){
        for(int i = 0; i < m; ++i){
          const T& a = arh.at(i,j);
          colnorms.at(j) += a * a;
        }
      }
    
    }
    
    for(int j = 0; j < n; ++j){
      
      if(useColumnPivoting){
       
        int pivot = colnorms.findIndexOfMaximum();
        
        if(j < pivot){
          swap(p.at(j),p.at(pivot));
          for(int i = 0; i < m; ++i){
            swap(arh.at(i,j),arh.at(i,pivot));
          }
          swap(colnorms.at(j),colnorms.at(pivot));
        }
        
      }
      
      v.resize(m - j, AllocateOnly);
      for(int i = j; i < m; ++i){
        v.at(i - j) = arh.at(i,j);
      }
      
      householder(v,tau.at(j));
      
      for(int i = j; i < m; ++i){
        for(int k = i; k < m; ++k){
          mm.at(k,i) = mm.at(i,k) = 
            (((i == k) ? 1 : 0 ) - (tau.at(j) * v.at(i - j) * v.at(k - j))); 
        }
      }
      
      for(int r = j; r < m; ++r){
        for(int c = j; c < n; ++c){
          mn.at(r,c) = T(0);
          for(int i = j; i < m; ++i){
            mn.at(r,c) += (mm.at(r,i) * arh.at(i,c));
          }
        }
      }
   
      for(int r = j; r < m; ++r){ 
        for(int c = j; c < n; ++c){
          arh.at(r,c) = mn.at(r,c);
        }
      }
      
      if(j < (m - 1)){
        for(int r = j + 1; r < m; ++r){
          arh.at(r,j) = v.at(r - j);
        }
      }
      
      if(useColumnPivoting){
       
        for(int i = j; i < n; ++i){
          const T& a = arh.at(j,i);
          colnorms.at(i) -= a * a;
        }
        
      }   
   
    }
    
    return true;
  
  }

  template <typename T>
  void qrDecomposition<T>::buildPermutationMatrix(const vector<integer>& pv, 
                                                  matrix<T>& pm) const {
    const int n = pv.size();
    pm.resize(n,n,T(0),Init);
    for(int i = 0; i < n; ++i){
      pm.at(i,pv.at(i)) = T(1);
    }
  }

}

#include "ltiUndebug.h"
