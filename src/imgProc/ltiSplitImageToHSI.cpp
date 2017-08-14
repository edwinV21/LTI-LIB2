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
 * \file   ltiSplitImageToHSI.cpp
 *         Contains the functor splitImageToHSI which splits images into hue
 *         saturation and intensity.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToHSI.cpp,v 1.5 2007-04-05 22:56:59 alvarado Exp $
 */

#include "ltiSplitImageToHSI.h"
#include "ltiConstants.h"
#include "ltiRound.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(HSI,splitImage,splitImageToHSI)

  // ========= splitImageToHSI ===============
  // Constructor
  splitImageToHSI::splitImageToHSI() : splitImage(), hueLUT_() {
  }

  // copy constructor
  splitImageToHSI::splitImageToHSI(const splitImageToHSI& other) 
    : splitImage(), hueLUT_() {
    copy(other);
  }

  // destructor
  splitImageToHSI::~splitImageToHSI() {
  }

  // name of the class
  const std::string& splitImageToHSI::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // returns a pointer to a clone of the functor.
  splitImageToHSI* splitImageToHSI::clone() const {
    return (new splitImageToHSI(*this));
  }

  // returns a pointer to a new instance of the functor.
  splitImageToHSI* splitImageToHSI::newInstance() const {
    return (new splitImageToHSI());
  }

  // copy member
  splitImageToHSI& 
  splitImageToHSI::copy(const splitImageToHSI& other) {
    splitImage::copy(other);

    return (*this);
  }

  // alias for copy member
  splitImageToHSI&
  splitImageToHSI::operator=(const splitImageToHSI& other) {
    return (copy(other));
  }

  // channels are float images...
  bool splitImageToHSI::apply(const matrix<image::value_type>& img,
                              matrix<channel::value_type>& c1,
                              matrix<channel::value_type>& c2,
                              matrix<channel::value_type>& c3) const {
    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    c1.allocate(rows,cols);  // Hue
    c2.allocate(rows,cols);  // Saturation
    c3.allocate(rows,cols);  // Intensity

    matrix<channel::value_type>::iterator it1=c1.begin();
    matrix<channel::value_type>::iterator it2=c2.begin();
    matrix<channel::value_type>::iterator it3=c3.begin();

    static const int maxVal = 511;


    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it1, ++it2, ++it3) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);

        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        const float I3 = static_cast<float>(R+G+B);
        if(I3 > 0) {

          const int r = static_cast<int>(maxVal*R/I3 + 0.49f);
          const int g = static_cast<int>(maxVal*G/I3 + 0.5f);
          const int mini= static_cast<int>(minimum(R, G, B));

          (*it1) = hueLUT_(r,g);                      // H: range 0..1
          (*it2) = 1.0f - ((3.0f*mini)/I3);           // S
          (*it3) = I3/765.0f;    // 765 = 255.0f*3    // I

        } else {

          (*it1) = 0;        // H
          (*it2) = 0;        // S
          (*it3) = 0;        // I

        }
      }// loops
    return true;
  }


  // channels are 8-bit grey value matrix<image::value_type>s
  bool splitImageToHSI::apply(const matrix<image::value_type>& img,
                              matrix<channel8::value_type>& c1,
                              matrix<channel8::value_type>& c2,
                              matrix<channel8::value_type>& c3) const {
    int x,y;      //coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    c1.allocate(rows,cols);  // Hue
    c2.allocate(rows,cols);  // Saturation
    c3.allocate(rows,cols);  // Intensity

    matrix<channel8::value_type>::iterator it1=c1.begin();
    matrix<channel8::value_type>::iterator it2=c2.begin();
    matrix<channel8::value_type>::iterator it3=c3.begin();
    
    static const int maxVal = 511;

    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..255
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it1, ++it2, ++it3) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);

        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        const float I3 = static_cast<float>(R+G+B);
        if(I3 > 0) {

          const int r = static_cast<int>(maxVal*R/I3 + 0.49f);
          const int g = static_cast<int>(maxVal*G/I3 + 0.5f);
          const int mini= static_cast<int>(minimum(R, G, B));

          (*it1) = static_cast<ubyte>(255.0f*hueLUT_(r,g)+0.5f); // H 
          (*it2) = static_cast<ubyte>(255.5f - ((765.f*mini)/I3)); // S
          (*it3) = static_cast<ubyte>(I3/3.0f + 0.5); // I

        } else {

          (*it1) = 0;        // H
          (*it2) = 0;        // S
          (*it3) = 0;        // I

        }
      }// loops
    return true;
  }

  bool splitImageToHSI::apply(const rgbaPixel& pix,
                              float& H,
                              float& S,
                              float& I) const {

    static const int maxVal = 511;


    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I
    // note:
    // Triangular matrix HueHSI -> 0..36000.0

    const int R=pix.red;
    const int G=pix.green;
    const int B=pix.blue;
    
    const float I3 = static_cast<float>(R+G+B);
    if(I3 > 0) {
      // we cannot round both, since, for instance if rgbaPixel(1,1,0) would
      // produce (256,256) which is not valid!  And green is more relevant!
      const int r = static_cast<int>(maxVal*R/I3 + 0.49f);  // cut
      const int g = static_cast<int>(maxVal*G/I3 + 0.5f);   // round
      const int mini= static_cast<int>(minimum(R, G, B));
      
      H = hueLUT_(r,g);                      // H: range 0..1
      S = 1.0f - ((3.0f*mini)/I3);           // S
      I = I3/765.0f;    // 765 = 255.0f*3    // I
      
    } else {
      
      H = 0;        // H
      S = 0;        // S
      I = 0;        // I
      
    }
    return true;
  }

  bool splitImageToHSI::apply(const rgbaPixel& pix,
                              ubyte& H,
                              ubyte& S,
                              ubyte& I) const {

    static const int maxVal = 511;

    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I
    // note:
    // Triangular matrix HueHSI -> 0..36000.0

    const int R=pix.red;
    const int G=pix.green;
    const int B=pix.blue;

    const float I3 = static_cast<float>(R+G+B);
    if(I3 > 0) {
      
      const int r = static_cast<int>(maxVal*R/I3 + 0.49f);
      const int g = static_cast<int>(maxVal*G/I3 + 0.5f);
      const int mini= static_cast<int>(minimum(R, G, B));
      
      H = static_cast<ubyte>(255.0f*hueLUT_(r,g)+0.5);    // H 
      S = static_cast<ubyte>(255.5f - ((765.f*mini)/I3)); // S
      I = static_cast<ubyte>(I3/3.f + 0.5);               // I
      
    } else {
      
      H = 0;        // H
      S = 0;        // S
      I = 0;        // I
      
    }
    return true;
  }

  /**
   * return the hue of the image.  If you need also the saturation and
   * the intensity please use the apply methods, which are much faster!
   */
  bool splitImageToHSI::extractHue(const matrix<image::value_type>& img,
                                   matrix<channel::value_type>& hue) const {

    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    hue.allocate(rows,cols);  
    matrix<channel::value_type>::iterator it1=hue.begin();

    static const int maxVal = 511;


    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it1) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);
        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        const float I3 = static_cast<float>(R+G+B);
        if(I3 > 0) {

          const int r = static_cast<int>(maxVal*R/I3 + 0.49f);
          const int g = static_cast<int>(maxVal*G/I3 + 0.5f);

          (*it1) = hueLUT_(r,g); // H: range 0..1
          
        } else {

          (*it1) = 0;        // H

        }
      }// loops
    return true;
  }

  /**
   * return the hue of the image.  If you need also the saturation and
   * the intensity please use the apply methods, which are much faster!
   */
  bool splitImageToHSI::extractHue(const matrix<image::value_type>& img,
                                   matrix<channel8::value_type>& hue) const {

    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    hue.allocate(rows,cols);  
    matrix<channel8::value_type>::iterator it1=hue.begin();

    static const int maxVal = 511;


    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it1) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);
        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        const float I3 = static_cast<float>(R+G+B);
        if(I3 > 0) {

          const int r = static_cast<int>((maxVal*R)/I3);
          const int g = static_cast<int>((maxVal*G)/I3);

          (*it1) = static_cast<ubyte>(255.f*hueLUT_(r,g) + 0.5);

        } else {

          (*it1) = 0;        // H

        }
      }// loops
    return true;
  }

  /**
   * return the saturation of the image.  If you need also the hue and
   * the intensity please use the apply methods, which are much faster!
   */
  bool splitImageToHSI::extractSaturation(const matrix<image::value_type>& img,
                                      matrix<channel::value_type>& sat) const {

    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    sat.allocate(rows,cols);  // Hue
    matrix<channel::value_type>::iterator it2=sat.begin();
    
    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it2) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);
        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        const float I3 = static_cast<float>(R+G+B);
        if(I3 > 0) {

          const float mini= static_cast<float>(minimum(R, G, B));

          (*it2) = 1.0f - ((3.0f*mini)/I3);           // S

        } else {

          (*it2) = 0;        // S

        }
      }// loops
    return true;
  }

  /**
   * return the saturation of the image.  If you need also the hue and
   * the saturation please use the apply methods, which are much faster!
   */
  bool splitImageToHSI::extractSaturation(const matrix<image::value_type>& img,
                                      matrix<channel8::value_type>& sat) const{

    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    sat.allocate(rows,cols);  // Hue
    matrix<channel8::value_type>::iterator it2=sat.begin();

    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it2) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);
        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        const float I3 = static_cast<float>(R+G+B);
        if(I3 > 0) {

          const float mini= static_cast<float>(minimum(R, G, B));

          (*it2) = static_cast<ubyte>(255.5f-((765.f*mini)/I3));

        } else {

          (*it2) = 0;        // S

        }
      }// loops
    return true;
  }

  /**
   * return the intensity of the image.  If you need also the hue and
   * the intensity please use the apply methods, which are much faster!
   */
  bool splitImageToHSI::extractIntensity(const matrix<image::value_type>& img,
                                     matrix<channel::value_type>& inty) const {
    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    inty.allocate(rows,cols);  
    matrix<channel::value_type>::iterator it3=inty.begin();
    
    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it3) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);
        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        (*it3) = static_cast<float>(R+G+B)/765.0f;    // 765 = 255.0f*3 
      }// loops
    return true;
  }

  /**
   * return the intensity of the image.  If you need also the hue and
   * the intensity please use the apply methods, which are much faster!
   */
  bool splitImageToHSI::extractIntensity(const matrix<image::value_type>& img,
                                     matrix<channel8::value_type>& inty) const{
    
    int x,y;      // coords

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    inty.allocate(rows,cols);  
    matrix<channel8::value_type>::iterator it3=inty.begin();

    // after Gonzales & Woods:
    // I between 0 and 1
    // H         0 and 360 here 0..1
    // S     0 and 1
    // with I = (R+G+B)/3
    //      H = acos(.5*((R-G)+(R-B))/sqrt((R-G)*(R-G)+(R-B)*(G-B)))
    //      S = 1 - min(R,G,B)/I

    for (y=0; y<rows; y++)
      for (x=0; x<cols ; x++, ++it3) {
        // take pixel at position p
        const rgbaPixel pix = img.at(y,x);
        const int R=pix.red;
        const int G=pix.green;
        const int B=pix.blue;

        (*it3) = static_cast<ubyte>((R+G+B)/3.0f + 0.5f); 
      }// loops
    return true;

  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageToHSI::extractFirst(const matrix<image::value_type>& img, 
                                 matrix<channel::value_type>& c1) const {
    return extractHue(img,c1);
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageToHSI::extractFirst(const matrix<image::value_type>& img, 
                                 matrix<channel8::value_type>& c1) const {
    return extractHue(img,c1);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageToHSI::extractSecond(const matrix<image::value_type>& img, 
                                  matrix<channel::value_type>& c2) const {
    return extractSaturation(img,c2);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageToHSI::extractSecond(const matrix<image::value_type>& img, 
                                  matrix<channel8::value_type>& c2) const {
    return extractSaturation(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageToHSI::extractThird(const matrix<image::value_type>& img, 
                                 matrix<channel::value_type>& c2) const {
    return extractIntensity(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageToHSI::extractThird(const matrix<image::value_type>& img, 
                                 matrix<channel8::value_type>& c2) const {
    return extractIntensity(img,c2);
  }

} // end of namespace lti

