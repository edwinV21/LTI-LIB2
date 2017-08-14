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
 * $Id: ltiMergeYCbCrToImage.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */


#ifndef _LTI_MERGE_Y_Cb_Cr_TO_IMAGE_H_
#define _LTI_MERGE_Y_Cb_Cr_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Creates RGB values from given YCbCr values by merging float or ubyte
   * values to an rgbPixel, merging channels(floats) or channel8s(ubytes) to an
   * Image
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
   * - YCbCr: lti::mergeYCbCrToImage used by IEEE 1394 FireWire cameras
   * - YPbPr: lti::mergeYPbPrToImage used by some WebCams
   * - YUV:   lti::mergeYUVToImage   did they really meant to use this?
   *
   * Here, the inverse transformation of lti::splitImageToYCbCr is followed:
   *
   * \f[
   *   \begin{bmatrix}
   *     R \\
   *     G \\
   *     B
   *   \end{bmatrix}
   *   =
   *   M^{-1}
   *   \left(
   *     \begin{bmatrix}
   *       Y \\
   *       Cb \\
   *       Cr
   *     \end{bmatrix}
   *     -
   *     \begin{bmatrix}
   *       16 \\
   *       128 \\
   *       128
   *     \end{bmatrix}
   *   \right)
   * \f]
   * where M is the matrix given in lti::splitImageToYCbCr.
   *
   * If you know you have a YCbCr space but it was given to you as
   * YUV, then the equivalences are U=Cb and V=Cr.
   *
   * A way of noticing if you have a YCbCr color space is determining the range
   * of the values of each channel.  Y should be in [16,235], while Cr and Cb
   * should be in [16,240].  If you channel Y has values in [0,255] then you
   * should use mergeYPbPrToImage instead.
   *
   * @ingroup gColor
   */
  class mergeYCbCrToImage : public mergeImage {
  public:

    /**
     * Constructor
     */
    mergeYCbCrToImage(void);

    
    /**
     * Destructor
     */
    ~mergeYCbCrToImage();

    /**
     * Return the name of this type
     */
    const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYCbCrToImage& copy(const mergeYCbCrToImage& other);
    
    /**
     * Alias for copy method.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYCbCrToImage& operator=(const mergeYCbCrToImage& other);

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual mergeYCbCrToImage* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual mergeYCbCrToImage* newInstance() const;

    /**
     * Merge channels Y, Cr, Cb to an image.
     *
     * @param Y the Y channel, i.e. black&white
     * @param Cb the Cb channel, chromatic 
     * @param Cr the Cr channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& Y,
                       const matrix<float>& Cb,
                       const matrix<float>& Cr,
                       image& img) const;

    /**
     * Merge  8-bit-channels Y, U, V to an image.
     *
     * @param Y the Y channel, i.e. black&white
     * @param Cb the Cb channel, chromatic 
     * @param Cr the Cr channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<ubyte>& Y,
                       const matrix<ubyte>& Cb,
                       const matrix<ubyte>& Cr,
                       image& img) const;

    /**
     * Merge the  values Y, Cr and Cb to a pixel.
     * @param Y the Y value, i.e. black&white
     * @param Cb the Cb value, chromatic 
     * @param Cr the Cr value, chromatic 
     * @param pixel the merged pixel
     */
    inline virtual bool apply(const float& Y,
		       const float& Cb,
		       const float& Cr,
		       rgbaPixel& pixel) const;

    /**
     * Merge the   8-bit-values Y, U and V to a pixel.
     *
     * @param Y the Y value, i.e. black&white
     * @param Cb the Cb  value, chromatic 
     * @param Cr the Cr value, chromatic 
     * @param pixel the merged pixel
     */ 
    inline virtual bool apply(const ubyte& Y,
                              const ubyte& Cb,
                              const ubyte& Cr,
                              rgbaPixel& pixel) const;
  protected:

    /**
     * Initialize the Look-Up-Tables
     */
    virtual void initializeLUTs();

    /**
     * Look up tables to accelerate conversion YUV -> RGB
     */
    //@{
    /**
     * Partial Y results
     */
    static const int* lutY_;

    /**
     * Partial results with Cr (equivalent to V) for the red channel
     * computation.
     */
    static const int* lutVr_;

    /**
     * Partial results with Cb (equivalent to U) for the green channel
     * computation.
     */
    static const int* lutUg_;

    /**
     * Partial results with Cr (equivalent to V) for the green channel
     * computation.
     */
    static const int* lutVg_;

    /**
     * Partial results with Cb (equivalent to U) for the blue channel
     * computation.
     */
    static const int* lutUb_;
    //@}

    /**
     * Clip function
     *
     * Equivalent to min(255,max(0,val)) but maybe faster
     */
    inline ubyte clip(const int val) const;

  };
}

#include "ltiMergeYCbCrToImage_inline.h"

#endif
