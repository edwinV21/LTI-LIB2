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
 * \file   ltiRectangle.h
 *         Contains template class for aligned rectangles, which are described
 *         through two points.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRectangle.h,v 1.11 2013-05-26 23:28:39 alvarado Exp $
 */

#ifndef _LTI_RECTANGLE_H
#define _LTI_RECTANGLE_H

#include "ltiIoHandler.h"
#include "ltiPoint.h"
#include "ltiMath.h"
#include "ltiTypeInfo.h"

#include <iostream>

namespace lti {

  /**
   * Policy class to compute the size of a rectangle's side and the
   * inverse transformation.
   * 
   * For integer types it assumes the rectangle lies on a discrete grid with
   * grid elements of size 1x1, which means that the size of a side defined by
   * two scalar values x1 and x2 (x2>x1) is x2-x1+1.
   *
   * For floating point values the size is just x2-x1, because of the
   * assumption that the rectangle lies on a real valued 2D space.
   *
   * The template parameter T represents the type used for the coordinates, and
   * is usually int or float.
   */
  template <typename T>
  class rectangleSide {
  public:
    /**
     * Return the size of a rectangle's side which is aligned with one
     * of the two coordinates, assuming that the delimiting coordinates are
     * x1 and x2.
     *
     * For example:
     * - if T is int, x1=0 and x2=2, then returns 3
     * - if T is float, x1=0 and x2=2 then returns 2
     */
    static inline T size(const T x1,const T x2);

    /**
     * For a given side size, compute two coordinates which generates it.
     *
     * For example:
     * - if T is int and sz is 3, the returned values would be
     *   x1=-1 and x2=1.
     * - if T is float and sz = 3, the returned values would be
     *   x1=-1.5 and x2=1.5
     */
    static inline void invSize(const T sz,T& x1,T& x2);
  };
  
  // specialization for float
  template <>
  class rectangleSide<float> {
  public:
    static inline float size(const float x1,const float x2);
    static inline void invSize(const float sz,float& x1,float& x2);
  };

  // specialization for double
  template <>
  class rectangleSide<double> {
  public:
    static inline double size(const double x1,const double x2);
    static inline void invSize(const double sz,double& x1,double& x2);
  };

  /**
   * Rectangle representation class.
   *
   * A rectangle is described by a pair of points: the upper-left and
   * bottom-right corners.
   *
   * A consistent rectangle will have the x and y coordinates of its upper-
   * left corner smaller than the ones of its bottom-right corner (note
   * the use of a left coordinate system, as usual with imaging systems).
   *
   * You can always check if a rectangle is consistent with the method 
   * \c isConsistent() or even force its consistency with
   * \c ensureConsistency().
   *
   * This is a template class, where the first template parameter \c T 
   * denotes the type used for the two points, which will be of type
   * \c point<T>.  For example, the type rectangle<int> (which
   * has the alias irectangle) contains to ipoints (point<int>) as corner 
   * representation.  You can access the type T through the public type
   * \c value_type.
   *
   * The second template parameter S is optional and allows the rectangle to
   * behave correctly in a discrete integer grid or in a continuous real grid
   * (please see the lti::rectangleSide policy for more information).
   *
   * @ingroup gGeomData
   */
  template <typename T,class S=rectangleSide<T> >
  class rectangle {
  public:
    /**
     * Type used in each coordinate of the two internal points
     */
    typedef T value_type;

    /**
     * Type used for the side policy
     */
    typedef S side_type;

    /**
     * \name The two corners of the rectangle
     */
    //@{
    /**
     * Upper-left point<T>
     */
    point<T> ul;

    /**
     * Bottom-right point<T>
     */
    point<T> br;
    //@}
    
    /**
     * Default constructor
     *
     * The two points \a ul and \a br will be initialized with (0,0)
     */
    rectangle();

    /**
     * Constructor with two points
     *
     * @param ul upper left corner
     * @param br bottom right corner
     */
    rectangle(const point<T>& ul,const point<T>& br);

    /**
     * Construct a rectangle via separate coordinates
     *
     * @param left left side coordinate
     * @param top top side coordinate
     * @param right right side coordinate
     * @param bottom bottom side coordinate
     */
    rectangle(const T& left,const T& top,
              const T& right,const T& bottom);

    /**
     * Construct a square with the given side size.
     *
     * If the size is even and T is an integer type, the rectangle
     * will be created from -|_size/2_| to size + |_size/2_| in both
     * coordinates x and y.
     */
    rectangle(const T& size);

    /**
     * Copy constructor
     */
    template<typename U,class US>
    rectangle(const rectangle<U,US>& other);


