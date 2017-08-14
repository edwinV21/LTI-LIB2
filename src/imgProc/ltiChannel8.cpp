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
 * $Id: ltiChannel8.cpp,v 1.9 2010-04-25 05:26:54 alvarado Exp $
 */

#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

namespace lti {

  // --------------------------------------------------------------------------
  //                    CHANNEL8
  // --------------------------------------------------------------------------

  channel8::channel8() : matrix<ubyte>() {
  }

  channel8::channel8(const int r,const int c)
    : matrix<ubyte>(r,c) {
  }

  channel8::channel8(const ipoint& sz)
    : matrix<ubyte>(sz) {
  }

  channel8::channel8(const int r,const int c,const ubyte& iniValue)
    : matrix<ubyte>(r,c,iniValue) {
  }

  channel8::channel8(const ipoint& sz,const ubyte& iniValue)
    : matrix<ubyte>(sz,iniValue) {
  }

  channel8::channel8(const int r,const int c,const ubyte _data[])
    : matrix<ubyte>(r,c,_data) {
  }

  channel8::channel8(const genericMatrix<ubyte>& other)
    : matrix<ubyte>(other) {
  }

  channel8::channel8(const genericMatrix<ubyte>& other,
                     const ipoint& from,
                     const ipoint& to)
    : matrix<ubyte>(other,from,to) {
  }

  channel8::channel8(const genericMatrix<ubyte>& other,
                     const int fromRow,
                     const int fromCol,
                     const int toRow,
                     const int toCol)
    : matrix<ubyte>(other,fromRow,fromCol,toRow,toCol) {
  }

  

  channel8::channel8(const bool copyData, genericMatrix<ubyte>& other,
                     const int fromRow,
                     const int fromCol,
                     const int toRow,
                     const int toCol)
    : matrix<ubyte>(copyData,other,fromRow,fromCol,toRow,toCol) {
  }

  const std::string& channel8::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * create a clone of this channel
   * @return a pointer to a copy of this matrix
   */
  channel8* channel8::clone() const {
    return new channel8(*this);
  }

  channel8* channel8::newInstance() const {
    return new channel8();
  }

  /*
   * sum of elements
   */
  int channel8::computeSumOfElements() const {
    int y;
    int acc = 0;
    vector<channel8::value_type>::const_iterator cit,eit;
    for (y=0;y<rows();y++) {
      const vector<value_type>& vct = getRow(y);
      for (cit = vct.begin(),eit=vct.end();
           cit!=eit;
           ++cit) {
        acc+=static_cast<int>(*cit);
      }
    }
    return acc;
  }

  /*
   * copy the <code>other</code> channel by casting each of its elements.
   *
   * The elements of the channel will be also multiplied by 255.
   *
   * @param other the channel8 to be casted
   * @return a reference to this channel
   * Example:
   * \code
   *   lti::channel matA(10,10,1); // a channel
   *   lti::channel8  matB;          // a channel8
   *
   *   matB.castFrom(matA);         // this will copy matA in matB!!
   *                                // and all elements will have 255
   * \endcode
   */
  channel8& channel8::castFrom(const channel& other,
                               const bool minToBlack,
                               const bool maxToWhite) {

    allocate(other.rows(),other.columns());

    iterator it;
    vector<channel::value_type>::const_iterator cit,eit;
    int y;

    if (!minToBlack && !maxToWhite) {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<channel::value_type>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast<ubyte>((*cit)*255.0f);
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
          (*it)=static_cast<ubyte>((*cit)*m+b);
        }
      }

    }

    return (*this);
  }

  /*
   * cast from image
   */
  channel8& channel8::castFrom(const image& other) {
    allocate(other.rows(),other.columns());
    iterator it;
    vector<rgbaPixel>::const_iterator cit,eit;
    int y;

    for (y=0,it=begin();y<other.rows();y++) {
      const vector<rgbaPixel>& vct=other.getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;cit++,it++) {
        (*it)=static_cast<ubyte>((static_cast<int>((*cit).getRed())+
                                  static_cast<int>((*cit).getGreen())+
                                  static_cast<int>((*cit).getBlue()))/3);
      }
    }

    return (*this);
  }

  channel8& channel8::mapLinear(const ubyte& minVal, const ubyte& maxVal,
                                const ubyte& minDest, const ubyte& maxDest) {

    vector<channel8::value_type>::iterator cit,eit;
    int y;

    float tm,tb;

    if (maxVal != minVal) {
      tm = static_cast<float>(maxDest-minDest)
        /static_cast<float>(maxVal-minVal);
    } else {
      tm = 1.0f;
    }

    tb = static_cast<float>(maxDest)-maxVal*tm;

    const float m=tm;
    const float b=tb;
    float tmp;

    if (b == 0.0f) {
      for (y=0;y<rows();++y) {
        vector<channel8::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit) = m<0.f ? 0 : 
            ((tmp=*cit*m)>255.5f ? 255 : static_cast<ubyte>(tmp+0.5f));
        }
      }
    } else if (m == 1.0f) {
      for (y=0;y<rows();++y) {
        vector<channel8::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit) = ((tmp=*cit+b)<-0.5f ? 0 : 
            (tmp > 255.5f ? 255 : static_cast<ubyte>(tmp+0.5f)));
        }
      }
    } else {
      for (y=0;y<rows();++y) {
        vector<channel8::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit) = ((tmp=*cit*m+b)<-0.5f ? 0 : 
            (tmp > 255.5f ? 255 : static_cast<ubyte>(tmp+0.5f)));
        }
      }
    }
    return (*this);
  }

}

