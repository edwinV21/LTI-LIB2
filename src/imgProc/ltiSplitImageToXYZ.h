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
 * \file   ltiSplitImageToXYZ.h
 *         Split color image to XYZ
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToXYZ.h,v 1.1 2008-04-27 02:41:22 alvarado Exp $
 */


#ifndef _LTI_SPLIT_IMAGE_TO_XYZ_H_
#define _LTI_SPLIT_IMAGE_TO_XYZ_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its XYZ norm channels
   *
   * The following is an excerpt of
   * <a href="http://www.cs.rit.edu/~ncs/color/t_convert.html">this page</a>
   *
   * RGB values in a particular set of primaries can be transformed to
   * and from CIE XYZ via a 3x3 matrix transform. These transforms
   * involve tristimulus values, that is a set of three linear-light
   * components that conform to the CIE color-matching functions. CIE
   * XYZ is a special set of tristimulus values. In XYZ, any color is
   * represented as a set of positive values.
   *
   * To transform from XYZ to RGB (with D65 white point), the matrix
   * transform used is [3]:
   *
   *   [ R ]   [  3.240479 -1.537150 -0.498535 ]   [ X ]<br>
   *   [ G ] = [ -0.969256  1.875992  0.041556 ] * [ Y ]<br>
   *   [ B ]   [  0.055648 -0.204043  1.057311 ]   [ Z ]<br>
   *
   * The range for valid R, G, B values is [0,1]. Note, this matrix
   * has negative coefficients. Some XYZ color may be transformed to
   * RGB values that are negative or greater than one. This means that
   * not all visible colors can be produced using the RGB system.
   *
   * The inverse transformation matrix is as follows:
   *
   *   [ X ]   [ 0.412453  0.357580  0.180423 ]   [ R ]<br>
   *   [ Y ] = [ 0.212671  0.715160  0.072169 ] * [ G ]<br>
   *   [ Z ]   [ 0.019334  0.119193  0.950227 ]   [ B ]<br>
   *
   *
   * @ingroup gColor
   */
  class splitImageToXYZ : public splitImage {
  public:

    /**
     * returns the name of this type
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of the functor
     */
    virtual splitImageToXYZ* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual splitImageToXYZ* newInstance() const;

    /**
     * split the image in XYZ.
     * XYZ splitting produces unnormalized channels !
     * @param img the image to be splitted
     * @param X the perceived luminance
     * @param Y color information and some luminance
     * @param Z color information and some luminance
     */
    virtual bool apply(const matrix<rgbaPixel>& img,
                       matrix<float>& X,
                       matrix<float>& Y,
                       matrix<float>& Z) const;

    /**
     * split the image in XYZ.
     * XYZ splitting produces unnormalized channels !
     * @param img the image to be splitted
     * @param X the perceived luminance
     * @param Y color information and some luminance
     * @param Z color information and some luminance
     */
    virtual bool apply(const matrix<rgbaPixel>& img,
                       matrix<ubyte>& X,
                       matrix<ubyte>& Y,
                       matrix<ubyte>& Z) const;

    /**
     * split the pixel in XYZ.
     * XYZ splitting produces unnormalized values !
     * @param pixel the pixel to be splitted
     * @param X the perceived luminance
     * @param Y color information and some luminance
     * @param Z color information and some luminance
     */
    virtual bool apply(const rgbaPixel& pixel,
                       float& X,
                       float& Y,
                       float& Z) const;

    /**
     * split the pixel in XYZ.
     * XYZ splitting produces unnormalized values !
     * @param pixel the pixel to be splitted
     * @param X the perceived luminance
     * @param Y color information and some luminance
     * @param Z color information and some luminance
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& X,
                       ubyte& Y,
                       ubyte& Z) const;
  };


} // namespace lti
#endif
