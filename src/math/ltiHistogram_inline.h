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


/**
 * \file   ltiHistogram_inline.h
 * \author Pablo Alvarado
 * \date   29.06.00
 * 
 * $Id: ltiHistogram_inline.h,v 1.5 2006-09-15 17:30:24 doerfler Exp $
 */

namespace lti {
  template<class T>
  inline const T& histogram<T>::outerBoundsCell() {
    static const T val(0);
    return val;
  }

  /*
   * returns the number of dimensions of this histogram
   */
  template<class T>
  inline int histogram<T>::getDimensions() const {
    return dimensionality_;
  }

  /*
   * get the number of cells of the dimension <code>dim</code>
   * @param dimension the index of the dimension to be checked
   * @return the number of cells of the dimension specified by
   *         <code>dim</code>
   */
  template<class T>
  inline int histogram<T>::getCellsInDimension(const int dimension) const {
    return theCellsPerDimension_.at(dimension);
  }

  template<class T>
  inline const ivector& histogram<T>::getCellsPerDimension() const {
    return theCellsPerDimension_;
  }

  /*
   * returns a vector to the first element of the histogram
   * (usually every element of the vector is 0;
   */
  template<class T>
  inline const ivector& histogram<T>::getFirstCell() const {
    return firstCell_;
  }

  /*
   * returns a vector to the last element of the histogram
   */
  template<class T>
  inline const ivector& histogram<T>::getLastCell() const {
    return lastCell_;
  }

  /*
   * get the index of the biggest element in the histogram
   */
  template<class T>
  inline ivector histogram<T>::findIndexOfMaximum() const {
    return indexToVector(theHistogram_.findIndexOfMaximum());
  }

  /**
   * get the index of the smallest element in the histogram
   */
  template<class T>
  inline ivector histogram<T>::findIndexOfMinimum() const {
    return indexToVector(theHistogram_.findIndexOfMinimum());
  }

  /*
   * returns the number of entries registered by now
   * (i.e. how many put() 'till now?).
   */
  template<class T>
  inline const T& histogram<T>::getNumberOfEntries() const {
    return numberOfEntries_;
  }

  /*
   * compute the integer index for the data vector using the
   * given index vector.
   */
  template<class T>
  inline int histogram<T>::vectorToIndex(const ivector& x) const {
    int i=dimensionality_-1;
    assert ((x.at(i)>=0) && (x.at(i)<theCellsPerDimension_.at(i)));
    int idx = x.at(i);

    for (--i;i>=0;--i) {
      assert ((x.at(i)>=0) && (x.at(i)<theCellsPerDimension_.at(i)));
      idx*=theCellsPerDimension_.at(i);
      idx+=x.at(i);
    }

    return idx;
  }

  /*
   * compute the index vector for the data vector using the
   * given integer index.
   */
  template<class T>
  inline ivector histogram<T>::indexToVector(const int x) const {
    assert (x>=0 && x<theHistogram_.size());
    int i=0;
    int tmp=x;
    ivector idx(dimensionality_);

    for (; i<dimensionality_-1; ++i) {
      const int &sz=theCellsPerDimension_.at(i);
      idx.at(i)=tmp%sz;
      tmp/=sz;
    }
    idx.at(dimensionality_-1)=tmp;

    return idx;
  }
  
  template<class T>
  inline typename histogram<T>::const_iterator histogram<T>::begin() const {
    return theHistogram_.begin();  
  }
  
  template<class T>
  inline typename histogram<T>::iterator histogram<T>::begin() {
    return theHistogram_.begin();
  }

  template<class T>
  inline typename histogram<T>::const_iterator histogram<T>::end() const {
    return theHistogram_.end();
  }
  
  template<class T>
  inline typename histogram<T>::iterator histogram<T>::end() {
    return theHistogram_.end();
  }

  template<class T>
  inline histogram<T>& histogram<T>::operator=(const histogram<T>& other) {
    return copy(other);
  }
  
  template<class T>
  inline bool histogram<T>::operator==(const histogram<T>& other) const {
    return equals(other);
  }

  template<class T>
  inline histogram<T>& histogram<T>::operator+=(const histogram<T>& other) {
    return add(other);
  }

  template<class T>
  inline histogram<T>& histogram<T>::operator-=(const histogram<T>& other) {
    return subtract(other);
  }

  template<class T>
  inline histogram<T>& histogram<T>::operator*=(const T& cst) {
    return multiply(cst);
  }

  template<class T>
  inline histogram<T>& histogram<T>::operator+=(const T& cst) {
    return add(cst);
  }

  template<class T>
  inline T histogram<T>::minimum() const {
    return theHistogram_.findMinimum();
  }

  template<class T>
  inline T histogram<T>::maximum() const {
    return theHistogram_.findMaximum();
  }

} // namespace lti

