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
 * \file   ltiRGBPixel_inline.h
 *         Contains the basic template class to represent RGB pixels (without
 *         an alpha channel)
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBPixel_inline.h,v 1.7 2008-04-27 02:42:22 alvarado Exp $
 */

namespace lti {

  template<typename T>
  template<typename U>
  inline rgbPixel<T>::rgbPixel(const rgbPixel<U>& other) {
    red = static_cast<T>(other.red);
    green = static_cast<T>(other.green);
    blue = static_cast<T>(other.blue);
  }

  template <typename T>
  template <typename U>
  inline void rgbPixel<T>::set(const U r, 
                               const U g,
                               const U b) {
    blue  = static_cast<T>(b);
    green = static_cast<T>(g);
    red   = static_cast<T>(r);
  }

  template <typename T>
  inline void rgbPixel<T>::setRed(const T r) {
    red = r;
  }

  template <typename T>
  inline void rgbPixel<T>::setGreen(const T g) {
    green = g;
  }

  template <typename T>
  inline void rgbPixel<T>::setBlue(const T b) {
    blue = b;
  }

  template <typename T>
  template <typename U>
  inline void rgbPixel<T>::get(U& r,U& g,U& b) const {
    r = static_cast<U>(red);
    g = static_cast<U>(green);
    b = static_cast<U>(blue);
  }

  template <typename T>
  inline const T& rgbPixel<T>::getRed() const {
    return red;
  }

  template <typename T>
  inline const T& rgbPixel<T>::getGreen() const {
    return green;
  }

  template <typename T>
  inline const T& rgbPixel<T>::getBlue() const {
    return blue;
  }
  
  template <typename T>
  inline T& rgbPixel<T>::getRed() {
    return red;
  }

  template <typename T>
  inline T& rgbPixel<T>::getGreen() {
    return green;
  }

  template <typename T>
  inline T& rgbPixel<T>::getBlue() {
    return blue;
  }

  template <typename T>
  inline T& rgbPixel<T>::at(const int x) {
    assert(static_cast<uint32>(x)<3);
    return data[x];
  }

  template <typename T>
  inline const T& rgbPixel<T>::at(const int x) const {
    assert(static_cast<uint32>(x)<3);
    return data[x];
  }

  template <typename T>
  inline T& rgbPixel<T>::operator[](const int x) {
    assert(static_cast<uint32>(x)<3);
    return data[x];
  }

  template <typename T>
  inline const T& rgbPixel<T>::operator[](const int x) const {
    assert(static_cast<uint32>(x)<3);
    return data[x];
  }

