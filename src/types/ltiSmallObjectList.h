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
 * \file   ltiSmallObjectList.h
 *         Defines a list similar to the std::list but specialized for
 *         an efficient access of small objects.
 * \author Gustavo Quiros
 * \date   12.12.03
 *
 * $Id: ltiSmallObjectList.h,v 1.16 2013-09-20 13:58:21 alvarado Exp $
 */

#ifndef _LTI_SMALL_OBJECT_LIST_H_
#define _LTI_SMALL_OBJECT_LIST_H_

#include "ltiObject.h"
#include "ltiIoObject.h"
#include "ltiTypes.h"
#include "ltiException.h"
#include "ltiAssert.h"
#include "ltiPerformanceConfig.h"

#include <cstddef>

namespace lti {
  
  /**
   * List of objects of small size.
   *
   * The %lti::smallObjectList is an efficient implementation
   * of a (double) linked list for small data types. 
   *
   * Each instance maintains its own heap, which may be expensive in memory
   * consumption, but they are intended as very fast data structures with
   * respect to memory reservation issues.  It should serve, in many cases, as
   * a drop-in replacement for std::list.
   *
   * @see lti::list
   */
  template <typename T>
  class smallObjectList {

  private:
    
    struct node;
    class heap;
    
  public:
    
    class iterator;
    class const_iterator;
    friend class iterator;
    friend class const_iterator;

    /**
     * The type used to store the size of this list.
     */
    typedef unsigned int size_type;

    /**
     * Reference type (allows read and write operations)
     * The use of the reference classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef T& reference;

    /**
     * Const_reference type (allows read-only operations)
     * The use of the reference classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef const T& const_reference;
    
    /**
     * Pointer type (allows read and write operations)
     * The use of the pointer classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef T* pointer;
    
    /**
     * Const_pointer type (allows read-only operations)
     * The use of the pointer classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef const T* const_pointer;

    /**
     * The type of the values stored in the list.
     */
    typedef T value_type;

    /**
     * Default constructor. Creates an empty smallObjectList.
     */
    smallObjectList();
    
    /**
     * Copy constructor. Creates a smallObjectList with the same
     * contents as the given list.
     */
    smallObjectList(const smallObjectList& l);
    
    /**
     * Destructor
     */
    ~smallObjectList();

    /**
     * Returns the number of elements in the list.
     */
    size_type size() const;

    /**
     * Returns true if the list has no elements, false otherwise.
     */
    bool empty() const;

    /**
     * Empties the list.
     */
    void clear();
    
    /**
     * Returns an iterator pointing to the first element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    iterator begin();

    /**
     * Returns an iterator pointing after the last element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    iterator end();
    
    /**
     * Returns a const_iterator pointing to the first element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    const_iterator begin() const;
    
    /**
     * Returns a const_iterator pointing after the last element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    const_iterator end() const;
    
    /**
     * Erases the element at position pos, and returns an iterator pointing
     * to the next element after pos.
     */
    iterator erase(iterator pos);
    
    /**
     * Erases the elements between first and last, and returns an iterator
     * pointing to the next element after last.
     */
    iterator erase(iterator first, iterator last);
    
    /**
     * Inserts the range [first, last) before pos, and returns an iterator
     * pointing after the last inserted element.
     */
    iterator insert(iterator pos, const_iterator first, const_iterator last);

    /**
     * Inserts n copies of x before pos, and returns an iterator pointing 
     * after the last inserted element.
     */
    iterator insert(iterator pos, size_type n, const T& x);
    
    /**
     * Inserts x before pos, and returns an iterator pointing after the
     * inserted element.
     */
    iterator insert(iterator pos, const T& x);

    /**
     * Removes all ocurrences of x found in the list. If the value
     * \a x is not in the list, the list remains unchanged.
     * @param x value to be removed from the list
     */
    void remove(const T& x);

    /**
     * Inserts x at the beginning of the list.
     */
    void push_front(const T& x);
    
    /**
     * Inserts x at the end of the list.
     */
    void push_back(const T& x);
    
    /**
     * Removes the first element from the list.
     */
    void pop_front();
    
    /**
     * Removes the last element from the list.
     */
    void pop_back();
    
    /**
     * Returns a reference to the first element of the list.
     */
    reference front();
    
    /**
     * Returns a const_reference to the first element of the list.
     */
    const_reference front() const;
    
    /**
     * Returns a reference to the last element of the list.
     */
    reference back();
    
    /**
     * Returns a const_reference to the last element of the list.
     */
    const_reference back() const;

    /**
     * Sorts this list according to the < operator.
     */
    void sort();

    /**
     * Sorts this list according to the comparison function \a comp
     * which must return a bool and take two arguments of type T.
     */
    template <class Compare>
    void sort(Compare comp);

    /**
     * Swaps the contents of this list with the given list.
     */
    void swap(smallObjectList<T>& l);
    
    /**
     * Inserts all elements from the given list \a other before the
     * given \a position, and removes them from the given list.
     *
     * This is a constant time operation.
     */
    void splice(iterator position, smallObjectList<T>& other);

