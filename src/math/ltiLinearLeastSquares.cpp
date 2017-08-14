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
 * \file   ltiLinearLeastSquares.cpp
 *         Contains the class lti::linearLeastSquares,
 *         to solve a least squares linear problem.
 *
 * \author Pablo Alvarado
 * \date   16.01.2008
 *
 * revisions ..: $Id: ltiLinearLeastSquares.cpp,v 1.2 2009-04-08 04:25:16 alvarado Exp $
 */


#include "ltiConfig.h"
#include "ltiMath.h"
#include "ltiRound.h"

#ifdef HAVE_LAPACK
#include "clapack.h"
#endif

#undef min
#undef max

#include "ltiLinearLeastSquares.h"
#include "ltiSVD.h"
#include <limits>



namespace lti {

#ifdef HAVE_LAPACK
  // LAPACK Routines as templates
  template<>
  int linearLeastSquares::helper<float>::gels(char *trans, integer *m,
                                              integer *n, integer *nrhs, 
                                              float* a, integer *lda,
                                              float* b, integer* ldb,
                                              float* work, integer *lwork,
                                              integer *info) const {
    lockInterface();
    int tmp=LA_SGELS(trans,m,n,nrhs,a,lda,b,ldb,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int linearLeastSquares::helper<float>::gelsy(integer *m, integer *n, 
                                               integer *nrhs,
                                               float* a, integer *lda, 
                                               float* b, integer *ldb, 
                                               integer *jpvt, 
                                               float* rcond, integer *rank,
                                               float* work,
                                               integer *lwork,
                                               integer *info) const {
    lockInterface();
    int tmp=LA_SGELSY(m,n,nrhs,a,lda,b,ldb,jpvt,rcond,rank,work,lwork,info);
    unlockInterface();
    return tmp;
  }


  template<>
  int linearLeastSquares::helper<float>::gelss(integer *m, integer *n,
                                               integer *nrhs,
                                               float* a, integer *lda,
                                               float* b, integer *ldb,
                                               float* s, float* rcond,
                                               integer *rank, float* work,
                                               integer *lwork,
                                               integer *info) const {
    lockInterface();
    int tmp=LA_SGELSS(m,n,nrhs,a,lda,b,ldb,s,rcond,rank,work,lwork,info);
    unlockInterface();
    return tmp;
  }


  template<>
  int linearLeastSquares::helper<float>::gelsd(integer *m, integer *n,
                                               integer *nrhs,
                                               float *a, integer *lda,
                                               float *b, integer *ldb, 
                                               float *s, float *rcond,
                                               integer *rank, float *work,
                                               integer *lwork, integer *iwork, 
                                               integer *info) const {
    lockInterface();
    int tmp=LA_SGELSD(m,n,nrhs,a,lda,b,ldb,s,rcond,rank,work,lwork,iwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int linearLeastSquares::helper<double>::gels(char *trans, integer *m,
                                               integer *n, integer *nrhs,
                                               double* a, integer *lda,
                                               double* b, integer* ldb,
                                               double* work, integer *lwork,
                                               integer *info) const {
    lockInterface();
    int tmp=LA_DGELS(trans,m,n,nrhs,a,lda,b,ldb,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int linearLeastSquares::helper<double>::gelsy(integer *m, integer *n, 
                                                integer *nrhs,
                                                double* a, integer *lda,
                                                double* b, integer *ldb,
                                                integer *jpvt, 
                                                double* rcond, integer *rank,
                                                double* work, integer *lwork,
                                                integer *info) const {
    lockInterface();
    int tmp=LA_DGELSY(m,n,nrhs,a,lda,b,ldb,jpvt,rcond,rank,work,lwork,info);
    unlockInterface();
    return tmp;
  }


  template<>
  int linearLeastSquares::helper<double>::gelss(integer *m, integer *n,
                                                integer *nrhs, 
                                                double* a, integer *lda,
                                                double* b, integer *ldb,
                                                double* s, double* rcond,
                                                integer *rank,
                                                double* work, integer *lwork,
                                                integer *info) const {
    lockInterface();
    int tmp=LA_DGELSS(m,n,nrhs,a,lda,b,ldb,s,rcond,rank,work,lwork,info);
    unlockInterface();
    return tmp;
  }


  template<>
  int linearLeastSquares::helper<double>::gelsd(integer *m, integer *n,
                                                integer *nrhs,
                                                double *a, integer *lda,
                                                double *b, integer *ldb,
                                                double *s, double *rcond,
                                                integer *rank,
                                                double *work, integer *lwork,
                                                integer *iwork, 
                                                integer *info) const {
    lockInterface();
    int tmp=LA_DGELSD(m,n,nrhs,a,lda,b,ldb,s,rcond,rank,work,lwork,iwork,info);
    unlockInterface();
    return tmp;
  }

#endif

  // --------------------------------------------------
  // linearLeastSquares::helper
  // --------------------------------------------------

  template<typename T>
  bool
  linearLeastSquares::helper<T>::solve(const parameters& pars,
                                       const matrix<T>& A,
                                       const vector<T>& b,
                                       vector<T>& x) const {

    if (A.empty() || (b.size() != A.rows())) {
      return false;
    }
#ifdef HAVE_LAPACK
    if (pars.useLapack) {
      // common variables to all algorithms
      
      // m number of rows
      integer m = A.rows();
      // n number of columns
      integer n = A.columns();
      
      // number of right hand sides:
      integer nrhs = 1;
      
      // we need a copy since it will be modified
      matrix<T> aCpy;
      aCpy.transpose(A);
      
      // On entry, the M-by-N matrix A.  On exit, if M >= N, A is over- written
      // by details of its QR factorization as returned by SGE-QRF;
      // if  M <  N, A is overwritten by details of its LQ factorization as
      // returned by SGELQF.
      T* a = &aCpy.elem(0);
      
      // lda The leading dimension of the array A.  LDA >= max(1,M).
      integer lda = m;
      
      // b On entry, the matrix B of right hand side vectors, stored
      // columnwise; B is M-by-NRHS if TRANS = 'N', or N-by-NRHS if TRANS =
      // 'T'.  On exit, if INFO = 0, B is overwritten by the solution vectors,
      // stored columnwise: if TRANS = 'N' and m >= n, rows 1 to n of B contain
      // the least squares solution vectors; the residual sum of squares for
      // the solution in each column is given by the sum of squares of elements
      // N+1 to M in that col- umn; if TRANS = 'N' and m < n, rows 1 to N of B
      // contain the minimum norm solution vectors

      // since the vector is modified copy it directly into the output vector
      x.copy(b);
      
      // The leading dimension of the array B. LDB >= MAX(1,M,N).
      integer ldb = max(integer(1),m,n);
      
      // temporal workspace to obtain optimal workspace
      T twork[1];
      twork[0]=0;
      integer tiwork[1];
      
      //  LWORK   (input) INTEGER
      //          The dimension of the array WORK.  LWORK >= max(1,N).
      //          For optimum performance LWORK >= N*NB, where NB is
      //          the optimal blocksize.
      //
      //          If LWORK = -1, then a workspace query is assumed; the routine
      //          only calculates the optimal size of the WORK array, returns
      //          this value as the first entry of the WORK array, and no error
      //          message related to LWORK is issued by XERBLA. */
      integer lwork = -1;
      
      // workspace: array, dimension (MAX(1,LWORK))
      // On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
      integer info=0;
      
      vector<integer> jpvt(A.columns(),integer(0));
      integer rank=0;
      T rcond = static_cast<T>(pars.rCondition);
      T* singVals = 0;
      // query each method for the optimal workspace
      switch(pars.algorithm) {
        case QR: {
          char trans = 'N';
          gels(&trans,&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,twork,&lwork,&info);
        } break;
        case CompleteFactorization:
          gelsy(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,jpvt.data(),&rcond,&rank,
                twork,&lwork,&info);
          break;
        case SVD:
          singVals = new T[A.columns()];
          gelss(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                twork, &lwork, &info);
          
          break;
        case DCSVD: {
          // it seems that the tiwork is sometimes not properly set, so
          // do some computations of our own...
          const int minnm = min(m,n);
          static double log2 = log(2.0);

          tiwork[0]=max(1,iround(3*minnm*(log(minnm)/log2) + 11*minnm));
          singVals = new T[A.columns()];
          gelsd(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                twork, &lwork, tiwork, &info);
        } break;
        default:
          return false;
      }
      
      lwork = static_cast<integer>(max(T(1),twork[0]));
      T* work = new T[lwork];
      
      // now do the real job
      switch(pars.algorithm) {
        case QR: {
          char trans = 'N';
          gels(&trans,&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,work,&lwork,&info);
        } break;
        case CompleteFactorization:
          gelsy(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,jpvt.data(),&rcond,&rank,
                work,&lwork,&info);        
          break;
        case SVD:
          gelss(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                work, &lwork, &info);
          delete[] singVals;
          singVals = 0;
          break;
        case DCSVD: {
          const int liwork = max(1,static_cast<int>(tiwork[0]));          
          integer* iwork = new integer[liwork];
          gelsd(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                work, &lwork, iwork ,&info);
          delete[] singVals;
          singVals = 0;
          delete[] iwork;
          iwork = 0;
        } break;
        default:
          return false;
      }
      
      delete[] work;
      
      // resize to the proper number of output variables, and in case it was
      // underdetermined, use zeros.
      x.resize(A.columns(),T(0));
      
      return (info==0);
    }
#endif

    // if there is no LAPACK, use SVD as a safe replacement
    svd::parameters svdPar;
    svdPar.sort = true;
    svdPar.transposeU = true;
    svdPar.transposeV = false;
    svdPar.useDivideAndConquer = true;
    svdPar.useMinDimensions = true;
    svd svdFunctor(svdPar);

    matrix<T> Q;
    matrix<T> Ut,V;
    vector<T> W,vtmp;
    if (svdFunctor.apply(A,Ut,W,V)) {
      const T maxVal = W.at(0)*static_cast<T>(pars.rCondition);

      // solve the LSE with V W^-1 U' b
      matrix<T> mtmp(V.rows(),Ut.columns());
      for (int i=0;i<A.columns();++i) {
        if (W.at(i)>=maxVal) {
          mtmp.getRow(i).multiply(Ut.getRow(i),1.0f/W.at(i));
        } else {
          mtmp.getRow(i).fill(T(0));
        }
      }

      Q.multiply(V,mtmp);
      Q.multiply(b,x);
      
      return true;
    }
    
    return false;
  }

  template<typename T>
  bool
  linearLeastSquares::helper<T>::solve(const parameters& pars,
                                       const matrix<T>& A,
                                       const matrix<T>& b,
                                       matrix<T>& x) const {
    if (A.empty() || (b.rows() != A.rows())) {
      return false;
    }
#ifdef HAVE_LAPACK
    if (pars.useLapack) {
      // common variables to all algorithms
      
      // m number of rows
      integer m = A.rows();
      // n number of columns
      integer n = A.columns();
      
      // number of right hand sides:
      integer nrhs = b.columns();
      
      // we need a copy since it will be modified
      matrix<T> aCpy;
      aCpy.transpose(A);
      
      // On entry, the M-by-N matrix A.  On exit, if M >= N, A is over- written
      // by details of its QR factorization as returned by SGE-QRF;
      // if  M <  N, A is overwritten by details of its LQ factorization as
      // returned by SGELQF.
      T* a = &aCpy.elem(0);
      
      // lda The leading dimension of the array A.  LDA >= max(1,M).
      integer lda = m;
      
      // b On entry, the matrix B of right hand side vectors, stored
      // columnwise; B is M-by-NRHS if TRANS = 'N', or N-by-NRHS if TRANS =
      // 'T'.  On exit, if INFO = 0, B is overwritten by the solution vectors,
      // stored columnwise: if TRANS = 'N' and m >= n, rows 1 to n of B contain
      // the least squares solution vectors; the residual sum of squares for
      // the solution in each column is given by the sum of squares of elements
      // N+1 to M in that col- umn; if TRANS = 'N' and m < n, rows 1 to N of B
      // contain the minimum norm solution vectors

      // since the vector is modified copy it directly into the output vector
      x.transpose(b);
      
      // The leading dimension of the array B. LDB >= MAX(1,M,N).
      integer ldb = max(integer(1),m,n);
      
      // temporal workspace to obtain optimal workspace
      T twork[1];
      twork[0]=0;
      integer tiwork[1];
      
      //  LWORK   (input) INTEGER
      //          The dimension of the array WORK.  LWORK >= max(1,N).
      //          For optimum performance LWORK >= N*NB, where NB is
      //          the optimal blocksize.
      //
      //          If LWORK = -1, then a workspace query is assumed; the routine
      //          only calculates the optimal size of the WORK array, returns
      //          this value as the first entry of the WORK array, and no error
      //          message related to LWORK is issued by XERBLA. */
      integer lwork = -1;
      
      // workspace: array, dimension (MAX(1,LWORK))
      // On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
      integer info=0;
      
      vector<integer> jpvt(A.columns(),integer(0));
      integer rank=0;
      T rcond = static_cast<T>(pars.rCondition);
      T* singVals = 0;
      // query each method for the optimal workspace
      switch(pars.algorithm) {
        case QR: {
          char trans = 'N';
          gels(&trans,&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,twork,&lwork,&info);
        } break;
        case CompleteFactorization:
          gelsy(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,jpvt.data(),&rcond,&rank,
                twork,&lwork,&info);
          break;
        case SVD:
          singVals = new T[A.columns()];
          gelss(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                twork, &lwork, &info);
          
          break;
        case DCSVD: {
          // it seems that the tiwork is sometimes not properly set, so
          // do some computations of our own...
          const int minnm = min(m,n);
          static double log2 = log(2.0);

          tiwork[0]=max(1,iround(3*minnm*(log(minnm)/log2) + 11*minnm));
          singVals = new T[A.columns()];
          gelsd(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                twork, &lwork, tiwork, &info);
        } break;
        default:
          return false;
      }
      
      lwork = static_cast<integer>(max(T(1),twork[0]));
      T* work = new T[lwork];
      
      // now do the real job
      switch(pars.algorithm) {
        case QR: {
          char trans = 'N';
          gels(&trans,&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,work,&lwork,&info);
        } break;
        case CompleteFactorization:
          gelsy(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,jpvt.data(),&rcond,&rank,
                work,&lwork,&info);        
          break;
        case SVD:
          gelss(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                work, &lwork, &info);
          delete[] singVals;
          singVals = 0;
          break;
        case DCSVD: {
          const int liwork = max(1,static_cast<int>(tiwork[0]));          
          integer* iwork = new integer[liwork];
          gelsd(&m,&n,&nrhs,a,&lda,&x.elem(0),&ldb,singVals,&rcond,&rank,
                work, &lwork, iwork ,&info);
          delete[] singVals;
          singVals = 0;
          delete[] iwork;
          iwork = 0;
        } break;
        default:
          return false;
      }
      
      delete[] work;
      
      // resize to the proper number of output variables, and in case it was
      // underdetermined, use zeros.
      x.transpose();
      x.resize(A.columns(),x.columns(),T(0));
      
      return (info==0);
    }
#endif

    // if there is no LAPACK, use SVD as a safe replacement
    typename svd::parameters svdPar;
    svdPar.sort = true;
    svdPar.transposeU = true;
    svdPar.transposeV = false;
    svdPar.useDivideAndConquer = true;
    svdPar.useMinDimensions = true;
    svd svdFunctor(svdPar);

    matrix<T> Q;
    matrix<T> Ut,V;
    vector<T> W,vtmp;
    if (svdFunctor.apply(A,Ut,W,V)) {
      const T maxVal = W.at(0)*static_cast<T>(pars.rCondition);

      // solve the LSE with V W^-1 U' b
      matrix<T> mtmp(V.rows(),Ut.columns());
      for (int i=0;i<A.columns();++i) {
        if (W.at(i)>=maxVal) {
          mtmp.getRow(i).multiply(Ut.getRow(i),1.0f/W.at(i));
        } else {
          mtmp.getRow(i).fill(T(0));
        }
      }

      Q.multiply(V,mtmp);

      // we need to compute col by col the product
      
      x.resize(b.columns(),A.columns());
      matrix<T> bt;
      bt.transpose(b);
      
      for (int i=0;i<bt.rows();++i) {
        Q.multiply(bt.getRow(i),x.getRow(i));
      }
      x.transpose();

      return true;
    }
    
    return false;
  }

  // --------------------------------------------------
  // linearLeastSquares::parameters
  // --------------------------------------------------

  // default constructor
  linearLeastSquares::parameters::parameters()
    : linearAlgebraFunctor::parameters() {

    algorithm = QR;
    rCondition = 0.0;
  }

  // copy constructor
  linearLeastSquares::parameters::parameters(const parameters& other)
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  // destructor
  linearLeastSquares::parameters::~parameters() {
  }

  // copy member

  linearLeastSquares::parameters&
  linearLeastSquares::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);


    algorithm = other.algorithm;
    rCondition = other.rCondition;

    return *this;
  }

  // alias for copy method
  linearLeastSquares::parameters&
  linearLeastSquares::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& linearLeastSquares::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  linearLeastSquares::parameters*
  linearLeastSquares::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  linearLeastSquares::parameters*
  linearLeastSquares::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool linearLeastSquares::parameters::write(ioHandler& handler,
                                             const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"algorithm",algorithm);
      lti::write(handler,"rCondition",rCondition);
    }

    b = b && linearAlgebraFunctor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool linearLeastSquares::parameters::read(ioHandler& handler,
                                            const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"algorithm",algorithm);
      lti::read(handler,"rCondition",rCondition);
    }

    b = b && linearAlgebraFunctor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // linearLeastSquares
  // --------------------------------------------------

  // default constructor
  linearLeastSquares::linearLeastSquares()
    : linearAlgebraFunctor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  linearLeastSquares::linearLeastSquares(const parameters& par)
    : linearAlgebraFunctor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  linearLeastSquares::linearLeastSquares(const linearLeastSquares& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  linearLeastSquares::~linearLeastSquares() {
  }

  // copy member
  linearLeastSquares&
  linearLeastSquares::copy(const linearLeastSquares& other) {
    linearAlgebraFunctor::copy(other);

    return (*this);
  }

  // alias for copy member
  linearLeastSquares&
  linearLeastSquares::operator=(const linearLeastSquares& other) {
    return (copy(other));
  }

  // class name
  const std::string& linearLeastSquares::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  linearLeastSquares* linearLeastSquares::clone() const {
    return new linearLeastSquares(*this);
  }

  // create a new instance
  linearLeastSquares* linearLeastSquares::newInstance() const {
    return new linearLeastSquares();
  }

  // return parameters
  const linearLeastSquares::parameters&
  linearLeastSquares::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&linearAlgebraFunctor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  /*
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
  bool linearLeastSquares::apply(const fmatrix& A,
                                 const fvector& b,
                                 fvector& x) const {
    return fhelper_.solve(getParameters(),A,b,x);
  }

  /*
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
  bool linearLeastSquares::apply(const dmatrix& A,
                                 const dvector& b,
                                 dvector& x) const {
    return dhelper_.solve(getParameters(),A,b,x);
  }

  /*
   * Find the solution \f$x\f$ for
   * \f[
   * A x = b
   * \f]
   *
   * @param A matrix A
   * @param b matrix b where each column is considered separatelly 
   * @param x matrix x where the solution is stored for each column of b.
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
  bool linearLeastSquares::apply(const fmatrix& A,
                                 const fmatrix& b,
                                 fmatrix& x) const {
    return fhelper_.solve(getParameters(),A,b,x);
  }

  /*
   * Find the solution \f$x\f$ for
   * \f[
   * A x = b
   * \f]
   *
   * @param A matrix A
   * @param b matrix b where each column is considered separatelly 
   * @param x matrix x where the solution is stored for each column of b.
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
  bool linearLeastSquares::apply(const dmatrix& A,
                                 const dmatrix& b,
                                 dmatrix& x) const {
    return dhelper_.solve(getParameters(),A,b,x);
  }

  /**
   * Read a linearLeastSquares::eAlgorithm
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            linearLeastSquares::eAlgorithm& data) {
    std::string str;

    if (handler.read(str)) {

      if (str.find("omplet") != std::string::npos) {
        data = linearLeastSquares::CompleteFactorization;
      } else if (str.find("QR") != std::string::npos) {
        data = linearLeastSquares::QR;
      } else if (str.find("DCSVD") != std::string::npos) {
        data = linearLeastSquares::DCSVD;
      } else if (str.find("SVD") != std::string::npos) {
        data = linearLeastSquares::SVD;
      } else {
        data = linearLeastSquares::SVD;
        handler.setStatusString("Unknown linearLeastSquares::eAlgorithm");
        return false;
      }

      return true;
    }

    return false;
  }

  /**
   * Write a linearLeastSquares::eAlgorithm
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const linearLeastSquares::eAlgorithm& data) {

    bool b = false;
    switch(data) {
      case linearLeastSquares::CompleteFactorization:
        b = handler.write("CompleteFactorization");
        break;
      case linearLeastSquares::QR:
        b = handler.write("QR");
        break;
      case linearLeastSquares::SVD:
        b = handler.write("SVD");
        break;
      case linearLeastSquares::DCSVD:
        b = handler.write("DCSVD");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("Undefined linearLeastSquares::eAlgorithm");
        break;
    }

    return b;
  }

}

