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
 * \file   ltiRGBAPixel.h
 *         Contains the basic template class to represent RGB pixels with
 *         an alpha channel
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBAPixel.h,v 1.12 2008-12-22 02:41:10 alvarado Exp $
 */

#ifndef LTI_RGBA_PIXEL_H
#define LTI_RGBA_PIXEL_H

#include "ltiConfig.h"
#include "ltiAssert.h"
#include "ltiTypes.h"
#include "ltiIoHandler.h"

#include <iostream>

namespace lti {

  /**
   * Color pixel representation in RGB color space with an alpha channel.
   *
   * Since most modern cameras provide the images as matrices of RGB triplets
   * with values between 0 and 255 for each color channel, and the modern
   * processor architectures allow a more efficient access to the memory if
   * each pixel is 32-bit aligned, the LTI-Lib uses a pixel data structure
   * which also corresponds with a lti::uint32 type.
   *
   * This type contains therefore four unsigned bytes (lti::ubyte) which can
   * be accessed through their color names (red, green, blue) and an additional
   * alpha channel, which is ignored in most operations but can also be used
   * to operate with partial "transparent" pixels.
   *
   * The alpha value of 0 is the default and means also zero transparency.
   * An alpha value of 255 means fully transparent.
   *
   * The related template type lti::rgbPixel<T> can be used to represent
   * the RGB triplets with other types.
   *
   * The use of rgbaPixel as a vector structure considers it as a three
   * dimensional point in the RGB color space, i.e. the alpha value is ignored
   * for the distance and magniture operators, the access operator[] or at()
   * methods can only access the three color components, and the arithmetical
   * operations always ignore the alpha component.
   *
   * @ingroup gColor
   */
  class rgbaPixel {
  public:
    /**
     * Anonymous union to provide efficient access through three different
     * mechanism to the pixel components.
     *
     * The order of the data in this union assumes little endianness.
     */
    union {
      /**
       * Anonymous structure with the three channels
       */
       __extension__ struct {
        //  the sequence blue, green, red _is_ important!

        /**
         * Blue channel
         */
        ubyte blue;

        /**
         * Green channel
         */
        ubyte green;

        /**
         * Red channel
         */
        ubyte red;

        /**
         * Alpha channel.
         *
         * This value can be considered proportional to transparency.
         * 255 means fully transparent, and 0 means a solid color.
         */
        ubyte alpha;
      };

      /**
       * All three channels (and the alpha channel) together
       */
      uint32 value;

      /**
       * The four values as array
       */
      ubyte data[4];
    };

    /**
     * Used for the template-based interface for pixels as vectors.
     */
    typedef ubyte value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * Default constructor
     *
     * The default constructor of the LTI-Lib rgbaPixel does \b not initialize
     * anything, in order to allow the creation of uninitialized images in a
     * more efficient way.
     */
    rgbaPixel();

    /**
     * Copy constructor
     */
    rgbaPixel(const rgbaPixel& other);

    /**
     * Constructor with initialization of attributes.
     *
     * The new rgbaPixel will be initialized with the given value.
     * @param val a 4 ubyte value to be assigned to the three channels and
     *            the alpha channel.
     *            Note that the order depends on the system endianness:
     *            - If you use little endian (for example: Intel Processor)
     *              a value of 0x00010203 means red=01,green=02 and blue=03
     *            - If you use big endian (for example: PowerPC Processor)
     *              a value of 0x00010203 means red=02,green=01 and blue=00
     *            Avoid the use of this constructor if you want to maintain
     *            platform compatibility.
     */
    rgbaPixel(const uint32 val);

    /**
     * RGB constructor
     * @param r 8 bit value for the red component
     * @param g 8 bit value for the green component
     * @param b 8 bit value for the blue component
     * @param d 8 bit value for the alpha byte (default value 0)
     */
    rgbaPixel(const ubyte r,const ubyte g,const ubyte b,const ubyte d=0);

