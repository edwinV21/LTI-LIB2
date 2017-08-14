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
 * \file   ltiColorNormalization.cpp
 *         Contains the abstract base class colorNormalization.
 * \author Pablo Alvarado
 * \date   30.03.2003
 *
 * $Id: ltiColorNormalization.cpp,v 1.1 2005-06-23 12:39:14 doerfler Exp $
 */

#include "ltiColorNormalization.h"

namespace lti {
  // --------------------------------------------------
  // colorNormalization::parameters
  // --------------------------------------------------

  // default constructor
  colorNormalization::parameters::parameters()
    : functor::parameters() {
  }

  // copy constructor
  colorNormalization::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  colorNormalization::parameters::~parameters() {
  }

  // copy member
  colorNormalization::parameters&
    colorNormalization::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    return *this;
  }

  const std::string& colorNormalization::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  colorNormalization::parameters* 
  colorNormalization::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  colorNormalization::parameters* 
  colorNormalization::parameters::newInstance() const {
    return new parameters();
  }

  //write
  bool colorNormalization::parameters::write(ioHandler& handler,
                                             const bool complete) const {
    bool b = true;

    b = b && functor::parameters::write(handler,complete);

    return b;
  }

  //read
  bool colorNormalization::parameters::read(ioHandler& handler,
                                            const bool complete) {
    bool b = true;
    b = b && functor::parameters::read(handler,complete);

    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  // --------------------------------------------------
  // colorNormalization
  // --------------------------------------------------

  // default constructor
  colorNormalization::colorNormalization()
    : functor() {
  }

  // copy constructor
  colorNormalization::colorNormalization(const colorNormalization& other)
    : functor()  {
    copy(other);
  }

  // destructor
  colorNormalization::~colorNormalization() {
  }

  // copy member
  colorNormalization&
  colorNormalization::copy(const colorNormalization& other) {
    functor::copy(other);
    return (*this);
  }
  
  const std::string& colorNormalization::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  const colorNormalization::parameters&
    colorNormalization::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}




