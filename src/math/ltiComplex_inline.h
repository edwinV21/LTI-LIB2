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
 * $Id: ltiComplex_inline.h,v 1.3 2008-05-25 18:54:58 alvarado Exp $
 */


namespace lti {

  /*
   * Square of the magnitude of a number (|x|*|x|)
   *
   * This function is useful in generic programming for those cases where
   * complex numbers will occur in products of conjugate pairs.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqrAbs(const complex<T>& x) {
    return sqrAbs(x.real())+sqrAbs(x.imag());
  }

  // -------------------------------------------------------------------------
  // implementation of class complex
  // -------------------------------------------------------------------------

  template<typename T>
  inline T complex<T>::real() const {
    return realPart_; 
  }
  
  template<typename T>
  inline T complex<T>::imag() const {
    return imagPart_; 
  }

  template<typename T>
  inline complex<T>::complex(const T& re, const T& im)
    : realPart_(re), imagPart_(im) { }

  template<typename T>
  template<typename U>
  inline complex<T>::complex(const complex<U>& other) 
    : realPart_(static_cast<T>(other.real())),
      imagPart_(static_cast<T>(other.imag())) {	
  }
  
  template<typename T>
  inline complex<T>::complex(const std::complex<T>& other)
    : realPart_(other.real()), imagPart_(other.imag()) {
  }

  template<typename T>
  inline complex<T>::operator std::complex<T>() const {
    return std::complex<T>(real(),imag());
  }

  template<typename T>
  inline complex<T>& complex<T>::operator=(const T& other) {
    realPart_ = other;
    imagPart_ = T();
    return *this;
  } 

  template<typename T>
  inline complex<T>& complex<T>::operator+=(const T& other) {
    realPart_ += other;
    return *this;
  }
  
  template<typename T>
  inline complex<T>& complex<T>::operator-=(const T& other) {
    realPart_ -= other;
    return *this;
  }

  template<typename T>
  inline complex<T>&  complex<T>::operator*=(const T& other) {
    realPart_ *= other;
    imagPart_ *= other;
    return *this;
  }

  template<typename T>
  inline complex<T>& complex<T>::operator/=(const T& other) {
    realPart_ /= other;
    imagPart_ /= other;
    return *this;
  }

  template<typename T>
  inline void complex<T>::set(const T& re, const T& im) {
    realPart_ = re;
    imagPart_ = im;
  }

  template<typename T>
  inline void complex<T>::setReal(const T& re) {
    realPart_ = re;
  }

  template<typename T>
  inline void complex<T>::setImag(const T& im) {
    imagPart_ = im;
  }
  
  template<typename T>
  inline void complex<T>::get(T& re, T& im) const {
    re = realPart_;
    im = imagPart_;
  }

  template<typename T>
  template<typename U>
  inline complex<T>& complex<T>::operator=(const complex<U>& other) {
    realPart_ = other.real();
    imagPart_ = other.imag();
    return (*this);
  }

  template<typename T>
  template<typename U>
  complex<T>& complex<T>::operator+=(const complex<U>& other) {
    realPart_+=static_cast<T>(other.real());
    imagPart_+=static_cast<T>(other.imag());
    return (*this);
  }
  
  template<typename T>
  template<typename U>
  complex<T>& complex<T>::operator-=(const complex<U>& other) {
    realPart_-=static_cast<T>(other.real());
    imagPart_-=static_cast<T>(other.imag());
    return (*this);
  }
    
  template<typename T>
  template<typename U>
  complex<T>& complex<T>::operator*=(const complex<U>& other) {
    const T re = realPart_ * other.real() - imagPart_ * other.imag();
    imagPart_ = realPart_ * other.imag() + imagPart_ * other.real();
    realPart_ = re;
    return *this;
  }
  
  template<typename T>
  template<typename U>
  complex<T>& complex<T>::operator/=(const complex<U>& other) {
    const T norm = other.real()*other.real() + other.imag()*other.imag();
    const T re = (realPart_ * other.real() + imagPart_ * other.imag())/norm;
    imagPart_ = (imagPart_ * other.real() - realPart_ * other.imag())/norm;
    realPart_ = re;
    return *this;
  }
  
  // -------------------------------------------------------------------------
  // global operators
  // -------------------------------------------------------------------------

  /*
   * add two complex numbers
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) += b; 
  }

  /*
   * add a complex number with a real number
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a, const T& b) {
    return complex<T>(a) += b; 
  }

  /*
   * add a real number with a complex number
   */
  template<typename T>
  inline complex<T> operator+(const T& a, const complex<T>& b) {
    return complex<T>(b) += a; 
  }

