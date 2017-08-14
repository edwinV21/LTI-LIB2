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
 * \file   ltiSplitImageToRGB.cpp
 *         Implementation of lti::splitImageToRGB which splits an
 *         image into three different color channels red, green, and
 *         blue (RGB).
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToRGB.cpp,v 1.9 2007-04-05 22:56:59 alvarado Exp $
 */

#include "ltiSplitImageToRGB.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(RGB,splitImage,splitImageToRGB)

  // default constructor
  splitImageToRGB::splitImageToRGB() : splitImage() {
  }

  // copy constructor
  splitImageToRGB::splitImageToRGB(const splitImageToRGB& other) 
    : splitImage() {
    copy(other);
  }

  // destructor
  splitImageToRGB::~splitImageToRGB() {
  }

  // name of the class
  const std::string& splitImageToRGB::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // returns a pointer to a clone of the functor.
  splitImageToRGB* splitImageToRGB::clone() const {
    return (new splitImageToRGB(*this));
  }

  // returns a pointer to a new instance of the functor.
  splitImageToRGB* splitImageToRGB::newInstance() const {
    return (new splitImageToRGB());
  }

  // copy member
  splitImageToRGB& 
  splitImageToRGB::copy(const splitImageToRGB& other) {
    splitImage::copy(other);

    //@TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  splitImageToRGB&
  splitImageToRGB::operator=(const splitImageToRGB& other) {
    return (copy(other));
  }

  // return parameters
  const splitImageToRGB::parameters&
  splitImageToRGB::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&splitImage::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool splitImageToRGB::apply(const matrix<image::value_type>& img,
                              matrix<channel::value_type>& c1,
                              matrix<channel::value_type>& c2,
                              matrix<channel::value_type>& c3) const {
    const int rows=img.rows();
    const int cols=img.columns();
    int i,j;
    
    c1.allocate(rows,cols);
    c2.allocate(rows,cols);
    c3.allocate(rows,cols);
    
    channel::iterator rit,git,bit;
    
    for (i=0,rit=c1.begin(),git=c2.begin(),bit=c3.begin();
         i<rows; i++) {
      for (j=0; j<cols; ++rit, ++git, ++bit, j++) {
        const rgbaPixel& pix = img.at(i,j);
        *rit = static_cast<float>(pix.red)/255.0f; 
        *git = static_cast<float>(pix.green)/255.0f;
        *bit = static_cast<float>(pix.blue)/255.0f;
      }
    }

    return true;
  }

  bool splitImageToRGB::apply(const matrix<image::value_type>& img,
                              matrix<channel::value_type>& c1,
                              matrix<channel::value_type>& c2,
                              matrix<channel::value_type>& c3,
                              matrix<channel::value_type>& c4) const {
    int i,j;
    const int rows=img.rows();
    const int cols=img.columns();

    c1.allocate(rows,cols);
    c2.allocate(rows,cols);
    c3.allocate(rows,cols);
    c4.allocate(rows,cols);
    channel::iterator rit=c1.begin();
    channel::iterator git=c2.begin();
    channel::iterator bit=c3.begin();
    channel::iterator ait=c4.begin();


    for (i=0; i<rows; i++) {
      for (j=0; j<cols; j++, ++rit, ++git, ++bit, ++ait) {
        const rgbaPixel& pix = img.at(i,j);
        (*rit) = static_cast<float>(pix.red)/255.0f;
        (*git) = static_cast<float>(pix.green)/255.0f;
        (*bit) = static_cast<float>(pix.blue)/255.0f;
        (*ait) = static_cast<float>(pix.getAlpha())/255.0f;
      }
    }

    return true;
  }

  bool splitImageToRGB::apply(const matrix<image::value_type>& img,
                              matrix<channel8::value_type>& c1,
                              matrix<channel8::value_type>& c2,
                              matrix<channel8::value_type>& c3) const {
    int i,j;
    const int rows=img.rows();
    const int cols=img.columns();
    c1.allocate(rows,cols);
    c2.allocate(rows,cols);
    c3.allocate(rows,cols);
    channel8::iterator rit=c1.begin();
    channel8::iterator git=c2.begin();
    channel8::iterator bit=c3.begin();

    for (i=0; i<rows; i++)
      for (j=0; j<cols; j++, ++rit, ++git, ++bit) {
        const rgbaPixel& pix = img.at(i,j);

        (*rit) = pix.red;
        (*git) = pix.green;
        (*bit) = pix.blue;
      }
    return true;
  }

  bool splitImageToRGB::apply(const matrix<image::value_type>& img,
                              matrix<channel8::value_type>& c1,
                              matrix<channel8::value_type>& c2,
                              matrix<channel8::value_type>& c3,
                              matrix<channel8::value_type>& c4) const {
    int i,j;
    const int rows=img.rows();
    const int cols=img.columns();

    c1.allocate(rows,cols);
    c2.allocate(rows,cols);
    c3.allocate(rows,cols);
    c4.allocate(rows,cols);
    channel8::iterator rit=c1.begin();
    channel8::iterator git=c2.begin();
    channel8::iterator bit=c3.begin();
    channel8::iterator ait=c4.begin();

    for (i=0; i<rows; i++) {
      for (j=0; j<cols; j++, ++rit, ++git, ++bit, ++ait) {
        const rgbaPixel& pix = img.at(i,j);
        (*rit) = pix.red;
        (*git) = pix.green;
        (*bit) = pix.blue;
        (*ait) = pix.getAlpha();
      }
    }

    return true;
  }

  bool splitImageToRGB::apply(const rgbaPixel& pixel,
                              float& c1,
                              float& c2,
                              float& c3) const {

    c1=static_cast<float>(pixel.red)/255.0f;
    c2=static_cast<float>(pixel.green)/255.0f;
    c3=static_cast<float>(pixel.blue)/255.0f;
    return true;
  }

  bool splitImageToRGB::apply(const rgbaPixel& pixel,
                              ubyte& c1,
                              ubyte& c2,
                              ubyte& c3) const {

    c1=pixel.red;
    c2=pixel.green;
    c3=pixel.blue;
    return true;
  }

  bool splitImageToRGB::extractRed(const matrix<image::value_type>& img,
                                   matrix<channel::value_type>& red) const {
    const int rows=img.rows();
    const int cols=img.columns();
    red.allocate(rows,cols);
    channel::iterator rit=red.begin();
    int i,j;

    for (i=0;i<rows;i++) {
      for (j=0;j<cols;j++) {
        (*rit)=static_cast<float>(img.at(i,j).red)/255.0f;
        ++rit;
      }
    }
    
    return true;
  }

  bool splitImageToRGB::extractRed(const matrix<image::value_type>& img,
                                   matrix<channel8::value_type>& red) const {
    const int rows=img.rows();
    const int cols=img.columns();
    red.allocate(rows,cols);
    channel8::iterator rit=red.begin();
    int i,j;

    for (i=0;i<rows;i++)
      for (j=0;j<cols;j++) {
        (*rit)=img.at(i,j).red;
        ++rit;
      }
    return true;
  }

  bool splitImageToRGB::extractGreen(const matrix<image::value_type>& img,
                                     matrix<channel::value_type>& green) const {
    const int rows=img.rows();
    const int cols=img.columns();
    green.allocate(rows,cols);
    channel::iterator rit=green.begin();
    int i,j;

    for (i=0;i<rows;i++) {
      for (j=0;j<cols;j++) {
        (*rit)=static_cast<float>(img.at(i,j).green)/255.0f;
        ++rit;
      }
    }
    return true;
  }

  bool splitImageToRGB::extractGreen(const matrix<image::value_type>& img,
                                     matrix<channel8::value_type>& green) const {
    const int rows=img.rows();
    const int cols=img.columns();
    green.allocate(rows,cols);
    channel8::iterator rit=green.begin();
    int i,j;

    for (i=0;i<rows;i++)
      for (j=0;j<cols;j++) {
        (*rit)=img.at(i,j).green;
        ++rit;
      }
    return true;
  }

  bool splitImageToRGB::extractBlue(const matrix<image::value_type>& img,
                                    matrix<channel::value_type>& blue) const {
    const int rows=img.rows();
    const int cols=img.columns();
    blue.allocate(rows,cols);
    channel::iterator rit=blue.begin();
    int i,j;

    for (i=0;i<rows;i++) {
      for (j=0;j<cols;j++) {
        (*rit)=static_cast<float>(img.at(i,j).blue)/255.0f;
        ++rit;
      }
    }
    return true;
  }

  bool splitImageToRGB::extractBlue(const matrix<image::value_type>& img,
                                    matrix<channel8::value_type>& blue) const {
    const int rows=img.rows();
    const int cols=img.columns();
    blue.allocate(rows,cols);
    channel8::iterator rit=blue.begin();
    int i,j;

    for (i=0;i<rows;i++)
      for (j=0;j<cols;j++) {
        (*rit)=img.at(i,j).blue;
        ++rit;
      }
    return true;
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageToRGB::extractFirst(const matrix<image::value_type>& img, 
                                     matrix<channel::value_type>& c1) const {
    return extractRed(img,c1);
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageToRGB::extractFirst(const matrix<image::value_type>& img, 
                                     matrix<channel8::value_type>& c1) const {
    return extractRed(img,c1);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageToRGB::extractSecond(const matrix<image::value_type>& img, 
                                      matrix<channel::value_type>& c2) const {
    return extractGreen(img,c2);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageToRGB::extractSecond(const matrix<image::value_type>& img,
                                      matrix<channel8::value_type>& c2) const {
    return extractGreen(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageToRGB::extractThird(const matrix<image::value_type>& img,
                                     matrix<channel::value_type>& c2) const {
    return extractBlue(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageToRGB::extractThird(const matrix<image::value_type>& img,
                                     matrix<channel8::value_type>& c2) const {
    return extractBlue(img,c2);
  }


} // end of namespace lti

