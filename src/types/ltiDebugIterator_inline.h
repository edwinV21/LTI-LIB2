/*
 * Copyright (C) 2006
 * Peter Doerfler
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
 * \file   ltiDebugIterator.h
 *         Contains a generic implementation of debug iterators (used in debug
 *         builds) for lti::containers
 * \author Peter Doerfler
 * \date   13.08.2006
 *
 * $Id: ltiDebugIterator_inline.h,v 1.1 2006-08-24 14:56:56 doerfler Exp $
 */

#include "ltiAssert.h"

namespace lti {
  
  namespace internal {

    // default constructor
    template <template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst>::debugIterator() 
      : ptr_(0), begin_(0), end_(0) {
    }
  
    // copy constructor
    template <template <class> class ContainerBase , typename T, bool isConst>
    inline 
    debugIterator<ContainerBase<T>, isConst>
    ::debugIterator(const debugIterator& other)
      : ptr_(other.ptr_), begin_(other.begin_), end_(other.end_) {
    }
  
    // protected constructor
    template <template <class> class ContainerBase , typename T, bool isConst>
    inline 
    debugIterator<ContainerBase<T>, isConst>::debugIterator(pointer ptr,
                                                            pointer begin,
                                                            pointer end)
      : ptr_(ptr), begin_(begin), end_(end) {
    }

    // constructor for const_iterator(const iterator&)
    //
    // the enable_if ensures that trying the opposite fails at the class level
    // (i.e. there is no matching constructor) instead of when assigning T* =
    // const T* inside this constructor. Hopefully, the error message is more
    // useful like this.
    //
    // And what does it do? Well, basically it's only there for the case shown
    // in the first line of this comment, because if isConst&&!b is false the
    // enable_if<>::type does not exist.
    //
    // This trick got some help from the gcc4.2 STL implementation, thanks.
    template <template <class> class ContainerBase , typename T, bool isConst>
    template <bool b>
    inline 
    debugIterator<ContainerBase<T>, isConst>
    ::debugIterator(const debugIterator<typename enable_if<ContainerBase<T>, 
                    isConst&&!b>::type, b>& other) {
      ptr_   = other.ptr_;
      begin_ = other.begin_;
      end_   = other.end_;
    }

    // prefix operator++
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst>& 
    debugIterator<ContainerBase<T>, isConst>::operator++() {
      ++ptr_;
      return *this;
    }

    // postfix operator++
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst> 
    debugIterator<ContainerBase<T>, isConst>::operator++(int) {
      debugIterator tmp(*this);
      ++ptr_;
      return tmp;
    }

    // prefix operator--
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst>& 
    debugIterator<ContainerBase<T>, isConst>::operator--() {
      --ptr_;
      return *this;
    }

    // postfix operator--
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst> 
    debugIterator<ContainerBase<T>, isConst>::operator--(int) {
      debugIterator tmp(*this);
      --ptr_;
      return tmp;
    }

    // operator+=
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst>& 
    debugIterator<ContainerBase<T>, isConst>
    ::operator+=(const difference_type& n) {
      ptr_+=n;
      return *this;
    }

    // operator-=
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst>& 
    debugIterator<ContainerBase<T>, isConst>
    ::operator-=(const difference_type& n) {
      ptr_-=n;
      return *this;
    }

    // operator+
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst> 
    debugIterator<ContainerBase<T>, isConst>
    ::operator+(const difference_type& n) {
      return debugIterator(ptr_+n,begin_,end_);
    }

    // operator-
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline debugIterator<ContainerBase<T>, isConst> 
    debugIterator<ContainerBase<T>, isConst>
    ::operator-(const difference_type& n) {
      return debugIterator(ptr_-n,begin_,end_);
    }

    // operator*
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline typename debugIterator<ContainerBase<T>, isConst>::reference
    debugIterator<ContainerBase<T>, isConst>::operator*() const {
      assert ((ptr_ >= begin_) && (ptr_ < end_));
      return *ptr_;
    }
  
    // operator->
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline typename debugIterator<ContainerBase<T>, isConst>::pointer
    debugIterator<ContainerBase<T>, isConst>
    ::operator->() const {
      assert ((ptr_ >= begin_) && (ptr_ < end_));
      return ptr_;
    }

    // operator[]
    template<template <class> class ContainerBase , typename T, bool isConst>
    inline typename debugIterator<ContainerBase<T>, isConst>::reference
    debugIterator<ContainerBase<T>, isConst>
    ::operator[](const difference_type& i) const {
      pointer tmp = ptr_+i;
      assert ((tmp >= begin_) && (tmp < end_));
      return *tmp;
    }

    //************************************************************
    // Comparison operators and operator-(debugIterator)
    //
    // These really should have been global but that poses a few
    //implementation problems. Other possibilities would have been:
    // - three (!) friend declarations per operator: this just looks to ugly
    //   to be true
    // - a helper struct with static member functions for all operators: a
    //   nice solution as only one friend declaration is needed. However,
    //   inlining these MF only works at higher optimization levels, and thus
    //   code would be slow.
    // - a const T* base() member or global function: The former would be
    //   incomplatible to the release mode T* alias. The later would be fine if
    //   never used but it's just to tempting and there's no way to hide that
    //   global function (I know of)
    //
    // So template member function operators which can either take the same or
    // a debugIterator with opposite constness are chosen as the most simple
    // way of implementing the requirement of DR179 that the following
    // operators can be used with any combination of iterator and
    // const_iterator
    // ************************************************************

    // operator==
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline bool
    debugIterator<ContainerBase<T>, isConst>
    ::operator==(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ == other.ptr_;
    }

    // operator!=
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline bool
    debugIterator<ContainerBase<T>, isConst>
    ::operator!=(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ != other.ptr_;
    }

    // operator<
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline bool
    debugIterator<ContainerBase<T>, isConst>
    ::operator<(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ < other.ptr_;
    }

    // operator>
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline bool
    debugIterator<ContainerBase<T>, isConst>
    ::operator>(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ > other.ptr_;
    }

    // operator<=
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline bool
    debugIterator<ContainerBase<T>, isConst>
    ::operator<=(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ <= other.ptr_;
    }

    // operator>=
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline bool
    debugIterator<ContainerBase<T>, isConst>
    ::operator>=(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ >= other.ptr_;
    }

    // operator- (this is used in std::distance)
    template<template <class> class ContainerBase, typename T, bool isConst>
    template <bool co>
    inline typename debugIterator<ContainerBase<T>, isConst>::difference_type
    debugIterator<ContainerBase<T>, isConst>
    ::operator-(const debugIterator<ContainerBase<T>, co>& other) const {
      return ptr_ - other.ptr_;
    }

  } // namespace internal

} // namespace lti
