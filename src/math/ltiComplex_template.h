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
 * \file   ltiComplex_inline.h
 *         LTI-Lib version of the complex numbers.  There are a few features
 *         besides the std::complex, but both types are compatible.
 * \author Pablo Alvarado
 * \date   06.04.2003
 *
 * $Id: ltiComplex_template.h,v 1.3 2005-03-16 16:20:50 doerfler Exp $
 */


namespace lti {    

  /**
   * get the absolute value of a complex number
   */
  template<typename T>
  T abs(const complex<T>& cn) {
    T a = cn.real();
    T b = cn.imag();

    const T tmp = max(abs(a), abs(b));
    if (tmp == T()) { // zero?
      return tmp;
    }

    a /= tmp; // normalize for more numerical stability
    b /= tmp;
    return tmp * sqrt(a * a + b * b);
  }

  template<typename T>
  complex<T> sqrt(const complex<T>& cn) {
    T a = cn.real();
    T b = cn.imag();

    if (a == T()) {
      T other = sqrt(abs(b) / 2);
      return complex<T>(other, b < T() ? -other : other);
    }
    else {
      T other = sqrt(2 * (abs(cn) + abs(a)));
      T tmp = other / 2;
      return (a > T()) ? 
        complex<T>(tmp, b / other)
        : complex<T>(abs(b) / other, b < T() ? -tmp : tmp);
    }
  }

  /*
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,complex<T>& p,const bool complete) {
    bool b(true);
    
    if (complete) {
      b = b && handler.readBegin();
    }

    T re,im;
    b = b && handler.read(re);
    b = b && handler.readDataSeparator();
    b = b && handler.read(im);

    p=complex<T>(re,im);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,const complex<T>& p,const bool complete) {
    bool b(true);
    
    if (complete) {
      b = b && handler.writeBegin();
    }
    
    b = b && handler.write(p.real());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.imag());
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

} // end namespace lti

namespace std {

  template<typename T>
  inline istream& operator>>(istream& in, lti::complex<T>& a) {
    T re, im;
    char ch;
    in >> ch;
    if (ch == '(') {
      in >> re >> ch;
      if (ch == ',') {
        in >> im >> ch;
        if (ch == ')') {
          a = lti::complex<T>(re, im);
        } else {
          in.setstate(ios_base::failbit);
        }
      }
      else if (ch == ')') {
        a = lti::complex<T>(re, T(0));
      } else {
        in.setstate(ios_base::failbit);
      }
    }
    else {
      in.putback(ch);
      in >> re;
      a = lti::complex<T>(re, T(0));
    }
    return in;
  }

  template<typename T>
  inline ostream& operator<<(ostream& out, const lti::complex<T>& a) {
    out << '(' << a.real() << ',' << a.imag() << ')';
    return out;
  }

} // namespace std
