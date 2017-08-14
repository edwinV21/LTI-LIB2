/*
 * Copyright (C) 1998-2005
 * Peter Doerfler
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
 * \file   ltiBresenhamCircle.cpp
 * \author Peter Doerfler
 * \date   30.07.2005
 * 
 * $Id: ltiBresenhamCircle.cpp,v 1.2 2006-05-16 11:59:36 doerfler Exp $
 */

#include "ltiBresenhamCircle.h"

namespace lti {
  // --------------------------------------------------
  // bresenhamCircle::parameters
  // --------------------------------------------------

  // default constructor
  bresenhamCircle::parameters::parameters()
    : functor::parameters() {
    //TODO: Initialize your parameter values!
    // If you add more parameters manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new parameters
    // 3. make sure that the read and write members also read and
    //    write your parameters
    
  }

  // copy constructor
  bresenhamCircle::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  bresenhamCircle::parameters::~parameters() {
  }

  // copy member

  bresenhamCircle::parameters&
  bresenhamCircle::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    return *this;
  }

  // alias for copy method
  bresenhamCircle::parameters&
  bresenhamCircle::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& bresenhamCircle::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  bresenhamCircle::parameters* 
  bresenhamCircle::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  bresenhamCircle::parameters* 
  bresenhamCircle::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool bresenhamCircle::parameters::write(ioHandler& handler,
                                          const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
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
  bool bresenhamCircle::parameters::read(ioHandler& handler,
                                         const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // bresenhamCircle
  // --------------------------------------------------

  // default constructor
  bresenhamCircle::bresenhamCircle()
    : functor() {

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
  bresenhamCircle::bresenhamCircle(const parameters& par)
    : functor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  bresenhamCircle::bresenhamCircle(const bresenhamCircle& other)
    : functor() {
    copy(other);
  }

  // destructor
  bresenhamCircle::~bresenhamCircle() {
  }

  // copy member
  bresenhamCircle& 
  bresenhamCircle::copy(const bresenhamCircle& other) {
    functor::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  bresenhamCircle&
  bresenhamCircle::operator=(const bresenhamCircle& other) {
    return (copy(other));
  }

  // class name
  const std::string& bresenhamCircle::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  bresenhamCircle* bresenhamCircle::clone() const {
    return new bresenhamCircle(*this);
  }

  // create a new instance
  bresenhamCircle* bresenhamCircle::newInstance() const {
    return new bresenhamCircle();
  }

  // return parameters
  const bresenhamCircle::parameters&
  bresenhamCircle::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}
