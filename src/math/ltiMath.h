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
 * \file   ltiMath.h
 *         Definition of some usually used global functions like min, max, 
 *         and many standard functions with are used differently by
 *         the different compilers.
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   28.04.1999
 *
 * $Id: ltiMath.h,v 1.17 2012-11-17 14:49:05 alvarado Exp $
 */

#ifndef _LTI_MATH_H_
#define _LTI_MATH_H_

#include "ltiObject.h"
#include "ltiTypes.h"
#include "ltiSinCos.h"
#include <cmath>
#include <limits>

/*
 * MS Visual C++ 6.0 and old GCC versions do not provide the standard
 * cmath interface of functions for several types in the namespace std.
 * For this compilers a cumbersome implementation must be provided.
 */
#undef _LTI_MATH_H_STYLE
#if (defined _LTI_MSC_6) || (defined _LTI_GNUC_2 )
#define _LTI_MATH_H_STYLE 1
#endif

# ifdef min
#   undef min
# endif

# ifdef max
#   undef max
# endif

namespace lti {

  /**
   * Constant Pi (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  extern const double Pi; //3.1415926535897932384626433832795;

  /**
   * Constant Not a Number (NaN) (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  extern const double NaN;

  /**
   * Constant Infinity (Inf) (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  extern const double Inf;

  /**
   * Returns true if \a a is close to \a b, i.e. if \a abs(a-b) <= \a epsilon.
   *
   * This is always better than testing equality with a==b for
   * floating point types. For default \a epsilon integers are tested
   * for equality.
   *
   * \b Note: Do not used for unsigned types T.
   * 
   * @param a first value
   * @param b second value
   * @param epsilon max difference between \a a and \a b
   *                         , default std::numeric_limits<T>::epsilon()
   * @returns true if distance between \a a and \a b is <= \a epsilon 
   *
   * @ingroup gBasicMathFunc
   */
  template <class T>
  inline bool closeTo(const T& a, const T& b, 
                      const T epsilon=std::numeric_limits<T>::epsilon()) {
    const T diff=a-b;
    return ((diff<=epsilon) && (diff>=-epsilon));
  }

  
  /**
   * Returns true if \a a is close to T(0), i.e. if abs(a) <= \a epsilon.
   *
   * This is always better than testing equality with a==0. for
   * floating point types. For default \a epsilon integers are tested
   * for ==0 automagically.
   *
   * \b Note: Do not used for unsigned types T.
   *
   * @param a value
   * @param epsilon maximum absolute value of \a a 
   *                         , default std::numeric_limits<T>::epsilon()
   * @returns true if absolute value of \a a is <= \a epsilon 
   *
   * @ingroup gBasicMathFunc
   */
  template <class T>
  inline bool closeToZero(const T& a,
                          const T epsilon=std::numeric_limits<T>::epsilon()) {
    return ((a<=epsilon) && (a>=-epsilon));
  }

  /*
   * Exchange the content of the two given variables.  It works for all
   * types T that properly define the copy constructor and operator=().
   *
   * @param a first variable
   * @param b second variable
   *
   * At the end \a b contains the previous contents of \a a, which at the
   * same time contains the previous contents of \a b.
   * 
   * @ingroup gBasicMathFunc
   */
//   template<typename T>
//   inline void swap(T& a,T& b);

  /**
   * Get the minimum of x and y
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T min(const T x, const T y);

  /**
   * Get the maximum of x and y
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T max(const T x, const T y);

  /**
   * Get the minimum of x, y and z
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T min(const T x, const T y, const T z);

  /**
   * Get the maximum of x, y and z
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T max(const T x, const T y, const T z);

  /**
   * Check if the \a x value lies within the lower and upper bounds, and
   * if not return the closest bound.
   *
   * This is equivalent (but faster than)  lti::max(lti::min(x,upper),lower).
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T within(const T x, const T lower, const T upper);

  /**
   * Store the minimum of x and y in theMin and the maximum in theMax
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline void minmax(const T x, const T y, T& theMin, T& theMax);

  /**
   * Store the minimum of x and theMin in theMin and the maximum of
   * x and theMax in theMax.
   *
   * \warning This method assumes that theMin <= theMax, and if this condition
   * is not met, be carefull as just one condition will be ensured, i.e. wether
   * theMin will be ok or theMax, but not both.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline void minmax(const T x, T& theMin, T& theMax);

  /**
   * Ensure that the minimum of the two given values is stored in theMin and
   * the maximum in theMax.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline void minmax(T& theMin, T& theMax);

  /**
   * Convert the given angle in degrees to an angle in radians
   *
   * @ingroup gBasicMathFunc
   */
  inline double degToRad(const double& deg);

