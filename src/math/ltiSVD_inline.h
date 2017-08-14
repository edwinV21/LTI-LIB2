/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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

/*
 * \file   ltiSVD_inline.h
 *         Singular Value Decomposition
 * \author Xin Gu
 * \author Pablo Alvarado
 * \date   11.01.2001
 *
 * revisions ..: $Id: ltiSVD_inline.h,v 1.2 2008-05-07 15:40:08 alvarado Exp $
 */

namespace lti {
  
  /*
   * Assigns the contents of the other object to this object
   */
  inline svd::parameters& svd::parameters::operator=(const parameters& other) {
    copy(other);
    return *this;
  }
  
  /*
   * help method:(a^2+b^2)^0.5 without destructive underflow or overflow
   */
  template<typename T>
  inline T svd::helper<T>::pythag(const T a,const T b) const {
    const T tmp1(abs(a));
    const T tmp2(abs(b));

    if (tmp1 > tmp2) {
      return static_cast<T>(tmp1*sqrt(T(1)+sqr(tmp2/tmp1)));
    } else {
      return ((tmp1==T(0)) ? T(0) :
              static_cast<T>(tmp2*sqrt(T(1)+sqr(tmp1/tmp2))));
    }
  }

  /*
   * sign
   */
  template<typename T>
  inline T svd::helper<T>::sign(const T a,const T b) const {
    return (b >= T(0)) ? abs(a) : -abs(a);
  }

  /*
   * check if the given number is zero
   */
  template<typename T>
  inline bool svd::helper<T>::isZero(const T x) const {
    return (x == T(0)); 
  }

  /*
   * check if the given number is not zero
   */
  template<typename T>
  inline bool svd::helper<T>::notZero(const T x) const {
    return (abs(x) >= std::numeric_limits<T>::epsilon());
  }

  /*
   * Compute the dot product of a part of two matrix rows
   */
  template<typename T>
  inline T svd::helper<T>::dotOfRows(const matrix<T>& data,
                                     const int row1, 
                                     const int row2,
                                           int lowCol,
                                     const int highCol) const {

    T sum(0);
    
    const vector<T>& rtmp1=data.getRow(row1);
    const vector<T>& rtmp2=data.getRow(row2);
    
    typename vector<T>::const_iterator i1=rtmp1.begin()+lowCol;
    typename vector<T>::const_iterator i2=rtmp2.begin()+lowCol;
    
    const int hc = min(highCol,data.lastColumn());
    
    while (lowCol++ <= hc) {
      sum+=(*i1++)*(*i2++);
    }
    
    return sum;
  }
  
  /*
   * Compute the dot product of a part of two matrix columns
   */
  template<typename T>
  inline T svd::helper<T>::dotOfColumns(const matrix<T>& data,
                                        const int col1, 
                                        const int col2,
                                              int lowRow,
                                        const int highRow) const {
    T sum(0);
    const int hr=min(highRow,data.lastRow());
    while (lowRow <= hr) {
      sum+=data.at(lowRow,col1)*data.at(lowRow,col2);
      lowRow++;
    }
    return sum;
  }


  /*
   * Compute the sum of a part of a matrix row
   */
  template<typename T>
  inline T svd::helper<T>::sumOfRowPart(const matrix<T>& data,
                                        const int row,
                                              int lowCol,
                                        const int highCol) const {
    T sum(0);
    const vector<T>& rtmp=data.getRow(row);
    const typename vector<T>::const_iterator ie=rtmp.end();
    
    typename vector<T>::const_iterator i=rtmp.begin()+lowCol;
    const int hc = min(highCol,data.lastColumn());
    while (lowCol++ <= hc && i != ie) {
      sum += *i++;
    }
    return sum;
  }

  /*
   * Compute the sum of a part of a matrix column
   */
  template<typename T>
  inline T svd::helper<T>::sumOfColumnPart(const matrix<T>& data,
                                           const int col,
                                                 int lowRow, 
                                           const int highRow) const {
    T sum(0);
    const int hr = min(highRow,data.lastRow());
    while (lowRow <= hr) {
      sum+=data.at(lowRow,col);
      lowRow++;
    }
    return sum;
  }


  /*
   * Compute the sum of the absolute value of a part of a matrix row
   */
  template<typename T>
  inline T svd::helper<T>::sumOfAbsRowPart(const matrix<T>& data,
                                           const int row,
                                                 int lowCol,
                                           const int highCol) const {
      T sum(0);
      const vector<T>& rtmp=data.getRow(row);

      typename vector<T>::const_iterator i=rtmp.begin()+lowCol;
      const int hc = min(highCol,data.lastColumn());
      while (lowCol <= hc) {
        sum+=abs(*i++);
        lowCol++;
      }
      return sum;
    }

  /*
   * Compute the sum of the absolute values of a part of a matrix column
   */
  template<typename T>
  inline T svd::helper<T>::sumOfAbsColumnPart(const matrix<T>& data,
                                              const int col,
                                                    int lowRow,
                                              const int highRow) const {
    T sum(0);
    const int hr=min(highRow,data.rows()-1);
    while (lowRow <= hr) {
      sum+=abs(data.at(lowRow,col));
      lowRow++;
    }
    return sum;
  }
  
  template<typename T>
  inline void svd::helper<T>::multiplyColumn(matrix<T>& data, 
                                             const int col,
                                             const T factor,
                                                   int lowRow,
                                             const int highRow) const {
    const int hr=min(highRow,data.rows()-1);
    for (int i=lowRow; i<=hr; i++) {
      data.at(lowRow++,col)*=factor;
    }
  }

  template<typename T>
  inline void svd::helper<T>::multiplyRow(matrix<T>& data,
                                          const int row,
                                          const T factor,
                                                int lowCol,
                                          const int highCol) const {
    vector<T>& rtmp=data.getRow(row);
    typename vector<T>::iterator i=rtmp.begin()+lowCol;
    const int hc = min(highCol,data.lastColumn());
    while (lowCol++ <= hc) {
      *i++*=factor;
    }
  }
  
  template<typename T>
  inline void svd::helper<T>::fillColumn(matrix<T>& data,
                                         const int col,
                                         const T value,
                                         int lowRow,
                                         const int highRow) const {
    const int hr=min(highRow,data.lastRow());
    for (int i=lowRow; i<=hr; i++) {
      data.at(lowRow++,col)=value;
    }
  }
  
  template<typename T>
  inline void svd::helper<T>::fillRow(matrix<T>& data,
                                      const int row,
                                      const T value,
                                      int lowCol,
                                      const int highCol) const {
    vector<T>& rtmp=data.getRow(row);
    typename vector<T>::iterator i=rtmp.begin()+lowCol;

    const int hc = min(highCol,data.lastColumn());
    while (lowCol++ <= hc) {
      *i++=value;
    }
  }
}
