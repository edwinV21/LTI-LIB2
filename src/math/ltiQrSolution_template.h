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
 * \file   ltiQrSolution_template.h
 * \author Thomas Rusert
 * \date   02.06.1999
 *
 * $Id: ltiQrSolution_template.h,v 1.3 2012-11-17 14:49:05 alvarado Exp $
 */

#include "ltiIncompatibleDimensionsException.h"

namespace lti {
  
  //------------------------------
  //--- qrSolution::parameters ---
  //------------------------------

  // default constructor 
  template <class T>
  qrSolution<T>::parameters::parameters() 
    : decompositionSolution<T>::parameters(),computeResiduum(false) {
  }

  // copy constructor 
  template <class T>
  qrSolution<T>::parameters::parameters(const typename qrSolution<T>::parameters& other)
    : decompositionSolution<T>::parameters() {
    copy(other);
  }

  // copy member
  template <class T>
  typename qrSolution<T>::parameters& 
  qrSolution<T>::parameters::copy(const parameters& other) {
    decompositionSolution<T>::parameters::copy(other);
    computeResiduum = other.computeResiduum;
    return (*this);
  }

  // name
  template<class T>
  const std::string& qrSolution<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template <class T>
  typename qrSolution<T>::parameters* 
  qrSolution<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance
  template <class T>
  typename qrSolution<T>::parameters* 
  qrSolution<T>::parameters::newInstance() const {
    return new parameters();
  }

  //------------------
  //--- qrSolution ---
  //------------------

  //default constructor
  template <class T>
  qrSolution<T>::qrSolution() : decompositionSolution<T>() {
  }

  // constructor
  template <class T>
  qrSolution<T>::qrSolution(const typename qrSolution<T>::parameters& param)
    : decompositionSolution<T>() {
    this->setParameters(param);
  }

  // constructor
  template <class T>
  qrSolution<T>::qrSolution(const matrix<T>& mat)
    : decompositionSolution<T>(mat) {
  }

  //copy constructor
  template <class T>
  qrSolution<T>::qrSolution(const qrSolution<T>& other)
    : decompositionSolution<T>() {
    copy(other);
  }

  // returns the current parameters
  template<class T>
  const typename qrSolution<T>::parameters&
  qrSolution<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if (isNull(params)) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  // onPlace version of apply
  template<class T>
  double qrSolution<T>::apply(vector<T>& b) {
    vector<T> tmpVec;
    double tmpRes;
    tmpRes=apply(b,tmpVec);
    b=tmpVec;
    return tmpRes;
  }

  // onCopy version of apply
  // from: Engeln-Müllges, Reutter
  //       Formelsammlung zur Numerischen Mathematik mit C-Programmen
  //       BI-Wissenschaftsverlag, 1990
  //       p. 492
  template<class T>
  double qrSolution<T>::apply(const vector<T>& b,vector<T>& x) {
    const parameters& tmpParam = getParameters();
    const matrix<T>& A = tmpParam.systemMatrix;
    const int n = A.columns();
    const int m = A.rows();
    vector<T> c(b);
    double radiant,alpha,factor,sum;
    int i,k,j;

    //        int si=b.size();
    if ((m < n)||(m != b.size())) {
       throw incompatibleDimensionsException();
    }

    if(!this->decomposed_) {
      // decompose A, compose R=HA, compose c=Hb (H: transposed Q-matrix)
      this->dcmpMat_.copy(A);
      dcmpVec_.resize(n);
      helpVec_.resize(n);
      for(i = 0; i < n; i++) {
        for(radiant = sqr(this->dcmpMat_[i][i]),
              k = i + 1; k < m; k++) {
          radiant += sqr(this->dcmpMat_[k][i]);
        }
        if(radiant < (4*std::numeric_limits<double>::epsilon())) {
          // matrix is singular
          x.clear();
          this->setStatusString("Matrix is singular\n");
          return false;
        }
        if(this->dcmpMat_[i][i] > 0) {
          alpha = sqrt(radiant);
        }
        else {
          alpha =- sqrt(radiant);
        }
        helpVec_[i] = 1.0 / (radiant + alpha * this->dcmpMat_[i][i]);
        this->dcmpMat_[i][i] += static_cast<T>(alpha);

        dcmpVec_[i] =- alpha;
        for(k = i + 1; k < n; k++) {
          for(factor = .0 ,j = i;j < m;j++) {
            factor += this->dcmpMat_[j][k] * this->dcmpMat_[j][i];
          }
          factor *= helpVec_[i];
          for(j = i; j < m; j++) {
            this->dcmpMat_[j][k] -= static_cast<T>(factor * 
                                                  this->dcmpMat_[j][i]);
          }
        }
        for(factor = .0 ,j = i;j < m;j++) {
          factor += c[j] * this->dcmpMat_[j][i];
        }
        factor *= helpVec_[i];
        for(j = i; j < m; j++) {
          c[j] -= static_cast<T>(factor * this->dcmpMat_[j][i]);
        }
      }
      this->decomposed_ = true;
    }
    else { // compose c=Hb
      for(i = 0; i < n; i++) {
        for(factor = .0, j = i; j < m; j++) {
          factor += c[j] * this->dcmpMat_[j][i];
        }
        factor *= helpVec_[i];
        for(j = i; j < m; j++) {
          c[j] -= static_cast<T>(factor * this->dcmpMat_[j][i]);
        }
      }
    }

    // solve Rx=c
    x.resize(n);
    for(i = n-1; i >= 0; i--) {
      for(sum = .0, k = i + 1; k < n; k++) {
        sum += this->dcmpMat_[i][k] * x[k];
      }
      x[i] = static_cast<T>((c[i] - sum) / dcmpVec_[i]);
    }

    if(tmpParam.computeResiduum) { // calculate residuum
      for(sum = .0, i = n; i < m; i++) {
        sum += c[i]*c[i];
      }
      return sqrt(sum);
    }

    return -1;
  }

  // copy data of "other" functor.
  template<class T>
  qrSolution<T>& qrSolution<T>::copy(const qrSolution<T>& other) {
    decompositionSolution<T>::copy(other);
    helpVec_=other.helpVec_;
    dcmpVec_=other.dcmpVec_;

    return (*this);
  }

  /*
   * Returns the name of this class.
   */
  template<class T>
  const std::string& qrSolution<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * Returns a pointer to a clone of this functor.
   */
  template<class T>
  qrSolution<T>* qrSolution<T>::clone() const {
    return new qrSolution<T>(*this);
  }
  
  /*
   * Returns a pointer to a new instance of this functor.
   */
  template<class T>
  qrSolution<T>* qrSolution<T>::newInstance() const {
    return new qrSolution<T>();
  }

}
