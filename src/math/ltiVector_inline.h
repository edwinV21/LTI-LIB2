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
 * \file   ltiVector_inline.h
 * \author Pablo Alvarado
 * \date   13.06.2000
 *
 * $Id: ltiVector_inline.h,v 1.5 2012-11-17 14:49:05 alvarado Exp $
 */

#ifndef _LTI_VECTOR_INLINE_H_
#define _LTI_VECTOR_INLINE_H_

namespace lti {

  /*
    Divide this vector with a constant.  This vector will changed!
    Returns this vector.
    synonym for divide(const T cst).
    @param cst the elements of the vector will be divided with this
    constant
    @return a reference to the actual vector
  */
  template<class T>
  inline vector<T>& vector<T>::edivide(const T cst) {
    return divide(cst);
  }

  /*
    Divide the other vector with a constant and leave the result here.
    Returns a reference to this vector. <p>
    synonym for divide(const vector<T>& other,const T cst).
    @param other the vector to be divide by the constant value
    @param cst the elements of the vector will be divided with this
    constant
    @return a reference to the actual vector
  */
  template<class T>
  inline vector<T>& vector<T>::edivide(const genericVector<T>& other,
                                       const T cst) {
    return divide(other, cst);
  }

  /**
   * Less-than operator for vector, needed in order to use vector as
   * key in std::map.
   */
  template <typename T>
  inline bool operator < (vector<T> a, vector<T> b) {
    for(typename vector<T>::iterator 
          ita = a.begin(),
          itb = b.begin(),
          enda = a.end(),
          endb = b.end();
        ita != enda && itb != endb;
        ++ita, ++itb){
      if(!(*ita < *itb)) return false;
    }
    return true;
  }

  // add another vector
  template<class T>
  template<class U>
  vector<T>& vector<T>::add(const genericVector<U>& other) {
    assert(this->size() == other.size());

    iterator i,e;
    typename genericVector<U>::const_iterator o;

    e = this->end();
    for (i=this->begin(),o=other.begin();i!=e;++i,++o)
      (*i)+=static_cast<T>(*o);

    return *this;
  }

  // add two vectors
  template<typename T>
  template<typename U,typename V>
  vector<T>& vector<T>::add(const genericVector<U>& first,
                            const genericVector<V>& second) {
    assert(first.size() == second.size());

    iterator i,e;
    typename genericVector<U>::const_iterator a;
    typename genericVector<V>::const_iterator b;

    this->allocate(first.size());

    for (i=this->begin(),e=this->end(),a=first.begin(),b=second.begin();
         i!=e;++i,++a,++b)
      (*i)=static_cast<T>((*a)+(*b));

    return *this;
  }

} // namespace lti

#endif

