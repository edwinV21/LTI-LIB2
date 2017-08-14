/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiGenericVector_inline.h
 * \author Pablo Alvarado
 * \date   13.06.2000
 *
 * $Id: ltiGenericVector_inline.h,v 1.7 2007-10-03 21:13:54 alvarado Exp $
 */

#ifndef _LTI_GENERIC_VECTOR_INLINE_H_
#define _LTI_GENERIC_VECTOR_INLINE_H_

namespace lti {


  // -------------------------------------------------------------------
  // genericVector<T>
  // -------------------------------------------------------------------

  template<typename T>
  inline bool genericVector<T>::ownsData() const {
    return ownData_;
  }

  // returns the number of elements of the genericVector
  template<typename T>
  inline typename genericVector<T>::size_type genericVector<T>::size() const {
    return vectorSize_;
  }
  
  // returns first index (normally 0)
  template<typename T>
  inline int genericVector<T>::firstIdx() const {
    return 0;
  }
  
  /* returns first element as a const_iterator.
     Note that you can not change the values of the genericVector
     elements when you use a const_iterator. See also begin() */
  template<typename T>
  inline typename genericVector<T>::const_iterator 
  genericVector<T>::begin() const {
#   ifdef NDEBUG
    return const_iterator(theElements_);
#   else
    return const_iterator(theElements_,
                          theElements_, 
                          &theElements_[vectorSize_]);
#   endif
  }

  /* returns first element as an iterator */
  template<typename T>
  inline typename genericVector<T>::iterator genericVector<T>::begin() {
#   ifdef NDEBUG
    return iterator(theElements_);
#   else
    return iterator(theElements_, theElements_, &theElements_[vectorSize_]);
#   endif
  }

  // returns last index (in a genericVector this is always size()-1)
  template<typename T>
  inline int genericVector<T>::lastIdx() const {
    return idxLastElement_;
  }

  /* returns last index as a const iterator.
     For an example see begin() */
  template<typename T>
  inline typename genericVector<T>::const_iterator 
  genericVector<T>::end() const {
#   ifdef NDEBUG
    return const_iterator(&theElements_[vectorSize_]);
#   else
    return const_iterator(&theElements_[vectorSize_], 
                          theElements_, 
                          &theElements_[vectorSize_]);
#   endif
  }

  /* returns last index as an iterator
     For an example see begin() */
  template<typename T>
  inline typename genericVector<T>::iterator genericVector<T>::end() {
#   ifdef NDEBUG
    return iterator(&theElements_[vectorSize_]);
#   else
    return iterator(&theElements_[vectorSize_], 
                    theElements_, 
                    &theElements_[vectorSize_]);
#   endif
  }


  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericVector. It is used for inverse order
   * iteration through the genericVector using normal iterators (as opposed
   * to reverse iterators). This has the advantage that iterators
   * going from front to end and in the inverse direction are the
   * same and can thus be compared, copied etc.
   */
  template<typename T>
  inline typename genericVector<T>::iterator genericVector<T>::inverseBegin() {
#   ifdef NDEBUG
    return iterator(&theElements_[idxLastElement_]);
#   else
    return iterator(&theElements_[idxLastElement_], 
                    theElements_, 
                    &theElements_[vectorSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericVector. See inverseBegin() for more details.
   */
  template<typename T>
  inline typename genericVector<T>::const_iterator
  genericVector<T>::inverseBegin() const {
#   ifdef NDEBUG
    return const_iterator(&theElements_[idxLastElement_]);
#   else
    return const_iterator(&theElements_[idxLastElement_], 
                          theElements_, 
                          &theElements_[vectorSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericVector. It is used to
   * mark the end for inverse order iteration through the genericVector
   * using normal iterators (as opposed to reverse iterators). This
   * has the advantage that iterators going from front to end and in
   * the inverse direction are the same and can thus be compared,
   * copied etc.
   */
  template<typename T>
  inline typename genericVector<T>::iterator genericVector<T>::inverseEnd() {
#   ifdef NDEBUG
    return iterator(&theElements_[-1]);
#   else
    return iterator(&theElements_[-1], 
                    theElements_, 
                    &theElements_[vectorSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericVector.
   */
  template<typename T>
  inline typename genericVector<T>::const_iterator 
  genericVector<T>::inverseEnd() const {
#   ifdef NDEBUG
    return const_iterator(&theElements_[-1]);
#   else
    return const_iterator(&theElements_[-1], 
                          theElements_, 
                          &theElements_[vectorSize_]);
#   endif
  }

  template<typename T>
  inline void genericVector<T>::resize(const int newSize) {
    resize(newSize,T(),Copy);
  }

  template<typename T>
  inline void genericVector<T>::allocate(const int newSize) {
    resize(newSize,T(),AllocateOnly);
  }

  template<typename T>
  inline void genericVector<T>::assign(const int newSize,
                                       const T& initValue) {
    resize(newSize,initValue,Init);
  }

  template<typename T>
  inline bool genericVector<T>::empty() const {
    return (vectorSize_ == 0);
  }

  template<typename T>
  inline T* genericVector<T>::data() {
    return theElements_;
  }

  template<typename T>
  inline const T* genericVector<T>::data() const {
    return theElements_;
  }

  template<typename T>
  inline T& genericVector<T>::at(const int x) {
    assert(static_cast<unsigned int>(x)<
           static_cast<unsigned int>(vectorSize_));
    assert(theElements_ != 0);
    return ( theElements_[x] );
  }

  template<typename T>
  inline const T& genericVector<T>::at(const int x) const {
    assert(static_cast<unsigned int>(x)<
           static_cast<unsigned int>(vectorSize_));
    assert(theElements_ != 0);
    return ( theElements_[x] );
  }

  // access operator (alias for at(const int x)).
  template<typename T>
  inline T& genericVector<T>::operator[](const int x) {
    return at(x);
  }

  // const access operator (alias for at(const int x) const).
  template<typename T>
  inline const T& genericVector<T>::operator[](const int x) const {
    return at(x);
  }

  // access member function (alias for at(const int x)).
  template<typename T>
  inline T& genericVector<T>::elem(int x) {
    return at(x);
  }

  // const access operator (alias for at(const int x) const).
  template<typename T>
  inline const T& genericVector<T>::elem(int x) const {
    return at(x);
  }

  template<typename T>
  inline genericVector<T>&
  genericVector<T>::operator=(const genericVector<T>& other) {
    return copy(other);
  }

  /* compare this genericVector with other
     @param other the other genericVector to be compared with
     @return true if both genericVectors have the same elements and same size
  */
  template<typename T>
  inline bool 
  genericVector<T>::operator==(const genericVector<T>& other) const {
    return equals(other);
  }

  /* compare this genericVector with other
     @param other the other genericVector to be compared with
     @return true if both genericVectors are different
  */
  template<typename T>
  inline bool 
  genericVector<T>::operator!=(const genericVector<T>& other) const {
    return !equals(other);
  }
} // namespace lti

#endif

