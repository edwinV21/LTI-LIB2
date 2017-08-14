/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiMergeRGBToImage.h
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 * 
 * $Id: ltiMergeRGBToImage.h,v 1.2 2009-10-31 14:08:46 alvarado Exp $
 */


#ifndef _LTI_MERGE_RGB_TO_IMAGE_H_
#define _LTI_MERGE_RGB_TO_IMAGE_H_

#include "ltiMergeImage.h"
#include "ltiRGBAPixel.h"

namespace lti {

  /**
   * Merge RGB channels
   *
   * Merge the three given channels in a color image.
   * @see splitImageToRGB
   * @ingroup gColor
   */
  class mergeRGBToImage : public mergeImage {
  public:

    /**
     * Default constructor
     */
    mergeRGBToImage();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;
      
    /**
     * Returns a pointer to a clone of the parameters
     */
    virtual mergeRGBToImage* clone() const;
      
    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual mergeRGBToImage* newInstance() const;

    /**
     * merge the channels red, green and blue to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param img the merged image
     */
    virtual bool apply(const matrix<float>& red,
                       const matrix<float>& green,
                       const matrix<float>& blue,
                       image& img) const;

    /**
     * merge the channels red, green and blue to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param img the merged image
     */
    virtual bool apply(const matrix<ubyte>& red,
                       const matrix<ubyte>& green,
                       const matrix<ubyte>& blue,
                       image& img) const;

    /**
     * Merge the channels red, green, blue and the dummy (or alpha) to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha the alpha or dummy channel
     * @param img the merged image
     */
    bool apply(const matrix<float>& red,
               const matrix<float>& green,
               const matrix<float>& blue,
               const matrix<float>& alpha,
               image& img) const;

    /**
     * Merge the channels red, green, blue and the dummy (or alpha) to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha the alpha or dummy channel
     * @param img the merged image
     */
    bool apply(const matrix<ubyte>& red,
               const matrix<ubyte>& green,
               const matrix<ubyte>& blue,
               const matrix<ubyte>& alpha,
               image& img) const;

    /**
     * merge the values red, green and blue to a pixel.
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     * @param pixel the pixel to be splitted
     */
    virtual bool apply(const float& red,
		       const float& green,
		       const float& blue,
		       rgbaPixel& pixel) const;

    /**
     * merge the values red, green and blue to a pixel
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& red,
		       const ubyte& green,
		       const ubyte& blue,
		       rgbaPixel& pixel) const;
  };
}

#endif

