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
 *         Contains the inline methods of class lti::genericMatrix
 *         which implements the generic bidimensional container
 * 
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * revisions ..: $Id: ltiGenericMatrix_inline.h,v 1.10 2012-09-16 04:58:34 alvarado Exp $
 */

#ifndef _LTI_GENERIC_MATRIX_INLINE_H_
#define _LTI_GENERIC_MATRIX_INLINE_H_

namespace lti {

  //------------------------------------------------------------------------
  // genericMatrix
  //------------------------------------------------------------------------

  template <typename T>
  inline genericVector<T>* genericMatrix<T>::allocRows(const int n) {
    return new genericVector<T>[n];
  }

  /* owns this %object the data?
     returns <em>false</em> if this genericMatrix contains a reference to
     extern data.
  */
  template<typename T>
  inline bool genericMatrix<T>::ownsData() const {
    return ownData_;
  }

  /* store mode
     @return <code>Connected</code> if the memory is a single block or
     <code>Line</code>      if the memory of each line is allocated
     in different places!
  */
  template<typename T>
  inline typename genericMatrix<T>::eStoreMode 
  genericMatrix<T>::getMode() const {
    return mode_;
  }

  // number of rows of the genericMatrix
  template<typename T>
  inline int genericMatrix<T>::rows() const {
    return theSize_.y;
  }

  // number of columns of the genericMatrix
  template<typename T>
  inline int genericMatrix<T>::columns() const {
    return theSize_.x;
  }

  // number of rows of the genericMatrix
  template<typename T>
  inline int genericMatrix<T>::lastRow() const {
    return lastRowIdx_;
  }

  // number of columns of the genericMatrix
  template<typename T>
  inline int genericMatrix<T>::lastColumn() const {
    return lastColIdx_;
  }

  /* number of "physical" rows of the matrix.
     @return If this is a <code>Connected</code> Matrix, (see getMode()),
     this member returns the same value as rows().

     If this is a <code>Line</code> Matrix, this value is bigger than
     rows().  If this was created with the copy constructor for a
     submatrix with "no copy data",
     this value will return the size of the original matrix.
  */
  template<typename T>
  inline int genericMatrix<T>::metaRows() const {
    return metaNumRows_;
  }

  /* number of "physical" columns of the matrix
     @return If this is a <code>Connected</code> Matrix, (see getMode()),
     this member returns the same value as columns().

     If this is a <code>Line</code> Matrix, this value is bigger than
     columns().  If this was created with the copy constructor for a
     submatrix with "no copy data",
     this value will return the size of the original matrix.
  */
  template<typename T>
  inline int genericMatrix<T>::metaColumns() const {
    return metaNumColumns_;
  }

  /* returns the size of the %genericMatrix in a lti::point structure.
     @return lti::point with the number of columns in its
     <code>x</code> coordinate and the number of rows in its
     <code>y</code> coordinate.
  */
  template<typename T>
  inline const ipoint& genericMatrix<T>::size() const {
    return theSize_;
  }

  /* returns iterator to first element of the genericMatrix
   */
  template<typename T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::begin() {
#   ifdef NDEBUG
    return iterator(theElements_);
#   else
    return iterator(theElements_, theElements_, 
                    &theElements_[totalSize_]);
#   endif
  }

  /* returns const_iterator to first element of the genericMatrix
   */
  template<typename T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::begin() const {
#   ifdef NDEBUG
    return const_iterator(theElements_);
#   else
    return const_iterator(theElements_, theElements_, 
                          &theElements_[totalSize_]);
#   endif
  }

  // returns iterator to the end of the genericMatrix
  template<typename T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::end() {
#   ifdef NDEBUG
    return iterator(&theElements_[totalSize_]);
#   else
    return iterator(&theElements_[totalSize_],
                    theElements_, 
                    &theElements_[totalSize_]);
#   endif
  }

