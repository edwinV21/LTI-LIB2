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
 * \file   ltiKernel2D_template.h
 *         Contains the template class lti::kernel2D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiKernel2D_template.h,v 1.15 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiRectangle.h"

namespace lti {
  // --------------------------------------------------
  // Two Dimensional Kernel
  // --------------------------------------------------

  // outer product of two 1D kernels
  template <typename T>
  kernel2D<T>& kernel2D<T>::outerProduct(const kernel1D<T>& k1,
					 const kernel1D<T>& k2) {
    matrix<T>::outerProduct(static_cast<const vector<T> >(k1),
			    static_cast<const vector<T> >(k2));


    offset_.y = k1.getOffset();
    offset_.x = k2.getOffset();

    matrix<T>::divide(k2.getNorm());

    norm_ = k1.getNorm();
    return (*this);
  }

  // specialization for unsigned bytes
  template <>
  kernel2D<ubyte>& kernel2D<ubyte>::outerProduct(const kernel1D<ubyte>& k1,
                                                 const kernel1D<ubyte>& k2) {

    kernel1D<int> ik1;
    kernel1D<int> ik2;
    matrix<int> mk;

    const int minNorm = min(k1.getNorm(),k2.getNorm());
    const int maxNorm = max(k1.getNorm(),k2.getNorm());

    ik1.castFrom(k1);
    ik2.castFrom(k2);

    mk.outerProduct(static_cast<const vector<int> >(ik1),
                    static_cast<const vector<int> >(ik2));

    mk.divide(minNorm);
    matrix<ubyte>::castFrom(mk);

    offset_.y = k1.getOffset();
    offset_.x = k2.getOffset();

    norm_ = maxNorm;
    return (*this);
  }

