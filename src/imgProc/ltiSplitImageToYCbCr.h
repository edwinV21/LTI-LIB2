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
 * \file   ltiSplitImageToYCbCr.h
 *         Split color image to YCbCr
 * \author Pablo Alvarado
 * \date   04.01.2007
 *
 * $Id: ltiSplitImageToYCbCr.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */


#ifndef _LTI_SPLIT_IMAGE_TO_Y_Cb_Cr_H_
#define _LTI_SPLIT_IMAGE_TO_Y_Cb_Cr_H_

#include "ltiSplitImage.h"

namespace lti {
  /**
   * Computes the YCbCr values from a given RGB color representation
   * (rgbaPixel).
   *
   * In the literature, technical and scientific, there is often confusion
   * among the color spaces YUV, YCbCr and YPbPr.  Poynton in
   * http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html explains that
   * YUV is usually never correctly meant, because the color space normally
   * used for component digital video is the YCbCr (ITU-RS601 or CCIR-601).
   * Other devices use the YPbPr, but the "real" YUV is rarely employed.
   *
   * The LTI-Lib provides all three spaces:
   *
   * - YCbCr: lti::splitImageToYCbCr used by IEEE 1394 FireWire cameras
   * - YPbPr: lti::splitImageToYPbPr used by some WebCams
   * - YUV:   lti::splitImageToYUV   did they really meant to use this?
   *
   * This functor splits an RGB images into the color space YCbCr (ITU-RS601)
   * using the conversion equations given by the above mentioned reference:
   *
   * \f[
   * \begin{bmatrix}
   * Y \\
   * Cb \\
   * Cr
   * \end{bmatrix}
   * =
   * \begin{bmatrix}
   * 16  \\
   * 128 \\
   * 128
   * \end{bmatrix}
   * +
   * \frac{1}{255}
   * \begin{bmatrix}
   * 65.481  & 128.553 & 24.966  \\
   * -37.797 & -74.203 & 112     \\
   * 112     & -93.786 & -18.214 
   * \end{bmatrix}
   * \begin{bmatrix}
   * R \\
   * G \\
   * B
   * \end{bmatrix}
   * \f]
   *
   * Note that the Y values will have an excursion of 219 with an offset of 16,
   * while the Cb and Cr values will have an excursion of +/-112 with an offset
   * of 128.
   *
   * The Cb value corresponds to U, and Cr to V, in case you need to provide
   * them with "wrong" names.
   *
   * @ingroup gColor
   */
  class splitImageToYCbCr : public splitImage {
  public:

    /**
     * default constructor
     */
    splitImageToYCbCr();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    splitImageToYCbCr(const splitImageToYCbCr& other);

    /**
     * destructor
     */
    virtual ~splitImageToYCbCr();

    /**
     * returns the name of this type ("splitImageToYCbCr")
     */
    const std::string& name() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToYCbCr& copy(const splitImageToYCbCr& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToYCbCr& operator=(const splitImageToYCbCr& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual splitImageToYCbCr* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual splitImageToYCbCr* newInstance() const;


    /**
     *  split pixel into float values (between 0 and 1)
     */
    inline virtual bool apply(const rgbaPixel& pixel,
                              float& c1,
                              float& c2,
                              float& c3) const; 

    /**
     * Split pixel into 8-bit values (between 0 and 255)
     *
     * N.B.: when casting the transformation result to unsigned shorts
     * (8-bit channel) major rounding errors will occur.
     *
     * As a result, the merging operation might produce negative
     * values or values > 1, which are truncated subsequently.
     *
     * When accurate Y, U and V values are required, prefer float values
     */
    inline virtual bool apply(const rgbaPixel& pixel,
                              ubyte& c1,
                              ubyte& c2,
                              ubyte& c3) const;  

    /**
     *  split pixel into float channels
     */   
    virtual bool apply(const matrix<rgbaPixel>& img,
                       matrix<float>& c1,
                       matrix<float>& c2,
		       matrix<float>& c3) const;
    
    /**
     * Split image into 8-bit channels.
     *
     * N.B.: when casting the transformation result to unsigned shorts
     * (8-bit channel) major rounding errors will occur.
     *
     * As a result, the merging operation might produce negative
     * values or values > 1, which are truncated subsequently.  When
     * accurate Y, U and V values are required, prefer float values.
     */
    virtual bool apply(const matrix<rgbaPixel>& img,
                       matrix<ubyte>& c1,
                       matrix<ubyte>& c2,
		       matrix<ubyte>& c3) const;

  };

}

#include "ltiSplitImageToYCbCr_inline.h"

#endif
