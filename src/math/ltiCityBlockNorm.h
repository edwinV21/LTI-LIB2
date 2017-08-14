/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
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
 * \file   ltiCityBlockNorm.h
 *         This file contains the functions T cityBlockNorm(const
 *         T&) which calculate the City Block norm.
 * \author Pablo Alvarado
 * \date   25.05.2008
 *
 * $Id: ltiCityBlockNorm.h,v 1.1 2008-05-26 00:46:57 alvarado Exp $
 */

#ifndef _LTI_CITY_BLOCK_NORM_H_
#define _LTI_CITY_BLOCK_NORM_H_

#include <vector>
#include "ltiPoint.h"
#include "ltiPoint3D.h"
#include "ltiRGBPixel.h"
#include "ltiRGBAPixel.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiDistanceType.h"

namespace lti {

  /**
   * @name City Block Norm Functions
   *
   * Global functions to compute the square of the L2 norm of a n-dimensional
   * point representation.
   *
   * @ingroup gLinearAlgebra
   */
  //@{

  /**
   * Default implementation of the L1 norm of a value of type T. It is assumed
   * that T is scalar and an efficient implementation is used. T must be
   * castable to distanceType<T>::distance_type, which in turn must be a
   * possible argument of lti::abs().
   *
   * Overloads for most LTI-Lib types are implemented.
   */
  template<class T>  
  inline typename distanceType<T>::distance_type
  cityBlockNorm(const T& a) {
    return abs(a);
  }

  /**
   * cityBlockNorm computes  the L1 norm of vector \c a.
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockNorm(const vector<T>& a) {
    typename distanceType<T>::distance_type sum(0);
    const typename vector<T>::const_iterator ite = a.end();
    typename vector<T>::const_iterator ita;
    for (ita=a.begin(); ita!=ite; ++ita) {
      sum+=abs(*ita);
    }
    return sum;
  }

  /**
   * cityBlockNorm computes  the L1 norm of the lti::matrix \c
   * a ( the Frobenius norm).
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockNorm(const matrix<T>& a) {

    typename distanceType<T>::distance_type sum(0);
    const typename matrix<T>::const_iterator ite = a.end();
    typename matrix<T>::const_iterator ita;
    for (ita=a.begin(); ita!=ite; ++ita) {
      sum+=abs(*ita);
    }

    return sum;
  }


  /**
   * cityBlockNorm computes  the L1 norm of a std::vector \c a.
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockNorm(const std::vector<T>& a) {
    typename distanceType<T>::distance_type sum(0);
    const typename vector<T>::const_iterator ite = a.end();
    typename vector<T>::const_iterator ita;
    for (ita=a.begin(); ita!=ite; ++ita) {
      sum+=abs(*ita);
    }
    return sum;

  }

  /**
   * cityBlockNorm computes  the L1 norm of a point<T> \c a.
   */
  template<class T >  
  inline typename distanceType<T>::distance_type
  cityBlockNorm(const point<T>& a) {
    return static_cast<typename distanceType<T>::distance_type>
      (abs(a.x)+abs(a.y));
  }

  /**
   * cityBlockNorm computes  the L1 norm of a point3D<T> \c a.
   */
  template<class T>
  inline typename distanceType<T>::distance_type
  cityBlockNorm(const point3D<T>& a) {
    return static_cast<typename distanceType<T>::distance_type>
      (abs(a.x)+abs(a.y)+abs(a.z));
  }

  /**
   * cityBlockNorm computes the L1 norm of a rgbPixel<T> \c a
   * in the RGB color space.
   */
  template<class T>
  inline typename distanceType<T>::distance_type
  cityBlockNorm(const rgbPixel<T>& a) {
    return static_cast<typename distanceType<T>::distance_type>
      (abs(a.red)+abs(a.green)+abs(a.blue));
  }

  /**
   * cityBlockNorm computes the L1 norm of a rgbaPixel \c a in
   * the RGB color space.
   */
  inline distanceType<rgbaPixel>::distance_type
  cityBlockNorm(const rgbaPixel& a) {
    return static_cast<distanceType<rgbaPixel>::distance_type>
      (abs(int(a.red))+abs(int(a.green))+abs(int(a.blue)));
  }

  //@}
}

#endif
