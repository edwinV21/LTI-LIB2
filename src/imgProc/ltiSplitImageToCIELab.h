/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2013
 * CanControls
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
 * \file   ltiSplitImageToCIELab.h
 *         
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToCIELab.h,v 1.1 2013-11-20 17:05:23 alvarado Exp $
 */


#ifndef _LTI_SPLIT_IMAGE_TO_CIELAB_H_
#define _LTI_SPLIT_IMAGE_TO_CIELAB_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its L\f$^*\f$a\f$^*\f$b\f$^*\f$ channels.
   *
   * The following is an excerpt of
   * <a href="http://www.cs.rit.edu/~ncs/color/t_convert.html">this page</a>
   *
   * CIE 1976 \f$L^*a^*b^*\f$ (CIELUV) is
   * based directly on CIE XYZ and is another attempt to linearize the
   * perceptibility of color differences.  The non-linear relations for
   * \f$L^*, a^*,\f$ and \f$b^*\f$ are intended to mimic the logarithmic 
   * response of the eye. Coloring information is referred to the color of
   * the white point of the system, subscript \f$n\f$:
   *
   * \f$L^*\f$, \f$a^*\f$, and \f$b^*\f$ are given below:
   *
   * - \f$L^* =  116 * f(Y/Y_n) - 16\f$
   * - \f$a^* =  500 \times (f(X/X_n)-f(Y/Y_n))\f$
   * - \f$b^* =  200 \times (f(Y/Y_n)-f(Z/Z_n))\f$
   *
   * where
   *
   * - \f$f(t) = t^{1/3}\f$ for \f$t > (6/29)^3\f$
   * - \f$f(t) = \frac{1}{3}(6/29)^2 t + 4/29 \f$ otherwise
   *
   * The values \f$X_n, Y_n\f$ and \f$Z_n\f$ are the tristimulus values
   * of the reference white.  Here, the D65 white point is used.
   *
   * The \f$L^*\f$ value with these definitions will be between 0 and 100.
   * This functor ensure that the outputs keep those values, even though
   * they are not typical for the LTI-Lib.  The reason is that 
   * this color space has been designed with a perceptual metric in mind
   * which would not be kept if the channels are scaled.
   *
   * For channel8 outputs this color space doesn't make much sense,
   * since some values are negative.  All L a and b will be scaled to
   * fit the 0 to 255 range (not really Lab), particularly a and b
   * will have a value of 128 added to them (this destroys the metric
   * completely!)
   *
   *
   * @ingroup gColor
   */
  class splitImageToCIELab : public splitImage {
  public:
    /**
     * default constructor.
     * Initializes (only once) the lut for the cubic root, with 2048 entries.
     */
    splitImageToCIELab();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    splitImageToCIELab(const splitImageToCIELab& other);

    /**
     * destructor
     */
    virtual ~splitImageToCIELab();

    /**
     * returns the name of this type ("splitImageToCIELab")
     */
    const std::string& name() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToCIELab& copy(const splitImageToCIELab& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToCIELab& operator=(const splitImageToCIELab& other);

    /**
     * returns a pointer to a clone of the functor
     */
    virtual splitImageToCIELab* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual splitImageToCIELab* newInstance() const;

    /**
     * Split the RGB image into the Lab components
     *
     * Note that the output channels will contain values outside the interval
     * 0.0-1.0.
     * 
     */
    virtual bool apply(const matrix<rgbaPixel>& img,
                       matrix<float>& L,
                       matrix<float>& a,
                       matrix<float>& b) const;

    /**
     * Split the RGB value into the Lab components
     */
    virtual bool apply(const rgbaPixel& pixel,
		       float& c1,
                       float& c2,
                       float& c3) const;

  protected:

    /**
     * on-copy
     * Do not use this method yet!  The output channels will contain invalid
     * data due to underflows and overflows of the channel valid value range!
     */
    virtual bool apply(const matrix<rgbaPixel>& img,
                       matrix<ubyte>& c1,
                       matrix<ubyte>& c2,
                       matrix<ubyte>& c3) const;

    /**
     * on-copy
     * Do not use this method yet!  The output values will contain invalid
     * data due to underflows and overflows of the valid value range!
     */
    virtual bool apply(const rgbaPixel& pixel,
                       ubyte& c1,
                       ubyte& c2,
                       ubyte& c3) const;

    /**
     * look-up-table for cubic root
     */
    static const float* lut_;

    /**
     * size of the lut for cubic root
     */
    static const int lutSize_;

    /**
     * Matrix coefficients from RGB to normalized XYZ
     */
    static const float m00_,m01_,m02_,m10_,m11_,m12_,m20_,m21_,m22_;
  };

} // namespace lti
#endif
