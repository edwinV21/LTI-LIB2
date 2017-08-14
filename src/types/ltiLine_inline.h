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
 * \file   ltiLine_template.h
 * \author Claudia Goenner
 * \date   07.02.2003
 * 
 * $Id: ltiLine_inline.h,v 1.4 2009-06-03 20:51:31 alvarado Exp $
 */

#include "ltiRound.h"

namespace lti {

  // -------------------------------------------------------------------------
  // implementation
  // -------------------------------------------------------------------------

  /**
   * copy constructor
   */
  template<class T>
  template<class U> 
  inline line<T>::line(const line<U>& other) 
    : start(castFrom(other.start)),end(castFrom(other.end)) {
  }

  /**
   * cast operator
   */
  template<class T>
  template<class U>
  inline line<T>& line<T>::castFrom(const line<U>& other) {
    start.castFrom(other.start);
    end.castFrom(other.end);
    return (*this);
  }

  template<class T>
  inline void line<T>::set(const point<T>& theStart,const point<T>& theEnd){
    start=theStart;
    end=theEnd;
  }

  template<class T>
  inline void line<T>::setStart(const point<T>& theStart) {
    start = theStart;
  }

  template<class T>
  inline void line<T>::setEnd(const point<T>& theEnd) {
    end = theEnd;
  }

  template <class T>
  inline void line<T>::invert() {
    point<T> tmp(start);
    start=end;
    end=tmp;
  }

  template <class T>
  inline const point<T>& line<T>::getStart() const {
    return start;
  }

  template <class T>
  inline const point<T>& line<T>::getEnd() const {
    return end;
  }

  // --------------------------------
  // distance computation
  // --------------------------------

  template <class T>
  inline T line<T>::distanceTo(const point<T>& c) const {
    return sqrt(distanceSqr(c));
  }

  template <class T>
  inline T line<T>::distanceSqr(const point<T>& c) const {
    point<T> dummy;
    return distanceSqr(c,dummy);
  }

  template <class T>
  inline T line<T>::distanceToXPol(const point<T>& c) const {
    return sqrt(distanceSqrXPol(c));
  }

  template <class T>
  inline T line<T>::distanceSqrXPol(const point<T>& c) const {
    point<T> dummy;
    return distanceSqrXPol(c,dummy);
  }

  template <class T>
  inline T line<T>::sqrLength() const {
    return start.distanceSqr(end);
  }

  // ---------------------------------------------
  // intersections
  // ---------------------------------------------

  template <class T>
  inline bool line<T>::intersect(const line<T>& other,
                                  const rectangle<T>& rect) {
    copy(other);
    return intersect(rect);
  }

  template <class T>
  inline bool line<T>::getCommonLine(const line<T>& first, const line<T>& second) {
    copy (first);
    return getCommonLine(second);
  }

  template <class T>
  inline bool line<T>::intersectXPol(const line<T>& other,
                                      const rectangle<T>& rect) {
    copy(other);
    return intersectXPol(rect);
  }

  // ----------------------------------
  // implementation of shift operations
  // ---------------------------------- 

  /**
   * scale this line by the given \a c factor.
   */
  template<class T>
  template<class U>
  inline line<T>& line<T>::scale(const U c) {
    start.multiply(c);
    end.multiply(c);
    return *this;
  }

  /**
   * create a new line equal this one scaled by the given \a c factor.
   */
  template<class T>
  template<class U>
  inline line<T> line<T>::operator*(const U c) const {
    return line<T>(start*c,end*c);
  }
  
  /**
   * scale this line by the given \a c factor.
   */
  template<class T>
  template<class U>
  inline line<T>& line<T>::operator*=(const U c) {
    return multiply(c);
  }
  
  /**
   * divide both points by the given \a c factor
   */
  template<class T>
  template<class U>
  inline line<T>& line<T>::divide(const U c) {
    start.divide(c);
    end.divide(c);
    return *this;      
  }
  
