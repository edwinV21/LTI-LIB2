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
 * $Id: ltiSplitImageToYUV.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */


#ifndef _LTI_SPLIT_IMAGE_TO_Y_U_V_H_
#define _LTI_SPLIT_IMAGE_TO_Y_U_V_H_

#include "ltiSplitImage.h"

namespace lti {
  /**
   * Computes the YUV values from a given RGB color representation (rgbaPixel).
   *
   * In the literature, technical and scientific, there is a confusion between
   * the color spaces YUV, YCrCb and YPbPr.  Poynton in
   * http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html explains that
   * YUV is usually never correctly meant, because the color space normally
   * used for component digital video is the YCrCb (ITU-RS601 or CCIR-601).
   * Other devices use the YPbPr, but the "real" YUV is usually not employed.
   *
   * The LTI-Lib provides all three spaces:
   *
   * - YCrCb: lti::splitImageToYCbCr used by IEEE 1394 FireWire cameras
   * - YPbPr: lti::splitImageToYPbPr used by some WebCams
   * - YUV:   lti::splitImageToYUV   did they really meant to use this?
   *
   * This functor splits an RGB images into the color space YUV as described
   * in the Poynton reference given above.
   *
   * \f[
   * \begin{bmatrix}
   * Y \\
   * U \\
   * V
   * \end{bmatrix}
   * =
   * \begin{bmatrix}
   *  0.299       &  0.587       &  0.114       \\
   * -0.147141189 & -0.288869157 &  0.436010346 \\
   *  0.614975383 & -0.514965121 & -0.100010262 
   * \end{bmatrix}
   * \begin{bmatrix}
   * R \\
   * G \\
   * B
   * \end{bmatrix}
   * \f]
   *
   * Here, RGB are values in the intervall [0,1].  Within this range, the
   * excursion for Y is also from 0.0 to 1.0, the excursion of U is from -0.436
   * to 0.436 and V varies from -0.615 to 0.615.  These ranges are respected in
   * the implentation for \c float types (lti::channel). 
   *
   * If you use \c ubyte (lti::matrix<ubyte>), then the values for U and V are
   * linearly mapped from 0 to 1, which results in the mapping
   *
   * \code
   * Y =  0.299*R + 0.587*G + 0.114*B 
   * U = -0.169*R - 0.331*G + 0.500*B + 128.0 
   * V =  0.500*R - 0.419*G - 0.081*B + 128.0 
   * \endcode
   *
   * The former mapping is equivalent to the color space YPbPr scaled in such a
   * way that "black" is mapped to Y=0 and "white" to Y=255, and the Pb and Pr
   * channels, which acquire values from -0.5 to 0.5, are linearly mapped to
   * the inteval [0,255] as well.  In other words, for \c ubyte operations it
   * is equivalent to use this functor or lti::splitImageToYPbPr (which is
   * maybe better named as the value ratios are at least kept there).
   *
   * @ingroup gColor
   */
  class splitImageToYUV : public splitImage {
  public:

    /**
     * default constructor
     */
    splitImageToYUV();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    splitImageToYUV(const splitImageToYUV& other);

    /**
     * destructor
     */
    virtual ~splitImageToYUV();

    /**
     * returns the name of this type ("splitImageToYUV")
     */
    const std::string& name() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToYUV& copy(const splitImageToYUV& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToYUV& operator=(const splitImageToYUV& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual splitImageToYUV* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual splitImageToYUV* newInstance() const;


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

#include "ltiSplitImageToYUV_inline.h"

#endif
