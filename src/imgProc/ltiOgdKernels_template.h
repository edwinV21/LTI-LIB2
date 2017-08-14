/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiOgdKernel_template.h
 *         This file contains the definitions of first and second
 *         order Oriented Gaussian Derivative (OGD) kernels.
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * $Id: ltiOgdKernels_template.h,v 1.3 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiMath.h"
#include "ltiGaussKernels.h"

namespace lti {

  //----------------
  // first order ogd
  //----------------

  // default constructor
  template<class T>
    ogd1Kernel<T>::ogd1Kernel() : separableKernel<T>() {
  }

  // copy constructor
  template<class T>
  ogd1Kernel<T>::ogd1Kernel(const ogd1Kernel& other) : separableKernel<T>() {
    this->copy(other);
  }

  // constructor
  template<class T>
  ogd1Kernel<T>::ogd1Kernel(const int size,
                            const double& theVariance,
                            const double& theAngle)
    : separableKernel<T>() {
    generate(size,theVariance,theAngle);
  }

  // constructor
  template<class T>
  ogd1Kernel<T>::ogd1Kernel(const eGradientDirection gradientDirection,
                            const int size,
                            const double& variance)
    : separableKernel<T>() {
    generateBasisKernel(gradientDirection,size,variance);
  }

  // clone
  template<class T> 
  ogd1Kernel<T>* ogd1Kernel<T>::clone() const {
      return (new ogd1Kernel<T>(*this));
  }
  
  // newInstance
  template<class T> 
  ogd1Kernel<T>* ogd1Kernel<T>::newInstance() const {
      return (new ogd1Kernel<T>());
  }

  // typename
  template<class T> 
  const std::string& ogd1Kernel<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // generate filter
  template<class T>
  void  ogd1Kernel<T>::generate(const int size,
                                const double& theVariance,
                                const double& theAngle) {

    gaussKernel1D<T> gauss(size,theVariance);
    // make sure to use the correct variance,
    // e.g. theVariance could be negative
    const double actVar=gauss.getActualVariance();

    this->setNumberOfPairs(2);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);
    this->getRowFilter(1).copy(gauss);
    this->getColFilter(1).copy(gauss);

    int x;
    double cost,sint;

    cost=cos(theAngle);
    sint=sin(theAngle);

