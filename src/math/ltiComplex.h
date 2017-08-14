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


/**
 * \file   ltiComplex.h
 *         LTI-Lib version of the complex numbers.  There are a few features
 *         besides the std::complex, but both types are compatible.
 * \author Pablo Alvarado
 * \date   06.04.2003
 *
 * $Id: ltiComplex.h,v 1.5 2009-08-31 14:04:03 alvarado Exp $
 */

#ifndef _LTI_COMPLEX_H
#define _LTI_COMPLEX_H

#include "ltiMath.h"
#include "ltiIoHandler.h"
#include <complex>
#include <iostream>

namespace lti {    
  /**
   * Class to represent and manipulate complex numbers.
   *
   * The complex numbers contain a real and an imaginary part, which can
   * be accessed through the methods real() and imag().
   *
   * The interface of this class is similar to the one provided by the
   * Standard C++ library.  It has being included in the LTI-Lib because 
   * different library versions do not provide the same functionality.
   *
   * Note that many of the provided mathematical operations make only sense if
   * the template type T used for the components of the complex number is
   * double or float.
   */
  template<typename T>
  class complex {
  public:
    typedef T value_type;
    
    /**
     * Return the real component of the complex number
     */
    T real() const;
    
    /**
     * Return the imaginary component of the complex number
     */
    T imag() const;

    /**
     * Default constructor.
     *
     * @param re real component
     * @param im imaginary component
     */
    inline complex(const T& re = T(), const T& im = T());
    
    /**
     * Copy constructor
     */
    template<class U>
    inline complex(const complex<U>& other);
    
    /**
     * Cast a std::complex to an lti::complex of the same type
     */
    inline complex(const std::complex<T>& other);

    /**
     * Cast a lti::complex into a std::complex of the same type
     */
    inline operator std::complex<T>() const;
   
    /**
     * Copy a real number into this complex one, making the imaginary part
     * zero.
     */
    inline complex<T>& operator=(const T& other);
    
    /**
     * Add a real number to this complex one.
     */     
    inline complex<T>& operator+=(const T& other);
    
    /**
     * Subtract a real number to this complex one.
     */     
    inline complex<T>& operator-=(const T& other);
    
    /**
     * Multiply this complex number with a real one.
     */     
    inline complex<T>& operator*=(const T& other);
    
    /**
     * Divide this complex number by a real one.
     */     
    inline complex<T>& operator/=(const T& other);
    
    /**
     * Copy another complex number into this one
     */
    template<typename U>
    inline complex<T>& operator=(const complex<U>& other);
    
    /**
     * Add another complex number
     */
    template<typename U>
    inline complex<T>& operator+=(const complex<U>& other);
    
    /**
     * subtract another complex number
     */
    template<typename U>
    inline complex<T>& operator-=(const complex<U>& other);
    
    /**
     * multiply with another complex number
     */
    template<typename U>
    inline complex<T>& operator*=(const complex<U>& other);
    
    /**
     * divide with another complex number
     */
    template<typename U>
    inline complex<T>& operator/=(const complex<U>& other);

    /**
     * Set real and imaginary components at once.
     */
    inline void set(const T& re, const T& im = T());

    /**
     * Set real part
     */
    inline void setReal(const T& re);

    /**
     * Set imaginary part
     */
    inline void setImag(const T& im);

    /**
     * Get the real and imaginary components at once.
     */
    inline void get(T& re, T& im) const;

  private:
    /**
     * real component
     */
    T realPart_;
    
    /**
     * imaginary component
     */
    T imagPart_;
  };

  // -------------------------------------------------------------------------
  // global operators
  // -------------------------------------------------------------------------
  /**
   * @name Operators for complex numbers
   */
  //@{
    
  /**
   * Add two complex numbers
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a, const complex<T>& b);

  /**
   * Add a complex number with a real number
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a, const T& b);

  /**
   * Add a real number with a complex number
   */
  template<typename T>
  inline complex<T> operator+(const T& a, const complex<T>& b);

  /**
   * Subtract two complex numbers
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a, const complex<T>& b);
    
  /**
   * Subtract a real number from a complex one
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a, const T& b);

  /**
   * Subtract a complex number from a real one
   */
  template<typename T>
  inline complex<T> operator-(const T& a, const complex<T>& b);

  /**
   * Multiply two complex numbers
   */
  template<typename T>
  inline complex<T> operator*(const complex<T>& a, const complex<T>& b);

  /**
   * Multiply a complex number with a real one
   */
  template<typename T>
  inline complex<T> operator*(const complex<T>& a, const T& b);

  /**
   * Multiply a real number with a complex one
   */
  template<typename T>
  inline complex<T> operator*(const T& a, const complex<T>& b);

  /**
   * Divide two complex numbers
   */
  template<typename T>
  inline complex<T> operator/(const complex<T>& a, const complex<T>& b);
    
  /**
   * Divide a complex number by a real one
   */
  template<typename T>
  inline complex<T> operator/(const complex<T>& a, const T& b);

  /**
   * Divide a real number by a complex one
   */
  template<typename T>
  inline complex<T> operator/(const T& a, const complex<T>& b);

  /**
   * Plus sign a complex number
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a);

  /**
   * Minus sign a complex number
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a);

  /**
   * Compare if two complex numbers are equal
   */
  template<typename T>
  inline bool operator==(const complex<T>& a, const complex<T>& b);

  /**
   * Compare if two complex numbers differ
   */
  template<typename T>
  inline bool operator==(const complex<T>& a, const T& b);

  /**
   * Compare if a real number and a complex number are the same
   */
  template<typename T>
  inline bool operator==(const T& a, const complex<T>& b);

