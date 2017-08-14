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
 * $Id: ltiChannel32.cpp,v 1.8 2010-04-25 05:26:54 alvarado Exp $
 */

#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel32.h"

namespace lti {

  // --------------------------------------------------------------------------
  //                    CHANNEL32
  // --------------------------------------------------------------------------

  channel32::channel32() : matrix<int32>() {
  }

  channel32::channel32(const int r,const int c)
    : matrix<int32>(r,c) {
  }

  channel32::channel32(const ipoint& sz)
    : matrix<int32>(sz) {
  }

  channel32::channel32(const int r,const int c,const int32& iniValue)
    : matrix<int32>(r,c,iniValue) {
  }

  channel32::channel32(const ipoint& sz,const int32& iniValue)
    : matrix<int32>(sz,iniValue) {
  }

  channel32::channel32(const int r,const int c,const int32 _data[])
    : matrix<int32>(r,c,_data) {
  }

  channel32::channel32(const genericMatrix<int32>& other)
    : matrix<int32>(other) {
  }

  channel32::channel32(const genericMatrix<int32>& other,
                       const ipoint& from,
                       const ipoint& to) 
    : matrix<int32>(other,from,to) {
  }

  channel32::channel32(const genericMatrix<int32>& other,
                       const int fromRow,
                       const int fromCol,
                       const int toRow,
                       const int toCol)
    : matrix<int32>(other,fromRow,fromCol,toRow,toCol) {
  }

  channel32::channel32(const bool copyData, 
                       genericMatrix<int>& other,
                       const int fromRow,
                       const int fromCol,
                       const int toRow,
                       const int toCol)
    : matrix<int32>(copyData,other,fromRow,fromCol,toRow,toCol) {
  }

  const std::string& channel32::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * create a clone of this channel
   * @return a pointer to a copy of this matrix
   */
  channel32* channel32::clone() const {
    return new channel32(*this);
  }

  channel32* channel32::newInstance() const {
    return new channel32();
  }

  /*
   * sum of elements
   */
  int channel32::computeSumOfElements() const {
    int y;
    int acc = 0;
    vector<channel32::value_type>::const_iterator cit,eit;
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
   * @param other the channel32 to be casted
   * @return a reference to this channel
   * Example:
   * \code
   *   lti::channel matA(10,10,1); // a channel
   *   lti::channel32  matB;          // a channel32
   *
   *   matB.castFrom(matA);         // this will copy matA in matB!!
   *                                // and all elements will have 255
   * \endcode
   */
  channel32& channel32::castFrom(const channel& other,
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
          (*it)=static_cast<int32>((*cit)*255.0f);
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
          (*it)=static_cast<int32>((*cit)*m+b);
        }
      }

    }

    return (*this);
  }

}

