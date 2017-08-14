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
 * $Id: ltiMergeYPbPrToImage.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */


#ifndef _LTI_MERGE_Y_Pb_Pr_TO_IMAGE_H_
#define _LTI_MERGE_Y_Pb_Pr_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Creates RGB values from given YPbPr values by merging float or ubyte
   * values to an rgbaPixel, merging channels(floats) or channel8s(ubytes) to an
   * Image
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
   * Here, the inverse transformation of lti::splitImageToYPbPr is followed:
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
   *     Pb \\
   *     Pr
   *   \end{bmatrix}
   * \f]
   * where M is the matrix given in lti::splitImageToYPbPr.
   *
   * If you know you have a YPbPr space but it was given to you as
   * YUV, then the equivalences are U=Pb and V=Pr.
   *
   * A way of noticing if you have a YPbPr color space is determining the range
   * of the values of each channel.  Y should be in [0,1], while Pr and Pb
   * should be in [-0.5,0.5].
   *
   * @ingroup gColor
   */
  class mergeYPbPrToImage : public mergeImage {
  public:

    /**
     * Constructor
     */
    mergeYPbPrToImage(void);

    
    /**
     * Destructor
     */
    ~mergeYPbPrToImage();

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
    mergeYPbPrToImage& copy(const mergeYPbPrToImage& other);
    
    /**
     * Alias for copy method.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYPbPrToImage& operator=(const mergeYPbPrToImage& other);

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual mergeYPbPrToImage* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual mergeYPbPrToImage* newInstance() const;

    /**
     * Merge channels Y, Pb, Pr to an image.
     *
     * @param Y the Y channel, i.e. black&white
     * @param Pb the Pb channel, chromatic 
     * @param Pr the Pr channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& Y,
                       const matrix<float>& Pb,
                       const matrix<float>& Pr,
                       image& img) const;

    /**
     * Merge  8-bit-channels Y, Pb, Pr to an image.
     *
     * @param Y the Y channel, i.e. black&white
     * @param Pb the Pb channel, chromatic 
     * @param Pr the Pr channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<ubyte>& Y,
                       const matrix<ubyte>& Pb,
                       const matrix<ubyte>& Pr,
                       image& img) const;

    /**
     * Merge the  values Y, Pb and Pr to a pixel.
     * @param Y the Y value, i.e. black&white
     * @param Pb the Pb value, chromatic 
     * @param Pr the Pr value, chromatic 
     * @param pixel the merged pixel
     */
    inline virtual bool apply(const float& Y,
		       const float& Pb,
		       const float& Pr,
		       rgbaPixel& pixel) const;

    /**
     * Merge the   8-bit-values Y, Pb and Pr to a pixel.
     *
     * @param Y the Y value, i.e. black&white
     * @param Pb the Pb  value, chromatic 
     * @param Pr the Pr value, chromatic 
     * @param pixel the merged pixel
     */ 
    inline virtual bool apply(const ubyte& Y,
                              const ubyte& Pb,
                              const ubyte& Pr,
                              rgbaPixel& pixel) const;

    /*
     * Merge the   8-bit-values Y, Pb and Pr to a pixel.
     *
     * This is a static method to do the conversion.  It is required by
     * other functors which need a fast access to the conversion without
     * requiring an instance.
     *
     * \waring Be careful while using this method, since its static nature
     * makes you responsible for the previous initialization of the LUTs.  For
     * that matter, just make sure that any instance of mergeYPbPrToImage is
     * created before calling this method.
     *
     * @param Y the Y value, i.e. black&white
     * @param Pb the Pb  value, chromatic 
     * @param Pr the Pr value, chromatic 
     * @param pixel the merged pixel
     */ 
    static bool convert(const ubyte& Y,
                        const ubyte& Pb,
                        const ubyte& Pr,
                        rgbaPixel& pixel);
    
  protected:

    /**
     * Initialize the Look-Up-Tables
     */
    virtual void initializeLUTs();

    /**
     * Look up tables to accelerate conversion YPbPr -> RGB
     */
    //@{
    /**
     * Partial Y results
     */
    static const int* lutY_;

    /**
     * Partial results with Pr (equivalent to V) for the red channel
     * computation.
     */
    static const int* lutVr_;

    /**
     * Partial results with Pb (equivalent to U) for the green channel
     * computation.
     */
    static const int* lutUg_;

    /**
     * Partial results with Pr (equivalent to V) for the green channel
     * computation.
     */
    static const int* lutVg_;

    /**
     * Partial results with Pb (equivalent to U) for the blue channel
     * computation.
     */
    static const int* lutUb_;
    //@}

    /**
     * Clip function
     *
     * Equivalent to min(255,max(0,val)) but maybe faster
     */
    inline static ubyte clip(const int val);

  };
}

#include "ltiMergeYPbPrToImage_inline.h"

#endif
