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
 * \file   ltiPoint3D.h
 *         Defines the template type lti::point3D<T> and all shortcuts like
 *         lti::ipoint3D.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiPoint3D.h,v 1.4 2006-06-06 09:50:03 doerfler Exp $
 */

#ifndef LTI_POINT_3D_H
#define LTI_POINT_3D_H

#include "ltiConfig.h"
#include "ltiAssert.h"
#include "ltiIoHandler.h"

#include <iostream>

namespace lti {

  /**
   * Three dimensional point, containing the coordinates x, y, z.
   *
   * The template type T will be the one used for each coordinate.  For
   * example point3D<float> uses float for \a x, \a y, and \a z.
   * 
   * This data structure simplifies the manipulation of 3D points providing
   * simple interfaces for adding, substracting, distance (L2), and more.
   *
   * There are some alias to shorten the notation:
   * -ipoint3D is equivalent to point3D<int>
   * -fpoint3D is equivalent to point3D<sreal>
   * -dpoint3D is equivalent to point3D<dreal>
   *
   * @ingroup gGeomData
   */
  template <typename T>
  class point3D {
  public:
    /**
     * Used for the template-based interface for pixels as vectors.
     */
    typedef T value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /*
     * Anonymous union to allow the fastes access to the elements as
     * semantic tokes (x and y) and as array as well
     */
    union {
      __extension__ struct {
        /**
         * Coordinate x
         */
        value_type x;
        
        /**
         * Coordinate y;
         */
        value_type y;

        /**
         * Coordinate z;
         */
        value_type z;
      };

      /**
       * Array that shares the same memory with x and y
       */
      value_type data[3];
    };

    /**
     * Default constructor
     */
    explicit point3D(const T newx=T(),const T newy=T(),const T newz=T());

    /**
     * Copy constructor
     */
    template <typename U>
    point3D(const point3D<U>& p);

    /**
     * Casts all elements of \c p to T and sets these values in this point.
     */
    template <typename U>
    point3D<T>& castFrom(const point3D<U>& p);

    /**
     * Set the coordinate values and return a reference to this point3D
     */
    inline point3D<T>& set(const T tx,const T ty,const T tz);

    /**
     * Get the coordinate values 
     */
    inline void get(T& tx,T& ty,T& tz) const;

    /**
     * Calculate %square of distance to the point3D c. This
     * method is faster than distanceTo (because it does not compute
     * the square root).
     */
    inline T distanceSqr(const point3D<T>& c) const;

    /**
     * Return the square of the magnitude of the point3D
     */
    inline T absSqr() const;

    /**
     * Dot product with another point3D.
     *
     * @return this->x*p.x + this->y*p.y + this->z*p.z.
     */
    inline T dot(const point3D<T>& p) const;

    /**
     * Cross product with another point3D
     */
    inline point3D<T>& cross(const point3D<T>& c);

    /**
     * Cross product between the given two point3D
     */
    inline point3D<T>& cross(const point3D<T>& a,const point3D<T>& b);
    
    /**
     * Multiply point3D<T> with a given factor
     */
    template<typename U>
    inline point3D<T>& multiply(const U c);

    /**
     * Multiply point3D<T> with a given factor
     */
    template<typename U>
    inline point3D<T> operator*(const U c) const;

    /**
     * Multiply the other point3D point3D<T> with a given factor
     */
    template<typename U>
    inline point3D<T>& multiply(const point3D<T>& other,const U c);

    /**
     * Multiply point3D<T> with a given factor
     */
    template<typename U>
    inline point3D<T>& operator*=(const U c);

    /**
     * Multiplies elementwise the components of this and the point3D c
     *
     * @see dot(),cross()
     */
    inline point3D<T> operator*(const point3D<T>& c) const;

    /**
     * Multiplies elementwise the components of this and the point3D c
     *
     * @see dot(),cross()
     */
    inline point3D<T>& operator*=(const point3D<T>& c);

    /**
     * Multiplies elementwise the components of this and the point3D c, and
     * leave the result here.
     *
     * @see dot(),cross()
     */
    inline point3D<T>& emultiply(const point3D<T>& c);

    /**
     * Multiplies elementwise the components of \a a and \a b and leave the
     * result here.
     *
     * @see dot(),cross()
     */
    inline point3D<T>& emultiply(const point3D<T>& a,const point3D<T>& b);

    /**
     * Divide each component of point3D<T> with a given factor
     */
    template<typename U>
    inline point3D<T>& divide(const U c);

    /**
     * Divide each component of other other point3D<T> with a given factor
     */
    template<typename U>
    inline point3D<T>& divide(const point3D<T>& other,const U c);

