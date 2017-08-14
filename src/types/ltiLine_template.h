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
 * \file    ltiLine_template.h
 * \author Claudia Goenner
 * \date    07.02.2003
 * 
 * $Id: ltiLine_template.h,v 1.6 2012-09-16 05:05:59 alvarado Exp $
 */

#include "ltiRound.h"

using std::cout;
using std::endl;

namespace lti {

  // -------------------------------------------------------------------------
  // implementation
  // -------------------------------------------------------------------------

  // constructors

  template<class T>
  line<T>::line() : start(),end() {}

  template<class T>
  line<T>::line(const point<T>& theStart, const point<T>& theEnd)
    : start(theStart),end(theEnd) {}

  // --------------------------------
  // distance computation
  // --------------------------------

  template <class T>
  T line<T>::distanceSqr(const point<T>& c,point<T>& p) const {
    const point<T> d1=end-start;
    const point<T> d0=c-start;
    
    const T d10 = d1.dot(d0);
    
    if (d10 < 0) {
      p.copy(start);
      return p.distanceSqr(c);
    }
    
    const T d11 = d1.dot(d1);
    if (d10 > d11) {
      p.copy(end);
      return p.distanceSqr(c);
    } else {
      if (d11 > 0) {
        const float f = static_cast<float>(d10)/static_cast<float>(d11);
        p.x = cround_cast<T>(start.x+f*d1.x); // condRoundCastTo(start.x+f*d1.x,p.x );
        p.y = cround_cast<T>(start.y+f*d1.y); // condRoundCastTo(start.y+f*d1.y,p.y );
      } else {
        p.copy(c);
        return T(0);
      }
      
      return p.distanceSqr(c);
    }
    
  }

  template <class T>
  T line<T>::distanceSqrXPol(const point<T>& c,point<T>& p) const {
    const point<T> d1=end-start;
    const point<T> d0=c-start;
    
    const T d10 = d1.dot(d0);
    const T d11 = d1.dot(d1); // d11 is always zero or positive
    if (d11 > 0) {
      const float f = static_cast<float>(d10)/static_cast<float>(d11);        
      p.x = cround_cast<T>(start.x+f*d1.x); // condRoundCastTo(start.x+f*d1.x,p.x );
      p.y = cround_cast<T>(start.y+f*d1.y); // condRoundCastTo(start.y+f*d1.y,p.y );
    } else {
      // d11 is zero only if the line segment has length zero!
      p.copy(start);
    }
    
    return p.distanceSqr(c);     
  }

  // ---------------------------------------------
  // intersections
  // ---------------------------------------------

