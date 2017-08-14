/*
 * Copyright (C) 1998-2005
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
 * \file   ltiChannelC.h
 *         Contains the data structure to represent complex valued channels
 *         with float as type for the real and imaginary components.
 * \author Pablo Alvarado
 * \date   13.10.2007
 *
 * $Id: ltiChannelC.h,v 1.1 2007-10-14 02:44:14 alvarado Exp $
 */

#ifndef _LTI_CHANNEL_C_H_
#define _LTI_CHANNEL_C_H_

#include "ltiMatrix.h"
#include "ltiChannel.h"
#include "ltiChannelC.h"
#include "ltiComplex.h"
#include "ltiTypes.h"

namespace lti {
  class channel8;
  class image;

  /**
   * Complex channel.
   *
   * This class is identical to a matrix of lti::fcomplex except for the
   * method castFrom(channel8).
   *
   * The typical value range is between 0.0f and 1.0f for both, real and
   * imaginary components (see lti::image for more information).
   *
   * @see lti::image, lti::channel8
   *
   * @ingroup gAggregate
   * @ingroup gImageProcessing
   */
  class channelC : public matrix< fcomplex > {
  public:

    /**
     * Default constructor creates an empty channel
     */
    channelC();

    /**
     * Create a connected \c rows x \c cols channel and leave the data
     * uninitialized.
     *
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     */
    channelC(const int rows,const int cols);

    /**
     * Create a connected \c size.y x \c size.x channel and initializes
     * all elements with \a iniValue
     *
     * @param size lti::ipoint with the size of the channel
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     */
    channelC(const ipoint& size);

    /**
     * Create a connected \c rows x \c cols channel
     * and initializes all elements with \a iniValue
     *
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     * @param iniValue all elements will be initialized with this value
     */
    channelC(const int rows,const int cols,const fcomplex& iniValue);

    /**
     * Create a connected \c size.y x \c size.x channel and initializes
     * all elements with \a iniValue
     *
     * @param size lti::ipoint with the size of the channel
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channelC(const ipoint& size,const fcomplex& iniValue);

    /**
     * Create a connected \c rows x \c cols channel and initializes all
     * elements with the data ipointed by \a data.  The first
     * \a cols-elements of the data will be copied on the first row,
     * the next ones on the second row and so on.
     *
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     * @param data pointer to the memory block with the data to be initialized
     * with.
     */
    channelC(const int rows,const int cols,const fcomplex data[]);

    /**
     * Copy constructor.
     *
     * Create a window from another channelC.
     *
     * @param other   the channel to be copied.
     * @param fromRow initial row of the other channel to be copied
     * @param fromCol initial column of the other channel to be copied
     * @param toRow   last row to be copied of the other channel
     * @param toCol   last column to be copied of the other channel
     *
     * Example:
     * \code
     * lti::channelC m(4,6,0); // channel with 24 elements
     * // ...
     * // initialize channel with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channelC sm(m,1,3,0,2)  // this line will lead to the
     * //                             following contents for sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channelC(const channelC& other,
             const int fromRow,
             const int fromCol=0,
             const int toRow=MaxIndex,
             const int toCol=MaxIndex);

    /**
     * Copy constructor.
     */
    channelC(const channelC& other);

    /**
     * Copy constructor.
     *
     * Create a window from another channel.
     *
     * @param other   the channel to be copied.
     * @param from    initial coordinates of the window.
     * @param to      final coordinates of the window.
     */
    channelC(const channelC& other,
             const ipoint& from,
             const ipoint& to);


    /**
     * Copy constructor (reference to a subchannel).
     *
     * Create subchannel of another channel.
     *
     * if \a copyData == \c true, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if \a copyData == \c false, the new object has references to
     * the other channel, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined channel)
     *
     * Those algorithms which use direct access to the channel memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other channel be copied or not
     * @param other the channel with the original data
     * @param fromRow initial row of the other channel to be copied
     * @param fromCol initial column of the other channel to be copied
     * @param toRow   last row to be copied of the other channel
     * @param toCol   last column to be copied of the other channel
     */
    channelC(const bool copyData, 
             channelC& other,
             const int fromRow=0,
             const int fromCol=0,
             const int toRow=MaxIndex,
             const int toCol=MaxIndex);
    
    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this channel
     * @return a pointer to a copy of this matrix
     */
    virtual channelC* clone() const;

    /**
     * Create a new empty channel
     * @return a pointer to new channel
     */
    virtual channelC* newInstance() const;

    /**
     * Copy the \a other channel8 by casting each of its elements.
     *
     * The elements of the channel8 will be also multiplied by 1/255.
     *
     * @param other the channel8 to be casted
     * @return a reference to this channel
     *
     * Example:
     * \code
     *   lti::channel8 matA(10,10,255); // a channel8
     *   lti::channel  matB;            // a channel
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have 1.0f
     * \endcode
     */
    channelC& castFrom(const channel8& other);

    /**
     * Cast the image to an channelC.
     * It extracts the intensity channel of the image, defined as
     * (R+G+B)/3, where R, G, and B are the red, green and blue components
     * of the pixel.
     *
     * The elements of the resulting channel will be between 0.0f (black) and
     * 1.0f (white) for the real part and zero as imaginary part.
     *
     * @param other the image to be casted
     * @return a reference to this channel
     */
    channelC& castFrom(const image& other);

    /**
     * Copy the \a other matrix by casting each of its elements.
     *
     * @param other The matrix to be casted
     * @return a reference to this channel
     */
    template<typename U>
    inline channelC& castFrom(const matrix<U>& other);

