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
 * \file   ltiDistanceFunctor_template.h
 *         This file contains the class distanceFunctor, the base
 *         class for calculation of distances between two vectors or
 *         matrices.
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiDistanceFunctor_template.h,v 1.3 2005-06-22 15:26:41 doerfler Exp $
 */


namespace lti {
  // --------------------------------------------------
  // distanceFunctor::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T>
  distanceFunctor<T>::parameters::parameters() 
    : functor::parameters() {
    rowWise=true;
  }

  // copy constructor
  template <typename T>
  distanceFunctor<T>::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }

  // destructor
  template <typename T>
  distanceFunctor<T>::parameters::~parameters() { 
  }

  // copy
  template <typename T>
  typename distanceFunctor<T>::parameters&
  distanceFunctor<T>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    rowWise=other.rowWise;
    
    return *this;
  }

  // class name
  template<class T>
  const std::string& distanceFunctor<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template <typename T>
  typename distanceFunctor<T>::parameters*
  distanceFunctor<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance
  template <typename T>
  typename distanceFunctor<T>::parameters*
  distanceFunctor<T>::parameters::newInstance() const {
    return new parameters();
  }

  // write
  template <typename T>
  bool distanceFunctor<T>::parameters::write(ioHandler& handler,
                                             const bool complete) const {
    bool b(true);
    
    if (complete) {
      b = b && handler.writeBegin();
    }
    
    b = b && lti::write(handler, "rowWise", rowWise);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  // read
  template <typename T>
  bool distanceFunctor<T>::parameters::read(ioHandler& handler,
                                            const bool complete) {
    bool b(true);
    
    if (complete) {
      b = b && handler.readBegin();
    }
    
    b = b && lti::read(handler, "rowWise", rowWise);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  
  // --------------------------------------------------
  // distanceFunctor
  // --------------------------------------------------

  // default constructor
  template <typename T>
  distanceFunctor<T>::distanceFunctor()
    : functor(){

    parameters param;
    setParameters(param);
  }

  // copy constructor
  template <typename T>
  distanceFunctor<T>::distanceFunctor(const distanceFunctor<T>& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <typename T>
  distanceFunctor<T>::~distanceFunctor() {
  }

  // copy member
  template <typename T> distanceFunctor<T>&
  distanceFunctor<T>::copy(const distanceFunctor<T>& other) {
    functor::copy(other);
    return (*this);
  }

  // class name
  template<class T>
  const std::string& distanceFunctor<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  template <typename T>
  const typename distanceFunctor<T>::parameters&
  distanceFunctor<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // shortcut function
  template <typename T>
  T distanceFunctor<T>::distance(const vector<T>& a,
                                 const vector<T>& b) const {
    T tmp;
    apply(a,b,tmp);
    return tmp;
  }

  // shortcut function
  template <typename T>
  T distanceFunctor<T>::distance(const matrix<T>& a,
                                 const matrix<T>& b) const {
    T tmp;
    apply(a,b,tmp);
    return tmp;
  }

}


