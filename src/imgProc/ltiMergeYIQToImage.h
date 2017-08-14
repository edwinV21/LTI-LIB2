/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiMergeYIQToImage.h
 *         Merge YIQ channels into a color image
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiMergeYIQToImage.h,v 1.1 2007-04-05 22:56:36 alvarado Exp $
 */


#ifndef _LTI_MERGE_YIQ_TO_IMAGE_H_
#define _LTI_MERGE_YIQ_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge YIQ channels (Luminance, Inphase, Quadrature)
   *
   * This is an almost forgotten color space, as the standards use now
   * the YCbCr, YPbPr or YUV spaces.
   *
   * @see lti::mergeYUVToImage, lti::mergeYCbCrToImage, lti::mergeYPbPrToImage
   * @see lti::splitImageToYIQ
   *
   * @ingroup gColor
   */
  class mergeYIQToImage : public mergeImage  {
  public:

    /**
     * returns the name of this type
     */
    const std::string& name() const;

    /**
     * returns a poinbter to a clone of the functor
     */
    virtual mergeYIQToImage* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual mergeYIQToImage* newInstance() const;

    /**
     * merge the Y,I,Q channel to an image
     * YIQ merging requires unnormalized channels !
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     * @param img the merged image
     */
    virtual bool apply(const matrix<float>& Y,
                       const matrix<float>& I,
                       const matrix<float>& Q,
                       image& img) const;

    /**
     * merge the Y,I,Q channel to an image
     * YIQ merging requires unnormalized channels !
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     * @param img the merged image
     */
    virtual bool apply(const matrix<ubyte>& Y,
                       const matrix<ubyte>& I,
                       const matrix<ubyte>& Q,
                       image& img) const;

    /**
     * merge the Y,I,Q values to a pixel
     * YIQ merging requires unnormalized values !
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     * @param pixel the merged pixel
     */
    virtual bool apply(const float& Y,
                       const float& I,
                       const float& Q,
                       rgbaPixel& pixel) const;

    /**
     * merge the Y,I,Q values to a pixel
     * YIQ merging requires unnormalized values !
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& Y,
                       const ubyte& I,
                       const ubyte& Q,
                       rgbaPixel& pixel) const;
  };
}

#endif