  /**
   * divide both points by the given \a c factor
   */
  template<class T>
  template<class U>
  inline line<T> line<T>::operator/(const U c) const {
    return line<T>(start/c,end/c);
  }
  
  /**
   * divide both points of line<T> by a given factor
   */
  template<class T>
  template<class U>
  inline line<T>& line<T>::operator/=(const U c) {
    return divide(c);  
  }
  
  
  template <class T>
  inline line<T>& line<T>::translate(const point<T>& p) {
    start+=p;
    end+=p;
    return (*this);
  }
  
  template <class T>
  inline line<T>& line<T>::translate(const line<T>& other,
                                     const point<T>& p) {
    start=other.start+p;
    end  =other.end+p;
    return (*this);
  }
  
  template <class T>
  inline line<T>& line<T>::copy(const line<T>& other) {
    start = other.start;
    end = other.end;
    return (*this);
  } 
  
  template <class T>
  inline bool line<T>::operator==(const line<T>& other) const {
    return ((end == other.end) && (start == other.start));
  }

  template <class T>
  inline bool line<T>::operator!=(const line<T>& other) const {
    return ((end != other.end) || (start != other.start));
  }

  template <class T>
  inline line<T>& line<T>::getOrthogonal(const line<T>& other, double offset) {
    copy (other);
    return getOrthogonal(offset);
  }

  // The template template need an implementation visible to all users!
  template<class T>
  template<class U>
  bool line<T>::getIntersectionPoint(const line<T>& other, point<U>& p,
                                     bool& colinear ) const
  {
    // due to the template argument point<U>, this method must be here
    // otherwise MS Visual C++ won't compile this file!
    colinear = false;
    
    // NOTE:
    // This method requires following time constants: (out of date -> ToDo)
    // Additions/LogicOp : 13
    // Multiplications   : 8 
    // Divisions         : 2    
    // Comparisons       : 6~   
    
    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);
    const point<T> d0(other.start-start);
    
    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;
    
    // auxiliar variables containing information of the intersection point
    const T t1 = d2.x*d0.y - d2.y*d0.x;
    const T t2 = d1.x*d0.y - d1.y*d0.x;
    
    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();
    
