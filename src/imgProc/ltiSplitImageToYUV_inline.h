/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007
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
 * \file   ltiSplitImageToYUV.h
 *         Split color image to YUV
 * \author Volker Schmirgel
 * \author Pablo Alvarado
 * \date   17.12.2002
 *
 * $Id: ltiSplitImageToYUV_inline.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */

namespace lti {

  //
  // --------------------------------
  // implementation of inline methods
  // --------------------------------
  //

  // split pixel into float values (between 0 and 1)
  inline bool splitImageToYUV::apply(const rgbaPixel& pixel,
                                     float& c1,
                                     float& c2,
                                     float& c3) const {
    
    const float red   = static_cast<float>( pixel.getRed() );
    const float green = static_cast<float>( pixel.getGreen() );
    const float blue  = static_cast<float>( pixel.getBlue() );
    
    c1 =  red   *  0.299f/255.0f  +
          green *  0.587f/255.0f  +
          blue  *  0.114f/255.0f;        // Y
    c2 =  red   *(-0.147141189f/255.0f) +
          green *(-0.288869157f/255.0f) +
          blue  *  0.436010346f/255.0f;  // U
    c3 =  red   *  0.614975383f/255.0f  +
          green *(-0.514965121f/255.0f) +
          blue  *(-0.100010262f/255.0f); // V

    return true;
  }

  // split pixel into 8-bit values (between 0 and 255)
  // N.B.: when casting the transformation result to unsigned shorts
  // (8-bit channel) major rounding errors will occur.
  // As a result, the merging operation might
  // produce negative values or values > 1,  which are truncated subsequently.
  // When accurate Y, U and V values are required, prefer float values!
  inline bool splitImageToYUV::apply(const rgbaPixel& pixel,
                                     ubyte& c1,
                                     ubyte& c2,
                                     ubyte& c3) const {

    const int r = pixel.getRed();
    const int g = pixel.getGreen();
    const int b = pixel.getBlue();

    // use fixed point arithmetic for faster code

    // The following coefficients are tuned to produce the smallest possible
    // error of RGB -> YPbPr -> RGB convertions.  Please DO NOT CHANGE! 
   
    // The error cannot be further reduced as the ubyte precision looses many
    // information when converting RGB->YPbPr.  With the given coefficients
    // 3999903 values of all 2^24 available were perfectly retrieved (31.3%),
    // an the mean deviation error is 0.761 (RGB color space L2 distance).

    // The values are the fixed point arithmetic with 16 bits decimals.
    // The offset 128 is multiplied by 65536, but 32768 is added also
    // to force correct rounding.
    c1 = static_cast<ubyte>(( 19595*r + 38470*g +  7471*b +   32768)>>16);
    c2 = static_cast<ubyte>((-11058*r - 21710*g + 32768*b + 8421376)>>16);
    c3 = static_cast<ubyte>(( 32768*r - 27439*g -  5329*b + 8421376)>>16);

    return true;
  }

  // ---------------------------------------
  // end of implementation of inline methods
  // ---------------------------------------

}
