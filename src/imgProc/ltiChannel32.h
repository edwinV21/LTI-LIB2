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
 * \file   ltiChannel32.h
 *         Contains the data structure to represent gray valued images
 *         with four bytes per pixel.
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiChannel32.h,v 1.8 2010-04-25 05:26:54 alvarado Exp $
 */

#ifndef _LTI_CHANNEL32_H_
#define _LTI_CHANNEL32_H_

#include "ltiMatrix.h"
#include "ltiTypes.h"

namespace lti {

  class channel;
  class image;

  /**
   * A format for 32-bit channels.
   *
   * This class is identical to a matrix of int32 except for the method
   * castFrom(channel)
   *
   * The value range is between -(2^31) and (2^31-1) (see
   * lti::image for more information).
   *
   * @see lti::image, lti::channel, lti::channel8
   *
   * @ingroup gAggregate
   * @ingroup gImageProcessing
   */
  class channel32 : public matrix<int32> {
  public:
    /**
     * Default constructor creates an empty channel32
     */
    channel32();

    /**
     * Create a connected \c rows x \c cols channel32 and leave all
     * elements uninitialized.
     *
     * @param rows number of rows of the channel32
     * @param cols number of columns of the channel32
     */
    channel32(const int rows,const int cols);

    /**
     * Create a connected \c size.y x \c size.x
     * channel32 and leave all elements uninitialized.
     *
     * @param size lti::point with the size of the channel32
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     */
    channel32(const ipoint& size);

    /**
     * Create a connected \c rows x \c cols channel32 and initializes
     * all elements with \a iniValue.
     *
     * @param rows number of rows of the channel32
     * @param cols number of columns of the channel32
     * @param iniValue all elements will be initialized with this value
     */
    channel32(const int rows,const int cols,const int32& iniValue);

    /**
     * Create a connected \c size.y x \c size.x
     * Channel32 and initializes all elements with \a iniValue
     * @param size lti::point with the size of the channel32
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    channel32(const ipoint& size,const int32& iniValue);

    /**
     * Create a connected \c rows x \c cols Channel32 and initializes
     * all elements with the data pointed by \a data.  The first
     * \a cols-elements of the data will be copied on the first row,
     * the next ones on the second row and so on.
     *
     * @param rows number of rows of the channel32
     * @param cols number of columns of the channel32
     * @param data pointer to the memory block with the data to be initialized
     *             with.
     */
    channel32(const int rows,const int cols,const int32 data[]);


    /**
     * Copy constructor.
     *
     * Create this channel32 as a connected copy of another channel32
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subchannel of another
     * channel.
     *
     * @param other the channel32 to be copied.
     * @param fromRow initial row of the other channel32 to be copied
     * @param fromCol initial column of the other channel32 to be copied
     * @param toRow   last row to be copied of the other channel32
     * @param toCol   last column to be copied of the other channel32
     *
     * Example:
     * \code
     * lti::channel32 m(4,6,0); // channel32 with 24 elements
     * // ...
     * // initialize channel32 with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::channel32 sm(m,1,3,0,2)  // last line will leat to
     * //                              following contents in sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     *
     */
    channel32(const genericMatrix<int32>& other,
              const int fromRow,
              const int fromCol=0,
              const int toRow=MaxIndex,
              const int toCol=MaxIndex);


    /**
     * Copy constructor.
     *
     * @param other the channel32 to be copied.
     */
    channel32(const genericMatrix<int32>& other);

    /**
     * Copy constructor.
     *
     * Copy a submatrix of another channel32
     *
     * @param other the channel32 to be copied.
     * @param from initial point of the window to be copied
     * @param to   final point (included) of the window to be copied.
     */
    channel32(const genericMatrix<int32>& other,
              const ipoint& from,
              const ipoint& to);


    /**
     * Copy constructor (reference to a subchannel32).
     *
     * Creates subchannel32 of another channel32.
     *
     * if \a copyData == \c true, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if \a copyData == \c false, the new object has references to
     * the other channel32, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined channel32)
     *
     * Those algorithms which use direct access to the channel32 memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other channel32 be copied or not
     * @param other   the channel with the original data
     * @param fromRow initial row of the other channel32 to be copied
     * @param fromCol initial column of the other channel32 to be copied
     * @param toRow   last row to be copied of the other channel32
     * @param toCol   last column to be copied of the other channel32
     */
    channel32(const bool copyData, genericMatrix<int32>& other,
              const int fromRow=0,
              const int fromCol=0,
              const int toRow=MaxIndex,
              const int toCol=MaxIndex);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this channel32
     * @return a pointer to a copy of this matrix
     */
    virtual channel32* clone() const;

    /**
     * Create a new empty channel32
     * @return a pointer to a copy of this matrix
     */
    virtual channel32* newInstance() const;

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
     *   lti::channel32  matB;       // a channel32
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have 255
     * \endcode */
    channel32& castFrom(const channel& other,
                        const bool minToBlack = false,
                        const bool maxToWhite = false);

    /**
     * Copy the \a other matrix by casting each of its elements
     *
     * @param other The matrix to be cast
     * @return a reference to this channel
     */
    template<class U>
    inline channel32& castFrom(const matrix<U>& other);
  };
  
  template<class U>
  inline channel32& channel32::castFrom(const matrix<U>& other) {
    matrix<value_type>::castFrom(other);
    return *this;
  }

}


#endif

