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
 * \file   ltiSplitImageToYIQ.cpp
 *         Split color image to YIQ
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToYIQ.cpp,v 1.3 2009-08-30 04:30:29 alvarado Exp $
 */


#include "ltiSplitImageToYIQ.h"
#include "ltiConstants.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(YIQ,splitImage,splitImageToYIQ)

  // ========= splitImageToYIQ ===============

  // split image to YIQ channels
  // returns the name of this type
  const std::string& splitImageToYIQ::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // returns a pointer to a clone of the functor
  splitImageToYIQ* splitImageToYIQ::clone() const {
    return (new splitImageToYIQ(*this));
  }

  // returns a pointer to a clone of the functor
  splitImageToYIQ* splitImageToYIQ::newInstance() const {
    return (new splitImageToYIQ);
  }

  // split image into float channels
  bool splitImageToYIQ::apply(const matrix<rgbaPixel>& img,
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
        const rgbaPixel& pix = img.at(p);

        const float red   = pix.getRed();
        const float green = pix.getGreen();
        const float blue  = pix.getBlue();

        // see Gonzales & Woods for explanation of magic numbers
        // channels I (inphase) and Q (quadrature) can be negative!
        // The Y, I and Q channel are here  - unlike to the other colour space
        // transforms - NOT normalized in order not to loose significant
        // information.
        // When normalized channels are required, this has to be done by
        // using a separate functor...

        c1.at(p) = ( red   * 0.299f +
                     green * 0.587f +
                     blue  * 0.114f )/255.0f;   // Y
        c2.at(p) = ( red   * 0.500f -
                     green * 0.231f -
                     blue  * 0.269f )/255.0f;   // I
        c3.at(p) = ( red   * 0.203f -
                     green * 0.500f +
                     blue  * 0.297f )/255.0f;   // Q

        // Y range: 0..1
        // I range: -0.5..0.5
        // Q range: -0.5..0.5

      } // loop
    return true;
  }


  // split image into 8-bit channels
  bool splitImageToYIQ::apply(const matrix<rgbaPixel>& img,
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
        const float red   = pix.getRed();
        const float green = pix.getGreen();
        const float blue  = pix.getBlue();

        // see Gonzales & Woods for explanation of magic numbers
        // channels I (inphase) and Q (quadrature) can be negative!
        // The Y, I and Q channel are here  - unlike to the other colour space
        // transforms - NOT normalized in order not to loose significant
        // information.
        // When normalized channels are required, this has to be done by
        // using a separate functor...

        c1.at(p) = static_cast<ubyte>(red   * 0.299f +
                                      green * 0.587f +
                                      blue  * 0.114f);   // Y
        c2.at(p) = static_cast<ubyte>(red   * 0.500f -
                                      green * 0.231f -
                                      blue  * 0.269f);   // I
        c3.at(p) = static_cast<ubyte>(red   * 0.203f -
                                      green * 0.500f +
                                      blue  * 0.297f);   // Q

      } // loop
    return true;
  }


  bool splitImageToYIQ::apply(const rgbaPixel& pixel,
                              float& Y,
                              float& I,
                              float& Q) const {

    const float red=pixel.getRed();
    const float green=pixel.getGreen();
    const float blue=pixel.getBlue();

    // see Gonzales & Woods for explanation of magic numbers
    // channels I (inphase) and Q (quadrature) can be negative!
    // The Y, I and Q channel are here  - unlike to the other colour space
    // transforms - NOT normalized in order not to loose significant
    // information.
    // When normalized channels are required, this has to be done by
    // using a separate functor...

    Y  = (red   * 0.299f +
          green * 0.587f +
          blue  * 0.114f)/255.0f;     // Y
    I =  (red   * 0.500f -
          green * 0.231f -
          blue  * 0.269f)/255.0f;   // I
    Q =  (red   * 0.203f -
          green * 0.500f +
          blue  * 0.297f)/255.0f;   // Q

    // Y range: 0..1
    // I range: -0.5..0.5
    // Q range: -0.5..0.5

    return true;
  }

  bool splitImageToYIQ::apply(const rgbaPixel& pixel,
                              ubyte& Y,
                              ubyte& I,
                              ubyte& Q) const {

    const float red  =pixel.getRed();
    const float green=pixel.getGreen();
    const float blue =pixel.getBlue();

    // see Gonzales & Woods for explanation of magic numbers
    // channels I (inphase) and Q (quadrature) can be negative!
    // The Y, I and Q channel are here  - unlike to the other colour space
    // transforms - NOT normalized in order not to loose significant
    // information.
    // When normalized channels are required, this has to be done by
    // using a separate functor...

    Y = static_cast<ubyte>(red  *0.299f +
                           green*0.587f +
                           blue *0.114f);   // Y
    I = static_cast<ubyte>(red  *0.500f -
                           green*0.231f -
                           blue *0.269f);   // I
    Q = static_cast<ubyte>(red  *0.203f -
                           green*0.500f +
                           blue *0.297f);   // Q
    return true;
  }

} // end of namespace lti
