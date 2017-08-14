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
 * \file   ltiSmallObjectList_template.h
 *         Defines a list similar to the std::list but specialized for
 *         an efficient access of small objects.
 * \author Gustavo Quiros
 * \date   12.12.03
 *
 * $Id: ltiSmallObjectList_template.h,v 1.14 2008-08-06 03:40:58 alvarado Exp $
 */

namespace lti {

  /*
   * Default constructor. Creates an empty smallObjectList.
   */
  template <typename T>
  smallObjectList<T>::smallObjectList() {
    // : heap_(heap::instance()), end_() {
    end_.next = &end_;
    end_.prev = &end_;
  }
  
  /*
   * Copy constructor. Creates a smallObjectList with the same
   * contents as the given list.
   */
  template <typename T>
  smallObjectList<T>::smallObjectList(const smallObjectList& l) {
    // : heap_(heap::instance()), end_() {
    end_.next = &end_;
    end_.prev = &end_;
    operator=(l); // use the "=" operator to copy the other list
  }

  /*
   * Destructor
   */
  template <typename T>
  smallObjectList<T>::~smallObjectList() { 
    clear(); 
  }

  /*
   * Returns the number of elements in the list.
   */
  template<typename T>
  typename smallObjectList<T>::size_type 
  smallObjectList<T>::size() const { 
    size_type count = 0;
    const const_iterator e = end();
    for(const_iterator it = begin(); it != e; ++it, ++count) {
    }
    return count;
  }
  
  /*
   * Erases the element at position pos, and returns an iterator pointing
   * to the next element after pos.
   */
  template <typename T>
  typename smallObjectList<T>::iterator 
  smallObjectList<T>::erase(iterator pos) {
    node *theNode = pos.node_;
    theNode->prev->next = theNode->next;
    theNode->next->prev = theNode->prev;
    iterator it(theNode->next);
    heap_.deallocate(theNode);
    return it;
  }

  /*
   * Erases the elements between first and last, and returns an iterator
   * pointing to the next element after last.
   */
  template <typename T>
  typename smallObjectList<T>::iterator 
  smallObjectList<T>::erase(iterator first,iterator last) {
    while(first != last) {
      first = erase(first);
    }
    return first;
  }

  /*
   * Inserts the range [first, last) before pos, and returns an iterator
   * pointing after the last inserted element.
   */
  template <typename T>
  typename smallObjectList<T>::iterator
  smallObjectList<T>::insert(iterator pos, 
                             const_iterator first, 
                             const_iterator last) {
    while(first != last) {
      pos = insert(pos,*first);
      ++first;
    }
    return pos;
  }

  /*
   * Inserts n copies of x before pos, and returns an iterator pointing 
   * after the last inserted element.
   */
  template <typename T>
  typename smallObjectList<T>::iterator
  smallObjectList<T>::insert(iterator pos, size_type n, const T& x) {
    for(size_type i = 0; i < n; ++i) {
      pos = insert(pos,x);
    }
    return pos;
  }

  /*
   * Inserts x before pos, and returns an iterator pointing after the
   * inserted element.
   */
  template <typename T>
  typename smallObjectList<T>::iterator 
  smallObjectList<T>::insert(iterator pos, const T& x) {
    node *next = pos.node_;
    node *newNode = heap_.allocate();
    newNode->obj = x;
    newNode->prev = next->prev;
    newNode->next = next;
    newNode->prev->next = newNode;
    next->prev = newNode;
    return pos;
  }

  /*
   * Removes all ocurrences of x found in the list.
   */
  template <typename T>
  void smallObjectList<T>::remove(const T& x) {
    node *n = end_.next;
    while(n != &end_){
      if(n->obj == x){
        node *prev = n->prev, *next = n->next;
        prev->next = next;
        next->prev = prev;
        heap_.deallocate(n);
        n = next;
      }
      else{
        n = n->next;
      }
    }
  }

  /*
   * Inserts x at the beginning of the list.
   */
  template <typename T>
  void smallObjectList<T>::push_front(const T& x) {
    node *newNode = heap_.allocate();
    newNode->obj = x;
    newNode->next = end_.next;
    newNode->prev = &end_;
    end_.next->prev = newNode;
    end_.next = newNode;
  }
  
