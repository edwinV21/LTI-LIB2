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
 * \file   ltiRGBPixel.h
 *         Contains the basic template class to represent RGB pixels (without
 *         an alpha channel)
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBPixel.h,v 1.10 2008-12-26 17:51:33 alvarado Exp $
 */

#ifndef LTI_RGB_PIXEL_H
#define LTI_RGB_PIXEL_H

#include "ltiAssert.h"
#include "ltiTypes.h"
#include "ltiTypeInfo.h"
#include "ltiIoHandler.h"

#include "ltiRGBAPixel.h"

#include <iosfwd>  // Remember: instead of <iostream> we can just include
                   // iosfwd as we only need the forward declaration of the
                   // stream!  This saves compilation times!

namespace lti {

  /**
   * Color pixel representation in RGB color space.
   *
   * This is a more general type than lti::rgbaPixel, which is used to
   * represent points in the RGB color space with arbitrary precision,
   * specified through the template type of the class.
   *
   * The types supported for T are byte, ubyte, int16, uint16, int32, uint32,
   * float and double.
   *
   * @ingroup gColor
   */
  template <typename T>
  class rgbPixel {
  public:
    /**
     * Anonymous union to provide efficient access through three different
     * mechanism to the pixel components.
     *
     * The order of the data in this union assumes little endianness.
     */
    union {
      /**
       * Anonymous union with the three channels
       */
       __extension__ struct {
        /**
         * Red channel
         */
        T red;

        /**
         * Green channel
         */
        T green;

        /**
         * Blue channel
         */
        T blue;
      };

      /**
       * The four values as array
       */
      T data[3];
    };

    /**
     * Used for the template-based interface for pixels as vectors.
     */
    typedef T value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * Default constructor
     *
     * The default constructor of the LTI-Lib rgbPixel<T> does \b not
     * initialize anything, in order to allow the creation of uninitialized 
     * images in a more efficient way.
     */
    rgbPixel();

    /**
     * RGB constructor
     * @param r value for the red component
     * @param g value for the green component
     * @param b value for the blue component
     */
    rgbPixel(const T r,const T g,const T b);

    /**
     * Copy constructor
     */
    rgbPixel(const rgbaPixel& other);

    /**
     * Copy constructor
     */
    template<class U>
    inline rgbPixel(const rgbPixel<U>& other);

    /**
     * Set the red, green, blue values for the pixel
     *
     * @param r red component
     * @param g green component
     * @param b blue component
     */
    template<class U>
    inline void set(const U r, const U g, const U b);

    /**
     * Sets the red component to given value
     */
    inline void setRed(const T r);

    /**
     * Sets the green component to given value
     */
    inline void setGreen(const T g);

    /**
     * Sets the blue component to given value
     */
    inline void setBlue(const T b);

    /**
     * Get the three color components and write them in the given arguments
     */
    template<class U>
    inline void get(U& r,U& g,U& b) const;

    /**
     * Returns red component
     */
    inline const T& getRed() const;

    /**
     * Returns green component
     */
    inline const T& getGreen() const;

    /**
     * Returns blue component
     */
    inline const T& getBlue() const;

    /**
     * Returns red component
     */
    inline T& getRed();

    /**
     * Returns green component
     */
    inline T& getGreen();

    /**
     * Returns blue component
     */
    inline T& getBlue();

    /**
     * Used to simulate vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components is at(0) for red, at(1) for green and
     * at(2) for blue.
     */
    inline T& at(const int x);

    /**
     * Used to simulate read-only vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components still depents on the endianness of the
     * system, but is usually at(0) for red, at(1) for green and
     * at(2) for blue.
     */
    inline const T& at(const int x) const;

    /**
     * Used to simulate vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components is [0] for red, [1] for green and
     * [2] for blue.
     */
    inline T& operator[](const int x);

    /**
     * Used to simulate read-only vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components still depents on the endianness of the
     * system, but is usually [0] for red, [1] for green and
     * [2] for blue.
     */
    inline const T& operator[](const int x) const;

    /**
     * Used to simulate the vector size.
     *
     * It returns always 3.
     */
    inline size_type size() const;

    /**
     * Copy the "other" pixel
     */
    inline rgbPixel<T>& copy(const rgbPixel<T>& other);

