/*
 * Copyright (C) 2007 by Pablo Alvarado
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

/* 
 * \file   ltiArray2D_template.h
 *         Contains the template class lti::array2D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiArray2D_template.h,v 1.2 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiRectangle.h"

namespace lti {
  // --------------------------------------------------
  // Two Dimensional Kernel
  // --------------------------------------------------

  // constructor
  template<typename T>
  array2D<T>::array2D() : matrix<T>() {
  }

  // copy constructor
  template<typename T>
  array2D<T>::array2D(const array2D<T>& other) : matrix<T>() {
    copy(other);
  }

  // constructor
  template<typename T>
  array2D<T>::array2D(const int theRows, const int theColumns,
                        const T& iniValue)
    : matrix<T>(theRows,theColumns,iniValue) {
  }

  // constructor
  template<typename T>
  array2D<T>::array2D(const int fromRow,const int fromCol,
                        const int toRow,const int toCol,
                        const T& iniValue)
    : matrix<T>(toRow-fromRow+1,toCol-fromCol+1,iniValue) {
    offset_ = ipoint(-fromCol,-fromRow);
  }

  // constructor
  template<typename T>
  array2D<T>::array2D(const int theRows, const int theColumns)
    : matrix<T>(theRows,theColumns) {
  }

  // constructor
  template<typename T>
  array2D<T>::array2D(const int fromRow,const int fromCol,
                        const int toRow,const int toCol)
    : matrix<T>(toRow-fromRow+1,toCol-fromCol+1) {
    offset_ = ipoint(-fromCol,-fromRow);
  }

  // destructor
  template<typename T>
  array2D<T>::~array2D() {
  }

  // copy
  template<typename T>
  array2D<T>& array2D<T>::copy(const array2D<T>& other) {
    matrix<T>::copy(other);
    offset_ = other.getOffset();
    return *this;
  }

  // clone
  template<typename T>
  const std::string& array2D<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template<typename T>
  array2D<T>* array2D<T>::clone() const {
    return new array2D<T>(*this);
  }

  // new instance
  template<typename T>
  array2D<T>* array2D<T>::newInstance() const {
    return new array2D<T>();
  }

  // get Offset
  template<typename T>
  const ipoint& array2D<T>::getOffset() const {
    return offset_;
  }

  // set Offset
  template<typename T>
  void array2D<T>::setOffset(const ipoint& p) {
    offset_ = p;
  }

  template<typename T>
  void array2D<T>::resize(const int fromRow,const int fromCol,
                           const int toRow,const int toCol,
                           const T& iniValue,
                           const eResizeType& resizeType) {
    matrix<T>::resize(toRow-fromRow+1,toCol-fromCol+1,
                      iniValue,resizeType);
    offset_ = ipoint(-fromCol,-fromRow);
  }

  /*
   * copy the content of the other matrix in this kernel and assign
   * the index (firstRow,firstColumn) to the first element of the matrix.
   * For example if <code>other</code> is a 3x3 matrix, then
   * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
   * lay inside [-1,1].
   * @param other the matrix with the data to be copied
   * @param firstRow index for the first row of the matrix
   * @param firstColumn index for the first column of the matrix
   * @return a reference to this instance
   */
  template<typename T>
  array2D<T>& array2D<T>::castFrom(const matrix<T>& other,
                                   const int theFirstRow,
                                   const int theFirstColumn) {
    matrix<T>::copy(other);
    setOffset(ipoint(-theFirstColumn,-theFirstRow));
    return (*this);
  }

  /*
   * copy the content of the other matrix in this kernel and assign
   * the index (firstRow,firstColumn) to the first element of the matrix.
   * For example if <code>other</code> is a 3x3 matrix, then
   * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
   * lay inside [-1,1].
   * @param other the matrix with the data to be copied
   * @param firstRow index for the first row of the matrix
   * @param firstColumn index for the first column of the matrix
   * @return a reference to this instance
   */
  template<typename T>
  array2D<T>& array2D<T>::castFrom(const matrix<T>& other,
                                   const ipoint& firstElement) {
    return castFrom(other,firstElement.y,firstElement.x);
  }

  template <typename T>
  void array2D<T>::fill(const matrix<T>& mat,
                        const int fromRow, const int fromCol,
                        const int toRow,   const int toCol,
                        const int startAtRow,
                        const int startAtCol) {
    ipoint from(fromRow,fromCol);
    ipoint to(toRow,toCol);
    from.add(offset_);
    to.add(offset_);

    matrix<T>::fill(mat,from.y,from.x,to.y,to.x,startAtRow,startAtCol);
  }

  template <typename T>
  void array2D<T>::fill(const T& value,
                        const int fromRow, const int fromCol,
                        const int toRow,   const int toCol) {
    ipoint from(fromRow,fromCol);
    ipoint to(toRow,toCol);
    from.add(offset_);
    to.add(offset_);

    matrix<T>::fill(value,from.y,from.x,to.y,to.x);
  }

  /*
   * fills this kernel between the "from's" and "to's" with the
   * contents of the matrix <code>mat</code> starting at
   * <code>startAtRow</code> and <code>startAtCol</code>
   *
   * @param data     the data that should fill this kernel
   * @param fromRow  first row of the submatrix to be filled
   * @param fromCol  first column of the submatrix to be filled
   * @param toRow    last row of the submatrix to be filled
   * @param toCol    last column of the submatrix to be filled
   */
  template <typename T>
  void array2D<T>::fill(const T* _data,
                        const int fromRow,
                        const int fromCol,
                        const int toRow,
                        const int toCol) {
    
    ipoint from(fromRow,fromCol);
    ipoint to(toRow,toCol);
    from.add(offset_);
    to.add(offset_);

    matrix<T>::fill(_data,from,to);
  }


  // mirror other kernel
  template <typename T>
  array2D<T>& array2D<T>::mirror(const array2D<T>& other) {
    matrix<T>& mat = (*this);
    const matrix<T>& omat = other;

    mat.allocate(omat.size());

    int y;
    const int lastR = mat.lastRow();
    typename vector<T>::const_iterator cit,eit;
    T* ptr;
    for (y=0;y<omat.rows();++y) {
      const vector<T>& vct = omat.getRow(y);
      for (cit=vct.begin(),eit=vct.end(),ptr=&mat.at(lastR-y,vct.lastIdx());
           cit!=eit;
           ++cit,--ptr) {
        (*ptr)=(*cit);
      }
    }

    const ipoint& oos = other.getOffset();

    setOffset(ipoint(omat.lastColumn()-oos.x,
                     omat.lastRow()-oos.y));

    return (*this);
  }

  // mirror
  template <typename T>
  array2D<T>& array2D<T>::mirror() {
    array2D<T> tmp;
    tmp.mirror(*this);
    setOffset(tmp.getOffset());
    tmp.detach(*this);
    return (*this);
  }


  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<typename T>
  bool array2D<T>::write(ioHandler& handler,
                          const bool complete) const {

    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"offset",getOffset());
      const matrix<T>& mat = *this; // array as vector
      lti::write(handler,mat,false);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }

  /*
   * read the object from the given ioHandler
   */
  template<typename T>
  bool array2D<T>::read(ioHandler& handler,const bool complete) {
    ipoint offset;

    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"offset",offset);
      matrix<T>& mat = *this; // array as vector
      lti::read(handler,mat,false);
      setOffset(offset);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }
} //namespace
