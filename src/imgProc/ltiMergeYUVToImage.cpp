/*
 * Copyright (C) 2003, 2004, 2005, 2006, 2007
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
 * \file   ltiMergeYUVToImage.cpp
 *         Merge YUV channels into a color image
 * \author Volker Schmirgel
 * \author Pablo Alvarado
 * \date   17.10.2003
 *
 * $Id: ltiMergeYUVToImage.cpp,v 1.6 2007-04-06 01:16:32 alvarado Exp $
 */


// merge YUV channels to image

#include "ltiMergeYUVToImage.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<mergeImage>
  _LTI_REGISTER_IN_FACTORY_AS(YUV,mergeImage,mergeYUVToImage)

  // Constructor  
  mergeYUVToImage::mergeYUVToImage() : mergeImage() {
    // create a dummy object, only to ensure that the LUT are also initialized!
    mergeYPbPrToImage dummy;
  }

  // destructor
  mergeYUVToImage::~mergeYUVToImage() {

  }

  // return the name of this type
  // returns the name of this class
  const std::string& mergeYUVToImage::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  mergeYUVToImage& mergeYUVToImage::copy(const mergeYUVToImage& other) {
    mergeImage::copy(other);
    
    return (*this);
  }

  // alias for copy member
  mergeYUVToImage&
    mergeYUVToImage::operator=(const mergeYUVToImage& other) {
    return (copy(other));
  }

  // returns a pointer to a clone of the functor.
  mergeYUVToImage* mergeYUVToImage::clone() const {
    return (new mergeYUVToImage(*this));
  }

  // returns a pointer to a clone of the functor.
  mergeYUVToImage* mergeYUVToImage::newInstance() const {
    return (new mergeYUVToImage);
  }

  // merge float channels
  bool mergeYUVToImage::apply(const matrix<float>& c1,
                              const matrix<float>& c2,
                              const matrix<float>& c3,
                              image& img) const {
 
    const int xSize=c1.columns();
    const int ySize=c1.rows();
    if ((c2.columns()!=xSize)||
	(c2.rows()!=ySize)||
	(c3.columns()!=xSize)||
	(c3.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.allocate(ySize,xSize);

    // Y = c1
    // U = c2
    // V = c3

    const int numRows (img.rows());
    const int numColumns (img.columns());
    int i;
    for(i=0; i<numRows; i++) {
      vector<rgbaPixel>& imgVct = img.getRow(i);
      const vector<float>& c1Vct = c1.getRow(i);
      const vector<float>& c2Vct = c2.getRow(i);
      const vector<float>& c3Vct = c3.getRow(i);

      int j;
      for(j=0; j<numColumns; j++) {
	// take pixel at position j
	apply(c1Vct.at(j), c2Vct.at(j), c3Vct.at(j), imgVct.at(j));

      } // loopcd
    }
    return true;
  }



  // merge 8-bit-channels
  bool mergeYUVToImage::apply(const matrix<ubyte>& c1,
                              const matrix<ubyte>& c2,
                              const matrix<ubyte>& c3,
                              image& img) const {
 
    //point p;            // coordinates
    //rgbaPixel pix;       // pixel structure

    const int xSize=c1.columns();
    const int ySize=c1.rows();
    if ((c2.columns()!=xSize) || (c2.rows()!=ySize) ||
	(c3.columns()!=xSize) || (c3.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.allocate(ySize,xSize);

    // Y = c1
    // U = c2
    // V = c3

    const int numRows (img.rows());
    const int numColumns (img.columns());
    int i;
    for(i=0; i<numRows; i++) {
      vector<rgbaPixel>& imgVct = img.getRow(i);
      const vector<ubyte>& c1Vct = c1.getRow(i);
      const vector<ubyte>& c2Vct = c2.getRow(i);
      const vector<ubyte>& c3Vct = c3.getRow(i);

      int j;
      for(j=0; j<numColumns; j++) {
	// take pixel at position j
	apply(c1Vct.at(j), c2Vct.at(j), c3Vct.at(j), imgVct.at(j));

      } // loopcd
    }

    return true;
  }
}