    for (x  = gauss.firstIdx();
         x <= gauss.lastIdx();
         x++) {
      this->getRowFilter(0).at(x) *= static_cast<T>(-x/actVar);
      this->getColFilter(0).at(x) *= static_cast<T>(cost);
      this->getRowFilter(1).at(x) *= static_cast<T>(sint);
      this->getColFilter(1).at(x) *= static_cast<T>(-x/actVar);
    }
  }

  template<class T>
  void  ogd1Kernel<T>::generateBasisKernel(
                                   const eGradientDirection gradientDirection,
                                   const int size,
                                   const double& theVariance) {

    gaussKernel1D<T> gauss(size,theVariance);
    // make sure to use the correct variance,
    // e.g. theVariance could be negative
    const double actVar=gauss.getActualVariance();

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);

    int x;
    kernel1D<T>* f = 0;

    if (gradientDirection == X) {
      f = &this->getRowFilter(0);
    } else {
      f = &this->getColFilter(0);
    }

    for (x  = gauss.firstIdx();
         x <= gauss.lastIdx();
         x++) {
      f->at(x) *= static_cast<T>(-x/actVar);
    }
  }

  // second order filter kernel

  // constructor
  template<class T>
  ogd2Kernel<T>::ogd2Kernel()
    : separableKernel<T>() {
  }

  // copy constructor
  template<class T>
  ogd2Kernel<T>::ogd2Kernel(const ogd2Kernel& other)
    : separableKernel<T>() {
    copy(other);
  }

  template<class T>
  ogd2Kernel<T>::ogd2Kernel(const int size,
                            const double& theVariance,
                            const double& theAngle)
    : separableKernel<T>() {
    generate(size,theVariance,theAngle);
  }

  // constructor
  template<class T>
  ogd2Kernel<T>::ogd2Kernel(const eSecondDerivativeDirection secondDerivativeDirection,
                            const int size,
                            const double& variance)
    : separableKernel<T>() {
    generateBasisKernel(secondDerivativeDirection,size,variance);
  }

  // clone
  template<class T> 
  ogd2Kernel<T>* ogd2Kernel<T>::clone() const {
      return (new ogd2Kernel<T>(*this));
  }
  
  // newInstance
  template<class T> 
  ogd2Kernel<T>* ogd2Kernel<T>::newInstance() const {
      return (new ogd2Kernel<T>());
  }

  // typename
  template<class T> 
  const std::string& ogd2Kernel<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // generate filter
  template<class T>
  void  ogd2Kernel<T>::generate(const int size,
                                const double& theVariance,
                                const double& theAngle) {

    gaussKernel1D<T> gauss(size,theVariance);
    // make sure to use the correct variance,
    // e.g. theVariance could be negative
    const double actVar=gauss.getActualVariance();

    this->setNumberOfPairs(3);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);
    this->getRowFilter(1).copy(gauss);
    this->getColFilter(1).copy(gauss);
    this->getRowFilter(2).copy(gauss);
    this->getColFilter(2).copy(gauss);

    int x;
    double xx;

    const double cost=cos(theAngle);
    const double sint=sin(theAngle);
    const double cost2=sqr(cost);
    const double sint2=sqr(sint);
    const double costw=2*cost;
    const double var2 = sqr(actVar);

    for (x  = gauss.firstIdx();
         x <= gauss.lastIdx();
         x++) {
      xx = sqr(double(x));
      this->getRowFilter(0).at(x) *= static_cast<T>((xx-actVar)/var2);
      this->getColFilter(1).at(x) *= static_cast<T>(cost2);

      this->getRowFilter(1).at(x) *= static_cast<T>(sint2);
      this->getColFilter(1).at(x) *= static_cast<T>((xx-actVar)/var2);

      this->getRowFilter(2).at(x) *= static_cast<T>(costw*x/actVar);
      this->getColFilter(2).at(x) *= static_cast<T>(sint*x/actVar);
    }
  }

  template<class T>
  void  ogd2Kernel<T>::generateBasisKernel(const eSecondDerivativeDirection secondDerivativeDirection,
                                           const int size,
                                           const double& theVariance) {
    gaussKernel1D<T> gauss(size,theVariance);
    // make sure to use the correct variance,
    // e.g. theVariance could be negative
    const double actVar=gauss.getActualVariance();

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);

    int x;
    kernel1D<T>* f = 0;

    if (secondDerivativeDirection == XY) {
      for (x = gauss.firstIdx();
           x <= gauss.lastIdx();
           x++) {
        this->getRowFilter(0).at(x) *= static_cast<T>(x/actVar);
        this->getColFilter(0).at(x) *= static_cast<T>(x/actVar);
      }

    } else {

      if (secondDerivativeDirection == XX) {
        f = &this->getRowFilter(0);
      } else {
        f = &this->getColFilter(0);
      }

      // the sum of this kernel is not 0 so as a quick fix
      // we shift the filter by an offset. This is not correct but
      // better than having derivative values !=0 for homogeneous input
      T ksum(0);
      const double actVarSqr=actVar*actVar;
      for (x  = gauss.firstIdx();
           x <= gauss.lastIdx();
           x++) {
        f->at(x) *= static_cast<T>(((x*x)-actVar)/actVarSqr);
        ksum+=f->at(x);
      }
      ksum/=size;
      for (x  = gauss.firstIdx();
           x <= gauss.lastIdx();
           x++) {
        f->at(x)-=ksum;
      }
    }
  }
}