  // returns iterator to the end of the genericMatrix
  template<typename T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::end() const {
#   ifdef NDEBUG
    return const_iterator(&theElements_[totalSize_]);
#   else
    return const_iterator(&theElements_[totalSize_],
                          theElements_, 
                          &theElements_[totalSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericMatrix. It is used for inverse order
   * iteration through the genericMatrix using normal iterators (as opposed
   * to reverse iterators). This has the advantage that iterators
   * going from front to end and in the inverse direction are the
   * same and can thus be compared, copied etc.
   */
  template<typename T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::inverseBegin() {
#   ifdef NDEBUG
    return iterator(&theElements_[totalSize_-1]);
#   else
    return iterator(&theElements_[totalSize_-1], 
                    theElements_, 
                    &theElements_[totalSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericMatrix. See inverseBegin() for more details.
   */
  template<typename T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::inverseBegin() const {
#   ifdef NDEBUG
    return const_iterator(&theElements_[totalSize_-1]);
#   else
    return const_iterator(&theElements_[totalSize_-1], 
                          theElements_, 
                          &theElements_[totalSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericMatrix. It is used to
   * mark the end for inverse order iteration through the genericMatrix
   * using normal iterators (as opposed to reverse iterators). This
   * has the advantage that iterators going from front to end and in
   * the inverse direction are the same and can thus be compared,
   * copied etc.
   */
  template<typename T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::inverseEnd() {
#   ifdef NDEBUG
    return iterator(&theElements_[-1]);
#   else
    return iterator(&theElements_[-1], 
                    theElements_, 
                    &theElements_[totalSize_]);
#   endif
  }

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericMatrix.
   */
  template<typename T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::inverseEnd() const {
#   ifdef NDEBUG
    return const_iterator(&theElements_[-1]);
#   else
    return const_iterator(&theElements_[-1], 
                          theElements_, 
                          &theElements_[totalSize_]);
#   endif
  }

  template<typename T>
  inline T* genericMatrix<T>::data() {
    return theElements_;
  }

  template<typename T>
  inline const T* genericMatrix<T>::data() const {
    return theElements_;
  }



  // access operators...

  /* access element at the given row and column
     @param row the row of the element
     @param col the column of the element
     @return a reference to the genericMatrix element
  */
  template<typename T>
  inline T& genericMatrix<T>::at(const int row,const int col) {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(theSize_.y));
    return ( (rowAddressTable_[row].at(col)) );
  }

  // access operator (const)

  /* read-only access at the given row and column
     @param row the row of the element
     @param col the column of the element
     @return a reference to the genericMatrix element
  */
  template<typename T>
  inline const T& genericMatrix<T>::at(const int row, const int col) const {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(theSize_.y));
    return ( (rowAddressTable_[row].at(col)) );
  }

  /* access element at the given position.

  With this operator the genericMatrix can be accessed as a vector,
  where the rows of the genericMatrix are concatenated.  The access
  to the genericMatrix with at(row,col) is equivalent to
  at(row*columns()+col) with this operator.

  This access operator doesn't work with lined matrices (see getMode()).

  @param pos the index of the element of the genericMatrix
  @return a reference to the genericMatrix element
  */
  template<typename T>
  inline T& genericMatrix<T>::elem(const int pos) {

    assert ( pos < totalSize_ );

    return (theElements_[pos]);
  }

  /* access element at the given position.

  With this operator the genericMatrix can be accessed as a vector,
  where the rows of the genericMatrix are concatenated.  The access
  to the genericMatrix with at(row,col) is equivalent to
  at(row*columns()+col).

  This access operator doesn't work with lined matrices (see getMode()).

  @param pos the index of the element of the genericMatrix
  @return a reference to the genericMatrix element
  */
  template<typename T>
  inline const T& genericMatrix<T>::elem(const int pos) const {
    assert ( pos < totalSize_ );
    return (theElements_[pos]);
  }

  /* access operator of genericMatrix element as a point in a 2D-Map
     @param p position of the element (this is equivalent to at(p.y,p.x))
     @return a reference to the genericMatrix element
  */
  template<typename T>
  inline T& genericMatrix<T>::at(const ipoint& p) {
    return at(p.y,p.x);
  }

  /* const access operator of genericMatrix element as a point in a 2D-Map
     @param p position of the element (this is equivalent to at(p.y,p.x))
     @return a const reference to the vector element
  */
  template<typename T>
  inline const T& genericMatrix<T>::at(const ipoint& p) const {
    return at(p.y,p.x);
  }

  /* resize member
   */
  template<typename T>
  inline void genericMatrix<T>::resize(const ipoint& newDim,
                                       const T& iniValue,
                                       const eResizeType resizeType) {
    resize(newDim.y,newDim.x,iniValue,resizeType);
  }

  template<typename T>
  inline void genericMatrix<T>::resize(const int newRows,const int newCols) {
    resize(newRows,newCols,T(),Copy);
  }

  template<typename T>
  inline void genericMatrix<T>::resize(const ipoint& newDim) {
    resize(newDim.y,newDim.x,T(),Copy);
  }

  template<typename T>
  inline void genericMatrix<T>::allocate(const int newRows,
                                         const int newCols) {
    resize(newRows,newCols,T(),AllocateOnly);
  }

  template<typename T>
  inline void genericMatrix<T>::allocate(const ipoint& newDim) {
    resize(newDim.y,newDim.x,T(),AllocateOnly);
  }

  template<typename T>
  inline void genericMatrix<T>::assign(const int newRows,
                                       const int newCols,
                                       const T& initValue) {
    resize(newRows,newCols,initValue,Init);
  }

  template<typename T>
  inline void genericMatrix<T>::assign(const ipoint& newDim,
                                       const T& initValue) {
    resize(newDim.y,newDim.x,initValue,Init);
  }

  template<typename T>
  inline bool genericMatrix<T>::empty() const {
    return ((theSize_.x <= 0) || (theSize_.y <= 0));
  }

  /* return genericMatrix-row as a vector.
     This method works fast, since it returns a reference to the row vector.
     The data will NOT be copied.
     @param row the row to be accessed
     @return a reference to the vector row
  */
  template<typename T>
  inline genericVector<T>& genericMatrix<T>::getRow(const int row) {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(theSize_.y));
    return rowAddressTable_[row];
  }

  /* return genericMatrix-row as a const vector.
     This method works fast, since it returns a reference to the row vector.
     The data will NOT be copied.
     @param row the row to be accessed
     @return a const reference to the vector row
  */
  template<typename T>
  inline const genericVector<T>& 
  genericMatrix<T>::getRow(const int row) const {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(theSize_.y));
    return rowAddressTable_[row];
  }


  // alias for getRow()
  template<typename T>
  inline genericVector<T>& genericMatrix<T>::operator[](const int row) {
    return getRow(row);
  }

  // alias for getRow()
  template<typename T>
  inline const genericVector<T>& 
  genericMatrix<T>::operator[](const int row) const{
    return getRow(row);
  }

  /* copy a row vector in the given parameter This method copies the
     data of a given row of the genericMatrix in the given vector.
     
     @param row the number of the row to be copied
     @param theRow the vector, where the data will be copied
     @see getRow()
  */
  template<typename T>
  inline void genericMatrix<T>::getRowCopy(const int row,
                                           genericVector<T>& theRow) const {
    theRow.copy(getRow(row));
  }

  /* return genericMatrix-row as a vector.
     This method copies the data of the genericMatrix, therefore is not as
     fast as getRow()
     @param row the number of tthe row to be copied
     @return a vector with the contents of the row of the genericMatrix
  */
  template<typename T>
  inline genericVector<T> genericMatrix<T>::getRowCopy(const int row) const {
    return getRow(row);
  }

  /* return genericMatrix-column as a vector.
     This method copies the data of the genericMatrix, therefore is not as
     fast as getRow()
     @param col the number of the column to be copied
     @return a vector with the contents of the column of the genericMatrix
  */
  template<typename T>
  inline genericVector<T> genericMatrix<T>::getColumnCopy(const int col) const{
    assert(static_cast<unsigned int>(col) <
           static_cast<unsigned int>(theSize_.x));
    genericVector<T> tmp;
    getColumnCopy(col,tmp);
    return tmp;
  }

  /* return diagonal as a vector.
     This method copies the data of the genericMatrix.
     @return a vector with the contents of the diagonal of the genericMatrix
  */
  template<typename T>
  inline genericVector<T> genericMatrix<T>::getDiagonal() const {
    genericVector<T> tmp;
    getDiagonal(tmp);
    return tmp;
  }

  /* copy the data of a vector in a given row
     @param row the row that receives the data.
     @param theRow the vector with the data to be copied
  */
  template<typename T>
  inline void genericMatrix<T>::setRow(const int row,
                                       const genericVector<T>& theRow) {
    getRow(row).copy(theRow);
  }

  /* assigment operator (alias for copy(other)).
     @param other the genericMatrix to be copied
     @return a reference to the actual vector
  */
  template<typename T>
  inline genericMatrix<T>& 
  genericMatrix<T>::operator=(const genericMatrix<T>& other) {
    return copy(other);
  }

  // copy an external genericMatrix
  template <typename T>
  inline genericMatrix<T>& 
  genericMatrix<T>::copy(const genericMatrix<T>& other,
                         const ipoint& from,
                         const ipoint& to) {
    return copy(other,from.y,from.x,to.y,to.x);
  }

  template<typename T>
  inline genericMatrix<T>& 
  genericMatrix<T>::copy(const genericMatrix<T>& other,
                         const irectangle& window) {
    return copy(other,window.ul.y,window.ul.x,window.br.y,window.br.x);
  }

  template<typename T>
  inline bool 
  genericMatrix<T>::operator==(const genericMatrix<T>& other) const {
    return equals(other);
  }

  /*
   * FILL Members
   */
  template<typename T>
  inline void genericMatrix<T>::fill(const T& iniValue,
                                     const ipoint& from,
                                     const ipoint& to) {
    fill(iniValue,from.y,from.x,to.y,to.x);
  }

  template<typename T>
  inline void genericMatrix<T>::fill(const T& iniValue,
                                     const irectangle& window) {
    fill(iniValue,window.ul.y,window.ul.x,window.br.y,window.br.x);
  }

  template<typename T>
  inline void genericMatrix<T>::fill(const T _data[],
                                     const ipoint& from,
                                     const ipoint& to) {
    fill(_data,from.y,from.x,to.y,to.x);
  }

  template<typename T>
  inline void genericMatrix<T>::fill(const T _data[],const irectangle& window) {
    fill(_data,window.ul.y,window.ul.x,window.br.y,window.br.x);
  }

  template<typename T>
  inline void genericMatrix<T>::fill(const genericMatrix<T>& mat,
                                     const ipoint& from,
                                     const ipoint& to,
                                     const ipoint& startAt) {
    fill(mat,from.y,from.x,to.y,to.x,startAt.y,startAt.x);
  }

  template<typename T>
  inline void genericMatrix<T>::fill(const genericMatrix<T>& mat,
                                     const irectangle& window,
                                     const ipoint& start) {
    fill(mat,window.ul.y,window.ul.x,window.br.y,window.br.x,start.y,start.x);
  }
}


#endif

