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
 * \file   ltiSplitImageTorgI.cpp
 *         Contains the functor splitImageTorgI which splits images into 
 *         chromaticity and intensity channels.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999 (LTI-Lib 1)
 * \date   11.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiSplitImageTorgI.cpp,v 1.4 2007-04-05 22:56:59 alvarado Exp $
 */

#include "ltiSplitImageTorgI.h"
#include "ltiConstants.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(rgI,splitImage,splitImageTorgI)

  // ========= splitImageTorgI ===============
  // Constructor
  splitImageTorgI::splitImageTorgI() : splitImage() {
  }

  // copy constructor
  splitImageTorgI::splitImageTorgI(const splitImageTorgI& other) 
    : splitImage() {
    copy(other);
  }

  // destructor
  splitImageTorgI::~splitImageTorgI() {
  }

  // name of the class
  const std::string& splitImageTorgI::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // returns a pointer to a clone of the functor.
  splitImageTorgI* splitImageTorgI::clone() const {
    return (new splitImageTorgI(*this));
  }

  // returns a pointer to a new instance of the functor.
  splitImageTorgI* splitImageTorgI::newInstance() const {
    return (new splitImageTorgI());
  }

  // copy member
  splitImageTorgI& 
  splitImageTorgI::copy(const splitImageTorgI& other) {
    splitImage::copy(other);

    return (*this);
  }

  // alias for copy member
  splitImageTorgI&
  splitImageTorgI::operator=(const splitImageTorgI& other) {
    return (copy(other));
  }

  bool splitImageTorgI::apply(const matrix<image::value_type>& img,
                              matrix<channel::value_type>& c1,
                              matrix<channel::value_type>& c2,
                              matrix<channel::value_type>& c3) const {
    float i;
    ipoint p;
    vector<rgbaPixel>::const_iterator imgIt,imgEIt;

    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++) {
      const vector<rgbaPixel>& theRow = img.getRow(p.y);
      for (p.x=0,imgIt=theRow.begin(),imgEIt=theRow.end();
           imgIt!=imgEIt;p.x++,imgIt++) {
        const rgbaPixel& pix = *imgIt;
        
        i = (static_cast<float>(pix.red) + 
             static_cast<float>(pix.green) +
             static_cast<float>(pix.blue));
        if (i>0) { // to avoid division by 0
          c1.at(p) = static_cast<float>(pix.red)/i;
          c2.at(p) = static_cast<float>(pix.green)/i;
          c3.at(p) = i/(765.0f); // 765 = 3*255;
        } else {
          c1.at(p) = 0.0f;
          c2.at(p) = 0.0f;
          c3.at(p) = 0.0f;
        }
      }
    }

    return true;
  }

  bool splitImageTorgI::apply(const matrix<image::value_type>& img,
                              matrix<channel8::value_type>& c1,
                              matrix<channel8::value_type>& c2,
                              matrix<channel8::value_type>& c3) const {
    int i;
    ipoint p;
    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {
        const rgbaPixel& pix = img.at(p);

        i = (static_cast<int>(pix.red) +
             static_cast<int>(pix.green) +
             static_cast<int>(pix.blue));
        if (i!=0) { // to avoid division by 0
          c1.at(p) = static_cast<ubyte>(static_cast<int>(pix.red)*255/i);
          c2.at(p) = static_cast<ubyte>(static_cast<int>(pix.green)*255/i);
          c3.at(p) = static_cast<ubyte>(i/3);
        } else {
          c1.at(p) = 0;
          c2.at(p) = 0;
          c3.at(p) = 0;
        }
      }
    return true;
  }

  bool splitImageTorgI::apply(const rgbaPixel& pix,
                              float& r,
                              float& g,
                              float& I) const {

    const float sum =  (static_cast<float>(pix.red) + 
                        static_cast<float>(pix.green) +
                        static_cast<float>(pix.blue));

    if (sum>0) {
      r=static_cast<float>(pix.red)/sum;
      g=static_cast<float>(pix.green)/sum;
      I=sum/765.0f;
    } else {
      r=0.0f;
      g=0.0f;
      I=0.0f;
    }
    return true;
  }

  bool splitImageTorgI::apply(const rgbaPixel& pix,
                              ubyte& r,
                              ubyte& g,
                              ubyte& I) const {

    int sum = (static_cast<int>(pix.red) +
               static_cast<int>(pix.green) +
               static_cast<int>(pix.blue));

    if (sum!=0) { // to avoid division by 0
      r = static_cast<ubyte>(static_cast<int>(pix.red)*255/sum);
      g = static_cast<ubyte>(static_cast<int>(pix.green)*255/sum);
      I = static_cast<ubyte>(sum/3);
    } else {
      r=0;
      g=0;
      I=0;
    }
    return true;
  }

  bool splitImageTorgI::getR(const matrix<image::value_type>& img,
                             matrix<channel::value_type>& r) const {
    ipoint p;
    float i;
    r.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {
        const rgbaPixel& pix = img.at(p);
        i = (static_cast<float>(pix.red) + 
             static_cast<float>(pix.green) +
             static_cast<float>(pix.blue));
        if (i>0) {
          r.at(p) = static_cast<float>(pix.red)/i;
        } else {
          r.at(p) = 0.0f;
        }
      }
    return true;
  }

  bool splitImageTorgI::getR(const matrix<image::value_type>& img,
                             matrix<channel8::value_type>& r) const {
    ipoint p;
    int i;
    r.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {
        const rgbaPixel& pix = img.at(p);
        i = (static_cast<int>(pix.red) +
             static_cast<int>(pix.green) +
             static_cast<int>(pix.blue));
        if (i!=0) {
          r.at(p) = static_cast<ubyte>(static_cast<int>(pix.red)*255/i);
        } else {
          r.at(p) = 0;
        }
      }
    return true;
  }

  bool splitImageTorgI::getG(const matrix<image::value_type>& img,
                             matrix<channel::value_type>& g) const {
    ipoint p;
    float i;
    g.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {
        const rgbaPixel& pix = img.at(p);
        i = (static_cast<float>(pix.red) + 
             static_cast<float>(pix.green) +
             static_cast<float>(pix.blue));
        if (i>0) {
          g.at(p) = static_cast<float>(pix.green)/i;
        } else {
          g.at(p) = 0.0f;
        }
      }
    return true;
  }

  bool splitImageTorgI::getG(const matrix<image::value_type>& img,
                             matrix<channel8::value_type>& g) const {
    ipoint p;
    int i;
    g.allocate(img.size());

    for (p.y=0;p.y<img.rows();p.y++)
      for (p.x=0;p.x<img.columns();p.x++) {
        const rgbaPixel& pix = img.at(p);
        i = (static_cast<int>(pix.red) +
             static_cast<int>(pix.green) +
             static_cast<int>(pix.blue));
        if (i!=0) {
          g.at(p) = static_cast<ubyte>(static_cast<int>(pix.green)*255/i);
        } else {
          g.at(p) = 0;
        }
      }
    return true;
  }

  bool 
  splitImageTorgI::getIntensity(const matrix<image::value_type>& img,
                                matrix<channel::value_type>& intensity) const {
    int y;
    intensity.allocate(img.size());

    if ((intensity.getMode() == matrix<channel::value_type>::Connected) &&
        (img.getMode() == matrix<image::value_type>::Connected)) {
      matrix<image::value_type>::const_iterator iit,eit;
      matrix<channel::value_type>::iterator it;

      for (iit=img.begin(),eit=img.end(),it=intensity.begin();
           iit != eit;
           ++iit,++it) {
        const rgbaPixel& pix = *iit;
        (*it) = (static_cast<float>(pix.red) + 
                 static_cast<float>(pix.green) +
                 static_cast<float>(pix.blue))/765.0f; // 255*3 = 765
      }
    } else {
      vector<channel::value_type>::iterator it;
      vector<rgbaPixel>::const_iterator iit,eit;

      for (y=0;y<img.rows();y++) {
        const vector<rgbaPixel>& imgVct = img.getRow(y);
        vector<channel::value_type>& intVct = intensity.getRow(y);

        for (iit=imgVct.begin(),eit=imgVct.end(),it=intVct.begin();
             iit!=eit;
             ++iit,++it) {
          const rgbaPixel& pix = *iit;
          (*it) = (static_cast<float>(pix.red) + 
                   static_cast<float>(pix.green) +
                   static_cast<float>(pix.blue))/765.0f; // 255*3 = 765
        }
      }
    }
    return true;
  }
  
  bool 
  splitImageTorgI::getIntensity(const matrix<image::value_type>& img,
                                matrix<channel8::value_type>& intensity) const{

    int y;
    intensity.allocate(img.size());

    if ((intensity.getMode() == matrix<channel8::value_type>::Connected) &&
        (img.getMode() == matrix<image::value_type>::Connected)) {
      matrix<image::value_type>::const_iterator iit,eit;
      matrix<channel8::value_type>::iterator it;

      for (iit=img.begin(),eit=img.end(),it=intensity.begin();
           iit != eit;
           ++iit,++it) {
        const rgbaPixel& pix = *iit;
        *it = static_cast<ubyte>((static_cast<int>(pix.red) +
                                  static_cast<int>(pix.green) +
                                  static_cast<int>(pix.blue))/3);
      }
    } else {
      vector<channel8::value_type>::iterator it;
      vector<rgbaPixel>::const_iterator iit,eit;

      for (y=0;y<img.rows();y++) {
        const vector<rgbaPixel>& imgVct = img.getRow(y);
        vector<channel8::value_type>& intVct = intensity.getRow(y);

        for (iit=imgVct.begin(),eit=imgVct.end(),it=intVct.begin();
             iit!=eit;
             ++iit,++it) {
          const rgbaPixel& pix = *iit;
          *it = static_cast<ubyte>((static_cast<int>(pix.red) +
                                    static_cast<int>(pix.green) +
                                    static_cast<int>(pix.blue))/3);
        }
      }
    }

    return true;
  }


  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageTorgI::getFirst(const matrix<image::value_type>& img,
                                 matrix<channel::value_type>& c1) const {
    return getR(img,c1);
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageTorgI::getFirst(const matrix<image::value_type>& img,
                                 matrix<channel8::value_type>& c1) const {
    return getR(img,c1);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageTorgI::getSecond(const matrix<image::value_type>& img, 
                                  matrix<channel::value_type>& c2) const {
    return getG(img,c2);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageTorgI::getSecond(const matrix<image::value_type>& img,
                                  matrix<channel8::value_type>& c2) const {
    return getG(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageTorgI::getThird(const matrix<image::value_type>& img, 
                                 matrix<channel::value_type>& c2) const {
    return getIntensity(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageTorgI::getThird(const matrix<image::value_type>& img,
                                 matrix<channel8::value_type>& c2) const {
    return getIntensity(img,c2);
  }

} // end of namespace lti