    /**
     * Alias for copy
     */
    inline rgbPixel<T>& operator=(const rgbPixel<T>& other);

    /**
     * Copy the "other" pixel
     */
    inline rgbPixel<T>& copy(const rgbaPixel& other);

    /**
     * Alias for copy
     */
    inline rgbPixel<T>& operator=(const rgbaPixel& other);

    /**
     * Cast To an lti::rgbaPixel.
     *
     * The alpha value of the rgbaPixel is always set to zero
     */
    inline void castTo(rgbaPixel& other) const;

    /**
     * Get a new lti::rgbaPixel with the contents of this pixel.
     *
     * Just a static_cast of the attributes will be made and the alpha is set
     * to zero.  No clipping of the values is made in case of under or
     * overflow.
     */
    inline rgbaPixel getRGBAPixel() const;

    /**
     * Get a new lti::rgbaPixel with the contents of this pixel.
     *
     * Just a static_cast of the attributes will be made and the alpha is set
     * to zero.  In case of under/overflow, the values are set to zero or 255.
     */
    inline rgbaPixel getClippedRGBAPixel() const;

    /**
     * Compare two pixels (true if equal!)
     *
     * Two pixels are \e equal if all \b four components (red, green,
     * blue, and \b alpha) are equal.  It is an usual error to leave the
     * alpha channel uninizalized and then compare for equalilty.
     */
    inline bool isEqual(const rgbPixel<T>& other) const;

    /**
     * Alias for compare()
     *
     * Two pixels are \e equal if all \b four components (red, green,
     * blue, and \b alpha) are equal.  It is an usual error to leave the
     * alpha channel uninizalized and then compare for equalilty.
     */
    inline bool operator==(const rgbPixel<T>& other) const;

    /**
     * Alias for !compare()
     *
     * Two pixels are \e equal if all \b four components (red, green,
     * blue, and \b alpha) are equal.  It is an usual error to leave the
     * alpha channel uninizalized and then compare for equalilty.
     */
    inline bool operator!=(const rgbPixel<T>& other) const;

    /**
     * Less than operator.
     *
     * An rgbPixel<T> is said to be "smaller" than another one, if
     * its red component its smaller, or (if both red components are
     * equal) if the green component if smaller, or (if both green components
     * are equal) if the blue component is smaller.
     */
    inline bool operator<(const rgbPixel<T>& other) const;

    /**
     * Greater than operator.
     *
     * An rgbPixel<T> is said to be "greater" than another one, if
     * its red component its greater, or (if both red components are
     * equal) if the green component if greater, or (if both green components
     * are equal) if the blue component is greater.
     */
    inline bool operator>(const rgbPixel<T>& other) const;

    /**
     * Add this pixel with another one.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbPixel<T>& add(const rgbPixel<T>& other);

    /**
     * Add this pixel with another one.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbPixel<T>& operator+=(const rgbPixel<T>& other);

    /**
     * Add this pixel with the other one without altering anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator+(const rgbPixel<T>& other) const;

    /**
     * Subtract 'other' from this pixel.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbPixel<T>& subtract(const rgbPixel<T>& other);

    /**
     * Subtract 'other' from this pixel.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbPixel<T>& operator-=(const rgbPixel<T>& other);

    /**
     * Subtract 'other' from this pixel without altering anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator-(const rgbPixel<T>& other) const;

    /**
     * Multiply this pixel with another one.
     *
     * The pixel multiplication multiplies elementwise the elements of
     * the pixel, except the alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& multiply(const rgbPixel<T>& other);

    /**
     * Multiply this pixel with another one.
     *
     * The pixel multiplication multiplies elementwise the elements of
     * the pixel, except the alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& operator*=(const rgbPixel<T>& other);

    /**
     * Multiply this pixel with another one without altering anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator*(const rgbPixel<T>& other) const;

    /**
     * Multiply all components of this pixel with an integer, except the alpha
     * channel, which is kept unchanged.
     */
    inline rgbPixel<T>& multiply(const int other);

    /**
     * Multiply all components of this pixel with a float, except the alpha
     * channel, which is kept unchanged.
     */
    inline rgbPixel<T>& multiply(const float& other);

    /**
     * Multiply all components of this pixel with a float, except the alpha
     * channel, which is kept unchanged.
     */
    inline rgbPixel<T>& multiply(const double& other);

