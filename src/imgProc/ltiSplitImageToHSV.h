/*
 * Copyright (C) 1998 - 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2012
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiSplitImageToHSV.h
 *         Contains the functor splitImageToHSV which splits images into hue
 *         saturation and value.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToHSV.h,v 1.1 2012-09-09 02:26:29 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_HSV_H_
#define _LTI_SPLIT_IMAGE_TO_HSV_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image into its Hue - Saturation - Value channels.
   *
   * The HSV color space is obtained by an angular estimation as hue, and the
   * saturation and value defined with extrema of the RGB channels.
   *
   * For the backtransformation see lti::mergeHSVToImage
   *
   * @ingroup gColor
   */
  class splitImageToHSV : public splitImage {
  public:
    /**
     * Constructor
     */
    splitImageToHSV();

    /**
     * Copy Constructor
     */
    splitImageToHSV(const splitImageToHSV& other);

    /**
     * Destructor
     */
    virtual ~splitImageToHSV();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToHSV& copy(const splitImageToHSV& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToHSV& operator=(const splitImageToHSV& other);

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual splitImageToHSV* clone() const;

    /**
     * returns a pointer to a new instance of the functor.
     */
    virtual splitImageToHSV* newInstance() const;

    /**
     * split the image in hue channel H, saturation S and value
     * channel V.
     * The values of each image will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel::value_type>& H,
                       matrix<channel::value_type>& S,
                       matrix<channel::value_type>& V) const;

    /**
     * split the image in hue channel H, saturation S and value
     * channel V.
     * The values of each image will be between 0 and 255
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel8::value_type>& H,
                       matrix<channel8::value_type>& S,
                       matrix<channel8::value_type>& V) const;

    /**
     * split the pixel in hue value H, saturation S and value
     * value V.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param V the value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       float& H,
                       float& S,
                       float& V) const;

    /**
     * split the pixel in hue value H, saturation S and value
     * value V.
     * The values of each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param V the value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& H,
                       ubyte& S,
                       ubyte& V) const;

    /**
     * return the hue of the image.  If you need also the saturation and
     * the value please use the apply methods, which are much faster!
     */
    bool extractHue(const matrix<image::value_type>& img, 
                    matrix<channel::value_type>& hue) const;

    /**
     * return the hue of the image.  If you need also the saturation and
     * the value please use the apply methods, which are much faster!
     */
    bool extractHue(const matrix<image::value_type>& img, 
                    matrix<channel8::value_type>& hue) const;

    /**
     * return the saturation of the image.  If you need also the hue and
     * the value please use the apply methods, which are much faster!
     */
    bool extractSaturation(const matrix<image::value_type>& img, 
                           matrix<channel::value_type>& saturation) const;

    /**
     * return the saturation of the image.  If you need also the hue and
     * the saturation please use the apply methods, which are much faster!
     */
    bool extractSaturation(const matrix<image::value_type>& img, 
                           matrix<channel8::value_type>& saturation) const;

    /**
     * return the value of the image.  If you need also the hue and
     * the value please use the apply methods, which are much faster!
     */
    bool extractValue(const matrix<image::value_type>& img, 
                          matrix<channel::value_type>& value) const;

    /**
     * return the value of the image.  If you need also the hue and
     * the value please use the apply methods, which are much faster!
     */
    bool extractValue(const matrix<image::value_type>& img,
                          matrix<channel8::value_type>& value) const;


    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c1 the extracted channel
     */
    virtual bool extractFirst(const matrix<image::value_type>& img, 
                              matrix<channel::value_type>& c1) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c1 the extracted channel
     */
    virtual bool extractFirst(const matrix<image::value_type>& img, 
                              matrix<channel8::value_type>& c1) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c2 the extracted channel
     */
    virtual bool extractSecond(const matrix<image::value_type>& img, 
                               matrix<channel::value_type>& c2) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c2 the extracted channel
     */
    virtual bool extractSecond(const matrix<image::value_type>& img, 
                               matrix<channel8::value_type>& c2) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c3 the extracted channel
     */
    virtual bool extractThird(const matrix<image::value_type>& img,
                              matrix<channel::value_type>& c3) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c3 the extracted channel
     */
    virtual bool extractThird(const matrix<image::value_type>& img,
                              matrix<channel8::value_type>& c3) const;
  };

} // namespace lti
#endif