  /**
   * Compare if two complex numbers differ
   */
  template<typename T>
  inline bool operator!=(const complex<T>& a, const complex<T>& b);

  /**
   * Compare if a complex number and a real one differ
   */
  template<typename T>
  inline bool operator!=(const complex<T>& a, const T& b);

  /**
   * Compare if a real number and a complex one differ
   */
  template<typename T>
  inline bool operator!=(const T& a, const complex<T>& b);

  /**
   * A complex number is called here "less than" another one if its real part
   * is smaller, or when both real parts are identical, if its imaginary part
   * is smaller
   */
  template<typename T>
  inline bool operator<(const complex<T>& a,const complex<T>& b);

  /**
   * A complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const complex<T>& a,const T& b);

  /**
   * A complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const T& a,const complex<T>& b);

  /**
   * A complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const complex<T>& a,const complex<T>& b);

  /**
   * A complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const complex<T>& a,const T& b);

  /**
   * A complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const T& a,const complex<T>& b);

  /**
   * Get the real part of a complex number
   */
  template<typename T>
  inline T real(const complex<T>& cn);
    
  /**
   * Get the imaginary part of a complex number
   */
  template<typename T>
  inline T imag(const complex<T>& cn);

  /**
   * Square of the magnitude of a number (|x|*|x|)
   *
   * This function is useful in generic programming for those cases where
   * complex numbers will occur in products of conjugate pairs.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqrAbs(const complex<T>& x);

  /**
   * Get the absolute value of a complex number
   */
  template<typename T>
  T abs(const complex<T>& cn);

  /**
   * Get the argument (angle or phase) of a complex number
   */
  template<typename T>
  inline T arg(const complex<T>& cn);

  /**
   * Return the square magnitude of the given complex number
   */ 
  template<typename T>
  inline T norm(const complex<T>& cn);

  /**
   * Convert the given polar values into a complex number
   *
   * @param radius magnitude of the number
   * @param angle  angle of the number
   * 
   * @return Equivalent complex number to the polar one
   */
  template<typename T>
  inline complex<T> polar(const T& radius, const T& angle);

  /**
   * Return the conjugate of the complex number
   */
  template<typename T>
  inline complex<T> conj(const complex<T>& cn);
  
  /**
   * Cosine of a complex number
   */
  template<typename T>
  inline complex<T> cos(const complex<T>& cn);

  /**
   * Hyperbolic cosine of a complex number
   */
  template<typename T>
  inline complex<T> cosh(const complex<T>& cn);

  template<typename T>
  inline complex<T> acos(const complex<T>& cn);

  /**
   * Exponetial of a complex number
   */
  template<typename T>
  inline complex<T> exp(const complex<T>& cn);

  /**
   * Natural logarithm of a complex number (base \e e)
   */
  template<typename T>
  inline complex<T> log(const complex<T>& cn);

  /**
   * Logarithm base 10 of a complex number
   */
  template<typename T>
  inline complex<T> log10(const complex<T>& cn);

  /**
   * Sine of a complex number
   */
  template<typename T>
  inline complex<T> sin(const complex<T>& cn);
  
  /**
   * Hyperbolic sine of a complex number
   */
  template<typename T>
  inline complex<T> sinh(const complex<T>& cn);

  /**
   * Arc sine of a complex number
   */
  template<typename T>
  inline complex<T> asin(const complex<T>& cn);

  /**
   * Square root of a complex number.
   *
   * A there are always two solutions for sqrt(x+iy), this method provides
   * the first one consisting in sqrt(m*e^(i*a))=sqrt(m)*e^(i*a/2)
   */
  template<typename T>
  complex<T> sqrt(const complex<T>& cn);

  /**
   * Cube root of a complex number
   *
   * A there are always two solutions for sqrt(x+iy), this method provides
   * the first one consisting in sqrt(m*e^(i*a))=sqrt(m)*e^(i*a/2)
   */
  template<typename T>
  inline complex<T> cbrt(const complex<T>& cn);

  /**
   * Cube root of a complex number (alias for cbrt())
   */
  template<typename T>
  inline complex<T> cubeRoot(const complex<T>& cn);
  
  /**
   * Tangent of a complex number
   */
  template<typename T>
  inline complex<T> tan(const complex<T>& cn);

  /**
   * Hyperbolic tangent of a complex number
   */
  template<typename T>
  inline complex<T> tanh(const complex<T>& cn);

  /**
   * Hyperbolic arc tangent of a complex number
   */
  template<typename T>
  inline complex<T> atan(const complex<T>& cn);

  /**
   * Complex number to the power of \a b, with \b real
   */
  template<typename T>
  inline complex<T> pow(const complex<T>& a, const T& b);

  /**
   * Complex number to the power of \a b, with \b complex
   */
  template<typename T>
  inline complex<T> pow(const complex<T>& a, const complex<T>& b);

  /**
   * Real number to the power of \a b, with \b complex
   */
  template<typename T>
  inline complex<T> pow(const T& a, const complex<T>& b);

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,complex<T>& p,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,const complex<T>& p,const bool complete=true);

  /**
   * Alias for lti::complex<float>
   */
  typedef complex<float> fcomplex;

  /**
   * Alias for lti::complex<double>
   */
  typedef complex<double> dcomplex;

} // end namespace lti

namespace std {

  template<typename T>
  istream& operator>>(istream& in, lti::complex<T>& a);

  template<typename T>
  inline ostream& operator<<(ostream& out, const lti::complex<T>& a);

} // namespace std

#include "ltiComplex_inline.h"
#include "ltiComplex_template.h"

#endif 
