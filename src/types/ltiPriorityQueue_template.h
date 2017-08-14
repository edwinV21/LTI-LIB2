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
 * \file   ltiPriorityQueue_template.h
 *         Contains the template class lti::priorityQueue used to manage queues
 *         in which the entries are sorted by a priority factor.
 * 
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiPriorityQueue_template.h,v 1.3 2008-04-27 02:43:08 alvarado Exp $
 */

#include "ltiSort2.h"

namespace lti {
  

  template<class T,class U>
  priorityQueue<T,U>::priorityQueue(const T& inv) 
    : container(),invalid_(inv) {
    clear();
  }

  template<class T,class U>
  priorityQueue<T,U>::priorityQueue(const priorityQueue<T,U>& other) 
    : container() {
    copy(other);
  }

  template<class T,class U>
  priorityQueue<T,U>::~priorityQueue() {
    clear();
  }

  template<class T,class U>
  const std::string& priorityQueue<T,U>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  template<class T,class U>
  priorityQueue<T,U>* priorityQueue<T,U>::clone() const {
    return new priorityQueue<T,U>(*this);
  }

  template<class T,class U>
  priorityQueue<T,U>* priorityQueue<T,U>::newInstance() const {
    return new priorityQueue<T,U>(invalid_);
  }

  template<class T,class U>
  priorityQueue<T,U>& 
  priorityQueue<T,U>::copy(const priorityQueue<T,U>& other) {
    invalid_     = other.invalid_;
    data_        = other.data_;
    numElements_ = other.numElements_;
    stackPtr_    = other.stackPtr_;
    numRemoved_  = other.numRemoved_;
    idToIndex_   = other.idToIndex_;
    indexToId_   = other.indexToId_;
    return *this;
  }

  template<class T,class U>
  priorityQueue<T,U>& 
  priorityQueue<T,U>::operator=(const priorityQueue<T,U>& other) {
    return copy(other);
  }

  template<class T,class U>
  void priorityQueue<T,U>::clear() {
    data_.clear();
    numElements_=0;
    stackPtr_=0;
    numRemoved_ = 0;
    idToIndex_.clear();
  }

  template<class T,class U>
  void priorityQueue<T,U>::pop() {
    assert(numElements_>0);

    _lti_debug3("PQ: pop(), still left " << numElements << " -> ");
    erase(indexToId_[stackPtr_]);
  }
  
  template<class T,class U>
  bool priorityQueue<T,U>::empty() const {
    return (numElements_ == 0);
  }

  template<class T,class U>
  const std::pair<T,U>& priorityQueue<T,U>::front() const {
    return data_[stackPtr_];
  }

  template<class T,class U>
  void priorityQueue<T,U>::getData(std::vector< std::pair<T,U> >& d) const {
    d.clear();
    typename std::vector< std::pair<T,U> >::const_iterator it;
    for (it=data_.begin()+stackPtr_;it!=data_.end();++it) {
      if ((*it).first != invalid_) {
        d.push_back(*it);
      }
    }
    
  }

  template<class T,class U>
  bool priorityQueue<T,U>::valid(const index_type id) const {
    return ((static_cast<unsigned int>(id) < idToIndex_.size()) &&
            (data_[idToIndex_[id]].first != invalid_));
  }

  template<class T,class U>
  const T& priorityQueue<T,U>::getKey(const index_type id) const {
    if (static_cast<unsigned int>(id) < idToIndex_.size()) {
      return data_[idToIndex_[id]].first;
    } else {
      return invalid_;
    }
  }

  template<class T,class U>
  const U& priorityQueue<T,U>::getData(const index_type id) const {
    if (static_cast<unsigned int>(id) < idToIndex_.size()) {
      return data_[idToIndex_[id]].second;
    } else {
      static const U dummyData = U();
      return dummyData;
    }
  }

  template<class T,class U>
  bool priorityQueue<T,U>::setData(const index_type id,
                                   const U& newData) {
    if (static_cast<unsigned int>(id) < idToIndex_.size()) {
      data_[idToIndex_[id]].second = newData;
      return true;
    }
    return false;
  }