    /**
     * Divide each component of point3D<T> by a given factor
     */
    template <typename U>
    inline point3D<T> operator/(const U c) const;

    /**
     * Divide each component of point3D<T> by a given factor
     */
    template <typename U>
    inline point3D<T>& operator/=(const U c);

    /**
     * Elementwise division of each component of the point3Ds
     */
    inline point3D<T> operator/(const point3D<T>& c) const;

    /**
     * Elementwise division of each component of the point3Ds
     */
    inline point3D<T>& operator/=(const point3D<T>& c);

    /**
     * Elementwise division of each component of the point3Ds
     */
    inline point3D<T>& edivide(const point3D<T>& c);

    /**
     * Elementwise division of each component of the point3Ds
     */
    inline point3D<T>& edivide(const point3D<T>& a,const point3D<T>& b);

    /**
     * Modulo c of the integer part of each component of the point3D
     */
    inline point3D<T> operator%(const int c) const;

    /**
     * Add given point3D to this point3D and leave the result here.
     * @param p the other point3D to be added to this one
     * @return a reference to this point3D
     */
    inline point3D<T>& add(const point3D<T>& p);

    /**
     * Add the two other point3Ds and leave the result here
     * @param a first point3D to be added
     * @param b second point3D to be added
     * @return a reference to this point3D, which will contain a+b
     */
    inline point3D<T>& add(const point3D<T>& a,
                         const point3D<T>& b);
    /**
     * Operator + is simmilar to add, but a new point3D is returned, i.e.
     * this point3D will not change.
     */
    inline point3D<T> operator+(const point3D<T>& p) const;

    /**
     * Operator += is an alias for add()
     */
    inline point3D<T>& operator+=(const point3D<T>& p);

    /**
     * Subtract
     */
    inline point3D<T>& subtract(const point3D<T>& p);


    /**
     * Subtract the two other point3Ds and leave the result here
     * @param a first point3D
     * @param b point3D to be subtracted from the first one
     * @return a reference to this point3D, which will contain a-b
     */
    inline point3D<T>& subtract(const point3D<T>& a,
                              const point3D<T>& b);

    /**
     * Operator -
     */
    inline point3D<T> operator-(const point3D<T>& p) const;

    /**
     * Operator -=
     */
    inline point3D<T>& operator-=(const point3D<T>& p);

    /**
     * Copy operator
     */
    inline point3D<T>& copy(const point3D<T>& p);

    /**
     * Operator =
     */
    inline point3D<T>& operator=(const point3D<T>& p);

    /**
     * Operator ==
     */
    inline bool operator==(const point3D<T>& p) const;

    /**
     * Operator !=
     */
    inline bool operator!=(const point3D<T>& p) const;

    /**
     * Operator <
     *
     * A point3D is "smaller" than another one if its coordinates produce
     * an earlier display of the point3D in the monitor. i.e. if it
     * has a smaller y component or (if the y components are the same) if
     * the x component is smaller
     */
    inline bool operator<(const point3D<T>& p) const;

    /**
     * Operator >
     *
     * A point3D is "bigger" than another one if its coordinates produce
     * an later display of the point3D in the monitor. i.e. if it
     * has a bigger y component or (if the y components are the same) if
     * the x component is bigger
     */
    inline bool operator>(const point3D<T>& p) const;

    /**
     * @name Access as vector
     */
    //@{
    /**
     * Used to simulate vector access.
     *
     * The correspondence between the elements of the vector and
     * the components will be [0] for x, [1] for y, and [2] for z
     */
    inline T& operator[](const int i);

    /**
     * Used to simulate read-only vector access.  It allows the use of point3D
     * in templates expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the components will be [0] for x, [1] for y, and [2] for z
     */
    inline const T& operator[](const int i) const;

    /**
     * Used to simulate the vector size
     */
    inline size_type size() const;
    //@}

  private:
    /**
     * Square of the given value
     * 
     * Reimplemented to avoid including ltiMath.h just for this extremely
     * simple function.
     */ 
    inline value_type sqr(const value_type x) const;
  };

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,point3D<T>& p,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,const point3D<T>& p,const bool complete=true);

  /**
   * A point3D with integer coordinates
   */
  typedef point3D<int> ipoint3D;

  /**
   * A point3D with float coordinates
   */
  typedef point3D<float> fpoint3D;

  /**
   * A point3D with double coordinates
   */
  typedef point3D<double> dpoint3D;

}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point3D& p);
  template <typename T>
  inline ostream& operator<<(ostream& s,const lti::point3D<T>& p);
  
  //inline ostream& operator>>(istream& s,const lti::point3D& p);
  template <typename T>
  inline istream& operator>>(istream& s,lti::point3D<T>& p);

}

#include "ltiPoint3D_template.h"

#endif