    /**
     * Set the red, green, blue and alpha values for the pixel
     */
    inline void set(const ubyte r, 
                    const ubyte g,
                    const ubyte b,
                    const ubyte a);

    /**
     * Set the red, green, blue values for the pixel
     */
    inline void set(const ubyte r, const ubyte g, const ubyte blue);

    /**
     * Sets the red component to given value
     */
    inline void setRed(const ubyte r);

    /**
     * Sets the green component to given value
     */
    inline void setGreen(const ubyte g);

    /**
     * Sets the blue component to given value
     */
    inline void setBlue(const ubyte b);

    /**
     * Sets the alpha component to given value
     */
    inline void setAlpha(const ubyte d);

    /**
     * Sets the value component (all components together) to given value
     */
    inline void setValue(const uint32& v);

    /**
     * Get the three color components and write them in the given arguments
     */
    inline void get(ubyte& r,ubyte& g,ubyte& b) const;

    /**
     * Get the three color components and write them in the given arguments
     */
    inline void get(int& r,int& g,int& b) const;

    /**
     * Get the four components and write them in the given arguments
     */
    inline void get(ubyte& r,ubyte& g,ubyte& b,ubyte& a) const;

    /**
     * Get the four components and write them in the given arguments
     */
    inline void get(int& r,int& g,int& b,int& a) const;

    /**
     * Returns red component
     */
    inline const ubyte& getRed() const;

    /**
     * Returns green component
     */
    inline const ubyte& getGreen() const;

    /**
     * Returns blue component
     */
    inline const ubyte& getBlue() const;

    /**
     * Returns alpha component
     */
    inline const ubyte& getAlpha() const;

    /**
     * Returns red component
     */
    inline ubyte& getRed();

    /**
     * Returns green component
     */
    inline ubyte& getGreen();

    /**
     * Returns blue component
     */
    inline ubyte& getBlue();

    /**
     * Returns alpha component
     */
    inline ubyte& getAlpha();

    /**
     * Returns 4 byte component with RGB and alpha value
     */
    inline const uint32& getValue() const;

    /**
     * Returns 4 byte component with RGB value
     */
    inline uint32& getValue();

    /**
     * Used to simulate vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components is at(0) for red, at(1) for green and
     * at(2) for blue.
     */
    inline ubyte& at(const int x);

    /**
     * Used to simulate read-only vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components still depents on the endianness of the
     * system, but is usually at(0) for red, at(1) for green and
     * at(2) for blue.
     */
    inline const ubyte& at(const int x) const;

    /**
     * Used to simulate vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components is [0] for red, [1] for green and
     * [2] for blue.
     */
    inline ubyte& operator[](const int x);

    /**
     * Used to simulate read-only vector access.
     *
     * The correspondence between the elements of the vector and
     * the color components still depents on the endianness of the
     * system, but is usually [0] for red, [1] for green and
     * [2] for blue.
     */
    inline const ubyte& operator[](const int x) const;

    /**
     * Used to simulate the vector size.
     *
     * It returns always 3.
     */
    inline size_type size() const;

    /**
     * Copy the "other" pixel
     */
    inline rgbaPixel& copy(const rgbaPixel& other);

    /**
     * Alias for copy
     */
    inline rgbaPixel& operator=(const rgbaPixel& other);

    /**
     * Compare two pixels (true if equal!)
     *
     * Two pixels are \e equal if all \b four components (red, green,
     * blue, and \b alpha) are equal.  It is an usual error to leave the
     * alpha channel uninizalized and then compare for equalilty.
     */
    inline bool isEqual(const rgbaPixel& other) const;

    /**
     * Alias for compare()
     *
     * Two pixels are \e equal if all \b four components (red, green,
     * blue, and \b alpha) are equal.  It is an usual error to leave the
     * alpha channel uninizalized and then compare for equalilty.
     */
    inline bool operator==(const rgbaPixel& other) const;