    /**
     * Removes the element at \a it from list \a other and inserts it
     * in this list before \a position.
     *
     * This is NOT a constant time operation.  The computational cost is
     * equivalent to use insert and erase.  The method is just provided for
     * interface compatibility with the std::list.
     */
    void splice(iterator position, smallObjectList<T>& other, iterator it);

    /**
     * Removes the elements in the interval [\a begin, \a end] from
     * list \a other and inserts them before \a position in this
     * list.
     *
     * This is NOT a constant time operation.  The computational cost is
     * equivalent to use several insert and erase calls sequentially.  The
     * method is just provided for interface compatibility with the std::list.
     */
    void splice(iterator position, smallObjectList<T>& other, 
                iterator begin, iterator end);
    
    /**
     * Assignment operator. Clears this list, and copies the contents
     * of the given list.
     */
    smallObjectList<T>& operator=(const smallObjectList<T>& l);
    
    /**
     * Iterator class (allows read and write operations)
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library).
     */
    class iterator {
      
    public:
      
      /**
       * Iterator traits. These are required by the algorithms
       * of the STL.
       */
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef T value_type;
      typedef ptrdiff_t difference_type;
      typedef T* pointer;
      typedef T& reference;

    private:

      friend class smallObjectList<T>;
      friend class const_iterator;
      
      /**
       * Pointer to the current node.
       */
      node *node_;
 
    protected:

      /**
       * Creates an iterator for the given list, at the given position.
       */
      iterator(node *n) : node_(n) {
      }
      
    public:

      /**
       * Creates an uninitialized iterator.
       */
      iterator() : node_(0) {
      }

      /**
       * Copy constructor. Creates a copy of the given iterator.
       */
      iterator(const iterator& i) : node_(i.node_) {
      }

      /**
       * Returns true if both iterators are at the same position on the
       * same list, false otherwise.
       */
      bool operator == (const iterator& i) const {
        return node_ == i.node_;
      }
      
      /**
       * Returns false if both iterators are at the same position on the
       * same list, true otherwise.
       */
      bool operator != (const iterator& i) const {
        return node_ != i.node_;
      }
      
      /**
       * Returns a reference to the current element.
       */
      reference operator * () const {
        return node_->obj;
      }

//       /**
//        * Overwrites the current element with the given element.
//        */
//       reference operator * (T obj) const {
//         return node_->obj = obj;
//       }

      /**
       * Returns a pointer to the current element.
       */
      pointer operator->() const {
        return &((*node_).obj);
      }
      
      /**
       * Moves forward one element in the list, and returns itself.
       */
      iterator& operator ++ () {
        node_ = node_->next;
        return *this;
      }
      
      /**
       * Moves forward one element in the list, and returns a copy of itself
       * before the move.
       */
      iterator operator ++ (int) { 
        iterator tmp(*this);
        ++*this;
        return tmp;
      }

      /**
       * Moves backward one element in the list, and returns itself.
       */
      iterator& operator -- () {
        node_ = node_->prev;
        return *this;
      }
      
      /**
       * Moves backward one element in the list, and returns a copy of itself
       * before the move.
       */
      iterator operator -- (int) { 
        iterator tmp(*this);
        --*this;
        return tmp;
      }

    };

    /**
     * Const_iterator class (allows read-only operations).
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library).
     */
    class const_iterator {
  
    public:

      /**
       * Iterator traits. These are required by the algorithms
       * of the STL.
       */
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef T value_type;
      typedef ptrdiff_t difference_type;
      typedef T* pointer;
      typedef T& reference;

    private:
      
      friend class smallObjectList<T>;
      
      /**
       * Pointer to the current position.
       */
      const node *node_;
      
    protected:
      
      /**
       * Creates an const_iterator for the given list, at the given position.
       */
      const_iterator(const node *p) : node_(p) {
      }
      
    public:
      
      /**
       * Creates an uninitialized const_iterator.
       */      
      const_iterator() : node_(0) {
      }
      
      /**
       * Copy constructor. Creates a copy of the given const_iterator.
       */
      const_iterator(const const_iterator& i)
        : node_(i.node_) {
      }
      
      /**
       * Copy constructor. Creates a copy of the given iterator.
       */
      const_iterator(const iterator& i)
        : node_(i.node_) {
      }

      /**
       * Returns true if both iterators are at the same position on the
       * same list, false otherwise.
       */
      bool operator == (const const_iterator& i) const {
        return node_ == i.node_;
      }

      /**
       * Returns false if both iterators are at the same position on the
       * same list, true otherwise.
       */
      bool operator != (const const_iterator& i) const {
        return node_ != i.node_;
      }
      
      /**
       * Returns a const_reference to the current element.
       */
      const_reference operator * () const {
        return node_->obj;
      }

      /**
       * Returns a const_pointer to the current element.
       */
      const_pointer operator -> () const {
        return &(node_->obj);
      }
      
