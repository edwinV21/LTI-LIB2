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

/*
 * \file   ltiRectangle_template.h
 *         Contains template class for aligned rectangles, which are described
 *         through two points.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRectangle_template.h,v 1.4 2013-05-27 00:26:11 alvarado Exp $
 */

namespace lti {

  template<typename T,class S>
  rectangle<T,S>::rectangle()
    : ul(point<T>(0,0)),br(point<T>(0,0)) {
  }

  template<typename T,class S>
  rectangle<T,S>::rectangle(const point<T>& p1,const point<T>& p2)
    : ul(p1), br(p2) {
  }

  template<typename T,class S>
  rectangle<T,S>::rectangle(const T& x1,const T& y1,
                            const T& x2,const T& y2)
    : ul(point<T>(x1,y1)),br(point<T>(x2,y2)) {
  }

  template<typename T,class S>
  rectangle<T,S>::rectangle(const T& size) {
    T low,high;
    side_type::invSize(size,low,high);
    ul.set(low,low);
    br.set(high,high);
  }

  template<typename T,class S>
  template<typename U,class US>
  rectangle<T,S>::rectangle(const rectangle<U,US>& other)
    : ul(other.ul),br(other.br) {
  }

  template<typename T,class S>
  void rectangle<T,S>::resize(const point<T>& dim) {
    point<T> cen = getCenter();
    S::invSize(dim.x,ul.x,br.x);
    S::invSize(dim.y,ul.y,br.y);
    ul.add(cen);
    br.add(cen);
  }

  template<typename T,class S>
  void rectangle<T,S>::resize(const T& x, const T& y) {
    point<T> cen = getCenter();
    S::invSize(x,ul.x,br.x);
    S::invSize(y,ul.y,br.y);
    ul.add(cen);
    br.add(cen);
  }

  template<typename T,class S>
  void rectangle<T,S>::setCenter(point<T> center) {
    point<T> dim(abs(br.x-ul.x),abs(br.y-ul.y));
    ul.set(center.x - dim.x/2,center.y - dim.y/2);
    br.set(ul.x + dim.x,ul.y + dim.y);
  }

  template<typename T,class S>
  void rectangle<T,S>::ensureConsistency() {
    if (ul.x > br.x) {
      swap(ul.x,br.x);
    }
    
    if (ul.y > br.y) {
      swap(ul.y,br.y);
    }
  }

  template<typename T,class S>
  rectangle<T,S>& rectangle<T,S>::copy(const rectangle<T,S>& other) {
    ul.copy(other.ul);
    br.copy(other.br);
    return (*this);
  }

  template<typename T,class S>
  template <class U,class SU>
  rectangle<T,S>& rectangle<T,S>::castFrom(const rectangle<U,SU>& other) {
    ul.castFrom(other.ul);
    br.castFrom(other.br);
    return (*this);
  }

  template<typename T,class S>
  rectangle<T,S>& rectangle<T,S>::intersect(const rectangle<T,S>& tRect) {
    ul.x=max(ul.x,tRect.ul.x);
    br.x=min(br.x,tRect.br.x);

    ul.y=max(ul.y,tRect.ul.y);
    br.y=min(br.y,tRect.br.y);

    return (*this);
  }
  
  template<typename T,class S>
  rectangle<T,S>& rectangle<T,S>::include(const point<T>& p) {
    return include(p.x,p.y);
  }

  template<typename T,class S>
  rectangle<T,S>& rectangle<T,S>::include(const T& x, const T& y) {
    if (x<ul.x) {
      ul.x = x;
    }
    if (x>br.x) {
      br.x = x;
    }

    if (y<ul.y) {
      ul.y = y;
    }
    if (y>br.y) {
      br.y = y;
    }

    return (*this);
  }


  template<typename T,class S>
  bool rectangle<T,S>::overlaps(const rectangle<T,S>& tRect) const {
    rectangle<T,S> rect(*this);
    rect.intersect(tRect);
    return rect.isConsistent();
  }

  template<typename T,class S>
  bool rectangle<T,S>::isClose(const rectangle<T,S>& tRect,
                               const point<T>& dist) const {

    const point<T> br1 ( tRect.br + dist );
    const point<T> ul1 ( tRect.ul - dist );
    
    if ( br.x >= ul1.x ) {
      if ( br.x <= br1.x ) {
        if ( br.y >= ul1.y ) {
          if ( br.y <= br1.y ) {
            return true;            
          } else if ( ul.y <= br1.y ) {
            return true;            
          } else {
            return false;
          }	    
        } else {
          return false;
        }        
      } else if ( ul.x <= br1.x ) {
        if ( ul.y >= ul1.y ) {
          if ( ul.y <= br1.y ) {
            return true;            
          } else {
            return false;
          }          
        } else if ( br.y >= ul1.y ) {
          if ( ul.y <= br1.y ) {
            return true;            
          } else {
            return false;
          }          
        } else {
          return false;
        }
      }
    }    
    return false;
  }

  template<typename T,class S>  
  rectangle<T,S>& rectangle<T,S>::join(const rectangle<T,S>& tRect) {
    if (tRect.ul.x < ul.x) {
      ul.x = tRect.ul.x;
    }
    if (tRect.ul.y < ul.y) {
      ul.y = tRect.ul.y;
    }
    if (tRect.br.x > br.x) {
      br.x = tRect.br.x;
    }
    if (tRect.br.y > br.y) {
      br.y = tRect.br.y;
    }
    return (*this);
  }

  template<typename T,class S>
  bool read(ioHandler& handler,rectangle<T,S>& p,const bool complete) {
    bool b;

    if (complete) {
      handler.readBegin();
    }

    read(handler,p.ul);
    handler.readDataSeparator();
    b = read(handler,p.br);

    if (complete) {
      b = handler.readEnd();
    }

    return b;
  }

  template <typename T,class S>
  bool write(ioHandler& handler,const rectangle<T,S>& p,const bool complete) {
    bool b;

    if (complete) {
      handler.writeBegin();
    }

    write(handler,p.ul);
    handler.writeDataSeparator();
    b = write(handler,p.br);

    if (complete) {
      b = handler.writeEnd();
    }

    return b;
  }
}

namespace std {

  template <typename T,class S>
  inline ostream& operator<<(ostream& s,const lti::rectangle<T,S>& p) {
    s << "(" 
      << p.ul << ","
      << p.br << ")";
    return s;
  }

  template <typename T,class S>
  inline istream& operator>>(istream& s,lti::rectangle<T,S>& p) {
    char c;
    lti::point<T> ul,br;
    s >> c
      >> ul >> c
      >> br >> c;
    p.ul = ul;
    p.br = br;

    return s;
  }
}

