/*
 * Copyright (C) 1998 - 2005
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2012
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiSplitImageToHSV.cpp
 *         Contains the functor splitImageToHSV which splits images into hue
 *         saturation and value
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImageToHSV.cpp,v 1.1 2012-09-09 02:26:29 alvarado Exp $
 */

#include "ltiSplitImageToHSV.h"
#include "ltiConstants.h"
#include "ltiRound.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(HSV,splitImage,splitImageToHSV)

  // ========= splitImageToHSV ===============
  // Constructor
  splitImageToHSV::splitImageToHSV() : splitImage() {
  }

  // copy constructor
  splitImageToHSV::splitImageToHSV(const splitImageToHSV& other) 
    : splitImage() {
    copy(other);
  }

  // destructor
  splitImageToHSV::~splitImageToHSV() {
  }

  // name of the class
  const std::string& splitImageToHSV::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // returns a ipointer to a clone of the functor.
  splitImageToHSV* splitImageToHSV::clone() const {
    return (new splitImageToHSV(*this));
  }

  // returns a ipointer to a new instance of the functor.
  splitImageToHSV* splitImageToHSV::newInstance() const {
    return (new splitImageToHSV());
  }

  // copy member
  splitImageToHSV& 
  splitImageToHSV::copy(const splitImageToHSV& other) {
    splitImage::copy(other);

    return (*this);
  }

  // alias for copy member
  splitImageToHSV&
  splitImageToHSV::operator=(const splitImageToHSV& other) {
    return (copy(other));
  }

  // channels are float images...
  bool splitImageToHSV::apply(const matrix<image::value_type>& img,
                              matrix<channel::value_type>& c1,
                              matrix<channel::value_type>& c2,
                              matrix<channel::value_type>& c3) const {
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    c1.allocate(rows,cols);
    c2.allocate(rows,cols);
    c3.allocate(rows,cols);

    float mini,delta;
    float H,S,V;          // channels

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {
        // take pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        mini = (float) minimum(pix.getRed(),pix.getGreen(),pix.getBlue());
        if (V!=0) {
          S = 1.0f - (mini/V);
          if (S==0) {
            H=0;
          }
          else {
            delta = (V-mini)*6.0f;
            if (pix.getRed()==V)
              H = (pix.getGreen()-pix.getBlue())/delta;
            else if (pix.getGreen()==V)
              H = 0.333333f + (pix.getBlue()-pix.getRed())/delta;
            else
              H = 0.666667f + (pix.getRed()-pix.getGreen())/delta;
            if (H<0)
              H = H + 1.0f;
          }
        }
        else {
          S = 1.0f;
          H = 0;
        }

        V /= 255.0f ;  // V zwischen 0 und 1

        c1.at(p) = H;
        c2.at(p) = S;
        c3.at(p) = V;
      } // loop
    }
    return true;
  }


  // channels are 8-bit grey value matrix<image::value_type>s
  bool splitImageToHSV::apply(const matrix<image::value_type>& img,
                              matrix<channel8::value_type>& c1,
                              matrix<channel8::value_type>& c2,
                              matrix<channel8::value_type>& c3) const {
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    c1.resize(rows,cols);
    c2.resize(rows,cols);
    c3.resize(rows,cols);

    float mini,delta;
    float H,S,V;

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {

        // take image-pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        mini = (float) minimum(pix.getRed(),pix.getGreen(),pix.getBlue());
        if (V!=0) {
          S = 1.0f - (mini/V);
          if (S==0) {
            H=0;
          }
          else {
            delta = (V-mini)*6.0f;
            if (pix.getRed()==V)
              H = (pix.getGreen()-pix.getBlue())/delta;
            else if (pix.getGreen()==V)
              H = 0.333333f + (pix.getBlue()-pix.getRed())/delta;
            else
              H = 0.666667f + (pix.getRed()-pix.getGreen())/delta;
            if (H<0)
              H = H + 1.0f;
          }
        }
        else {
          S = 1.0f;
          H = 0;
        }

        c1.at(p) = (ubyte) (H*255.0f);
        c2.at(p) = (ubyte) (S*255.0f);
        c3.at(p) = (ubyte) V;
      }// loop
    }
    return true;
  }

  bool splitImageToHSV::apply(const rgbaPixel& pix,
                              float& H,
                              float& S,
                              float& V) const {
    float mini,delta;
    const ubyte& red=pix.getRed();
    const ubyte& green=pix.getGreen();
    const ubyte& blue=pix.getBlue();


    V = (float) maximum(red,green,blue);
    mini = (float) minimum(red,green,blue);
    if (V!=0) {
      S = 1.0f - (mini/V);
      if (S==0) {
        H=0;
      } else {
        delta = (V-mini)*6.0f;
        if (red==V) {
          H = (green-blue)/delta;
        } else if (green==V) {
          H = 0.333333f + (blue-red)/delta;
        } else {
          H = 0.666667f + (red-green)/delta;
        }
        if (H<0) {
          H = H + 1.0f;
        }
      }
    }
    else {
      S = 1.0f;
      H = 0;
    }

    V /= 255.0f ;  // V between 0 and 1


    return true;
  }

  bool splitImageToHSV::apply(const rgbaPixel& pix,
                              ubyte& H,
                              ubyte& S,
                              ubyte& V) const {

    float mini,delta;
    float h,s,v;

    const ubyte& red=pix.getRed();
    const ubyte& green=pix.getGreen();
    const ubyte& blue=pix.getBlue();

    v = (float) maximum(red,green,blue);
    mini = (float) minimum(red,green,blue);
    if (v!=0) {
      s = 1.0f - (mini/v);
      if (s==0) {
        h=0;
      } else {
        delta = (v-mini)*6.0f;
        if (red==v) {
          h = (green-blue)/delta;
        } else if (green==v) {
          h = 0.333333f + (blue-red)/delta;
        } else {
          h = 0.666667f + (red-green)/delta;
        }
        if (h<0) {
          h = h + 1.0f;
        }
      }
    } else {
      s = 1.0f;
      h = 0;
    }

    H = (ubyte) (h*255.0f);
    S = (ubyte) (s*255.0f);
    V = (ubyte) v;


    return true;
  }

  /**
   * return the hue of the image.  If you need also the saturation and
   * the value please use the apply methods, which are much faster!
   */
  bool splitImageToHSV::extractHue(const matrix<image::value_type>& img,
                                   matrix<channel::value_type>& hue) const {
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    hue.allocate(rows,cols);

    float mini,delta;
    float H,S,V;          // channels

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {
        // take pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        mini = (float) minimum(pix.getRed(),pix.getGreen(),pix.getBlue());
        if (V!=0) {
          S = 1.0f - (mini/V);
          if (S==0) {
            H=0;
          }
          else {
            delta = (V-mini)*6.0f;
            if (pix.getRed()==V)
              H = (pix.getGreen()-pix.getBlue())/delta;
            else if (pix.getGreen()==V)
              H = 0.333333f + (pix.getBlue()-pix.getRed())/delta;
            else
              H = 0.666667f + (pix.getRed()-pix.getGreen())/delta;
            if (H<0)
              H = H + 1.0f;
          }
        }
        else {
          S = 1.0f;
          H = 0;
        }

        V /= 255.0f ;  // V zwischen 0 und 1

        hue.at(p) = H;
      } // loop
    }
    
    return true;
  }

  /**
   * return the hue of the image.  If you need also the saturation and
   * the value please use the apply methods, which are much faster!
   */
  bool splitImageToHSV::extractHue(const matrix<image::value_type>& img,
                                   matrix<channel8::value_type>& hue) const {

    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    hue.resize(rows,cols);

    float mini,delta;
    float H,S,V;

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {

        // take image-pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        mini = (float) minimum(pix.getRed(),pix.getGreen(),pix.getBlue());
        if (V!=0) {
          S = 1.0f - (mini/V);
          if (S==0) {
            H=0;
          }
          else {
            delta = (V-mini)*6.0f;
            if (pix.getRed()==V)
              H = (pix.getGreen()-pix.getBlue())/delta;
            else if (pix.getGreen()==V)
              H = 0.333333f + (pix.getBlue()-pix.getRed())/delta;
            else
              H = 0.666667f + (pix.getRed()-pix.getGreen())/delta;
            if (H<0)
              H = H + 1.0f;
          }
        }
        else {
          S = 1.0f;
          H = 0;
        }

        hue.at(p) = (ubyte) (H*255.0f);
      }// loop
    }
    return true;
  }

  /**
   * return the saturation of the image.  If you need also the hue and
   * the value please use the apply methods, which are much faster!
   */
  bool splitImageToHSV::extractSaturation(const matrix<image::value_type>& img,
                                      matrix<channel::value_type>& sat) const {
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    sat.allocate(rows,cols);

    float mini;
    float S,V;          // channels

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {
        // take pixel at position p
        pix = img.at(p);
        
        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        mini = (float) minimum(pix.getRed(),pix.getGreen(),pix.getBlue());
        if (V!=0) {
          S = 1.0f - (mini/V);
        } else {
          S = 1.0f;
        }
        
        sat.at(p) = S;
      } // loop
    }
    return true;

  }

  /**
   * return the saturation of the image.  If you need also the hue and
   * the saturation please use the apply methods, which are much faster!
   */
  bool splitImageToHSV::extractSaturation(const matrix<image::value_type>& img,
                                      matrix<channel8::value_type>& sat) const{
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    sat.resize(rows,cols);

    float mini;
    float S,V;

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {

        // take image-pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        mini = (float) minimum(pix.getRed(),pix.getGreen(),pix.getBlue());
        if (V!=0) {
          S = 1.0f - (mini/V);
        } else {
          S = 1.0f;
        }

        sat.at(p) = (ubyte) (S*255.0f);
      }// loop
    }
    return true;

  }

  /**
   * return the value of the image.  If you need also the hue and
   * the value please use the apply methods, which are much faster!
   */
  bool splitImageToHSV::extractValue(const matrix<image::value_type>& img,
                                     matrix<channel::value_type>& val) const {
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    val.allocate(rows,cols);

    float V;          // channels

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {
        // take pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        V /= 255.0f ;  // V zwischen 0 und 1

        val.at(p) = V;
      } // loop
    }

    return true;
  }

  /**
   * return the value of the image.  If you need also the hue and
   * the value please use the apply methods, which are much faster!
   */
  bool splitImageToHSV::extractValue(const matrix<image::value_type>& img,
                                     matrix<channel8::value_type>& val) const{
    
    ipoint p;        // coordinates
    rgbaPixel pix;   // single Pixel Element in RGB-values...

    const int rows = img.rows();
    const int cols = img.columns();

    // make the channels size of source image...
    val.resize(rows,cols);

    float V;

    for (p.y=0;p.y<rows;p.y++) {
      for (p.x=0;p.x<cols;p.x++) {

        // take image-pixel at position p
        pix = img.at(p);

        V = (float) maximum(pix.getRed(),pix.getGreen(),pix.getBlue());
        val.at(p) = (ubyte) V;
      }// loop
    }
    return true;
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageToHSV::extractFirst(const matrix<image::value_type>& img, 
                                 matrix<channel::value_type>& c1) const {
    return extractHue(img,c1);
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImageToHSV::extractFirst(const matrix<image::value_type>& img, 
                                 matrix<channel8::value_type>& c1) const {
    return extractHue(img,c1);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageToHSV::extractSecond(const matrix<image::value_type>& img, 
                                  matrix<channel::value_type>& c2) const {
    return extractSaturation(img,c2);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImageToHSV::extractSecond(const matrix<image::value_type>& img, 
                                  matrix<channel8::value_type>& c2) const {
    return extractSaturation(img,c2);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageToHSV::extractThird(const matrix<image::value_type>& img, 
                                 matrix<channel::value_type>& c3) const {
    return extractValue(img,c3);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImageToHSV::extractThird(const matrix<image::value_type>& img, 
                                 matrix<channel8::value_type>& c3) const {
    return extractValue(img,c3);
  }

} // end of namespace lti

