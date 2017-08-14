/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2009
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiErosion_template.h
 *         Contains the class lti::erosion,
 *         which implements the morphological operator known as erosion.
 * \author Pablo Alvarado
 * \date   19.07.2000
 *
 * revisions ..: $Id: ltiErosion_template.h,v 1.2 2009-10-14 03:42:19 alvarado Exp $
 */

#include <limits>

namespace lti {


  //============================================================
  // Gray Non-Flat
  //============================================================

  /*
   * default constructor
   */
  template<class T,class U>
  erosion::accumulatorGrayNonFlat<T,U>::accumulatorGrayNonFlat() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
  void erosion::accumulatorGrayNonFlat<T,U>::reset() {
    state_ = std::numeric_limits<U>::max();
  }

  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void erosion::accumulatorGrayNonFlat<T,U>::accumulate(const T& filter,
                                                             const T& src) {
    if (filter != 0) {
      state_ = min(state_,static_cast<U>(src)-static_cast<U>(filter));
    }
  }
  template<class T,class U>
  void erosion::accumulatorGrayNonFlat<T,U>::accumulateZero(const T&) {
    // do nothing, filter value is zero by definition
  }

  template<class T,class U>
  void erosion::accumulatorGrayNonFlat<T,U>::accumulateSym(const T& filter,
                                                         const T& srcL,
                                                         const T& srcR) {
    if (filter != 0) {
      const U f = static_cast<U>(filter);
      state_ = min(state_,static_cast<U>(srcL)-f,static_cast<U>(srcR)-f);
    }
  }

  template<class T,class U>
  void erosion::accumulatorGrayNonFlat<T,U>::accumulateASym(const T& flter,
                                                          const T& srcL,
                                                          const T& srcR) {
    if (flter != 0) {
      const U f = static_cast<U>(flter);
      state_ = min(state_,static_cast<U>(srcL)-f,static_cast<U>(srcR)-f);
    }
  }

  template<class T,class U>
  inline T erosion::accumulatorGrayNonFlat<T,U>::getResult() const {
    return (static_cast<T>(state_));
  }

  template<class T,class U>
  void erosion::accumulatorGrayNonFlat<T,U>::setNorm(const T& n) {
    norm_ = n;
  }


  // specialization for <ubyte, int>
  template<>
  inline ubyte erosion::accumulatorGrayNonFlat<ubyte, int>::getResult() const {
    return (state_ < 0 ? ubyte(0) : static_cast<ubyte>(state_));
  }

  //============================================================
  // Gray Flat
  //============================================================

  /*
   * default constructor
   */
  template<class T,class U>
  erosion::accumulatorGrayFlat<T,U>::accumulatorGrayFlat() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
  void erosion::accumulatorGrayFlat<T,U>::reset() {
    state_ = std::numeric_limits<U>::max();
  }

  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void erosion::accumulatorGrayFlat<T,U>::accumulate(const T& filter,
                                                          const T& src) {
    if (filter != 0) {
      state_ = min(state_,static_cast<U>(src));
    }
  }
  template<class T,class U>
  void erosion::accumulatorGrayFlat<T,U>::accumulateZero(const T&) {
    // do nothing, filter value is zero by definition
  }

  template<class T,class U>
  void erosion::accumulatorGrayFlat<T,U>::accumulateSym(const T& filter,
                                                         const T& srcL,
                                                         const T& srcR) {
    if (filter != 0) {
      state_ = min(state_,static_cast<U>(srcL),static_cast<U>(srcR));
    }
  }

  template<class T,class U>
  void erosion::accumulatorGrayFlat<T,U>::accumulateASym(const T& flter,
                                                          const T& srcL,
                                                          const T& srcR) {
    if (flter != 0) {
      state_ = min(state_,static_cast<U>(srcL),static_cast<U>(srcR));
    }
  }

  template<class T,class U>
  inline T erosion::accumulatorGrayFlat<T,U>::getResult() const {
    return (static_cast<T>(state_));
  }

  template<class T,class U>
  void erosion::accumulatorGrayFlat<T,U>::setNorm(const T& n) {
    norm_ = n;
  }


  // specialization for <ubyte, int>
  template<>
  inline ubyte erosion::accumulatorGrayFlat<ubyte, int>::getResult() const {
    return (state_ < 0 ? ubyte(0) : static_cast<ubyte>(state_));
  }


  //============================================================
  // Binary
  //============================================================

  template<class T,class U>
  erosion::accumulatorBin<T,U>::accumulatorBin() {
    reset();
  }

  template<class T,class U>
  void erosion::accumulatorBin<T,U>::reset() {
    state_ = static_cast<U>(norm_);
  }

  template<class T,class U>
  void erosion::accumulatorBin<T,U>::accumulate(const T& filter,
                                                     const T& src) {
    if ((state_ != 0) && (filter != 0) && (src == 0)) {
      state_ = U(0);
    }
  }

  template<class T,class U>
  void erosion::accumulatorBin<T,U>::accumulateZero(const T&) {
    // do nothing, filter value is zero by definition
  }

  template<class T,class U>
  void erosion::accumulatorBin<T,U>::accumulateSym(const T& filter,
                                                        const T& srcL,
                                                        const T& srcR) {
    if ((state_ != 0) && (filter != 0) && ((srcL == 0) || (srcR == 0))) {
      state_ = U(0);
    }
  }

  template<class T,class U>
  void erosion::accumulatorBin<T,U>::accumulateASym(const T& filter,
                                                         const T& srcL,
                                                         const T& srcR) {
    if ((state_ != 0) && (filter != 0) && ((srcL == 0) || (srcR == 0))) {
      state_ = U(0);
    }
  }

  template<class T,class U>
  inline T erosion::accumulatorBin<T,U>::getResult() const {
    return static_cast<T>(state_);
  }

  template<class T,class U>
  void erosion::accumulatorBin<T,U>::setNorm(const T& n) {
    norm_ = n;
  }

}