    /**
     * Alias for !compare()
     *
     * Two pixels are \e equal if all \b four components (red, green,
     * blue, and \b alpha) are equal.  It is an usual error to leave the
     * alpha channel uninizalized and then compare for equalilty.
     */
    inline bool operator!=(const rgbaPixel& other) const;

    /**
     * Less than operator.
     *
     * An rgbaPixel is said to be "smaller" than another one, if
     * getValue() < other.getValue()
     */
    inline bool operator<(const rgbaPixel& other) const;

    /**
     * Greater than operator.
     *
     * An rgbaPixel is said to be "bigger" than another one, if
     * getValue() > other.getValue()
     */
    inline bool operator>(const rgbaPixel& other) const;

    /**
     * Add this pixel with another one.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbaPixel& add(const rgbaPixel& other);

    /**
     * Add this pixel with another one.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbaPixel& operator+=(const rgbaPixel& other);

    /**
     * Add this pixel with the other one without altering anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator+(const rgbaPixel& other) const;

    /**
     * Subtract 'other' from this pixel.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbaPixel& subtract(const rgbaPixel& other);

    /**
     * Subtract 'other' from this pixel.
     *
     * The alpha channel is kept unchanged.
     */
    inline rgbaPixel& operator-=(const rgbaPixel& other);

    /**
     * Subtract 'other' from this pixel without altering anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator-(const rgbaPixel& other) const;

    /**
     * Multiply this pixel with another one.
     *
     * The pixel multiplication multiplies elementwise the elements of
     * the pixel, except the alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& multiply(const rgbaPixel& other);

    /**
     * Multiply this pixel with another one.
     *
     * The pixel multiplication multiplies elementwise the elements of
     * the pixel, except the alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& operator*=(const rgbaPixel& other);

    /**
     * Multiply this pixel with another one without altering anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator*(const rgbaPixel& other) const;

    /**
     * Multiply all components of this pixel with an integer, except the alpha
     * channel, which is kept unchanged.
     */
    inline rgbaPixel& multiply(const int other);

    /**
     * Multiply all components of this pixel with a float, except the alpha
     * channel, which is kept unchanged.
     */
    inline rgbaPixel& multiply(const float& other);

    /**
     * Multiply all components of this pixel with a float, except the alpha
     * channel, which is kept unchanged.
     */
    inline rgbaPixel& multiply(const double& other);

    /**
     * Multiply all components of this pixel with a an integer, except the
     * alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& operator*=(const int other);

    /**
     * Multiply all components of this pixel with a a float, except the
     * alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& operator*=(const float& other);

    /**
     * Multiply all components of this pixel with a a double, except the
     * alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& operator*=(const double& other);

    /**
     * Multiply all components of this pixel with a an integer, except the
     * alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator*(const int other) const;

    /**
     * Multiply all components of this pixel with a a float, except the
     * alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator*(const float& other) const;

    /**
     * Multiply all components of this pixel with a a double, except the
     * alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator*(const double& other) const;

    /**
     * Divide this pixel with another one.
     *
     * The pixel division divides elementwise the elements of the pixel
     * except the alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& divide(const rgbaPixel& other);

    /**
     * Divide this pixel with another one.
     *
     * The pixel division divides elementwise the elements of the pixel
     * except the alpha channel, which is kept unchanged.
     */
    inline rgbaPixel& operator/=(const rgbaPixel& other);

    /**
     * Divide this pixel with another one.
     *
     * The pixel division divides elementwise the elements of the pixel
     * except the alpha channel.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator/(const rgbaPixel& other) const;

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbaPixel& divide(const int other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbaPixel& divide(const float& other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbaPixel& divide(const double& other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbaPixel& operator/=(const int other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbaPixel& operator/=(const float& other);

    /**
     * Divide all components of this pixel with an integer.
     */
    inline rgbaPixel& operator/=(const double& other);

    /**
     * Divide all components of this pixel with an integer without altering
     * anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator/(const int other) const;

    /**
     * Divide all components of this pixel with an integer without altering
     * anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator/(const float& other) const;

    /**
     * Divide all components of this pixel with an integer without altering
     * anything.
     *
     * The alpha channel of the resulting pixel is equal to
     * the one of this pixel (the first operand in the binary expression)
     */
    inline rgbaPixel operator/(const double& other) const;

