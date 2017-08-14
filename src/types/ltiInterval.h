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
 * \file   ltiInterval.h
 *         Defines the template type lti::interval<T> and all shortcuts like
 *         lti::iinterval.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiInterval.h,v 1.3 2010-04-04 13:58:46 alvarado Exp $
 */

#ifndef LTI_INTERVAL_H
#define LTI_INTERVAL_H

#include "ltiAssert.h"
#include "ltiIoHandler.h"
#include "ltiMath.h"
#include "ltiRound.h"

#include <iostream>

namespace lti {

  /**
   * A one dimensional interval, giving the \a from and \a to values.
   *
   * The template type T will be the one used for each limit value.  For
   * example interval<float> uses float for \a from and \a to.
   * 
   * This data structure simplifies the manipulation of one
   * dimensional intervals providing simple interfaces for joining,
   * intersecting, computing lengths, and some extended arithmetical
   * operations to modify both limits simultaneously..
   *
   * There are some alias to shorten the notation:
   * - lti::iinterval is equivalent to lti::interval<int>
   * - lti::finterval is equivalent to lti::interval<sreal>
   * - lti::dinterval is equivalent to lti::interval<dreal>
   *
   * An inteval with a \c from value greater than the \c to value is
   * considered as invalid.  So, methods which return such
   * configuration can be interpreted as empty intervals.
   *
   * @ingroup gGeomData
   */
  template <typename T>
  class interval {
  public:
    /**
     * Used for the template-based interface for pixels as vectors.
     */
    typedef T value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * Anonymous union to allow the fastes access to the elements as
     * semantic tokes (x and y) and as array as well
     */
    union {
      __extension__ struct {
        /**
         * From value
         */
        value_type from;
        
        /**
         * To value
         */
        value_type to;
      };

      /**
       * Array that shares the same memory with from and to
       */
      value_type data[2];
    };

    /**
     * Default constructor
     */
    explicit interval(const T newx=T(),const T newy=T());

    /**
     * Copy constructor
     */
    template <typename U>
    interval(const interval<U>& p);

    /**
     * Copy constructor
     */
    template <typename U>
    interval<T>& castFrom(const interval<U>& p);

    /**
     * Set the coordinate values and return a reference to this interval
     */
    inline interval<T>& set(const T tx,const T ty);

    /**
     * Get the limit values
     */
    inline void get(T& tx,T& ty) const;

    /**
     * Return \c true if \a from is less or equal than \a to
     */
    inline bool valid() const;

    /**
     * Return \c true if \a from is greater than \a to
     */
    inline bool empty() const;

    /**
     * Scale an interval, multiplying each limit by the given scalar.
     *
     * The type U has to be compatible with the type T in the sense that
     * they can be multiplied and casted to T.  This applies to most build 
     * in type like int, float, etc.
     */
    template<typename U>
    inline interval<T>& multiply(const U c);

    /**
     * Scale an interval, multiplying each limit by the given scalar.
     *
     * The type U has to be compatible with the type T in the sense that
     * they can be multiplied and casted to T.  This applies to most build 
     * in type like int, float, etc.
     */
    template<typename U>
    inline interval<T> operator*(const U c) const;

    /**
     * Multiply the other interval interval<T> with a given scale factor
     *
     * The type U has to be compatible with the type T in the sense that
     * they can be multiplied and casted to T.  This applies to most build 
     * in type like int, float, etc.
     */
    template<typename U>
    inline interval<T>& multiply(const interval<T>& other,const U c);

    /**
     * Multiply interval<T> with a given factor
     *
     * The type U has to be compatible with the type T in the sense that
     * they can be multiplied and casted to T.  This applies to most build 
     * in type like int, float, etc.
     */
    template<typename U>
    inline interval<T>& operator*=(const U c);

    /**
     * Multiplies elementwise the components of this and the interval c, and
     * leave the result here.
     */
    inline interval<T>& emultiply(const interval<T>& c);

    /**
     * Multiplies elementwise the components of \a a and \a b and leave the
     * result here.
     */
    inline interval<T>& emultiply(const interval<T>& a,const interval<T>& b);

    /**
     * Divide each component of interval<T> with a given factor
     */
    template<typename U>
    inline interval<T>& divide(const U c);

    /**
     * Divide each component of other other interval<T> with a given factor
     */
    template<typename U>
    inline interval<T>& divide(const interval<T>& other,const U c);

    /**
     * Divide each component of interval<T> by a given factor
     */
    template <typename U>
    inline interval<T> operator/(const U c) const;

    /**
     * Divide each component of interval<T> by a given factor
     */
    template <typename U>
    inline interval<T>& operator/=(const U c);

    /**
     * Elementwise division of each component of the intervals
     */
    inline interval<T>& edivide(const interval<T>& c);

    /**
     * Elementwise division of each component of the intervals
     */
    inline interval<T>& edivide(const interval<T>& a,const interval<T>& b);

    /**
     * Modulo c of the integer part of each component of the interval
     */
    inline interval<T> operator%(const int c) const;

