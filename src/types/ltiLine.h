/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiLine.h
 * \author Claudia Goenner
 * \date   07.02.2003
 * 
 * $Id: ltiLine.h,v 1.3 2005-07-09 04:26:30 alvarado Exp $
 */

#ifndef LTI_LINE_H
#define LTI_LINE_H

#include "ltiConfig.h"

#include <iostream>
#include <limits>

#include "ltiIoHandler.h"
#include "ltiMath.h"
#include "ltiPoint.h"
#include "ltiRectangle.h"

namespace lti {


  /**
   * Type for computations with lines.
   *
   * A line (or more generally a line<T>) is represented by a start point
   * and an end point.
   *
   * The type T correspond to the coordinate type used in both points.
   *
   * This class stores only the two points, and provides some functionality
   * using them.  Other operations can be achieved more efficiently if more
   * information about the line, like its slope, is also stored.  This is 
   * done by some derived classes.
   *
   * @ingroup gGeomData
   */
  template <class T>
  class line {
  protected:
    /**
     * start point
     */
    point<T> start;
    
    /**
     * end point
     */
    point<T> end;

  public:
    /**
     * default constructor.
     *
     * Both points are left uninitialized (this can save some time)
     */
    explicit line();
    
    /**
     * constructor with both points
     */
    line(const point<T>& theStart, const point<T>& theEnd);
    
    /**
     * copy constructor
     */
    template <class U> 
    inline line(const line<U>& other);
    
    /**
     * cast operator
     */
    template <class U>
    inline line<T>& castFrom(const line<U>& other);

    /**
     * general operator to set both points of the line
     */
    inline void set(const point<T>& theStart,const point<T>& theEnd);

    /**
     * set the start point. 
     */
    inline void setStart(const point<T>& theStart);

    /**
     * set the end point. Does not compute the slope.
     */
    inline void setEnd(const point<T>& theEnd);

    /**
     * exchange the start and end points, making the previous end a
     * start point and the previous start the end point.
     */
    inline void invert();
    
    /**
     * return a read only reference to the start point
     */
    inline const point<T>& getStart() const;

    /**
     * return a read only reference to the end point
     */
    inline const point<T>& getEnd() const;

    /**
     * @name Distance computation
     */
    //@{
    /**
     * calculate minimal euclidian distance of the line segment to the point c.
     *
     * This method is slower than the sqrDistanceTo, which avoids the
     * computation of a (in many cases not required) square root.
     *
     * @see sqrDistanceTo() distanceToXPol()
     */
    inline T distanceTo(const point<T>& c) const;

    /**
     * Calculate minimal %square of euclidian distance to the point c. This
     * method is faster than distanceTo (because it does not calculate
     * the square root).
     *
     * @param c point to which the minimal distance is searched.
     * @return the square of the minimal distance to c
     */
    inline T distanceSqr(const point<T>& c) const;

    /**
     * Calculate minimal %square of euclidian distance to the point c. This
     * method is faster than distanceTo (because it does not calculate
     * the square root).
     *
     * @param c point to which the minimal distance is searched.
     * @param p point in the line segment with the minimal distance to c.
     *
     * @return the square of the minimal distance to c
     */
    T distanceSqr(const point<T>& c,point<T>& p) const;

    /**
     * Calculate distance to the point c to the infinite line (eXtraPolated)
     * containing this line segment.
     *
     * @return the minimal distance to c
     */
    inline T distanceToXPol(const point<T>& c) const;

    /**
     * Calculate %square of distance to the point c to the infinite
     * line (eXtraPolated) containing this line segment.
     *
     * @param c point to which the minimal distance is searched.
     *
     * @return the square of the minimal distance to c
     * @see sqrDistanceTo()
     *
     * This method is faster than distanceToXPol (because it does not
     * calculate the square root).
     */
    inline T distanceSqrXPol(const point<T>& c) const;

    /**
     * Calculate %square of distance to the point c to the infinite
     * line (eXtraPolated) containing this line segment.
     *
     * @param c point to which the minimal distance is searched.
     * @param p point in the extrapolated line segment with the
     *          minimal distance to c.
     *
     * This method is faster than distanceToXPol (because it does not
     * calculate the square root).
     *
     *
     * @see sqrDistanceTo()
     */
    T distanceSqrXPol(const point<T>& c,point<T>& p) const;

