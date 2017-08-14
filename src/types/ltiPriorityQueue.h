/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiPriorityQueue.h
 *         Contains the template class lti::priorityQueue used to manage queues
 *         in which the entries are sorted by a priority factor.
 * 
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiPriorityQueue.h,v 1.1 2008-04-21 01:31:21 alvarado Exp $
 */


#ifndef _LTI_PRIORITY_QUEUE_H_
#define _LTI_PRIORITY_QUEUE_H_

#include "ltiContainer.h"
#include "ltiSTLIoInterface.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {


  /**
   * Simple priority queue data class.
   *
   * The main difference with the std::priority_queue is that when you insert
   * an element in the queue, you get an identification token (positive
   * integer value), that will uniquely identify the element in the queue as
   * long as the element exists.  This is used to access the specific element
   * independently of its position in the queue, allowing to change its key or
   * data, changing its positions, but still keeping a valid reference to it.
   *
   * The queue is always sorted in increasing order of the key.  This means
   * that the smallest key is always on the top of the queue.
   *
   * Removing elements of the queue (pop() or erase()) are O(1) operations.
   * Inserting an elements is a O(n) operation.  Therefore, a specialized
   * method to create the queue at once (create()) is provided, which will
   * build the queue in O(n*log(n)), instead of the O(n^2) required inserting
   * the elements one by one.
   *
   * The type T is the type for the key.  The queue is sorted with
   * respect to this key.
   *
   * The type U is the type of the data contained in each element.  
   */
  template<class T,class U>
  class priorityQueue : public container {
  public:
    
    /**
     * Index type used to reference elements in the queue
     */
    typedef int index_type;
    
    /**
     * Constructor
     *
     * @param inv key used to indicate an invalid queue entry.  Entries
     *            are for example invalid, if they were removed or "poped"
     *            from the queue.
     */
    priorityQueue(const T& inv);
    
    /**
     * Copy constructor
     */
    priorityQueue(const priorityQueue<T,U>& other);

    /**
     * Destructor
     */
    virtual ~priorityQueue();
    
    /**
     * Insert a new node with the given key and data and return
     * a pointer to the created node
     * @param key key of the element
     * @param data data of the element
     */
    index_type insert(const T& key,const U& data);
    
    /**
     * Remove the node with the given id
     */
    void erase(const index_type id);
    
    /**
     * Update the key of an element without changing its id
     */
    void update(const index_type id,
                const T& newKey);
    
    /**
     * Update the key of an element without changing its id
     */
    void update(const index_type id,
                const T& newKey,
                const U& data);
    
    /**
     * Check if the given id is valid
     */
    bool valid(const index_type id) const;
    
    /**
     * Get the key for the given index id.  You must ensure first that the
     * id is valid
     */
    const T& getKey(const index_type id) const;

    /**
     * Get the data for the given index id.  You must ensure first that the
     * id is valid
     */
    const U& getData(const index_type id) const;

    /**
     * Change the data content of an element indexed by the given id.
     * You must first ensure that the id is valid.
     * @param id identification key for the element to be modified.
     * @param newData new data to be written in the element
     * @return true if successful, false otherwise.
     */
    bool setData(const index_type id,const U& newData);

    /**
     * Return true if the queue is empty
     */
    bool empty() const;
    
    /**
     * Create the queue using the given data.  The ids will be the original
     * element index
     */
    void create(const std::vector<T>& key,
                const std::vector<U>& data);
    
    /**
     * Create the queue using the given data.  The ids will be the original
     * element index
     */
    void create(const std::vector< std::pair<T,U> >& data);

    /**
     * Return reference to element at the front
     */
    const std::pair<T,U>& front() const;
    
    /**
     * Copy all data in the priority queue into the given std::vector.
     */
    void getData(std::vector< std::pair<T,U> >& d) const;
    
    /**
     * Remove the first element of the queue
     */
    void pop();
    
    /**
     * Remove all elements from the priority queue
     */
    void clear();
    
    /**
     * Copy the priority queue
     * @param other the priority queue to be copied.
     * @return a reference to the current object
     */
    priorityQueue<T,U>& copy(const priorityQueue<T,U>& other);

    /**
     * Copy the priority queue
     * @param other the priority queue to be copied.
     * @return a reference to the current object
     */
    priorityQueue<T,U>& operator=(const priorityQueue<T,U>& other);

    /**
     * Returns a copy of this object
     */
    virtual priorityQueue<T,U>* clone() const;

    /**
     * Returns a copy of this object
     */
    virtual priorityQueue<T,U>* newInstance() const;

    /**
     * Returns name of this type
     */
    const std::string& name() const;

    /**
     * Write the priority queue
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;
    
    /**
     * Read the priority queue
     */
    virtual bool read(ioHandler& handler,const bool complete=true);
    
    /**
     * Write all attributes of the class
     *
     * To serialize a priority queue, not all data is necessary, but
     * for debugging purposes you might want to have all internals written.
     * This method writes everything.
     */
    virtual bool writeAll(ioHandler& handler,const bool complete=true) const;
    
    /**
     * Read the output of the writeAll method.
     */
    virtual bool readAll(ioHandler& handler,const bool complete=true);

  protected:
    
    /**
     * Debugging method to check the consistency of the priority queue
     */
    bool checkConsistency() const;
    
    /**
     * Code used to indicate a removed element'
     */
    T invalid_;
    
    /**
     * The real data.  Some of its elements can be removed, so the real
     * number of elements is NOT data.size() but numElements.  The data
     * here is sorted.  Because the indices of the elements will change
     * each time an element is inserted/removed, an equivalences vector
     * will keep track of the element movements.
     */
    std::vector< std::pair<T,U> > data_;
    
    /**
     * Number of elements in data
     */
    int numElements_;
    
    /**
     * Top of the stack.  This pointer MUST always point to the first valid 
     * element!
     */
    int stackPtr_;
    
    /**
     * Number of elements removed until now
     */
    int numRemoved_;
    
    /**
     * Get the real element position in the data vector given the
     * original index.
     */
    std::vector<int> idToIndex_;
    
    /**
     * Get the real element position in the data vector given the
     * original index.
     */
    std::vector<int> indexToId_;
  };

}

#include "ltiPriorityQueue_template.h"
#include "ltiUndebug.h"
#endif
