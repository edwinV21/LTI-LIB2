/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiPyramid_template.h
 *         Contains the container pyramid, which is a pyramid of matrices
 * \author Pablo Alvarado
 * \date   30.10.2000
 *
 * $Id: ltiPyramid_template.h,v 1.4 2005-03-16 16:20:50 doerfler Exp $
 */


namespace lti {

  /*
   * default constructor creates an empty pyramid;
   */
  template <class T>
  pyramid<T>::pyramid()
    : container(), levels_(0) {
  }

  /*
   * create a pyramid of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the pyramid
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the pyramid.
   * @param cells the number of cells per dimension.
   */
  template <class T>
  pyramid<T>::pyramid(const int levels)
    : container(), levels_(levels) {
    resize(levels_,false);
  }

  /*
   * create this pyramid as a copy of another pyramid
   * @param other the pyramid to be copied.
   */
  template <class T>
  pyramid<T>::pyramid(const pyramid<T>& other)
    : container() {
    copy(other);
  }

  /*
   * destructor
   */
  template <class T>
  pyramid<T>::~pyramid() {
    thePyramid_.clear();
  }

  // return classname
  template <class T>
  const std::string& pyramid<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // resize
  template <class T>
  void pyramid<T>::resize(const int levels,
                          const bool copyData) {
    if (!copyData) {
      thePyramid_.resize(0);
    }

    levels_ = levels;
    thePyramid_.resize(levels);
  }

  template <class T>
  void pyramid<T>::clear() {
    resize(0);
  }

  /**
   * append one element to the pyramid
   *
   * @param theElement Element that will be appended to the pyramid
   */
  template <class T>
  void pyramid<T>::append(const T& theElement) {

    thePyramid_.push_back(theElement);
    levels_ = size();
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source pyramid to be copied.
   * @return a reference to this object
   */
  template <class T>
  pyramid<T>& pyramid<T>::copy(const pyramid<T>& other) {

    thePyramid_ = other.thePyramid_;
    levels_ = other.levels_;

    return *this;
  }

  // clone member function
  template <class T>
  pyramid<T>* pyramid<T>::clone() const {
    return new pyramid(*this);
  }

  // newInstance member function
  template <class T>
  pyramid<T>* pyramid<T>::newInstance() const {
    return new pyramid(*this);
  }

  /*
   * compare this pyramid with another one.
   *
   * @param other the other pyramid to be compared with
   * @return true if both pyramids have the same elements and same size
   */
  template <class T>
  bool pyramid<T>::equals(const pyramid& other) const {

    bool result = (size() == other.size());
    const_iterator it,it2;

    for (it=begin(),it2=other.begin();
         result && (it!=end());
         ++it,++it2) {
      result = result && ((*it)==(*it2));
    }

    return (result);
  }

  /*
   * compare this pyramid with another one.
   *
   * @param other the other pyramid to be compared with
   * @return true if both pyramids have the same number of levels and same size
   */
  template <class T>
  bool pyramid<T>::sameResolutions(const pyramid& other) const {

    bool result = (size() == other.size());
    const_iterator it,it2;

    for (it=begin(),it2=other.begin();
         result && (it!=end());
         ++it,++it2) {
      result = result && ((*it).size()==(*it2).size());
    }

    return (result);
  }

}