    /**
     * Set the corners via separate coordinates
     *
     * You must ensure that the data is consistent, i.e. left < right and
     * top < bottom.  See setConsistent().
     *
     * @param left left side coordinate
     * @param top top side coordinate
     * @param right right side coordinate
     * @param bottom bottom side coordinate
     * @return true when successful, false otherwise
     */
    inline bool set(const T& left, const T& top, 
                    const T& right, const T& bottom);


    /**
     * Set the corners via two points
     *
     * You must ensure that the data is consistent, i.e. ul.x < br.x and
     * ul.y < br.y.  See setConsistent().
     *
     * @param ul upper left corner
     * @param br bottom right corner
     * @return true upon success, false otherwise
     */
    inline bool set(const point<T>& ul, const point<T>& br);

    /**
     * Set the corners via separate coordinates.
     *
     * This method ensures consistency of the resulting rectangle, no matter if
     * the given data implies negative sides.
     *
     * @param left left side coordinate
     * @param top top side coordinate
     * @param right right side coordinate
     * @param bottom bottom side coordinate
     * @return true when successful, false otherwise
     */
    inline bool setConsistent(const T& left, const T& top, 
                              const T& right, const T& bottom);


    /**
     * Set the corners via two points
     *
     * This method ensures consistency of the resulting rectangle, no matter if
     * the given data implies negative sides.
     *
     * @param ul upper left corner
     * @param br bottom right corner
     * @return true upon success, false otherwise
     */
    inline bool setConsistent(const point<T>& ul, const point<T>& br);


    /**
     * @name Location and Dimensions
     */
    //@{

    /**
     * Resize rectangle<T,S> with new dimensions, but keep its center
     */
    void resize(const point<T>& dim);

    /**
     * Resize rectangle<T,S> with new dimensions, but keep its center
     */
    void resize(const T& x, const T& y);

    /**
     * Get dimensions of rectangle<T,S>.  If one coordinate of the two
     * points coincide and the rectangle contains fixed point typed
     * points, the returned dimension is one.  For floating point
     * types this case will return 0.
     *
     * \warning Note that the result of this method depends on the type T
     *
     * For integer types it will be assumed that the rectangle lies on
     * a discrete grid with grid elements of size 1x1, and that is not
     * the rectangle itself what is described but the number of grid
     * elements.  This means that the size of a side defined by two
     * scalar values x1 and x2 (x2>x1) is x2-x1+1.  This is useful for 
     * example when dealing with rectangular regions of images.
     *
     * For floating point values the size is just x2-x1, because of
     * the assumtion that the rectangle lies on a real valued 2D space.
     */
    inline point<T> getDimensions() const;

    /**
     * Get the area of this rectangle. This value is always
     * positive, regardless of the position of the upper left and
     * bottom right corner. A rectangle whose corners coincide
     * has an area of 1 for fixed point types or 0 for floating point ones.
     *
     * \warning Note that the result of this method depends on the type T
     *
     * For integer types it will be assumed that the rectangle lies on
     * a discrete grid with grid elements of size 1x1, and that is not
     * the rectangle itself what is described but the index of grid
     * elements.  This means that the size of a side defined by two
     * scalar values x1 and x2 (x2>x1) is x2-x1+1.  This is useful for 
     * example when dealing with rectangular regions of images.
     *
     * For floating point values the size is just x2-x1, because of
     * the assumtion that the rectangle lies on a real valued 2D space.
     */
    inline typename typeInfo<T>::square_accumulation_type getArea() const;

    /**
     * Get center of rectangle<T,S>
     */
    inline point<T> getCenter() const;

    /**
     * Set the center of this rectangle.
     */
    void setCenter(point<T> center);

    /**
     * Ensure consistency of upper-left and bottom-right corners.
     *
     * A rectangle<T,S> is called "consistent" if the upper-left point<T> has
     * lower coordinate-values than the bottom-right point<T>.
     *
     * The consistency is ensured by exchanging the inconsistent values.
     */
    void ensureConsistency();

    /**
     * Check for consistent rectangle<T,S>.
     * @return true if both upper-left corner and bottom-right are correct, i.e.
     * if the upper coordinate is smaller than the bottom coordinate, and the
     * left coordinate is smaller than the right one.
     */
    inline bool isConsistent() const;

    /**
     * Shift rectangle<T,S> by delta
     *
     * @param delta The shift amount to be added to both corners
     */
    inline void shift(const point<T>& delta);

