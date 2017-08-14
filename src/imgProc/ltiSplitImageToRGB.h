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
 * \file   ltiSplitImageToRGB.h
 *         Definition of lti::splitImageToRGB which splits an image into three
 *         different color channels red, green, and blue (RGB).
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToRGB.h,v 1.5 2005-04-28 15:34:00 doerfler Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_RGB_H_
#define _LTI_SPLIT_IMAGE_RGB_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image into its Red - Green - Blue channels.
   * You can split all the channels at the same time (with apply) or
   * get just one channel using the shortcut functions getRed(), getGreen()
   * or getBlue().
   *
   * @see lti::mergeRGBToImage
   *
   * @ingroup gColor
   */
  class splitImageToRGB : public splitImage {
  public:

    /**
     * default constructor
     */
    splitImageToRGB();

    /**
     * copy constructor
     */
    splitImageToRGB(const splitImageToRGB& other);

    /**
     * destructor
     */
    virtual ~splitImageToRGB();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToRGB& copy(const splitImageToRGB& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToRGB& operator=(const splitImageToRGB& other);

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual splitImageToRGB* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual splitImageToRGB* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Split the image in red green and blue channels.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel::value_type>& red,
                       matrix<channel::value_type>& green,
                       matrix<channel::value_type>& blue) const;

    /**
     * split the image in red green and blue channels.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha or dummy channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel::value_type>& red,
                       matrix<channel::value_type>& green,
                       matrix<channel::value_type>& blue,
                       matrix<channel::value_type>& alpha) const;

    /**
     * split the image in red green and blue channels.  The values of
     * each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel8::value_type>& red,
                       matrix<channel8::value_type>& green,
                       matrix<channel8::value_type>& blue) const;

    /**
     * split the image in red green and blue channels.  The values of
     * each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha or dummy channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel8::value_type>& red,
                       matrix<channel8::value_type>& green,
                       matrix<channel8::value_type>& blue,
                       matrix<channel8::value_type>& alpha) const;

    /**
     * split the pixel in red green and blue values.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const rgbaPixel& pixel,
                       float& red,
                       float& green,
                       float& blue) const;

    /**
     * split the pixel in red green and blue values.  The values of
     * each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& red,
                       ubyte& green,
                       ubyte& blue) const;

    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param red the red channel
     */
    bool extractRed(const matrix<image::value_type>& img,
                    matrix<channel::value_type>& red) const;


    /**
     * shortcut to extract the red channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param red the red channel
     */
    bool extractRed(const matrix<image::value_type>& img,
                    matrix<channel8::value_type>& red) const;

    /**
     * shortcut to extract the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param green the green channel
     */
    bool extractGreen(const matrix<image::value_type>& img,
                      matrix<channel::value_type>& green) const;


    /**
     * shortcut to extract the green channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param green the green channel
     */
    bool extractGreen(const matrix<image::value_type>& img,
                      matrix<channel8::value_type>& green) const;

    /**
     * shortcut to extract the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param blue the blue channel
     */
    bool extractBlue(const matrix<image::value_type>& img,
                     matrix<channel::value_type>& blue) const;


    /**
     * shortcut to extract the red channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param blue the blue channel
     */
    bool extractBlue(const matrix<image::value_type>& img,
                     matrix<channel8::value_type>& blue) const;

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

