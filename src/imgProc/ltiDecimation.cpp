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
 * \file   ltiDecimation.cpp
 * \author Pablo Alvarado
 * \date   6.12.2000
 *
 * $Id: ltiDecimation.cpp,v 1.4 2006-05-16 11:59:36 doerfler Exp $
 */

#include "ltiDecimation.h"

namespace lti {
  // --------------------------------------------------
  // decimation::parameters
  // --------------------------------------------------

  // default constructor
  decimation::parameters::parameters()
    : functor::parameters() {
    factor = ipoint(2,2);
  }

  // copy constructor
  decimation::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  decimation::parameters::~parameters() {
  }

  // get type name
  const std::string& decimation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  // copy member
  
  decimation::parameters&
  decimation::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    factor = other.factor;
    return *this;
  }
  
  // alias for copy member
  decimation::parameters&
    decimation::parameters::operator=(const parameters& other) {
    return copy(other);
  }


  // clone member
  decimation::parameters* decimation::parameters::clone() const {
    return new parameters(*this);
  }

  decimation::parameters* decimation::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool decimation::parameters::write(ioHandler& handler,
                                     const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"factor",factor);
    }

    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool decimation::parameters::read(ioHandler& handler,
                                    const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"factor",factor);
    }

    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // decimation
  // --------------------------------------------------

  // default constructor
  decimation::decimation()
    : functor(){
    parameters par;
    setParameters(par);
  }

  // default constructor
  decimation::decimation(const ipoint& factor)
    : functor(){
    parameters par;
    par.factor = factor;
    setParameters(par);
  }


  // copy constructor
  decimation::decimation(const decimation& other)
    : functor()  {
    copy(other);
  }

  // destructor
  decimation::~decimation() {
  }

  // returns the name of this type
  const std::string& decimation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  decimation& decimation::copy(const decimation& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  decimation* decimation::clone() const {
    return new decimation(*this);
  }

  decimation* decimation::newInstance() const {
    return new decimation;
  }

  // return parameters
  const decimation::parameters&
    decimation::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}