  /*
   * Inserts x at the end of the list.
   */
  template <typename T>
  void smallObjectList<T>::push_back(const T& x) {
    node *newNode = heap_.allocate();
    newNode->obj = x;
    newNode->next = &end_;
    newNode->prev = end_.prev;
    end_.prev->next = newNode;
    end_.prev = newNode;
  }
  
  /*
   * Removes the first element from the list.
   */
  template <typename T>
  void smallObjectList<T>::pop_front() {
    if(!empty()) {
      node *newFirst = end_.next->next;
      newFirst->prev = &end_;
      heap_.deallocate(end_.next);
      end_.next = newFirst;
    }
  }
  
  /*
   * Removes the last element from the list.
   */
  template <typename T>
  void smallObjectList<T>::pop_back() {
    if(!empty()) {
      node *newLast = end_.prev->prev;
      newLast->next = &end_;
      heap_.deallocate(end_.prev);
      end_.prev = newLast;
    }
  }
  
  /*
   * Swaps the contents of this list with the given list.
   */
  template <typename T>
  void smallObjectList<T>::swap(smallObjectList<T>& l) {
    // swap the heaps
    heap_.swap(l.heap_);


    // and now the pointers to the elements in the heaps
    if(empty()){
      if(l.empty()){
        // Both are empty
        return;
      }
      else{
        // This one is empty, that one isn't
        end_.next = l.end_.next;
        end_.prev = l.end_.prev;
        end_.next->prev = &end_;
        end_.prev->next = &end_;
        l.end_.next = &l.end_;
        l.end_.prev = &l.end_;
      }
    }
    else{
      if(l.empty()){
        // This one is not empty, that one is
        l.end_.next = end_.next;
        l.end_.prev = end_.prev;
        l.end_.next->prev = &l.end_;
        l.end_.prev->next = &l.end_;
        end_.next = &end_;
        end_.prev = &end_;
      }
      else{
        // Neither is empty
        std::swap(end_.next,l.end_.next);
        std::swap(end_.prev,l.end_.prev);
        std::swap(end_.next->prev,l.end_.next->prev);
        std::swap(end_.prev->next,l.end_.prev->next);
      }
    }
  }
  
  /**
   * Inserts all elements from the given list before the given position,
   * and removes them from the given list. This is a constant
   * time operation.
   */
  template <typename T>
  void smallObjectList<T>::splice(iterator position, smallObjectList<T>& l) {

    if(!l.empty()){
      l.end_.next->prev = position.node_->prev;
      position.node_->prev->next = l.end_.next;
      l.end_.prev->next = position.node_;
      position.node_->prev = l.end_.prev;
      l.end_.next = l.end_.prev = &l.end_;

      l.heap_.detach(heap_);
    }
  }

  /**
   * Inserts element at position 'it' in the given list before the given 
   * position, and removes it from the given list.
   */
  template <typename T>
  void smallObjectList<T>::splice(iterator position, 
                                  smallObjectList<T>& other,
                                  iterator it) {
    insert(position,*it);
    other.erase(it);

    // the next code does not work any more, since now the heap is 
    // managed by each instance

//     iterator it2 = it;
//     // Nothing todo when inserting at same position or before itself
//     if ((position == it) || (position == ++it2)) return;
//     //spliceHelper(position, other, it, it2);
//     node *n = it.node_;
//     node *newNext = position.node_;
//     node *newPrev = newNext->prev;
//     node *oldNext = n->next;
//     node *oldPrev = n->prev;
//     // Insert here
//     n->next = newNext;
//     n->prev = newPrev;
//     newNext->prev = n;
//     newPrev->next = n;
//     // Remove there
//     oldNext->prev = oldPrev;
//     oldPrev->next = oldNext;
  }

  /**
   * Moves elements from [it,ite) in the given list to this list,
   * inserting them before the given position. This is a constant time
   * operation.
   */ 
  template <typename T>
  void smallObjectList<T>::splice(iterator position, 
                                  smallObjectList<T>& other,
                                  iterator it, iterator ite) {

    insert(position,it,ite);
    other.erase(it,ite);

    // The next code does not work anymore, since now the heaps are managed
    // in and instance-based way, rather than globally (multithreading issues)

//     // Nothing is moved is [it,ite) is empty, or if inserting before ite.
//     if ((it==ite) || (position==ite)) return;
//     // spliceHelper(position, it, ite);
//     node *n1 = it.node_;
//     node *n2 = ite.node_->prev;
//     node *newNext = position.node_;
//     node *newPrev = newNext->prev;
//     node *oldNext = n2->next;
//     node *oldPrev = n1->prev;
//     // Insert here
//     n2->next = newNext;
//     n1->prev = newPrev;
//     newNext->prev = n2;
//     newPrev->next = n1;
//     // Remove there
//     oldNext->prev = oldPrev;
//     oldPrev->next = oldNext;
  }

