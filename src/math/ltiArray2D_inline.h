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

/** 
 * \file   ltiArray2D_inline.h
 *         Contains the template class lti::array2D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiArray2D_inline.h,v 1.2 2012-09-16 05:22:59 alvarado Exp $
 */


namespace lti {

  template <typename T>
  inline void array2D<T>::allocate(const int fromRow, const int fromCol,
                                    const int toRow, const int toCol) {
    resize(fromRow, fromCol, toRow, toCol, T(), AllocateOnly);
  }

  template <typename T>
  inline void array2D<T>::assign(const int fromRow, const int fromCol,
                                  const int toRow, const int toCol,
                                  const T& initValue) {
    resize(fromRow, fromCol, toRow, toCol, initValue, Init);
  }

  template <typename T>
  inline int array2D<T>::firstRow() const {
    return -offset_.y;
  }
  
  template <typename T>
  inline int array2D<T>::lastRow() const {
    return this->rows()-this->offset_.y-1;
  }
  
  template <typename T>
  inline int array2D<T>::firstColumn() const {
    return -this->offset_.x;
  }

  template <typename T>
  inline int array2D<T>::lastColumn() const {
    return this->columns()-this->offset_.x-1;
  }

  template <typename T>
  inline T& array2D<T>::at(const ipoint& p) {
    return matrix<T>::at(p+this->offset_);
  }

  template <typename T>
  inline const T& array2D<T>::at(const ipoint& p) const {
    return matrix<T>::at(p+offset_);
  }

  template <typename T>
  inline T& array2D<T>::at(const int y,const int x) {
    return matrix<T>::at(y+offset_.y,x+offset_.x);
  }
 
  template <typename T>
  inline const T& array2D<T>::at(const int y,const int x) const {
    return matrix<T>::at(y+offset_.y,x+offset_.x);
  }

  template <typename T>
  inline array2D<T>& array2D<T>::operator=(const array2D<T>& other) {
    return copy(other);
  }
}
