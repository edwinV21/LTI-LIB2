/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiChannel.cpp
 * \author Pablo Alvarado
 * \date   10.07.2000
 *
 * $Id: ltiChannel.cpp,v 1.9 2010-04-25 05:26:54 alvarado Exp $
 */

#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiImage.h"

namespace lti {

  // --------------------------------------------------------------------------
  //                    CHANNEL
  // --------------------------------------------------------------------------


  /*
   * default constructor creates an empty channel
   */
  channel::channel()
    : matrix<float>() {
  }

  channel::channel(const int r,const int c)
    : matrix<float>(r,c) {
  }

  channel::channel(const ipoint& sz)
    : matrix<float>(sz) {
  }

  channel::channel(const int r,const int c,const float& iniValue)
    : matrix<float>(r,c,iniValue) {
  }

  channel::channel(const ipoint& sz,const float& iniValue)
    : matrix<float>(sz,iniValue) {
  }
  
  channel::channel(const int r,const int c,const float _data[])
    : matrix<float>(r,c,_data) {
  }
  
  channel::channel(const genericMatrix<float>& other,
                   const int fromRow,
                   const int fromCol,
                   const int toRow,
                   const int toCol)
    : matrix<float>(other,fromRow,fromCol,toRow,toCol) {
  }

  channel::channel(const genericMatrix<float>& other)
    : matrix<float>(other) {
  }

  channel::channel(const genericMatrix<float>& other,
                   const ipoint& from,
                   const ipoint& to)
    : matrix<float>(other,from,to) {
  }

  channel::channel(const bool copyData, genericMatrix<float>& other,
                   const int fromRow,
                   const int fromCol,
                   const int toRow,
                   const int toCol)
    : matrix<float>(copyData,other,fromRow,fromCol,toRow,toCol) {
  }

  const std::string& channel::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * create a clone of this channel
   * @return a pointer to a copy of this matrix
   */
  channel* channel::clone() const {
    return new channel(*this);
  }

  channel* channel::newInstance() const {
    return new channel();
  }

  /*
   * copy the <code>other</code> channel8 by casting each of its elements.
   *
   * The elements of the channel8 will be also multiplied by 1/255.
   *
   * @param other the channel8 to be casted
   * @return a reference to this channel
   * Example:
   * \code
   *   lti::channel8 matA(10,10,255); // a channel8
   *   lti::channel  matB;            // a channel
   *
   *   matB.castFrom(matA);         // this will copy matA in matB!!
   *                                // and all elements will have 1.0f
   * \endcode
   */
  channel& channel::castFrom(const channel8& other) {
    allocate(other.rows(),other.columns());
    iterator it;
    vector<channel8::value_type>::const_iterator cit,eit;
    int y;
    for (y=0,it=begin();y<other.rows();y++) {
      const vector<channel8::value_type>& vct=other.getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;cit++,it++) {
        (*it)=static_cast<float>(*cit)/255.0f;
      }
    }
    return (*this);
  }

  /*
   * cast from image
   */
  channel& channel::castFrom(const image& other) {
    allocate(other.rows(),other.columns());
    iterator it;
    vector<rgbaPixel>::const_iterator cit,eit;
    int y;
    for (y=0,it=begin();y<other.rows();y++) {
      const vector<rgbaPixel>& vct=other.getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;cit++,it++) {
        (*it)=(static_cast<float>((*cit).getRed())+
               static_cast<float>((*cit).getGreen())+
               static_cast<float>((*cit).getBlue()))/765.0f;
      }
    }

    return (*this);
  }

  /*
   * Apply a gray valued transformation which maps the given intervall to
   * [0.0,1.0] (default) or the explicitly given "destination" interval
   * @param minVal the lower limit of the original data interval
   * @param maxVal the higher limit of the original data interval
   * @param minDest the lower limit of the mapped interval (default 0.0f)
   * @param maxDest the higher limit of the mapped interval (default 1.0f)
   * @return a reference to this object
   */
  channel& channel::mapLinear(const float& minVal, const float& maxVal,
                              const float& minDest, const float& maxDest) {

    vector<channel::value_type>::iterator cit,eit;
    int y;

    float tm,tb;

    if (maxVal != minVal) {
      tm = (maxDest-minDest)/(maxVal-minVal);
    } else {
      tm = 1.0f;
    }

    tb = maxDest-maxVal*tm;

    const float m=tm;
    const float b=tb;

    if (b == 0.0f) {
      for (y=0;y<rows();++y) {
        vector<channel::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit)*=m;
        }
      }
    } else if (m == 1.0f) {
      for (y=0;y<rows();++y) {
        vector<channel::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit)+=b;
        }
      }
    } else {
      for (y=0;y<rows();++y) {
        vector<channel::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit)=(*cit)*m+b;
        }
      }
    }
    return (*this);
  }
}
