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
 * \file   ltiGeometricTransform.cpp
 *         Contains the class geometricTransform,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   08.09.2007
 *
 * revisions ..: $Id: ltiGeometricTransform.cpp,v 1.2 2009-04-19 04:06:10 alvarado Exp $
 */

#include "ltiGeometricTransform.h"

namespace lti {
  // --------------------------------------------------
  // geometricTransformBase::parameters
  // --------------------------------------------------

  // default constructor
  geometricTransformBase::parameters::parameters()
    : functor::parameters() {
    resizeMode = KeepDimensions;
  }

  // copy constructor
  geometricTransformBase::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  geometricTransformBase::parameters::~parameters() {
  }

  // copy member

  geometricTransformBase::parameters&
  geometricTransformBase::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    resizeMode = other.resizeMode;

    return *this;
  }

  // alias for copy method
  geometricTransformBase::parameters&
  geometricTransformBase::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& geometricTransformBase::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  geometricTransformBase::parameters*
  geometricTransformBase::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  geometricTransformBase::parameters*
  geometricTransformBase::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool geometricTransformBase::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"resizeMode",resizeMode);
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
  bool geometricTransformBase::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"resizeMode",resizeMode);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // geometricTransformBase
  // --------------------------------------------------

  // default constructor
  geometricTransformBase::geometricTransformBase()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  geometricTransformBase::geometricTransformBase(const parameters& par)
    : functor() {


    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  geometricTransformBase::geometricTransformBase(const geometricTransformBase& other)
    : functor() {
    copy(other);
  }

  // destructor
  geometricTransformBase::~geometricTransformBase() {
  }

  // copy member
  geometricTransformBase& 
  geometricTransformBase::copy(const geometricTransformBase& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  geometricTransformBase&
  geometricTransformBase::operator=(const geometricTransformBase& other) {
    return (copy(other));
  }

  // class name
  const std::string& geometricTransformBase::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  const geometricTransformBase::parameters&
  geometricTransformBase::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  geometricTransformBase::parameters& 
  geometricTransformBase::getRWParameters() {
    parameters* par =
      dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  /*
   * Read the resize mode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,geometricTransformBase::eResizeMode& data) {
    std::string str;
    if (handler.read(str)) {
      if ( (str.find("eepDim") != std::string::npos) ||
           (str.find("eepdim") != std::string::npos)) {
        data = geometricTransformBase::KeepDimensions;
      } else if ( (str.find("eepOri") != std::string::npos) ||
                  (str.find("eepori") != std::string::npos)) { 
        data = geometricTransformBase::KeepOrigin;
      } else if (str.find("djust") != std::string::npos) { 
        data = geometricTransformBase::AdjustDimensions;
      } else {
        data = geometricTransformBase::KeepDimensions;
        str = "Invalid geometricTransformBase::eResizeMode value " + str;
        handler.setStatusString(str);
        return false;
      }
    } else {
      handler.setStatusString("Could not read geometricTransformBase::" \
                              "eResizeMode from string.");
      return false;
    }
    return true;
  }

  /*
   * Write the resize mode
   *
   * @ingroup gStorable
   */
  bool 
  write(ioHandler& handler,const geometricTransformBase::eResizeMode& data) {
    bool b=false;
    switch(data) {
      case geometricTransformBase::KeepDimensions:
        b=handler.write("KeepDimensions");
        break;
      case geometricTransformBase::KeepOrigin:
        b=handler.write("KeepOrigin");
        break;
      case geometricTransformBase::AdjustDimensions:
        b=handler.write("AdjustDimensions");
        break;
      default:
        handler.setStatusString("Unknown geometricTransformBase::eResizeMode");
        handler.write("Unknown");        
    }
    return b;
  }
}