  /*
   * Assignment operator. Clears this list, and copies the contents
   * of the given list.
   */
  template <typename T>
  smallObjectList<T>& 
  smallObjectList<T>::operator=(const smallObjectList<T>& l) {
    clear();
    const const_iterator e = l.end();
    for(const_iterator i = l.begin(); i != e; ++i) {
      push_back(*i);
    }
    return *this;
  }

  /*
   * Sorts this list according to the < operator.
   */
  template <typename T>
  void smallObjectList<T>::sort() {
    quicksort(begin(),--end());
  }

  /*
   * Sorts this list according to the compare function \a comp.
   */
  template <typename T>
  template <typename Compare>
  void smallObjectList<T>::sort(Compare comp) {
    quicksort(begin(),--end(),comp);
  }

  /*
   * Implementation of quicksort
   */
  template <typename T>
  void smallObjectList<T>::quicksort(iterator first, iterator last) {
    // Nothing to sort, return
    if(first == last) return;    
    iterator f = first, b = last;
    T pivot = *f;
    while(f != b) {
      while(pivot < *b && f != b) --b;
      if(f != b) { *f = *b; ++f; }
      while(!(pivot < *f) && f != b) ++f;
      if(f != b) { *b = *f; --b; }
    }
    *f = pivot;
    if(f != first) quicksort(first,--f);
    if(b != last) quicksort(++b,last);
  }

  /*
   * Implementation of quicksort
   */
  template <typename T>
  template <typename Compare>
  void smallObjectList<T>::quicksort(iterator first, iterator last,
                                     Compare comp) {
    // Nothing to sort, return
    if(first == last) {
      return;    
    }
    iterator f = first, b = last;
    T pivot = *f;
    while(f != b) {
      while(comp(pivot,*b) && f != b) --b;
      if(f != b) { *f = *b; ++f; }
      while(!comp(pivot,*f) && f != b) ++f;
      if(f != b) { *b = *f; --b; }
    }
    *f = pivot;
    if(f != first) quicksort(first,--f,comp);
    if(b != last) quicksort(++b,last,comp);
  }
    
  /*
   * Constructor for segment. Initializes the list of free nodes.
   */
  template <typename T>
  smallObjectList<T>::segment::segment()
    : firstFree(&nodes_[0]), next(0), prev(0), nodeCount(0) {
    for(int i = 0;
        i < _LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE - 1;
        ++i) {
      nodes_[i].next = &nodes_[i+1];
    }
    nodes_[_LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE - 1].next = 0;
  }

  /*
   * Destructor for segment.
   */
  template <typename T>
  smallObjectList<T>::segment::~segment() {
  }

  /**
   * Constructor for heap.
   */
  template <typename T>
  smallObjectList<T>::heap::heap() : first_(new segment),
                                     recentAlloc_(first_), 
                                     recentDealloc_(first_),
                                     objectCount_(0), 
                                     segmentCount_(1) {
  }

  /**
   * Destructor for heap.
   */
  template <typename T>
  smallObjectList<T>::heap::~heap() {
    segment *s, *n;
    s = first_;
    while(s) {
      n = s->next;
      delete s;
      s = n;
    }
  }

  /*
   * The singleton instance accessor for heap.
   */
//   template <typename T>
//   typename smallObjectList<T>::heap& smallObjectList<T>::heap::instance(){
//     static heap theInstance;
//     return theInstance;
//   }

