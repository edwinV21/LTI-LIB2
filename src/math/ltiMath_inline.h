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
 * \file   ltiMath_inline.h
 *         Definition of some usually used global functions like min, max, 
 *         and many standard functions with are used differently by
 *         the different compilers.
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   28.04.1999
 *
 * $Id: ltiMath_inline.h,v 1.9 2010-06-23 00:52:31 alvarado Exp $
 */

namespace lti {

  /**
   * Get the minimum of x and y
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T min(const T x, const T y) {
    return ((x<y) ? x : y);
  }

  /**
   * Get the maximum of x and y
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T max(const T x, const T y) {
    return ((x<y) ? y : x);
  }

  /**
   * Get the minimum of x, y and z
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T min(const T x, const T y, const T z) {
    return (min(x,min(y,z)));
  }

  /**
   * Get the maximum of x, y and z
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T max(const T x, const T y, const T z) {
    return (max(x,max(y,z)));
  }

  /**
   * Within intervall
   */
  template<typename T>
  inline T within(const T x, const T lower, const T upper) {
    // use only "<", since it is easier to ensure than both "<" and ">"
    return ((upper<x)?upper:((x<lower)?lower:x));
  }

  /**
   * Store the minimum of x and y in theMin and the maximum in theMax
   */
  template<typename T>
  inline void minmax(const T x, const T y, T& theMin, T& theMax) {
    if (x<y) {
      theMin=x;
      theMax=y;
    } else {
      theMin=y;
      theMax=x;
    }
  }

  /**
   * Store the minimum of x an theMin in theMin and the maximum in theMax
   */
  template<typename T>
  inline void minmax(const T x, T& theMin, T& theMax) {
    if (x<theMin) {
      theMin=x;
    } else if (x>theMax) {
      theMax=x;
    }
  }

  /**
   * Ensure that the minimum of the two given values is stored in theMin and
   * the maximum in theMax.
   */
  template<typename T>
  inline void minmax(T& theMin, T& theMax) {
    if (theMax < theMin) {
      swap(theMin,theMax);
    }
  }

  /**
   * Convert the given angle in degrees to an angle in radians
   */
  inline double degToRad(const double& deg) {
    static const double factor = Pi/180.0;
    return deg*factor;
  }

  /**
   * Convert the given angle in degrees to an angle in radians
   */
  inline float degToRad(const float& deg) {
    static const float factor = static_cast<float>(Pi/180.0);
    return deg*factor;
  }

  /**
   * Convert the given angle in degrees to an angle in radians
   */
  inline double degToRad(const int deg) {
    static const double factor = Pi/180.0;
    return deg*factor;
  }


  /**
   * Convert the given angle in radians to an angle in degrees
   */
  inline double radToDeg(const double& rad) {
    static const double factor = 180.0/Pi;
    return rad*factor;
  }

  /**
   * Convert the given angle in radians to an angle in degrees
   */
  inline float radToDeg(const float& rad) {
    static const float factor = static_cast<float>(180.0/Pi);
    return rad*factor;
  }

  inline byte mod(const byte x,const byte n) {
    const byte tmp(x%n);
    return (tmp>=0) ? tmp : n+tmp;
  }

  inline ubyte mod(const ubyte x,const ubyte n) {
    return x%n;
  }

  inline int32 mod(const int32 x,const int32 n) {
    const int32 tmp(x%n);
    return (tmp>=0) ? tmp : n+tmp;
  }

  inline uint32 mod(const uint32 x,const uint32 n) {
    return x%n;
  }

  /**
   * Absolute value for signed bytes
   *
   * @ingroup gBasicMathFunc
   */
  inline byte abs(byte x) {
    return ((x>=0) ? x : -x);
  }

  /**
   * Absolute value for integers
   *
   * @ingroup gBasicMathFunc
   */
  inline int abs(int x) {
    return ((x>=0) ? x : -x);
  }

  /**
   * Absolute value for any type
   *
   * @ingroup gBasicMathFunc
   */
  inline float abs(float x) {
#ifdef _LTI_MATH_H_STYLE
    return ((x>=0) ? x : -x);
#else
    return std::abs(x);
#endif
  }

  /**
   * Absolute value for any type
   *
   * @ingroup gBasicMathFunc
   */
  inline double abs(const double& x) {
#ifdef _LTI_MATH_H_STYLE
    return ((x>=0) ? x : -x);
#else
    return std::abs(x);
#endif
  }

  /**
   * Absolute difference for the given values
   * equals if (x>y) then (x-y) else (y-x).
   * 
   * Note that with complex numbers it does not return the real abs value.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T absdiff(const T& x,const T& y) {
    return ((x>y) ? (x-y) : (y-x));
  }

  /**
   * Compute the reciprocal of the given value \e x, defined as 1/x.
   */
  inline double reciprocal(const double& x) {
    return 1.0/x;
  }