  template <class T>
  bool line<T>::doesIntersect(const line<T>& other) const {

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
      return ( ((t1>=static_cast<T>(0)) && (det >= t1)) &&
               ((t2>=static_cast<T>(0)) && (det >= t2)) );
    } else if (det < -epsilon) {
      return ( ((t1<=static_cast<T>(0)) && (det <= t1)) &&
               ((t2<=static_cast<T>(0)) && (det <= t2)) );
    } else {
      // lines are parallel, we just need to check, if the distance between
      // them is zero (in which case they may overlap in more than one point)
      if ((abs(t1) <= epsilon) && (abs(t2) <= epsilon)) {
        // lines are colinear, but do they overlap? in latter case they
        // intersect!
        if (d1.x > epsilon) {
          return (((start.x <= other.start.x) && (end.x >= other.start.x)) ||
                  ((start.x <= other.end.x)   && (end.x >= other.end.x)));
        } else if (d1.x < -epsilon) {
          return (((end.x <= other.start.x) && (start.x >= other.start.x)) ||
                  ((end.x <= other.end.x)   && (start.x >= other.end.x)));
        } else if (d1.y > epsilon) {
          return (((start.y <= other.start.y) && (end.y >= other.start.y)) ||
                  ((start.y <= other.end.y)   && (end.y >= other.end.y)));
        } else {
          return (((end.y <= other.start.y) && (start.y >= other.start.y)) ||
                  ((end.y <= other.end.y)   && (start.y >= other.end.y)));
        }          
      } else {
        return false;
      }

    }
  }

  template <class T>
  bool line<T>::isParallel(const line<T>& other) const {

    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);
 
    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;

    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();
    
    // the numerical tolerance for parallelism is limited by epsilon.
    if (det <= epsilon && det >= -epsilon)
      return true;
    
    return false;
  }

  template <class T>
  bool line<T>::isColinear(const line<T>& other) const {

    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);
    const point<T> d0(other.start-start);

    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;

    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();

    // the numerical tolerance for parallelism is limited by epsilon.
    if (det <= epsilon && det >= -epsilon) {

      // lines are parallel, we just need to check, if the distance between
      // them is zero (in which case they may overlap in more than one point)
      const T t1 = d2.x*d0.y - d2.y*d0.x;
      const T t2 = d1.x*d0.y - d1.y*d0.x;
 
      if ((abs(t1) <= epsilon) && (abs(t2) <= epsilon)) {
        // lines are colinear
	return true;
      }
    }
    return false;
  }

  template <class T>
  bool line<T>::intersect(const rectangle<T>& rect) {
    line<T> xpol(*this);
    if (xpol.intersectXPol(rect)) {
      
      if (start.x > end.x) {
        if ((end.x > xpol.start.x) || (start.x < xpol.end.x)){
          return false;
        }
        if (start.x > xpol.start.x) {
          start = xpol.start;
        }
        if (end.x < xpol.end.x) {
          end = xpol.end;
        }
        
      } else {
        if ((start.x > xpol.end.x) || (end.x < xpol.start.x)) {
          return false;
        }
        if (start.x < xpol.start.x) {
          start = xpol.start;
        }
        if (end.x > xpol.end.x) {
          end = xpol.end;
        }
      }

      if (start.y > end.y) {
        if ((end.y > xpol.start.y)  || (start.y < xpol.end.y)) {
          return false;
        }
        if (start.y > xpol.start.y) {
          start = xpol.start;
        }
        if (end.y < xpol.end.y) {
          end = xpol.end;
        }
        
      } else {
        if ((start.y > xpol.end.y) || (end.y < xpol.start.y)) {
          return false;
        }
        if (start.y < xpol.start.y) {
          start = xpol.start;
        }
        if (end.y > xpol.end.y) {
          end = xpol.end;
        }
      }
      
      return true;
    }

    return false;
  }
  
  

  template <class T>
  bool line<T>::getCommonLine(const line<T>& other) {
    // NOTE:
    // This method requires following time constants: (ToDo: update, wrong currently)
    // Additions/LogicOp : 13
    // Multiplications   : 8 
    // Divisions         : 2    
    // Comparisons       : 6~   
    
    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);

    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;
 
    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();

    // the numerical tolerance for parallelism is limited by epsilon.
    if ( det <= epsilon && det >= -epsilon) {

      // lines are parallel, we just need to check, if the distance between
      // them is zero (in which case they may overlap in more than one point)
      const point<T> d0(other.start-start);
      const T t1 = d2.x*d0.y - d2.y*d0.x;
      const T t2 = d1.x*d0.y - d1.y*d0.x;

      if ( (abs(t1) <= epsilon) && (abs(t2) <= epsilon) ) {
        // lines are colinear, but do they overlap? in latter case they
        // intersect!
        if (d1.x > epsilon) {
	  if ( (start.x <= other.start.x) && (end.x >= other.start.x) ) {
	    if ( d2.x > epsilon ) {
	      start = other.start;
	      if ( end.x > other.end.x ) end = other.end;
	    } else {
	      end = other.start;
	      if ( start.x < other.end.x ) start = other.end;
	    }
	    return true;

	  } else if ( (start.x <= other.end.x)   && (end.x >= other.end.x) ) {
	    if ( d2.x > epsilon ) {
	      end = other.end;
	    } else {
	      start = other.end;
	    }
	    return true;
	  }
	  return false;
	  
        } else if (d1.x < -epsilon) {
	  if ( (end.x <= other.start.x) && (start.x >= other.start.x) ) {
	    if ( d2.x < epsilon ) {
	      start = other.start;
	      if ( end.x < other.end.x ) end = other.end;
	    } else {
	      end = other.start;
	      if ( start.x > other.end.x ) start = other.end;
	    }
	    return true;

	  } else if ( (end.x <= other.end.x)   && (start.x >= other.end.x) ) {
	    if ( d2.x < epsilon ) {
	      end = other.end;
	    } else {
	      start = other.end;
	    }
	    return true;
	  }
	  return false;

	} else if (d1.y > epsilon) {
	  if ( (start.y <= other.start.y) && (end.y >= other.start.y) ) {
	    if ( d2.y > epsilon ) {
	      start = other.start;
	      if ( end.y > other.end.y ) end = other.end;
	    } else {
	      end = other.start;
	      if ( start.y < other.end.y ) start = other.end;
	    }
	    return true;

	  } else if ( (start.y <= other.end.y) && (end.y >= other.end.y) ) {
	    if ( d2.y > epsilon ) {
	      end = other.end;
	    } else {
	      start = other.end;
	    }
	    return true;
	  }

	  return false;

        } else {
	  if ( (end.y <= other.start.y) && (start.y >= other.start.y) ) {
	    if ( d2.y < epsilon ) {
	      start = other.start;
	      if ( end.y < other.end.y ) end = other.end;
	    } else {
	      end = other.start;
	      if ( start.y > other.end.y ) start = other.end;
	    }
	    return true;

	  } else if ( (end.y <= other.end.y)   && (start.y >= other.end.y) ) {
	    if ( d2.y < epsilon ) {
	      end = other.end;
	    } else {
	      start = other.end;
	    }
	    return true;
	  }
	  return false;
        }
      }
    }
    
    return false;
  }

  template <class T>
  bool line<T>::doesPointIntersectXPol(const line<T>& other) const {
    
    // due to the template argument point<U>, this method must be here
    // otherwise MS Visual C++ won't compile this file!
 
    // NOTE:
    // This method requires following time constants: (ToDo: count again)
    // Additions/LogicOp : 7
    // Multiplications   : 6
    // Divisions         : 1
    // Comparisons       : 2~
    
    const point<T> d1(end-start);
    const point<T> d2(other.end-other.start);
 
    // determinant
    const T det = d1.y*d2.x - d1.x*d2.y;
  
    // just one copy for the whole history of this class (faster!)
    static const T epsilon = std::numeric_limits<T>::epsilon();
    
    // the numerical tolerance for parallelism is limited by epsilon.
    if (det > epsilon || det < -epsilon) {
      return true;
    }
    
    return false;      
  }

  
  template <class T>
  bool line<T>::intersectXPol(const rectangle<T>& rect) {

    const point<T> d1(end - start);
    
    // is a vertical line?
    if (d1.x == static_cast<T>(0)) {
      if ((start.x >= rect.ul.x) && (start.x <= rect.br.x)) {
        // keep the line direction
        if (start.y>end.y) {
          start.y=rect.br.y;
          end.y  =rect.ul.y;
        } else {
          start.y=rect.ul.y;
          end.y  =rect.br.y;
        }
        return true;
      } else {
        return false;
      }
    }

    // is an horizontal line?
    if (d1.y == static_cast<T>(0)) {
      if ((start.y >= rect.ul.y) && (start.y <= rect.br.y)) {
        // keep the line direction
        if (start.x>end.x) {
          start.x=rect.br.x;
          end.x  =rect.ul.x;
        } else {
          start.x=rect.ul.x;
          end.x  =rect.br.x;
        }
        return true;
      } else {
        return false;
      }
    }

    if (abs(d1.x) >= abs(d1.y)) {
      // lines between -45 degrees and +45 degrees
      // working with equations y = m*x + b
      const float m(static_cast<float>(d1.y)/static_cast<float>(d1.x));
      const float b(static_cast<float>(start.y)-static_cast<float>(m*start.x));

      if (start.x > end.x) {
        end.x=rect.ul.x;
        start.x=rect.br.x;

        end.y = cround_cast<T>(m*end.x+b); // condRoundCastTo(m*end.x+b,end.y);
        if (end.y > rect.br.y) {
          end.y=rect.br.y;
          end.x = cround_cast<T>((end.y-b)/m); // condRoundCastTo((end.y-b)/m,end.x);
          if ((end.x<rect.ul.x) || (end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (end.y < rect.ul.y) {
          end.y=rect.ul.y;
          end.x = cround_cast<T>((end.y-b)/m); // condRoundCastTo((end.y-b)/m,end.x);
          if ((end.x<rect.ul.x) || (end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }

        start.y = cround_cast<T>(m*start.x+b); // condRoundCastTo(m*start.x+b,start.y);
        if (start.y > rect.br.y) {
          start.y=rect.br.y;
          start.x = cround_cast<T>((start.y-b)/m); // condRoundCastTo((start.y-b)/m,start.x);
          if ((start.x<rect.ul.x) || (start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (start.y < rect.ul.y) {
          start.y=rect.ul.y;
          start.x = cround_cast<T>((start.y-b)/m); // condRoundCastTo((start.y-b)/m,start.x);
          if ((start.x<rect.ul.x) || (start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }
        
      } else {
        // for start.x < end.x
        start.x=rect.ul.x;
        end.x=rect.br.x;

        start.y = cround_cast<T>(m*start.x+b); // condRoundCastTo(m*start.x+b,start.y);
        if (start.y > rect.br.y) {
          start.y=rect.br.y;
          start.x = cround_cast<T>((start.y-b)/m); // condRoundCastTo((start.y-b)/m,start.x);
          if ((start.x<rect.ul.x) || (start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (start.y < rect.ul.y) {
          start.y=rect.ul.y;
          start.x = cround_cast<T>((start.y-b)/m); // condRoundCastTo((start.y-b)/m,start.x);
          if ((start.x<rect.ul.x) || (start.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }

        end.y = cround_cast<T>(m*end.x+b); // condRoundCastTo(m*end.x+b,end.y);
        if (end.y > rect.br.y) {
          end.y=rect.br.y;
          end.x = cround_cast<T>((end.y-b)/m); // condRoundCastTo((end.y-b)/m,end.x);
          if ((end.x<rect.ul.x) || (end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        } else if (end.y < rect.ul.y) {
          end.y=rect.ul.y;
          end.x = cround_cast<T>((end.y-b)/m); // condRoundCastTo((end.y-b)/m,end.x);
          if ((end.x<rect.ul.x) || (end.x>rect.br.x)) {
            // do not intersect
            return false;
          }
        }
      }

    } else {
      // lines between 45 degrees and 135 degrees
      // working with equations x = m*y + b
      const float m(static_cast<float>(d1.x)/static_cast<float>(d1.y));
      const float b(static_cast<float>(start.x)-static_cast<float>(m*start.y));

      if (start.y > end.y) {
        end.y=rect.ul.y;
        start.y=rect.br.y;

        end.x = cround_cast<T>(m*end.y+b); // condRoundCastTo(m*end.y+b,end.x);
        if (end.x > rect.br.x) {
          end.x=rect.br.x;
          end.y = cround_cast<T>((end.x-b)/m); // condRoundCastTo((end.x-b)/m,end.y);
          if ((end.y<rect.ul.y) || (end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (end.x < rect.ul.x) {
          end.x=rect.ul.x;
          end.y = cround_cast<T>((end.x-b)/m); // condRoundCastTo((end.x-b)/m,end.y);
          if ((end.y<rect.ul.y) || (end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }

        start.x = cround_cast<T>(m*start.y+b); // condRoundCastTo(m*start.y+b,start.x);
        if (start.x > rect.br.x) {
          start.x=rect.br.x;
          start.y = cround_cast<T>((start.x-b)/m); // condRoundCastTo((start.x-b)/m,start.y);
          if ((start.y<rect.ul.y) || (start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (start.x < rect.ul.x) {
          start.x=rect.ul.x;
          start.y = cround_cast<T>((start.x-b)/m); // condRoundCastTo((start.x-b)/m,start.y);
          if ((start.y<rect.ul.y) || (start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }
        
      } else {
        // for start.y < end.y
        start.y=rect.ul.y;
        end.y=rect.br.y;

        start.x = cround_cast<T>(m*start.y+b); // condRoundCastTo(m*start.y+b,start.x);
        if (start.x > rect.br.x) {
          start.x=rect.br.x;
          start.y = cround_cast<T>((start.x-b)/m); // condRoundCastTo((start.x-b)/m,start.y);
          if ((start.y<rect.ul.y) || (start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (start.x < rect.ul.x) {
          start.x=rect.ul.x;
          start.y = cround_cast<T>((start.x-b)/m); // condRoundCastTo((start.x-b)/m,start.y);
          if ((start.y<rect.ul.y) || (start.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }

        end.x = cround_cast<T>(m*end.y+b); // condRoundCastTo(m*end.y+b,end.x);
        if (end.x > rect.br.x) {
          end.x=rect.br.x;
          end.y = cround_cast<T>((end.x-b)/m); // condRoundCastTo((end.x-b)/m,end.y);
          if ((end.y<rect.ul.y) || (end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        } else if (end.x < rect.ul.x) {
          end.x=rect.ul.x;
          end.y = cround_cast<T>((end.x-b)/m); // condRoundCastTo((end.x-b)/m,end.y);
          if ((end.y<rect.ul.y) || (end.y>rect.br.y)) {
            // do not intersect
            return false;
          }
        }
      }
    }

    return true;
  }

  // ---------------------------------------------------
  // get the orthogonal line
  // ---------------------------------------------------

  template <class T>
  line<T>& line<T>::getOrthogonal(double offset) {
    
    const point<T> d1 = end - start;
    start.x = cround_cast<T>(start.x + d1.x * offset); // condRoundCastTo(start.x + d1.x * offset, start.x);
    start.y = cround_cast<T>(start.y + d1.y * offset); // condRoundCastTo(start.y + d1.y * offset, start.y);

    end.x = start.x + d1.y;
    end.y = start.y - d1.x;

    return (*this);
  }

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
  bool read(ioHandler& handler,line<T>& l, const bool complete) {
    bool success(true);

    if (complete) {
      success = success && handler.readBegin();
    }

    point<T> a,b;
    success = success && lti::read(handler,a);
    success = success && handler.readDataSeparator();
    success = success && lti::read(handler,b);
    l.set(a,b);

    if (complete) {
      success = success && handler.readEnd();
    }

    return success;
  }
  
  /**
   * write the vector in the given ioHandler. The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<class T>
  bool write(ioHandler& handler,const line<T>& l,const bool complete) {
    bool success(true);
    
    if (complete) {
      success = success && handler.writeBegin();
    }
    
    success = success && lti::write(handler,l.getStart());
    success = success && handler.writeDataSeparator();
    success = success && lti::write(handler,l.getEnd());

    if (complete) {
      success = success && handler.writeEnd();
    }

    return success;
  }


}//namespace lti

namespace std {

  template <class T>
  inline ostream& operator<<(ostream& s,const lti::line<T>& l) {
 
    s << "(" 
      << l.getStart() << "," 
      << l.getEnd() 
      << ")";
 
    return s;
  }
  
  template <class T>
  inline istream& operator>>(istream& s,lti::line<T>& l) {
    char c;
    lti::point<T> a,b;
    s >> c 
      >> a >> c 
      >> b >> c;

    l.set(a,b);
    return s;
  }

}

