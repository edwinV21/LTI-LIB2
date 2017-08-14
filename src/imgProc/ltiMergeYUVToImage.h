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
 * $Id: ltiMergeYUVToImage.h,v 1.3 2007-04-05 22:56:59 alvarado Exp $
 */


#ifndef _LTI_MERGE_Y_U_V_TO_IMAGE_H_
#define _LTI_MERGE_Y_U_V_TO_IMAGE_H_

#include "ltiMergeImage.h"
#include "ltiMergeYPbPrToImage.h"

namespace lti {

  /**
   * Compute RGB values from given YUV values by merging float or ubyte values
   * to an rgbaPixel, merging channels(floats) or matrix<ubyte>s(ubytes) to an Image
   *
   * In the literature, technical and scientific, there is often confusion
   * among the color spaces YUV, YPbPr and YPbPr.  Poynton in
   * http://www.poynton.com/notes/colour_and_gamma/ColorFAQ.html explains that
   * YUV is usually never correctly meant, because the color space normally
   * used for component digital video is the YCbCr (ITU-RS601 or CCIR-601).
   * Other devices use the YPbPr, but the "real" YUV is rarely employed.
   *
   * The LTI-Lib provides all three spaces:
   *
   * - YCbCr: lti::mergeYCbCrToImage used by IEEE 1394 FireWire cameras
   * - YPbPr: lti::mergeYPbPrToImage used by some WebCams
   * - YUV:   lti::mergeYUVToImage   did they really meant to use this?
   *
   * Here, the inverse transformation of lti::splitImageToYUV is followed
   *
   * \f[
   *   \begin{bmatrix}
   *     R \\
   *     G \\
   *     B
   *   \end{bmatrix}
   *   =
   *   M^{-1}
   *   \begin{bmatrix}
   *     Y \\
   *     U \\
   *     V
   *   \end{bmatrix}
   * \f]
   * where M is the matrix given in lti::splitImageToYUV.
   *
   * If you use \c ubyte values, then this functor is equivalent to
   * lti::mergeYPbPrToImage, as the U and V values have to be linearly mapped
   * to make use of the limited range from 0 to 255.
   *
   * @ingroup gColor
   */
  class mergeYUVToImage : public mergeImage {
  public:

    /**
     * constructor
     */
    mergeYUVToImage(void);

    
    /**
     * destructor
     */
    ~mergeYUVToImage();

    /**
     * return the name of this type
     */
    const std::string& name() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYUVToImage& copy(const mergeYUVToImage& other);
    
    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYUVToImage& operator=(const mergeYUVToImage& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual mergeYUVToImage* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual mergeYUVToImage* newInstance() const;

    /**
     * merge channels Y, U, V to an image
     * @param Y the Y channel, i.e. black&white
     * @param U the U channel, chromatic 
     * @param V the V channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& Y,
                       const matrix<float>& U,
                       const matrix<float>& V,
                       image& img) const;

    /**
     * merge  8-bit-channels Y, U, V to an image
     * @param Y the Y channel, i.e. black&white
     * @param U the U channel, chromatic 
     * @param V the V channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<ubyte>& Y,
                       const matrix<ubyte>& U,
                       const matrix<ubyte>& V,
                       image& img) const;

    /**
     * merge the  values Y, U and V
     * to a pixel
     * @param Y the Y value, i.e. black&white
     * @param U the U  value, chromatic 
     * @param V the V value, chromatic 
     * @param pixel the merged pixel
     */
    inline virtual bool apply(const float& Y,
		       const float& U,
		       const float& V,
		       rgbaPixel& pixel) const;

    /**
     * merge the   8-bit-values Y, U and V
     * to a pixel
     * @param Y the Y value, i.e. black&white
     * @param U the U  value, chromatic 
     * @param V the V value, chromatic 
     * @param pixel the merged pixel
     */ 
    inline virtual bool apply(const ubyte& Y,
                              const ubyte& U,
                              const ubyte& V,
                              rgbaPixel& pixel) const;

  protected:

    /**
     * Clip function
     *
     * Equivalent to min(255,max(0,val)) but maybe faster
     */
    inline ubyte clip(const int val) const;
  };

}

#include "ltiMergeYUVToImage_inline.h"

#endif
