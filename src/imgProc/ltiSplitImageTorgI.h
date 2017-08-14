/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Electronics Engineering School, ITCR, Costa Rica
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
 * \file   ltiSplitImageTorgI.h
 *         Contains the functor splitImageTorgI which splits images into 
 *         chromaticity and intensity channels.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999 (LTI-Lib 1)
 * \date   11.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiSplitImageTorgI.h,v 1.1 2006-01-11 20:07:01 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_RGI_H_
#define _LTI_SPLIT_IMAGE_TO_RGI_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its chromaticity channels.
   *
   * The chromaticity channels are defined as follows:
   * - intensity channel \f$ I = \frac{R+G+B}{3} \f$
   * - chromacity red    \f$ r = \frac{R}{R+G+B} \f$
   * - chromacity green  \f$ g = \frac{G}{R+G+B} \f$
   *
   * You can get all channels at the same time using apply() or just
   * get one using the shortcut methods getR(), getG() or getIntensity().
   *
   * @ingroup gColor
   */
  class splitImageTorgI : public splitImage {
  public:
    /**
     * Constructor
     */
    splitImageTorgI();

    /**
     * Copy Constructor
     */
    splitImageTorgI(const splitImageTorgI& other);

    /**
     * Destructor
     */
    virtual ~splitImageTorgI();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageTorgI& copy(const splitImageTorgI& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageTorgI& operator=(const splitImageTorgI& other);

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual splitImageTorgI* clone() const;

    /**
     * Returns a pointer to a new instance of the functor.
     */
    virtual splitImageTorgI* newInstance() const;

    /**
     * Split the image in chromacity channels r and g and intensity channel.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param r the chromacity channel
     * @param g the chromacity channel
     * @param I the intensity channel
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel::value_type>& r,
                       matrix<channel::value_type>& g,
                       matrix<channel::value_type>& I) const;

    /**
     * Split the image in chromacity channels r and g and intensity channel.
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param r the chromacity channel
     * @param g the chromacity channel
     * @param I the intensity channel
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel8::value_type>& r,
                       matrix<channel8::value_type>& g,
                       matrix<channel8::value_type>& I) const;

    /**
     * Split the pixel in chromacity values r and g and intensity value.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       float& r,
                       float& g,
                       float& I) const;

    /**
     * Split the pixel in chromacity values r and g and intensity value.
     * The values of each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& r,
                       ubyte& g,
                       ubyte& I) const;

    /** 
     * Shortcut to get the red channel only.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param r the chromacity red channel
     */
    bool getR(const matrix<image::value_type>& img,
              matrix<channel::value_type>& r) const;


    /**
     * Shortcut to get the red channel only.
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param r the chromacity red channel
     */
    bool getR(const matrix<image::value_type>& img,
              matrix<channel8::value_type>& r) const;

    /**
     * Shortcut to get the red channel only.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param g the chromacity green channel
     */
    bool getG(const matrix<image::value_type>& img,
              matrix<channel::value_type>& g) const;

    /**
     * Shortcut to get the green channel only.
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param g the chromacity green channel
     */
    bool getG(const matrix<image::value_type>& img,
              matrix<channel8::value_type>& g) const;

    /**
     * Shortcut to get the red channel only.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param I the intensity channel
     */
    bool getIntensity(const matrix<image::value_type>& img,
                      matrix<channel::value_type>& I) const;


    /**
     * Shortcut to get the red channel only.
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param I the intensity channel
     */
    bool getIntensity(const matrix<image::value_type>& img,
                      matrix<channel8::value_type>& I) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c1 the extracted channel
     */
    virtual bool getFirst(const matrix<image::value_type>& img, 
                                matrix<channel::value_type>& c1) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c1 the extracted channel
     */
    virtual bool getFirst(const matrix<image::value_type>& img,
                                matrix<channel8::value_type>& c1) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c2 the extracted channel
     */
    virtual bool getSecond(const matrix<image::value_type>& img,
                                 matrix<channel::value_type>& c2) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c2 the extracted channel
     */
    virtual bool getSecond(const matrix<image::value_type>& img, 
                                 matrix<channel8::value_type>& c2) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c3 the extracted channel
     */
    virtual bool getThird(const matrix<image::value_type>& img,
                                matrix<channel::value_type>& c3) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c3 the extracted channel
     */
    virtual bool getThird(const matrix<image::value_type>& img,
                                matrix<channel8::value_type>& c3) const;

  };

} // namespace lti
#endif

