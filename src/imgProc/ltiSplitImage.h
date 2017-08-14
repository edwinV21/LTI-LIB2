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
 * \file   ltiSplitImage.h
 *         Definition of lti::splitImage which is the base class of
 *         all functors that split an image into three different color
 *         channels of some color space.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImage.h,v 1.7 2006-06-07 13:02:42 doerfler Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_H_
#define _LTI_SPLIT_IMAGE_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

namespace lti {

  /**
   * Abstract parent class for all classes that split an image into
   * different color spaces components (color channels).
   *
   * @ingroup gColor
   */
  class splitImage : public functor {
  public:
    /**
     * default constructor
     */
    splitImage();

    /**
     * copy constructor
     */
    splitImage(const splitImage& other);

    /**
     * destructor
     */
    virtual ~splitImage();

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual splitImage* clone() const = 0;

    /**
     * returns a pointer to a new instance of the functor.
     */
    virtual splitImage* newInstance() const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImage& copy(const splitImage& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImage& operator=(const splitImage& other);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Splits the image \c img into three color channels \c c1, \c c2,
     * and \c c3
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel::value_type>& c1,
                       matrix<channel::value_type>& c2,
                       matrix<channel::value_type>& c3) const = 0;
    /**
     * Splits the image \c img into three color channel8s \c c1, \c c2,
     * and \c c3
     */
    virtual bool apply(const matrix<image::value_type>& img,
                       matrix<channel8::value_type>& c1,
                       matrix<channel8::value_type>& c2,
                       matrix<channel8::value_type>& c3) const = 0;

    /**
     * Splits the rgbPixel \c pixel into three color values \c c1, \c c2,
     * and \c c3
     */
    virtual bool apply(const rgbaPixel& pixel,
                       float& c1,
                       float& c2,
                       float& c3) const = 0;
    
    /**
     * Splits the rgbPixel \c pixel into three color values \c c1, \c c2,
     * and \c c3
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& c1,
                       ubyte& c2,
                       ubyte& c3) const = 0;

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
 
  protected:

    /**
     * return the minimum of three values
     */
    template <typename T>
    inline T maximum(const T a, const T b, const T c) const;

    /**
     * return the maximum of three value
     */
    template <typename T>
    inline T minimum(const T a, const T b, const T c) const;

  };

} // namespace lti

#include "ltiSplitImage_inline.h"

#endif

