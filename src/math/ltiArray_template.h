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
 * \file   ltiArray_template.h
 *         Contains the template class array.
 *
 * \author Pablo Alvarado
 * \date   09.04.99
 *
 * revisions ..: $Id: ltiArray_template.h,v 1.18 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiArray.h"
#include "ltiMath.h" //for min() max()
#include <cstring> // for memcpy

namespace lti {

  // --------------------------------------------------
  // array
  // --------------------------------------------------

  // default constructor
  template<typename T>
  array<T>::array()
    : vector<T>(),offset_(0),
      firstArrayElement_(0), lastArrayElement_(-1), 
      theElement0_(0) {
  }

  template<typename T>
  array<T>::array(const int from,
                  const int to,
                  const T& iniValue)
    : vector<T>(to-from+1,iniValue) {

    offset_ = -from; /* offset represent the absolute index of
                       the 0-th element.*/

    firstArrayElement_ = from;
    lastArrayElement_  = this->size()-offset_-1;
    if (notNull(this->theElements_)) {
      theElement0_=this->theElements_ + offset_; // address of the element at 0
    } else {
      theElement0_=0;
    }
  }  


  // copy constructor
  template<typename T>
  array<T>::array(const vector<T>& other,const int theOffset)
    : vector<T>(other), 
      offset_(theOffset),
      firstArrayElement_(-theOffset),
      lastArrayElement_(this->vectorSize_-theOffset-1),
      theElement0_(notNull(this->theElements_) ?
                   this->theElements_ + offset_ : 0) {
  }


  template<typename T>
  array<T>::array(const int from, const int to)
    : vector<T>(to-from+1),
      offset_(-from), // offset represent the absolute index of
                      // the 0-th element
      firstArrayElement_(from),
      lastArrayElement_(to),
      // address of the element at 0
      theElement0_(notNull(this->theElements_) ?
                   this->theElements_ + offset_ : 0) { 
  }

  // copy constructor
  template<typename T>
  array<T>::array(const array<T>& other)
    : vector<T>(other),
      offset_(other.offset_),
      firstArrayElement_(other.firstArrayElement_),
      lastArrayElement_(other.lastArrayElement_),
      // address of the element at 0
      theElement0_(notNull(this->theElements_) ? 
                   this->theElements_ + offset_ : 0) { 
  }
  
  // destructor
  template<typename T>
  array<T>::~array() {
    // The following outcommented code is performed by the vector destructor
    // if (this->ownData_) {
    //   delete[] this->theElements_;
    // }
    // this->theElements_ = 0;
    // this->vectorSize_ = 0;
    // this->idxLastElement_ = -1;
    offset_ = 0;
    firstArrayElement_ = 0;
    lastArrayElement_ = -1;
    theElement0_ = 0;
  }


  template<typename T>
  void array<T>::useExternData(const int from,
                               const int to,
                               T* _data,
                               const eConstantReference constRef) {
    vector<T>::useExternData(to-from+1,_data,constRef);
    offset_ = -from;
    firstArrayElement_ = from;
    lastArrayElement_  = to;
    
    theElement0_ = (notNull(this->theElements_) ?
                    this->theElements_ + offset_ : 0);
  }

  template<typename T>
  void array<T>::attach(const int from,
                        const int to,
                        T* _data) {
    vector<T>::attach(to-from+1,_data);

    offset_ = -from;
    firstArrayElement_ = from;
    lastArrayElement_  = to;
    
    theElement0_ = (notNull(this->theElements_) ?
                    this->theElements_ + offset_ : 0);
  }

  template<typename T>
  void array<T>::detach(array<T>& receiver) {
    // receiver should know whether he owns data, so it is not
    // accidentally deleted
    if (this->ownData_) {
      receiver.attach(firstArrayElement_,lastArrayElement_,
                      this->theElements_);
    } else {
      receiver.useExternData(firstArrayElement_,lastArrayElement_,
                             this->theElements_);
    }
    this->ownData_ = false;
    allocate(0,0);
  }

  template<typename T>
  void array<T>::swap(array<T>& other) {
    vector<T>::swap(other);
    lti::swap(offset_,other.offset_);
    lti::swap(firstArrayElement_,other.firstArrayElement_);
    lti::swap(lastArrayElement_,other.lastArrayElement_);
    lti::swap(theElement0_,other.theElement0_);
  }


