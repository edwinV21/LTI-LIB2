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
 * \file   ltiLuSolution_template.h
 * \author Thomas Rusert
 * \date   02.06.1999
 *
 * $Id: ltiLuSolution_template.h,v 1.4 2012-11-17 14:49:05 alvarado Exp $
 */

#include "ltiLuDecomposition.h"

namespace lti {

  //---------------------------
  //--- LU solution functor ---
  //---------------------------

  // default constructor
  template <class T>
  luSolution<T>::luSolution() : decompositionSolution<T>() {
  }

  // copy constructor
  template <class T>
  luSolution<T>::luSolution(const luSolution& other)
    : decompositionSolution<T>() {
    copy(other);
  }

  //  constructor
  template <class T>
  luSolution<T>::luSolution(const typename luSolution<T>::parameters& param)
    : decompositionSolution<T>() {
    this->setParameters(param);
  }

  //  constructor
  template <class T>
  luSolution<T>::luSolution(const matrix<T>& mat)
    : decompositionSolution<T>(mat) {
  }

  // onPlace version of apply
  // see for example: Press, Vetterling, Teukolsky, Flannery
  //                  Numerical Recipes in C, 2nd edition
  //                  Cambridge University Press, 1992
  //                  p. 47
  template<class T>
  bool luSolution<T>::apply(vector<T>& b) {
    const parameters& tmpParam = this->getParameters();
    const int n=tmpParam.systemMatrix.rows();
    if((n != tmpParam.systemMatrix.columns()) || (n!=b.size())) {
      this->setStatusString("Incompatible dimensions");
      return false;
    }
    int i,ii=-1,ip,j;
    T sum;

    // lu decomposition
    if(!this->decomposed_) {
      luDecomposition<T> decmp;
      int pivot;
      decmp.apply(tmpParam.systemMatrix,this->dcmpMat_,dcmpVec_,pivot);
      this->decomposed_ = true;
    }

    // forward substitution and backsubstitution
    for (i=0;i<n;i++) {
      ip=dcmpVec_[i];
      sum=b[ip];
      b[ip]=b[i];
      if (ii+1) {
        for (j=ii;j<=i-1;j++) {
          sum -= this->dcmpMat_[i][j]*b[j];
        }
      } else if (!closeToZero(sum)) {
        ii=i;
      }
      b[i]=sum;
    }

    for (i=n-1;i>=0;i--) {
      sum=b[i];
      for (j=i+1;j<n;j++) {
        sum -= this->dcmpMat_[i][j]*b[j];
      }
      if (closeToZero(sum) || closeToZero(this->dcmpMat_[i][i])) {
        b[i]=static_cast<T>(0);
      } else {
        b[i]=sum/this->dcmpMat_[i][i];
      }
    }

    return true;
  }

  // onCopy version of apply
  template<class T>
  bool luSolution<T>::apply(const vector<T>& b,vector<T>& x) {
    x.copy(b);
    return apply(x);
  }

  // solve many equations at the same time!
  template<class T>
  bool luSolution<T>::apply(matrix<T>& X) {
    bool result=false;
    X.transpose();
    for (int i=0;i<X.rows();i++) {
      result=apply(X.getRow(i));
    }
    X.transpose();
    return result;
  }

  // solve many equations at the same time!
  template<class T>
  bool luSolution<T>::apply(const matrix<T>& B,matrix<T>& X) {
    X.copy(B);
    return apply(X);
  }

  // copy data of "other" functor.
  template<class T>
  luSolution<T>& luSolution<T>::copy(const luSolution<T>& other) {
    decompositionSolution<T>::copy(other);
    dcmpVec_=other.dcmpVec_;

    return (*this);
  }
  /*
   * Returns the name of this class.
   */
  template<class T>
  const std::string& luSolution<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * Returns a pointer to a clone of this functor.
   */
  template<class T>
  luSolution<T>* luSolution<T>::clone() const {
    return new luSolution<T>(*this);
  }
  
  /*
   * Returns a pointer to a new instance of this functor.
   */
  template<class T>
  luSolution<T>* luSolution<T>::newInstance() const {
    return new luSolution<T>();
  }

}

