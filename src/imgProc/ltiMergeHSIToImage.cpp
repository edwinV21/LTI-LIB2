/*
 * Copyright (C) 1998-2006
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
 * \file   ltiMergeHSIToImage.cpp
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.99
 *
 * $Id: ltiMergeHSIToImage.cpp,v 1.6 2007-04-05 22:55:43 alvarado Exp $
 */

#include "ltiMergeHSIToImage.h"
#include "ltiChannel8.h"
#include "ltiRound.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<mergeImage>
  _LTI_REGISTER_IN_FACTORY_AS(HSI,mergeImage,mergeHSIToImage)

  // merge HSI channels to image

  // return the name of this type 
  const std::string& mergeHSIToImage::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // returns a pointer to a clone of the functor.
  mergeHSIToImage* mergeHSIToImage::clone() const {
    return new mergeHSIToImage(*this);
  }

  mergeHSIToImage* mergeHSIToImage::newInstance() const {
    return new mergeHSIToImage;
  }

  const matrix<ubyte>* mergeHSIToImage::DeHSI_ = 0; 

  // Constructor
  mergeHSIToImage::mergeHSIToImage(void) {

    if (isNull(DeHSI_)) {
      // we have to initialize the LUT
      int s,h;
      double alpha, beta;
      static matrix<ubyte> LUT(Reds,Sats);      
      
      static const double hToAlpha = 2*Pi/(3*Reds);

      for (h=0;h<Reds;h++) {
        for (s = 0; s < LUT.columns(); s++) {
          // h ranges is from 0 to 120 degrees
          alpha = h*hToAlpha;
          beta = s*cos(alpha) / cos((Pi/3.0)-alpha) / SatsMax;
          beta = (1.0+beta)*85.0;
          LUT.at(h,s) = static_cast<ubyte>(beta+0.5);
        }
      }

      // make LUT accessible to all 
      DeHSI_ = &LUT;
        
    }
  }

  // fix RGB tuple
  inline void mergeHSIToImage::fix(float& R,float& G,float& B) const {

    if (R >= 255.5f) {
      G*=255.0f/R;
      B*=255.0f/R;
      R=255.0f;
    }
    
    if (G >= 255.5f) {
      R*=255.0f/G;
      B*=255.0f/G;
      G=255.0f;
    }
    
    if (B >= 255.5f) {
      R*=255.0f/B;
      G*=255.0f/B;
      B=255.0f;
    }

    // here all RGB should be in range!
  }

  // merge float channels
  bool mergeHSIToImage::apply(const matrix<float>& c1,
                              const matrix<float>& c2,
                              const matrix<float>& c3,
                              image& img) const {
    int xSize,ySize;
    ipoint p;            // coordinates
    int h;
    float r,g,b,f;       // channels

    xSize=c1.columns();
    ySize=c1.rows();
    if ((c2.columns()!=xSize)||
	(c2.rows()!=ySize)||
	(c3.columns()!=xSize)||
	(c3.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.allocate(ySize,xSize);

    // H = c1
    // S = c2
    // I = c3

    for (p.y=0;p.y<ySize;p.y++) {
      for (p.x=0;p.x<xSize;p.x++) {

	h = static_cast<int>(c1.at(p)*Blues);

        f = c2.at(p)*255.0f; // scaled saturation

	if(h<Reds) {
	  r = DeHSI_->at(h,static_cast<int>(f));
	  b = static_cast<float>((255.0f-f)/3.0f);
	  g = 255.0f-r-b;

	}
	else if (h<Greens) {
	  r = static_cast<float>((255.0f-f)/3.0f);
	  g = DeHSI_->at(h-Reds,static_cast<int>(f));
	  b = 255.0f-r-g;
	}
	else if (h<Blues) {
	  g = static_cast<float>((255.0f-f)/3.0f);
	  b = DeHSI_->at(h-Greens,static_cast<int>(f));
	  r = 255.0f-g-b;
	}
	else {
	  r = DeHSI_->at(h-Blues,static_cast<int>(f));
	  b = static_cast<float> ((255.0f-f)/3.0f);
	  g=255.0f-r-b;
	}

        f = 3.0f*c3.at(p);
	r *= f;
	g *= f;
	b *= f;

        // These corrections are heuristics!  Keep H and I constant and reduce
        // S to bring the larger number to its limit.
        fix(r,g,b);

	img.at(p).set(static_cast<ubyte>(r),
                      static_cast<ubyte>(g),
                      static_cast<ubyte>(b),
                      0);
      }
    }  

    return true;
  }

  // merge 8-bit-channels (quick and dirty, to be speed - optimized...)
  bool mergeHSIToImage::apply(const matrix<ubyte>& c1,
                              const matrix<ubyte>& c2,
                              const matrix<ubyte>& c3,
                              image& img) const {
    int xSize,ySize;
    ipoint p;            // coordinates
    int h,s,I;
    float r,g,b,f;

    xSize = c1.columns();
    ySize = c1.rows();
    if ((c2.columns() != xSize)||
	(c2.rows() != ySize)||
	(c3.columns() != xSize)||
	(c3.rows() != ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.allocate(ySize,xSize);

    // H = c1
    // S = c2
    // I = c3
    for (p.y=0;p.y<ySize;p.y++) {
      for (p.x=0;p.x<xSize;p.x++) {

	// H = static_cast<float>(c1.at(p))/255.0f;
	s = static_cast<int>(c2.at(p));
	I = static_cast<int>(c3.at(p));
	h = (static_cast<int>(c1.at(p))*Blues)/255;
        
        if(h<Reds) {
	  r = DeHSI_->at(h,s);
	  b = static_cast<float>(255-s)/3.0f;
	  g = 255.0f-r-b;
	} else if (h<Greens) {
	  r = static_cast<float>(255-s)/3.0f;
	  g = DeHSI_->at(h-Reds,s);
	  b = 255.0f-r-g;
	} else if (h<Blues) {
	  g = static_cast<float>(255-s)/3.0f;
	  b = DeHSI_->at(h-Greens,s);
	  r = 255-g-b;
	} else {
	  r = DeHSI_->at(h-Blues,s);
	  b = static_cast<float>(255-s)/3.0f;
	  g=255-r-b;
	}

        f = I/85.0f;
        
	r *= f;
	g *= f;
	b *= f;

        fix(r,g,b);

	img.at(p).set(static_cast<ubyte>(r),
                      static_cast<ubyte>(g),
                      static_cast<ubyte>(b),
                      0);
      }
    }
    return true;
  }

  bool mergeHSIToImage::apply(const float& c1,
                              const float& c2,
                              const float& c3,
                              rgbaPixel& pixel) const {

    int h,fi;
    float r,g,b,f;        // channels
    
    // H = c1
    // S = c2
    // I = c3

    h = iround(c1*Blues);
    fi = iround(c2*SatsMax); // scaled saturation

    if(h<Reds) {
      r = DeHSI_->at(h,fi);
      b = static_cast<float>((1.0f-c2)*85.0f);
      g = 255.0f-r-b;      
    }
    else if (h<Greens) {
      r = static_cast<float>((1.0f-c2)*85.0f);
      g = DeHSI_->at(h-Reds,fi);
      b = 255.0f-r-g;
    }
    else if (h<Blues) {
      g = static_cast<float>((1.0f-c2)*85.0f);
      b = DeHSI_->at(h-Greens,fi);
      r = 255.0f-g-b;
    }
    else { // again in the reds
      r = DeHSI_->at(h-Blues,fi);
      b = static_cast<float> ((1.0f-c2)*85.0f);
      g=255.0f-r-b;
    }


    f = 3.0f*c3;
    r *= f;
    g *= f;
    b *= f;

    // These corrections are heuristics!  Keep H and I constant and reduce
    // S to bring the larger number to its limit.
    fix(r,g,b);

    pixel.set(static_cast<ubyte>(r+0.5f),
              static_cast<ubyte>(g+0.5f),
              static_cast<ubyte>(b+0.5f),
              0);

    return true;
  }

  // merge 8-bit-values (quick and dirty, to be speed - optimized...)
  bool mergeHSIToImage::apply(const ubyte& c1,
                              const ubyte& c2,
                              const ubyte& c3,
                              rgbaPixel& pixel) const {
    float r,g,b,f;

    const int s = static_cast<int>(c2);
    const int I = static_cast<int>(c3);
    const int h = (static_cast<int>(c1)*Blues)/255;

    if(h<Reds) {
      r = DeHSI_->at(h,s);
      b = static_cast<float>(255-s)/3.0f;
      g = 255.0f-r-b;
    } else if (h<Greens) {
      r = static_cast<float>(255-s)/3.0f;
      g = DeHSI_->at(h-Reds,s);
      b = 255.0f-r-g;
    } else if (h<Blues) {
      g = static_cast<float>(255-s)/3.0f;
      b = DeHSI_->at(h-Greens,s);
      r = 255-g-b;
    } else {
      r = DeHSI_->at(h-Blues,s);
      b = static_cast<float>(255-s)/3.0f;
      g=255-r-b;
    }
    
    f = I/85.0f;
    
    r *= f;
    g *= f;
    b *= f;
    
    fix(r,g,b);

    pixel.set(static_cast<ubyte>(r),
              static_cast<ubyte>(g),
              static_cast<ubyte>(b),
              0);

    return true;
  }
  
} // end of namespace
