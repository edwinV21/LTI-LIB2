/*
 * Copyright (C) 1998-2004
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
 * \file   ltiBilinearInterpolation_inline.h
 *         Contains inlined function definitions for the template class
 *         bilinearInterpolation
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiBilinearInterpolation_inline.h,v 1.3 2005-04-18 13:23:24 arndh Exp $
 */

#ifndef _LTI_BILINEAR_INTERPOLATION_INLINE_H_
#define _LTI_BILINEAR_INTERPOLATION_INLINE_H_


#include "ltiRGBPixel.h"
#include "ltiRGBAPixel.h"

namespace lti {

  
  template<typename T>
  inline T bilinearInterpolation<T>::compute(const float fx,
                                             const T sx, const T sx1) const {
    return static_cast<T>(sx + fx*(sx1-sx));
  }

  // specialization for pixels
  template<>
  inline rgbaPixel 
  bilinearInterpolation<rgbaPixel>::compute(const float fx,
                                            const rgbaPixel sx,
                                            const rgbaPixel sx1) const {
    const rgbPixel<float> sxf(sx);
    const rgbPixel<float> sx1f(sx1);
    return ((sxf + (sx1f-sxf)*fx).getRGBAPixel());
  }

  template<typename T>
  inline T 
  bilinearInterpolation<T>::compute(const float fy,
                                    const float fx,
                                    const T syx, const T syx1,
                                    const T sy1x, const T sy1x1) const {
    const float tmp1 = static_cast<float>(syx  + (syx1-syx)*fx);
    const float tmp2 = static_cast<float>(sy1x + (sy1x1-sy1x)*fx);
    return static_cast<T>(tmp1 + (tmp2-tmp1)*fy);
  }

  template<>
  inline rgbaPixel
  bilinearInterpolation<rgbaPixel>::compute(const float fy,
                                            const float fx,
                                            const rgbaPixel syx,
                                            const rgbaPixel syx1,
                                            const rgbaPixel sy1x,
                                            const rgbaPixel sy1x1) const {
    
    const rgbPixel<float> syxf(syx);
    const rgbPixel<float> syx1f(syx1);
    const rgbPixel<float> sy1xf(sy1x);
    const rgbPixel<float> sy1x1f(sy1x1);

    const rgbPixel<float> tmp1 = syxf  + (syx1f-syxf)*fx;
    const rgbPixel<float> tmp2 = sy1xf + (sy1x1f-sy1xf)*fx;
    
    return (tmp1 + (tmp2-tmp1)*fy).getRGBAPixel();
  }

  template<typename T>
  inline T bilinearInterpolation<T>::interpolate(const float row,
                                                 const float col) const {
    return interpolate(*this->theMatrix_,row,col);
  }

  template<typename T>
  inline T bilinearInterpolation<T>::interpolateUnchk(const matrix<T>& src,
                                                      const float row,
                                                      const float col) const {
    const int truncY = static_cast<int>(row);
    const float fractY = row - static_cast<float>(truncY);

    const int truncX = static_cast<int>(col);
    const float fractX = col - static_cast<float>(truncX);

    const T *const row1 = &src.at(truncY,truncX);
    const T *const row2 = &src.at(truncY+1,truncX);

    // the interpolation:
    return compute(fractY,fractX,row1[0],row1[1],row2[0],row2[1]);
  }
  
  template<typename T>
  inline T bilinearInterpolation<T>::interpolateUnchk(const float row,
                                                      const float col) const {
    return interpolateUnchk(*this->theMatrix_,row,col);
  }

} // namespace lti

#endif
