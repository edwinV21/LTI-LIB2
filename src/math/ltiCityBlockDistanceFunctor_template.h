/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiCityBlockDistanceFunctor_template.h
 *         This file contains the class cityBlockDistanceFunctor, 
 *         a class for calculating the L1 distance between two vectors or
 *         matrices.
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiCityBlockDistanceFunctor_template.h,v 1.4 2007-04-05 22:13:26 alvarado Exp $
 */

#include "ltiCityBlockDistanceFunctor.h"

namespace lti {

  template <typename T>
  cityBlockDistanceFunctor<T>::cityBlockDistanceFunctor() 
    : distanceFunctor<T>() {
  }

  template <typename T>
  cityBlockDistanceFunctor<T>::cityBlockDistanceFunctor(const cityBlockDistanceFunctor<T>& other) : distanceFunctor<T>() {
    copy(other);
  }

  template <typename T>
  cityBlockDistanceFunctor<T>::~cityBlockDistanceFunctor() {
  }

  template <typename T>
  cityBlockDistanceFunctor<T>& 
  cityBlockDistanceFunctor<T>::copy(const cityBlockDistanceFunctor<T>& other) {
    distanceFunctor<T>::copy(other);
    return (*this);
  }

  // class name
  template<class T>
  const std::string& cityBlockDistanceFunctor<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  template <typename T>
  cityBlockDistanceFunctor<T>* cityBlockDistanceFunctor<T>::clone() const {
    return new cityBlockDistanceFunctor(*this);
  }

  template <typename T>
  cityBlockDistanceFunctor<T>* 
  cityBlockDistanceFunctor<T>::newInstance() const {
    return new cityBlockDistanceFunctor();
  }

  // return parameters
  template <typename T>
  const typename cityBlockDistanceFunctor<T>::parameters&
  cityBlockDistanceFunctor<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  template <typename T>
  bool cityBlockDistanceFunctor<T>::apply(const vector<T>& a,
                                          const vector<T>& b, T& dist) const {
    assert(a.size() == b.size());

    typename vector<T>::const_iterator ait,bit;

    ait = a.begin();
    bit = b.begin();
    const typename vector<T>::const_iterator eit = a.end();

    dist=T(0);
    while (ait != eit) {
      dist += lti::abs((*ait)-(*bit));
      ++ait;
      ++bit;
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool cityBlockDistanceFunctor<T>::apply(const matrix<T>& a,
                                          const matrix<T>& b,
                                          T& dist) const {
    assert(a.size() == b.size());

    const int sz=a.rows()*a.columns();
    dist=T(0);

    for (int i=0; i < sz; i++) {
      dist += lti::abs(a.elem(i)-b.elem(i));
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool cityBlockDistanceFunctor<T>::apply(const matrix<T>& a,
                                          const vector<T>& b,
                                          vector<T>& dest) const {

    const bool rowWise=this->getParameters().rowWise;
    assert((rowWise && (a.columns()==b.size())) ||
	   (!rowWise && (a.rows()==b.size())));

    int i=0;
    const int r=a.rows();
    const int c=a.columns();

    if (rowWise) {
      dest.allocate(r);
      typename vector<T>::iterator it=dest.begin();

      for (i=0; i < r; i++, ++it) {
        apply(a.getRow(i), b, *it);
      }
    } else {
      dest.allocate(c);
      typename vector<T>::const_iterator bit=b.begin();
      typename vector<T>::const_iterator ait;
      typename vector<T>::const_iterator aite;
      typename vector<T>::iterator dit;

      for (i=0; i<r; i++, ++bit) {
        const vector<T>& aRow=a.getRow(i);
        ait=aRow.begin();
        aite=aRow.end();
        dit=dest.begin();
        for ( ; ait!=aite; ++ait, ++dit) {
          *dit += lti::abs(*ait-*bit);
        }
      }
    }
    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool cityBlockDistanceFunctor<T>::apply(const matrix<T>& a,
                                          const matrix<T>& b,
                                          vector<T>& dest) const {
    assert(a.size() == b.size());

    const bool rowWise=this->getParameters().rowWise;
    const int r=a.rows();
    int i=0;
    
    if (rowWise) {
      dest.allocate(r);
      
      for (i=0; i < r; i++) {
        apply(a.getRow(i), b.getRow(i), dest.at(i));
      }
    } else {
      dest.allocate(a.columns());
      
      typename vector<T>::const_iterator bit;
      typename vector<T>::const_iterator ait;
      typename vector<T>::const_iterator aite;
      typename vector<T>::iterator dit;
      
      for (i=0; i < r; i++) {
        bit=b.getRow(i).begin();
        const vector<T>& aRow=a.getRow(i);
        ait=aRow.begin();
        aite=aRow.end();
        dit=dest.begin();
        for (; ait!=aite; ++ait, ++bit, ++dit) {
          *dit+=lti::abs(*ait-*bit);
        }
      }
    }
    
    return true;
  }

}
