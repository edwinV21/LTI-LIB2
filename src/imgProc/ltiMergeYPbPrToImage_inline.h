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
 * \file   ltiMergeYPbPrToImage.h
 *         Merge YPbPr channels into a color image.
 * \author Pablo Alvarado
 * \date   04.01.2007
 *
 * $Id: ltiMergeYPbPrToImage_inline.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */

namespace lti {

  //
  // -------------------------------------
  // Implementation of inline methods
  // -------------------------------------
  //

  inline ubyte mergeYPbPrToImage::clip(const int val) {
    if (val>255) {
      return 255;
    }
    if (val<0) {
      return 0;
    }
    return static_cast<ubyte>(val);
  }

  // create rgbaPixel (ubyte)  from YUV float values
  inline bool mergeYPbPrToImage::apply(const float& c1,
                                       const float& c2,
                                       const float& c3,
                                       rgbaPixel& pixel) const {

    // The following coefficients are tuned to produce 0% of error of
    // RGB -> YPbPr -> RGB convertions.  Please DO NOT CHANGE!
    const float Y = c1*255.f + 0.5f;

    pixel.set(clip(static_cast<int>(Y + c3*357.509895107622f)),
              clip(static_cast<int>(Y - c2* 87.7545979321608f 
                                      - c3*182.104719673362f)),
              clip(static_cast<int>(Y + c2*451.860016848823f)),
              0);

    return true;
  }

  // merge 8-bit-values to create an rgbaPixel
  inline bool mergeYPbPrToImage::convert(const ubyte& c1,
                                         const ubyte& c2,
                                         const ubyte& c3,
                                         rgbaPixel& pixel) {

    // The following coefficients are tuned to produce the smallest possible
    // error of RGB -> YPbPr -> RGB convertions.  Please DO NOT CHANGE! 
   
    // The error cannot be further reduced as the ubyte precision looses many
    // information when converting RGB->YPbPr.  With the given coefficients
    // 3999903 values of all 2^24 available were perfectly retrieved (31.3%),
    // an the mean deviation error is 0.761 (RGB color space L2 distance).

    // The 32768 is half the 16 bit precision and is added to force a correct
    // rounding.
    
    // The next lines are 100% equivalent to this, but using LUT, which
    // requires approximatelly 85% of the direct computation time.
    //
    //     const int Y =  static_cast<int>(c1)*65536 + 32768;
    //     const int U = (static_cast<int>(c2)-128);
    //     const int V = (static_cast<int>(c3)-128);
    
    //     pixel.set(clip((Y              +  91881 * V)>>16),
    //               clip((Y -  22553 * U -  46802 * V)>>16),
    //               clip((Y + 116130 * U             )>>16),
    //               0);

    const int Y = lutY_[c1];
    pixel.set(clip( (Y              + lutVr_[c3])>>16),
              clip( (Y + lutUg_[c2] + lutVg_[c3])>>16),
              clip( (Y + lutUb_[c2]             )>>16),
              0);

    return true;
  }

  // merge 8-bit-values to create an rgbaPixel
  inline bool mergeYPbPrToImage::apply(const ubyte& c1,
                                       const ubyte& c2,
                                       const ubyte& c3,
                                       rgbaPixel& pixel) const {
    // alias for convert, so just call it
    return convert(c1,c2,c3,pixel);
  }
}
