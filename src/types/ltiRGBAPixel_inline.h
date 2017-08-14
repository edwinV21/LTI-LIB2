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
 * \file   ltiRGBAPixel_inline.h
 *         Contains the basic template class to represent RGB pixels with
 *         an alpha channel
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBAPixel_inline.h,v 1.3 2005-03-16 16:20:52 doerfler Exp $
 */

namespace lti {

  inline void rgbaPixel::set(const ubyte r, 
                             const ubyte g,
                             const ubyte b,
                             const ubyte a) {
    alpha = a;
    red   = r;
    green = g;
    blue  = b;
  }

  inline void rgbaPixel::set(const ubyte r, 
                             const ubyte g,
                             const ubyte b) {
    blue  = b;
    green = g;
    red   = r;
  }

  inline void rgbaPixel::setRed(const ubyte r) {red = r;}
  inline void rgbaPixel::setGreen(const ubyte g) {green = g;}
  inline void rgbaPixel::setBlue(const ubyte b) {blue = b;}
  inline void rgbaPixel::setAlpha(const ubyte d) {alpha = d;}
  inline void rgbaPixel::setValue(const uint32& v) {value = v;}
  inline void rgbaPixel::get(ubyte& r,ubyte& g,ubyte& b) const {
    r = red;
    g = green;
    b = blue;
  }

  inline void rgbaPixel::get(int& r,int& g,int& b) const {
    r = red;
    g = green;
    b = blue;
  }

  inline void rgbaPixel::get(ubyte& r,ubyte& g,ubyte& b,ubyte& a) const {
    a = alpha;
    b = blue;
    g = green;
    r = red;
  }

  inline void rgbaPixel::get(int& r,int& g,int& b,int& a) const {
    a = alpha;
    b = blue;
    g = green;
    r = red;
  }

  inline const ubyte& rgbaPixel::getRed()   const {return red;}
  inline const ubyte& rgbaPixel::getGreen() const {return green;}
  inline const ubyte& rgbaPixel::getBlue()  const {return blue;}
  inline const ubyte& rgbaPixel::getAlpha() const {return alpha;}

  inline ubyte& rgbaPixel::getRed()   {return red;}
  inline ubyte& rgbaPixel::getGreen() {return green;}
  inline ubyte& rgbaPixel::getBlue()  {return blue;}
  inline ubyte& rgbaPixel::getAlpha() {return alpha;}

  inline const uint32& rgbaPixel::getValue() const {return value;}
  inline uint32& rgbaPixel::getValue() {return value;}

  inline ubyte& rgbaPixel::at(const int x) {
    assert(static_cast<uint32>(x)<3);
    return data[2-x];
  }

  inline const ubyte& rgbaPixel::at(const int x) const {
    assert(static_cast<uint32>(x)<3);
    return data[2-x];
  }

  inline ubyte& rgbaPixel::operator[](const int x) {
    assert(static_cast<uint32>(x)<3);
    return data[2-x];
  }

  inline const ubyte& rgbaPixel::operator[](const int x) const {
    assert(static_cast<uint32>(x)<3);
    return data[2-x];
  }

  inline int rgbaPixel::size() const {
    return 3;
  }

  inline rgbaPixel& rgbaPixel::copy(const rgbaPixel& other) {
    value=other.value;
    return (*this);
  }

  inline rgbaPixel& rgbaPixel::operator=(const rgbaPixel& other) {
    value=other.value;
    return (*this);
  }

  inline bool rgbaPixel::isEqual(const rgbaPixel& other) const {
    return (value == other.value);
  }

  inline bool rgbaPixel::operator==(const rgbaPixel& other) const {
    return (value == other.value);
  }
  
  inline bool rgbaPixel::operator!=(const rgbaPixel& other) const {
    return (value != other.value);
  }
  
  inline bool rgbaPixel::operator<(const rgbaPixel& other) const {
    return (value < other.value);
  }

  inline bool rgbaPixel::operator>(const rgbaPixel& other) const {
    return (value > other.value);
  }