    // the numerical tolerance for parallelism is limited by epsilon.
    if (det > epsilon) {
      //check overlap
      if ( (t1 < 0) || (det < t1) || (t2 < 0) || (det < t2) )
        return false;
      
      // compute the intersection point
      const double f = static_cast<double>(t1)/static_cast<double>(det);
      p.set(cround_cast<U>(start.x + f*d1.x),
	    cround_cast<U>(start.y + f*d1.y));
      return true;
      
    } else if (det < -epsilon) {
      if ( (t1 > 0) || (det > t1) || (t2 > 0) || (det > t2) )
        return false;
      
      // compute the intersection point
      const double f = static_cast<double>(t1)/static_cast<double>(det);
      p.set(cround_cast<U>(start.x + f*d1.x),
	    cround_cast<U>(start.y + f*d1.y));
      return true;
      
    } else {
      // lines are parallel, we just need to check, if the distance between
      // them is zero (in which case they may overlap in more than one point)
      colinear   = (abs(t1) <= epsilon) && (abs(t2) <= epsilon);
      
      if (colinear) {
        // lines are colinear, but do they overlap? in latter case they
        // intersect! also check if they overlap is at a single point
        if ( end.x <= other.start.x + epsilon &&
             end.x >= other.start.x - epsilon ) {
          p = end;
          return true;
        }
        else if ( end.x <= other.end.x + epsilon &&
                  end.x >= other.end.x - epsilon ) {
          p = end;
          return true;
        }
        else if ( start.x <= other.start.x + epsilon &&
                  start.x >= other.start.x - epsilon ) {
          p = start;
          return true;
        }
        else if ( start.x <= other.end.x + epsilon &&
                  start.x >= other.end.x - epsilon ) {
          p = start;
          return true;
        }
      }
    }
    return false;
  }

  template<class T>
  template<class U>
  bool line<T>::getIntersectionPointXPol(const line<T>& other, point<U>& p,
                                         bool& onThisLine, bool& onOtherLine,
                                         bool& colinear) const {
    
    // due to the template argument point<U>, this method must be here
    // otherwise MS Visual C++ won't compile this file!
    colinear = false; 
    
    // NOTE:
    // This method requires following time constants:
    // Additions/LogicOp : 13
    // Multiplications   : 8 
    // Divisions         : 2    
    // Comparisons       : 6~   
    
    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);
    const point<T> d0(other.start-start);
    
    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;
    
    // auxiliar variables containing information of the intersection point
    const T t1 = d2.x*d0.y - d2.y*d0.x;
    const T t2 = d1.x*d0.y - d1.y*d0.x;
    
    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();
    
    // the numerical tolerance for parallelism is limited by epsilon.
    if (det > epsilon) {
      // compute the intersection point on the extrapolated lines
      const double f = static_cast<double>(t1)/static_cast<double>(det);
      p.set(cround_cast<U>(start.x + f*d1.x),
	    cround_cast<U>(start.y + f*d1.y));
      
      onThisLine  = ((t1>=0) && (det >= t1));
      onOtherLine = ((t2>=0) && (det >= t2));
      
      return true;
    } else if (det < -epsilon) {
      // compute the intersection point on the extrapolated lines
      const double f = static_cast<double>(t1)/static_cast<double>(det);
      p.set(cround_cast<U>(start.x + f*d1.x),
	    cround_cast<U>(start.y + f*d1.y));
      
      onThisLine  = ((t1<=0) && (det <= t1));
      onOtherLine = ((t2<=0) && (det <= t2));
      
      return true;
    } else {
      // lines are parallel, we just need to check, if the distance between
      // them is zero (in which case they may overlap in more than one point)
      colinear = (abs(t1) <= epsilon) && (abs(t2) <= epsilon);
      
      if (colinear) {
        // lines are colinear, but do they overlap? in latter case they
        // intersect! But we only return true, if a single inersection point exists
        if (d1.x > epsilon) {	  
          onThisLine = onOtherLine
            = ((start.x <= other.start.x) && (end.x >= other.start.x)) ||
            ((start.x <= other.end.x)   && (end.x >= other.end.x));
        } else if (d1.x < -epsilon) {
          onThisLine = onOtherLine
            = ((end.x <= other.start.x) && (start.x >= other.start.x)) ||
            ((end.x <= other.end.x)   && (start.x >= other.end.x));
        } else if (d1.y > epsilon) {
          onThisLine = onOtherLine
            = ((start.y <= other.start.y) && (end.y >= other.start.y)) ||
            ((start.y <= other.end.y)   && (end.y >= other.end.y));
        } else {
          onThisLine = onOtherLine
            = ((end.y <= other.start.y) && (start.y >= other.start.y)) ||
            ((end.y <= other.end.y)   && (start.y >= other.end.y));
        }
      }
    }
    
    return false;
  }
  
  template<class T>
  template<class U>
  bool line<T>::getIntersectionPointXPol(const line<T>& other, point<U>& p) const {
    
    // due to the template argument point<U>, this method must be here
    // otherwise MS Visual C++ won't compile this file!
    
    // NOTE:
    // This method requires following time constants:
    // Additions/LogicOp : 7
    // Multiplications   : 6
    // Divisions         : 1
    // Comparisons       : 2~
    
    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);
    
    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;
    //cout << " det " << det << " d1 " << d1 << " d2 " << d2;
    
    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();
    
    // the numerical tolerance for parallelism is limited by epsilon.
    if (det > epsilon || det < -epsilon) {
      // compute the intersection point on the extrapolated lines
      const point<T> d0(other.start-start);
      const T t1 = d2.x*d0.y - d2.y*d0.x;
      const double f = static_cast<double>(t1)/static_cast<double>(det);
      //cout << " f " << f;
      p.set(cround_cast<U>(start.x + f*d1.x),
	    cround_cast<U>(start.y + f*d1.y));
      
      return true;
    }
    
    return false;      
  }


} // namespace lti
