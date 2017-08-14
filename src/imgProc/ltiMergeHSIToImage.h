/*
 * Copyright (C) 1998-2006
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
 * \file   ltiMergeHSIToImage.h
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.99
 * 
 * $Id: ltiMergeHSIToImage.h,v 1.3 2006-01-06 05:02:10 alvarado Exp $
 */

#ifndef _LTI_MERGE_HSI_TO_IMAGE_H_
#define _LTI_MERGE_HSI_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge HSI (Hue Saturation Intensity) channels.
   *
   * It is assumed that all channels have "valid" values, i.e., if the inputs
   * are of type lti::channel, the Hue has values between 0.0 and 1.0 (meaning
   * 0 to 360 degrees), Saturation and Intensity between 0.0 and 1.0.  For
   * lti::channel8, the possible value ranges are always valid.
   * 
   * If any component is not valid, then unpredictible results have to be
   * expected, including segmentation faults, as the computations use some LUT
   * to improve speed.
   *
   * The transformation from HSI to RGB depends on the value range of hue (H).
   * It first computes chromaticity values r,g,b:
   *
   * If \f$0^\circ \leq H < 120^\circ\f$
   *
   * \f[
   *   \begin{aligned}
   *     r &= \frac{1}{3}\left[1+\frac{S \cos(H)}{\cos(60^\circ-H)}\right] \\
   *     g &= 1-(r+b) \\
   *     b &= \frac{1}{3}(1-S)
   *   \end{aligned}
   * \f]
   *
   * If \f$120^\circ \leq H < 240^\circ\f$
   *
   * \f[
   *   \begin{aligned}
   *     r &= \frac{1}{3}(1-S) \\
   *     g &= \frac{1}{3}\left[1+\frac{S \cos(H-120^\circ)} 
   *                                  {\cos(180^\circ-H)}\right] \\
   *     b &= 1-(r+g)
   *   \end{aligned}
   * \f]
   *
   * If \f$240^\circ \leq H < 360^\circ\f$
   *
   * \f[
   *   \begin{aligned}
   *     g &= \frac{1}{3}(1-S) \\
   *     b &= \frac{1}{3}\left[1+\frac{S \cos(H-240^\circ)}
   *                                  {\cos(300^\circ-H)}\right] \\
   *     r &= 1-(g+b)
   *   \end{aligned}
   * \f]
   *
   * With these chromaticity values, the RGB tuple can easily be computed as
   *
   * \f[ \begin{aligned} R &= 3rI \\ G &= 3gI \\ B &= 3bI \\ \end{aligned} \f]
   *
   * @see lti::splitImageToHSI
   *
   * @ingroup gColor
   */
  class mergeHSIToImage : public mergeImage {
  public:

    /**
     * Constructor
     */
    mergeHSIToImage(void);

    /**
     * return the name of this type
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual mergeHSIToImage* clone() const;

    /**
     * returns a pointer to a new instance of the functor.
     */
    virtual mergeHSIToImage* newInstance() const;

    /**
     * merge hue channel H, saturation S and intensity
     * channel I to an image
     * @param H the hue channel
     * @param S the saturation channel
     * @param I the intensity channel
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& H,
                       const matrix<float>& S,
                       const matrix<float>& I,
                       image& img) const;


    /**
     * merge hue channel H, saturation S and intensity
     * channel I to an image
     * @param H the hue channel
     * @param S the saturation channel
     * @param I the intensity channel
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<ubyte>& H,
                       const matrix<ubyte>& S,
                       const matrix<ubyte>& I,
                       image& img) const;

    /**
     * merge the hue value H, saturation S and intensity value I
     * to a pixel
     * @param H the hue value
     * @param S the saturation value
     * @param I the intensity value
     * @param pixel the merged pixel
     */
    virtual bool apply(const float& H,
		       const float& S,
		       const float& I,
		       rgbaPixel& pixel) const;

    /**
     * merge the hue value H, saturation S and intensity value I
     * to a pixel
     * @param H the hue value
     * @param S the saturation value
     * @param I the intensity value
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& H,
		       const ubyte& S,
		       const ubyte& I,
		       rgbaPixel& pixel) const;
    
  private:
    /**
     * Some constants required to access the LUT
     */
    enum {
      Reds   = 360,  // at least 120 degrees 
      Greens = 2*Reds,
      Blues  = 3*Reds,
      Sats   = 384,  // at least 256
      SatsMax = Sats-1
    };

    /**
     * matrix for HSI->RGB-conversion
     */
    static const matrix<ubyte>* DeHSI_;

    /**
     * Fix the representation in case RGB are not in a valid range.
     *
     * This method expects:
     * @param R red greater 0
     * @param G green greater 0
     * @param B blue greater 0
     *
     * The values R,G,B will be left unmodified if they are in a correct
     * range.
     */
    inline void fix(float& R,float& G,float& B) const;
                    
    
  };

}

#endif

