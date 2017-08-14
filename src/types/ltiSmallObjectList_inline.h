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

/*
 * \file   ltiSmallObjectList_inline.h
 *         Defines a list similar to the std::list but specialized for
 *         an efficient access of small objects.
 * \author Gustavo Quiros
 * \date   12.12.03
 *
 * $Id: ltiSmallObjectList_inline.h,v 1.4 2007-12-25 19:58:24 alvarado Exp $
 */

namespace lti {
  
  /*
   * Returns true if the list has no elements, false otherwise.
   */
  template<typename T>
  inline bool smallObjectList<T>::empty() const { 
    return end_.next == &end_;
  }
  
  /*
   * Empties the list.
   */
  template<typename T>
  inline void smallObjectList<T>::clear() { 
    // TODO do this more efficiently
    while(!empty()) {
      pop_back();
    } 
  }
  
  /*
   * Returns an iterator pointing to the first element of the list.
   * The use of the interators is similar to the iterators of the
   * Standard Template Library (STL).
   */
  template<typename T>
  inline typename smallObjectList<T>::iterator smallObjectList<T>::begin() { 
    return iterator(end_.next); 
  }

  /*
   * Returns an iterator pointing after the last element of the list.
   * The use of the interators is similar to the iterators of the
   * Standard Template Library (STL).
   */
  template<typename T>
  inline typename smallObjectList<T>::iterator smallObjectList<T>::end() { 
    return iterator(&end_); 
  }
  
  /*
   * Returns a const_iterator pointing to the first element of the list.
   * The use of the interators is similar to the iterators of the
   * Standard Template Library (STL).
   */
  template<typename T>
  inline typename smallObjectList<T>::const_iterator 
  smallObjectList<T>::begin() const { 
    return const_iterator(end_.next); 
  }
  
  /*
   * Returns a const_iterator pointing after the last element of the list.
   * The use of the interators is similar to the iterators of the
   * Standard Template Library (STL).
   */
  template<typename T>
  inline typename smallObjectList<T>::const_iterator 
  smallObjectList<T>::end() const { 
    return const_iterator(&end_); 
  }

  /*
   * Returns a reference to the first element of the list.
   */
  template<typename T>
  inline typename smallObjectList<T>::reference smallObjectList<T>::front() { 
    return *begin(); 
  }
  
  /*
   * Returns a const_reference to the first element of the list.
   */
  template<typename T>
  inline typename smallObjectList<T>::const_reference 
  smallObjectList<T>::front() const { 
    return *begin(); 
  }
  
  /*
   * Returns a reference to the last element of the list.
   */
  template<typename T>
  inline typename smallObjectList<T>::reference smallObjectList<T>::back() { 
    return *(--end()); 
  }
  
  /*
   * Returns a const_reference to the last element of the list.
   */
  template<typename T>
  inline typename smallObjectList<T>::const_reference 
  smallObjectList<T>::back() const { 
    return *(--end()); 
  }
  
  /**
   * Determines if the given node is contained in this segment, that is,
   * if the pointer points within the inner node array.
   */
  template<typename T>
  inline bool smallObjectList<T>::segment::contains(node *node) {
    return
      (node >= &nodes_[0]) &&
      (node < &nodes_[_LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE]);
  }
  
  /**
   * 'Grabs' (allocates) a node in this segment.
   */
  template<typename T>
  inline typename smallObjectList<T>::node* 
  smallObjectList<T>::segment::grab(){
    ++nodeCount;
    node *n = firstFree;
    firstFree = firstFree->next;
    return n;
  }
  
  /**
   * Releases the given node in this segment
   */
  template<typename T>
  inline void smallObjectList<T>::segment::release(node *n){
    --nodeCount;
    n->next = firstFree;
    firstFree = n;
  }
  
}