  // copy member
  template<typename T>
  array<T>& 
  array<T>::copy(const vector<T>& other) {
    vector<T>::copy(other);

    offset_ = 0;
    firstArrayElement_ = 0;
    lastArrayElement_  = this->size()-1;
    theElement0_ = this->theElements_;

    return (*this);
  }

  template<typename T>
  array<T>& array<T>::copy(const array<T>& other) {

    vector<T>::copy(other);

    offset_ = other.offset_;
    firstArrayElement_ = other.firstArrayElement_;
    lastArrayElement_  = other.lastArrayElement_;
    theElement0_ = (notNull(this->theElements_) ? 
                    this->theElements_ + offset_ : 0);
    

    return (*this);
  }
  // alias for copy member
  template<typename T>
  array<T>&
  array<T>::operator=(const vector<T>& other) {
    return (copy(other));
  }

  // alias for copy member
  template<typename T>
  array<T>&
  array<T>::operator=(const array<T>& other) {
    return (copy(other));
  }

  // name  member
  template<typename T>
  const std::string& array<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template<typename T>
  array<T>* array<T>::clone() const {
    return new array<T>(*this);
  }

  // newInstance member
  template<typename T>
  array<T>* array<T>::newInstance() const {
    return new array<T>();
  }

  template<typename T>
  void array<T>::setFirstIdx(const int newIdx) {
    offset_ = -newIdx;
    firstArrayElement_ = newIdx;
    lastArrayElement_ = firstArrayElement_+this->size()-1;
    theElement0_ = (notNull(this->theElements_) ? 
                    this->theElements_ + offset_ : 0 );
  }

  template<typename T>
  void array<T>::resize(const int from,
                        const int to,
                        const T& iniValue,
                        const eResizeType resizeType) {

    int oldFrom = firstIdx();
    int oldTo   = lastIdx();
    int newSize = to-from+1;

    // if the size is unchanged we're done quickly
    if ( newSize == this->vectorSize_ ) {
      // if data is not copied fill the array
      if (resizeType==Init) {
        fill(iniValue,oldFrom,oldTo);
      }
      // if starting point is different set
      // indices and offset_ correctly
      if (from != oldFrom) {
        offset_ = -from;
        firstArrayElement_ = from;
        lastArrayElement_ = to;
        theElement0_ = (notNull(this->theElements_) ?
                        this->theElements_ + offset_ : 0);
      }
      return;
    }

    T* newElements = 0;
    int idxLow,idxHigh;

    if (newSize <= 0) {
      // Wrong size! Assume this->size()==0
      if (this->ownData_) {
	delete[] this->theElements_;
      } else {
	this->ownData_ = true;
      }

      this->theElements_ = 0;
      this->vectorSize_ = 0;
      this->idxLastElement_ = -1;
      offset_ = 0;
      firstArrayElement_ = 0;
      lastArrayElement_ = -1;
      theElement0_ = 0;

      return; // nothing to do!
    }

    idxLow = to+1;
    idxHigh = to-1;

    newElements = new T[newSize];

    if (newElements == 0) { // check memory allocation
      throw allocException();
    }

    if (((resizeType==Copy)||(resizeType==CopyAndInit))
        && (oldTo>=oldFrom)) { // old data should be copied
      int n;
      // calculate the (0-based) indexes of the actual memory
      // which must be copied...
      idxLow  = max(oldFrom,from);
      idxHigh = min(oldTo,to);
      n = idxHigh - idxLow + 1;

      memcpy(&newElements[idxLow-from],
	     &this->theElements_[idxLow+offset_],n*sizeof(T));
    }
    

    if (this->ownData_) {
      delete[] this->theElements_;
    } else {
      this->ownData_ = true;
    }

    this->theElements_ = newElements;
    this->vectorSize_ = newSize;
    this->idxLastElement_ = newSize-1;
    offset_ = -from;
    firstArrayElement_ = from;
    lastArrayElement_ = to;
    theElement0_ = this->theElements_ + offset_;

    //init only new places
    if (resizeType==CopyAndInit) {
      fill(iniValue,from,idxLow-1);
      fill(iniValue,idxHigh+1,to);
    }
    // init all places
    if (resizeType==Init) {
      fill(iniValue,from,to);
    }
   

  }

