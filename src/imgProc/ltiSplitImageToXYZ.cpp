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
 * \file   ltiSplitImageToXYZ.cpp
 *         Split color image to XYZ
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToXYZ.cpp,v 1.2 2009-08-30 04:30:29 alvarado Exp $
 */


#include "ltiSplitImageToXYZ.h"
#include "ltiConstants.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(XYZ,splitImage,splitImageToXYZ)

  // ========= splitImageToXYZ ===============

  // split image to XYZ channels
  // returns the name of this type
  const std::string& splitImageToXYZ::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // returns a pointer to a clone of the functor
  splitImageToXYZ* splitImageToXYZ::clone() const {
    return (new splitImageToXYZ(*this));
  }

  // returns a pointer to a clone of the functor
  splitImageToXYZ* splitImageToXYZ::newInstance() const {
    return (new splitImageToXYZ);
  }

  // split image into float channels
  bool splitImageToXYZ::apply(const matrix<rgbaPixel>& img,
                              matrix<float>& c1,
                              matrix<float>& c2,
                              matrix<float>& c3) const {

    ipoint p;      // coordinates

    // make the channels size of source image...
    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {
        // take pixel at position p

        // single Pixel Element in RGB-values...
        const rgbaPixel& pix = img.at(p);

        // see Gonzales & Woods for explanation of magic numbers
        // cast just once
        const float red(pix.getRed());
        const float green(pix.getGreen());
        const float blue(pix.getBlue());

        c1.at(p) = (red  * 0.412453f +
                    green* 0.357580f +
                    blue * 0.180423f)/255.0f;   // X
        c2.at(p) = (red  * 0.212671f +
                    green* 0.715160f +
                    blue * 0.072169f)/255.0f;   // Y
        c3.at(p) = (red  *0.019334f +
                    green*0.119193f +
                    blue *0.950227f)/255.0f;   // Z
      } // loop
    return true;
  }


  // split image into 8-bit channels
  bool splitImageToXYZ::apply(const matrix<rgbaPixel>& img,
                              matrix<ubyte>& c1,
                              matrix<ubyte>& c2,
                              matrix<ubyte>& c3) const {
    ipoint p;      // coordinates

    // make the channels size of source image...
    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {

        // take pixel at position p
        const rgbaPixel& pix = img.at(p);

        const float red = pix.getRed();
        const float green = pix.getGreen();
        const float blue = pix.getBlue();

        // see Gonzales & Woods for explanation of magic numbers
        c1.at(p) = static_cast<ubyte>(red   *0.412453f +
                                      green *0.357580f +
                                      blue  *0.180423f);  //X
        c2.at(p) = static_cast<ubyte>(red   *0.212671f +
                                      green *0.715160f +
                                      blue  *0.072169f);  //Y
        c3.at(p) = static_cast<ubyte>(red   *0.019334f +
                                      green *0.119193f +
                                      blue  *0.950227f);  //Z
      } // loop
    return true;
  }


  bool splitImageToXYZ::apply(const rgbaPixel& pixel,
                              float& c1,
                              float& c2,
                              float& c3) const {

    const float red=pixel.getRed();
    const float green=pixel.getGreen();
    const float blue=pixel.getBlue();

    c1 = (red *0.412453f +
          green *0.357580f +
          blue *0.180423f)/255.0f;   // X
    c2 = (red *0.212671f +
          green *0.715160f +
          blue *0.072169f)/255.0f;   // Y
    c3 = (red *0.019334f +
          green *0.119193f +
          blue *0.950227f)/255.0f;   // Z
    return true;
  }

  bool splitImageToXYZ::apply(const rgbaPixel& pixel,
                              ubyte& c1,
                              ubyte& c2,
                              ubyte& c3) const {

    const float red=pixel.getRed();
    const float green=pixel.getGreen();
    const float blue=pixel.getBlue();

    // see Gonzales & Woods for explanation of magic numbers
    c1 = static_cast<ubyte>(red   *0.412453f +
                            green *0.357580f +
                            blue  *0.180423f);  //X
    c2 = static_cast<ubyte>(red   *0.212671f +
                            green *0.715160f +
                            blue  *0.072169f);  //Y
    c3 = static_cast<ubyte>(red   *0.019334f +
                            green *0.119193f +
                            blue  *0.950227f);  //Z
    return true;
  }

} // end of namespace lti
