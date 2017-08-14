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
 * \file   ltiPoint.h
 *         Defines the template type lti::point<T> and all shortcuts like
 *         ipoint.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiPoint_template.h,v 1.4 2007-11-03 23:50:17 alvarado Exp $
 */

namespace lti {
  
  template<typename T>
  inline typename point<T>::value_type 
  point<T>::sqr(const value_type a) const {
    return a*a;
  }

  // constructor
  template <typename T>
  point<T>::point(const T newx,const T newy) 
    : x(newx),y(newy) {
  }

  // copy constructor
  template <typename T>
  template <typename U>
  point<T>::point(const point<U>& p)
    : x(static_cast<T>(p.x)),y(static_cast<T>(p.y)) {
  }

  // cast from
  template <typename T>
  template <typename U>
  point<T>& point<T>::castFrom(const point<U>& p) {
    x = static_cast<T>(p.x);
    y = static_cast<T>(p.y);
    return (*this);
  }

  template <typename T>
  inline point<T>& point<T>::set(const T tx,const T ty) {
    x = tx;
    y = ty;
    return *this; 
  }


  template <typename T>
  inline void point<T>::get(T& tx,T& ty) const {
    tx = x;
    ty = y;
  }

  template <typename T>
  inline T point<T>::distanceSqr(const point<T>& c) const {
    return (sqr(x-c.x)+sqr(y-c.y));
  }

  template <typename T>
  inline T point<T>::absSqr() const {
    return (x*x) + (y*y);
  }

  template <typename T>
  inline T point<T>::dot(const point<T>& p) const {
    return (x*p.x) + (y*p.y);
  }

  template <typename T>
  template<typename U>
  inline point<T>& point<T>::multiply(const U c) {
    x = static_cast<T>(x*c);
    y = static_cast<T>(y*c);
    return *this;
  }

  template <typename T>
  template<typename U>
  inline point<T> point<T>::operator*(const U c) const {
    return point<T>(static_cast<T>(x*c),static_cast<T>(y*c));
  }

  template <typename T>
  template<typename U>
  inline point<T>& point<T>::multiply(const point<T>& other,const U c) {
    x = static_cast<T>(other.x*c);
    y = static_cast<T>(other.y*c);
    return *this;
  }

  template <typename T>
  template<typename U>
  inline point<T>& point<T>::operator*=(const U c) {
    x = static_cast<T>(x*c);
    y = static_cast<T>(y*c);
    return *this;
  }

  template <typename T>
  inline point<T> point<T>::operator*(const point<T>& c) const {
    return point<T>(c.x*x,c.y*y);
  }

  template <typename T>
  inline point<T>& point<T>::operator*=(const point<T>& c) {
    x*=c.x;
    y*=c.y;
    return *this;
  }

  template <typename T>
  inline point<T>& point<T>::emultiply(const point<T>& c) {
    x*=c.x;
    y*=c.y;
    return *this;
  }

  template <typename T>
  inline point<T>& point<T>::emultiply(const point<T>& a,
                                       const point<T>& b) {
    x=a.x*b.x;
    y=a.y*b.y;
    return *this;
  }
  
  template <typename T>
  template<typename U>
  inline point<T>& point<T>::divide(const U c) {
    x = static_cast<T>(x/c);
    y = static_cast<T>(y/c);
    return *this;
  }

  template <typename T>
  template<typename U>
  inline point<T>& point<T>::divide(const point<T>& other,const U c) {
    x = static_cast<T>(other.x/c);
    y = static_cast<T>(other.y/c);
    return *this;
  }

  template <typename T>
  template <typename U>
  inline point<T> point<T>::operator/(const U c) const {
    return point<T>(static_cast<T>(x/c),static_cast<T>(y/c));
  }

  template <typename T>
  template <typename U>
  inline point<T>& point<T>::operator/=(const U c) {
    x = static_cast<T>(x/c);
    y = static_cast<T>(y/c);
    return *this;
  }

  template <typename T>
  inline point<T> point<T>::operator/(const point<T>& c) const {
    return point<T>(x/c.x,y/c.y);
  }