      /**
       * Moves forward one element in the list, and returns itself.
       */
      const_iterator& operator ++ () {
        node_ = node_->next;
        return *this;
      }
      
      /**
       * Moves forward one element in the list, and returns a copy of itself
       * before the move.
       */
      const_iterator operator ++ (int) { 
        const_iterator tmp(*this);
        ++*this;
        return tmp;
      }

      /**
       * Moves backward one element in the list, and returns itself.
       */
      const_iterator& operator -- () {
        node_ = node_->prev;
        return *this;
      }

      /**
       * Moves backward one element in the list, and returns a copy of itself
       * before the move.
       */
      const_iterator operator -- (int) { 
        const_iterator tmp(*this);
        --*this;
        return tmp;
      }

    };

  private:
    
    /**
     * Node structure.
     *
     * Represents a node of the linked list.
     */
    struct node {

      /**
       * default constructor
       */
      node() : obj(), prev(0), next(0) {};

      /**
       * copy constructor
       */
      node(const node& other) 
        : obj(other.obj), prev(other.prev), next(other.next) {};

      /**
       * copy the other node
       */
      node& operator=(const node& other) {
        obj  = other.obj;
        prev = other.prev;
        next = other.next;
        return *this;
      }
      
      /**
       * The stored element.
       */
      T obj;

      /**
       * Pointer to the previous node.
       */
      node *prev;

      /**
       * Pointer to the next node.
       */
      node *next;

    };
    
    /**
     * A segment of the heap, used to allocate memory for nodes. Since
     * these nodes are used for a linked list, the unused nodes are
     * kept in a (singly) linked list of free nodes.
     */
    class segment {
      
      friend class heap;
      
    private:

      /**
       * The array of nodes contained in this segment
       */
      node nodes_[_LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE];

      /**
       * A pointer to the first free node of the segment
       */
      node *firstFree;
      
      /**
       * A pointer to the next segment in the heap
       */
      segment* next;

      /**
       * A pointer to the previous segment in the heap
       */
      segment* prev;

      /**
       * The number of nodes currently allocated in this segment
       */
      short int nodeCount;

      /**
       * don't copy it
       */
      segment& operator=(const segment& o) {}

      segment(const segment& o) {}

    public:

      /**
       * Constructor. Initializes the list of free nodes.
       */
      segment();

      /**
       * Destructor
       */
      ~segment();

      /**
       * Determines if the given node is contained in this segment, that is,
       * if the pointer points within the inner node array.
       */
      inline bool contains(node *node);

      /**
       * 'Grabs' (allocates) a node in this segment.
       */
      inline node* grab();

      /**
       * Releases the given node in this segment
       */
      inline void release(node *n);
      
    };
    
    /**
     * A heap of nodes.
     *
     * Stores nodes in dynamically allocated segments.
     */
    class heap {

    private:

      /**
       * A pointer to the first segment in the list of segments
       */
      segment *first_;
      
      /**
       * A pointer to the segment most recently used for allocation
       */
      segment *recentAlloc_;

      /**
       * A pointer to the segment most recently used for deallocation
       */
      segment *recentDealloc_;

      /**
       * The total object count contained in this heap
       */
      unsigned long int objectCount_;

      /**
       * The number of segments in this heap
       */
      unsigned long int segmentCount_;
      
    public:
      
      /**
       * Constructor
       */
      heap();
      
      /**
       * Destructor
       */
      ~heap();
      
      /**
       * Disable the copy constructor
       */
      heap(const heap&);

      /**
       * Disable the assignment operator
       */
      heap& operator= (const heap&);

      /*
       * The singleton instance accessor
       */
      //static heap& instance();
      
      /**
       * Allocates a new node in the heap
       */
      node *allocate();
      
      /**
       * Frees the given node from the heap
       */
      void deallocate(node *n);

      /**
       * Transfer all data to the other heap
       */
      void detach(heap& other);

      /**
       * Assign all data of the other heap to this one
       */
      void attach(heap& other);

      /**
       * Swap the contents of two heaps
       */
      void swap(heap& other);
    };

    /**
     * The heap of nodes, used for memory allocation and management.
     */
    heap heap_;
    
    /**
     * The 'end' node. This node marks the end of the list, and contains no
     * user data. Its 'next' is the first element of the list, and its 'prev' 
     * is the last. Therefore, for an empty list, both of these pointers point
     * at end_;
     */
    node end_;

    /**
     * Quicksort implementation used by sort().
     */
    void quicksort(iterator front, iterator back);

    /**
     * Quicksort implementation used by sort(comp). See sort(Compare
     * comp) for details.
     */
    template <class Compare>
    void quicksort(iterator front, iterator back, Compare comp);

    /**
     * Moves the elements from [\a first,\a last) before \a position
     *
     * This is a helper function for splice().
     */
    /*
    void spliceHelper(iterator position, 
                      smallObjectList<T>& other,
                      iterator first, 
                      iterator last);
    */ 

  };
  
}

#include "ltiSmallObjectList_template.h"
#include "ltiSmallObjectList_inline.h"

#endif