  /**
   * Compute the reciprocal of the given value \e x, defined as 1/x.
   */
  inline float reciprocal(const float& x) {
    return 1.0f/x;
  }

  /**
   * Compute 1 minus the given value.
   *
   * This function is usually employed to revert a probability value
   */
  inline double oneMinus(const double& x) {
    return 1.0-x;
  }

  /**
   * Compute 1 minus the given value
   *
   * This function is usually employed to revert a probability value
   */
  inline float oneMinus(const float& x) {
    return 1.0f-x;
  }

  /**
   * Compute 1 minus the given value
   */
  inline int oneMinus(const int x) {
    return 1-x;
  }

  /**
   * Rectify is 0 if x<0 or x otherwise
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T rectify(const T& x) {
    return ((x>=0) ? x : 0);
  }

  /**
   * Square (x*x)
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqr(const T& x) {
    return (x*x);
  }

  /*
   * Square of the magnitude of a number (|x|*|x|)
   *
   * This function is useful in generic programming for those cases where
   * complex numbers will occur in products of conjugate pairs.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T sqrAbs(const T& x) {
    return x*x;
  }

  /**
   * Return the conjugate of a scalar number, i.e. the same number.
   *
   * This function is used in generic programming algorithms in which also
   * complex numbers are expected.
   */
  template<typename T>
  inline T conj(const T cn) {
    return cn;
  }

  /**
   * Square root of integer type.  Equals floor(sqrt(x))
   *
   * @ingroup gBasicMathFunc
   */
  inline int sqrt(int x) {
    return static_cast<int>(::sqrt(static_cast<double>(x)));
  }

  /**
   * Square root
   *
   * @ingroup gBasicMathFunc
   */
  inline unsigned int sqrt(unsigned int x) {
    return static_cast<unsigned int>(::sqrt(static_cast<double>(x)));
  }

  /**
   * Square root
   *
   * @ingroup gBasicMathFunc
   */
  inline float sqrt(float x) {
    return static_cast<float>(::sqrt(x));
  }

  /**
   * Square root
   *
   * @ingroup gBasicMathFunc
   */
  inline double sqrt(const double& x) {
#ifdef _LTI_MATH_H_STYLE
    // MS Visual does not support std namespace functions, which
    // can be optimized for the different types
    return static_cast<double>(::sqrt(x));
#else
    // use std method optimized for double
    return static_cast<double>(std::sqrt(x));
#endif
  }

  /**
   * Square root of rectified value, i.e. returns 0 if x<=0 or sqrt(x)
   * otherwise.
   */
  template<typename T>
  inline T sqrtrect(const T x) {
    if (x<=static_cast<T>(0)) {
      return static_cast<T>(0);
    }
    return sqrt(x);
  }

  // other standard functions used

#ifdef _LTI_MATH_H_STYLE
  /**
   * Sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline float sin(float x) {
    return static_cast<float>(::sin(x));
  }

  /**
   * Sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double sin(const double& x) {
    return ::sin(x);
  }

  /**
   * Cosinus
   *
   * @ingroup gBasicMathFunc
   */
  inline float cos(float x) {
    return static_cast<float>(::cos(x));
  }

  /**
   * Cosinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double cos(const double& x) {
    return ::cos(x);
  }

  /**
   * Tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float tan(float x) {
    return static_cast<float>(::tan(x));
  }

  /**
   * Tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double tan(const double& x) {
    return ::tan(x);
  }

  /**
   * Arc sine
   *
   * @ingroup gBasicMathFunc
   */
  inline float asin(float x) {
    return static_cast<float>(::asin(x));
  }

  /**
   * Arc sine
   *
   * @ingroup gBasicMathFunc
   */
  inline double asin(const double& x) {
    return ::asin(x);
  }

  /**
   * Arc cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline float acos(float x) {
    return static_cast<float>(::acos(x));
  }

  /**
   * Arc cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline double acos(const double& x) {
    return ::acos(x);
  }

  /**
   * Arc tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float atan(float x) {
    return static_cast<float>(::atan(x));
  }

  /**
   * Arc tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double atan(const double& x) {
    return ::atan(x);
  }

  /**
   * Arc tangent of y/x
   *
   * @see lti::arctanLUT
   *
   * @ingroup gBasicMathFunc
   */
  inline float atan2(float y,float x) {
    return static_cast<float>(::atan2(y,x));
  }