  template <typename T>
  inline point<T>& point<T>::operator/=(const point<T>& c) {
    x/=c.x;
    y/=c.y;
    return *this;
  }

  template <typename T>
  inline point<T>& point<T>::edivide(const point<T>& c) {
    x/=c.x;
    y/=c.y;
    return *this;
  }

  template <typename T>
  inline point<T>& point<T>::edivide(const point<T>& a,
                                       const point<T>& b) {
    x=a.x/b.x;
    y=a.y/b.y;
    return *this;
  }


  template <typename T>
  inline point<T> point<T>::operator%(const int c) const {
    return point<T>(static_cast<int>(x)%c,static_cast<int>(y)%c);
  }

  template <typename T>
  inline point<T>& point<T>::add(const point<T>& p) {
    x+=p.x;
    y+=p.y;
    return (*this);
  }

  template <typename T>
  inline point<T>& point<T>::add(const point<T>& a,
                                 const point<T>& b) {
    x=a.x+b.x;
    y=a.y+b.y;

    return (*this);
  }


  template <typename T>
  inline point<T> point<T>::operator+(const point<T>& p) const {
    return point<T>(x+p.x,y+p.y);
  }

  template <typename T>
  inline point<T>& point<T>::operator+=(const point<T>& p) {
    return add(p);
  }

  template <typename T>
  inline point<T>& point<T>::subtract(const point<T>& p) {
    x-=p.x;
    y-=p.y;
    return (*this);
  }

  template <typename T>
  inline point<T>& point<T>::subtract(const point<T>& a,
                                      const point<T>& b) {
    x=a.x-b.x;
    y=a.y-b.y;

    return (*this);
  }

  template <typename T>
  inline point<T> point<T>::operator-(const point<T>& p) const {
    return point<T>(x-p.x,y-p.y);
  }

  template <typename T>
  inline point<T> point<T>::operator-() const {
    return point<T>(-x,-y);
  }

  template <typename T>
  inline point<T>& point<T>::operator-=(const point<T>& p) {
    return subtract(p);
  }

  template <typename T>
  inline point<T>& point<T>::copy(const point<T>& p) {
    x = p.x;
    y = p.y;
    return (*this);
  }

  template <typename T>
  inline point<T>& point<T>::operator=(const point<T>& p) {
    x = p.x;
    y = p.y;
    return (*this);
  }
  

  template <typename T>
  inline bool point<T>::operator==(const point<T>& p) const {
    return ((p.y == y) && (p.x == x));
  }

  template <typename T>
  inline bool point<T>::operator!=(const point<T>& p) const {
    return ((p.y != y) || (p.x != x));
  }

  template <typename T>
  inline bool point<T>::operator<(const point<T>& p) const {
    return (y < p.y) || ((y == p.y) && (x < p.x));
  }

  template <typename T>
  inline bool point<T>::operator>(const point<T>& p) const {
    return (y > p.y) || ((y == p.y) && (x > p.x));
  }

  template <typename T>
  inline T& point<T>::operator[](const int i) {
    assert(i<2);
    return data[i];
  }

  template <typename T>
  inline const T& point<T>::operator[](const int i) const {
    assert(i<2);
    return data[i];
  }

  template <typename T>
  inline typename point<T>::size_type point<T>::size() const {
    return 2;
  }

  template <typename T>
  bool read(ioHandler& handler,point<T>& p,const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && handler.read(p.x);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.y);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  template<typename T>
  bool write(ioHandler& handler,const point<T>& p,const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && handler.write(p.x);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.y);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point& p);
  template <typename T>
  inline ostream& operator<<(ostream& s,const lti::point<T>& p) {
    s << "(" 
      << p.x << ","
      << p.y << ")";
    return s;
  }

  //inline ostream& operator>>(istream& s,const lti::point& p);
  template <typename T>
  inline istream& operator>>(istream& s,lti::point<T>& p) {
    char c;
    s >> c
      >> p.x >> c
      >> p.y >> c;

    return s;
  }

}


