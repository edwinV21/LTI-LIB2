/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiCorrelation_template.h
 *         Provides implementation of the accumulators for the correlation
 *         functor.
 * \author Pablo Alvarado
 * \date   17.11.99
 *
 * $Id: ltiCorrelation_template.h,v 1.1 2007-09-24 19:01:05 alvarado Exp $
 */

namespace lti {

  /* --------------------------------------------------------
   * CLASSIC
   * --------------------------------------------------------
   */

  /*
   * default constructor
   */
  template<class T,class U>
  correlation::classic<T,U>::classic() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
  void correlation::classic<T,U>::reset() {
    state_ = static_cast<U>(0);
  }

  /*
   * classic (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void correlation::classic<T,U>::accumulate(const T& filter,
                                             const T& src) {
    state_ += static_cast<U>(src*filter);
  }

  template<class T,class U>
  void correlation::classic<T,U>::accumulateZero(const T&) {
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::classic<T,U>::accumulateSym(const T& filter,
                                                const T& srcL,
                                                const T& srcR) {
    state_ += static_cast<U>((srcL+srcR)*filter);
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::classic<T,U>::accumulateASym(const T& filter,
                                                 const T& srcL,
                                                 const T& srcR) {
    state_ += static_cast<U>((srcL-srcR)*filter);
  }
  
  
  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T correlation::classic<T,U>::getResult() const {
    return (static_cast<T>(state_));
  }
    
  /*
   * set norm
   */
  template<class T,class U>
  void correlation::classic<T,U>::setNorm(const T& n) {
    norm_ = n;
  }
  

  /* --------------------------------------------------------
   * COEFFICIENT
   * --------------------------------------------------------
   */

  /*
   * default constructor
   */
  template<class T,class U>
  correlation::coefficient<T,U>::coefficient() {
    reset();
  }
  
  /*
   * reset
   */
  template<class T,class U>
  void correlation::coefficient<T,U>::reset() {
    sumW_=sumWW_=sumFW_=sumFF_=sumF_=static_cast<U>(0);
  }
  
  /*
   * coefficient (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void correlation::coefficient<T,U>::accumulate(const T& filter,
                                                 const T& src) {

    U tmp = static_cast<U>(filter-kernAvg_);
    sumW_+=tmp;
    sumWW_+=(tmp*tmp);
    sumFW_+=(src*tmp);
    sumFF_+=(src*src);
    sumF_+=src;
  }

  template<class T,class U>
  void correlation::coefficient<T,U>::accumulateZero(const T& src) {

    U tmp = static_cast<U>(-kernAvg_);
    sumW_+=tmp;
    sumWW_+=(tmp*tmp);
    sumFW_+=(src*tmp);
    sumFF_+=(src*src);
    sumF_+=src;
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::coefficient<T,U>::accumulateSym(const T& filter,
                                                    const T& srcL,
                                                    const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }
  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::coefficient<T,U>::accumulateASym(const T& filter,
                                                     const T& srcL,
                                                     const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T correlation::coefficient<T,U>::getResult() const {
    U tmp = (sumFF_-(sumF_*sumF_/kernSize_))*sumWW_;
    if (tmp>0)
      return static_cast<T>((sumFW_-(sumF_*sumW_/kernSize_))/sqrt(tmp));
    else
      return 0;
  }


  template<class T,class U>
  void correlation::coefficient<T,U>::setData(const T& kernelAverage,
                                              const T& kernelSize)  {
    kernSize_ = kernelSize;
    kernAvg_  = kernelAverage;
  }

  /*
   * set norm
   */
  template<class T,class U>
  void
  correlation::coefficient<T,U>::setNorm(const T& n) {
    norm_ = n;
  }


  /* --------------------------------------------------------
   * MATCHING CRITERIUM 1
   * --------------------------------------------------------
   */

  /*
   * default constructor
   */
  template<class T,class U>
  correlation::criterium1<T,U>::criterium1() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
  void correlation::criterium1<T,U>::reset() {
    state_ = static_cast<U>(0);
  }

  /*
   * criterium1 (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void correlation::criterium1<T,U>::accumulate(const T& filter,
                                                const T& src) {
    if (src>filter) {
      state_ = max(state_,static_cast<U>(src-filter));
    } else {
      state_ = max(state_,static_cast<U>(filter-src));
    }
  }

  template<class T,class U>
  void correlation::criterium1<T,U>::accumulateZero(const T& src) {
    accumulate(T(0),src);
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::criterium1<T,U>::accumulateSym(const T& filter,
                                                   const T& srcL,
                                                   const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }
  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::criterium1<T,U>::accumulateASym(const T& filter,
                                                    const T& srcL,
                                                    const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T correlation::criterium1<T,U>::getResult() const {
    return (static_cast<T>(static_cast<U>(1)/(state_+static_cast<U>(1))));
  }

  /*
   * set norm
   */
  template<class T,class U>
  void correlation::criterium1<T,U>::setNorm(const T& n) {
    norm_ = n;
  }

  /* --------------------------------------------------------
   * MATCHING CRITERIUM 2
   * --------------------------------------------------------
   */

  /*
   * criterium2 (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void correlation::criterium2<T,U>::accumulate(const T& filter,
                                                const T& src) {
    if (filter>src) {
      this->state_ += static_cast<U>(filter-src);
    } else {
      this->state_ += static_cast<U>(src-filter);
    }

  }

  template<class T,class U>
  void correlation::criterium2<T,U>::accumulateZero(const T& src) {
    accumulate(T(0),src);
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::criterium2<T,U>::accumulateSym(const T& filter,
                                                   const T& srcL,
                                                   const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::criterium2<T,U>::accumulateASym(const T& filter,
                                                    const T& srcL,
                                                    const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

  /* --------------------------------------------------------
   * MATCHING CRITERIUM 3
   * --------------------------------------------------------
   */

  /*
   * criterium3 (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void correlation::criterium3<T,U>::accumulate(const T& filter,
                                                const T& src) {
    U tmp = static_cast<U>(filter-src);
    this->state_ += tmp*tmp;
  }

  template<class T,class U>
  void correlation::criterium3<T,U>::accumulateZero(const T& src) {
    this->state_ += static_cast<U>(src*src);
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::criterium3<T,U>::accumulateSym(const T& filter,
                                                   const T& srcL,
                                                   const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }

  /*
   * Accumulate the values of filter and src
   * (srcL and srcR for symetric or asymetric kernel)
   */
  template<class T,class U>
  void correlation::criterium3<T,U>::accumulateASym(const T& filter,
                                                    const T& srcL,
                                                    const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

}
