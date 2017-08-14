/*
 * Copyright (C) 2003, 2004, 2005, 2006, 2007
 * Department of Electronics, ITCR, Costa Rica
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
 * \file   ltiMergeYCbCrToImage.h
 *         Merge YCbCr channels to color image
 * \author Pablo Alvarado
 * \date   04.01.2007
 *
 * $Id: ltiMergeYCbCrToImage_inline.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */

namespace lti {

  //
  // -------------------------------------
  // Implementation of inline methods
  // -------------------------------------
  //

  inline ubyte mergeYCbCrToImage::clip(const int val) const {
    if (val>255) {
      return 255;
    }
    if (val<0) {
      return 0;
    }
    return static_cast<ubyte>(val);
  }

  // create rgbaPixel (ubyte)  from YUV float values
  inline bool mergeYCbCrToImage::apply(const float& c1,
                                       const float& c2,
                                       const float& c3,
                                       rgbaPixel& pixel) const {

    // The values here are chosen to exactly invert the method followed in
    // splitImageToYCrBr
    static const float offuv = 128.f/255.f;
    static const float offY  = 16.f/255.f;

    // The following coefficients are tuned to produce 0% of error of
    // RGB -> YCbCr -> RGB convertions.  Please DO NOT CHANGE!

    const float Y = (c1-offY)*296.917808219178f + 0.5f;
    const float U = c2-offuv;
    const float V = c3-offuv;

    pixel.set(clip(static_cast<int>(Y + V*406.986856270605f)),
              clip(static_cast<int>(Y - U*99.8994476850698f 
                                      - V*207.306914501362f)),
              clip(static_cast<int>(Y + U*514.394323086897f)),
              0);

    return true;
  }

  // merge 8-bit-values to create an rgbaPixel
  inline bool mergeYCbCrToImage::apply(const ubyte& c1,
                                       const ubyte& c2,
                                       const ubyte& c3,
                                       rgbaPixel& pixel) const {

    // The following coefficients are tuned to produce the smallest possible
    // error of RGB -> YCbCr -> RGB convertions.  Please DO NOT CHANGE! 
   
    // The error cannot be further reduced as the ubyte precision looses many
    // information when converting RGB->YCbCr.  With the given coefficients
    // 2660449 values of all 2^24 available were perfectly retrieved (18.8%),
    // an the mean deviation error is 0.845 (RGB color space L2 distance).

    // the 32768 is half the 16 bit precision and is added to force a correct
    // rounding
    
    // The next lines are 100% equivalent to this, but using LUT, which
    // requires approximatelly 85% of the direct computation time.
    //
    //     const int Y = (static_cast<int>(c1)- 16)*76309 + 32768;
    //     const int U = (static_cast<int>(c2)-128);
    //     const int V = (static_cast<int>(c3)-128);
    
    //     pixel.set(clip((Y              + 104597 * V)>>16),
    //               clip((Y -  25675 * U -  53279 * V)>>16),
    //               clip((Y + 132201 * U             )>>16),
    //               0);

    const int Y = lutY_[c1];
    pixel.set(clip( (Y              + lutVr_[c3])>>16),
              clip( (Y + lutUg_[c2] + lutVg_[c3])>>16),
              clip( (Y + lutUb_[c2]             )>>16),
              0);

    return true;
  }
}
