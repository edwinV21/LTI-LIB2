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
 * \file   ltiGenericInterpolation_inline.h
 *         Contains the functor genericInterpolation which uses LUT
 *         for interpolation kernels
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiGenericInterpolation_inline.h,v 1.4 2007-09-10 02:41:36 alvarado Exp $
 */

#include "ltiRGBPixel.h"

namespace lti {

  //
  // Private class used to get the smallest floating point accumulation type
  // that can be safetly used.
  //
  template<typename T>
  struct giMinimalFPType {
    typedef float fp_type;
  };

  template<>
  struct giMinimalFPType<double> {
    typedef double fp_type;
  };
  
  
  // --------------------------------------------
  // implementation of some inline helper methods
  // --------------------------------------------
  
  template<typename T>
  inline T genericInterpolation<T>::compute(const float fx,
                                            const T* y) const {

    typedef typename giMinimalFPType<T>::fp_type fp_type;

    int i=static_cast<int>(fx*intervalSize_)+lastIntervalFirstSample_;
    fp_type acc = static_cast<fp_type>((*y++)*interpolationKernel_.at(i));
    i-=intervalSize_;
    while (i>0) {
      acc += static_cast<fp_type>((*y++)*interpolationKernel_.at(i));
      i-=intervalSize_;
    }

    return static_cast<T>(acc);
  }

  template<>
  inline rgbaPixel 
  genericInterpolation<rgbaPixel>::compute(const float fx,
                                           const rgbaPixel* y) const {

    int i=static_cast<int>(fx*intervalSize_)+lastIntervalFirstSample_;
    rgbPixel<float> tmp;
    rgbPixel<float> acc(*y++);
    acc*=interpolationKernel_.at(i);
    i-=intervalSize_;
    while (i>0) {
      tmp.copy(*y++);
      tmp*=interpolationKernel_.at(i);
      acc.add(tmp);
      i-=intervalSize_;
    }

    return acc.getClippedRGBAPixel();
  }

  template<typename T>
  inline T genericInterpolation<T>::compute(const float fy,
                                            const float fx,
                                            const T** data) const {
    int i;
    for (i=0;i<numberOfIntervals_;++i) {
      firstElemColumn_[i]=compute(fx,data[i]);
    }
    return compute(fy,firstElemColumn_);
  }

  template<typename T>
  inline T genericInterpolation<T>::interpolate(const float row,
                                                const float col) const {
    return interpolate(*this->theMatrix_,row,col);
  }


  template<typename T>
  inline T genericInterpolation<T>::interpolateUnchk(const matrix<T>& src,
                                                     const float row,
                                                     const float col) const {
    // precomputations for x
    int truncX = static_cast<int>(col);
    float fractX = col - static_cast<float>(truncX);
    truncX-=firstSampleOffset_;

    // precomputation for y
    int truncY = static_cast<int>(row);
    float fractY = row - static_cast<float>(truncY);
    truncY-=firstSampleOffset_;
    int i;

    for (i=0;i<numberOfIntervals_;++i) {
      rows_[i]=&src.at(truncY+i,truncX);
    }
    return compute(fractY,fractX,rows_);
  }

  template<typename T>
  inline T genericInterpolation<T>::interpolateUnchk(const float row,
                                                     const float col) const {
    return interpolateUnchk(*this->theMatrix_,row,col);
  }

  template<typename T>
  inline const vector<float>& genericInterpolation<T>::getLUT() const {
    return interpolationKernel_;
  }
}

