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
 * \file   ltiRectangle_inline.h
 *         Contains template class for aligned rectangles, which are described
 *         through two points.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRectangle_inline.h,v 1.5 2009-09-23 16:24:31 alvarado Exp $
 */

namespace lti {

  template <typename T>
  inline T rectangleSide<T>::size(const T x1,const T x2) {
    return (x2-x1+static_cast<T>(1));
  }

  template <typename T>
  inline void rectangleSide<T>::invSize(const T sz,T& x1,T& x2) {
    x1=(static_cast<T>(1)-sz)/2;
    x2=x1+sz-1;
  }
  
  inline float rectangleSide<float>::size(const float x1,const float x2) {
    return (x2-x1);
  }

  inline void rectangleSide<float>::invSize(const float sz,
                                            float& x1,
                                            float& x2) {
    x1=-sz/2.0f;
    x2=x1+sz;
  }

  
  inline double rectangleSide<double>::size(const double x1,const double x2) {
    return (x2-x1);
  }

  inline void rectangleSide<double>::invSize(const double sz,
                                             double& x1,
                                             double& x2) {
    x1=-sz/2.0;
    x2=x1+sz;
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::set(const T& x1,const T& y1,
                                  const T& x2,const T& y2) {
    assert (x1<=x2);
    assert (y1<=y2);
    ul.set(x1,y1);
    br.set(x2,y2);
    return true;
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::set(const point<T>& p1,const point<T>& p2) {
    assert (p1.x<=p2.x);
    assert (p1.y<=p2.y);
    ul.copy(p1);
    br.copy(p2);
    return true;
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::setConsistent(const T& x1,const T& y1,
                                            const T& x2,const T& y2) {
    if (x1 < x2) {
      ul.x = x1;
      br.x = x2;
    } else {
      ul.x = x2;
      br.x = x1;
    }

    if (y1 < y2) {
      ul.y = y1;
      br.y = y2;
    } else {
      ul.y = y2;
      br.y = y1;
    }

    return true;
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::setConsistent(const point<T>& p1,
                                            const point<T>& p2) {
    if (p1.x < p2.x) {
      ul.x = p1.x;
      br.x = p2.x;
    } else {
      ul.x = p2.x;
      br.x = p1.x;
    }

    if (p1.y < p2.y) {
      ul.y = p1.y;
      br.y = p2.y;
    } else {
      ul.y = p2.y;
      br.y = p1.y;
    }
    return true;
  }



  template<typename T,class S>
  inline point<T> rectangle<T,S>::getDimensions() const {
    return point<T>(side_type::size(ul.x,br.x),
                    side_type::size(ul.y,br.y));
  }

  template<typename T,class S>
  inline typename typeInfo<T>::square_accumulation_type
  rectangle<T,S>::getArea() const {
    return abs(S::size(ul.x,br.x)*
               S::size(ul.y,br.y));
  }

  template<typename T,class S>
  inline point<T> rectangle<T,S>::getCenter() const {
    return point<T>((br.x+ul.x)/2,(br.y+ul.y)/2);
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::isConsistent() const {
    return !((ul.x > br.x) || (ul.y > br.y));
  }

  template<typename T,class S>
  inline void rectangle<T,S>::shift(const point<T>& delta) {
    ul.add(delta);
    br.add(delta);
  }

  template<typename T,class S>
  inline void rectangle<T,S>::shift(const rectangle<T,S>& other,
                                    const point<T>& delta) {
    ul.add(other.ul,delta);
    br.add(other.br,delta);
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::contains(const point<T>& p) const {
    return ((p.x>=ul.x) && (p.x<=br.x) &&
            (p.y>=ul.y) && (p.y<=br.y));
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::contains(const T& x, const T& y) const {
    return ((x>=ul.x) && (x<=br.x) &&
            (y>=ul.y) && (y<=br.y));
  }

  template<typename T,class S>
  inline rectangle<T,S>& 
  rectangle<T,S>::operator=(const rectangle<T,S>& other) {
    return copy(other);
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::operator==(const rectangle<T,S>& other) const {
    return ((other.ul == ul) &&
            (other.br == br));
  }

  template<typename T,class S>
  inline bool rectangle<T,S>::operator!=(const rectangle<T,S>& other) const {
    return ((other.ul != ul) ||
            (other.br != br));
  }

  template<typename T,class S>
  inline rectangle<T,S>& 
  rectangle<T,S>::operator&=(const rectangle<T,S>& tRect) {
    return intersect(tRect);
  }

  template<typename T,class S>
  inline rectangle<T,S> 
  rectangle<T,S>::operator&(const rectangle<T,S>& tRect) {
    rectangle<T,S> tempRect(*this);
    return tempRect.intersect(tRect);
  }
      
  template<typename T,class S>
  inline rectangle<T,S>& 
  rectangle<T,S>::operator|=(const rectangle<T,S>& tRect) {
    return join(tRect);
  }

  template<typename T,class S>
  inline rectangle<T,S> 
  rectangle<T,S>::operator|(const rectangle<T,S>& tRect) {
    rectangle<T,S> tempRect(*this);
    return tempRect.join(tRect);
  }

}