    /**
     * Shift the other rectangle<T,S> by delta and leave the result here
     *
     * @param other The reference rectangle
     * @param delta The shift amount to be added to both corners of the
     *              reference rectangle
     */
    inline void shift(const rectangle<T,S>& other,
                      const point<T>& delta);
    //@}

    /**
     * @name Simple operations
     */
    //@{

    /**
     * Check if point<T> p is inside this rectangle. (border inclusive!)
     */
    inline bool contains(const point<T>& p) const;

    /**
     * Check if the given coordinates are inside this rectangle. 
     * (border included!)
     */
    inline bool contains(const T& x, const T& y) const;

    /**
     * Modify this rectangle to include the given point
     *
     * @see contains
     *
     * @return this rectangle
     */
    rectangle<T,S>& include(const point<T>& p);

    /**
     * Modify this rectangle to include the given point
     *
     * @see contains
     *
     * @return this rectangle
     */
    rectangle<T,S>& include(const T& x, const T& y);

    //@}

    /**
     * @name Duplication
     */
    //@{

    /**
     * Copy member
     */
    rectangle<T,S>& copy(const rectangle<T,S>& other);

    /**
     * Cast from a rectangle of another type
     */
    template <class U,class SU>
    rectangle<T,S>& castFrom(const rectangle<U,SU>& other);

    /**
     * Alias for copy
     */
    inline rectangle<T,S>& operator=(const rectangle<T,S>& other);

    //@}

    /**
     * @name Comparison
     */
    //@{

    /**
     * Test for equality
     */
    inline bool operator==(const rectangle<T,S>& other) const;

    /**
     * Test for inequality
     */
    inline bool operator!=(const rectangle<T,S>& other) const;

    //@}

    /**
     * @name Geometrical combination
     */
    //@{

    /**
     * Intersection of this rectangle with tRect
     *
     * (the largest rectangle which is contained both inside this
     * rectangle and inside tRect).
     *
     * The result will be left in this instance. If the rectangles do
     * not intersect, the resul will be inconsistent, so you should
     * call isConsistent() to check for this case unless you know for
     * sure that it cannot occur.
     *
     * @return A reference to this (modified) instance.
     */
    rectangle<T,S>& intersect(const rectangle<T,S>& tRect);

    /**
     * Check whether this rectangle overlaps with the given rectangle.
     */
    bool overlaps(const rectangle<T,S>& tRect) const;

    /**
     * Check whether this rectangle is close to the given rectangle, i.e.
     * if the two rectangles overlap if one is extended by the given distance.
     */
    bool isClose(const rectangle<T,S>& tRect, const point<T>& dist) const;

    /**
     * This rectangle is set to the intersection of himself with tRect.
     * (alias for intersect)
     */
    inline rectangle<T,S>& operator&=(const rectangle<T,S>& tRect);

    /**
     * New rectangle equals the intersection of this rectangle with tRect
     */
    inline rectangle<T,S> operator&(const rectangle<T,S>& tRect);
    
    /**
     * Union-rectangle of this rectangle with tRect.
     *
     * This is the smallest rectangle containing both this rectangle and
     * tRect. The result will be left in this instance.
     *
     * @return A reference to this (modified) instance.
     */
    rectangle<T,S>& join(const rectangle<T,S>& tRect);

    /**
     * This rectangle is set to the union-rectangle of this rectangle with
     * tRect (alias for join()).
     */
    inline rectangle<T,S>& operator|=(const rectangle<T,S>& tRect);

    /**
     * Generates a new rectangle equal to the smallest rectangle that
     * contains the union of this rectangle with tRect
     */
    rectangle<T,S> operator|(const rectangle<T,S>& tRect);
    //@}
  };

  /**
   * A rectangle with integer coordinates
   */
  typedef rectangle<int> irectangle;

  /**
   * A rectangle with float coordinates
   */
  typedef rectangle<float> frectangle;

  /**
   * A rectangle with double coordinates
   */
  typedef rectangle<double> drectangle;

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also read.
   *
   * @ingroup gStorable
   */
  template<typename T,class S>
  bool read(ioHandler& handler,rectangle<T,S>& p,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also written.
   *
   * @ingroup gStorable
   */
  template <typename T,class S>
  bool write(ioHandler& handler,const rectangle<T,S>& p,
             const bool complete=true);
}

namespace std {

  template <typename T,class S>
  inline ostream& operator<<(ostream& s,const lti::rectangle<T,S>& p);

  template <typename T,class S>
  inline istream& operator>>(istream& s,lti::rectangle<T,S>& p);
}

#include "ltiRectangle_inline.h"
#include "ltiRectangle_template.h"

#endif

