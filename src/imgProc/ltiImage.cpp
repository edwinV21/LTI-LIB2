/*
 * Copyright (C) 1998-2004
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


/*
 * \file   ltiImage.cpp
 * \author Pablo Alvarado
 * \date   10.07.2000
 *
 * $Id: ltiImage.cpp,v 1.12 2013-09-10 14:37:25 alvarado Exp $
 */

#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiChannel32.h"
 
namespace lti {
  // --------------------------------------------------------------------------
  //                             IMAGE
  // --------------------------------------------------------------------------

  image::image() : matrix<rgbaPixel>() {
  }

  image::image(const int theRows,const int theCols)
    : matrix<rgbaPixel>(theRows,theCols) {
  }

  image::image(const ipoint& theSize)
    : matrix<rgbaPixel>(theSize) {
  }

  image::image(const int theRows,const int theCols,const rgbaPixel& iniValue)
    : matrix<rgbaPixel>(theRows,theCols,iniValue) {
  }

  image::image(const ipoint& theSize,const rgbaPixel& iniValue)
      : matrix<rgbaPixel>(theSize,iniValue) {
  }

  image::image(const int theRows,const int theCols,const rgbaPixel _data[])
    : matrix<rgbaPixel>(theRows,theCols,_data) {
  }

  image::image(const image& other) 
    : matrix<rgbaPixel>(other) {    
  }

  image::image(const image& other,
               const ipoint& from,
               const ipoint& to) 
    : matrix<rgbaPixel>(other,from,to) {    
  }

  image::image(const image& other,
               const int fromRow,
               const int fromCol,
               const int toRow,
               const int toCol)
    : matrix<rgbaPixel>(other,fromRow,fromCol,toRow,toCol) {
  }

  image::image(const bool copyData, image& other,
               const int fromRow,
               const int fromCol,
               const int toRow,
               const int toCol)
    : matrix<rgbaPixel>(copyData,other,fromRow,fromCol,toRow,toCol) {
  }

  // class name
  const std::string& image::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * create a clone of this image
   * @return a pointer to a copy of this matrix
   */
  image* image::clone() const {
    return new image(*this);
  }

  image* image::newInstance() const {
    return new image();
  }

  /*
   * Cast from the <code>other</code> channel8.
   */
  image& image::castFrom(const matrix<ubyte>& other) {
    allocate(other.size());
    if (other.empty()) {
      return *this;
    }
    iterator it;
    vector<ubyte>::const_iterator cit,eit;
    int y;
    it = begin();
    for (y=0;y<other.rows();++y) {
      cit = other.getRow(y).begin();
      eit = other.getRow(y).end();
      for (;cit!=eit;++cit,++it) {
        (*it).set(*cit,*cit,*cit);
      }
    }

    return *this;
  }


  /*
   * Cast from the <code>other</code> channel.
   */
  image& image::castFrom(const fmatrix& other,
                         const bool minToBlack,
                         const bool maxToWhite) {
    
    allocate(other.size());
    if (other.empty()) {
      return *this;
    }
    iterator it;
    vector<float>::const_iterator cit,eit;
    int y;
    ubyte c;
    it = begin();
    if (!minToBlack && !maxToWhite) {
      for (y=0;y<other.rows();++y) {
        cit = other.getRow(y).begin();
        eit = other.getRow(y).end();
        for (;cit!=eit;++cit,++it) {
          c = ((*cit) <= 0) ? 0 : (((*cit) >= 1.0f) ?
                                   255 : static_cast<ubyte>((*cit)*255.0f));
          (*it).set(c,c,c);
        }
      }
    } else {

      float theMin,theMax;
      if (minToBlack && maxToWhite) {
        other.findExtremes(theMin,theMax);
      } else if (minToBlack) {
        theMin = other.findMinimum();
        theMax = 1.0f;
      } else {
        theMin = 0.0f;
        theMax = other.findMaximum();
      }

      float tm,tb;

      if (theMax == theMin) {
        tm = 0.0f;
        tb = 127.0f;
      } else {
        tm = 255.0f/(theMax-theMin);
        tb = -tm*theMin+0.5f;
      }

      const float m=tm;
      const float b=tb;

      for (y=0,it=begin();y<other.rows();++y) {
        const vector<channel::value_type>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          c = static_cast<ubyte>((*cit)*m+b);
          (*it).set(c,c,c);
        }
      }
    }

    return *this;
  }


  /*
   * cast from the <code>other</code> channel.
   */
  image& image::castFrom(const matrix<int32>& other,
                         const bool minToBlack,
                         const bool maxToWhite) {
    
    allocate(other.size());
    if (other.empty()) {
      return *this;
    }
    iterator it;
    vector<int32>::const_iterator cit,eit;
    int y;
    ubyte c;
    it = begin();
    if (!minToBlack && !maxToWhite) {
      for (y=0;y<other.rows();++y) {
        cit = other.getRow(y).begin();
        eit = other.getRow(y).end();
        for (;cit!=eit;++cit,++it) {
          c = ((*cit) <= 0) ? 0 : (((*cit) >= 65536) ?
                                   255 : static_cast<ubyte>((*cit)/256));
          (*it).set(c,c,c);
        }
      }
    } else {

      int32 theMin,theMax;
      if (minToBlack && maxToWhite) {
        other.findExtremes(theMin,theMax);
      } else if (minToBlack) {
        theMin = other.findMinimum();
        theMax = 65535;
      } else {
        theMin = 0;
        theMax = other.findMaximum();
      }

      float tm,tb;

      if (theMax == theMin) {
        tm = 0.0f;
        tb = 127.0f;
      } else {
        tm = 255.0f/(theMax-theMin);
        tb = -tm*theMin+0.5f;
      }

      const float m=tm;
      const float b=tb;

      for (y=0,it=begin();y<other.rows();++y) {
        const vector<int32>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          c = static_cast<ubyte>((*cit)*m+b);
          (*it).set(c,c,c);
        }
      }
    }

    return *this;
  }

}

