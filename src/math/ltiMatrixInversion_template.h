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

/*
 * \file   ltiMatrixInversion_template.h
 *         Inversion of matrices.
 * \author Peter Doerfler
 * \date   02.08.2003
 *
 * $Id: ltiMatrixInversion_template.h,v 1.15 2012-11-17 14:49:05 alvarado Exp $
 */

#include <limits>
#include "ltiLuSolution.h"
#include "ltiSingularValueDecomposition.h"
#include "ltiMath.h"
#include "ltiIncompatibleDimensionsException.h"

#ifdef HAVE_LAPACK
# include "clapack.h"
# include "ltiLuDecomposition.h"
#endif

#undef min

namespace lti {

#ifdef HAVE_LAPACK
  template<>
  int matrixInversion<float>::getri(integer* n, float* a, integer* lda,
                                    integer *ipiv,
                                    float* work, integer* lwork,
                                    integer* info) const {
    lockInterface();
    int tmp=LA_SGETRI(n,a,lda,ipiv,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int matrixInversion<double>::getri(integer* n, double* a, integer* lda,
                                     integer *ipiv,
                                     double* work, integer* lwork,
                                     integer* info) const {
    lockInterface();
    int tmp=LA_DGETRI(n,a,lda,ipiv,work,lwork,info);
    unlockInterface();
    return tmp;
  }
#endif

  /* matrixInversion */
  
  /**
   * Use Sqrt((std::numeric_limits<double>::min)()) as epsilon value
   */
  template <typename T>
  const T matrixInversion<T>::epsilon = sqrt(std::numeric_limits<T>::min());

  /**
   * Default constructor
   */
  template<typename T> 
  matrixInversion<T>::parameters::parameters() 
    : linearAlgebraFunctor::parameters() {
    method=LUD;
  }
      
  /**
   * Copy constructor
   */
  template<typename T>
  matrixInversion<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  /**
   * Destructor
   */
  template<typename T>
  matrixInversion<T>::parameters::~parameters() {}

  /**
   * copy member
   */
  template<typename T>
  typename matrixInversion<T>::parameters& 
  matrixInversion<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    
    method=other.method;
    return *this;
  }

  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& matrixInversion<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /**
   * Return a cloned instance of this one.
   */
  template<typename T>
  typename matrixInversion<T>::parameters*
  matrixInversion<T>::parameters::clone() const {
    return new parameters(*this);
  }

  /**
   * Return a cloned instance of this one.
   */
  template<typename T>
  typename matrixInversion<T>::parameters*
  matrixInversion<T>::parameters::newInstance() const {
    return new parameters();
  }

  /**
   * Write the parameters in the given ioHandler
   */
  template<typename T>
  bool matrixInversion<T>::parameters::write(ioHandler& handler, const bool complete) const {
    
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      if (method == LUD) {
        lti::write(handler,"method","LUD");
      } else {
        lti::write(handler,"method","SVD");
      }
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }
      
  /**
   * Read the parameters from the given ioHandler
   */
  template<typename T>
  bool matrixInversion<T>::parameters::read(ioHandler& handler, const bool complete) {
        
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      std::string str;
      lti::read(handler,"method",str);
      if (str == "LUD") {
        method = LUD;
      } else {
        method = SVD;
      }
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  
  /**
   * Constructor
   */
  template<typename T>
  matrixInversion<T>::matrixInversion() 
    : linearAlgebraFunctor() 
#ifdef HAVE_LAPACK
      , internal::luDecompositionLapackInterface<T>()
#endif
  {
    parameters tmp;
    setParameters(tmp);
  }

  /**
   * Constructor
   */
  template<typename T>
  matrixInversion<T>::matrixInversion(const parameters& par) 
    : linearAlgebraFunctor() 
#ifdef HAVE_LAPACK
      , internal::luDecompositionLapackInterface<T>()
#endif
  {
    setParameters(par);
  }


  /**
   * Destructor
   */
  template<typename T>
  matrixInversion<T>::~matrixInversion() {
  }

  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& matrixInversion<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /**
   * Returns a pointer to a clone of the functor.
   */
  template<typename T>
  matrixInversion<T>* matrixInversion<T>::clone() const {
    return (new matrixInversion<T>(*this));
  }
  
  /**
   * Returns a pointer to a new instance of the functor.
   */
  template<typename T>
  matrixInversion<T>* matrixInversion<T>::newInstance() const {
    return new matrixInversion<T>();
  }

  /**
   * onCopy version of apply.
   */
  template<typename T>
  bool matrixInversion<T>::apply(const matrix<T>& theMatrix,
                                 matrix<T>& theInverse) const {

    const parameters& p = getParameters();
    if(p.method == LUD){
      return ludMethod(theMatrix,theInverse);
    } else {
      return svdMethod(theMatrix,theInverse);
    }
  }

  /**
   * onCopy version of apply.
   */
  template<typename T>
  bool matrixInversion<T>::svdMethod(const matrix<T>& theMatrix,
                                     matrix<T>& theInverse) const {

    typename singularValueDecomposition<T>::parameters svdPar;
    svdPar.transposeU = true;
    svdPar.transposeV = false;
    singularValueDecomposition<T> svd(svdPar);
    
    matrix<T> u;
    vector<T> w;
    
    if (svd.apply(theMatrix, u,w,theInverse)) {
      
      for (int i=0; i<u.rows(); i++) {
        T wi= (i < w.size()) ? w.at(i) : T(0);

        if (abs(wi) < epsilon) {
          wi= T(0);
        } else {
          wi=T(1)/wi;
        }
        u.getRow(i).multiply(wi);
      }
      theInverse.multiply(u);
      return true;
    } else {
      setStatusString(svd.getStatusString());
    }

    return false;
  }


  /**
   * onCopy version of apply.
   */
  template<typename T>
  bool matrixInversion<T>::ludMethod(const matrix<T>& theMatrix,
                                     matrix<T>& theInverse) const {

#ifdef HAVE_LAPACK
    /*  N       (input) INTEGER
     *          The order of the matrix A.  N >= 0. */
    integer n = theMatrix.rows();
    
    /*  LDA     (input) INTEGER
     *          The leading dimension of the array A.  LDA >= max(1,M). */
    integer lda = n;
    
    /*  IPIV    (input) INTEGER array, dimension (N)
     *          The pivot indices from SGETRF; for 1<=i<=N, row i of the
     *          matrix was interchanged with row IPIV(i). */
    vector<integer> ipiv(n);

    integer info=0;
    
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
    
    theInverse.transpose(theMatrix);

    // computes the LU decomposition of the input matrix, since it is required
    // for 'getri'
    this->getrf(&n, &n, &theInverse.at(0,0), &lda, ipiv.data(), &info);

    if(info != 0){
      return false;
    }

    // ask for optimal workspace size
    this->getri(&n,&theInverse.at(0,0), &lda, ipiv.data(),
		&twork[0], &lwork, &info);

    lwork=static_cast<integer>(twork[0]);
    work = new T[lwork];
    // now compute the inversion
    getri(&n,&theInverse.at(0,0), &lda, ipiv.data(), &work[0], &lwork, &info);
    delete[] work;
    
    theInverse.transpose();

    return info == 0;

#else

    const int n=theMatrix.rows();
    if(n!=theMatrix.columns()) throw incompatibleDimensionsException();
    vector<T> unitVector(n,0.0);

    luSolution<T> eqs;
    typename luSolution<T>::parameters luSolPar;
    // do not copy (theMatrix could be huge!)
    // const_cast valid since luDecomp and luSol don't change
    // the SystemMatrix
    luSolPar.systemMatrix.useExternData(theMatrix.rows(),
                                        theMatrix.columns(),
                                        const_cast<T*>(&theMatrix.at(0,0)));
    
    eqs.setParameters(luSolPar);

    theInverse.resize(n,n);
    int i;
    for(i=0;i<n-1;i++) {
      unitVector[i]=1;
      eqs.apply(unitVector,theInverse[i]);
      unitVector[i]=0;
    }
    unitVector[i]=1;
    eqs.apply(unitVector,theInverse[i]);
    
    theInverse.transpose();
    
    return true;
 
#endif
    
  }


  /**
   * onPlace version of apply.
   */
  template<typename T>
  bool matrixInversion<T>::apply(matrix<T>& theMatrix) const
  {
    matrix<T> tmp;
    bool r=apply(theMatrix,tmp);
    tmp.detach(theMatrix);
    return r;
  }

  template<typename T>
  void matrixInversion<T>::useLUD() {
    parameters t;
    t.method=LUD;
    setParameters(t);
  }

  template<typename T>
  void matrixInversion<T>::useSVD() {
    parameters t;
    t.method=SVD;
    setParameters(t);
  }

  template <typename T>
  const typename matrixInversion<T>::parameters&
  matrixInversion<T>::getParameters() const {
    return
      dynamic_cast<const parameters&>(linearAlgebraFunctor::getParameters());
  }

}
