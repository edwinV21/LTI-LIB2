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
 * \file   ltiMergeRGBToImage.cpp
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 * 
 * $Id: ltiMergeRGBToImage.cpp,v 1.5 2009-10-31 14:08:45 alvarado Exp $
 */


#include "ltiMergeRGBToImage.h"
#include "ltiFactory.h"

namespace lti {

  // register in factory<mergeImage>
  _LTI_REGISTER_IN_FACTORY_AS(RGB,mergeImage,mergeRGBToImage)


  mergeRGBToImage::mergeRGBToImage() : mergeImage() {
  }

  // merge RGB channels to image
  // clone member
  mergeRGBToImage* mergeRGBToImage::clone() const {
    return new mergeRGBToImage(*this);
  }

  // newInstance() member
  mergeRGBToImage* mergeRGBToImage::newInstance() const {
    return new mergeRGBToImage();
  }

  // class name
  const std::string& mergeRGBToImage::name() const {
    _LTI_RETURN_CLASS_NAME
  }


  bool mergeRGBToImage::apply(const matrix<float>& c1,
                              const matrix<float>& c2,
                              const matrix<float>& c3,
                              image& img) const {
    int xSize,ySize;
    ipoint p;               // coordinates

    xSize=c1.columns();
    ySize=c1.rows();
    if ((c2.columns()!=xSize)||
	(c2.rows()!=ySize)||
	(c3.columns()!=xSize)||
	(c3.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.resize(ySize,xSize,rgbaPixel(),AllocateOnly);

    for (p.y=0;p.y<ySize;p.y++)
      for (p.x=0;p.x<xSize;p.x++) {
	img.at(p).set(static_cast<ubyte>(c1.at(p)*255.0f),
                      static_cast<ubyte>(c2.at(p)*255.0f),
                      static_cast<ubyte>(c3.at(p)*255.0f),
                      0);
      }

    return true;
  }

  bool mergeRGBToImage::apply(const matrix<float>& c1,
                              const matrix<float>& c2,
                              const matrix<float>& c3,
                              const matrix<float>& c4,
                              image& img) const {
    int xSize,ySize;
    ipoint p;               // coordinates

    xSize=c1.columns();
    ySize=c1.rows();
    if ((c2.columns()!=xSize) || (c2.rows()!=ySize) ||
	(c3.columns()!=xSize) || (c3.rows()!=ySize) ||
        (c4.columns()!=xSize) || (c4.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.resize(ySize,xSize,rgbaPixel(),AllocateOnly);

    for (p.y=0;p.y<ySize;p.y++)
      for (p.x=0;p.x<xSize;p.x++) {
	img.at(p).set(static_cast<ubyte>(c1.at(p)*255.0f),
                      static_cast<ubyte>(c2.at(p)*255.0f),
                      static_cast<ubyte>(c3.at(p)*255.0f),
                      static_cast<ubyte>(c4.at(p)*255.0f));
      }

    return true;
  }

  bool mergeRGBToImage::apply(const matrix<ubyte>& c1,
                              const matrix<ubyte>& c2,
                              const matrix<ubyte>& c3,
                              image& img) const {
    int xSize,ySize;
    ipoint p;            // coordinates

    xSize=c1.columns();
    ySize=c1.rows();
    if((c2.columns()!=xSize) || (c2.rows()!=ySize)||
       (c3.columns()!=xSize) || (c3.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.resize(ySize,xSize,rgbaPixel(),AllocateOnly);

    for (p.y=0;p.y<ySize;p.y++) {
      for (p.x=0;p.x<xSize;p.x++) {
	img.at(p).set(c1.at(p),c2.at(p),c3.at(p),0);
      }
    }

    return true;
  }

  bool mergeRGBToImage::apply(const matrix<ubyte>& c1,
                              const matrix<ubyte>& c2,
                              const matrix<ubyte>& c3,
                              const matrix<ubyte>& c4,
                              image& img) const {
    int xSize,ySize;
    ipoint p;            // coordinates

    xSize=c1.columns();
    ySize=c1.rows();
    if((c2.columns()!=xSize) || (c2.rows()!=ySize) ||
       (c3.columns()!=xSize) || (c3.rows()!=ySize) ||
       (c4.columns()!=xSize) || (c4.rows()!=ySize)) {
      setStatusString("sizes of channels do not match");
      return false;
    }

    img.resize(ySize,xSize,rgbaPixel(),AllocateOnly);

    for (p.y=0;p.y<ySize;p.y++) {
      for (p.x=0;p.x<xSize;p.x++) {
	img.at(p).set(c1.at(p),c2.at(p),c3.at(p),c4.at(p));
      }
    }

    return true;
  }

  bool mergeRGBToImage::apply(const float& red,
			      const float& green,
			      const float& blue,
			      rgbaPixel& pixel) const {

    pixel.set(static_cast<ubyte>(red  *255.0f),
              static_cast<ubyte>(green*255.0f),
              static_cast<ubyte>(blue *255.0f),
              0);

    return true;
  }

  bool mergeRGBToImage::apply(const ubyte& red,
			      const ubyte& green,
			      const ubyte& blue,
			      rgbaPixel& pixel) const{

    pixel.set(red,green,blue,0);
    return true;
  }


} // end of namespace