  inline rgbaPixel& rgbaPixel::add(const rgbaPixel& other) {
    blue += other.blue;
    green += other.green;
    red +=other.red;

    return (*this);
  }

  inline rgbaPixel& rgbaPixel::operator+=(const rgbaPixel& other) {
    return add(other);
  }

  inline rgbaPixel rgbaPixel::operator+(const rgbaPixel& other) const {
    return rgbaPixel(red+other.red,
                     green+other.green,
                     blue+other.blue,
                     alpha);
  }

  inline rgbaPixel& rgbaPixel::subtract(const rgbaPixel& other) {
    blue -= other.blue;
    green -= other.green;
    red -=other.red;
    
    return (*this);
  }

  inline rgbaPixel& rgbaPixel::operator-=(const rgbaPixel& other) {
    return subtract(other);
  }

  inline rgbaPixel rgbaPixel::operator-(const rgbaPixel& other) const {
    return rgbaPixel(red-other.red,
                     green-other.green,
                     blue-other.blue,
                     alpha);
  }

  inline rgbaPixel& rgbaPixel::multiply(const rgbaPixel& other) {
    blue *= other.blue;
    green *= other.green;
    red *= other.red;
    
    return (*this);
  }

  inline rgbaPixel& rgbaPixel::operator*=(const rgbaPixel& other) {
    return multiply(other);
  }

  inline rgbaPixel rgbaPixel::operator*(const rgbaPixel& other) const {
    return rgbaPixel(red*other.red,
                     green*other.green,
                     blue*other.blue,
                     alpha);
  }

  inline rgbaPixel& rgbaPixel::multiply(const int other) {

    blue = static_cast<ubyte>(blue*other);
    green = static_cast<ubyte>(green*other);
    red = static_cast<ubyte>(red*other);

    return (*this);
  }

  inline rgbaPixel& rgbaPixel::multiply(const float& other) {
    
    blue = static_cast<ubyte>(blue*other);
    green = static_cast<ubyte>(green*other);
    red = static_cast<ubyte>(red*other);

    return (*this);
  }

  inline rgbaPixel& rgbaPixel::multiply(const double& other) {
    
    blue = static_cast<ubyte>(blue*other);
    green = static_cast<ubyte>(green*other);
    red = static_cast<ubyte>(red*other);
    
    return (*this);
  }
  
  inline rgbaPixel& rgbaPixel::operator*=(const int other) {
    return multiply(other);
  }

  inline rgbaPixel& rgbaPixel::operator*=(const float& other) {
    return multiply(other);
  }

  inline rgbaPixel& rgbaPixel::operator*=(const double& other) {
    return multiply(other);
  }

  inline rgbaPixel rgbaPixel::operator*(const int other) const {
    return rgbaPixel(static_cast<ubyte>(red*other),
                     static_cast<ubyte>(green*other),
                     static_cast<ubyte>(blue*other),
                     alpha);
  }

  inline rgbaPixel rgbaPixel::operator*(const float& other) const {
    return rgbaPixel(static_cast<ubyte>(red*other),
                     static_cast<ubyte>(green*other),
                     static_cast<ubyte>(blue*other),
                     alpha);
  }
  
  inline rgbaPixel rgbaPixel::operator*(const double& other) const {
    return rgbaPixel(static_cast<ubyte>(red*other),
                     static_cast<ubyte>(green*other),
                     static_cast<ubyte>(blue*other),
                     alpha);
  }

  inline rgbaPixel& rgbaPixel::overlay(const rgbaPixel& below) {
    const ubyte oneminus = ubyte(255)-alpha;
    set((static_cast<int>(red)*oneminus + 
         static_cast<int>(below.red)*alpha)/255,
        (static_cast<int>(green)*oneminus + 
         static_cast<int>(below.green)*alpha)/255,
        (static_cast<int>(blue)*oneminus + 
         static_cast<int>(below.blue)*alpha)/255,
        alpha*below.alpha/255);
    return *this;
  }

