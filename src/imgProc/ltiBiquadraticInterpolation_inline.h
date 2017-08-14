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
 * \file   ltiBiquadraticInterpolation_template.h
 *         The file contains the functor biquadraticInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiBiquadraticInterpolation_inline.h,v 1.2 2005-01-11 15:31:03 doerfler Exp $
 */

#include "ltiRGBPixel.h"

namespace lti {

  // --------------------------------------------------
  // biquadraticInterpolation
  // --------------------------------------------------

  
  template<class T>
  inline T biquadraticInterpolation<T>::compute(const float& fx,
                                                const T& y0,
                                                const T& y1,
                                                const T& y2) const {

    const T a = (y0+y2)/2 - y1;
    const T b = (y2-y0)/2;
    const T c = y1;

    return (static_cast<T>((a*fx + b)*fx + c));
  }

  template<>
  inline rgbaPixel 
  biquadraticInterpolation<rgbaPixel>::compute(const float& fx,
                                               const rgbaPixel& y0,
                                               const rgbaPixel& y1,
                                               const rgbaPixel& y2) const {
    const rgbPixel<float> y0f(y0);
    const rgbPixel<float> y1f(y1);
    const rgbPixel<float> y2f(y2);

    const rgbPixel<float> a = (y0f+y2f)/2.0f - y1f;
    const rgbPixel<float> b = (y2f-y0f)/2.0f;
    const rgbPixel<float> c = y1f;

    const rgbPixel<float> res((a*fx + b)*fx + c);

    //rgbPixel::getCLIPPED no longer available, ask Pablo why
    return rgbaPixel(res.red<0 ? 0 : res.red>255 ? 255 :
                     static_cast<ubyte>(res.red),
                     res.green<0 ? 0 : res.green>255 ? 255 :
                     static_cast<ubyte>(res.green),
                     res.blue<0 ? 0 : res.blue>255 ? 255 :
                     static_cast<ubyte>(res.blue));
  }

  template<class T>
  inline T biquadraticInterpolation<T>::compute(const float& fy,
                                                const float& fx,
                                                const T *const y) const {
    
    const T v1 = compute(fx,y[0],y[1],y[2]);
    const T v2 = compute(fx,y[3],y[4],y[5]);
    const T v3 = compute(fx,y[6],y[7],y[8]);

    return compute(fy,v1,v2,v3);
  }

  template<class T>
  inline T biquadraticInterpolation<T>::compute(const float& fy,
                                                const float& fx,
                                                const T& y00,
                                                const T& y01,
                                                const T& y02,
                                                const T& y10,
                                                const T& y11,
                                                const T& y12,
                                                const T& y20,
                                                const T& y21,
                                                const T& y22) const {
    
    const T v1 = compute(fx,y00,y01,y02);
    const T v2 = compute(fx,y10,y11,y12);
    const T v3 = compute(fx,y20,y21,y22);
    
    return compute(fy,v1,v2,v3);
  }
  
  template<class T>
  inline T biquadraticInterpolation<T>::interpolate(const float& row,
                                                    const float& col) const {
    return interpolate(*this->theMatrix_,row,col);
  }

  template<class T>
  inline T biquadraticInterpolation<T>::interpolateUnchk(const matrix<T>& src,
                                                         const float& row,
                                                         const float& col) const{
    // precomputations for x
    int truncX = static_cast<int>(col);
    float fractX = col - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // precomputation for y
    int truncY = static_cast<int>(row);
    float fractY = row - static_cast<float>(truncY);
    if (fractY <= 0.5f) {
      --truncY;
    } else {
      fractY-=1.0f;
    }

    const T *const row1 = &src.at(truncY,truncX);
    const T *const row2 = &src.at(truncY+1,truncX);
    const T *const row3 = &src.at(truncY+2,truncX);

    return compute(fractY,fractX,
                   row1[0],row1[1],row1[2],
                   row2[0],row2[1],row2[2],
                   row3[0],row3[1],row3[2]);
    
  }

  template<class T>
  inline T biquadraticInterpolation<T>::interpolateUnchk(const float& row,
                                                         const float& col)const{
    return interpolateUnchk(*this->theMatrix_,row,col);
  }


}
