/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiScramble.cpp
 *         Contains the class scramble,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   09.10.2007
 *
 * revisions ..: $Id: ltiScramble.cpp,v 1.2 2010-04-22 04:58:26 alvarado Exp $
 */

#include "ltiScramble.h"

namespace lti {
  // --------------------------------------------------
  // scramble::parameters
  // --------------------------------------------------

  // default constructor
  scramble::parameters::parameters()
    : functor::parameters() {
  }

  // copy constructor
  scramble::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  scramble::parameters::~parameters() {
  }

  // copy member

  scramble::parameters&
  scramble::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    randomParams.copy(other.randomParams);

    return *this;
  }

  // alias for copy method
  scramble::parameters&
  scramble::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& scramble::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  scramble::parameters*
  scramble::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  scramble::parameters*
  scramble::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool scramble::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"randomParams",randomParams);
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
  bool scramble::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"randomParams",randomParams);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // scramble
  // --------------------------------------------------

  // default constructor
  scramble::scramble()
    : functor(),max_(-1) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  scramble::scramble(const parameters& par)
    : functor(),max_(-1) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  scramble::scramble(const scramble& other)
    : functor(),max_(-1) {
    copy(other);
  }

  // destructor
  scramble::~scramble() {
  }

  // copy member
  scramble& 
  scramble::copy(const scramble& other) {
    functor::copy(other);
    max_=other.max_;

    return (*this);
  }

  // alias for copy member
  scramble&
  scramble::operator=(const scramble& other) {
    return (copy(other));
  }

  // class name
  const std::string& scramble::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  scramble* scramble::clone() const {
    return new scramble(*this);
  }

  // create a new instance
  scramble* scramble::newInstance() const {
    return new scramble();
  }

  // return parameters
  const scramble::parameters&
  scramble::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool scramble::updateParameters() {
    if (functor::updateParameters()) {
      return rnd_.setParameters(getParameters().randomParams);
    }

    return false;
  }

}

