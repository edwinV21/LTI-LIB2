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
 * \file   ltiEuclidianDistanceFunctor_template.h
 *         This file contains the class euclidianDistanceFunctor, 
 *         a class for calculating the L2 distance between two vectors or
 *         matrices.
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiEuclidianDistanceFunctor_template.h,v 1.4 2007-04-05 22:13:26 alvarado Exp $
 */

#include "ltiEuclidianDistanceFunctor.h"

namespace lti {

  template <typename T>
  euclidianDistanceFunctor<T>::euclidianDistanceFunctor() 
    : distanceFunctor<T>() {
  }

  template <typename T>
  euclidianDistanceFunctor<T>::euclidianDistanceFunctor(const euclidianDistanceFunctor<T>& other) : distanceFunctor<T>() {
    copy(other);
  }

  template <typename T>
  euclidianDistanceFunctor<T>::~euclidianDistanceFunctor() {
  }

  template <typename T>
  euclidianDistanceFunctor<T>& 
  euclidianDistanceFunctor<T>::copy(const euclidianDistanceFunctor<T>& other) {
    distanceFunctor<T>::copy(other);
    return (*this);
  }

  // class name
  template<class T>
  const std::string& euclidianDistanceFunctor<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  template <typename T>
  euclidianDistanceFunctor<T>* euclidianDistanceFunctor<T>::clone() const {
    return new euclidianDistanceFunctor(*this);
  }

  template <typename T>
  euclidianDistanceFunctor<T>* 
  euclidianDistanceFunctor<T>::newInstance() const {
    return new euclidianDistanceFunctor();
  }

  // return parameters
  template <typename T>
  const typename euclidianDistanceFunctor<T>::parameters&
  euclidianDistanceFunctor<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  template <typename T>
  bool euclidianDistanceFunctor<T>::apply(const vector<T>& a,
                                          const vector<T>& b, T& dist) const {
    assert (a.size() == b.size());

    typename vector<T>::const_iterator ait,bit;

    ait = a.begin();
    bit = b.begin();
    const typename vector<T>::const_iterator eit = a.end();

    dist=T(0);
    while (ait != eit) {
      const T tmp = (*ait)-(*bit);
      dist += tmp*tmp;
      ++ait;
      ++bit;
    }
    dist=sqrt(dist);

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool euclidianDistanceFunctor<T>::apply(const matrix<T>& a,
                                          const matrix<T>& b,
                                          T& dist) const {

    assert (a.size() == b.size());

    dist=T(0);
    typename vector<T>::const_iterator ait, bit, eit;

    for (int i=0;i<a.rows();++i) {
      ait=a.getRow(i).begin();
      eit=a.getRow(i).end();
      bit=b.getRow(i).begin();
      for (; ait!=eit; ++ait, ++bit) {
        const T tmp=(*ait)-(*bit);
        dist+=tmp*tmp;
      }
    }
    dist=sqrt(dist);

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool euclidianDistanceFunctor<T>::apply(const matrix<T>& a,
                                          const vector<T>& b,
                                          vector<T>& dest) const {
    const parameters& param=this->getParameters();
    assert((param.rowWise && (a.columns()==b.size())) ||
	   (!param.rowWise && (a.rows()==b.size())));
    

    int i=0;
    const int r=a.rows();
    const int c=a.columns();

    if (param.rowWise) {
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
          const T tmp=*ait-*bit;
          *dit += tmp*tmp;
        }
      }
      
      dest.apply(sqrt);
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool euclidianDistanceFunctor<T>::apply(const matrix<T>& a,
                                          const matrix<T>& b,
                                          vector<T>& dest) const {

    assert(a.size() == b.size());

    const parameters& param=this->getParameters();
    const int r=a.rows();
    int i=0;
    
    if (param.rowWise) {
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
          const T tmp=*ait-*bit;
          *dit+=tmp*tmp;
        }
      }
      dest.apply(sqrt);
    }
    return true;
  }

}
