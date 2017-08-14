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
 * \file   ltiGenericVector.cpp
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiGenericVector_template.h,v 1.19 2008-08-06 03:40:58 alvarado Exp $
 */

// This allows to deactivate the instantiation of the generic vector
#ifndef _LTI_GENERIC_VECTOR_DONT_INSTANTIATE_REQUEST

#ifndef _LTI_GENERIC_VECTOR_TEMPLATE_H_
#define _LTI_GENERIC_VECTOR_TEMPLATE_H_

#include "ltiMath.h" // for min(T,T)
#include <sstream> // defines operator<< for const char* e.g. s << "(";
#include <cstring> // for memcpy

#include "ltiRGBAPixel.h" //needed for write() member function with ICC
#include "ltiPoint.h"//needed for write() member function with ICC

namespace lti {
  // implementation of template functions



  // -------------------------------------------------------------------
  // genericVector
  // -------------------------------------------------------------------

  // default constructor
  template<typename T>
  genericVector<T>::genericVector()
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {
  }

  template<typename T>
  genericVector<T>::genericVector(const int theSize)
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {

    if (theSize <= 0) {
      return;
    }

    vectorSize_ = theSize;
    idxLastElement_ = theSize-1;
    ownData_ = true;

    theElements_ = new T[vectorSize_];

    if (theElements_ == 0) { // allocation check
      throw allocException();
    }
  }

  template<typename T>
  genericVector<T>::genericVector(const int theSize,const T& iniValue)
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {

    if (theSize <= 0) {
      return;
    }

    vectorSize_ = theSize;
    idxLastElement_ = theSize-1;
    ownData_ = true;

    theElements_ = new T[vectorSize_];

    if (theElements_ == 0) { // allocation check
      throw allocException();
    }

    fill(iniValue);
  }

  template<typename T>
  genericVector<T>::genericVector(const int theSize,const T* _data)
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {

    if (theSize <= 0) {
      return;
    }

    vectorSize_ = theSize;
    idxLastElement_ = theSize-1;
    ownData_ = true;

    theElements_ = new T[vectorSize_];

    if (theElements_ == 0) { // allocation check
      throw allocException();
    }

    fill(_data);
  }


  template<typename T>
  genericVector<T>::genericVector(const int theSize,
                                  T* _data,
                                  const eConstantReference constRef)
    : vectorSize_(theSize),idxLastElement_(theSize-1),
      theElements_(_data),ownData_(false),constReference_(constRef) {

    if (theSize <= 0) {
      vectorSize_ = 0;
      idxLastElement_ = -1;
      theElements_ = 0;
      ownData_ = true;
      constReference_ = VariableReference;
      return;
    }

  }

  // copy constructor
  template<typename T>
  genericVector<T>::genericVector(const genericVector<T>& other)
    : container(),vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {

    if (other.vectorSize_ <= 0) {
      return;
    }

    vectorSize_ = other.vectorSize_;
    idxLastElement_ = vectorSize_-1; // ensure consistent data!
    ownData_ = true;

    theElements_ = new T[vectorSize_];

    if (theElements_ == 0) { // allocation check
      throw allocException();
    }

    // copy contents of other genericVector
    memcpy(theElements_,other.theElements_,vectorSize_*sizeof(T));    
  }

  // copy constructor
  template<typename T>
  genericVector<T>::genericVector(const genericVector<T>& other,
                                  const int from, 
                                  const int to)
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {
    copy(other, from, to);
  }

  // copy constructor
  template<typename T>
  genericVector<T>::genericVector(const genericVector<T>& other,
                                  const genericVector<int>& idx)
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {
    copy(other, idx);
  }

  // pseudo-copy constructor
  template<typename T>
  genericVector<T>::genericVector(const std::vector<T>& other)
    : vectorSize_(0),idxLastElement_(-1),
      theElements_(0),ownData_(true),constReference_(VariableReference) {

    if (other.size() <= 0) {
      return;
    }

    vectorSize_ = static_cast<size_type>(other.size());
    idxLastElement_ = vectorSize_-1;
    ownData_ = true;

    theElements_ = new T[vectorSize_];

    if (theElements_ == 0) { // allocation check
      throw allocException();
    }

    // copy contents of other genericVector
    typename std::vector<T>::const_iterator it;
    typename lti::genericVector<T>::iterator dit;

    for (it=other.begin(),dit=begin();it!=other.end();++it,++dit) {
      *dit = *it;
    }
  }

  template<typename T>
  genericVector<T>::~genericVector() {
    if (ownData_) {
      delete[] theElements_;
    }
    theElements_ = 0;
    vectorSize_ = 0;
    idxLastElement_ = -1;
  }

