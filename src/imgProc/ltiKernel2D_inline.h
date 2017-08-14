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

/** 
 * \file   ltiKernel2D_inline.h
 *         Contains the template class lti::kernel2D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiKernel2D_inline.h,v 1.4 2012-09-16 05:22:59 alvarado Exp $
 */


namespace lti {

  template <typename T>
  inline void kernel2D<T>::allocate(const int fromRow, const int fromCol,
                                    const int toRow, const int toCol) {
    resize(fromRow, fromCol, toRow, toCol, T(), AllocateOnly);
  }

  template <typename T>
  inline void kernel2D<T>::assign(const int fromRow, const int fromCol,
                                  const int toRow, const int toCol,
                                  const T& initValue) {
    resize(fromRow, fromCol, toRow, toCol, initValue, Init);
  }

  template <typename T>
  inline const T& kernel2D<T>::getNorm() const {
    return norm_;
  }
  
  template <typename T>
  inline void kernel2D<T>::setNorm(const T& n) {
    norm_=n;
  }

  template <typename T>
  inline int kernel2D<T>::firstRow() const {
    return -offset_.y;
  }
  
  template <typename T>
  inline int kernel2D<T>::lastRow() const {
    return this->rows()-this->offset_.y-1;
  }
  
  template <typename T>
  inline int kernel2D<T>::firstColumn() const {
    return -this->offset_.x;
  }

  template <typename T>
  inline int kernel2D<T>::lastColumn() const {
    return this->columns()-this->offset_.x-1;
  }

  template <typename T>
  inline T& kernel2D<T>::at(const ipoint& p) {
    return matrix<T>::at(p+this->offset_);
  }

  template <typename T>
  inline const T& kernel2D<T>::at(const ipoint& p) const {
    return matrix<T>::at(p+offset_);
  }

  template <typename T>
  inline T& kernel2D<T>::at(const int y,const int x) {
    return matrix<T>::at(y+offset_.y,x+offset_.x);
  }
 
  template <typename T>
  inline const T& kernel2D<T>::at(const int y,const int x) const {
    return matrix<T>::at(y+offset_.y,x+offset_.x);
  }

  template <typename T>
  inline kernel2D<T>& kernel2D<T>::operator=(const kernel2D<T>& other) {
    return copy(other);
  }
}
