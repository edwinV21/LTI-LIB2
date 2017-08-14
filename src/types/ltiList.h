/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiList.h
 *         Defines a wrapper that automatically decides if it is more 
 *         efficient to use the lti::smallObjectList or the std::list
 * \author Gustavo Quiros
 * \date   02.04.2004
 *
 * $Id: ltiList.h,v 1.4 2005-05-24 14:51:56 doerfler Exp $
 */

#ifndef _LTI_LIST_H_
#define _LTI_LIST_H_

#include "ltiObject.h"
#include "ltiIoObject.h"
#include "ltiSmallObjectList.h"
#include <list>
#include "ltiPerformanceConfig.h"

namespace lti {

  template <typename T, bool, class Alloc>
  class listHelper : public std::list<T,Alloc> {};

  template <typename T>
  class listHelper<T,true,std::allocator<T> > : public smallObjectList<T> {};


  /**
   * A linked list, implemented either as a lti::smallObjectList for
   * objects whose size is less than or equal to
   * _LTI_PERFORMANCE_LIST_OBJECT_SIZE_THRESHOLD, or as a std::list
   * for bigger objects.
   *
   * In case a different allocator \a Alloc than std::allocator is
   * chosen the std::list is used, no matter how big \a T is. This is
   * necessary since lti::smallObjectList doesn't know any allocators.
   *
   * It is highly recommended that you use lti::list in all places
   * that require a list. Test show superior performance in almost any
   * case. If you don't follow this recommendation use
   * lti::smallObjectList or std::list directly and clearly document
   * why you have done so
   *
   * Note that this requires a C++ compiler that supports partial
   * template specialization.
   */
  template <typename T, class Alloc=std::allocator<T> >
  class list 
    : public listHelper<T,(sizeof(T) <= _LTI_PERFORMANCE_LIST_OBJECT_SIZE_THRESHOLD),Alloc>{
  };

}

#endif