  /*
   * subtract two complex numbers
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) -= b; 
  }
    
  /*
   * subtract a real number from a complex one
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a, const T& b) {
    return complex<T>(a) -= b; 
  }

  /*
   * subtract a complex number from a real one
   */
  template<typename T>
  inline complex<T> operator-(const T& a, const complex<T>& b) {
    return complex<T>(a) -= b;
  }

  /*
   * multiply two complex numbers
   */
  template<typename T>
  inline complex<T> operator*(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) *= b; 
  }

  /*
   * multiply a complex number with a real one
   */
  template<typename T>
  inline complex<T> operator*(const complex<T>& a, const T& b) {
    return complex<T>(a) *= b; 
  }

  /*
   * multiply a real number with a complex one
   */
  template<typename T>
  inline complex<T> operator*(const T& a, const complex<T>& b) {
    return complex<T>(b) *= a;
  }

  /*
   * divide two complex numbers
   */
  template<typename T>
  inline complex<T> operator/(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) /= b; 
  }
    
  /*
   * divide a complex number by a real one
   */
  template<typename T>
  inline complex<T> operator/(const complex<T>& a, const T& b) {
    return complex<T>(a) /= b; 
  }

  /*
   * divide a real number by a complex one
   */
  template<typename T>
  inline complex<T> operator/(const T& a, const complex<T>& b) {
    return complex<T>(a) /= b; 
  }

  /*
   * plus sign a complex number
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a) {
    return a;
  }

  /*
   * minus sign a complex number
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a) {
    return complex<T>(-a.real(), -a.imag());
  }

  /*
   * compare if two complex numbers are equal
   */
  template<typename T>
  inline bool operator==(const complex<T>& a, const complex<T>& b) {
    return (a.real() == b.real()) && (a.imag() == b.imag());
  }

  /*
   * compare if two complex numbers differ
   */
  template<typename T>
  inline bool operator==(const complex<T>& a, const T& b) {
    return (a.real() == b) && (a.imag() == T());
  }

  /*
   * compare if a real number and a complex number are the same
   */
  template<typename T>
  inline bool operator==(const T& a, const complex<T>& b) {
    return (a == b.real()) && (T() == b.imag()); 
  }

  /*
   * compare if two complex numbers differ
   */
  template<typename T>
  inline bool operator!=(const complex<T>& a, const complex<T>& b) {
    return (a.real() != b.real()) || (a.imag() != b.imag());
  }

  /*
   * compare if a complex number and a real one differ
   */
  template<typename T>
  inline bool operator!=(const complex<T>& a, const T& b) {
    return (a.real() != b) || (a.imag() != T());
  }

  /*
   * compare if a real number and a complex one differ
   */
  template<typename T>
  inline bool operator!=(const T& a, const complex<T>& b) {
    return (a != b.real()) || (T() != b.imag());
  }

  /*
   * a complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const complex<T>& a,const complex<T>& b) {
    return ((a.real()<b.real()) || 
            ((a.real() == b.real()) && (a.imag() < b.imag())));
  }

  /*
   * a complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const complex<T>& a,const T& b) {
    return ((a.real()<b) || 
            ((a.real() == b) && (a.imag() < T())));
  }

  /*
   * a complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const T& a,const complex<T>& b) {
    return ((a<b.real()) || 
            ((a == b.real()) && (T() < b.imag())));
  }

  /*
   * a complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const complex<T>& a,const complex<T>& b) {
    return ((a.real()>b.real()) || 
            ((a.real() == b.real()) && (a.imag() > b.imag())));
  }

  /*
   * a complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const complex<T>& a,const T& b) {
    return ((a.real()>b) || 
            ((a.real() == b) && (a.imag() > T())));
  }

  /*
   * a complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const T& a,const complex<T>& b) {
    return ((a > b.real()) || 
            ( (a == b.real()) && ( b.imag() < T(0) ) ));
  }

  /*
   * get the real part of a complex number
   */
  template<typename T>
  inline T real(const complex<T>& cn) {
    return cn.real(); 
  }
    
  /*
   * get the imaginary part of a complex number
   */
  template<typename T>
  inline T imag(const complex<T>& cn) {
    return cn.imag(); 
  }

  /*
   * get the argument (angle or phase) of a complex number
   */
  template<typename T>
  inline T arg(const complex<T>& cn) {
    return atan2(cn.imag(), cn.real());
  }


  /*
   * return the square magnitude of the given complex number
   */ 
  template<typename T>
  inline T norm(const complex<T>& cn) {
    return (sqr(cn.real())+sqr(cn.imag()));
  }

  /*
   * convert the given polar values into a complex number
   *
   * @param radius magnitude of the number
   * @param angle  angle of the number
   * 
   * @return Equivalent complex number to the polar one
   */
  template<typename T>
  inline complex<T> polar(const T& radius, const T& angle) {
    return complex<T>(radius * cos(angle), radius * sin(angle)); 
  }

  /*
   * return the conjugate of the complex number
   */
  template<typename T>
  inline complex<T> conj(const complex<T>& cn) {
    return complex<T>(cn.real(), -cn.imag()); 
  }
  
  /*
   * cosine of a complex number
   */
  template<typename T>
  inline complex<T> cos(const complex<T>& cn) {
    const T a = cn.real();
    const T b = cn.imag();
    return complex<T>(cos(a) * std::cosh(b), -sin(a) * std::sinh(b));
  }

  /*
   * hyperbolic cosine of a complex number
   */
  template<typename T>
  inline complex<T> cosh(const complex<T>& cn) {
    const T a = cn.real();
    const T b = cn.imag();
    return complex<T>(std::cosh(a) * cos(b), std::sinh(a) * sin(b));
  }

  template<typename T>
  inline complex<T> acos(const complex<T>& cn) {
    complex<T> tmp(log(cn+sqrt(cn*cn-T(1))));
    return complex<T>(tmp.imag(),-tmp.real());;
  }

  /*
   * exponetial of a complex number
   */
  template<typename T>
  inline complex<T> exp(const complex<T>& cn) {
    return polar(exp(cn.real()), cn.imag()); 
  }

  /*
   * natural logarithm of a complex number (base \e e)
   */
  template<typename T>
  inline complex<T> log(const complex<T>& cn) {
    return complex<T>(log(abs(cn)), arg(cn)); 
  }

  /*
   * logarithm base 10 of a complex number
   */
  template<typename T>
  inline complex<T> log10(const complex<T>& cn) {
    return log(cn) / log(T(10.0)); 
  }

  /*
   * sine of a complex number
   */
  template<typename T>
  inline complex<T> sin(const complex<T>& cn) {
    const T a = cn.real();
    const T b = cn.imag();
    return complex<T>(sin(a) * std::cosh(b), cos(a) * std::sinh(b)); 
  }
  
  /*
   * hyperbolic sine of a complex number
   */
  template<typename T>
  inline complex<T> sinh(const complex<T>& cn) {
    const T a = cn.real();
    const T  b = cn.imag();
    return complex<T>(std::sinh(a) * cos(b), std::cosh(a) * sin(b));
  }

  template<typename T>
  inline complex<T> asin(const complex<T>& cn) {
    complex<T> tmp(log(complex<T>(-cn.imag(),cn.real())+sqrt(1-cn*cn)));
    return complex<T>(tmp.imag(),-tmp.real());;
  }

  /*
   * cube root of a complex number
   *
   * A there are always two solutions for sqrt(x+iy), this method provides
   * the first one consisting in sqrt(m*e^(i*a))=sqrt(m)*e^(i*a/2)
   */
  template<typename T>
  inline complex<T> cbrt(const complex<T>& cn) {
    const T m = abs(cn);
    const T a = arg(cn);
    return polar(pow(double(m),1.0/3.0),m/3.0);
  }

  /*
   * cube root of a complex number (alias for cbrt())
   */
  template<typename T>
  inline complex<T> cubeRoot(const complex<T>& cn) {
    return cbrt(cn);
  }  
  
  /*
   * tangent of a complex number
   */
  template<typename T>
  inline complex<T> tan(const complex<T>& cn) {
    return sin(cn) / cos(cn);
  }

  /*
   * hyperbolic tangent of a complex number
   */
  template<typename T>
  inline complex<T> tanh(const complex<T>& cn) {
    return std::sinh(cn) / std::cosh(cn);
  }

  /*
   * hyperbolic arc tangent of a complex number
   */
  template<typename T>
  inline complex<T> atan(const complex<T>& cn) {
    return log((complex<T>(1)+complex<T>(-cn.real(),cn.imag()))/
               (complex<T>(1)-complex<T>(-cn.real(),cn.imag())))/
      (complex<T>(0,2));
  }

  /*
   * complex number to the power of \a b, with \b real
   */
  template<typename T>
  inline complex<T> pow(const complex<T>& a, const T& b) {
    return exp(b * log(a));
  }

  /*
   * complex number to the power of \a b, with \b complex
   */
  template<typename T>
  inline complex<T> pow(const complex<T>& a, const complex<T>& b) {
    return exp(b * log(a));
  }

  /*
   * real number to the power of \a b, with \b complex
   */
  template<typename T>
  inline complex<T> pow(const T& a, const complex<T>& b) {
    return exp(b * log(a));
  }

} // end namespace lti
