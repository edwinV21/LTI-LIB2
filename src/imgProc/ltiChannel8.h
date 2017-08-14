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
 * \file   ltiChannel8.h
 *         Contains the data structure to represent gray valued images
 *         with one byte per pixel.
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiChannel8.h,v 1.8 2010-04-25 05:26:54 alvarado Exp $
 */

#ifndef _LTI_CHANNEL8_H_
#define _LTI_CHANNEL8_H_

#include "ltiMatrix.h"
#include "ltiTypes.h"

namespace lti {

  class channel;
  class image;

  /**
   * A format for 8-bit channels.
   *
   * This class is identical to a matrix of bytes except for the method
   * castFrom(channel)
   *
   * The typical value range is between 0 and 255 (see lti::image for more
   * information).
   *
   * @see lti::image, lti::channel
   *
   * @ingroup gAggregate
   * @ingroup gImageProcessing
   */
  class channel8 : public matrix<ubyte> {
  public:
    /**
     * Default constructor creates an empty channel8
     */
    channel8();

    /**
     * Create a connected \c rows x \c cols channel8 and leave all
     * elements uninitialized.
     *
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     */
    channel8(const int rows,const int cols);

    /**
     * Create a connected \c size.y x \c size.x
     * channel8 and leave all elements uninitialized.
     *
     * @param size lti::point with the size of the channel8
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     */
    channel8(const ipoint& size);

    /**
     * Create a connected \c rows x \c cols channel8 and initializes
     * all elements with \a iniValue.
     *
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     * @param iniValue all elements will be initialized with this value
     */
    channel8(const int rows,const int cols,const ubyte& iniValue);

    /**
     * Create a connected \c size.y x \c size.x
     * Channel8 and initializes all elements with \a iniValue
     * @param size lti::point with the size of the channel8
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channel8(const ipoint& size,const ubyte& iniValue);

    /**
     * Create a connected \c rows x \c cols Channel8 and initializes
     * all elements with the data pointed by \a data.  The first
     * \a cols-elements of the data will be copied on the first row,
     * the next ones on the second row and so on.
     *
     * @param rows number of rows of the channel8
     * @param cols number of columns of the channel8
     * @param data pointer to the memory block with the data to be initialized
     *             with.
     */
    channel8(const int rows,const int cols,const ubyte data[]);

    /**
     * Copy constructor.
     */
    channel8(const genericMatrix<ubyte>& other);     

    /**
     * Copy constructor.
     *
     * Create this channel8 as a connected copy of a submatrix of another
     * channel8.
     */
    channel8(const genericMatrix<ubyte>& other,
             const ipoint& from,
             const ipoint& to);

    /**
     * Copy constructor.
     *
     * Create this channel8 as a connected copy of another channel8
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subchannel of another
     * channel.
     *
     * @param other the channel8 to be copied.
     * @param fromRow initial row of the other channel8 to be copied
     * @param fromCol initial column of the other channel8 to be copied
     * @param toRow   last row to be copied of the other channel8
     * @param toCol   last column to be copied of the other channel8
     *
     * Example:
     * \code
     * lti::channel8 m(4,6,0); // channel8 with 24 elements
     * // ...
     * // initialize channel8 with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channel8 sm(m,1,3,0,2)  // last line will leat to
     * //                              following contents in sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channel8(const genericMatrix<ubyte>& other,
             const int fromRow,
             const int fromCol=0,
             const int toRow=MaxIndex,
             const int toCol=MaxIndex) _LTI_DEPRECATED;

    /**
     * Copy constructor (reference to a subchannel8).
     *
     * Creates subchannel8 of another channel8.
     *
     * if \a copyData == \c true, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if \a copyData == \c false, the new object has references to
     * the other channel8, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined channel8)
     *
     * Those algorithms which use direct access to the channel8 memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other channel8 be copied or not
     * @param other   the channel with the original data
     * @param fromRow initial row of the other channel8 to be copied
     * @param fromCol initial column of the other channel8 to be copied
     * @param toRow   last row to be copied of the other channel8
     * @param toCol   last column to be copied of the other channel8
     */
    channel8(const bool copyData, genericMatrix<ubyte>& other,
             const int fromRow=0,
             const int fromCol=0,
             const int toRow=MaxIndex,
             const int toCol=MaxIndex);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this channel8
     * @return a pointer to a copy of this matrix
     */
    virtual channel8* clone() const;

    /**
     * Create a new empty channel8
     * @return a pointer to a copy of this matrix
     */
    virtual channel8* newInstance() const;

    /**
     * Compute the sum of all elements in the channel
     */
    int computeSumOfElements() const;

    /**
     * Copy the \a other channel by casting each of its elements.
     *
     * The elements of the channel will be multiplied by 255 if no
     * other %parameter but the channel is given..
     *
     * @param other the channel to be cast from
     *
     * @param minToBlack if minToBlack is true, a linear gray-valued
     * tranformation will be applied, which maps the minimal value in
     * the channel to zero.  If false, the value zero will be mapped
     * to zero.
     *
     * @param maxToWhite if maxToWhite is true, a linear gray-valued
     * transformation will be applied, which maps the maximal value in
     * the channel to 255.  If false, the value 1.0f will be mapped to
     * 255.
     *
     * @return a reference to this channel
     * Example:
     * \code
     *   lti::channel matA(10,10,1); // a channel
     *   lti::channel8  matB;          // a channel8
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have 255
     * \endcode */
    channel8& castFrom(const channel& other,
                       const bool minToBlack = false,
                       const bool maxToWhite = false);

    /**
     * Cast the image to an channel8.
     *
     * It extracts the intensity channel of the image, defined as
     * (R+G+B)/3, where R, G, and B are the red, green and blue components
     * of the pixel.
     *
     * The elements of the resulting channel will be between 0 (black) and
     * 255 (white).
     *
     * @param other the image to be cast
     * @return a reference to this channel
     */
    channel8& castFrom(const image& other);

    /**
     * Copy the \a other matrix by casting each of its elements
     *
     * @param other The matrix to be cast
     * @return a reference to this channel
     */
    template<class U>
    inline channel8& castFrom(const matrix<U>& other);

    /**
     * Apply a gray valued transformation which maps the given intervall to
     * [0,255] (default) or the explicitly given "destination" interval
     *
     * @param minVal the lower limit of the original data interval
     * @param maxVal the higher limit of the original data interval
     * @param minDest the lower limit of the mapped interval (default 0)
     * @param maxDest the higher limit of the mapped interval (default 255)
     * @return a reference to this object
     *
     * For example, if you want to map the interval [20,200] to
     * the "usual" interval [0,255] just use one of following methods:
     *
     * \code
     * lti::channel8 chnl;
     * // ...
     * chnl.mapLinear(20,200,0,255); // map [20,200] to  [0,255]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(20,200);
     * \endcode
     *
     * Not that you can use this method to "invert" your gray values with
     * \code
     * chnl.mapLinear(0,255,255,0); // map [0,255] to  [255,0]
     * // this is equivalent to (due to default "destination" interval)
     * chnl.mapLinear(255,0);
     * \endcode
     *
     */
    channel8& mapLinear(const ubyte& minVal, const ubyte& maxVal,
                        const ubyte& minDest=0, const ubyte& maxDest=255);
  };
  
  template<class U>
  inline channel8& channel8::castFrom(const matrix<U>& other) {
    matrix<value_type>::castFrom(other);
    return *this;
  }

}


#endif