  /* --------------
   * Insert method
   * --------------
   */
  template<class T,class U>
  typename priorityQueue<T,U>::index_type
  priorityQueue<T,U>::insert(const T& key,const U& d) {

    _lti_debug4("PQ::insert key " << key << ", data " << d);
    
    int i(stackPtr_),lastFree(i-1),j;
    int id,nid;
    int pre(i); // number of "free" spaces before i

    // compute position where the new element should be inserted
    while ((i<static_cast<int>(data_.size())) && 
           ((data_[i].first < key) ||
            (data_[i].first == invalid_))) {
      if (data_[i].first == invalid_) {
        lastFree=i;
        ++pre;
      }
      ++i;
    }

    _lti_debug4(" at idx " << i);

    // new element should be at exactly position i

    // if pre>0, we can avoid the resizing of the whole vectors and just
    // shift the elements before i, updating of course the equivalence vectors

    if (pre>0) {
      _lti_debug4(" shift to left." << std::endl);
      _lti_debug4("    stackPtr: " << stackPtr << std::endl);
      _lti_debug4("    lastFree: " << lastFree << std::endl);
      // just shift to the left

      // reuse an old label
      nid = indexToId_[lastFree];

      j=lastFree;
      
      if (!(data_[i].first < key)) {
        i--;
      }
      
      const int last = min(i,static_cast<int>(data_.size())-1);
      while(j<last) {
        data_[j]=data_[j+1];
        id = ( indexToId_[j] = indexToId_[j+1] );
        idToIndex_[id]--;
        ++j;
      }
      
      idToIndex_[nid]=last;
      indexToId_[last]=nid;
      data_[last]=std::make_pair(key,d);
      numRemoved_--; // one removed element has been reused
      
      if (lastFree < stackPtr_) {
        stackPtr_--;
      }
    
    } else {
      _lti_debug4(" shift to right." << std::endl);

      // pre == 0 here!
      // shift to the right.  This maybe requires increasing the vector size!
      // numRemoved - pre should be the number of "free" spaces on the right
      // side
      if (numRemoved_ <= 0) {
        if (i==static_cast<int>(data_.size())) {
          // element must be inserted at the end.
          data_.push_back(std::make_pair(key,d));
          // new id will be the new index in the data vector!
          id = static_cast<int>(data_.size())-1;
          indexToId_.push_back(id);
          idToIndex_.push_back(id);
          numElements_++;

#       if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
          checkConsistency();
#       endif    

          return id;
        }

        // it is required to resize all vectors
        id = indexToId_[data_.size()-1];
        indexToId_.push_back(id);        
        idToIndex_[id]++;

        data_.push_back(data_[data_.size()-1]);
        j=static_cast<int>(data_.size())-2;

        // now shift elements
        while (j>i) {
          data_[j]=data_[j-1];
          id = indexToId_[j] = indexToId_[j-1];
          idToIndex_[id]++;
          j--;
        };

        // now we can insert the new elements
        idToIndex_.push_back(i);
        nid=indexToId_[i]=static_cast<int>(idToIndex_.size())-1;
        data_[i]=std::make_pair(key,d);
      } else {
        // there should be a free element on the right side.  So we just need
        // to shift until this element is reached!
        
        // find the removed element
        j=i;
        while ((j<static_cast<int>(data_.size())) &&
               (data_[j].first != invalid_)) {
          ++j;
        }

        if (j==static_cast<int>(data_.size())) {
          // hmmm, we shouldn't get here at all! but we just insert the
          // element at the end.

          _lti_debug4("WARNING!! We should not get here at all!\n");

          data_.push_back(std::make_pair(key,d));
          // new id will be the new index in the data vector!
          id = static_cast<int>(data_.size())-1;
          indexToId_.push_back(id);
          idToIndex_.push_back(id);
          numElements_++;

#       if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
          checkConsistency();
#       endif    

          return id;
        }

        // reuse the freed id:
        nid = indexToId_[j];

        while (j>i) {
          // shift the elements
          data_[j]=data_[j-1];
          id = indexToId_[j] = indexToId_[j-1];
          idToIndex_[id]++;
          j--;
        }

        // now we can insert the new elements
        idToIndex_[nid]=i;
        indexToId_[i]=nid;
        data_[i]=std::make_pair(key,d);
        numRemoved_--; // one removed element has been reused!    
      }
    }

    numElements_++;

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
    checkConsistency();
#endif    

    return nid;
  }
  