  /**
   * Convert the given angle in degrees to an angle in radians
   *
   * @ingroup gBasicMathFunc
   */
  inline float degToRad(const float& deg);

  /**
   * Convert the given angle in degrees to an angle in radians
   *
   * @ingroup gBasicMathFunc
   */
  inline double degToRad(const int deg);

  /**
   * Convert the given angle in radians to an angle in degrees
   *
   * @ingroup gBasicMathFunc
   */
  inline double radToDeg(const double& rad);

  /**
   * Convert the given angle in radians to an angle in degrees
   *
   * @ingroup gBasicMathFunc
   */
  inline float radToDeg(const float& rad);


  /**
   * Modulus
   *
   * With positive values this is equivalent to the remainer of the x/n
   * division.
   *
   * The diference between the modulus operation and the default operator% is
   * that this function returns always a positive value, whereas the default C
   * operator can return negative values.  For instance -1 % 5 returns -5,
   * but mod(-1,5) returns 4.
   */
  byte mod(const byte x,const byte n);

  /**
   * Modulus
   *
   * With positive values this is equivalent to the remainer of the x/n
   * division.
   *
   * The diference between the modulus operation and the default operator% is
   * that this function returns always a positive value, whereas the default C
   * operator can return negative values.  For instance -1 % 5 returns -5,
   * but mod(-1,5) returns 4.
   */
  ubyte mod(const ubyte x,const ubyte n);

  /**
   * Modulus
   *
   * With positive values this is equivalent to the remainer of the x/n
   * division.
   *
   * The diference between the modulus operation and the default operator% is
   * that this function returns always a positive value, whereas the default C
   * operator can return negative values.  For instance -1 % 5 returns -5,
   * but mod(-1,5) returns 4.
   */
  int mod(const int x,const int n);

  /**
   * Modulus
   *
   * With positive values this is equivalent to the remainer of the x/n
   * division.
   *
   * The diference between the modulus operation and the default operator% is
   * that this function returns always a positive value, whereas the default C
   * operator can return negative values.  For instance -1 % 5 returns -5,
   * but mod(-1,5) returns 4.
   */
  uint32 mod(const uint32 x,const uint32 n);

  /**
   * Absolute value for signed bytes
   *
   * @ingroup gBasicMathFunc
   */
  inline byte abs(byte x);

  /**
   * Absolute value for integers
   *
   * @ingroup gBasicMathFunc
   */
  inline int abs(int x);

  /**
   * Absolute value for any type
   *
   * @ingroup gBasicMathFunc
   */
  inline float abs(float x);

  /**
   * Absolute value for any type
   *
   * @ingroup gBasicMathFunc
   */
  inline double abs(const double& x);

  /**
   * Absolute difference for the given values
   * equals if (x>y) then (x-y) else (y-x).
   * 
   * Note that with complex numbers it does not return the real abs value.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T absdiff(const T& x,const T& y);

  /**
   * Compute the reciprocal of the given value \e x, defined as 1/x.
   *
   * @ingroup gBasicMathFunc
   */
  inline double reciprocal(const double& x);

  /**
   * Compute the reciprocal of the given value \e x, defined as 1/x.
   *
   * @ingroup gBasicMathFunc
   */
  inline float reciprocal(const float& x);

  /**
   * Compute 1 minus the given value.
   *
   * This function is usually employed to revert a probability value
   *
   * @ingroup gBasicMathFunc
   */
  inline double oneMinus(const double& x);

  /**
   * Compute 1 minus the given value
   *
   * This function is usually employed to revert a probability value
   *
   * @ingroup gBasicMathFunc
   */
  inline float oneMinus(const float& x);

  /**
   * Compute 1 minus the given value
   *
   * @ingroup gBasicMathFunc
   */
  inline int oneMinus(const int x);

  /**
   * Rectify is 0 if x<0 or x otherwise
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T rectify(const T& x);

  /**
   * Square (x*x)
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqr(const T& x);

  /**
   * Square of the magnitude of a number (|x|*|x|)
   *
   * This function is useful in generic programming for those cases where
   * complex numbers will occur in products of conjugate pairs.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqrAbs(const T& x);

  /**
   * Return the conjugate of a scalar number, i.e. the same number.
   *
   * This function is used in generic programming algorithms in which also
   * complex numbers are expected.
   */
  template<typename T>
  inline T conj(const T cn);

  /**
   * Square root of integer type.  Equals floor(sqrt(x))
   *
   * @ingroup gBasicMathFunc
   */
  inline int sqrt(int x);

  /**
   * Square root
   *
   * @ingroup gBasicMathFunc
   */
  inline unsigned int sqrt(unsigned int x);

  /**
   * Square root
   *
   * @ingroup gBasicMathFunc
   */
  inline float sqrt(float x);