  template <typename T>
  inline typename rgbPixel<T>::size_type rgbPixel<T>::size() const {
    return 3;
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::copy(const rgbPixel<T>& other) {
    red=other.red;
    green=other.green;
    blue=other.blue;

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator=(const rgbPixel<T>& other) {
    return copy(other);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::copy(const rgbaPixel& other) {
    red=static_cast<T>(other.red);
    green=static_cast<T>(other.green);
    blue=static_cast<T>(other.blue);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator=(const rgbaPixel& other) {
    return copy(other);
  }

  template <typename T>
  inline void rgbPixel<T>::castTo(rgbaPixel& other) const {
    other.red = static_cast<ubyte>(red);
    other.green = static_cast<ubyte>(green);
    other.blue = static_cast<ubyte>(blue);
  }

  template <typename T>
  inline rgbaPixel rgbPixel<T>::getRGBAPixel() const {
    return rgbaPixel(static_cast<ubyte>(red),
                     static_cast<ubyte>(green),
                     static_cast<ubyte>(blue));
  }

  template <typename T>
  inline rgbaPixel rgbPixel<T>::getClippedRGBAPixel() const {
    return rgbaPixel(red<0?0:red>255?255:static_cast<ubyte>(red),
                     green<0?0:green>255?255:static_cast<ubyte>(green),
                     blue<0?0:blue>255?255:static_cast<ubyte>(blue));
  }

  template <>
  inline rgbaPixel rgbPixel<byte>::getClippedRGBAPixel() const {
    return rgbaPixel(red<0?0:static_cast<ubyte>(red),
                     green<0?0:static_cast<ubyte>(green),
                     blue<0?0:static_cast<ubyte>(blue));
  }

  template <>
  inline rgbaPixel rgbPixel<ubyte>::getClippedRGBAPixel() const {
    return rgbaPixel(red,green,blue);
  }

  template <>
  inline rgbaPixel rgbPixel<uint16>::getClippedRGBAPixel() const {
    return rgbaPixel(red>255?255:static_cast<ubyte>(red),
                     green>255?255:static_cast<ubyte>(green),
                     blue>255?255:static_cast<ubyte>(blue));
  }

  template <>
  inline rgbaPixel rgbPixel<uint32>::getClippedRGBAPixel() const {
    return rgbaPixel(red>255?255:static_cast<ubyte>(red),
                     green>255?255:static_cast<ubyte>(green),
                     blue>255?255:static_cast<ubyte>(blue));
  }

  template <typename T>
  inline bool rgbPixel<T>::isEqual(const rgbPixel<T>& other) const {
    return ((red == other.red) &&
            (green == other.green) &&
            (blue == other.blue));
  }

  template <typename T>
  inline bool rgbPixel<T>::operator==(const rgbPixel<T>& other) const {
    return isEqual(other);
  }
  
  template <typename T>
  inline bool rgbPixel<T>::operator!=(const rgbPixel<T>& other) const {
    return ((red != other.red) &&
            (green != other.green) &&
            (blue != other.blue));
  }
  
  template <typename T>
  inline bool rgbPixel<T>::operator<(const rgbPixel<T>& other) const {
    return 
      (red < other.red) || 
      ((red == other.red) && ((green < other.green) ||
                              ((green == other.green) && blue < other.blue)));
  }

  template <typename T>
  inline bool rgbPixel<T>::operator>(const rgbPixel<T>& other) const {
    return 
      (red > other.red) || 
      ((red == other.red) && ((green > other.green) ||
                              ((green == other.green) && blue > other.blue)));
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::add(const rgbPixel<T>& other) {
    red +=other.red;
    green += other.green;
    blue += other.blue;

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator+=(const rgbPixel<T>& other) {
    return add(other);
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator+(const rgbPixel<T>& other) const {
    return rgbPixel<T>(red+other.red,
                       green+other.green,
                       blue+other.blue);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::subtract(const rgbPixel<T>& other) {
    red -=other.red;
    green -= other.green;
    blue -= other.blue;
    
    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator-=(const rgbPixel<T>& other) {
    return subtract(other);
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator-(const rgbPixel<T>& other) const {
    return rgbPixel<T>(red-other.red,
                       green-other.green,
                       blue-other.blue);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::multiply(const rgbPixel<T>& other) {
    red *= other.red;
    green *= other.green;
    blue *= other.blue;
    
    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator*=(const rgbPixel<T>& other) {
    return multiply(other);
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator*(const rgbPixel<T>& other) const {
    return rgbPixel<T>(red*other.red,
                       green*other.green,
                       blue*other.blue);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::multiply(const int other) {

    red = static_cast<T>(red*other);
    green = static_cast<T>(green*other);
    blue = static_cast<T>(blue*other);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::multiply(const float& other) {
    
    red = static_cast<T>(red*other);
    green = static_cast<T>(green*other);
    blue = static_cast<T>(blue*other);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::multiply(const double& other) {
    
    red = static_cast<T>(red*other);
    green = static_cast<T>(green*other);
    blue = static_cast<T>(blue*other);
    
    return (*this);
  }
  
  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator*=(const int other) {
    return multiply(other);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator*=(const float& other) {
    return multiply(other);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator*=(const double& other) {
    return multiply(other);
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator*(const int other) const {
    return rgbPixel<T>(static_cast<T>(red*other),
                       static_cast<T>(green*other),
                       static_cast<T>(blue*other));
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator*(const float& other) const {
    return rgbPixel<T>(static_cast<T>(red*other),
                       static_cast<T>(green*other),
                       static_cast<T>(blue*other));
  }
  
  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator*(const double& other) const {
    return rgbPixel<T>(static_cast<T>(red*other),
                       static_cast<T>(green*other),
                       static_cast<T>(blue*other));
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const rgbPixel<T>& other) {
    red /= other.red;   
    green /= other.green;
    blue /= other.blue;
    
    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator/=(const rgbPixel<T>& other) {
    return divide(other);
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator/(const rgbPixel<T>& other) const {
    return rgbPixel<T>(red/other.red,
                       green/other.green,
                       blue/other.blue);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const int other) {
    
    red = static_cast<T>(red/other);
    green = static_cast<T>(green/other);
    blue = static_cast<T>(blue/other);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const float& other) {
    
    red = static_cast<T>(red/other);
    green = static_cast<T>(green/other);
    blue = static_cast<T>(blue/other);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const double& other) {
    
    red = static_cast<T>(red/other);
    green = static_cast<T>(green/other);
    blue = static_cast<T>(blue/other);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const rgbPixel<T>& other,
                                          const int cst) {
    
    red = static_cast<T>(other.red/cst);
    green = static_cast<T>(other.green/cst);
    blue = static_cast<T>(other.blue/cst);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const rgbPixel<T>& other,
                                          const float& cst) {
    
    red = static_cast<T>(other.red/cst);
    green = static_cast<T>(other.green/cst);
    blue = static_cast<T>(other.blue/cst);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::divide(const rgbPixel<T>& other,
                                          const double& cst) {
    
    red = static_cast<T>(other.red/cst);
    green = static_cast<T>(other.green/cst);
    blue = static_cast<T>(other.blue/cst);

    return (*this);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator/=(const int other) {
    return divide(other);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator/=(const float& other) {
    return divide(other);
  }

  template <typename T>
  inline rgbPixel<T>& rgbPixel<T>::operator/=(const double& other) {
    return divide(other);
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator/(const int other) const {
    return rgbPixel<T>(static_cast<T>(red/other),
                       static_cast<T>(green/other),
                       static_cast<T>(blue/other));
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator/(const float& other) const {
    return rgbPixel<T>(static_cast<T>(red/other),
                       static_cast<T>(green/other),
                       static_cast<T>(blue/other));
  }

  template <typename T>
  inline rgbPixel<T> rgbPixel<T>::operator/(const double& other) const {
    return rgbPixel<T>(static_cast<T>(red/other),
                       static_cast<T>(green/other),
                       static_cast<T>(blue/other));
  }

  template <typename T>
  inline typename typeInfo<T>::square_accumulation_type
  rgbPixel<T>::absSqr() const {
    typedef typename typeInfo<T>::square_accumulation_type sqt;
    return ( static_cast<sqt>(red)*static_cast<sqt>(red) +
             static_cast<sqt>(green)*static_cast<sqt>(green) +
             static_cast<sqt>(blue)*static_cast<sqt>(blue) );
  }

  template <typename T>
  inline typename typeInfo<T>::square_accumulation_type
  rgbPixel<T>::dot(const rgbPixel<T>& other) const {
    typedef typename typeInfo<T>::square_accumulation_type sqt;
    return ( static_cast<sqt>(red)*static_cast<sqt>(other.red) +
             static_cast<sqt>(green)*static_cast<sqt>(other.green) +
             static_cast<sqt>(blue)*static_cast<sqt>(other.blue) );
  }

  template <typename T>
  inline typename typeInfo<T>::square_accumulation_type
  rgbPixel<T>::distanceSqr(const rgbPixel<T>& other) const {
    typedef typename typeInfo<T>::square_accumulation_type sqt;
    const sqt 
      r(static_cast<sqt>(other.red)-
        static_cast<sqt>(red)),
      g(static_cast<sqt>(other.green)-
        static_cast<sqt>(green)),
      b(static_cast<sqt>(other.blue)-
        static_cast<sqt>(blue));

      return (r*r+g*g+b*b);
  }

  template <typename T>
  inline rgbPixel<T> operator*(const int p,const rgbPixel<T>& other) {
    return other*p;
  }

  template <typename T>
  inline rgbPixel<T> operator*(const float p,const rgbPixel<T>& other) {
    return other*p;
  }

  template <typename T>
  inline rgbPixel<T> operator*(const double& p,const rgbPixel<T>& other) {
    return other*p;
  }
}


