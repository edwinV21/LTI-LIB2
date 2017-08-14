/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiSplitImageToYUV.cpp
 *         Split color image to YUV
 * \author Volker Schmirgel
 * \date   17.12.2002
 *
 * $Id: ltiSplitImageToYUV.cpp,v 1.3 2010-05-08 00:52:49 alvarado Exp $
 */


#include "ltiSplitImageToYUV.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<splitImage>
  _LTI_REGISTER_IN_FACTORY_AS(YUV,splitImage,splitImageToYUV)

  // --------------------------------------------------
  // splitImageToYUV
  // --------------------------------------------------

  // default constructor
  splitImageToYUV::splitImageToYUV() : splitImage() {
  }

  // copy constructor
  splitImageToYUV::splitImageToYUV(const splitImageToYUV& other)  : splitImage() {
    copy(other);
  }

  // destructor
  splitImageToYUV::~splitImageToYUV() {
  }

  // returns the name of this class
  const std::string& splitImageToYUV::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  splitImageToYUV& splitImageToYUV::copy(const splitImageToYUV& other) {
    splitImage::copy(other);

    return (*this);
  }

  // alias for copy member
  splitImageToYUV&
  splitImageToYUV::operator=(const splitImageToYUV& other) {
    return (copy(other));
  }


  // clone member
  splitImageToYUV* splitImageToYUV::clone() const {
    return (new splitImageToYUV(*this));
  }

  splitImageToYUV* splitImageToYUV::newInstance() const {
    return (new splitImageToYUV);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // split image into float channels
  bool splitImageToYUV::apply(const matrix<rgbaPixel>& img,
                              matrix<float>& c1,
                              matrix<float>& c2,
                              matrix<float>& c3) const {

    // make the channels size of source image...
    // Todo: don't initialize
    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    const int numRows (img.rows());
    const int numColumns (img.columns());
    int i;

    for(i=0; i<numRows; i++) {
      const vector<rgbaPixel>& srcVct = img.getRow(i);
      vector<float>& c1Vct = c1.getRow(i);
      vector<float>& c2Vct = c2.getRow(i);
      vector<float>& c3Vct = c3.getRow(i);

      int j;
      for(j=0; j<numColumns; j++) {
        apply(srcVct.at(j), c1Vct.at(j), c2Vct.at(j), c3Vct.at(j));

      } // loopcd
    }
    return true;
  }


  // split image into 8-bit channels
  // N.B.: when casting the transformation result to unsigned shorts
  // (8-bit channel) major rounding errors will occur.
  // As a result, the merging operation might produce negative
  // values or values > 1,  which are truncated subsequently.
  // When accurate Y, U, and V channels are required, prefer float channels!
  bool splitImageToYUV::apply(const matrix<rgbaPixel>& img,
                              matrix<ubyte>& c1,
                              matrix<ubyte>& c2,
                              matrix<ubyte>& c3) const {

    // make the channels size of source image...
    c1.allocate(img.size());
    c2.allocate(img.size());
    c3.allocate(img.size());

    const int numRows (img.rows());
    const int numColumns (img.columns());
    int i;

    for(i=0; i<numRows; i++) {
      const vector<rgbaPixel>& srcVct = img.getRow(i);
      vector<ubyte>& c1Vct = c1.getRow(i);
      vector<ubyte>& c2Vct = c2.getRow(i);
      vector<ubyte>& c3Vct = c3.getRow(i);

      int j;
      for(j=0; j<numColumns; j++) {
        apply(srcVct.at(j), c1Vct.at(j), c2Vct.at(j), c3Vct.at(j));
        
      } // loopcd
    }
    return true;
  }
}
