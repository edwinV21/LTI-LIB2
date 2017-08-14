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
 * \file   ltiMatrix_inline.h
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiMatrix_inline.h,v 1.11 2012-11-17 14:49:05 alvarado Exp $
 */

#ifndef _LTI_MATRIX_INLINE_H_
#define _LTI_MATRIX_INLINE_H_

namespace lti {

  /* return matrix-row as a vector.
      This method works fast, since it returns a reference to the row vector.
      The data will NOT be copied.
      @param row the row to be accessed
      @return a reference to the vector row
  */
  template<typename T>
  inline vector<T>& matrix<T>::getRow(const int row) {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(this->theSize_.y));
    return static_cast<vector<T>*>(this->rowAddressTable_)[row];
  }

  /* return matrix-row as a const vector.
      This method works fast, since it returns a reference to the row vector.
      The data will NOT be copied.
      @param row the row to be accessed
      @return a const reference to the vector row
  */
  template<typename T>
  inline const vector<T>& matrix<T>::getRow(const int row) const {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(this->theSize_.y));
    return static_cast<vector<T>*>(this->rowAddressTable_)[row];
  }


  // alias for getRow()
  template<typename T>
  inline vector<T>& matrix<T>::operator[](const int row) {
    return getRow(row);
  }

  // alias for getRow()
  template<typename T>
  inline const vector<T>& matrix<T>::operator[](const int row) const{
    return getRow(row);
  }

  /* return matrix-row as a vector.
     This method copies the data of the matrix, therefore is not as
     fast as getRow()
     @param row the number of tthe row to be copied
     @return a vector with the contents of the row of the matrix
  */
  template<typename T>
  inline vector<T> matrix<T>::getRowCopy(const int row) const {
    return getRow(row);
  }

  template<typename T>
  inline void matrix<T>::getRowCopy(const int row,
				    vector<T>& theRow) const {
    genericMatrix<T>::getRowCopy(row,theRow);
  }


  /* return matrix-column as a vector.
     This method copies the data of the matrix, therefore is not as
     fast as getRow()
     @param col the number of the column to be copied
     @return a vector with the contents of the column of the matrix
  */
  template<typename T>
  inline vector<T> matrix<T>::getColumnCopy(const int col) const {
    assert(static_cast<unsigned int>(col) <
           static_cast<unsigned int>(this->theSize_.x));
    vector<T> tmp;
    getColumnCopy(col,tmp);
    return tmp;
  }

  template<typename T>
  inline void matrix<T>::getColumnCopy(const int col,
				       vector<T>& theCol) const {
    genericMatrix<T>::getColumnCopy(col,theCol);
  }

  /* return diagonal as a vector.
     This method copies the data of the matrix.
     @return a vector with the contents of the diagonal of the matrix
  */
  template<typename T>
  inline vector<T> matrix<T>::getDiagonal() const {
    vector<T> tmp;
    getDiagonal(tmp);
    return tmp;
  }

  /* return diagonal as a vector.
     This method copies the data of the matrix.
     @return a vector with the contents of the diagonal of the matrix
  */
  template<typename T>
  inline void matrix<T>::getDiagonal(vector<T>& diag) const {
    genericMatrix<T>::getDiagonal(diag);
  }

  // copy an external matrix
  template <typename T>
  inline matrix<T>& matrix<T>::copy(const genericMatrix<T>& other) {
    genericMatrix<T>::copy(other);
    return *this;
  }

  // copy an external matrix
  template <typename T>
  inline matrix<T>& matrix<T>::copy(const genericMatrix<T>& other,
                                    const int fromRow,
                                    const int fromCol,
                                    const int toRow,
                                    const int toCol) {
    genericMatrix<T>::copy(other,fromRow,fromCol,toRow,toCol);
    return (*this);
  }

  // copy an external matrix
  template <typename T>
  inline matrix<T>& matrix<T>::copy(const genericMatrix<T>& other,
                                    const ipoint& from,
                                    const ipoint& to) {
    genericMatrix<T>::copy(other,from,to);
    return (*this);
  }


  // copy an external matrix
  template <typename T>
  inline matrix<T>& matrix<T>::copy(const genericMatrix<T>& other,
                                    const vector<int>& idx,
                                    bool rowWise) {
    genericMatrix<T>::copy(other,idx,rowWise);
    return (*this);
  }

  /* assigment operator (alias for copy(other)).
     @param other the matrix to be copied
     @return a reference to the actual vector
  */
  template<typename T>
  inline matrix<T>& matrix<T>::operator=(const genericMatrix<T>& other) {
    return copy(other);
  }

  template<typename T>
  inline matrix<T>& matrix<T>::copy(const genericMatrix<T>& other,
                                    const irectangle& window) {
    return copy(other,window.ul,window.br);
  }

  template<typename T>
  inline matrix<T>& matrix<T>::operator+=(const T value) {
    return add(value);
  }

  template<typename T>
  inline matrix<T>& matrix<T>::operator+=(const genericMatrix<T>& other) {
    return add(other);
  }

  template<typename T>
  inline matrix<T>& matrix<T>::operator-=(const T value) {
    return subtract(value);
  }

  template<typename T>
  inline matrix<T>& matrix<T>::operator-=(const genericMatrix<T>& other) {
    return subtract(other);
  }
  
  template<typename T>
  inline matrix<T>& matrix<T>::operator*=(const genericMatrix<T>& other) {
    return multiply(other);
  }

  template<typename T>
  inline matrix<T>& matrix<T>::operator*=(const T value) {
    return multiply(value);
  }

  template<typename T>
  inline matrix<T> matrix<T>::operator*(const genericMatrix<T>& other) const {
    matrix<T> tmp;
    tmp.multiply(*this,other);
    return tmp;
  }

  template<typename T>
  inline matrix<T>& matrix<T>::operator/=(const T value) {
    return divide(value);
  }

  template <typename T>
  template <typename U>
  matrix<T>& matrix<T>::castFrom(const genericMatrix<U>& other) {
    this->allocate(other.rows(),other.columns());
    int y;
    for (y=0;y<this->rows();y++) {
      this->getRow(y).castFrom(other.getRow(y));
    }
    
    return (*this);
  }

  // castFrom uses copy if matrices have same type
  template <typename T>
  inline matrix<T>& matrix<T>::castFrom(const genericMatrix<T>& other) {
    return copy(other);
  }

  // transpose matrix
  template <typename T>
  template <typename U>
  matrix<T>& matrix<T>::transpose(const genericMatrix<U>& other) {
    const int m = other.rows();
    const int n = other.columns();
    // resizes this matrix
    // it should have m columns and n rows
    if ((this->rows() != n) || (this->columns() != m)) {
      this->allocate(n,m);
    }

    if ((other.getMode()==genericMatrix<U>::Connected) &&
        (this->getMode()==genericMatrix<T>::Connected)) {
      
//       iterator itr = this->begin();
//       iterator itc;
//       const_iterator itoc,itor,itore;
//       // end of first row
//       iterator itre = itr+m;
//       for (itoc=other.begin();
//            itr!=itre; //i<m
//            ++itr,itoc+=n) {
//         // end of row of other 
//         itore = itoc+n;
//         for (itc=itr,itor=itoc;
//              itor!=itore; //j<n
//              itc+=m,++itor) {
//           (*itc) = (*itor);
//         }
//       }
      
      // NOTE
      // The code below is about 10% faster for m>n than the previous
      // implementation above while losing about 1% in the opposite case.
      // Like this the performance difference between the two is not as
      // big as before: P4 2.8 gcc3.4.4:  2260musec and 2010 musec on 
      // 640x400 and 400x640, respectively.
      // Switching the processing order for the slower case does *NOT*
      // lead to faster transpose - on the contrary.

      // iterates complete other row-wise
      typename matrix<U>::const_iterator itor=other.begin();
      // starter for each col of this
      iterator itr=this->begin();
      // stop when starters are leaving first row
      const iterator itre=itr+m;
      // iterates each col of this
      iterator itc;
      // end of current col of this
      iterator itce=this->end();
      for (; itr!=itre; itr++, itce++) {
        itc=itr;
        for (; itc!=itce; itor++, itc+=m) {
          (*itc)=static_cast<T>(*itor);
        }
      }

    } else {

      int i,j;
      for(i=0;i<this->columns();i++) {
        for(j=0;j<this->rows();j++) {
          this->at(j,i)=static_cast<T>(other.at(i,j));
        }
      }
    }

    return (*this);
  }


}


#endif

