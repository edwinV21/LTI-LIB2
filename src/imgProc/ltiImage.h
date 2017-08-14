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


/**
 * \file   ltiImage.h
 *         Contains the color image data structure of the LTI-Lib, which is
 *         a matrix of lti::rgbaPixel
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiImage.h,v 1.14 2013-09-10 14:37:25 alvarado Exp $
 */

#ifndef _LTI_IMAGE_H_
#define _LTI_IMAGE_H_

#include "ltiMatrix.h"
#include "ltiRGBAPixel.h"

namespace lti {
  class channel8;
  class channel;
  class channel32;

  /**
   * The one and only RGBA-image format.
   *
   * This class is an specialization of a matrix of lti::rgbaPixel.
   *
   * The concept for color images and gray valued images in the LTI-Lib is
   * simple:  they are specializations of the class lti::matrix.
   *
   * Several aspects must however be clarified.  The rows of the matrix will
   * represent horizontal lines in the image, and the columns vertical ones.
   * The row with index zero will be the row at the top of the image.  The
   * column with row zero is the one at the left of the image.  This means
   * that the used coordinate system for the position of a pixel is
   * "left-handed":  the origin is situated at the top-left corner of the
   * image, the x-coordinate gives the position in the horizontal axis and
   * the y-coordinate the position in the vertical axis.  In other words,
   * the y coordinate give the row and x the column of the matrix.
   * This fact is important to remember when accessing the image elements:
   *
   * \code
   * image img; // our image
   * if (img.at(y,x) == img[y][x] == img.at(point(x,y))) {
   *   cout << "This is always true!";
   * } else {
   *   cout << "ERROR: it's imposible to get here";
   *   exit 1;
   * }
   * \endcode
   *
   * The gray valued channels lti::channel and lti::channel8 differ on
   * the type and valid value ranges of their elements.  The former
   * accepts floating point values, with a default value range from
   * 0.0 to 1.0.  Many algorithms produce other values with specific
   * meanings like angles or gradients, but using the default range you can
   * assume 0.0 as a representation for black and 1.0 for white.
   *
   * The lti::channel8 is a much smaller representation but allows
   * only integer values between 0 and 255, fact that can be advantageous
   * in many algorithms.  Here 0 usually means black and 255 white.
   *
   * The lti::image as lti::matrix<lti::rgbaPixel> allows the representation of
   * true-color images, i.e. images with pixels that can be chosen from a
   * palette of more than 16 million colors.
   *
   * @see lti::matrix for a reference to all inherited methods.
   *
   * @ingroup gAggregate
   * @ingroup gColor
   * @ingroup gImageProcessing
   */
  class image : public matrix<rgbaPixel> {
  public:
    /**
     * Default constructor creates an empty image
     */
    image();

    /**
     * Create a connected \c rows x \c cols image but leave all
     * elements uninitialized
     *
     * @param rows number of rows of the image
     * @param cols number of columns of the image
     */
    image(const int rows,const int cols);

    /**
     * Create a connected \c size.y x \c size.x image but leave all
     * elements uninitialized.
     *
     * @param size lti::point with the size of the image
     *             (size.x is the number of columns and
     *              size.y the number of rows)
     */
    image(const ipoint& size);

    /**
     * Create a connected \c rows x \c cols image and initializes all
     * elements with \a iniValue 
     *
     * @param rows number of rows of the image
     * @param cols number of columns of the image
     * @param iniValue all elements will be initialized with this value
     */
    image(const int rows,const int cols,
          const rgbaPixel& iniValue);

    /**
     * Creates a connected \c size.y x \c size.x image and initializes
     * all elements with \a iniValue
     * @param size lti::point with the size of the image
     *            (size.x is the number of columns and
     *             size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    image(const ipoint& size,const rgbaPixel& iniValue);

    /**
     * Creates a connected \c rows x \c cols image and initializes all
     * elements with the data pointed by \a data.  The first
     * \a cols-elements of the data will be copied on the first row,
     * the next ones on the second row and so on.
     *
     * @param rows number of rows of the image
     * @param cols number of columns of the image
     * @param data pointer to the memory block with the data to be initialized
     *             with.
     */
    image(const int rows,const int cols,const rgbaPixel data[]);

    /**
     * Copy constructor.
     *
     * @param other   the image to be copied.
     */
    image(const image& other);

    /**
     * Copy constructor.
     *
     * Create a subimage of the other image
     *
     * @param other   the image to be copied.
     * @param from    initial coordinates of the window.
     * @param to      final coordinates of the window.
     */
    image(const image& other,
          const ipoint& from,
          const ipoint& to);