  template<class T,class U>
  void priorityQueue<T,U>::erase(const index_type id) {

    if (id<0) {
      return;
    }

    int i = idToIndex_[id];

    _lti_debug4("PQ::erase id " << id << ", idx " << i 
                << ", key=" << data[i].first
                << ", data=" << data[i].second << std::endl);

    if (i==stackPtr_) { // deleting top of the queue requires updating the
                        // stack pointer

      // make sure the stackPtr points to a valid element
      stackPtr_++;
      while ((stackPtr_ < static_cast<int>(data_.size())) && 
             (data_[stackPtr_].first == invalid_)) {
        ++stackPtr_;
      }
      
      if (stackPtr_ == static_cast<int>(data_.size())) {
        // oh! it seems the whole thing is empty! so we clear it completelly
        clear();
        return;
      }
    }

    if (data_[i].first != invalid_) {
      data_[i].first = invalid_;
      numElements_--;
      numRemoved_++;
    }

  }

  template<class T,class U>
  bool priorityQueue<T,U>::checkConsistency() const {

    bool res = true;
    if ((numElements_+numRemoved_) != static_cast<int>(data_.size())) {
      res = false;
    }

    if (stackPtr_ > numRemoved_) {
      res = false;
    }

    // check idToIndex indexToId consistency:
    int i,id;
    for (i=0;i<static_cast<int>(indexToId_.size());++i) {
      id = indexToId_[i];
      if (idToIndex_[id] != i) {
        res = false;
      }
    }

    // check queue consistency (data must be sorted!)
    T last=data_[stackPtr_].first;
    for (i=stackPtr_;i<static_cast<int>(data_.size());++i) {
      if (data_[i].first != invalid_) {
        if (data_[i].first<last) {
          _lti_debug2("Priority queue inconsistency at idx " << i << 
                      " last value: " << last <<
                      " this value: " << data[i].first << 
                      " stackPtr: " << stackPtr << std::endl);
          last = data_[i].first;
          res=false;
        }
      }
    }

    return res;
  }

  template<class T,class U>
  void priorityQueue<T,U>::update(const index_type id,
                                  const T& newKey) {

    int idx = idToIndex_[id];
    int j,i(idx);

    _lti_debug4("PQ::update id " << id << ", new key " << newKey << 
                " (old key " << data[idx].first << ")" << 
                " idx: " << idx << " -> ");

    assert(data_[idx].first != invalid_);

    if (data_[idx].first < newKey) { // new key is greater than old one
      // try to shift to the right
      std::pair<T,U> elem(newKey,data_[i].second); // remember the old data

      const int last = static_cast<int>(data_.size())-1;
      while ((i<last) && ((data_[i+1].first == invalid_) || 
                          (data_[i+1].first < newKey))) {
        data_[i]=data_[i+1];
        j=indexToId_[i]=indexToId_[i+1];
        idToIndex_[j]--;
        ++i;
      } 

      data_[i]=elem;
      idToIndex_[id]=i;
      indexToId_[i]=id;

      // the stack pointer may require some updating
      while (data_[stackPtr_].first == invalid_) {
        stackPtr_++;
      }

    } else if (newKey < data_[idx].first) {
      // try to shift to the left
      std::pair<T,U> elem(newKey,data_[i].second); // remember the old data

      while ((i>stackPtr_) && ((data_[i-1].first == invalid_) || 
                               (newKey<data_[i-1].first))) {

        data_[i]=data_[i-1];
        j=indexToId_[i]=indexToId_[i-1];
        idToIndex_[j]++;        
        --i;
      }

      data_[i]=elem;
      idToIndex_[id]=i;
      indexToId_[i]=id;
    }

    _lti_debug4(i << std::endl);

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
    checkConsistency();
#endif
  }

  template<class T,class U>
  void priorityQueue<T,U>::update(const index_type id,
                                  const T& newKey,
                                  const U& dat) {

    update(id,newKey);
    data_[idToIndex_[id]].second = dat;
  }

  template<class T,class U>
  bool priorityQueue<T,U>::write(ioHandler& handler,
                                 const bool complete) const {

    std::vector< std::pair<T,U> > d;
    getData(d);

    return lti::write(handler,d,complete);
  }

