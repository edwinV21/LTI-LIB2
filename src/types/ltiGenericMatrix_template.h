/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiGenericMatrix_template.h
 *         Contains the class lti::genericMatrix
 *         which implements the generic bidimensional container
 * 
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * revisions ..: $Id: ltiGenericMatrix_template.h,v 1.16 2012-09-16 04:58:34 alvarado Exp $
 */

// this allows to deactivate the instantiation of the generic matrix
#ifndef _LTI_GENERIC_MATRIX_DONT_INSTANTIATE_REQUEST

#ifndef _LTI_GENERIC_MATRIX_TEMPLATE_H_
#define _LTI_GENERIC_MATRIX_TEMPLATE_H_

namespace lti {

  // default constructor
  template <typename T>
  genericMatrix<T>::genericMatrix()
    : container(),theSize_(0,0),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(0),metaNumColumns_(0),totalSize_(0),
      ownData_(true),mode_(Connected),theElements_(0),rowAddressTable_(0) {    
  }

  // constructor: rows X cols genericMatrix
  template <typename T>
  genericMatrix<T>::genericMatrix(const int r, const int c)
    : container(),theSize_(c,r),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(r),metaNumColumns_(c),
      totalSize_(r*c),ownData_(true),mode_(Connected),theElements_(0),
      rowAddressTable_(0) {

    if((r<=0)||(c<=0)) {
      return;
    }

    int i;

    if ( int64(metaNumRows_*metaNumColumns_) != 
         int64(metaNumRows_)*int64(metaNumColumns_) ) {
      // memory allocation check
      throw allocException();
    }

    theElements_ = new T[metaNumRows_*metaNumColumns_];
    rowAddressTable_ = allocRows(metaNumRows_);

    if ((theElements_ == 0) || (rowAddressTable_ == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements_;
    for (i=0; i<metaNumRows_; ++i,tptr+=metaNumColumns_) {
      rowAddressTable_[i].useExternData(metaNumColumns_,
                                        tptr,
                                        ConstantReference);
    }
  }

  // constructor: rows X cols genericMatrix
  template <typename T>
  genericMatrix<T>::genericMatrix(const ipoint& sz)
    : container(),theSize_(sz),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(theSize_.y),metaNumColumns_(theSize_.x),
      totalSize_(theSize_.x*theSize_.y),ownData_(true),
      mode_(Connected),theElements_(0),
      rowAddressTable_(0) {

    if((sz.y<=0)||(sz.x<=0)) {
      return;
    }

    int i;

    if ( int64(metaNumRows_*metaNumColumns_) != 
         int64(metaNumRows_)*int64(metaNumColumns_) ) {
      // memory allocation check
      throw allocException();
    }

    theElements_ = new T[metaNumRows_*metaNumColumns_];
    rowAddressTable_ = allocRows(metaNumRows_);

    if ((theElements_ == 0) || (rowAddressTable_ == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements_;
    for (i=0; i<metaNumRows_; ++i,tptr+=metaNumColumns_) {
      rowAddressTable_[i].useExternData(metaNumColumns_,
                                        tptr,
                                        ConstantReference);
    }
  }

  // constructor: rows X cols genericMatrix
  template <typename T>
  genericMatrix<T>::genericMatrix(const int r, const int c, 
                                  const T& iniValue)
    : container(),theSize_(c,r),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(r),metaNumColumns_(c),
      totalSize_(r*c),ownData_(true),mode_(Connected),theElements_(0),
      rowAddressTable_(0) {

    if((r<=0)||(c<=0)) {
      return;
    }

    int i;

    if ( int64(metaNumRows_*metaNumColumns_) != 
         int64(metaNumRows_)*int64(metaNumColumns_) ) {
      // memory allocation check
      throw allocException();
    }

    theElements_ = new T[metaNumRows_*metaNumColumns_];
    rowAddressTable_ = allocRows(metaNumRows_);

    if ((theElements_ == 0) || (rowAddressTable_ == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements_;
    for (i=0; i<metaNumRows_; ++i,tptr+=metaNumColumns_) {
      rowAddressTable_[i].useExternData(metaNumColumns_,
                                        tptr,
                                        ConstantReference);
    }

    fill(iniValue);
  }

  // constructor: rows X cols genericMatrix
  template <typename T>
  genericMatrix<T>::genericMatrix(const int r, const int c,
                                  const T _data[])
    : container(),theSize_(c,r),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(r),metaNumColumns_(c),
      totalSize_(r*c),ownData_(true),mode_(Connected), theElements_(0),
      rowAddressTable_(0) {
    // theSize_.y and theSize_.x are REFERENCES to theSize_ attributes!

    if((r<=0)||(c<=0)) {
      return;
    }

    int i;

    if ( int64(metaNumRows_*metaNumColumns_) != 
         int64(metaNumRows_)*int64(metaNumColumns_) ) {
      // memory allocation check
      throw allocException();
    }

    theElements_ = new T[metaNumRows_*metaNumColumns_];
    rowAddressTable_ = allocRows(metaNumRows_);

    if ((theElements_ == 0) || (rowAddressTable_ == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements_;
    for (i = 0; i<metaNumRows_; ++i,tptr+=metaNumColumns_) {
      rowAddressTable_[i].useExternData(metaNumColumns_,
                                        tptr,
                                        ConstantReference);
    }

    fill(_data);
  }

  // constructor: rows X cols genericMatrix
  template <typename T>
  genericMatrix<T>::genericMatrix(const ipoint& dim, const T& iniValue)
    : container(),theSize_(dim),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(dim.y),metaNumColumns_(dim.x),totalSize_(dim.y*dim.x),
      ownData_(true),mode_(Connected), theElements_(0), rowAddressTable_(0) {

    if((dim.y<=0)||(dim.x<=0)) {
      return;
    }

    int i;

    if ( int64(metaNumRows_*metaNumColumns_) != 
         int64(metaNumRows_)*int64(metaNumColumns_) ) {
      // memory allocation check
      throw allocException();
    }

    theElements_ = new T[metaNumRows_*metaNumColumns_];
    rowAddressTable_ = allocRows(metaNumRows_);

    if ((theElements_ == 0) || (rowAddressTable_ == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements_;
    for (i = 0; i<metaNumRows_; ++i,tptr+=metaNumColumns_) {
      rowAddressTable_[i].useExternData(metaNumColumns_,
                                        tptr,
                                        ConstantReference);
    }

    fill(iniValue);
  }


  // copy constructor
  template <typename T>
  genericMatrix<T>::genericMatrix(const bool copyData, 
                                  genericMatrix<T>& other,
                                  const int fromRow,
                                  const int fromCol,
                                  const int toRow,
                                  const int toCol)
    : container(),theSize_(0,0),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(0),metaNumColumns_(0),totalSize_(0),
      ownData_(true),mode_(Connected),theElements_(0),rowAddressTable_(0) {

    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    // check bounds
    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= other.columns()) {
      tc = other.lastColumn();
    }
    if (tr >= other.rows()) {
      tr = other.lastRow();
    }

    if ((fc > tc) || (fr > tr)) { // negative areas?
      return;                     // yes -> do nothing!
    }

    theSize_.x = tc - fc + 1;
    theSize_.y    = tr - fr + 1;

    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;

    if (copyData) {
      copy(other,fr,fc,tr,tc);
    } else { // don't copy -> reference to 'other'`s data.
      int i,j;
      metaNumColumns_ = other.metaColumns();
      metaNumRows_    = other.metaRows();
      totalSize_ = metaNumRows_*metaNumColumns_;

      theElements_ = other.theElements_;

      ownData_ = false;

      if (metaNumColumns_ == theSize_.x) {
        mode_ = Connected;
      } else {
        mode_ = Line;
      }

      // initialize row address table
      rowAddressTable_ = allocRows(theSize_.y);
      if (rowAddressTable_ == 0) {  // memory allocation check
        throw allocException();
      }

      for (i=0,j=fr;j<=tr;++i,++j) {
        rowAddressTable_[i].useExternData(theSize_.x,
                                          &other.at(j,fc),
                                          ConstantReference);
      }
    }
  }

  // copy constructor
  template <typename T>
  genericMatrix<T>::genericMatrix(const genericMatrix<T>& other)
    : container(),theSize_(0,0),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(0),metaNumColumns_(0),totalSize_(0),
      ownData_(true),mode_(Connected),theElements_(0),rowAddressTable_(0) {

    copy(other);
  }

  // copy constructor
  template <typename T>
  genericMatrix<T>::genericMatrix(const genericMatrix<T>& other,
                                  const ipoint& from,
                                  const ipoint& to)
    : container(),theSize_(0,0),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(0),metaNumColumns_(0),totalSize_(0),
      ownData_(true),mode_(Connected),theElements_(0),rowAddressTable_(0) {

    copy(other,from,to);
  }

  // copy constructor
  template <typename T>
  genericMatrix<T>::genericMatrix(const genericMatrix<T>& other,
                                  const int fromRow,
                                  const int fromCol,
                                  const int toRow,
                                  const int toCol)
    : container(),theSize_(0,0),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(0),metaNumColumns_(0),totalSize_(0),
      ownData_(true),mode_(Connected),theElements_(0),rowAddressTable_(0) {

    copy(other,fromRow,fromCol,toRow,toCol);
  }

  // copy constructor
  template <typename T>
  genericMatrix<T>::genericMatrix(const genericMatrix<T>& other,
                                  const genericVector<int>& idx)
    : container(),theSize_(0,0),
      lastRowIdx_(theSize_.y-1),lastColIdx_(theSize_.x-1),
      metaNumRows_(0),metaNumColumns_(0),totalSize_(0),
      ownData_(true),mode_(Connected),theElements_(0),rowAddressTable_(0) {
    copy(other, idx);
  }

  template <typename T>
  genericMatrix<T>::~genericMatrix() {
    if (ownData_) {
      delete[] theElements_;
    }

    delete[] rowAddressTable_;

    theElements_ = 0;
    rowAddressTable_ = 0;
    theSize_.x = metaNumColumns_ = theSize_.y = metaNumRows_ = totalSize_ = 0;
  }

  // restore ownership
  template <typename T>
  void genericMatrix<T>::restoreOwnership() {

    // already my data, go away...
    if (ownData_) {
      return;
    }

    int newRows = rows();
    int newCols = columns();
    const int sz = newCols*newRows;

    if ( int64(sz) != int64(newRows)*int64(newCols) ) {
      // memory allocation check
      throw allocException();
    }


    T* newElements = new T[sz];

    if (newElements == 0) { // memory allocation check
      throw allocException();
    }

    // if connected memory then data can be copied at once...
    if ((mode_ == Connected) && (newCols == metaNumColumns_)) {
      if (sz > 0) {
        memcpy(newElements, theElements_, sz*sizeof(T));
      }
    } else {
      int i;
      if (newCols>0) {
        // copy line by line...
        T* tptr=newElements;
        for (i=0;i<newRows;++i,tptr+=newCols) {
          memcpy(tptr,&at(i,0),newCols*sizeof(T));
        }
      }
    }

    ownData_ = true;
    mode_ = Connected;
    theSize_.y    = metaNumRows_    = newRows;
    theSize_.x = metaNumColumns_ = newCols;
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;
    totalSize_  = metaNumRows_*metaNumColumns_;

    delete[] rowAddressTable_;
    rowAddressTable_ = allocRows(theSize_.y);
    if (rowAddressTable_ == 0) {
      throw allocException();
    }

    theElements_ = newElements;
    T* tptr = theElements_;
    for (int i=0;i<theSize_.y;++i,tptr+=theSize_.x) {
      rowAddressTable_[i].useExternData(theSize_.x,
                                        tptr,
                                        ConstantReference);
    }
  }

  template <typename T>
  void genericMatrix<T>::useExternData(const int r, const int c, 
                                       T* _data) {
    if (isNull(_data) ||
        (r == 0) ||
        (c == 0)) {
      clear();
      return;
    }

    if (ownData_) {
      delete [] theElements_;
    }

    ownData_ = false;

    delete[] rowAddressTable_;

    mode_ = Connected;
    theElements_ = _data;       // reference to the data
    theSize_.y = metaNumRows_ = r;
    theSize_.x = metaNumColumns_ = c;
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;

    totalSize_ = metaNumColumns_*metaNumRows_;

    rowAddressTable_ = allocRows(theSize_.y);
    if (rowAddressTable_ == 0) { // memory allocation check
      throw allocException();
    }

    T* tptr = theElements_;
    for (int i=0; i<theSize_.y; ++i,tptr+=theSize_.x) {
      rowAddressTable_[i].useExternData(theSize_.x,
                                        tptr,
                                        ConstantReference);
    }
  }

  // attach external memory region to this object
  template <typename T>
  void genericMatrix<T>::attach(const int r, const int c, T* _data) {
    useExternData(r,c,_data);
    ownData_ = true;
  }

  // detach internal memory to an external 'receiver' genericMatrix
  template <typename T>
  void genericMatrix<T>::detach(genericMatrix<T>& receiver) {
    // cannot detach data that is not connected since the call to
    // useExternData() would fail.
    if (mode_==Connected) {
      // receiver should know whether he owns data, so it is not
      // accidentally deleted
      if (ownData_) {
        // with "attach" the receiver will take control of _theElements
        receiver.attach(metaNumRows_,metaNumColumns_,theElements_);
      } else {
        // just use _theElements
        receiver.useExternData(metaNumRows_,metaNumColumns_,theElements_);
      }
      ownData_ = false;  // avoid removing theElements_
      clear();
    } else {
      // lined matrix
      // just transfer everything as it is
      receiver.clear();

      receiver.theSize_.copy(theSize_);
      theSize_.set(0,0);

      receiver.lastRowIdx_ = lastRowIdx_;
      lastRowIdx_=-1;

      receiver.lastColIdx_ = lastColIdx_;
      lastColIdx_=-1;

      receiver.metaNumRows_ = metaNumRows_;
      metaNumRows_=0;

      receiver.metaNumColumns_ = metaNumColumns_;
      metaNumColumns_=0;

      receiver.totalSize_ = totalSize_;
      totalSize_=0;

      receiver.ownData_ = ownData_;
      ownData_ = true;

      receiver.mode_ = mode_;
      mode_=Connected;

      receiver.theElements_ = theElements_;
      theElements_=0;

      receiver.rowAddressTable_ = rowAddressTable_;
      rowAddressTable_=0;
    }
    
  }

  // detach internal memory to an external 'receiver' genericVector
  template <typename T>
  void genericMatrix<T>::detach(genericVector<T>& receiver) {
    // cannot detach data that is not connected since the call to
    // useExternData() would fail.
    assert(mode_==Connected);
    // receiver should know whether he owns data, so it is not
    // accidentally deleted
    if (ownData_) {
      receiver.attach(totalSize_,theElements_);
    } else {
      receiver.useExternData(totalSize_,theElements_);
    }
    ownData_ = false;
    clear();
  }

  // exchange data with another genericMatrix
  template <typename T>
  void genericMatrix<T>::swap(genericMatrix<T>& other) {

    bool tmpBool;
    eStoreMode tmpMode;
    T* tmpTPtr;
    genericVector<T>* tmpVctPtr;
    int tmpInt;

    tmpInt = theSize_.x;
    theSize_.x = other.theSize_.x;
    other.theSize_.x = tmpInt;

    tmpInt = theSize_.y;
    theSize_.y = other.theSize_.y;
    other.theSize_.y = tmpInt;

    tmpInt = lastRowIdx_;
    lastRowIdx_ = other.lastRowIdx_;
    other.lastRowIdx_ = tmpInt;
      
    tmpInt = lastColIdx_;
    lastColIdx_ = other.lastColIdx_;
    other.lastColIdx_ = tmpInt;

    tmpInt = metaNumRows_;
    metaNumRows_ = other.metaNumRows_;
    other.metaNumRows_ = tmpInt;

    tmpInt = metaNumColumns_;
    metaNumColumns_ = other.metaNumColumns_;
    other.metaNumColumns_ = tmpInt;

    tmpInt = totalSize_;
    totalSize_ = other.totalSize_;
    other.totalSize_ = tmpInt;

    tmpBool = ownData_;
    ownData_ = other.ownData_;
    other.ownData_ = tmpBool;

    tmpMode = mode_;
    mode_ = other.mode_;
    other.mode_ = tmpMode;

    tmpTPtr = theElements_;
    theElements_ = other.theElements_;
    other.theElements_ = tmpTPtr;

    tmpVctPtr = rowAddressTable_;
    rowAddressTable_ = other.rowAddressTable_;
    other.rowAddressTable_ = tmpVctPtr;
  }

  // resize genericMatrix
  template <typename T>
  void genericMatrix<T>::resize(const int newRows,const int newCols,
                                const T& iniValue,
                                const eResizeType resizeType) {

    // if the new size is zero then clear all data
    if ((newRows <= 0) || (newCols <= 0)) {
      if (ownData_) {
        delete[] theElements_;
      } else {
        ownData_ = true;
      }

      theSize_.y = metaNumRows_ = 0;
      theSize_.x = metaNumColumns_ = 0;
      lastRowIdx_=theSize_.y-1;
      lastColIdx_=theSize_.x-1;
      totalSize_ = 0;

      theElements_ = 0;

      delete[] rowAddressTable_;
      rowAddressTable_ = 0;

      return; // ready!
    }

    // is resize needed?
    if ((newRows==rows()) &&
        (newCols==columns()) &&
        (mode_==Connected)) {
      // resize not needed, just initialize if asked...
      if (resizeType==Init) {
        fill(iniValue);
      }

      return;
    }

    int fromCol,fromRow;
    T* newElements = 0;
    fromCol = fromRow = 0;

    if ( int64(newRows*newCols) != int64(newRows)*int64(newCols) ) {
      // memory allocation check
      throw allocException();
    }

    newElements = new T[newRows*newCols];

    // memory allocation check
    if (newElements == 0) { 
      throw allocException();
    }

    // old data should be copied
    if (resizeType==Copy || resizeType==CopyAndInit) {
      fromRow = min(theSize_.y,newRows);
      fromCol = min(theSize_.x,newCols);
      // if connected memory then data can be copied at once...
      if ((mode_ == Connected) && (newCols == metaNumColumns_)) {
        const int sz = newCols*fromRow;
        if (sz > 0) {
          memcpy(newElements, theElements_, sz*sizeof(T));
        }
      } else {
        int i;
        if (fromCol>0) {
          // copy line by line...
          T* tptr = newElements;
          for (i=0;i<fromRow;++i,tptr+=newCols) {
            memcpy(tptr,&at(i,0),fromCol*sizeof(T));
          }
        }
      }
    }

    if (ownData_) {
      delete[] theElements_;
    } else {
      ownData_ = true;
    }

    mode_ = Connected;
    theSize_.y    = metaNumRows_    = newRows;
    theSize_.x = metaNumColumns_ = newCols;
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;
    totalSize_  = metaNumRows_*metaNumColumns_;

    delete[] rowAddressTable_;
    rowAddressTable_ = allocRows(theSize_.y);

    if (rowAddressTable_ == 0) {
      throw allocException();
    }

    theElements_ = newElements;

    T* tptr = theElements_;
    for (int i=0;i<theSize_.y;++i,tptr+=theSize_.x) {
      rowAddressTable_[i].useExternData(theSize_.x,
                                        tptr,
                                        ConstantReference);
    }

    if (resizeType==Init || resizeType==CopyAndInit) {
      fill(iniValue,0,fromCol,fromRow-1,lastColIdx_);
      fill(iniValue,fromRow,0,theSize_.y-1,lastColIdx_);
    }
  }

  //

  template<typename T>
  void genericMatrix<T>::clear() {
    resize(0,0,T(),AllocateOnly);
  }

  // fill the genericMatrix with 'iniValue'
  template <typename T>
  void genericMatrix<T>::fill(const T& iniValue,
                              const int fromRow,
                              const int fromCol,
                              const int toRow,
                              const int toCol) {
    // check and correct boundaries
    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= columns()) {
      tc = lastColumn();
    }
    if (tr >= rows()) {
      tr = lastRow();
    }

    if ((fc > tc) || (fr > tr)) { // negative areas?
      return;                     // yes -> do nothing!
    }

    if ((fc == 0) && (tc == lastColumn()) && (mode_ == Connected)) {
      // fill as a genericVector!!
      genericVector<T> v;
      v.useExternData(columns()*(tr-fr+1),&at(fr,0),ConstantReference);
      v.fill(iniValue);
    } else {
      // each line will be treated as a genericVector.
      genericVector<T> v;
      const int sz = tc-fc+1;

      for (int i=fr; i<=tr; ++i) {
        v.useExternData(sz, &at(i,fc), VariableReference);
        v.fill(iniValue);
      }
    }
  }

  // fill the genericMatrix with 'iniValue'
  template <typename T>
  void genericMatrix<T>::fill(const T   _data[],
                              const int fromRow,
                              const int fromCol,
                              const int toRow,
                              const int toCol) {
    // check and correct boundaries
    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= columns()) {
      tc = lastColumn();
    }
    if (tr >= rows()) {
      tr = lastRow();
    }

    if ((fc > tc) || (fr > tr)) { // negative areas?
      return;                     // yes -> do nothing!
    }

    if ((fc == 0) && (tc == lastColumn()) && (mode_ == Connected)) {
      // fill as a genericVector!!
      genericVector<T> v;
      v.useExternData(columns()*(tr-fr+1),&at(fr,0),ConstantReference);
      v.fill(_data);
    } else {
      // each line will be treated as a genericVector.
      genericVector<T> v;
      const int sz = tc-fc+1;
      const T* ptr = _data;

      for (int i=fr; i<=tr; ++i) {
        v.useExternData(sz, &at(i,fc), VariableReference);
        v.fill(ptr);
        ptr+=sz;
      }

      ptr = 0;
    }
  }

  // fill (partially) this genericMatrix with the contents of another genericMatrix
  template <typename T>
  void genericMatrix<T>::fill(const genericMatrix<T>& mat,
                              const int fromRow, const int fromCol,
                              const int toRow, const int toCol,
                              const int startAtRow,
                              const int startAtCol) {
    int fr,tr,fc,tc,nr,i,row,sr;
    genericVector<T> vct,tmp;

    // check bounds

    fr = max(0,fromRow);
    fr = min(fr,lastRow());

    tr = min(lastRow(),toRow);
    tr = max(0,tr);

    fc = max(0,fromCol);
    fc = min(fc,lastColumn());

    tc = min(lastColumn(),toCol);
    tc = max(0,tc);

    // invalid source or target genericMatrix boundaries?
    if ((fc > tc) || (fr > tr) ||
        (startAtRow >= mat.rows()) || (startAtCol >= mat.columns())) {
      return; // yes!
    }

    // number of rows
    nr = tr-fr+1;
    nr = min(nr,mat.rows()-startAtRow);

    // copy data
    for (i=0,row=fr,sr=startAtRow;i<nr;++i,++sr,++row) {
      getRow(row).fill(mat.getRow(sr),fc,tc,startAtCol);
    }
  }

  // return a copy of a columns genericVector
  template <typename T>
  void genericMatrix<T>::getColumnCopy(const int col,
                                       genericVector<T>& theCol) const {
    assert(col<theSize_.x);

    theCol.allocate(rows());
    for (int i=0;i<rows();++i)
      theCol.at(i) = at(i,col);
  }

  // return a copy of the diagonal of the genericMatrix
  template <typename T>
  void genericMatrix<T>::getDiagonal(genericVector<T>& theCol) const {
    const int n=min(rows(),columns());
    theCol.allocate(n);
    typename genericVector<T>::iterator j=theCol.begin();
    for (int i=0; i<n; ++i) {
      *j++ = at(i,i);
    }
  }
  
  template <typename T>
  void genericMatrix<T>::setDiagonal(const genericVector<T>& diag) {
    
    typename genericVector<T>::const_iterator dit=diag.begin();
    const int stopper=min(diag.size(), min(rows(),columns()));

    if (mode_==Connected) {
      iterator it=begin();
      const int incr=columns()+1;
      iterator ite=it+incr*stopper;
      for (;it!=ite;it+=incr,++dit) {
        (*it)=(*dit);
      }
    } else {
      int i=0;
      for (;i<stopper;i++,++dit) {
        at(i,i)=(*dit);
      }
    }
    
  }


  // fills a column with the contents of an external genericVector
  template <typename T>
  void genericMatrix<T>::setColumn(const int col,
                                   const genericVector<T>& theCol) {
    assert(col<theSize_.x);
    int i,minRows;
    minRows=min(rows(),theCol.size());
    for (i=0;i<minRows;++i)
      at(i,col) = theCol.at(i);
  }


  // copy an external genericMatrix
  template <typename T>
  genericMatrix<T>& genericMatrix<T>::copy(const genericMatrix<T>& other) {

    if (&other == this) { // Caution!! It could be the same genericMatrix...
      return (*this);
    }

    /*
     * pointer to the elements of the genericMatrix.
     *
     * If set to non-zero means that, for some reason, the data cannot be 
     * deleted yet, and needs to be removed after everything has been
     * copied.
     */
    T* delayedElements = 0;

    // erase old data!
    if (ownData_) {
      if ((other.columns() != columns()) || (other.rows() != rows())) {

        if ((other.theElements_ == theElements_) && !other.ownData_) {
          // The other genericMatrix seems to be a submatrix of this one
          // Delay the removal of theElements_ until the data has been copied.
          delayedElements = theElements_;
          theElements_ = 0;
        } else {
          delete[] theElements_;
          theElements_ = 0;
        }

        delete[] rowAddressTable_;
        rowAddressTable_ = 0;        
      }
    } else {
      ownData_ = true;
      theElements_ = 0;
      delete[] rowAddressTable_;
      rowAddressTable_ = 0;
    }

    if (other.size() == ipoint(0,0)) { // empty genericMatrix?
      // generate empty genericMatrix
      theSize_.x = metaNumColumns_ = 0;
      theSize_.y    = metaNumRows_    = 0;
      lastRowIdx_=theSize_.y-1;
      lastColIdx_=theSize_.x-1;
      totalSize_  = 0;
      ownData_ = true;
      mode_ = Connected;

      if (notNull(delayedElements)) {
        delete[] delayedElements;
      }
      return (*this);
    }

    theSize_.x = other.columns();
    theSize_.y    = other.rows();
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;


    // The data must be copied!
    // It will be saved as "connected" memory
    mode_ = Connected;

    // Physical and access boundaries are the same!
    metaNumColumns_ = theSize_.x;
    metaNumRows_    = theSize_.y;
    totalSize_      = metaNumColumns_*metaNumRows_;

    // allocate memory
    if (isNull(theElements_)) {

      if ( int64(totalSize_) != int64(metaNumColumns_)*int64(metaNumRows_) ) {
        // memory allocation check
        throw allocException();
      }

      theElements_ =  new T[totalSize_];
      if (theElements_ == 0) { // memory allocation check
        if (notNull(delayedElements)) {
          delete[] delayedElements;
        }
        throw allocException();
      }
    }

    if (isNull(rowAddressTable_)) {
      rowAddressTable_ = allocRows(theSize_.y);

      T* tptr=theElements_;
      for (int i=0;i<theSize_.y;++i,tptr+=theSize_.x) {
        rowAddressTable_[i].useExternData(theSize_.x,tptr,ConstantReference);
      }
    }

    // Could the whole data be copied at once?
    if ((other.getMode() == Connected) &&
        (metaNumColumns_ == other.metaColumns())) {
      const int sz = theSize_.y*theSize_.x;
      if (sz > 0) {
        // the whole data will be copied at once
        memcpy(theElements_, &other.at(0,0), sz*sizeof(T));
      }

    } else {
      // the data must be copied line by line
      int i;
      if (theSize_.x>0) {
        T* tptr = theElements_;
        for (i=0;i<other.rows();++i,tptr+=theSize_.x) {
          memcpy(tptr,&other.at(i,0),theSize_.x*sizeof(T));
        }
      }
    }

    // Remove the data of the src genericMatrix, in those cases where that data
    // belonged also to this genericMatrix:
    if (notNull(delayedElements)) {
      delete[] delayedElements;
    }
    
    return (*this);
  }

  // copy an external genericMatrix
  template <typename T>
  genericMatrix<T>& genericMatrix<T>::copy(const genericMatrix<T>& other,
                                           const int fromRow,
                                           const int fromCol,
                                           const int toRow,
                                           const int toCol) {
    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    // check bounds
    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= other.columns()) {
      tc = other.lastColumn();
    }
    if (tr >= other.rows()) {
      tr = other.lastRow();
    }

    if ((fc == 0) && (tc == other.lastColumn()) &&
        (fr == 0) && (tr == other.lastRow())) {
      return copy(other);
    }

    if (&other == this) { // Caution!! It could be the same genericMatrix...
      if (((tc - fc + 1) == theSize_.x) && ((tr - fr + 1) == theSize_.y)) {
        return (*this); // a copy of itself is ready!
      } else {
        genericMatrix<T> tmpMat;
        tmpMat.copy(*this,fr,fc,tr,tc);
        swap(tmpMat);
        return *this;             // return itself!
      }
    }

    // erase old data!
    if (ownData_) {
      delete[] theElements_;
    } else {
      ownData_ = true;
    }

    theElements_ = 0;

    delete[] rowAddressTable_;
    rowAddressTable_ = 0;

    if ((fc > tc) || (fr > tr)) { // negative areas?
      // generate empty genericMatrix
      theSize_.x = metaNumColumns_ = 0;
      theSize_.y    = metaNumRows_    = 0;
      lastRowIdx_=theSize_.y-1;
      lastColIdx_=theSize_.x-1;
      totalSize_  = 0;
      ownData_ = true;
      mode_ = Connected;
      return (*this);
    }

    theSize_.x = tc - fc + 1;
    theSize_.y    = tr - fr + 1;
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;

    // The data must be copied!
    // It will be saved as "connected" memory
    mode_ = Connected;

    // Physical and access boundaries are the same!
    metaNumColumns_ = theSize_.x;
    metaNumRows_    = theSize_.y;
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;
    totalSize_      = metaNumColumns_*metaNumRows_;

    if ( int64(totalSize_) != int64(metaNumColumns_)*int64(metaNumRows_) ) {
      // memory allocation check
      throw allocException();
    }

    // allocate memory
    theElements_ =  new T[totalSize_];

    if (theElements_ == 0) { // memory allocation check
      throw allocException();
    }

    rowAddressTable_ = allocRows(theSize_.y);

    // Could the whole data be copied at once?
    if ((other.getMode() == Connected) &&
        (metaNumColumns_ == other.metaColumns())) {
      const int sz = theSize_.y*theSize_.x;
      if (sz > 0) {
        // the whole data will be copied at once
        memcpy(theElements_, &other.at(fr,0), sz*sizeof(T));
      }

      T* tptr = theElements_;
      for (int i=0;i<theSize_.y;++i,tptr+=theSize_.x) {
        rowAddressTable_[i].useExternData(theSize_.x,tptr,ConstantReference);
      }
    } else {
      // the data must be copied line by line
      if (theSize_.x>0) {
        T* tptr = theElements_;
        for (int i=0,j=fr; j<=tr; ++i,++j,tptr+=theSize_.x) {
          rowAddressTable_[i].useExternData(theSize_.x,tptr,ConstantReference);
          memcpy(tptr,&other.at(j,fc),theSize_.x*sizeof(T));
        }
      }
    }

    return (*this);
  }

  // copy an external genericMatrix
  template <typename T>
  genericMatrix<T>& genericMatrix<T>::copy(const genericMatrix<T>& other,
                                           const genericVector<int>& idx,
                                           bool rowWise) {


    if (&other == this) { // Caution!! It could be the same genericMatrix...
                          // a submatrix should be generated:

      genericMatrix<T> tmpMat;
      tmpMat.copy(*this,idx);
      swap(tmpMat);
      return *this;             // return itself!
    }

    // erase old data!
    if (ownData_) {
      delete[] theElements_;
    } else {
      ownData_ = true;
    }

    theElements_ = 0;

    delete[] rowAddressTable_;
    rowAddressTable_ = 0;

    if (idx.empty()) { // copy no rows
      // generate empty genericMatrix
      theSize_.x = metaNumColumns_ = 0;
      theSize_.y    = metaNumRows_    = 0;
      lastRowIdx_=theSize_.y-1;
      lastColIdx_=theSize_.x-1;
      totalSize_  = 0;
      ownData_ = true;
      mode_ = Connected;
      return (*this);
    }

    if (rowWise) {
      theSize_.x = other.theSize_.x;
      theSize_.y    = idx.size();
    } else {
      theSize_.x = idx.size();
      theSize_.y    = other.theSize_.y;
    }

    // The data must be copied!
    // It will be saved as "connected" memory
    mode_ = Connected;

    // Physical and access boundaries are the same!
    metaNumColumns_ = theSize_.x;
    metaNumRows_    = theSize_.y;
    lastRowIdx_=theSize_.y-1;
    lastColIdx_=theSize_.x-1;
    totalSize_      = metaNumColumns_*metaNumRows_;

    if ( int64(totalSize_) != int64(metaNumColumns_)*int64(metaNumRows_) ) {
      // memory allocation check
      throw allocException();
    }

    // allocate memory
    theElements_ =  new T[totalSize_];

    if (theElements_ == 0) { // memory allocation check
      throw allocException();
    }

    rowAddressTable_ = allocRows(theSize_.y);

    int i;
    T* tptr = theElements_;
    if (rowWise) {
      for (i=0; i<theSize_.y; ++i,tptr+=theSize_.x) {
        rowAddressTable_[i].useExternData(theSize_.x,tptr,ConstantReference);
        memcpy(tptr,&other.at(idx.at(i),0),theSize_.x*sizeof(T));
      }
    } else {
      int j;
      for (i=0; i<theSize_.y; ++i) {
        const genericVector<T>& vct = other.getRow(i);
        rowAddressTable_[i].useExternData(theSize_.x,tptr,ConstantReference);
        for (j=0; j<theSize_.x; ++j, ++tptr) {
          (*tptr)=vct.at(idx.at(j));
        }
      }
    }

    return (*this);
  }

  template <typename T>
  template<typename U>
  genericMatrix<T>& genericMatrix<T>::castFrom(const genericMatrix<U>& other) {
    allocate(other.rows(),other.columns());
    int y;
    for (y=0;y<rows();y++) {
      getRow(y).castFrom(other.getRow(y));
    }
    
    return (*this);
  }

  template <typename T>
  genericMatrix<T>& genericMatrix<T>::castFrom(const genericMatrix<T>& other) {
    return copy(other);
  }

  template <typename T>
  template<typename U>
  genericMatrix<T>& genericMatrix<T>::castFrom(const genericMatrix<U>& other,
                                               const int fromRow,
                                               const int fromCol,
                                               const int toRow,
                                               const int toCol) {

    const int fc= (fromCol>0) ? fromCol : 0;
    const int tc= (toCol<other.columns()) ? toCol : other.lastColumn();

    const int fr= (fromRow>0) ? fromRow : 0;
    const int tr= (toRow<other.rows()) ? toRow : other.lastRow();

    allocate(tr-fr+1,tc-fc+1);
    int y,yy;
    typename genericVector<U>::const_iterator it;
    typename genericVector<T>::iterator dit,eit;

    // we do not use the genericVector<T>::castFrom to avoid many unnecessary
    // computations for each row.
    
    for (y=fr,yy=0;y<=tr;++y,++yy) {
      it = other.getRow(y).begin()+fc;
      genericVector<T>& vct = getRow(yy);
      for (dit=vct.begin(),eit=vct.end();dit!=eit;++it,++dit) {
        (*dit)=static_cast<U>(*it);
      }
    }
    
    return (*this);
  }

  template <typename T>
  genericMatrix<T>& genericMatrix<T>::castFrom(const genericMatrix<T>& other,
                                               const int fromRow,
                                               const int fromCol,
                                               const int toRow,
                                               const int toCol) {
    return copy(other, fromRow, fromCol, toRow, toCol);
  }

  // returns name of the class
  template<typename T>
  const std::string& genericMatrix<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // creates a clone of this genericMatrix
  template<typename T>
  genericMatrix<T>* genericMatrix<T>::clone() const {
    return new genericMatrix<T>(*this);
  }

  template<typename T>
  genericMatrix<T>* genericMatrix<T>::newInstance() const {
    return new genericMatrix<T>();
  }

  /* compare this genericMatrix with other
     @param other the other genericMatrix to be compared with
     @return true if both matrices have the same elements and same size
  */
  template<typename T>
  bool genericMatrix<T>::equals(const genericMatrix<T>& other) const {
    bool result;

    result = (size() == other.size());

    if (mode_ == Connected) {
      result = result &&
        (0 == memcmp(theElements_,other.theElements_,totalSize_*sizeof(T)));
    } else {
      int y;
      for (y=0;result && (y<rows());++y) {
        result = result && getRow(y).equals(other.getRow(y));
      }
    }

    return result;
  }

  // applies a C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(T (*function)(T)) {
    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(function);
    };

    return (*this);
  }

  // applies a C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(T)) {
    if (size() != other.size()) {
      allocate(other.size());
    }

    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    };

    return (*this);
  }


  // applies a C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(T (*function)(const T&)) {
    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(function);
    };

    return (*this);
  }