  template<typename T>
  void array<T>::fill(const T& _data,const int f, const int t) {
    int from,to,ready,wr,rest;
    from  = f;
    to    = t;

    // check bounds
    if (to > lastIdx()) {
      to = lastIdx();
    }

    if (from < firstIdx()) {
      from = firstIdx();
    }

    if ((from > lastIdx()) || (to < from)) { // nothing to do
      return;
    }

    T* ptrData;
    ptrData = &this->theElements_[from+offset_]; // ptrData points to the first
    ready = 1;                           // element to be "filled"
    rest  = to-from;

    ptrData[0] = _data;

    // fills the region with memcpy
    while (rest > 0) {
      wr = min(ready,rest);
      memcpy(&ptrData[ready],ptrData,wr*sizeof(T));
      ready += wr;
      rest  -= wr;
    }
  }

  template<typename T>
  void array<T>::fill(const T _data[],const int f, const int t) {
    int from,to;
    from  = f;
    to    = t;

    // check bounds
    if (to >= lastArrayElement_) {
      to = lastArrayElement_;
    }

    if (from < firstArrayElement_) {
      from = firstArrayElement_;
    }

    if ((from >= lastArrayElement_) || (to < from)) { // nothing to do
      return;
    }

    T* ptrData;
    ptrData = &this->theElements_[from+offset_]; // ptrData points to the first
                                                 // element to be "filled"

    memcpy(ptrData,_data,(to-from+1)*sizeof(T));
  }



  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<class T>
  bool array<T>::write(ioHandler& handler,const bool complete) const {

    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"offset",-firstIdx());
      vector<T>::write(handler,false);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }

  /*
   * read the object from the given ioHandler
   */
  template<class T>
  bool array<T>::read(ioHandler& handler,const bool complete) {
    int offset;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"offset",offset);
      vector<T>::read(handler,false);
      setFirstIdx(-offset);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------



  // applies a C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(T (*function)(T)) {
    vector<T>::apply(*function);
    return (*this);
  }

  // applies a C-Function to each element
  template<class T>
  array<T>& array<T>::apply(T (*function)(const T&)) {
    vector<T>::apply(*function);
    return (*this);
  }

  // applies a C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(const array<T>& other,T (*function)(T)) {
    vector<T>::apply(other,*function);

    offset_ = other.offset_;
    firstArrayElement_ = other.firstArrayElement_;
    lastArrayElement_  = other.lastArrayElement_;
    theElement0_ = this->theElements_ + offset_;

    return (*this);
  }

  // applies a C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(const array<T>& other,T (*function)(const T&)) {
    vector<T>::apply(other,*function);

    offset_ = other.offset_;
    firstArrayElement_ = other.firstArrayElement_;
    lastArrayElement_  = other.lastArrayElement_;
    theElement0_ = this->theElements_ + offset_;

    return (*this);
  }


  // applies a two-parameter C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(const array<T>& other,
                            T (*function)(const T&,const T&)) {

    assert (other.size() == this->size());
    assert (firstArrayElement_ == other.firstArrayElement_);

    vector<T>::apply(other,*function);

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(const array<T>& other,
                            T (*function)(T,T)) {

    assert (other.size() == this->size());
    assert (firstArrayElement_ == other.firstArrayElement_);

    vector<T>::apply(other,*function);

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(const array<T>& a,
                            const array<T>& b,
                            T (*function)(const T&,const T&)) {

    assert (a.size() == b.size());
    assert (a.firstArrayElement_ == b.firstArrayElement_);
    
    vector<T>::apply(a,b,*function);

    offset_ = a.offset_;
    firstArrayElement_ = a.firstArrayElement_;
    lastArrayElement_  = a.lastArrayElement_;
    theElement0_ = this->theElements_ + offset_;

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  array<T>& array<T>::apply(const array<T>& a,
                            const array<T>& b,
                            T (*function)(T,T)) {

    assert (a.size() == b.size());
    assert (a.firstArrayElement_ == b.firstArrayElement_);
    
    vector<T>::apply(a,b,*function);

    offset_ = a.offset_;
    firstArrayElement_ = a.firstArrayElement_;
    lastArrayElement_  = a.lastArrayElement_;
    theElement0_ = this->theElements_ + offset_;

    return (*this);
  }

}