  /**
   * Arc tangent of y/x
   *
   * @see lti::arctanLUT
   *
   * @ingroup gBasicMathFunc
   */
  inline double atan2(const double& y,const double& x) {
    return ::atan2(y,x);
  }

  /**
   * Hyperbolic sine
   *
   * @ingroup gBasicMathFunc
   */
  inline double sinh(const double& x) {
    return ::sinh(x);
  }

  /**
   * Hyperbolic sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double sinh(float x) {
    return static_cast<float>(::sinh(x));
  }

  /**
   * Hyperbolic cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline float cosh(float x) {
    return static_cast<float>(::cosh(x));
  }

  /**
   * Hyperbolic cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline double cosh(const double& x) {
    return ::cosh(x);
  }

  /**
   * Hyperbolic tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float tanh(float x) {
    return static_cast<float>(::tanh(x));
  }

  /**
   * Hyperbolic tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double tanh(const double& x) {
    return ::tanh(x);
  }

  /**
   * Natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline float log(float x) {
    return static_cast<float>(::log(x));
  }

  /**
   * Natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline double log(const double& x) {
    return ::log(x);
  }

  /**
   * Natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline double log(int x) {
    return ::log(static_cast<double>(x));
  }

  /**
   * Logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline float log10(float x) {
    return static_cast<float>(::log10(x));
  }

  /**
   * Logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline double log10(const double& x) {
    return ::log10(x);
  }

  /**
   * Logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline double log10(int x) {
    return ::log10(static_cast<double>(x));
  }

  /**
   * Natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline float exp(float x) {
    return static_cast<float>(::exp(x));
  }

  /**
   * Natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline double exp(const double& x) {
    return ::exp(x);
  }

  /**
   * Natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline double exp(int x) {
    return ::exp(static_cast<double>(x));
  }

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double pow(const double& x, const double& y) {
    return ::pow(x,y);
  }

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float pow(float x,float y) {
    return static_cast<float>(::pow(x,y));
  }

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double pow(const double& x, int y) {
    return ::pow(x,y);
  }

  /**
   * Power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float pow(float x,int y) {
    return static_cast<float>(::pow(x,y));
  }

#else
  // methods optimized for different types
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
#endif

  /**
   * Sigmoid for floats.
   *
   * The sigmoid is defined as \$f\frac{1}{1+\exp(-x)}\$f
   *
   * @ingroup gBasicMathFunc
   */
  inline float sigmoid(const float x) {
    return static_cast<float>(1.0/(1.0+exp(-x)));
  }

  /**
   * Sigmoid for doubles
   *
   * The sigmoid is defined as \$f\frac{1}{1+\exp(-x)}\$f
   *
   * @ingroup gBasicMathFunc
   */
  inline double sigmoid(const double& x) {
    return 1.0/(1.0+exp(-x));
  }

  /**
   * Sigmoid for ints
   *
   * The sigmoid is defined as \$f\frac{1}{1+\exp(-x)}\$f
   *
   * @ingroup gBasicMathFunc
   */
  inline double sigmoid(const int x){
    return 1.0/(1.0+exp(-static_cast<double>(x)));
  }

  /**
   * Signum of x (for x==0 -> signum = 1) (@see signum0)
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T signum(T x) {
    return ((x>=T()) ? T(1) : T(-1));
  }

  /**
   * Signum of x (for x==0 -> signum = 0) (@see signum)
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline T signum0(T x) {
    return ((x>0) ? T(1) : ((x<0) ? T(-1) : T(0)));
  }

  /**
   * Return true if the given number is an even number. 
   *
   * For floating point types, this function will return true only if the
   * number is \e exacltly an integer and it is even.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline bool even(const T x) {
    return ((x & static_cast<T>(1)) == 0); 
  }

  template<>
  inline bool even(const double x) {
    const int64 i = static_cast<int64>(x);
    return ((static_cast<double>(i) == x) && even(i));
  }

  template<>
  inline bool even(const float x) {
    const int i = static_cast<int>(x);
    return ((static_cast<float>(i) == x) && even(i));
  }


  /**
   * Return true if the given number is an odd number.
   *
   * For floating point types, this function will return true only if the
   * number is \e exacltly an integer and it is odd.
   *
   * @ingroup gBasicMathFunc
   */
  template<typename T>
  inline bool odd(const T x) {
    return ((x & static_cast<T>(1)) != 0); 
  }

  template<>
  inline bool odd(const double x) {
    const int64 i = static_cast<int64>(x);
    return ((static_cast<double>(i) == x) && odd(i));
  }

  template<>
  inline bool odd(const float x) {
    const int i = static_cast<int>(x);
    return ((static_cast<float>(i) == x) && odd(i));
  }
}