  /*
   * Allocates a new node in the heap
   */
  template <typename T>
  typename smallObjectList<T>::node *smallObjectList<T>::heap::allocate() {
    if (!recentAlloc_->firstFree) {
      // The recent alloc segment is full
      if(objectCount_ < segmentCount_ *
	              _LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE) {
        // We know that there must be at least one free node in some segment,
        // so look for it.
        // First try the recentDealloc
        if(recentDealloc_->firstFree) {
          recentAlloc_ = recentDealloc_;
        }
        else {
          // Search bidirectionally
          segment *up = recentAlloc_->next;
          segment *down = recentAlloc_->prev;
          recentAlloc_ = 0;
          while(true) {
            if(up) {
              if(up->firstFree) { recentAlloc_ = up; break; }
              up = up->next;
            }
            else {
              // Only down now
              while(down) {
                if(down->firstFree) { recentAlloc_ = down; break; }
                down = down->prev;
              }
              break;
            }
            if(down) {
              if(down->firstFree) { recentAlloc_ = down; break; }
              down = down->prev;
            }
            else {
              // Only up now
              while(up) {
                if(up->firstFree) { recentAlloc_ = up; break; }
                up = up->next;
              }
              break;
            }
          }
          if(!recentAlloc_) {
            // for debug mode use assert, which is easier to track
            assert(!"Free node not found when allocating smallObjectList.");
            // We didn't find the free node, this should never happen!
            throw exception("Fatal: Free node not found when allocating " \
                            "(smallObjectList).");
          }
        }
      }
      else{
        // All segments are full, so add a new one
        recentAlloc_ = new segment;
        if(!recentAlloc_){
          // for debug mode use an assert, which is easier to track
          assert(!"Fatal: Couldn't allocate memory for list segment.");
          // Couldn't allocate a new segment, we probably ran out of memory
          throw exception("Fatal: Couldn't allocate memory for segment " \
                          "(smallObjectList).");
        }
        // Add the new segment at the begining of the list
        recentAlloc_->next = first_;
        first_->prev = recentAlloc_;
        first_ = recentAlloc_;
        ++segmentCount_;
      }
    }
    ++objectCount_;
    return recentAlloc_->grab();
  }
  
  /*
   * Frees the given node from the heap
   */
  template <typename T>
  void smallObjectList<T>::heap::deallocate(node *n) {
    if(!recentDealloc_->contains(n)) {
      // Node not in recentDealloc, find segment that contains it
      if(recentAlloc_->contains(n)) {
        recentDealloc_ = recentAlloc_;
      }
      else{
        // Search bidirectionally
        segment *up = recentDealloc_->next;
        segment *down = recentDealloc_->prev;
        recentDealloc_ = 0;
        while(true) {
          if(up) {
            if(up->contains(n)) { recentDealloc_ = up; break; }
            up = up->next;
          }
          else {
            // Only down now
            while(down) {
              if(down->contains(n)) { recentDealloc_ = down; break; }
              down = down->prev;
            }
            break;
          }
          if(down) {
            if(down->contains(n)) { recentDealloc_ = down; break; }
            down = down->prev;
          }
          else {
            // Only up now
            while(up) {
              if(up->contains(n)) { recentDealloc_ = up; break; }
              up = up->next;
            }
            break;
          }
        }
        if(!recentDealloc_) {
          assert(!"Fatal: Node not found when deallocating smallObjectList.");
          // Node not found, this should never happen!!!
          throw exception("Fatal: Node not found when deallocating " \
                          "(smallObjectList).");
        }
      }
    }
    recentDealloc_->release(n);
    --objectCount_;
    if(recentDealloc_->nodeCount == 0 && recentDealloc_ != first_) {
      segment *s = recentDealloc_;
      // Set the previous segment as the current recentDealloc_
      recentDealloc_ = s->prev;
      // If necessary, set the previous segment as the current recentAlloc_
      if(recentAlloc_ == s) recentAlloc_ = s->prev;
      if(s->next) s->next->prev = s->prev;
      // We know there is a prev, because this segment is not the first
      s->prev->next = s->next;
      --segmentCount_;
      delete s;
    }
  }


  template <typename T>
  void smallObjectList<T>::heap::detach(heap& other) {
    other.attach(*this);

  }
  
