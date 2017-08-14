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
 * \file   ltiFixedGridInterpolation_template.h
 *         Contains the class fixedGridInterpolation.
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   03.05.2003
 *
 * $Id: ltiFixedGridInterpolation_template.h,v 1.5 2012-10-31 03:27:17 alvarado Exp $
 */

//TODO: include files

namespace lti {
  // --------------------------------------------------
  // fixedGridInterpolation<T>::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T>
  fixedGridInterpolation<T>::parameters::parameters()
    : functor::parameters() {
    
    boundaryType = lti::Zero;
    
  }

  template <typename T>
  fixedGridInterpolation<T>::parameters::parameters(const eBoundaryType btype)
    : functor::parameters() {
    boundaryType = btype;
  }

  // copy constructor
  template <typename T>
  fixedGridInterpolation<T>::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  template <typename T>
  fixedGridInterpolation<T>::parameters::~parameters() {
  }

  // copy member

  template <typename T>
  typename fixedGridInterpolation<T>::parameters&
  fixedGridInterpolation<T>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    boundaryType = other.boundaryType;

    return *this;
  }

  // alias for copy method
  template <typename T>
  typename fixedGridInterpolation<T>::parameters&
  fixedGridInterpolation<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  template <typename T>
  const std::string& 
  fixedGridInterpolation<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  template <typename T>
  typename fixedGridInterpolation<T>::parameters*
  fixedGridInterpolation<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template <typename T>
  typename fixedGridInterpolation<T>::parameters* 
  fixedGridInterpolation<T>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template <typename T>
  bool fixedGridInterpolation<T>::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  template <typename T>
  bool fixedGridInterpolation<T>::parameters::read(ioHandler& handler,
                                                const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // fixedGridInterpolation<T>
  // --------------------------------------------------

  // default constructor
  template <typename T>
  fixedGridInterpolation<T>::fixedGridInterpolation()
    : functor(), theVector_(0),theMatrix_(0) {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template <typename T>
  fixedGridInterpolation<T>::fixedGridInterpolation(const parameters& par)
    : functor(), theVector_(0),theMatrix_(0) {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <typename T>
  fixedGridInterpolation<T>::fixedGridInterpolation(const fixedGridInterpolation<T>& other)
    : functor(), theVector_(0),theMatrix_(0) {
    copy(other);
  }

  // destructor
  template <typename T>
  fixedGridInterpolation<T>::~fixedGridInterpolation() {
  }

  // copy member
  template <typename T>
  fixedGridInterpolation<T>& 
  fixedGridInterpolation<T>::copy(const fixedGridInterpolation<T>& other) {
    functor::copy(other);

    theVector_ = other.theVector_;
    theMatrix_ = other.theMatrix_;

    return (*this);
  }

  // alias for copy member
  template <typename T>
  fixedGridInterpolation<T>&
  fixedGridInterpolation<T>::operator=(const fixedGridInterpolation<T>& other) {
    return (copy(other));
  }

  // class name
  template <typename T>
  const std::string& fixedGridInterpolation<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  template <typename T>
  const typename fixedGridInterpolation<T>::parameters&
  fixedGridInterpolation<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // set boundary type
  template <typename T>
  bool fixedGridInterpolation<T>::
  setBoundaryType(const eBoundaryType boundaryType) {
    parameters *param = 
      dynamic_cast<parameters*>(&parametersManager::getRWParameters());
    if(param==0) {
      throw invalidParametersException(name());
    }
    param->boundaryType = boundaryType;
    return updateParameters();
  }

  
  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  template<class T>
  bool fixedGridInterpolation<T>::use(const vector<T>& vct) {
    theVector_ = &vct;
    return true;
  }

  template<class T>
  bool fixedGridInterpolation<T>::use(const matrix<T>& mat) {
    theMatrix_ = &mat;
    return true;
  }

  


}

