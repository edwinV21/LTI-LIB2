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
 * \file   ltiRound_inline.h
 *         Definition of some usually used global functions for
 *         mathematical rounding (round, iround, lround and cround_cast)
 * \author Pablo Alvarado
 * \date   28.04.1999
 *
 * $Id: ltiRound_inline.h,v 1.3 2004-07-27 08:22:42 alvarado Exp $
 */

namespace lti {

  template<typename T>
  inline T round(T x) {
    if( x >= static_cast<T>(0) ) {
      return static_cast<T>(static_cast<long>(x + 0.5));
    }
    return static_cast<T>(static_cast<long>(x - 0.5));
  }

  template<typename T>
  inline int iround(T x) {
    if( x >= 0 ) {
      return static_cast<int>(x + 0.5);
    }

    return static_cast<int>(x - 0.5);
  }

  template<typename T>
  inline long lround(T x) {
    if( x >= 0 ) {
      return static_cast<long>(x + 0.5);
    }
    return static_cast<long>(x - 0.5);
  }

  // helper structure to create at compile time efficiently the
  // cround_cast operator.
  // The trailing underscore denotes "private".
  // The booleans indicate if the types are floating point types or not.
  template<typename T,typename F,bool B1,bool B2> 
  struct cround_cast_ {
    static inline T cast(const F from) {
      return static_cast<T>(from);
    }
  };

  template<typename T,typename F>
  struct cround_cast_<T,F,false,true> {
    static inline T cast(const F x) {
      if( x >= 0 ) {
        return static_cast<T>(x + static_cast<F>(0.5f));
      }
      return static_cast<T>(x - static_cast<F>(0.5f));
    }
  };

  // small "private" structure to avoid including <limits> just to check if
  // a type is a floating point type or not
  // The trailing underscore denotes "private"
  template <typename T> 
  struct fp_info_ {
    static const bool isFP = false;
  };

  template<>
  struct fp_info_<float> {
    static const bool isFP = true;
  };

  template<>
  struct fp_info_<double> {
    static const bool isFP = true;
  };

  template<typename T,typename F>
  inline T cround_cast(const F from) {
    return cround_cast_<T,F,fp_info_<T>::isFP,fp_info_<F>::isFP>::cast(from);
  }

}