  // restore ownership
  template<typename T>
  void genericVector<T>::restoreOwnership() {
    if (ownData_) {
      return; // already my data!
    }

    int newSize = vectorSize_;

    if (constReference_==ConstantReference) {
      // it is now allowed to change the ownership of a constant vector,
      // as the pointer would change, and that's exactly what the constant
      // stuff means!
      throw constReferenceException();
    }

    T* newElements = new T[newSize];

    if (newElements == 0) { // check memory allocation
      throw allocException();
    }

    if (newSize>0) {
      memcpy(newElements,theElements_,newSize*sizeof(T));
    }

    vectorSize_ = newSize;
    idxLastElement_ = newSize-1;

    ownData_ = true;

    theElements_ = newElements;
  }

  template<typename T>
  void genericVector<T>::useExternData(const int theSize,
                                       T* _data,
                                       const eConstantReference constRef) {
    if (constReference_==ConstantReference) {
      // not allowed!
      throw constReferenceException();
    }

    vectorSize_ = theSize;
    idxLastElement_ = theSize-1;

    if (ownData_) {
      delete[] theElements_;
    }

    theElements_ = _data;
    ownData_ = false; // do not delete data!
    constReference_ = constRef;
  }

  // attach external memory region to this object
  template <typename T>
  void genericVector<T>::attach(const int theSize, T* _data) {
    useExternData(theSize,_data);
    ownData_ = true;
  }

  // detach internal memory to an external 'receiver' genericVector
  template <typename T>
  void genericVector<T>::detach(genericVector<T>& receiver) {
    // receiver should know whether he owns data, so it is not
    // accidentally deleted
    if (ownData_) {
      receiver.attach(vectorSize_,theElements_);
    } else {
      receiver.useExternData(vectorSize_,theElements_);
    }
    ownData_ = false;
    allocate(0);
  }

  // detach internal memory to an external 'receiver' genericVector
  template <typename T>
  void genericVector<T>::swap(genericVector<T>& other) {

    if ((constReference_==ConstantReference) ||
        (other.constReference_==ConstantReference)) {
      throw constReferenceException();
    }

    bool tmpBool;
    T* tmpTPtr;
    int tmpInt;

    if (vectorSize_ != other.vectorSize_) {
      tmpInt = vectorSize_;
      vectorSize_ = other.vectorSize_;
      other.vectorSize_ = tmpInt;

      tmpInt = idxLastElement_;
      idxLastElement_ = other.idxLastElement_;
      other.idxLastElement_ = tmpInt;
    }

    tmpTPtr = theElements_;
    theElements_ = other.theElements_;
    other.theElements_ = tmpTPtr;

    tmpBool = ownData_;
    ownData_ = other.ownData_;
    other.ownData_ = tmpBool;
  }

  template<typename T>
  void genericVector<T>::resize(const int newSize,
                                const T& iniValue,
                                const eResizeType resizeType) {

    if (newSize == vectorSize_) {
      if (resizeType==Init) {
        fill(iniValue,0,idxLastElement_);
      }
      return;
    }

    if (constReference_==ConstantReference) {
      throw constReferenceException();
    }

    int from = 0;
    T* newElements = 0;

    if (newSize <= 0) {
      if (ownData_) {
        delete[] theElements_;
      } else {
        ownData_ = true;
      }

      theElements_ = 0;
      vectorSize_ = 0;
      idxLastElement_ = -1;

      return; // ready!!!
    }

    newElements = new T[newSize];

    if (newElements == 0) { // check memory allocation
      throw allocException();
    }

    if (resizeType==Copy || resizeType==CopyAndInit) {
      // old data should be copied
      const int top = min(newSize,size());
      if (top>0) {
        memcpy(newElements,theElements_,top*sizeof(T));
      }
      from = top;
    }

    vectorSize_ = newSize;
    idxLastElement_ = newSize-1;

    if (ownData_) {
      delete[] theElements_;
    } else {
      ownData_ = true;
    }

    theElements_ = newElements;

    if (resizeType==Init || resizeType==CopyAndInit) {
      fill(iniValue,from,idxLastElement_);
    }
  }

  template<typename T>
  void genericVector<T>::clear() {
    if (constReference_==ConstantReference) {
      throw constReferenceException();
    }

    if (ownData_) {
      delete[] theElements_;
    } else {
      ownData_ = true;
    }

    theElements_ = 0;
    vectorSize_ = 0;
    idxLastElement_ = -1;
  }

