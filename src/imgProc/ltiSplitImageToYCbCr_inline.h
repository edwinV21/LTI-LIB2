/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007
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
 * \file   ltiSplitImageToYCbCr_inline.h
 *         Split color image to YCbCr
 * \author Pablo Alvarado
 * \date   04.01.2007
 *
 * $Id: ltiSplitImageToYCbCr_inline.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */

namespace lti {

  //
  // --------------------------------
  // implementation of inline methods
  // --------------------------------
  //

  // split pixel into float values (between 0 and 1)
  inline bool splitImageToYCbCr::apply(const rgbaPixel& pixel,
                                       float& c1,
                                       float& c2,
                                       float& c3) const {
    
    const float red   = static_cast<float>( pixel.getRed() );
    const float green = static_cast<float>( pixel.getGreen() );
    const float blue  = static_cast<float>( pixel.getBlue() );
    
    c1 = (red   *( 0.2567882352941177f/255.0f) +
          green *( 0.5041294117647058f/255.0f) +
          blue  *( 0.0979058823529412f/255.0f) + 16.f/255.0f); // Y
    c2 = (red   *(-0.1482235294117647f/255.0f) +
          green *(-0.2909921568627451f/255.0f) +
          blue  *( 0.4392156862745098f/255.0f) + 128.f/255.0f); // U=Cb
    c3 = (red   *( 0.4392156862745098f/255.0f) +
          green *(-0.3677882352941176f/255.0f) +
          blue  *(-0.0714274509803921f/255.0f) + 128.f/255.0f); // V=Cr
    
    return true;
  }

  // split pixel into 8-bit values (between 0 and 255)
  // N.B.: when casting the transformation result to unsigned shorts
  // (8-bit channel) major rounding errors will occur.
  // As a result, the merging operation might
  // produce negative values or values > 1,  which are truncated subsequently.
  // When accurate Y, U and V values are required, prefer float values!
  inline bool splitImageToYCbCr::apply(const rgbaPixel& pixel,
                                       ubyte& c1,
                                       ubyte& c2,
                                       ubyte& c3) const {
    const int r = pixel.getRed();
    const int g = pixel.getGreen();
    const int b = pixel.getBlue();

    // use fixed point arithmetic for faster code

    // The following coefficients are tuned to produce the smallest possible
    // error of RGB -> YCbCr -> RGB convertions.  Please DO NOT CHANGE! 
   
    // The error cannot be further reduced as the ubyte precision looses many
    // information when converting RGB->YCbCr.  With the given coefficients
    // 2660449 values of all 2^24 available were perfectly retrieved (18.8%),
    // an the mean deviation error is 0.845 (RGB color space L2 distance).

    // The values are the fixed point arithmetic with 16 bits decimals.
    // The offsets are 16 or 128 multiplied by 65536, but 32768 is added
    // to force correct rounding.
    c1 = static_cast<ubyte>((16829*r + 33039*g +  6416*b + 1081344)>>16);
    c2 = static_cast<ubyte>((-9714*r - 19070*g + 28784*b + 8421376)>>16);
    c3 = static_cast<ubyte>((28784*r - 24103*g -  4681*b + 8421376)>>16);

    return true;
  }

  // ---------------------------------------
  // end of implementation of inline methods
  // ---------------------------------------

}