  // applies a C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(const T&)) {
    if (size() != other.size()) {
      allocate(other.size());
    }

    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    };

    return (*this);
  }


  // applies a two-parameter C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(const T&,const T&)) {

    assert (other.size() == size());

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    }

    return (*this);
  }

  // applies a two-parameter C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(T,T)) {

    assert (other.size() == size());

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    }

    return (*this);
  }


  // applies a two-parameter C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& a,
                                            const genericMatrix<T>& b,
                                            T (*function)(T,T)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      allocate(a.size());
    }

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(a.getRow(y),b.getRow(y),function);
    }

    return (*this);
  }


  // applies a two-parameter C-Function to each element
  template<typename T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& a,
                                            const genericMatrix<T>& b,
                                            T (*function)(const T&,const T&)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      allocate(a.size());
    }

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(a.getRow(y),b.getRow(y),function);
    }

    return (*this);
  }

  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /**
   * write the object in the given ioHandler
   */
  template<typename T>
  bool genericMatrix<T>::write(ioHandler& handler,const bool complete) const {

    ipoint i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"size",size());

      handler.writeBegin();
      handler.writeSymbol(std::string("data"));
      handler.writeKeyValueSeparator();
      handler.writeBegin();

      int lastCol = lastColumn();

      for (i.y=0;i.y<rows();++i.y) {
        handler.writeBegin();
        if (columns() > 0) {
          for (i.x=0;i.x<lastCol;++i.x) {
            lti::write(handler,at(i));
            handler.writeDataSeparator();
          }
          lti::write(handler,at(i));
        }
        handler.writeEnd();
        handler.writeEOL();
      }

      handler.writeEnd();
      handler.writeEnd();
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template<typename T>
  bool genericMatrix<T>::read(ioHandler& handler,const bool complete) {
    ipoint i,sz;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b = b && lti::read(handler,"size",sz);
      std::string str;

      int level = handler.getLevel();

      b = b && handler.readBegin();
      b = b && handler.readSymbol(str);

      if (str == "data") {
        b = b && handler.readKeyValueSeparator();
        b = b && handler.readBegin();
        allocate(sz);
        if (sz.y > 0) {
          sz.x--;
          i.y=0;
          while (b && (i.y<sz.y)) {
            b = handler.readBegin();
            if (b) {
              i.x=0;
              while (b && (i.x<sz.x)) {
                b = b && lti::read(handler,at(i));
                b = b && handler.readKeyValueSeparator();
                if (b) {
                  ++i.x;
                }
              }

              b = b && lti::read(handler,at(i));
              b = b && handler.readEnd();
              ++i.y;
            }
          }
        }
        if (!b) {
          handler.setStatusString("Error reading genericMatrix at row ");
          handler.appendStatusString(i.y);
          handler.appendStatusString(", col ");
          handler.appendStatusString(i.x);
          handler.appendStatusString(".");
        }
      }

      while (handler.readEnd() && (handler.getLevel() > level)) {
      }
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


} // namespace lti

/// ASCII output of a genericMatrix to a std stream.
namespace std {
  template <typename T>
  ostream& operator<<(ostream& s,const lti::genericMatrix<T>& amat) {

    int row;
    s << "(";

    if (amat.rows()>0) {
      s << amat.getRow(0);

      for (row=1;row<amat.rows();++row) {
        s << std::endl << " " << amat.getRow(row);
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