    /**
     * Find the smallest interval which contains all points of both,
     * this interval and the given one.
     *
     * The operation is performed by obtaining the minimum value between
     * both \a from attributes as new \a from, and the maximum value between
     * both \a to attributes as new \a to.
     *
     * @param p the other interval to be joined to this one
     * @return a reference to this interval
     */
    inline interval<T>& join(const interval<T>& p);
    
    /**
     * Find the smallest interval which contains all points of both
     * intervals \a a and \a b.
     *
     * The operation is performed by obtaining the minimum value between
     * both \a from attributes as new \a from, and the maximum value between
     * both \a to attributes as new \a to.
     *
     * @param a first interval to be joined
     * @param b second interval to be joined
     * @return a reference to this interval, which will contain a+b
     */
    inline interval<T>& join(const interval<T>& a,
                             const interval<T>& b);

    /**
     * Find the smallest interval which contains points in both,
     * this interval and the given one.
     *
     * The operation is performed by obtaining the maximum value between
     * both \a from attributes as new \a from, and the minimum value between
     * both \a to attributes as new \a to.
     *
     * @param p the other interval to be intersected to this one
     * @return a reference to this interval
     */
    inline interval<T>& intersect(const interval<T>& p);
    
    /**
     * Find the smallest interval which contains points in both
     * intervals \a a and \a b.
     *
     * The operation is performed by obtaining the maximum value between
     * both \a from attributes as new \a from, and the minimum value between
     * both \a to attributes as new \a to.
     *
     * @param a first interval to be intersected
     * @param b second interval to be intersected
     * @return a reference to this interval, which will contain a+b
     */
    inline interval<T>& intersect(const interval<T>& a,
                                  const interval<T>& b);

    /**
     * Set subtraction of this and the given interval.
     *
     * Here, the subtraction means the smallest interval which
     * contains all the points of the set-subtraction of this interval
     * minus all the elements of the given one.
     *
     * @param p interval to be subtracted from this one
     * @return reference to this instance.
     */
    inline interval<T>& subtract(const interval<T>& p);


    /**
     * Set subtraction of \a b from \a a
     *
     * Here, the subtraction means the smallest interval which
     * contains all the points of the set-subtraction of this interval
     * minus all the elements of the given one.

     * @param a first interval
     * @param b interval to be subtracted from the first one
     * @return a reference to this interval, which will contain a-b
     */
    inline interval<T>& subtract(const interval<T>& a,
                                 const interval<T>& b);

    /**
     * Operator -
     *
     * Alias for subtract
     */
    inline interval<T> operator-(const interval<T>& p) const;

    /**
     * Operator -=
     *
     * Alias for subtract
     */
    inline interval<T>& operator-=(const interval<T>& p);

    /**
     * Copy operator
     */
    inline interval<T>& copy(const interval<T>& p);

    /**
     * Operator =
     */
    inline interval<T>& operator=(const interval<T>& p);

    /**
     * Operator ==
     */
    inline bool operator==(const interval<T>& p) const;

    /**
     * Operator !=
     */
    inline bool operator!=(const interval<T>& p) const;

    /**
     * Length of the interval
     *
     * For this method, the intervals are considered closed.  This is
     * specially important for integer types, which will return
     * from-to+1, while for floating-point types it just return from-to.
     *
     */
    inline T length() const;

    /**
     * Check if element is contained in the interval
     *
     * Return true if the given value is contained within the
     * container.  This method assumes that the intervall represents a closed
     * interval, so that the boundaries are considered as part of it.
     */
    inline bool contains(const T val) const;

    /**
     * Check if the given interval is fully contained in the interval
     *
     * Return true if the given value is contained within the
     * container.  This method assumes that the intervall represents a closed
     * interval, so that the boundaries are considered as part of it.
     */
    inline bool contains(const interval<T>& val) const;

    /**
     * Return the closest value of type T which lies on the interval.
     *
     * This method returns the same value of the argument iff that
     * value lies within the interval, \a from if the given value is
     * less than it, or \a to if it is greater.
     */
    inline T closest(const T val) const;

    /**
     * @name Access as vector
     */
    //@{
    /**
     * Used to simulate vector access.  It allows the use of interval
     * in templates expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the components will be [0] for x and [1] for y
     */
    inline T& operator[](const int i);

    /**
     * Used to simulate read-only vector access.  It allows the use of interval
     * in templates expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the components will be [0] for x and [1] for y
     */
    inline const T& operator[](const int i) const;

    /**
     * Used to simulate the vector size.
     */
    inline size_type size() const;
    //@}

  };

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,interval<T>& p,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,const interval<T>& p,const bool complete=true);

  /**
   * A interval with integer coordinates
   */
  typedef interval<int> iinterval;

  /**
   * A interval with unsigned integer coordinates
   */
  typedef interval<unsigned int> uiinterval;

  /**
   * A interval with double coordinates
   */
  typedef interval<double> dinterval;

  /**
   * A interval with float coordinates
   */
  typedef interval<float> finterval;

}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::interval& p);
  template <typename T>
  inline ostream& operator<<(ostream& s,const lti::interval<T>& p);
  
  //inline ostream& operator>>(istream& s,const lti::interval& p);
  template <typename T>
  inline istream& operator>>(istream& s,lti::interval<T>& p);

}

#include "ltiInterval_template.h"

#endif



