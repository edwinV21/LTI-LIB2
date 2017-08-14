/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiGaussKernels_template.h
 *         Contains the definintions for 1D and 2D Gauss kernels.
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * $Id: ltiGaussKernels_template.h,v 1.5 2007-04-05 22:55:43 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {

  // -------------------------------------------------------------------
  // 1D filter kernel
  // -------------------------------------------------------------------

  // constructor
  template<typename T>
  gaussKernel1D<T>::gaussKernel1D(const int sz,
                                  const double& theVariance)
    : kernel1D<T>() {

    generate(sz,theVariance);
  }

  // constructor
  template<typename T>
  gaussKernel1D<T>::gaussKernel1D(const double& theVariance,
                                  const double& err)
    : kernel1D<T>() {

    generate(theVariance,err);
  }

  // clone
  template<typename T> 
  gaussKernel1D<T>* gaussKernel1D<T>::clone() const {
      return (new gaussKernel1D<T>(*this));
  }
  
  // newInstance
  template<typename T> 
  gaussKernel1D<T>* gaussKernel1D<T>::newInstance() const {
      return (new gaussKernel1D<T>());
  }

  // typename
  template<typename T> 
  const std::string& gaussKernel1D<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // generate filter
  template<typename T>
  void gaussKernel1D<T>::generate(const int sz,
                                  const double& theVariance) {

    double var = theVariance;

    // calculate the variance such that the element at (size/2) is
    // the element at 0 divided by (1+int(size/2))
    if (theVariance<0) {
      int n = (sz/2);
      if (n<1) {
        n = 1;
      }

      // log == ln (natural logarithm)
      var = (n*n)/(2*log(static_cast<double>(n+1)));
    }

    int i;
    const double snorm = static_cast<double>(typeInfo<T>::suggestedNorm());

    double accu,factor;
    variance = var;  // initialize member variance
    this->allocate(-(sz/2),sz-(sz/2)-1);
    factor = 1.0/sqrt(2*Pi*var);

    accu=0;
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(snorm*factor*exp(-double(i*i)/(2*var)));
      accu+=this->at(i);
    }
    accu /= snorm;
    // make sure the sum of all elements is one!
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(this->at(i)/accu);
    }

  }

  template <typename T>
  void gaussKernel1D<T>::generate(const double& var,
                                  const double& err) {
    
    const int n=static_cast<int>(sqrt(-2.*var*log(err*sqrt(2*Pi*var))));
    const int sz=max(3,2*n+1);

    int i;
    const double snorm = static_cast<double>(typeInfo<T>::suggestedNorm());

    double accu,factor;
    variance = var;  // initialize member variance
    this->allocate(-(sz/2),sz-(sz/2)-1);
    factor = 1.0/sqrt(2*Pi*var);

    accu=0;
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(snorm*factor*exp(-double(i*i)/(2*var)));
      accu+=this->at(i);
    }
    accu /= snorm;
    // make sure the sum of all elements is one!
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(this->at(i)/accu);
    }
    

  }

  template <typename T>
  const double& gaussKernel1D<T>::getActualVariance() const {
    return variance;
  }

  // -------------------------------------------------------------------
  // 2D filter kernel
  // -------------------------------------------------------------------

  // constructor
  template<typename T>
  gaussKernel2D<T>::gaussKernel2D(const int size,
                                    const double& theVariance)
    : separableKernel<T>() {
    generate(size,theVariance);
  }

  // constructor
  template<typename T>
  gaussKernel2D<T>::gaussKernel2D(const double& theVariance,
                                  const double& err)
    : separableKernel<T>() {
    
    generate(theVariance,err);
  }

  // clone
  template<typename T> 
  gaussKernel2D<T>* gaussKernel2D<T>::clone() const {
      return (new gaussKernel2D<T>(*this));
  }
  
  // newInstance
  template<typename T> 
  gaussKernel2D<T>* gaussKernel2D<T>::newInstance() const {
      return (new gaussKernel2D<T>());
  }

  // typename
  template<typename T> 
  const std::string& gaussKernel2D<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // generate filter
  template<typename T>
  void gaussKernel2D<T>::generate(const int size,
                                  const double& theVariance) {

    gaussKernel1D<T> oneD(size,theVariance);
    variance=oneD.getActualVariance();

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(oneD);
    this->getColFilter(0).copy(oneD);
  }

  // generate filter
  template<typename T>
  void gaussKernel2D<T>::generate(const double& theVariance,
                                  const double& err) {

    gaussKernel1D<T> oneD;
    oneD.generate(theVariance,err);
    variance=oneD.getActualVariance();

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(oneD);
    this->getColFilter(0).copy(oneD);
  }

  template <typename T>
  const double& gaussKernel2D<T>::getActualVariance() const {
    return variance;
  }
}

