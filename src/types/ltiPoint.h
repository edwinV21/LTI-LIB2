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
 * \file   ltiPoint.h
 *         Defines the template type lti::point<T> and all shortcuts like
 *         lti::ipoint.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiPoint.h,v 1.5 2007-11-03 23:50:17 alvarado Exp $
 */

#ifndef LTI_POINT_H
#define LTI_POINT_H

#include "ltiAssert.h"
#include "ltiIoHandler.h"

#include <iostream>

namespace lti {

  /**
   * Two dimensional point, containing the coordinates x, y.
   *
   * The template type T will be the one used for each coordinate.  For
   * example point<float> uses float for \a x and \a y.
   * 
   * This data structure simplifies the manipulation of 2D points providing
   * simple interfaces for adding, substracting, distance (L2), and more.
   *
   * There are some alias to shorten the notation:
   * - lti::ipoint is equivalent to lti::point<int>
   * - lti::fpoint is equivalent to lti::point<sreal>
   * - lti::dpoint is equivalent to lti::point<dreal>
   *
   * @ingroup gGeomData
   */
  template <typename T>
  class point {
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
      };

      /**
       * Array that shares the same memory with x and y
       */
      value_type data[2];
    };

    /**
     * Default constructor
     */
    explicit point(const T newx=T(),const T newy=T());

    /**
     * Copy constructor
     */
    template <typename U>
    point(const point<U>& p);

    /**
     * Copy constructor
     */
    template <typename U>
    point<T>& castFrom(const point<U>& p);

    /**
     * Set the coordinate values and return a reference to this point
     */
    inline point<T>& set(const T tx,const T ty);

    /**
     * Get the coordinate values
     */
    inline void get(T& tx,T& ty) const;

    /**
     * Calculate %square of distance to the point c.
     *
     * If you need the Euclidean distance just call sqrt(distanceSqr).
     */
    inline T distanceSqr(const point<T>& c) const;

    /**
     * Return the square of the magnitude of the point
     */
    inline T absSqr() const;

    /**
     * Dot product with another point.
     *
     * @return this->x*p.x + this->y*p.y.
     */
    inline T dot(const point<T>& p) const;

    /**
     * Multiply point<T> with a given factor
     */
    template<typename U>
    inline point<T>& multiply(const U c);

    /**
     * Multiply point<T> with a given factor
     */
    template<typename U>
    inline point<T> operator*(const U c) const;

    /**
     * Multiply the other point point<T> with a given factor
     */
    template<typename U>
    inline point<T>& multiply(const point<T>& other,const U c);

    /**
     * Multiply point<T> with a given factor
     */
    template<typename U>
    inline point<T>& operator*=(const U c);

    /**
     * Multiplies elementwise the components of this and the point c
     */
    inline point<T> operator*(const point<T>& c) const;

    /**
     * Multiplies elementwise the components of this and the point c
     */
    inline point<T>& operator*=(const point<T>& c);

    /**
     * Multiplies elementwise the components of this and the point c, and
     * leave the result here.
     */
    inline point<T>& emultiply(const point<T>& c);

    /**
     * Multiplies elementwise the components of \a a and \a b and leave the
     * result here.
     */
    inline point<T>& emultiply(const point<T>& a,const point<T>& b);

    /**
     * Divide each component of point<T> with a given factor
     */
    template<typename U>
    inline point<T>& divide(const U c);

    /**
     * Divide each component of other other point<T> with a given factor
     */
    template<typename U>
    inline point<T>& divide(const point<T>& other,const U c);

    /**
     * Divide each component of point<T> by a given factor
     */
    template <typename U>
    inline point<T> operator/(const U c) const;

    /**
     * Divide each component of point<T> by a given factor
     */
    template <typename U>
    inline point<T>& operator/=(const U c);

    /**
     * Elementwise division of each component of the points
     */
    inline point<T> operator/(const point<T>& c) const;

    /**
     * Elementwise division of each component of the points
     */
    inline point<T>& operator/=(const point<T>& c);

    /**
     * Elementwise division of each component of the points
     */
    inline point<T>& edivide(const point<T>& c);

    /**
     * Elementwise division of each component of the points
     */
    inline point<T>& edivide(const point<T>& a,const point<T>& b);

    /**
     * Modulo c of the integer part of each component of the point
     */
    inline point<T> operator%(const int c) const;

    /**
     * Add given point to this point and leave the result here.
     * @param p the other point to be added to this one
     * @return a reference to this point
     */
    inline point<T>& add(const point<T>& p);

    /**
     * Add the two other points and leave the result here
     * @param a first point to be added
     * @param b second point to be added
     * @return a reference to this point, which will contain a+b
     */
    inline point<T>& add(const point<T>& a,
                         const point<T>& b);
    /**
     * Operator + is simmilar to add, but a new point is returned, i.e.
     * this point will not change.
     */
    inline point<T> operator+(const point<T>& p) const;

    /**
     * Operator += is an alias for add()
     */
    inline point<T>& operator+=(const point<T>& p);

    /**
     * Subtract
     */
    inline point<T>& subtract(const point<T>& p);


    /**
     * Subtract the two other points and leave the result here
     * @param a first point
     * @param b point to be subtracted from the first one
     * @return a reference to this point, which will contain a-b
     */
    inline point<T>& subtract(const point<T>& a,
                              const point<T>& b);

    /**
     * Operator -
     */
    inline point<T> operator-(const point<T>& p) const;

    /**
     * Operator -
     */
    inline point<T> operator-() const;

    /**
     * Operator -=
     */
    inline point<T>& operator-=(const point<T>& p);

    /**
     * Copy operator
     */
    inline point<T>& copy(const point<T>& p);

    /**
     * Operator =
     */
    inline point<T>& operator=(const point<T>& p);

    /**
     * Operator ==
     */
    inline bool operator==(const point<T>& p) const;

    /**
     * Operator !=
     */
    inline bool operator!=(const point<T>& p) const;

    /**
     * Operator <
     *
     * A point is "smaller" than another one if its coordinates produce
     * an earlier display of the point in the monitor. i.e. if it
     * has a smaller y component or (if the y components are the same) if
     * the x component is smaller
     */
    inline bool operator<(const point<T>& p) const;

    /**
     * Operator >
     *
     * A point is "bigger" than another one if its coordinates produce
     * an later display of the point in the monitor. i.e. if it
     * has a bigger y component or (if the y components are the same) if
     * the x component is bigger
     */
    inline bool operator>(const point<T>& p) const;

    /**
     * @name Access as vector
     */
    //@{
    /**
     * Used to simulate vector access.  It allows the use of point in templates
     * expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the components will be [0] for x and [1] for y
     */
    inline T& operator[](const int i);

    /**
     * Used to simulate read-only vector access.  It allows the use of point
     * in templates expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the components will be [0] for x and [1] for y
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
  bool read(ioHandler& handler,point<T>& p,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,const point<T>& p,const bool complete=true);

  /**
   * A point with integer coordinates
   */
  typedef point<int> ipoint;

  /**
   * A point with unsigned integer coordinates
   */
  typedef point<unsigned int> uipoint;

  /**
   * A point with double coordinates
   */
  typedef point<double> dpoint;

  /**
   * A point with float coordinates
   */
  typedef point<float> fpoint;

}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point& p);
  template <typename T>
  inline ostream& operator<<(ostream& s,const lti::point<T>& p);
  
  //inline ostream& operator>>(istream& s,const lti::point& p);
  template <typename T>
  inline istream& operator>>(istream& s,lti::point<T>& p);

}

#include "ltiPoint_template.h"

#endif