  // This could be implemented faster for small vectors by using a
  // simple for-loop to fill the memory. E.g. for
  // genericVector<double> it's faster when size is below
  // three. However, the threshold depends on the actual type,
  // regarding size and alignment. Also for such vectors it takes
  // around one microsecond (yes, micro) to fill. Thus, it doesn't
  // make sense to introduce the necessary code complexity.
  template<typename T>
  void genericVector<T>::fill(const T& _data,const int f, const int t) {
    int from,to,ready,wr,rest;
    from  = f;
    to    = t;

    // check bounds
    if (to >= size()) {
      to = idxLastElement_;
    }

    if (from < 0) {
      from = 0;
    }

    if ((from >= size()) || (to < from)) { // nothing to do
      return;
    }

    T* ptrData;
    ptrData = &theElements_[from]; // ptrData points to the first
    ready = 1;                    // element to be "filled"
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
  void genericVector<T>::fill(const T _data[],const int f, const int t) {
    int from,to;
    from  = f;
    to    = t;

    // check bounds
    if (to >= vectorSize_) {
      to = idxLastElement_;
    }

    if (from < 0) {
      from = 0;
    }

    if ((from >= vectorSize_) || (to < from)) { // nothing to do
      return;
    }

    memcpy(&theElements_[from],_data,(to-from+1)*sizeof(T));
  }

  template<typename T>
  void genericVector<T>::fill(const genericVector<T>& vct,
                              const int from,
                              const int to,
                              const int startAt) {
    int f,t;

    f = from;
    t = to;

    if (f < 0) {
      f = 0;
    }
    if (t > idxLastElement_) {
      t = idxLastElement_;
    }

    if ((f > t) || (startAt > vct.idxLastElement_)) {
      return;
    }

    const int numElem = min(t-f+1,vct.idxLastElement_-startAt+1);

    if (numElem>0) {
      memcpy(&at(f),&vct.at(startAt),numElem*sizeof(T));
    }
  }


  // copy member
  template<typename T>
  genericVector<T>& genericVector<T>::copy(const genericVector& other) {

    if (&other == this) {
      return (*this); // copy of itself is ready!
    }

    if (constReference_==ConstantReference) {
      // reference is constant => only the contents will be copied
      fill(other);
    } else {
      if (ownData_) {
        if (vectorSize_ != other.size()) {
          delete[] theElements_;
          theElements_ = 0;
        }
      } else {
        ownData_ = true;
        theElements_ = 0;
      }

      vectorSize_ = other.vectorSize_;
      idxLastElement_ = vectorSize_-1;

      if (vectorSize_ > 0) {
        if (isNull(theElements_)) {
          theElements_ = new T[vectorSize_];

          if (isNull(theElements_)) { // memory allocation check
              throw allocException();
          }
        }

        memcpy(theElements_,other.data(),sizeof(T)*vectorSize_);
      } else {
        theElements_ = 0;
        vectorSize_ = 0;
        idxLastElement_ = -1;
      }
    }

    return *this;
  }

  // copy member
  template<typename T>
  genericVector<T>& genericVector<T>::copy(const genericVector<T>& other,
                                           const int from, const int to) {

    int f=from;
    int t=to;

    // check bounds
    const int osize=other.size();
    if (f<0) {
      f=0;
    }
    if (t>=osize) {
      t=other.lastIdx();
    }

    const int nsize = t-f+1;

    if (&other == this) { // same genericVector!      
      genericVector<T> tmpCopy(other,from,to); //temporal copy of this
      swap(tmpCopy); //extract subgenericVector
      return (*this);  //return itself
    }

    //check for complete copy
    if (f==0 && t==other.lastIdx()) {
      return copy(other);
    }

    if (constReference_==ConstantReference) {
      if  (vectorSize_ != nsize) {
        throw constReferenceException();
      }
    } else {
      if (ownData_) {
        if (vectorSize_ != nsize) {
          delete[] theElements_;
          theElements_ = 0;
        }
      } else {
        ownData_ = true;
        theElements_ = 0;
      }
    }

    vectorSize_ = nsize;
    idxLastElement_ = vectorSize_-1;

    if (vectorSize_ > 0) {
      if (isNull(theElements_)) {
        theElements_ = new T[vectorSize_];

        if (isNull(theElements_)) { // memory allocation check
          throw allocException();
        }
      }

      memcpy(theElements_,&other.at(f),sizeof(T)*vectorSize_);

    } else {
      theElements_ = 0;
      vectorSize_ = 0;
      idxLastElement_ = -1;
    }

    return *this;

  }

  // copy member
  template<typename T>
  genericVector<T>& genericVector<T>::copy(const genericVector& other,
                                           const genericVector<int>& idx) {

    if (&other == this) {              // same genericVector!
      genericVector<T> tmpCopy(*this); // temporal copy of this
      copy(tmpCopy, idx);              // extract subgenericVector
      return (*this);                  // return itself
    }

    if (constReference_==ConstantReference) {
      if  (vectorSize_ != idx.size()) {
        throw constReferenceException();
      }
    } else {
      if (ownData_) {
        if (vectorSize_ != idx.size()) {
          delete[] theElements_;
          theElements_ = 0;
        }
      } else {
        ownData_ = true;
        theElements_ = 0;
      }
    }

    vectorSize_ = idx.size();
    idxLastElement_ = vectorSize_-1;

    if (vectorSize_ > 0) {
      if (isNull(theElements_)) {
        theElements_ = new T[vectorSize_];

        if (isNull(theElements_)) { // memory allocation check
          throw allocException();
        }
      }
      typename genericVector<T>::iterator it=begin();
      typename genericVector<T>::iterator eit=end();
      genericVector<int>::const_iterator idxIt=idx.begin();
      for (;it!=eit; ++it, ++idxIt) {
        (*it)=other.at(*idxIt);
      }

    } else {
      theElements_ = 0;
      vectorSize_ = 0;
      idxLastElement_ = -1;
    }

    return *this;
  }

  // castfrom
  template<typename T>
  template<class U>
  genericVector<T>& genericVector<T>::castFrom(const genericVector<U>& other) {
    typename genericVector<U>::const_iterator it,eit;
    typename genericVector<T>::iterator dit;
    
    allocate(other.size());
    
    for (it=other.begin(),dit=begin(),eit=other.end();
         it!=eit;++it,++dit) {
      *dit = static_cast<T>(*it);
    }
    
    return *this;
  }

  template<typename T>
  genericVector<T>& genericVector<T>::castFrom(const genericVector<T>& other) {
    return copy(other);
  }

  // castfrom
  template<typename T>
  template<class U>
  genericVector<T>& genericVector<T>::castFrom(const genericVector<U>& other,
                                               const int from, const int to) {
    
    const int f= (from>0) ? from : 0;
    const int t= (to<other.size()) ? to : other.lastIdx();
    const int nsize = t-f+1; // compute the final vector size

    if (nsize != vectorSize_) {
      allocate(nsize);
    }

    typename genericVector<U>::const_iterator it;
    typename genericVector<T>::iterator dit,eit;
    
    for (it=other.begin()+f,dit=begin(),eit=end();
         dit!=eit;
         ++dit,++it) {
      (*dit)=static_cast<T>(*it);
    }

    return *this;
  }

  template<typename T>
  genericVector<T>& genericVector<T>::castFrom(const genericVector<T>& other,
                                               const int from, const int to) {
    return copy(other, from, to);
  }

  // castfrom
  template<typename T>
  template<class U>
  genericVector<T>& genericVector<T>::castFrom(const std::vector<U>& other) {
    typename std::vector<U>::const_iterator it,eit;
    typename genericVector<T>::iterator dit;
    
    allocate(static_cast<int>(other.size()));
    
    for (it=other.begin(),dit=begin(),eit=other.end();
         it!=eit;++it,++dit) {
      *dit = static_cast<T>(*it);
    }
    
    return *this;
  }

  // creates a clone of this genericVector
  template<typename T>
  const std::string& genericVector<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // creates a clone of this genericVector
  template<typename T>
  genericVector<T>* genericVector<T>::clone() const {
    return new genericVector<T>(*this);
  }

  template<typename T>
  genericVector<T>* genericVector<T>::newInstance() const {
    return new genericVector<T>();
  }

  /* compare this genericVector with other
     @param other the other genericVector to be compared with
     @return true if both genericVectors have the same elements and same size
  */
  template<typename T>
  bool genericVector<T>::equals(const genericVector<T>& other) const {
    bool result;

    result = (size() == other.size());

    return (result &&
            (0 == memcmp(theElements_,other.theElements_,size()*sizeof(T))));

  }

  // applies a C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(T (*function)(T)) {
    iterator i,e;

    e = end();
    for (i=begin();i!=e;++i)
      *i=(*function)(*i);

    return (*this);
  }