    /** 
     * square of the length of this line
     */
    inline T sqrLength() const;
    //@}
  
    /**
     * @name Intersections
     */
    //@{ 

    /**
     * Check if this line segment intersects the \a other given one.
     *
     * @param other the other line segment to which an intersection is
     *              going to be checked.
     * @return true if both line segments intersect.
     */
    bool doesIntersect(const line<T>& other) const;

    /**
     * Check if this line segment is parallel to the \a other given one.
     *
     * @param other the other line segment to which parallelism is
     *              going to be checked.
     * @return true if both line segments are parallel.
     */
    bool isParallel(const line<T>& other) const;  

    /**
     * Check if this line segment is parallel and colinear to the
     * \a other given one.
     *
     * @param other the other line segment to which parallelism is
     *              going to be checked.
     * @return true if both line segments are parallel.
     */
    bool isColinear(const line<T>& other) const;    

    /**
     * Compute the part of this line segment which lies within the
     * given rectangle, and leave the result here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    bool intersect(const rectangle<T>& rect);

    /**
     * Compute the part of the \a other line segment which lies within the
     * given rectangle, and leave the result here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    inline bool intersect(const line<T>& other,
			  const rectangle<T>& rect);

    /**
     * Compute the intersection point of this line segment with the 
     * \a other given one.
     *
     * @param other the other line segment to which the intersection point
     *              is going to be computed.
     * @param p     if there is an intersection between both line segments
     *              the intersection point will be written here.
     * @param colinear this parameter is set to true in case both line segments
     *              are parallel and co-linear.
     *
     * @return true if an unique intersection point exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c true and determines the
     *              intersection if the intersection is inifinitely small.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    template<class U>
    bool getIntersectionPoint(const line<T>& other, point<U>& p,
			      bool& colinear) const;
    
    /**
     * Compute the common line segment between this line segment and the 
     * \a other given one and leave the result here. This intersection is only
     * going to be computed if both lines are colinear.
     *
     * @param other the other line segment to which the intersection
     *              is going to be computed.
     *
     * @return true if an common line segment exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c true and determines the
     *              line segment even if it is inifinitely small, i.e. a point.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    bool getCommonLine(const line<T>& other);
    
    /**
     * Compute the common line segment between the given line
     * segments.  This intersection is only going to be computed if
     * both lines are colinear.
     *
     * @param first first line segment.
     * @param second second line segment.
     *
     * @return true if a common line segment exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c true and determines the
     *              line segment even if it is inifinitely small, i.e. a point.
     *
     * The common line segment will be left in this instance.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations. 
     */
    inline bool getCommonLine(const line<T>& first, const line<T>& second);

    /**
     * Check if this infinitely extrapolated line intersects the \a other given
     * infinite line at a single finite point.
     *
     * @param other the other line segment to which an intersection is
     *              going to be checked.
     *
     * @return true if both inifinite lines intersect at a single finite point.
     *            
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    bool doesPointIntersectXPol(const line<T>& other) const;
    
    /**
     * Compute the intersection point of this infinitely extrapolated line with the 
     * \a other given infinite line.
     *
     * @param other the other line segment to which the intersection point
     *              is going to be computed.
     * @param p     if there is an intersection between both line segments
     *              or between their respective infinite line extrapolations,
     *              the intersection point will be written here.
     * @param onThisLine if the intersection occurs at a point on the line
     *              segment, this parameter will be set to true.  Otherwise
     *              false.
     * @param onOtherLine if the intersection occurs at a point on the other
     *              line segment, this parameter will be set to true.
     * @param colinear this parameter is set to true in case both line segments
     *              are parallel and co-linear.
     *
     * @return true if an unique intersection point exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c false.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    template<class U>
    bool getIntersectionPointXPol(const line<T>& other, point<U>& p,
				  bool& onThisLine, bool& onOtherLine,
				  bool& colinear) const;

    /**
     * Compute the intersection point of this infinitely extrapolated line with the 
     * \a other given infinite line.
     *
     * @param other the other line segment to which the intersection point
     *              is going to be computed.
     * @param p     if there is an intersection between both line segments
     *              or between their respective infinite line extrapolations,
     *              the intersection point will be written here.
     *
     * @return true if an unique intersection point exists. It returns 
     *              \c false otherwise.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    template<class U>
    bool getIntersectionPointXPol(const line<T>& other, point<U>& p) const;

    /**
     * Compute the part of the infinite extrapolated line containing this line
     * segment which lies within the given rectangle, and leave the result
     * here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    bool intersectXPol(const rectangle<T>& rect);

    /**
     * Compute the part of the infinite extrapolated line containing the \a
     * other line segment which lies within the given rectangle, and leave the
     * result here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    inline bool intersectXPol(const line<T>& other,
                              const rectangle<T>& rect);
    //@}

    /**
     * @name Scaling and Translation operations
     */
    //@{
    