  inline rgbaPixel& rgbaPixel::overlay(const rgbaPixel& above,
                                       const rgbaPixel& below) {
    const ubyte oneminus = ubyte(255)-alpha;
    set((static_cast<int>(above.red)*oneminus + 
         static_cast<int>(below.red)*alpha)/255,
        (static_cast<int>(above.green)*oneminus + 
         static_cast<int>(below.green)*alpha)/255,
        (static_cast<int>(above.blue)*oneminus + 
         static_cast<int>(below.blue)*alpha)/255,
        above.alpha*below.alpha/255);
    return *this;
  }

  inline rgbaPixel& rgbaPixel::divide(const rgbaPixel& other) {
    blue /= other.blue;
    green /= other.green;
    red /= other.red;   
    
    return (*this);
  }

  inline rgbaPixel& rgbaPixel::operator/=(const rgbaPixel& other) {
    return divide(other);
  }

  inline rgbaPixel rgbaPixel::operator/(const rgbaPixel& other) const {
    return rgbaPixel(red/other.red,
                     green/other.green,
                     blue/other.blue,
                     alpha);
  }

  inline rgbaPixel& rgbaPixel::divide(const int other) {
    
    blue /= other;
    green /= other;
    red /= other;

    return (*this);
  }

  inline rgbaPixel& rgbaPixel::divide(const float& other) {
    
    blue = static_cast<ubyte>(blue/other);
    green = static_cast<ubyte>(green/other);
    red = static_cast<ubyte>(red/other);

    return (*this);
  }

  inline rgbaPixel& rgbaPixel::divide(const double& other) {
    
    blue = static_cast<ubyte>(blue/other);
    green = static_cast<ubyte>(green/other);
    red = static_cast<ubyte>(red/other);

    return (*this);
  }

  inline rgbaPixel& rgbaPixel::operator/=(const int other) {
    return divide(other);
  }

  inline rgbaPixel& rgbaPixel::operator/=(const float& other) {
    return divide(other);
  }

  inline rgbaPixel& rgbaPixel::operator/=(const double& other) {
    return divide(other);
  }

  inline rgbaPixel rgbaPixel::operator/(const int other) const {
    return rgbaPixel(static_cast<ubyte>(red/other),
                     static_cast<ubyte>(green/other),
                     static_cast<ubyte>(blue/other),
                     alpha);
  }

  inline rgbaPixel rgbaPixel::operator/(const float& other) const {
    return rgbaPixel(static_cast<ubyte>(red/other),
                     static_cast<ubyte>(green/other),
                     static_cast<ubyte>(blue/other),
                     alpha);
  }

  inline rgbaPixel rgbaPixel::operator/(const double& other) const {
    return rgbaPixel(static_cast<ubyte>(red/other),
                     static_cast<ubyte>(green/other),
                     static_cast<ubyte>(blue/other),
                     alpha);
  }

  inline int rgbaPixel::absSqr() const {
    return ( static_cast<int>(red)*static_cast<int>(red) +
             static_cast<int>(green)*static_cast<int>(green) +
             static_cast<int>(blue)*static_cast<int>(blue) );
  }

  inline int rgbaPixel::dot(const rgbaPixel& other) const {
    return ( static_cast<int>(red)*static_cast<int>(other.red) +
             static_cast<int>(green)*static_cast<int>(other.green) +
             static_cast<int>(blue)*static_cast<int>(other.blue) );
  }

  inline int rgbaPixel::distanceSqr(const rgbaPixel& other) const {
    const int 
      r(static_cast<int>(other.red)-
        static_cast<int>(red)),
      g(static_cast<int>(other.green)-
        static_cast<int>(green)),
      b(static_cast<int>(other.blue)-
        static_cast<int>(blue));

      return (r*r+g*g+b*b);
  }

  inline rgbaPixel operator*(const int p,const rgbaPixel& other) {
    return other*p;
  }

  inline rgbaPixel operator*(const float p,const rgbaPixel& other) {
    return other*p;
  }

  inline rgbaPixel operator*(const double& p,const rgbaPixel& other) {
    return other*p;
  }
}


