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
 * \file   ltiChannelC.cpp
 * \author Pablo Alvarado
 * \date   10.07.2000
 *
 * $Id: ltiChannelC.cpp,v 1.2 2007-11-03 23:43:25 alvarado Exp $
 */

#include "ltiComplex.h"
#include "ltiChannelC.h"
#include "ltiChannel8.h"
#include "ltiArctanLUT.h"
#include "ltiImage.h"

namespace lti {

  // --------------------------------------------------------------------------
  //                    CHANNELC
  // --------------------------------------------------------------------------


  /*
   * default constructor creates an empty channel
   */
  channelC::channelC()
    : matrix< fcomplex >() {
  }

  channelC::channelC(const int r,const int c)
    : matrix< fcomplex >(r,c) {
  }

  channelC::channelC(const ipoint& sz)
    : matrix< fcomplex >(sz) {
  }
  
  channelC::channelC(const int r,const int c,const fcomplex& iniValue)
    : matrix< fcomplex >(r,c,iniValue) {
  }

  channelC::channelC(const ipoint& sz,const fcomplex& iniValue)
    : matrix< fcomplex >(sz,iniValue) {
  }
  
  channelC::channelC(const int r,const int c,const fcomplex _data[])
    : matrix< fcomplex >(r,c,_data) {
  }
  
  channelC::channelC(const channelC& other,
                     const int fromRow,
                     const int fromCol,
                     const int toRow,
                     const int toCol)
    : matrix< fcomplex >(other,fromRow,fromCol,toRow,toCol) {
  }

  channelC::channelC(const channelC& other)
    : matrix< fcomplex >(other) {
  }

  channelC::channelC(const channelC& other,
                     const ipoint& from,
                     const ipoint& to)
    : matrix< fcomplex >(other,from,to) {
  }

  channelC::channelC(const bool copyData, 
                     channelC& other,
                     const int fromRow,
                     const int fromCol,
                     const int toRow,
                     const int toCol)
    : matrix< fcomplex >(copyData,other,fromRow,fromCol,toRow,toCol) {
  }

  const std::string& channelC::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * create a clone of this channelC
   * @return a pointer to a copy of this matrix
   */
  channelC* channelC::clone() const {
    return new channelC(*this);
  }

  channelC* channelC::newInstance() const {
    return new channelC();
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
  channelC& channelC::castFrom(const channel8& other) {
    allocate(other.rows(),other.columns());
    iterator it;
    vector<channel8::value_type>::const_iterator cit,eit;
    int y;
    for (y=0,it=begin();y<other.rows();y++) {
      const vector<channel8::value_type>& vct=other.getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it).set((*cit)/255.0f,0.0f);
      }
    }
    return (*this);
  }

  /*
   * cast from image
   */
  channelC& channelC::castFrom(const image& other) {
    allocate(other.rows(),other.columns());
    iterator it;
    vector<rgbaPixel>::const_iterator cit,eit;
    int y;
    for (y=0,it=begin();y<other.rows();y++) {
      const vector<rgbaPixel>& vct=other.getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it)=fcomplex((static_cast<float>((*cit).getRed())+
                        static_cast<float>((*cit).getGreen())+
                        static_cast<float>((*cit).getBlue()))/765.0f,
                       0.0f);
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
  channelC& channelC::mapLinear(const fcomplex& minVal,
                                const fcomplex& maxVal,
                                const fcomplex& minDest,
                                const fcomplex& maxDest) {

    vector<fcomplex>::iterator cit,eit;
    int y;

    fcomplex tm,tb;

    if (maxVal != minVal) {
      tm = (maxDest-minDest)/(maxVal-minVal);
    } else {
      tm = 1.0f;
    }

    tb = maxDest-maxVal*tm;

    const fcomplex m=tm;
    const fcomplex b=tb;

    if (b == 0.0f) {
      for (y=0;y<rows();++y) {
        vector<channelC::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit)*=m;
        }
      }
    } else if (m == 1.0f) {
      for (y=0;y<rows();++y) {
        vector<channelC::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit)+=b;
        }
      }
    } else {
      for (y=0;y<rows();++y) {
        vector<channelC::value_type>& vct=getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit) {
          (*cit)=(*cit)*m+b;
        }
      }
    }
    return (*this);
  }

  // the channelC to components methods

  /*
   * Extract the real part of each pixel as a channel
   */
  void channelC::getReal(channel& real) const {
    real.allocate(size());

    channel::iterator it = real.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it) = (*cit).real();
      }
    }
  }

  /*
   * Extract the imaginary part of each pixel as a channel
   */
  void channelC::getImag(channel& imag) const {
    imag.allocate(size());

    channel::iterator it = imag.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it) = (*cit).imag();
      }
    }
  }

  /*
   * Extract the squared magnitude of each pixel.
   *
   * This method is faster than getMagnitude() as it avoids taking the square
   * root of each pixel.
   */
  void channelC::getNorm(channel& smag) const {
    smag.allocate(size());

    channel::iterator it = smag.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it) = norm(*cit);
      }
    }
  }

  /*
   * Extract the magnitude of each pixel.
   *
   * This method is slower than getSqrMagnitude() as it has to apply the
   * square root to each pixel.
   */
  void channelC::getAbs(channel& mag) const {
    mag.allocate(size());
    
    channel::iterator it = mag.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it) = abs(*cit);
      }
    }
  }

  /*
   * Extract the argument of each pixel (its angle).
   *
   * This method is slow as it needs to apply the arctan to each pixel.
   * 
   * For time critical applications, use the getArgumentFast, which makes use
   * of the lti::arctanLUT, but is not as precise as this one.
   */
  void channelC::getArg(channel& arg) const {
    arg.allocate(size());
    
    channel::iterator it = arg.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it) = lti::arg(*cit);
      }
    }
  }
  
  /*
   * Extract the argument of each pixel (its angle).
   *
   * This method makes use of the lti::arctanLUT to accelerate the
   * computation of the angle.  If you need very precise angular values, use
   * the getArgument() method instead.
   */
  void channelC::getArgFast(channel& arg) const {
    
    arctanLUT lut;
    arg.allocate(size());

    channel::iterator it = arg.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
        (*it) = lut((*cit).imag(),(*cit).real());
      }
    }
  }

  /*
   * Set the real and imaginary parts from two different channels.
   *
   * Both channels must have the same size.
   */
  bool channelC::set(const channel& real,const channel& imag) {
    if (real.size() != imag.size()) {
      return false;
    }

    allocate(real.size());
    iterator it = begin();
    fvector::const_iterator rit,iit,erit;

    for (int y=0;y<rows();++y) {
      const fvector& rvct = real.getRow(y);
      const fvector& ivct = imag.getRow(y);
      for (rit=rvct.begin(),erit=rvct.end(),iit=ivct.begin();
           rit!=erit;
           ++it,++rit,++iit) {
        (*it).set(*rit,*iit);
      }
    }
    
    return true;
  }

  /*
   * Get the real and imaginary parts as two different channels.
   */
  void channelC::get(channel& real,channel& imag) const {
    real.allocate(size());
    imag.allocate(size());

    channel::iterator rit = real.begin();
    channel::iterator iit = imag.begin();
    vector<fcomplex>::const_iterator cit,eit;
    for (int y=0;y<rows();++y) {
      const vector<fcomplex>& vct = getRow(y);
      for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++iit,++rit) {
        (*cit).get(*rit,*iit);
      }
    }
  }
}