    /**
     * Copy constructor.
     *
     * Create this image as a connected copy of another image
     * for this const version, the data will be always copied!
     * It is also possible to create a copy of a subimage of another image.
     *
     * @param other   the image to be copied.
     * @param fromRow initial row of the other image to be copied
     * @param toRow   last row to be copied of the other image
     * @param fromCol initial column of the other image to be copied
     * @param toCol   last column to be copied of the other image
     *
     * Example:
     * \code
     * lti::image m(4,6,0); // image with 24 elements
     * // ...
     * // initialize image with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::image sm(m,1,0,3,2)  // this line will lead to the
     * //                             following contents for sm:
     * //        2  1  5
     * //        1  2  1
     * //        3  3  2
     * \endcode
     *
     */
    image(const image& other,
          const int fromRow,
          const int fromCol=0,
          const int toRow=MaxIndex,
          const int toCol=MaxIndex);

    /**
     * Copy constructor (reference to a subimage).
     *
     * creates subimage of another image.
     *
     * if \a copyData == \c true, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if \a copyData == \c false, the new object has references to
     * the other image, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined image)
     *
     * Those algorithms which use direct access to the image memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * @param copyData should the data of the other image be copied or not
     * @param other   the image with the original data
     * @param fromRow initial row of the other image to be copied
     * @param fromCol initial column of the other image to be copied
     * @param toRow   last row to be copied of the other image
     * @param toCol   last column to be copied of the other image
     */
    image(const bool copyData, image& other,
          const int fromRow=0,
          const int fromCol=0,
          const int toRow=MaxIndex,
          const int toCol=MaxIndex);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this image
     * @return a pointer to a copy of this image
     */
    virtual image* clone() const;

    /**
     * Create a new empty image
     * @return a pointer to the new image
     */
    virtual image* newInstance() const;

    /**
     * Cast from the \a other matrix<ubyte>, interpreted as a channel8.
     *
     * For the transformation it assumes the channel8 as a gray valued
     * channel where 0 means black and 255 means white.
     *
     * @param other the channel8 to be cast
     * @return a reference to this image
     *
     * Example:
     * \code
     *   lti::channel8 matA(10,10,255); // a channel8
     *   lti::image  matB;              // an image
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have
     *                                // rgbaPixel(255,255,255)
     * \endcode
     */
    image& castFrom(const matrix<ubyte>& other);

    /**
     * Cast from the \a other fmatrix, intepreted as a single precision
     * floating point channel.
     *
     * For the transformation it assumes the channel as a gray valued
     * channel where 0 means black and 1.0f means white.  All other
     * values will be clipped (less than zero to zero and more than 1.0 to 1.0)
     *
     * @param other the channel8 to be cast
     * @param minToBlack if minToBlack is true, a linear gray-valued
     * tranformation will be applied, which maps the minimal value in
     * the channel to (0,0,0).  If false, the value zero will be mapped
     * to zero.
     * @param maxToWhite if maxToWhite is true, a linear gray-valued
     * transformation will be applied, which maps the maximal value in
     * the channel to (255,255,255).  If false, the value 1.0f will be
     * mapped to 255.
     * @return a reference to this image
     * Example:
     * \code
     *   lti::channel matA(10,10,1.0f); // a channel
     *   lti::image  matB;             // an image
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have
     *                                // rgbaPixel(255,255,255)
     * \endcode
     */
    image& castFrom(const fmatrix& other,
                    const bool minToBlack = false,
                    const bool maxToWhite = false);

    /**
     * Cast from the \a other channel32.
     * For the transformation it assumes the channel as a gray valued
     * channel where 0 means black and 1.0f means white.  All other
     * values will be clipped (less than zero to zero and more than 1.0 to 1.0)
     *
     * @param other the channel32 to be cast
     * @param minToBlack if minToBlack is true, a linear gray-valued
     * tranformation will be applied, which maps the minimal value in
     * the channel to (0,0,0).  If false, the value zero will be mapped
     * to zero.
     * @param maxToWhite if maxToWhite is true, a linear gray-valued
     * transformation will be applied, which maps the maximal value in
     * the channel to (255,255,255).  If false, the value 1.0f will be
     * mapped to 255.
     * @return a reference to this image
     * Example:
     * \code
     *   lti::channel matA(10,10,1.0f); // a channel
     *   lti::image  matB;             // an image
     *
     *   matB.castFrom(matA);         // this will copy matA in matB!!
     *                                // and all elements will have
     *                                // rgbaPixel(255,255,255)
     * \endcode
     */
    image& castFrom(const matrix<int32>& other,
                    const bool minToBlack = false,
                    const bool maxToWhite = false);

    /**
     * Alias for copy
     */
    inline image& castFrom(const image& other) {
      copy(other);
      return *this;
    }

  };

  // =========================================
  // Define a palette type for indexed images
  // =========================================

  /**
   *  Vector of rgbaPixel:  used as a color palette
   */
  typedef vector<rgbaPixel> palette;

  /**
   *  An empty vector used to denote an empty palette
   */
  const palette emptyPalette;

}


#endif