    /**
     * Extract the real part of each pixel as a channel
     */
    void getReal(channel& real) const;

    /**
     * Extract the imaginary part of each pixel as a channel
     */
    void getImag(channel& imag) const;

    /**
     * Extract the squared magnitude of each pixel.
     *
     * This method is faster than getAbs() as it avoids taking the square root
     * of each pixel.
     */
    void getNorm(channel& smag) const;

    /**
     * Extract the magnitude of each pixel.
     *
     * This method is slower than getSqrMagnitude() as it has to apply the
     * square root to each pixel.
     */
    void getAbs(channel& mag) const;

    /**
     * Extract the argument of each pixel (its angle).
     *
     * This method is slow as it needs to apply the arctan to each pixel.
     * 
     * For time critical applications, use the getArgumentFast, which makes use
     * of the lti::arctanLUT, but is not as precise as this one.
     */
    void getArg(channel& arg) const;

    /**
     * Extract the argument of each pixel (its angle) in a fast way.
     *
     * This method requires that both the real and imaginary parts lie within
     * the interval [0,1].
     *
     * This method makes use of the lti::arctanLUT to accelerate the
     * computation of the angle.  If you need very precise angular values, use
     * the getArg() method instead.
     */
    void getArgFast(channel& arg) const;

    /**
     * Set the real and imaginary parts from two different channels.
     *
     * Both channels must have the same size.  If they are not, the method
     * returns \c false.
     */
    bool set(const channel& real,const channel& imag);

    /**
     * Get the real and imaginary parts as two different channels.
     */
    void get(channel& real,channel& imag) const;

    /**
     * Apply a gray valued transformation which maps the given interval to
     * [0.0,1.0] (default) or the explicitly given "destination" interval
     *
     * @param minVal the lower limit of the original data interval
     * @param maxVal the higher limit of the original data interval
     * @param minDest the lower limit of the mapped interval (default 0.0f)
     * @param maxDest the higher limit of the mapped interval (default 1.0f)
     * @return a reference to this object
     *
     * A linear mapping is applied, where now the slope and offset are complex
     * values computed by and analytical extension of the real counterpart.
     *
     * For example, if you want to map the interval [-1.0f,2.0f] to the "usual"
     * interval [0.0,1.0] just use one of following methods:
     *
     * \code
     * lti::channel chnl;
     * // ...
     * chnl.mapLinear(-1.0f,2.0f,0.0,1.0); // map [-1,2] to  [0,1]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(-1.0f,2.0f);
     * \endcode
     *
     * Not that you can use this method to "invert" your gray values with
     * \code
     * chnl.mapLinear(0.0f,1.0f,1,0f,0.0f); // map [0,1] to  [1,0]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(1.0f,0.0f);
     * \endcode
     *
     */
    channelC& mapLinear(const fcomplex& minVal, 
                        const fcomplex& maxVal,
                        const fcomplex& minDest=0.0f,
                        const fcomplex& maxDest=1.0f);
    

    /**
     * Apply a gray valued transformation which maps the given
     * intervall of the other channel into [0.0,1.0] (default) or the
     * explicitly given "destination" interval in this channel.
     *
     * @param other the other channel which values are to be mapped into
     *              the new interval
     * @param minVal the lower limit of the original data interval
     * @param maxVal the higher limit of the original data interval
     * @param minDest the lower limit of the mapped interval (default 0.0f)
     * @param maxDest the higher limit of the mapped interval (default 1.0f)
     * @return a reference to this object
     *
     * For example, if you want to map the interval [-1.0f,2.0f] to
     * the "usual" interval [0.0,1.0] just use one of following methods:
     *
     * \code
     * lti::channel chnl;
     * // ...
     * chnl.mapLinear(-1.0f,2.0f,0.0,1.0); // map [-1,2] to  [0,1]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(-1.0f,2.0f);
     * \endcode
     *
     * Not that you can use this method to "invert" your gray values with
     * \code
     * chnl.mapLinear(0.0f,1.0f,1,0f,0.0f); // map [0,1] to  [1,0]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(1.0f,0.0f);
     * \endcode
     * 
     */
    template<typename U>
    channelC& mapLinear(const matrix<U>& other,
                        const U& minVal,
                        const U& maxVal,
                        const fcomplex& minDest=0.0f, 
                        const fcomplex& maxDest=1.0f);

  };

  template<class U>
  channelC& channelC::castFrom(const matrix<U>& other) {
    matrix<value_type>::castFrom(other);
    return *this;
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
  template<class U>
  channelC& channelC::mapLinear(const matrix<U>& other,
                                const U& minVal, const U& maxVal,
                                const fcomplex& minDest,
                                const fcomplex& maxDest) {

    allocate(other.size());

    iterator it;
    typename vector<U>::const_iterator cit,eit;
    int y;

    fcomplex tm,tb;

    if (maxVal != minVal) {
      tm = (maxDest-minDest)/static_cast< fcomplex >(maxVal-minVal);
    } else {
      tm = 1.0f;
    }

    tb = maxDest-maxVal*tm;

    const fcomplex m=tm;
    const fcomplex b=tb;

    // check possible speed improvements (unnecessary computations avoided)

    if (b == 0.0f) {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<U>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast< fcomplex >((*cit)*m);
        }
      }
    } else if (m == 1.0f) {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<U>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast< fcomplex >((*cit)+b);
        }
      }
    } else {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<U>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast< fcomplex >((*cit)*m+b);
        }
      }
    }
    return (*this);
  }

  

}

#endif

