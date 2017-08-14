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
 * \file   ltiSplitImageToCIELab.cpp
 *         
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToCIELab.cpp,v 1.2 2013-11-20 17:22:16 alvarado Exp $
 */


#include "ltiSplitImageToCIELab.h"
#include "ltiFactory.h"


#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  _LTI_REGISTER_IN_FACTORY_AS(CIELab,splitImage,splitImageToCIELab)

  // ========= splitImageToCIELab ===============

  // -------------------------------------------------
  // the CIE L* a* b* color space
  // -------------------------------------------------
  // static member initialization
  const float* splitImageToCIELab::lut_ = 0;

  // some very private absolute constants
  static const int Lan=16;
  static const float Xn=0.95046f;
  static const float Yn=1.0f;
  static const float Zn=1.08875f;

  const int splitImageToCIELab::lutSize_ = 255*Lan; // 4080
  const float splitImageToCIELab::m00_ = 0.412453f*Lan/Xn;
  const float splitImageToCIELab::m01_ = 0.357580f*Lan/Xn;
  const float splitImageToCIELab::m02_ = 0.180423f*Lan/Xn;
  const float splitImageToCIELab::m10_ = 0.212671f*Lan/Yn;
  const float splitImageToCIELab::m11_ = 0.715160f*Lan/Yn;
  const float splitImageToCIELab::m12_ = 0.072169f*Lan/Yn;
  const float splitImageToCIELab::m20_ = 0.019334f*Lan/Zn;
  const float splitImageToCIELab::m21_ = 0.119193f*Lan/Zn;
  const float splitImageToCIELab::m22_ = 0.950227f*Lan/Zn;


  // default constructor
  splitImageToCIELab::splitImageToCIELab() {
    if (isNull(lut_)) {
      _lti_debug("building LUT, size is " << lutSize_ << "\n");

      // the singleton approach: just one static member, this way the
      // compiler will take care of deleting the static object at the end
      // of the application:
      static float lutData[lutSize_+Lan];  // some additional items for safety

      for (int i=0; i<lutSize_+Lan; ++i) {
        float y = static_cast<float>(i)/lutSize_;
        if (y<=0.0088565f) {
          lutData[i] = 7.7870370f*y+0.1379310f;
        } else {
          lutData[i] = std::pow(y,1.0f/3.0f);
        }
      }

      lut_ = lutData; // provide access from the whole class to the static
                      // member.
    }
  }

  // copy constructor
  splitImageToCIELab::splitImageToCIELab(const splitImageToCIELab& other) 
  : splitImage() {
    copy(other);
  }

  // destructor
  splitImageToCIELab::~splitImageToCIELab() {
  }

  // returns the name of this class
  const std::string& splitImageToCIELab::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // alias for copy member
  splitImageToCIELab&
  splitImageToCIELab::operator=(const splitImageToCIELab& other) {
    return (copy(other));
  }

  // copy member
  splitImageToCIELab& 
  splitImageToCIELab::copy(const splitImageToCIELab& other) {
    splitImage::copy(other);

    return (*this);
  }

  // clone member
  splitImageToCIELab* splitImageToCIELab::clone() const {
    return (new splitImageToCIELab(*this));
  }

  // new instance
  splitImageToCIELab* splitImageToCIELab::newInstance() const {
    return (new splitImageToCIELab);
  }

  // split image into float channels
  bool splitImageToCIELab::apply(const matrix<rgbaPixel>& img,
                                 matrix<float>& c1,
                                 matrix<float>& c2,
                                 matrix<float>& c3) const {

    // make the channels size of source image...
    c1.allocate(img.size()); // L
    c2.allocate(img.size()); // a
    c3.allocate(img.size()); // b

    const int rows = img.rows();
    const int cols = img.columns();

    for (int y=0;y<rows;++y) {

      const vector<rgbaPixel>& ri = img.getRow(y);
      fvector& r1 = c1.getRow(y);
      fvector& r2 = c2.getRow(y);
      fvector& r3 = c3.getRow(y);

      for (int x=0;x<cols;++x) {

        // take pixel at position p
        const rgbaPixel& pix = ri.at(x); // single Pixel (RGB-values)

	// ensure casting just once (well, the compiler could do that too, but
	//                           anyway this is shorter. . .)
	const float r=pix.getRed();
	const float g=pix.getGreen();
	const float b=pix.getBlue();
	
	const float fx = lut_[static_cast<int>(m00_*r+m01_*g+m02_*b+0.5f)];
	const float fy = lut_[static_cast<int>(m10_*r+m11_*g+m12_*b+0.5f)];
	const float fz = lut_[static_cast<int>(m20_*r+m21_*g+m22_*b+0.5f)];

        // Y has now values between 0 and 100.
        r1.at(x) = 116.0f*fy-16.0f; // 
	r2.at(x) = 500.0f*(fx-fy);
	r3.at(x) = 200.0f*(fy-fz);
      } // end for x
    } // end for y
    return true;
  }

  // split Image to 8-bit channels
  // channels RG and BY arwe shifted up to obtain positive values !
  // for more accuracy use float channels
  bool splitImageToCIELab::apply(const matrix<rgbaPixel>& img,
                                 matrix<ubyte>& c1,
                                 matrix<ubyte>& c2,
                                 matrix<ubyte>& c3) const {

    // make the channels size of source image...
    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    const int rows = img.rows();
    const int cols = img.columns();

    for (int y=0;y<rows;++y) {

      const vector<rgbaPixel>& ri = img.getRow(y);
      vector<ubyte>& r1 = c1.getRow(y);
      vector<ubyte>& r2 = c2.getRow(y);
      vector<ubyte>& r3 = c3.getRow(y);

      for (int x=0;x<cols;++x) {

        // take pixel at position p
        const rgbaPixel& pix = ri.at(x); // single Pixel (RGB-values)

	// ensure casting just once (well, the compiler could do that too, but
	//                           anyway this is shorter. . .)
	const float r=pix.getRed();
	const float g=pix.getGreen();
	const float b=pix.getBlue();
	
	const float fx = lut_[static_cast<int>(m00_*r+m01_*g+m02_*b+0.5f)];
	const float fy = lut_[static_cast<int>(m10_*r+m11_*g+m12_*b+0.5f)];
	const float fz = lut_[static_cast<int>(m20_*r+m21_*g+m22_*b+0.5f)];

        r1.at(x) = static_cast<int>(295.8f*fy-40.8f+0.5f);  // map from 0 to 255
	r2.at(x) = static_cast<ubyte>(147.32f*(fx-fy) + 128.5f);
	r3.at(x) = static_cast<ubyte>(147.32f*(fy-fz) + 128.5f);
      } // end for x
    } // end for y

    return true;
  }

  bool splitImageToCIELab::apply(const rgbaPixel& pixel,
                                 float& c1,
                                 float& c2,
                                 float& c3) const {


    // ensure casting just once (well, the compiler could do that too, but
    //                           anyway this is shorter. . .)
    const float r=pixel.getRed();
    const float g=pixel.getGreen();
    const float b=pixel.getBlue();
    
    const float fx = lut_[static_cast<int>(m00_*r+m01_*g+m02_*b+0.5)];
    const float fy = lut_[static_cast<int>(m10_*r+m11_*g+m12_*b+0.5)];
    const float fz = lut_[static_cast<int>(m20_*r+m21_*g+m22_*b+0.5)];
    
    // Y has now values between 0 and 100.
    c1 = 116.0f*fy-16.0f; // L
    c2 = 500.0f*(fx-fy);  // a
    c3 = 200.0f*(fy-fz);  // b
    
    return true;
  }

  bool splitImageToCIELab::apply(const rgbaPixel& pixel,
                                 ubyte& c1,
                                 ubyte& c2,
                                 ubyte& c3) const {

    // ensure casting just once (well, the compiler could do that too, but
    //                           anyway this is shorter. . .)
    const float r=pixel.getRed();
    const float g=pixel.getGreen();
    const float b=pixel.getBlue();
    
    const float fx = lut_[static_cast<int>(m00_*r+m01_*g+m02_*b+0.5)];
    const float fy = lut_[static_cast<int>(m10_*r+m11_*g+m12_*b+0.5)];
    const float fz = lut_[static_cast<int>(m20_*r+m21_*g+m22_*b+0.5)];
    
    c1 = static_cast<ubyte>(295.8f*fy-40.8f+0.5f);  // map from 0 to 255
    c2 = static_cast<ubyte>(147.32f*(fx-fy) + 128.5f);
    c3 = static_cast<ubyte>(147.32f*(fy-fz) + 128.5f);

    return true;
  }

} // end of namespace lti
