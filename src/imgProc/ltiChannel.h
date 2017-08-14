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
 * \file   ltiChannel.h
 *         Contains the data structure to represent gray valued images
 *         with one float per pixel.
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiChannel.h,v 1.11 2010-04-25 05:26:54 alvarado Exp $
 */

#ifndef _LTI_CHANNEL_H_
#define _LTI_CHANNEL_H_

#include "ltiMatrix.h"
#include "ltiTypes.h"

namespace lti {
  class channel8;
  class image;

  /**
   * A format for float channels.
   *
   * This class is identical to a matrix of floats except for the method
   * castFrom(channel8).
   *
   * The typical value range is between 0.0f and 1.0f (see lti::image for more
   * information).
   *
   * @see lti::image, lti::channel8
   *
   * @ingroup gAggregate
   * @ingroup gImageProcessing
   */
  class channel : public matrix<float> {
  public:

    /**
     * Default constructor creates an empty channel
     */
    channel();

    /**
     * Create a connected \c rows x \c cols channel and leave the data
     * uninitialized.
     *
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     */
    channel(const int rows,const int cols);

    /**
     * Create a connected \c size.y x \c size.x channel and initializes
     * all elements with \a iniValue
     *
     * @param size lti::ipoint with the size of the channel
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     */
    channel(const ipoint& size);

    /**
     * Create a connected \c rows x \c cols channel
     * and initializes all elements with \a iniValue
     *
     * @param rows number of rows of the channel
     * @param cols number of columns of the channel
     * @param iniValue all elements will be initialized with this value
     */
    channel(const int rows,const int cols,const float& iniValue);

    /**
     * Create a connected \c size.y x \c size.x channel and initializes
     * all elements with \a iniValue
     *
     * @param size lti::ipoint with the size of the channel
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channel(const ipoint& size,const float& iniValue);

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
    channel(const int rows,const int cols,const float data[]);

    /**
     * Copy constructor.
     *
     * Create a window from another channel.
     *
     * @param other   the channel to be copied.
     * @param fromRow initial row of the other channel to be copied
     * @param fromCol initial column of the other channel to be copied
     * @param toRow   last row to be copied of the other channel
     * @param toCol   last column to be copied of the other channel
     *
     * Example:
     * \code
     * lti::channel m(4,6,0); // channel with 24 elements
     * // ...
     * // initialize channel with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channel sm(m,0,1,2,3);  // this line will lead to the
     * //                              following contents for sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channel(const genericMatrix<float>& other,
            const int fromRow,
            const int fromCol=0,
            const int toRow=MaxIndex,
            const int toCol=MaxIndex);

    /**
     * Copy constructor.
     */
    channel(const genericMatrix<float>& other);

    /**
     * Copy constructor.
     *
     * Create a window from another channel.
     *
     * @param other   the channel to be copied.
     * @param from    initial coordinates of the window.
     * @param to      final coordinates of the window.
     */
    channel(const genericMatrix<float>& other,
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
    channel(const bool copyData, 
            genericMatrix<float>& other,
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
    virtual channel* clone() const;

    /**
     * Create a new empty channel
     * @return a pointer to new channel
     */
    virtual channel* newInstance() const;

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
    channel& castFrom(const channel8& other);

    /**
     * Cast the image to an channel.
     * It extracts the intensity channel of the image, defined as
     * (R+G+B)/3, where R, G, and B are the red, green and blue components
     * of the pixel.
     *
     * The elements of the resulting channel will be between 0.0f (black) and
     * 1.0f (white).
     *
     * @param other the image to be casted
     * @return a reference to this channel
     */
    channel& castFrom(const image& other);

    /**
     * Copy the \a other matrix by casting each of its elements
     * @param other The matrix to be casted
     * @return a reference to this channel
     */
    template<typename U>
    inline channel& castFrom(const matrix<U>& other);

    /**
     * Apply a gray valued transformation which maps the given intervall to
     * [0.0,1.0] (default) or the explicitly given "destination" interval
     *
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
    channel& mapLinear(const float& minVal, const float& maxVal,
                       const float& minDest=0.0f, const float& maxDest=1.0f);


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
    channel& mapLinear(const matrix<U>& other,
                       const U& minVal, const U& maxVal,
                       const float& minDest=0.0f, const float& maxDest=1.0f);

  };

  template<class U>
  channel& channel::castFrom(const matrix<U>& other) {
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
  channel& channel::mapLinear(const matrix<U>& other,
                              const U& minVal, const U& maxVal,
                              const float& minDest,const float& maxDest) {

    allocate(other.size());

    iterator it;
    typename vector<U>::const_iterator cit,eit;
    int y;

    float tm,tb;

    if (maxVal != minVal) {
      tm = (maxDest-minDest)/static_cast<float>(maxVal-minVal);
    } else {
      tm = 1.0f;
    }

    tb = maxDest-maxVal*tm;

    const float m=tm;
    const float b=tb;

    // check possible speed improvements (unnecessary computations avoided)

    if (b == 0.0f) {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<U>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast<float>((*cit)*m);
        }
      }
    } else if (m == 1.0f) {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<U>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast<float>((*cit)+b);
        }
      }
    } else {
      for (y=0,it=begin();y<other.rows();++y) {
        const vector<U>& vct=other.getRow(y);
        for (cit=vct.begin(),eit=vct.end();cit!=eit;++cit,++it) {
          (*it)=static_cast<float>((*cit)*m+b);
        }
      }
    }
    return (*this);
  }

  

}

#endif