  /**
   * Square root
   *
   * @ingroup gBasicMathFunc
   */
  inline double sqrt(const double& x);

  /**
   * Square root of rectified value, i.e. returns 0 if x<=0 or sqrt(x)
   * otherwise.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqrtrect(const T x);

 // other standard functions used

  /**
   * Calculate the sine and cosine values of \p angle in one step if
   * the setup allows it.
   *
   * @param angle the angle
   * @param sval sine of the \p angle
   * @param cval cosine of the \p angle
   */
  template <class T>
  inline void sincos(T angle, T& sval, T& cval) {
    internal::sincos_impl(static_cast<double>(angle),
                static_cast<double&>(sval),
                static_cast<double&>(cval));
  }

  // overload for float
  inline void sincos(float angle, float& sval, float& cval) {
    internal::sincosf_impl(angle,sval,cval);
  }

  // other standard functions used

#ifdef _LTI_MATH_H_STYLE
  /**
   * \name Standard math functions
   */
  //@{
  /**
   * Sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline float sin(float x);

  /**
   * Sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double sin(const double& x);

  /**
   * Cosinus
   *
   * @ingroup gBasicMathFunc
   */
  inline float cos(float x);

  /**
   * Cosinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double cos(const double& x);

  /**
   * Tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float tan(float x);

  /**
   * Tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double tan(const double& x);

  /**
   * Arc sine
   *
   * @ingroup gBasicMathFunc
   */
  inline float asin(float x);

  /**
   * Arc sine
   *
   * @ingroup gBasicMathFunc
   */
  inline double asin(const double& x);

  /**
   * Arc cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline float acos(float x);

  /**
   * Arc cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline double acos(const double& x);

  /**
   * Arc tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float atan(float x);

  /**
   * Arc tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double atan(const double& x);

  /**
   * Arc tangent of y/x
   *
   * @see lti::arctanLUT
   *
   * @ingroup gBasicMathFunc
   */
  inline float atan2(float y,float x);

  /**
   * Arc tangent of y/x
   *
   * @see lti::arctanLUT
   *
   * @ingroup gBasicMathFunc
   */
  inline double atan2(const double& y,const double& x);

  /**
   * Hyperbolic sine
   *
   * @ingroup gBasicMathFunc
   */
  inline double sinh(const double& x);

  /**
   * Hyperbolic sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double sinh(float x);

  /**
   * Hyperbolic cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline float cosh(float x);

  /**
   * Hyperbolic cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline double cosh(const double& x);

  /**
   * Hyperbolic tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float tanh(float x);

  /**
   * Hyperbolic tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double tanh(const double& x);

  /**
   * Natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline float log(float x);

  /**
   * Natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline double log(const double& x);

  /**
   * Natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline double log(int x);

  /**
   * Logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline float log10(float x);

  /**
   * Logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline double log10(const double& x);

  /**
   * Logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline double log10(int x);

  /**
   * Natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline float exp(float x);

  /**
   * Natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline double exp(const double& x);

  /**
   * Natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline double exp(int x);

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double pow(const double& x, const double& y);

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float pow(float x,float y);

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double pow(const double& x, int y);

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float pow(float x,int y);
  //@}

#else
  // methods optimized for different types
  /**
   * \name Standard math functions
   */
  //@{
  using std::swap;
  using std::sin;
  using std::cos;
  using std::tan;
  using std::asin;
  using std::acos;
  using std::atan;
  using std::atan2;
  using std::sinh;
  using std::cosh;
  using std::tanh;
  using std::log;
  using std::log10;
  using std::exp;
  using std::pow;
  //@}
#endif

  /**
   * Sigmoid for floats.
   *
   * The sigmoid is defined as \f$\frac{1}{1+\exp(-x)}\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float sigmoid(const float x);

  /**
   * Sigmoid for doubles
   *
   * The sigmoid is defined as \f$\frac{1}{1+\exp(-x)}\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double sigmoid(const double& x);

  /**
   * Sigmoid for ints
   *
   * The sigmoid is defined as \f$\frac{1}{1+\exp(-x)}\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double sigmoid(const int x);

  /**
   * Signum of x (for x==0 -> signum = 1) (@see signum0)
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T signum(T x);

  /**
   * Signum of x (for x==0 -> signum = 0) (@see signum)
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T signum0(T x);

  /**
   * Return true if the given number is an even number. 
   *
   * For floating point types, this function will return true only if the
   * number is \e exacltly an integer and it is even.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline bool even(const T x);

  /**
   * Return true if the given number is an odd number.
   *
   * For floating point types, this function will return true only if the
   * number is \e exacltly an integer and it is odd.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline bool odd(const T x);
}

#include "ltiMath_inline.h"

#endif
