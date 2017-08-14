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
 * \file   ltiPyramid_inline.h
 *         Contains the container pyramid, which is a pyramid of matrices
 * \author Pablo Alvarado
 * \date   30.10.2000
 *
 * $Id: ltiPyramid_inline.h,v 1.3 2005-03-16 16:20:49 doerfler Exp $
 */

namespace lti {

  /*
   * returns the number of dimensions of this pyramid
   */
  template <class T>
  inline typename pyramid<T>::size_type pyramid<T>::size() const {
    return thePyramid_.size();
  }

  /*
   * returns true if the size is zero
   */
  template <class T>
  inline bool pyramid<T>::empty() const {
    return thePyramid_.empty();
  }

  /*
   * read-only access to the element x of the pyramid
   * @param x index of the pyramid element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  template <class T>
  inline const T& pyramid<T>::at(const int x) const {
    return thePyramid_[x];
  }

  template <class T>
  inline const T& pyramid<T>::operator[](const int x) const {
    return at(x);
  }

  /*
   * access element x of the pyramid
   * @param x index of the pyramid element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  template <class T>
  inline T& pyramid<T>::at(const int x) {
    return thePyramid_[x];
  }

  template <class T>
  inline T& pyramid<T>::operator[](const int x) {
    return at(x);
  }

  /*
   * returns an iterator pointing to the first element.
   * Note that you can not change the values of the pyramid
   * elements when you use a const_iterator. See also begin()
   */
  template <class T>
  inline typename pyramid<T>::const_iterator pyramid<T>::begin() const {
    return thePyramid_.begin();
  }

  /*
   * returns an iterator pointing to the first element.
   * The use of the interators is similar to the iterators of the
   * Standard Template Library (STL).
   */
  template <class T>
  inline typename pyramid<T>::iterator pyramid<T>::begin() {
    return thePyramid_.begin();
  }

  /*
   * returns last index as a const iterator.
   * For an example see begin()
   */
  template <class T>
  inline typename pyramid<T>::const_iterator pyramid<T>::end() const {
    return thePyramid_.end();
  }

  /*
   * returns last index as an iterator
   * For an example see begin()
   */
  template <class T>
  inline typename pyramid<T>::iterator pyramid<T>::end() {
    return thePyramid_.end();
  }

  // alias for copy
  template <class T>
  inline bool pyramid<T>::operator==(const pyramid& other) const {
    return equals(other);
  }

  // assignment operator
  template <class T>
  inline pyramid<T>& pyramid<T>::operator=(const pyramid& other) {
    return copy(other);
  }



} // namespace lti