    /**
     * scale this line by the given \a c factor.
     */
    template<class U>
    inline line<T>& scale(const U c);

    /**
     * create a new line equal this one scaled by the given \a c factor.
     */
    template<class U>
    inline line<T> operator*(const U c) const;

    /**
     * scale this line by the given \a c factor.
     */
    template<class U>
    inline line<T>& operator*=(const U c);

    /**
     * divide both points by the given \a c factor
     */
    template<class U>
    inline line<T>& divide(const U c);

    /**
     * divide both points by the given \a c factor
     */
    template <class U>
    inline line<T> operator/(const U c) const;

    /**
     * divide both points of line<T> by a given factor
     */
    template <class U>
    inline line<T>& operator/=(const U c);

    /**
     * add given point to both ends of this line and leave the result here.
     * @param p the other line to be added to this one
     * @return a reference to this line
     */
    inline line<T>& translate(const point<T>& p);

    /**
     * add given point to both ends of the \a other line and leave the
     * result here.  
     * @param other the other line to be tranlated
     * @param p the translation factor
     * @return a reference to this line
     */
    inline line<T>& translate(const line<T>& other,const point<T>& p);

    /**
     * Compute the orthogonal line and leave the result here.
     *
     * @param offset the offset to the point on the line, where the orthogonal
     *               shall start. This parameter is scaled by the length of the line.
     * @return a reference to this line
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    line<T>& getOrthogonal(double offset);
    
    /**
     * Compute the orthogonal line to the other line and leave the result here.
     *
     * @param other  the line segment of which the orthogonal line
     *               is going to be computed.
     * @param offset the offset to the point on the line, where the orthogonal
     *               shall start. This parameter is scaled by the length of the line.
     * @return a reference to this line
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    inline line<T>& getOrthogonal(const line<T>& other, double offset);
  
    //@}

    /**
     * copy operator
     */
    inline line<T>& copy(const line<T>& other);

    /**
     * operator =
     */
    inline line<T>& operator=(const line<T>& other) {return copy(other);};

    /**
     * operator ==
     */
    inline bool operator==(const line<T>& other) const;

    /**
     * operator !=
     */
    inline bool operator!=(const line<T>& other) const;
  };


  // ----------------------------------------------------------------------
  // Type definitions
  // ----------------------------------------------------------------------

  /**
   * A line with integer coordinates
   */
  typedef line<int> iline;

  /**
   * A line with double coordinates
   */
  typedef line<double> dline;

  /**
   * A line with float coordinates
   */
  typedef line<float> fline;

  // ---------------------------------------------------
  // Storable interface
  // ---------------------------------------------------

  /**
   * read the vector from the given ioHandler. The complete flag indicates
   * if the enclosing begin and end should be also be read
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,line<T>& l, const bool complete=true);
  
  /**
   * write the vector in the given ioHandler. The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<class T>
  bool write(ioHandler& handler,const line<T>& l,const bool complete=true);

} // namespace lti

namespace std {

  template <class T>
  ostream& operator<<(ostream& s,const lti::line<T>& l);
  
  template <class T>
  istream& operator>>(istream& s,lti::line<T>& l);

} // namespace std

// implementation of inline methods
#include "ltiLine_inline.h"

#endif