  // applies a C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                                            T (*function)(T)) {

    if (size() != other.size()) {
      allocate(other.size());
    }

    const_iterator io,e;
    iterator i;

    e = other.end();
    for (i=begin(),io=other.begin();io!=e;++i,++io)
      *i=(*function)(*io);

    return (*this);
  }



  // applies a two-parameter C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                                            T (*function)(const T&,const T&)) {

    assert (other.size() == size());

    iterator i1,e;
    const_iterator i2;

    e = end();
    for (i1=begin(),i2=other.begin();i1!=e;i1++,i2++) {
      *i1=(*function)(*i1,*i2);  // applies a C-Function to each element
    }

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                                            T (*function)(T,T)) {

    assert (other.size() == size());

    iterator i1,e;
    const_iterator i2;

    e = end();
    for (i1=begin(),i2=other.begin();i1!=e;i1++,i2++) {
      *i1=(*function)(*i1,*i2);  // applies a C-Function to each element
    }

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& a,
                                            const genericVector<T>& b,
                                            T (*function)(const T&,const T&)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      allocate(a.size());
    }

    const_iterator ia,ib,e;
    iterator it;

    e = a.end();
    for (ia=a.begin(),ib=b.begin(),it=begin();ia!=e;++ia,++ib,++it) {
      *it=(*function)(*ia,*ib);  // applies a C-Function to each element
    }

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& a,
                                            const genericVector<T>& b,
                                            T (*function)(T,T)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      allocate(a.size());
    }

    const_iterator ia,ib,e;
    iterator it;

    e = a.end();
    for (ia=a.begin(),ib=b.begin(),it=begin();ia!=e;++ia,++ib,++it) {
      *it=(*function)(*ia,*ib);  // applies a C-Function to each element
    }

    return (*this);
  }


  // applies a C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(T (*function)(const T&)) {
    iterator i,e;

    e = end();
    for (i=begin();i!=e;++i)
      *i=(*function)(*i);  // applies a C-Function to each element


    return (*this);
  }

  // applies a C-Function to each element
  template<typename T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                                            T (*function)(const T&)) {
    if (size() != other.size()) {
      allocate(other.size());
    }

    const_iterator io,e;
    iterator i;

    e = other.end();
    for (i=begin(),io=other.begin();io!=e;++i,++io)
      *i=(*function)(*io);

    return (*this);
  }

  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /**
   * write the object in the given ioHandler
   */
  template<typename T>
  bool genericVector<T>::write(ioHandler& handler,const bool complete) const {

    int i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      handler.write("size",vectorSize_);

      handler.writeBegin();             // Data block scope (1)
                                        //
      handler.writeSymbol("data");      //
      handler.writeKeyValueSeparator(); //
                                        //
      handler.writeBegin();             // Data             (2)
      if (size() > 0) {                 //
        for (i=0;i<idxLastElement_;++i){//
          lti::write(handler,at(i));    //
          handler.writeDataSeparator(); //
        }                               //
        lti::write(handler,at(i));      //
      }                                 //
      handler.writeEnd();               // Data             (1)
                                        //
      handler.writeEnd();               // Data block scope (0)
    }                                   //

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template<typename T>
  bool genericVector<T>::read(ioHandler& handler,const bool complete) {


    int i,sz;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      handler.read("size",sz);
      std::string str;

      handler.readBegin();                   // Data scope (1)
      handler.readSymbol(str);               //
                                             //
      if (str == "data") {                   //
        handler.readKeyValueSeparator();     //
        handler.readBegin();                 // Data       (2)
        allocate(sz);                        //
        if (sz > 0) {                        //
          sz--;                              //
          for (i=0;i<sz;++i) {               // 
            lti::read(handler,at(i));        //
            handler.readKeyValueSeparator(); //
          }                                  //
          lti::read(handler,at(i));          //
        }                                    //
        handler.readEnd();                   // Data       (1)
      }                                      //
                                             //
      handler.readEnd();                     // Data scope (0)
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;

  }

} // namespace lti


namespace std {
  // undocumented class required by the next operator in order to display
  // ubyte and byte as numbers and not as characters.
  template<typename T>
  class numeric_type {
  public:
    typedef T value_type;
  };

  template<>
  class numeric_type<lti::ubyte> {
  public:
    typedef int value_type;
  };

  template<>
  class numeric_type<lti::byte> {
  public:
    typedef int value_type;
  };

  /// ASCII output of a genericVector to a std stream.
  template <typename T>
  ostream& operator<<(ostream& s,const lti::genericVector<T>& v) {
    int i;

    s << "(";

    if (v.size()>0) {
      s << static_cast< typename numeric_type<T>::value_type>(v.at(0));

      for (i=1;i<v.size();++i) {
        s << " \t" 
          << static_cast<typename numeric_type<T>::value_type>(v.at(i));
      }

      s << ")";
    } else {
      s << " )";
    }

    return s;
  }
}

#endif
#endif
