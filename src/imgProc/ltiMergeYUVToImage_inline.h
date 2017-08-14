/*
 * Copyright (C) 2003, 2004, 2005, 2006, 2007
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
j */ 

 
/**
 * \file   ltiMergeYUVToImage.h
 *         Merge YUV channels into a color image
 * \author Volker Schmirgel
 * \author Pablo Alvarado
 * \date   17.10.2003
 *
 * $Id: ltiMergeYUVToImage_inline.h,v 1.3 2007-04-05 22:56:59 alvarado Exp $
 */



namespace lti {

  //
  // -------------------------------------
  // Implementation of inline methods
  // -------------------------------------
  //

  inline ubyte mergeYUVToImage::clip(const int val) const {
    if (val>255) {
      return 255;
    }
    if (val<0) {
      return 0;
    }
    return static_cast<ubyte>(val);
  }

  // create rgbaPixel (ubyte)  from YUV float values
  inline bool mergeYUVToImage::apply(const float& c1,
                                     const float& c2,
                                     const float& c3,
                                     rgbaPixel& pixel) const {

    // The following coefficients are tuned to produce 0% of error of
    // RGB -> YUV -> RGB convertions.  Please DO NOT CHANGE!
    const float Y = 255.f*c1 + 0.5f;

    pixel.set(clip(static_cast<int>(Y + c3*290.67f)),
              clip(static_cast<int>(Y - c2*100.63f 
                                      - c3*148.06f)),
              clip(static_cast<int>(Y + c2*518.18f)),
              0);

    return true;
  }

  // merge 8-bit-values to create an rgbaPixel
  inline bool mergeYUVToImage::apply(const ubyte& c1,
                                     const ubyte& c2,
                                     const ubyte& c3,
                                     rgbaPixel& pixel) const {
    return mergeYPbPrToImage::convert(c1,c2,c3,pixel);
  }
}
