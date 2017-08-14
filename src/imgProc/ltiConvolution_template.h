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
 * \file   ltiConvolution_template.h
 *         Contains the class convolution.
 * \author Pablo Alvarado
 * \date   17.11.99
 *
 * revisions ..: $Id: ltiConvolution_template.h,v 1.4 2006-09-15 15:02:55 doerfler Exp $
 */

namespace lti {

  /*
   * default constructor
   */
  template<class T,class U>
  convolution::accumulator<T,U>::accumulator() {
    reset();
  }
  
  /*
   * reset
   */
  template<class T,class U>
  inline void convolution::accumulator<T,U>::reset() {
    state_ = static_cast<U>(0);
  }
  
  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  inline void convolution::accumulator<T,U>::accumulate(const T& filter,
							const T& src) {
    state_ += static_cast<U>(src*filter);
  }
  
  template<class T,class U>
  inline void convolution::accumulator<T,U>::accumulateZero(const T&) {
  }
  
  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  inline void convolution::accumulator<T,U>::accumulateSym(const T& filter,
							   const T& srcL,
							   const T& srcR) {
    state_ += static_cast<U>((srcL+srcR)*filter);
  }
  
  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  inline void convolution::accumulator<T,U>::accumulateASym(const T& filter,
							    const T& srcL,
							    const T& srcR) {
    state_ += static_cast<U>((srcL-srcR)*filter);
  }
  
  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  inline T convolution::accumulator<T,U>::getResult() const {
    return (static_cast<T>(state_));
  }
  
  /*
   * set norm
   */
  template<class T,class U>
  inline void convolution::accumulator<T,U>::setNorm(const T& n) {
    norm_ = n;
  }

  // --------------------------------------------------
  // specializations!
  // --------------------------------------------------

  template<>
  inline void
  convolution::accumulator<ubyte,int>::accumulate(const ubyte& filter,
						       const ubyte& src) {
    state_ += static_cast<int>(src)*static_cast<int>(filter);
  }

  /*
   * specialization for integers
   */
  template<>
  inline int convolution::accumulator<int,int>::getResult() const {
    return state_/norm_;
  }

  template<>
  inline ubyte convolution::accumulator<ubyte,int>::getResult() const {
    return static_cast<ubyte>(state_/norm_);
  }

  
}