  template <typename T>
  void smallObjectList<T>::heap::attach(heap& other) {
    // find the last segment of this list
    segment* s=first_;
    while(notNull(s->next)) {
      s=s->next;
    }
    // s is the last valid segment of this heap's segment list
    s->next = other.first_;
    s->next->prev = s;
    
    objectCount_  += other.objectCount_;
    segmentCount_ += other.segmentCount_;
    
    // now ensure a state in other as freshly created:
    other.first_         = new segment;
    other.recentAlloc_   = other.first_;
    other.recentDealloc_ = other.first_;
    other.objectCount_   = 0;
    other.segmentCount_  = 1;
  }

  template <typename T>
  void smallObjectList<T>::heap::swap(heap& other) {
    std::swap(first_,other.first_);
    std::swap(recentAlloc_,other.recentAlloc_);
    std::swap(recentDealloc_,other.recentDealloc_);
    std::swap(objectCount_,other.objectCount_);
    std::swap(segmentCount_,other.segmentCount_);
  }

  /*
  // Moves the elements from [first,last) before position.
  template <typename T>
  void smallObjectList<T>::spliceHelper(iterator position, 
                                        smallObjectList& other,
                                        iterator first, 
                                        iterator last) {
    // if [first,last) spans the whole list call that splice function
    if ( (first.node_==other.first_) && (last.node_==0)) {
      splice(position, other);
      return;
    }

    if(position.node_ == 0) {
      if (first.node_ == other.first_) {
        // Remove [first, last) from its old position.
        last.node_->prev->next     = 0;
        other.first_              = last.node_;
        last_->next               = first.node_;
      
        // Splice [first, last) into its new position.
        node* tmp           = last_;
        last_               = last.node_->prev;
        last.node_->prev     = first.node_->prev;
        first.node_->prev    = tmp;
      } else if (last.node_==0) {
        // Remove [first, last) from its old position.
        //other.last_->next         = 0; //not needed, already thus
        first.node_->prev->next    = 0;
        last_->next               = first.node_;
      
        // Splice [first, last) into its new position.
        node* tmp           = last_;
        last_               = other.last_;
        other.last_         = first.node_->prev;
        first.node_->prev    = tmp;
      } else {
        // Remove [first, last) from its old position.
        last.node_->prev->next     = 0;
        first.node_->prev->next    = last.node_;
        last_->next               = first.node_;
      
        // Splice [first, last) into its new position.
        node* tmp           = last_;
        last_               = last.node_->prev;
        last.node_->prev     = first.node_->prev;
        first.node_->prev    = tmp;
      }
    } else if(position.node_ == first_) {
      if (first.node_ == other.first_) {
        first_->prev           = last.node_->prev;
        last.node_->prev->next  = first_;
        last.node_->prev        = 0;
        other.first_           = last.node_;
        first_                 = first.node_;
      } else if (last.node_==0) {
        first_->prev           = other.last_;
        other.last_->next      = first_;
        other.last_            = first.node_->prev;
        other.last_->next      = 0;
        first_                 = first.node_;
        first_->prev           = 0;
      } else {
        first_->prev           = last.node_->prev;
        last.node_->prev->next  = first_;
        last.node_->prev        = first.node_->prev;
        first.node_->prev->next = last.node_;
        first_                 = first.node_;
        first_->prev           = 0;
      }
    } else {
      if (first.node_ == other.first_) {
        // Remove [first, last) from its old position.
        last.node_->prev->next     = position.node_;
        other.first_              = last.node_;
        position.node_->prev->next = first.node_;
        
        // Splice [first, last) into its new position.
        node* tmp           = position.node_->prev;
        position.node_->prev = last.node_->prev;
        last.node_->prev     = 0;
        first.node_->prev    = tmp;
      } else if (last.node_==0) {
        // Remove [first, last) from its old position.
        other.last_->next         = position.node_;
        first.node_->prev->next    = 0;
        position.node_->prev->next = first.node_;
      
        // Splice [first, last) into its new position.
        node* tmp           = position.node_->prev;
        position.node_->prev = other.last_;
        other.last_         = first.node_->prev;
        first.node_->prev    = tmp;
      } else {
        // Remove [first, last) from its old position.
        last.node_->prev->next     = position.node_;
        first.node_->prev->next    = last.node_;
        position.node_->prev->next = first.node_;
      
        // Splice [first, last) into its new position.
        node* tmp           = position.node_->prev;
        position.node_->prev = last.node_->prev;
        last.node_->prev     = first.node_->prev;
        first.node_->prev    = tmp;
      }
    }
  }
  */
  
}

