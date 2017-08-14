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
 * \file   ltiSymmetricMatrixInversion_template.h
 *         Inversion of symmetric matrices.
 * \author Peter Doerfler
 * \date   02.08.2003
 *
 * $Id: ltiSymmetricMatrixInversion_template.h,v 1.14 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiSymmetricMatrixInversion.h"
#include "ltiClassName.h"

#include <limits>

#include "ltiDebug.h"

namespace lti {

  /* symmetricMatrixInversion */

  /**
   * default constructor
   */
  template<typename T>
  symmetricMatrixInversion<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters() {
  }

  /**
   * Copy constructor
   */
  template<typename T>
  symmetricMatrixInversion<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }
      
  /**
   * Destructor
   */
  template<typename T>
  symmetricMatrixInversion<T>::parameters::~parameters() {
  }
      
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename symmetricMatrixInversion<T>::parameters& 
  symmetricMatrixInversion<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    
    return *this;
  }
  
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename symmetricMatrixInversion<T>::parameters& 
  symmetricMatrixInversion<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }
  
  /**
   * Return the name of the class 
   */
  template<typename T>
  const std::string& symmetricMatrixInversion<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /**
   * Returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename symmetricMatrixInversion<T>::parameters* 
  symmetricMatrixInversion<T>::parameters::clone() const {
    return new parameters(*this);
  }
  
  /**
   * Returns a pointer to a new instance of the parameters
   */
  template<typename T>
  typename symmetricMatrixInversion<T>::parameters* 
  symmetricMatrixInversion<T>::parameters::newInstance() const {
    return new parameters();
  }

  /**
   * Write the parameters in the given ioHandler
   */
  template<typename T>
  bool symmetricMatrixInversion<T>::parameters::write(ioHandler& handler, const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      
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
  bool symmetricMatrixInversion<T>::parameters::read(ioHandler& handler, const bool complete){
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      
    }

    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  /*
   * Default constructor
   */
  template<class T>
  symmetricMatrixInversion<T>::symmetricMatrixInversion()
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
      , chol() {
    
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
    
  }

  /*
   * constructor
   */
  template<class T>
  symmetricMatrixInversion<T>::symmetricMatrixInversion(const parameters& par)
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
      , chol() {
    // set the given parameters
    setParameters(par);
  }

  /*
   * Copy constructor
   */
  template<class T>
  symmetricMatrixInversion<T>::symmetricMatrixInversion(const symmetricMatrixInversion<T>& other)
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
      , chol() {
    copy(other);
  }

  /*
   * Destructor
   */
  template<class T>
  symmetricMatrixInversion<T>::~symmetricMatrixInversion() {
  }

  /*
   * copy member
   */
  template<class T>
  symmetricMatrixInversion<T>&
  symmetricMatrixInversion<T>::copy(const symmetricMatrixInversion<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  /*
   * Alias for copy member
   */
  template<class T>
  symmetricMatrixInversion<T>& symmetricMatrixInversion<T>::
  operator=(const symmetricMatrixInversion<T>& other) {
    return (copy(other));
  }

  /*
   * class name
   */
  template<class T>
  const std::string& symmetricMatrixInversion<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /* 
   * clone member
   */
  template<class T>
  linearAlgebraFunctor* symmetricMatrixInversion<T>::clone() const {
    return new symmetricMatrixInversion<T>(*this);
  }

  /* 
   * new instance member
   */
  template<class T>
  linearAlgebraFunctor* symmetricMatrixInversion<T>::newInstance() const {
    return new symmetricMatrixInversion<T>();
  }

  /*
   * Return parameters
   */
  template<class T>
  const typename symmetricMatrixInversion<T>::parameters&
  symmetricMatrixInversion<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(className::get(this));
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  /*
   * On place apply for type matrix<T>!
   */
  template <class T>
  bool symmetricMatrixInversion<T>::apply(matrix<T>& srcdest) const {
    matrix<T> inv;
    bool b=apply(srcdest, inv);
    inv.detach(srcdest);
    return b;
  }

  /*
   * On copy apply for type matrix<T>!
   */
  template <class T>
  bool symmetricMatrixInversion<T>::apply(const matrix<T>& src,matrix<T>& dest) const {
    
    assert(src.rows()==src.columns());

    if (src.empty()) {
      dest.clear();
      setStatusString("Empty src matrix in symmetricMatrixInversion\n");
      return false;
    }

#ifdef HAVE_LAPACK

    /*  UPLO    (input) CHARACTER*1
     *          = 'U':  Upper triangle of A is stored;
     *          = 'L':  Lower triangle of A is stored.
     */

    char uplo = 'U';

    /*  N       (input) INTEGER
     *          The order of the matrix A.  N >= 0.
     */

    integer n = src.rows();

    /*  A       (input/output) REAL array, dimension (LDA,N)
     *          On entry, the triangular factor U or L from the Cholesky
     *          factorization A = U**T*U or A = L*L**T, as computed by
     *          SPOTRF.
     *          On exit, the upper or lower triangle of the (symmetric)
     *          inverse of A, overwriting the input factor U or L.
     */

    /*  LDA     (input) INTEGER
     *          The leading dimension of the array A.  LDA >= max(1,N).
     */

    integer lda = n;

    /*  INFO    (output) INTEGER
     *          = 0:  successful exit
     *          < 0:  if INFO = -i, the i-th argument had an illegal value
     *          > 0:  if INFO = i, the (i,i) element of the factor U or L is
     *                zero, and the inverse could not be computed.
     */

    integer info = 0;

    choleskyDecomposition<T> chd;

    if(!chd.apply(src,dest)){
      setStatusString("Error in choleskyDecomposition used in symmetricMatrixInversion:\n");
      appendStatusString(chd);
      return false;
    }
    
    dest.transpose();

    potri(&uplo, &n, &dest.at(0,0), &lda, &info);

    dest.transpose();

    return info == 0;

#else

    int sz=src.rows();

    bool b=false;

    switch(sz) {
      case 1: {
        const T tmp=src.at(0,0);
        if (tmp < std::numeric_limits<T>::epsilon()) {
          setStatusString("Matrix is singular\n");
          dest.clear();
          b = false;
        } else {
          dest.at(0,0)=T(1)/tmp;
        }
        break;
      }
      case 2:
        b = invert2(src, dest);
        break;
      case 3:
        b = invert3(src, dest);
        break;
      case 4:
        b = invert4(src, dest);
        break;
      default:
        b = choleskyInversion(src, dest);
        break;
    }
    
    return b;
    
#endif
    
  }

  template <class T>
  bool symmetricMatrixInversion<T>::choleskyInversion(const matrix<T>& m,
                                                      matrix<T>& inv) const {
    bool b=true;
    int i,j,k;

    // threshold for singularity is sqrt(epsilon()) suggested by Michael Baus
    // see 
    // https://sourceforge.net/tracker/index.php?func=detail&aid=1019521&group_id=45767&atid=444017
    // Actually it seems like the nth root of epsilon with n=m.rows() should
    // be used to get the same behaviour as for 2-4 dimensions.
    static const T threshold=sqrt(std::numeric_limits<T>::epsilon());

    matrix<T> u;
    if (!chol.apply(m,u,Upper)) {
      setStatusString(chol.getStatusString());
      inv.clear();
      return false;
    }
    _lti_debug2("choldecomp:\n" << u << "\n");

    const int r=u.rows();
    inv.allocate(u.size());

    // TODO: CHECK IF THIS IS FASTER WITH ITERATORS
    T s=T(0);
    for (i=0; i<r; i++) {
      u.at(i,i)=T(1)/u.at(i,i);
      for (j=i+1; j<r; j++) {
        s=T(0);
        for (k=i; k<j; k++) {
          s-= u.at(k,j)*u.at(i,k);
        }
        const T diag=u.at(j,j);
        if (diag<threshold) {
          setStatusString("Matrix is singular\n");
          inv.clear();
          return false;
        } else {
          u.at(i,j)=s/diag;
        }
      }
    }
    _lti_debug2("inverse of choldecomp:\n" << u << "\n");


    //calculate u * uT taking into account that u is upper triangular
    //note: not faster using references to row vectors
    T sum;
    for (i=0; i<r; i++) {
      sum=T(0);
      for (k=i; k<r; k++) {
        sum+=u.at(i,k)*u.at(i,k);
      }
      inv.at(i,i)=sum;
      for (j=i+1; j<r; j++) {
        sum=T(0);
        for (k=j; k<r; k++) {
          sum+=u.at(i,k)*u.at(j,k);
        }
        inv.at(i,j)=sum;
        inv.at(j,i)=sum;
      }
    }

//     T sum;
//     for (i=0; i<r; i++) {
//       vector<T>& v1=u.getRow(i);
//       sum=T(0);
//       for (k=i; k<r; k++) {
//         sum+=v1.at(k)*v1.at(k);
//       }
//       inv.at(i,i)=sum;
//       for (j=i+1; j<r; j++) {
//         vector<T>& v2=u.getRow(j);
//         sum=T(0);
//         for (k=j; k<r; k++) {
//           sum+=v1.at(k)*v2.at(k);
//         }
//         inv.at(i,j)=sum;
//         inv.at(j,i)=sum;
//       }
//     }

    return b;
  }
  
  template <class T>
  bool symmetricMatrixInversion<T>::invert2(const matrix<T>& m,
                                            matrix<T>& inv) const {
    bool b=true;

    inv.allocate(2,2);

    // tested to be the fastest implementation

    const T of=m.at(0,1);
    T det=m.at(0,0)*m.at(1,1)-of*of;

    if (det<std::numeric_limits<T>::epsilon()) {
      setStatusString("Matrix is singular");
      inv.clear();
      return false;
    }
      
    T off=T(-1)*of/det;
    inv.at(0,0)=m.at(1,1)/det;
    inv.at(0,1)=off;
    inv.at(1,0)=off;
    inv.at(1,1)=m.at(0,0)/det;

    return b;

  }

  template <class T>
  bool symmetricMatrixInversion<T>::invert3(const matrix<T>& m,
                                            matrix<T>& inv) const {
    bool b=true;

    //matrix elements
    const T a00=m.at(0,0);
    const T a01=m.at(0,1);
    const T a02=m.at(0,2);
    const T a11=m.at(1,1);
    const T a12=m.at(1,2);
    const T a22=m.at(2,2);

    //cofactors
    T c00=a11*a22-a12*a12;
    T c11=a00*a22-a02*a02;
    T c22=a00*a11-a01*a01;
    T c01=a12*a02-a01*a22;
    T c02=a01*a12-a11*a02;
    T c12=a01*a02-a00*a12;

    //determinant
    T det=a00*c00+a01*c01+a02*c02;

    if (det<std::numeric_limits<T>::epsilon()) {
      setStatusString("Matrix is singular");
      inv.clear();
      return false;
    }

    //resize matrix
    inv.allocate(3,3);

    //set inverse matrix elements
    c00/=det;
    c11/=det;
    c22/=det;
    c01/=det;
    c02/=det;
    c12/=det;
    typename matrix<T>::iterator invIt=inv.begin();
    *invIt=c00;
    ++invIt;
    *invIt=c01;
    ++invIt;
    *invIt=c02;
    ++invIt;
    *invIt=c01;
    ++invIt;
    *invIt=c11;
    ++invIt;
    *invIt=c12;
    ++invIt;
    *invIt=c02;
    ++invIt;
    *invIt=c12;
    ++invIt;
    *invIt=c22;

    return b;

  }

  template <class T>
  bool symmetricMatrixInversion<T>::invert4(const matrix<T>& m,
                                            matrix<T>& inv) const {
    bool b=true;

    //matrix elements
    const T a00=m.at(0,0);
    const T a01=m.at(0,1);
    const T a02=m.at(0,2);
    const T a03=m.at(0,3);
    const T a11=m.at(1,1);
    const T a12=m.at(1,2);
    const T a13=m.at(1,3);
    const T a22=m.at(2,2);
    const T a23=m.at(2,3);
    const T a33=m.at(3,3);

    T d2_12_01 = a01*a12 - a11*a02;
    T d2_12_02 = a01*a22 - a12*a02;
    T d2_12_12 = a11*a22 - a12*a12;
    T d2_13_01 = a01*a13 - a11*a03;
    T d2_13_02 = a01*a23 - a12*a03;
    T d2_13_03 = a01*a33 - a13*a03;
    T d2_13_12 = a11*a23 - a12*a13;
    T d2_13_13 = a11*a33 - a13*a13;
    T d2_23_01 = a02*a13 - a12*a03;
    T d2_23_02 = a02*a23 - a22*a03;
    T d2_23_03 = a02*a33 - a23*a03;
    T d2_23_12 = a12*a23 - a22*a13;
    T d2_23_13 = a12*a33 - a23*a13;
    T d2_23_23 = a22*a33 - a23*a23;
    
    // Find all NECESSARY 3x3 dets:   (10 of them)
    
    T d3_012_012 = a00*d2_12_12 - a01*d2_12_02 
      + a02*d2_12_01;
    T d3_013_012 = a00*d2_13_12 - a01*d2_13_02 
      + a02*d2_13_01;
    T d3_013_013 = a00*d2_13_13 - a01*d2_13_03
      + a03*d2_13_01;
    T d3_023_012 = a00*d2_23_12 - a01*d2_23_02 
      + a02*d2_23_01;
    T d3_023_013 = a00*d2_23_13 - a01*d2_23_03
      + a03*d2_23_01;
    T d3_023_023 = a00*d2_23_23 - a02*d2_23_03
      + a03*d2_23_02;
    T d3_123_012 = a01*d2_23_12 - a11*d2_23_02 
      + a12*d2_23_01;
    T d3_123_013 = a01*d2_23_13 - a11*d2_23_03 
      + a13*d2_23_01;
    T d3_123_023 = a01*d2_23_23 - a12*d2_23_03 
      + a13*d2_23_02;
    T d3_123_123 = a11*d2_23_23 - a12*d2_23_13 
      + a13*d2_23_12;
    
    // Find the 4x4 det:
    
    T det = a00*d3_123_123 
      - a01*d3_123_023 
      + a02*d3_123_013 
      - a03*d3_123_012;
    
    if (det<std::numeric_limits<T>::epsilon()) {  
      setStatusString("Matrix is singular");
      inv.clear();
      return false;
    } 
 
    inv.allocate(4,4);
   
    T oneOverDet = static_cast<T>(1)/det;
    T mn1OverDet = - oneOverDet;
 
//     d3_123_123*=oneOverDet;
    d3_123_023*=mn1OverDet;
    d3_123_013*=oneOverDet;
    d3_123_012*=mn1OverDet;
    
//     d3_023_023*=oneOverDet;
    d3_023_013*=mn1OverDet;
    d3_023_012*=oneOverDet;
    
//     d3_013_013*=oneOverDet;
    d3_013_012*=mn1OverDet;
    
//     d3_012_012*=oneOverDet;

    typename matrix<T>::iterator invIt=inv.begin();

    (*invIt)=d3_123_123*oneOverDet;
      ++invIt;
    (*invIt)=d3_123_023;
      ++invIt;
    (*invIt)=d3_123_013;
      ++invIt;
    (*invIt)=d3_123_012;
      ++invIt;
    
    (*invIt)=d3_123_023;
      ++invIt;
    (*invIt)=d3_023_023*oneOverDet;
      ++invIt;
    (*invIt)=d3_023_013;
      ++invIt;
    (*invIt)=d3_023_012;
      ++invIt;
    
    (*invIt)=d3_123_013;
      ++invIt;
    (*invIt)=d3_023_013;
      ++invIt;
    (*invIt)=d3_013_013*oneOverDet;
      ++invIt;
    (*invIt)=d3_013_012;
      ++invIt;
    
    (*invIt)=d3_123_012;
      ++invIt;
    (*invIt)=d3_023_012;
      ++invIt;
    (*invIt)=d3_013_012;
      ++invIt;
    (*invIt)=d3_012_012*oneOverDet;
      ++invIt;
    
    return b;
    
  }
}