  // constructor
  template<typename T>
  kernel2D<T>::kernel2D() : matrix<T>() {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // copy constructor
  template<typename T>
  kernel2D<T>::kernel2D(const kernel2D<T>& other) : matrix<T>() {
    copy(other);
  }

  // constructor
  template<typename T>
  kernel2D<T>::kernel2D(const int theRows, const int theColumns,
                        const T& iniValue)
    : matrix<T>(theRows,theColumns,iniValue) {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<typename T>
  kernel2D<T>::kernel2D(const int fromRow,const int fromCol,
                        const int toRow,const int toCol,
                        const T& iniValue)
    : matrix<T>(toRow-fromRow+1,toCol-fromCol+1,iniValue) {
    offset_ = ipoint(-fromCol,-fromRow);
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<typename T>
  kernel2D<T>::kernel2D(const int theRows, const int theColumns)
    : matrix<T>(theRows,theColumns) {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<typename T>
  kernel2D<T>::kernel2D(const int fromRow,const int fromCol,
                        const int toRow,const int toCol)
    : matrix<T>(toRow-fromRow+1,toCol-fromCol+1) {
    offset_ = ipoint(-fromCol,-fromRow);
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // destructor
  template<typename T>
  kernel2D<T>::~kernel2D() {
  }

  // copy
  template<typename T>
  kernel2D<T>& kernel2D<T>::copy(const kernel2D<T>& other) {
    matrix<T>::copy(other);
    offset_ = other.getOffset();
    norm_ = other.getNorm();
    return *this;
  }

  // clone
  template<typename T>
  const std::string& kernel2D<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template<typename T>
  kernel2D<T>* kernel2D<T>::clone() const {
    return new kernel2D<T>(*this);
  }

  // new instance
  template<typename T>
  kernel2D<T>* kernel2D<T>::newInstance() const {
    return new kernel2D<T>();
  }

  // get Offset
  template<typename T>
  const ipoint& kernel2D<T>::getOffset() const {
    return offset_;
  }

  // set Offset
  template<typename T>
  void kernel2D<T>::setOffset(const ipoint& p) {
    offset_ = p;
  }

  template<typename T>
  void kernel2D<T>::resize(const int fromRow,const int fromCol,
                           const int toRow,const int toCol,
                           const T& iniValue,
                           const eResizeType& resizeType) {
    matrix<T>::resize(toRow-fromRow+1,toCol-fromCol+1,
                      iniValue,resizeType);
    offset_ = ipoint(-fromCol,-fromRow);
  }

  // denormalize
  template<typename T>
  void kernel2D<T>::denormalize() {
    if (norm_ == T(1)) {
      return; // nothing to be done (already normalized!)
    }

    this->divide(norm_);
    norm_ = T(1);
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
  kernel2D<T>& kernel2D<T>::castFrom(const matrix<T>& other,
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
  kernel2D<T>& kernel2D<T>::castFrom(const matrix<T>& other,
                                     const ipoint& firstElement) {
    return castFrom(other,firstElement.y,firstElement.x);
  }

  template <typename T>
  void kernel2D<T>::fill(const matrix<T>& mat,
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
  void kernel2D<T>::fill(const T& value,
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
   * @param _data    the data that should fill this kernel
   * @param fromRow  first row of the submatrix to be filled
   * @param fromCol  first column of the submatrix to be filled
   * @param toRow    last row of the submatrix to be filled
   * @param toCol    last column of the submatrix to be filled
   */
  template <typename T>
  void kernel2D<T>::fill(const T* _data,
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

  
  template<typename T>
  kernel2D<T>& kernel2D<T>::castFrom(const separableKernel<T>& other) {
 
    this->clear();

    if (other.getNumberOfPairs()<1) {
      return *this;
    }

    int i,j;
    // find out filter dimensions
    irectangle tmpSize;
    tmpSize.ul.y = other.getColFilter(0).firstIdx();
    tmpSize.br.y = other.getColFilter(0).lastIdx();

    tmpSize.ul.x = other.getRowFilter(0).firstIdx();
    tmpSize.br.x = other.getRowFilter(0).lastIdx();

    for (i=1;i<other.getNumberOfPairs();i++) {
      tmpSize.ul.y = min(tmpSize.ul.x,other.getColFilter(i).firstIdx());
      tmpSize.br.y = max(tmpSize.br.x,other.getColFilter(i).lastIdx());

      tmpSize.ul.x = min(tmpSize.ul.y,other.getRowFilter(i).firstIdx());
      tmpSize.br.x = max(tmpSize.br.y,other.getRowFilter(i).lastIdx());
    }

    // resize the kernel and initialize it with zeros.
    assign(tmpSize.ul.y,tmpSize.ul.x,tmpSize.br.y,tmpSize.br.x,T(0));

    // add all kernels
    vector<T> tmpRow(tmpSize.br.x-tmpSize.ul.x+1,T(0));

    for (i=0;i<other.getNumberOfPairs();i++) {
      tmpRow.fill(static_cast<vector<T> >(other.getRowFilter(i)),
                  other.getRowFilter(i).firstIdx()-tmpSize.ul.x);
      for (j=other.getColFilter(i).firstIdx();
           j<=other.getColFilter(i).lastIdx();
           j++) {
        this->getRow(j+offset_.y).addScaled(other.getColFilter(i).at(j),tmpRow);
      }
      tmpRow.fill(T(0));
    }

    return (*this);
  } 


  // mirror other kernel
  template <typename T>
  kernel2D<T>& kernel2D<T>::mirror(const kernel2D<T>& other) {
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

    norm_ = other.getNorm();

    return (*this);
  }

  // mirror
  template <typename T>
  kernel2D<T>& kernel2D<T>::mirror() {
    kernel2D<T> tmp;
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
  bool kernel2D<T>::write(ioHandler& handler,
                          const bool complete) const {

    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"norm",getNorm());
      lti::write(handler,"offset",getOffset());
      matrix<T>::write(handler,false);
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
  bool kernel2D<T>::read(ioHandler& handler,const bool complete) {
    T norm;
    ipoint offset;

    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"norm",norm);
      lti::read(handler,"offset",offset);
      matrix<T>::read(handler,false);
      setNorm(norm);
      setOffset(offset);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }
} //namespace
