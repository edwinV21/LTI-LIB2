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
 * \file   ltiPoint3D.h
 *         Defines the template type lti::point3D<T> and all shortcuts like
 *         ipoint3D.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiPoint3D_template.h,v 1.4 2008-08-06 03:40:58 alvarado Exp $
 */

namespace lti {
  
  template<typename T>
  inline typename point3D<T>::value_type
  point3D<T>::sqr(const value_type a) const {
    return a*a;
  }

  // constructor
  template <typename T>
  point3D<T>::point3D(const T newx,const T newy,const T newz)
    : x(newx),y(newy),z(newz) {
  }

  // copy constructor
  template <typename T>
  template <typename U>
  point3D<T>::point3D(const point3D<U>& p)
    : x(static_cast<T>(p.x)),
      y(static_cast<T>(p.y)),
      z(static_cast<T>(p.z)) {
  }

  // cast from
  template <typename T>
  template <typename U>
  point3D<T>& point3D<T>::castFrom(const point3D<U>& p) {
    x = static_cast<T>(p.x);
    y = static_cast<T>(p.y);
    z = static_cast<T>(p.z);
    return (*this);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::set(const T tx,const T ty,const T tz) {
    x = tx;
    y = ty;
    z = tz;
    return *this; 
  }

  template <typename T>
  inline void point3D<T>::get(T& tx,T& ty,T& tz) const {
    tx = x;
    ty = y;
    tz = z;
  }

  template <typename T>
  inline T point3D<T>::distanceSqr(const point3D<T>& c) const {
    return (sqr(x-c.x)+sqr(y-c.y)+sqr(z-c.z));
  }

  template <typename T>
  inline T point3D<T>::absSqr() const {
    return (x*x) + (y*y) + (z*z);
  }

  template <typename T>
  inline T point3D<T>::dot(const point3D<T>& p) const {
    return (x*p.x) + (y*p.y) + (z*p.z);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::cross(const point3D<T>& p) {
    return cross(*this,p);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::cross(const point3D<T>& a,
                                       const point3D<T>& b) {
    x = a.y*b.z - a.z*b.y;
    y = a.z*b.x - a.x*b.z;
    z = a.x*b.y - a.y*b.x;

    return *this;
  }

  template <typename T>
  template<typename U>
  inline point3D<T>& point3D<T>::multiply(const U c) {
    x = static_cast<T>(x*c);
    y = static_cast<T>(y*c);
    z = static_cast<T>(z*c);
    return *this;
  }

  template <typename T>
  template<typename U>
  inline point3D<T> point3D<T>::operator*(const U c) const {
    return point3D<T>(static_cast<T>(x*c),
                      static_cast<T>(y*c),
                      static_cast<T>(z*c));
  }

  template <typename T>
  template<typename U>
  inline point3D<T>& point3D<T>::multiply(const point3D<T>& other,const U c) {
    x = static_cast<T>(other.x*c);
    y = static_cast<T>(other.y*c);
    z = static_cast<T>(other.z*c);
    return *this;
  }

  template <typename T>
  template<typename U>
  inline point3D<T>& point3D<T>::operator*=(const U c) {
    x = static_cast<T>(x*c);
    y = static_cast<T>(y*c);
    z = static_cast<T>(z*c);
    return *this;
  }

  template <typename T>
  inline point3D<T> point3D<T>::operator*(const point3D<T>& c) const {
    return point3D<T>(c.x*x,c.y*y,c.z*z);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::operator*=(const point3D<T>& c) {
    x*=c.x;
    y*=c.y;
    z*=c.z;
    return *this;
  }

  template <typename T>
  inline point3D<T>& point3D<T>::emultiply(const point3D<T>& c) {
    x*=c.x;
    y*=c.y;
    z*=c.z;
    return *this;
  }

  template <typename T>
  inline point3D<T>& point3D<T>::emultiply(const point3D<T>& a,
                                           const point3D<T>& b) {
    x=a.x*b.x;
    y=a.y*b.y;
    z=a.z*b.z;
    return *this;
  }
  
  template <typename T>
  template<typename U>
  inline point3D<T>& point3D<T>::divide(const U c) {
    x = static_cast<T>(x/c);
    y = static_cast<T>(y/c);
    z = static_cast<T>(z/c);
    return *this;
  }

  template <typename T>
  template<typename U>
  inline point3D<T>& point3D<T>::divide(const point3D<T>& other,const U c) {
    x = static_cast<T>(other.x/c);
    y = static_cast<T>(other.y/c);
    z = static_cast<T>(other.z/c);
    return *this;
  }

  template <typename T>
  template <typename U>
  inline point3D<T> point3D<T>::operator/(const U c) const {
    return point3D<T>(static_cast<T>(x/c),
                      static_cast<T>(y/c),
                      static_cast<T>(z/c));
  }

  template <typename T>
  template <typename U>
  inline point3D<T>& point3D<T>::operator/=(const U c) {
    x = static_cast<T>(x/c);
    y = static_cast<T>(y/c);
    z = static_cast<T>(z/c);
    return *this;
  }

  template <typename T>
  inline point3D<T> point3D<T>::operator/(const point3D<T>& c) const {
    return point3D<T>(x/c.x,y/c.y,z/c.z);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::operator/=(const point3D<T>& c) {
    x/=c.x;
    y/=c.y;
    z/=c.z;
    return *this;
  }

  template <typename T>
  inline point3D<T>& point3D<T>::edivide(const point3D<T>& c) {
    x/=c.x;
    y/=c.y;
    z/=c.z;
    return *this;
  }

  template <typename T>
  inline point3D<T>& point3D<T>::edivide(const point3D<T>& a,
                                       const point3D<T>& b) {
    x=a.x/b.x;
    y=a.y/b.y;
    z=a.z/b.z;
    return *this;
  }


  template <typename T>
  inline point3D<T> point3D<T>::operator%(const int c) const {
    return point3D<T>(static_cast<int>(x)%c,
                      static_cast<int>(y)%c,
                      static_cast<int>(z)%c);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::add(const point3D<T>& p) {
    x+=p.x;
    y+=p.y;
    z+=p.z;
    return (*this);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::add(const point3D<T>& a,
                                 const point3D<T>& b) {
    x=a.x+b.x;
    y=a.y+b.y;
    z=a.z+b.z;

    return (*this);
  }


  template <typename T>
  inline point3D<T> point3D<T>::operator+(const point3D<T>& p) const {
    return point3D<T>(x+p.x,y+p.y,z+p.z);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::operator+=(const point3D<T>& p) {
    return add(p);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::subtract(const point3D<T>& p) {
    x-=p.x;
    y-=p.y;
    z-=p.z;
    return (*this);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::subtract(const point3D<T>& a,
                                      const point3D<T>& b) {
    x=a.x-b.x;
    y=a.y-b.y;
    z=a.z-b.z;

    return (*this);
  }

  template <typename T>
  inline point3D<T> point3D<T>::operator-(const point3D<T>& p) const {
    return point3D<T>(x-p.x,y-p.y,z-p.z);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::operator-=(const point3D<T>& p) {
    return subtract(p);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::copy(const point3D<T>& p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return (*this);
  }

  template <typename T>
  inline point3D<T>& point3D<T>::operator=(const point3D<T>& p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return (*this);
  }
  

  template <typename T>
  inline bool point3D<T>::operator==(const point3D<T>& p) const {
    return ((p.z == z) && (p.y == y) && (p.x == x));
  }

  template <typename T>
  inline bool point3D<T>::operator!=(const point3D<T>& p) const {
    return ((p.z != p.z) || (p.y != y) || (p.x != x));
  }

  template <typename T>
  inline bool point3D<T>::operator<(const point3D<T>& p) const {
    return (z < p.z) || ((z == p.z) &&
                         ((y < p.y) || ((y == p.y) && (x < p.x))));
  }

  template <typename T>
  inline bool point3D<T>::operator>(const point3D<T>& p) const {
    return (z > p.z) || ((z == p.z) &&
                         ((y > p.y) || ((y == p.y) && (x > p.x))));
  }

  template <typename T>
  inline T& point3D<T>::operator[](const int i) {
    assert(i<3);
    return data[i];
  }

  template <typename T>
  inline const T& point3D<T>::operator[](const int i) const {
    assert(i<3);
    return data[i];
  }

  template <typename T>
  inline typename point3D<T>::size_type point3D<T>::size() const {
    return 3;
  }

  template <typename T>
  bool read(ioHandler& handler,point3D<T>& p,const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && handler.read(p.x);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.y);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.z);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  template<typename T>
  bool write(ioHandler& handler,const point3D<T>& p,const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && handler.write(p.x);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.y);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.z);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point3D& p);
  template <typename T>
  inline ostream& operator<<(ostream& s,const lti::point3D<T>& p) {
    s << "(" 
      << p.x << ","
      << p.y << ","
      << p.z << ")";
    return s;
  }

  //inline ostream& operator>>(istream& s,const lti::point3D& p);
  template <typename T>
  inline istream& operator>>(istream& s,lti::point3D<T>& p) {
    char c;
    s >> c
      >> p.x >> c
      >> p.y >> c
      >> p.z >> c;

    return s;
  }

}