    /**
     * Overlay the current pixel on the given one.
     * 
     * The current pixel will be altered considering the "transparency" given
     * in the alpha channel.
     *
     * The new pixel will be equal to the current one multiplied by 
     * (1.0-alpha/255) plus the other pixel multiplied by (alpha/255).  
     * The new alpha value is equal to the product of both alpha values
     * divided by 255.
     */
    inline rgbaPixel& overlay(const rgbaPixel& below);

    /**
     * Overlay the first pixel on the second pixel
     * 
     * The current pixel will be computed considering the "transparency" given
     * in the alpha channel of the first pixel.
     *
     * This pixel will be equal to the first one multiplied by
     * (1.0-alpha/255) plus the other pixel multiplied by (alpha/255).  The
     * new alpha value is equal to the product of the alpha values of the
     * other pixels divided by 255.
     */
    inline rgbaPixel& overlay(const rgbaPixel& above,
                              const rgbaPixel& below);


   /**
    * Compute the square of the magnitud of this pixel 
    * \f$red^2+green^2+blue^2\f$.
    */
    inline int absSqr() const;

   /**
    * Scalar product in the 3D RGB color space.
    *
    * Get the scalar product of this pixel with another one, considering
    * them as a 3D point in the RGB color space.
    *
    * The dot product will be the sum of the 
    * red*other.red + green*other.green + blue*other.blue
    */
    inline int dot(const rgbaPixel& other) const;

    /**
     * Square of the distance between this pixel and the other one.
     * 
     * \f$(red-other.red)^2+(green-other.green)^2+(blue-other.blue)^2\f$.
     */
    inline int distanceSqr(const rgbaPixel& other) const;

    /**
     * @name Storable interface
     */
    //@{
    /**
     * Read the pixel from the given ioHandler.
     *
     * The complete flag indicates if the enclosing begin and end should be
     * also be readed
     *
     * @ingroup gStorable
     */
    bool read(ioHandler& handler,const bool complete=true);
    
    /**
     * Write the vector in the given ioHandler.  
     *
     * The complete flag indicates if the enclosing begin and end should be
     * also be written or not.
     *
     * @ingroup gStorable
     */
    bool write(ioHandler& handler,const bool complete=true) const;
    //@}
  };

  /**
   * Multiply all components of the given pixel with an integer
   * except the alpha channel.
   */
  inline rgbaPixel operator*(const int p,const rgbaPixel& other);

  /**
   * Multiply all components of the given pixel with an integer
   * except the alpha channel.
   */
  inline rgbaPixel operator*(const float p,const rgbaPixel& other);

  /**
   * Multiply all components of the given pixel with an integer
   * except the alpha channel.
   */
  inline rgbaPixel operator*(const double& p,const rgbaPixel& other);

  /**
   * Read the rgbaPixel from the given ioHandler.
   *
   * The complete flag indicates if the enclosing begin and end should be also
   * be readed
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,rgbaPixel& p,const bool complete=true);

  /**
   * Write the rgbaPixel in the given ioHandler.  
   *
   * The complete flag indicates if the enclosing begin and end should be also
   * be written or not
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const rgbaPixel& p,const bool complete=true);
}



namespace std {
  ostream& operator<<(ostream& s,const lti::rgbaPixel& p);
  istream& operator>>(istream& s,lti::rgbaPixel& p);
}


#include "ltiTypeInfo.h"
namespace lti {
  template<>
  class typeInfo<rgbaPixel> {
  public:
    typedef int32 accumulation_type;
    typedef int32 square_accumulation_type;
    static ubyte suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::rgbaPixel";};
    static ubyte min() throw() {return std::numeric_limits<ubyte>::min();}
  private:
    typeInfo() {};
  };
}

#include "ltiRGBAPixel_inline.h"

#endif