    /**
     * Multiply all components of this pixel with a an integer, except the
     * alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& operator*=(const int other);

    /**
     * Multiply all components of this pixel with a a float, except the
     * alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& operator*=(const float& other);

    /**
     * Multiply all components of this pixel with a a double, except the
     * alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& operator*=(const double& other);

    /**
     * Multiply all components of this pixel with a an integer, except the
     * alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator*(const int other) const;

    /**
     * Multiply all components of this pixel with a a float, except the
     * alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator*(const float& other) const;

    /**
     * Multiply all components of this pixel with a a double, except the
     * alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator*(const double& other) const;

    /**
     * Divide this pixel with another one.
     *
     * The pixel division divides elementwise the elements of the pixel
     * except the alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& divide(const rgbPixel<T>& other);

    /**
     * Divide this pixel with another one.
     *
     * The pixel division divides elementwise the elements of the pixel
     * except the alpha channel, which is kept unchanged.
     */
    inline rgbPixel<T>& operator/=(const rgbPixel<T>& other);

    /**
     * Divide this pixel with another one.
     *
     * The pixel division divides elementwise the elements of the pixel
     * except the alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator/(const rgbPixel<T>& other) const;

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& divide(const int other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& divide(const float& other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& divide(const double& other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& divide(const rgbPixel<T>& other,const int cst);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& divide(const rgbPixel<T>& other,const float& cst);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& divide(const rgbPixel<T>& other,const double& cst);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& operator/=(const int other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& operator/=(const float& other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbPixel<T>& operator/=(const double& other);

    /**
     * Divide all components of this pixel with an integer without altering
     * anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator/(const int other) const;

    /**
     * Divide all components of this pixel with an integer without altering
     * anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator/(const float& other) const;

    /**
     * Divide all components of this pixel with an integer without altering
     * anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbPixel<T> operator/(const double& other) const;

   /**
    * Compute the square of the magnitud of this pixel 
    * \f$red^2+green^2+blue^2\f$.
    */
    inline typename typeInfo<T>::square_accumulation_type absSqr() const;

   /**
    * Scalar product in the 3D RGB color space.
    *
    * Get the scalar product of this pixel with another one, considering
    * them as a 3D point in the RGB color space.
    *
    * The dot product will be the sum of the 
    * red*other.red + green*other.green + blue*other.blue
    */
    inline typename typeInfo<T>::square_accumulation_type 
    dot(const rgbPixel<T>& other) const;

    /**
     * Square of the distance between this pixel and the other one.
     * 
     * \f$(red-other.red)^2+(green-other.green)^2+(blue-other.blue)^2\f$.
     */
    inline typename typeInfo<T>::square_accumulation_type 
    distanceSqr(const rgbPixel<T>& other) const;
  };

  /**
   * Multiply all components of the given pixel with an integer
   * except the alpha channel.
   */
  template <typename T>
  inline rgbPixel<T> operator*(const int p,const rgbPixel<T>& other);

  /**
   * Multiply all components of the given pixel with an integer
   * except the alpha channel.
   */
  template <typename T>
  inline rgbPixel<T> operator*(const float p,const rgbPixel<T>& other);

  /**
   * Multiply all components of the given pixel with an integer
   * except the alpha channel.
   */
  template <typename T>
  inline rgbPixel<T> operator*(const double& p,const rgbPixel<T>& other);

  /**
   * Read the vector from the given ioHandler.
   *
   * The complete flag indicates if the enclosing begin and end should be also
   * be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,rgbPixel<T>& p,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  
   *
   * The complete flag indicates if the enclosing begin and end should be also
   * be written or not
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool write(ioHandler& handler,const rgbPixel<T>& p,const bool complete=true);

  /**
   * Alias for rgbPixel<int>
   */
  typedef rgbPixel<int> irgbPixel;

  /**
   * Alias for rgbPixel<float>
   */
  typedef rgbPixel<float> frgbPixel;

  /**
   * Alias for rgbPixel<double>
   */
  typedef rgbPixel<double> drgbPixel;
}

namespace std {
  template <typename T>
  ostream& operator<<(ostream& s,const lti::rgbPixel<T>& p);

  template <typename T>
  istream& operator>>(istream& s,lti::rgbPixel<T>& p);
}

#include "ltiRGBPixel_inline.h"

#endif