  template<class T,class U>
  bool priorityQueue<T,U>::read(ioHandler& handler,
                                const bool complete) {
    std::vector< std::pair<T,U> > d;
    if (lti::read(handler,d,complete)) {
      create(d);
      return true;
    }
    return false;
  }


  template<class T,class U>
  bool priorityQueue<T,U>::writeAll(ioHandler& handler,
                                    const bool complete) const {

    if (complete) {
      handler.writeBegin();
    }

    lti::write(handler,"invalid",invalid_);
    lti::write(handler,"data",data_);
    lti::write(handler,"idToIndex",idToIndex_);
    lti::write(handler,"indexToId",indexToId_);
    lti::write(handler,"numElements",numElements_);
    lti::write(handler,"numRemoved",numRemoved_);
    lti::write(handler,"stackPtr",stackPtr_);

    if (complete) {
      handler.writeEnd();
    }

    return true;
  }

  template<class T,class U>
  bool priorityQueue<T,U>::readAll(ioHandler& handler,
                                   const bool complete) {

    if (complete) {
      handler.readBegin();
    }

    lti::read(handler,"invalid",invalid_);
    lti::read(handler,"data",data_);
    lti::read(handler,"idToIndex",idToIndex_);
    lti::read(handler,"indexToId",indexToId_);
    lti::read(handler,"numElements",numElements_);
    lti::read(handler,"numRemoved",numRemoved_);
    lti::read(handler,"stackPtr",stackPtr_);

    if (complete) {
      handler.readEnd();
    }

    return true;
  }

  template<class T,class U>
  void priorityQueue<T,U>::create(const std::vector< std::pair<T,U> >& da) {
    
    // first than all, clear all old data
    clear();
    
    vector<T> lkey;
    vector<index_type> index;

    lkey.allocate(static_cast<int>(data_.size()));
    index.allocate(static_cast<int>(lkey.size()));
    index_type i;
    for (i=0;i<lkey.size();++i) {
      lkey.at(i)=data_[i].first;
      index.at(i)=i;
    }
  
    sort2 sorter;
    sorter.apply(lkey,index);

    numElements_ = lkey.size();
    numRemoved_ = 0;
    stackPtr_ = 0;

    idToIndex_.resize(lkey.size());
    indexToId_.resize(lkey.size());
    data_.resize(lkey.size());

    for (i=0;i<lkey.size();++i) {
      data_[i].first          = lkey.at(i);
      data_[i].second         = da[index.at(i)].second;
      indexToId_[i]           = index.at(i);
      idToIndex_[index.at(i)] = i;
    }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
    _lti_debug4("Creation consistency ");
    if (checkConsistency()) {
      _lti_debug4("ok\n");
    } else {
      _lti_debug4("failed\n");
    }
#endif    
  }

  template<class T,class U>
  void priorityQueue<T,U>::create(const std::vector<T>& key,
                                  const std::vector<U>& da) {

    assert(key.size() == da.size());

    _lti_debug("PQC0\n");

    // first than all, clear all old data
    clear();
    
    _lti_debug("PQC1\n");

    vector<T> lkey;
    vector<int> index;
    lkey.allocate(static_cast<int>(key.size())); 
    index.allocate(lkey.size());
    int i;
    for (i=0;i<index.size();++i) {
      lkey.at(i)=key[i];
      index.at(i)=i;
    }

    _lti_debug("PQC2\n");

    sort2 sorter;
    sorter.apply(lkey,index);

    _lti_debug("PQC3\n");

    numElements_ = lkey.size();
    numRemoved_ = 0;
    stackPtr_ = 0;

    idToIndex_.resize(lkey.size());
    indexToId_.resize(lkey.size());
    data_.resize(lkey.size());

    _lti_debug("PQC4\n");

    for (i=0;i<lkey.size();++i) {
      data_[i].first          = lkey.at(i);
      data_[i].second         = da[index.at(i)];
      indexToId_[i]           = index.at(i);
      idToIndex_[index.at(i)] = i;
    }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 2)
    _lti_debug("Creation consistency ");
    if (checkConsistency()) {
      _lti_debug("ok\n");
    } else {
      _lti_debug("failed\n");
    }
#endif    
    
  }

}
