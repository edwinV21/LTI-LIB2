/*
 * Copyright (C) 1998 - 2006
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
 * \file   ltiSplitImageToHSI.h
 *         Contains the functor splitImageToHSI which splits images into hue
 *         saturation and intensity.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToHSI.h,v 1.4 2006-01-06 05:02:10 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_HSI_H_
#define _LTI_SPLIT_IMAGE_TO_HSI_H_

#include "ltiSplitImage.h"
#include "ltiHueLUT.h"

namespace lti {

  /**
   * Split image into its Hue - Saturation - Intensity channels.
   *
   * The HSI color space is obtained by a rotation of the RGB color cube in a
   * way that the gray-value axis is oriented with the intensity (I) axis, the
   * hue (H) is the angle and the saturation (S) the distance from the I
   * axis. 
   *
   * The transformation as defined by 
   *
   * Gonzalez and Woods.  "Digital Image Processing". Addison Wesley. 1992
   *
   * is obtained with
   * \f[
   * \begin{aligned}
   *   I &= \frac{R+G+B}{3} \\
   *   S &= 1- \frac{\min(R,G,B)}{I} \\
   *   H &= \arccos\left(\frac{\frac{1}{2}\left[(R-G) + (R-B)\right]}
   *                          {\sqrt{(R-G)^2+(R-B)(G-B))}}\right)
   * \end{aligned}
   * \f]
   *
   * For the backtransformation see lti::mergeHSIToImage
   *
   * @ingroup gColor
   */
  class splitImageToHSI : public splitImage {
  public:
    /**
     * Constructor
     */
    splitImageToHSI();

    /**
     * Copy Constructor
     */
    splitImageToHSI(const splitImageToHSI& other);

    /**
     * Destructor
     */
    virtual ~splitImageToHSI();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToHSI& copy(const splitImageToHSI& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToHSI& operator=(const splitImageToHSI& other);

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual splitImageToHSI* clone() const;

    /**
     * returns a pointer to a new instance of the functor.
     */
    virtual splitImageToHSI* newInstance() const;

    /**
     * split the image in hue channel H, saturation S and intensity
     * channel I.
     * The values of each image will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param I the intensity channel
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel::value_type>& H,
                       matrix<channel::value_type>& S,
                       matrix<channel::value_type>& I) const;

    /**
     * split the image in hue channel H, saturation S and intensity
     * channel I.
     * The values of each image will be between 0 and 255
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param I the intensity channel
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel8::value_type>& H,
                       matrix<channel8::value_type>& S,
                       matrix<channel8::value_type>& I) const;

    /**
     * split the pixel in hue value H, saturation S and intensity
     * value I.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param I the intensity value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       float& H,
                       float& S,
                       float& I) const;

    /**
     * split the pixel in hue value H, saturation S and intensity
     * value I.
     * The values of each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param I the intensity value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& H,
                       ubyte& S,
                       ubyte& I) const;

    /**
     * return the hue of the image.  If you need also the saturation and
     * the intensity please use the apply methods, which are much faster!
     */
    bool extractHue(const matrix<image::value_type>& img, 
                    matrix<channel::value_type>& hue) const;

    /**
     * return the hue of the image.  If you need also the saturation and
     * the intensity please use the apply methods, which are much faster!
     */
    bool extractHue(const matrix<image::value_type>& img, 
                    matrix<channel8::value_type>& hue) const;

    /**
     * return the saturation of the image.  If you need also the hue and
     * the intensity please use the apply methods, which are much faster!
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
     * return the intensity of the image.  If you need also the hue and
     * the intensity please use the apply methods, which are much faster!
     */
    bool extractIntensity(const matrix<image::value_type>& img, 
                          matrix<channel::value_type>& intensity) const;

    /**
     * return the intensity of the image.  If you need also the hue and
     * the intensity please use the apply methods, which are much faster!
     */
    bool extractIntensity(const matrix<image::value_type>& img,
                          matrix<channel8::value_type>& intensity) const;


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

  private:

    hueLUT hueLUT_;
  };

} // namespace lti
#endif

