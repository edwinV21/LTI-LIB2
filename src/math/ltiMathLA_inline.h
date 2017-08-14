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
 * \file   ltiMathLA_inline.h
 *         Implementation of some useful global functions which are
 *         often needed in Linear Algebra Algorithms.
 * \author Peter Doerfler
 * \date   27.04.2004
 *
 * $Id: ltiMathLA_inline.h,v 1.2 2006-09-22 11:28:38 doerfler Exp $
 */


#include "ltiMath.h"

namespace lti {

  template<typename T>
  inline void householder(vector<T>& v,T& beta) {

    const T x0=v.at(0);
    typename vector<T>::iterator itStart=v.begin();
    //always need to iterator from second element on.
    ++itStart;
    const typename vector<T>::iterator itEnd=v.end();
    typename vector<T>::iterator it=itStart;

    T sigma=T(0);
    for (; it!=itEnd; ++it) {
      sigma+=(*it)*(*it);
    }
    
    //no need to do anything
    if (closeToZero(sigma)) {
      v.at(0)=T(1);
      beta=T(0);
      return;
    }

    const T mu=sqrt(x0*x0+sigma);
    T v0;
    if (x0<=T(0)) {
      v0 = x0 - mu;
    } else {
      v0 = -sigma/(x0+mu);
    }

    const T v0sqr=v0*v0;
    beta = 2*v0sqr/(sigma+v0sqr);
    v.at(0)=T(1);
    for (it=itStart; it!=itEnd; ++it) {
      (*it)/=v0;
    }

  }

  template<typename T>
  inline void householder(const vector<T>& src, vector<T>& v,T& beta) {
    
    v.copy(src);
    householder(v,beta);
  }

  template<typename T>
  inline void givens(const T& a, const T& b, T& c, T& s) {

    //depending on the definition there s and c values should have the
    //opposite signs depending on the sign of a or b.
    //here we follow Golub.

    if (b==T(0)) {
      c=T(1); s=T(0);
    } else if (abs(b)>abs(a)) {
      const T tmp = -a/b;
      s = T(1)/sqrt(1+tmp*tmp);
      c = s*tmp;
    } else {
      const T tmp = -b/a;
      c = T(1)/sqrt(1+tmp*tmp);
      s = c*tmp;
    }
  }

}
