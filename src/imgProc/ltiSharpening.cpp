/*
 * Copyright (C) 2007
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiContrastEnhancement.cpp
 *         Contains the abstract class lti::sharpening, 
 *         from which all sharpening classes are derived.
 * 
 * \author Pablo Alvarado
 * \date   18.10.2007
 *
 * revisions ..: $Id: ltiSharpening.cpp,v 1.1 2007-10-18 18:56:24 alvarado Exp $
 */

#include "ltiSharpening.h"

namespace lti {
  // --------------------------------------------------
  // sharpening::parameters
  // --------------------------------------------------

  // default constructor
  sharpening::parameters::parameters()
    : functor::parameters() {
    
    boundaryType = Zero;
  }

  // copy constructor
  sharpening::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  sharpening::parameters::~parameters() {
  }

  // copy member

  sharpening::parameters&
  sharpening::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    boundaryType = other.boundaryType;

    return *this;
  }

  // alias for copy method
  sharpening::parameters&
  sharpening::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& sharpening::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  sharpening::parameters*
  sharpening::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  sharpening::parameters*
  sharpening::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool sharpening::parameters::write(ioHandler& handler,
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
  bool sharpening::parameters::read(ioHandler& handler,
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
  // sharpening
  // --------------------------------------------------

  // default constructor
  sharpening::sharpening()
    : functor() {

  }

  // copy constructor
  sharpening::sharpening(const sharpening& other)
    : functor(),
      channelProcessingInterface<float>(),
      channelProcessingInterface<ubyte>() {

    copy(other);
  }

  // destructor
  sharpening::~sharpening() {
  }

  // copy member
  sharpening& 
  sharpening::copy(const sharpening& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  sharpening&
  sharpening::operator=(const sharpening& other) {
    return (copy(other));
  }

  // class name
  const std::string& sharpening::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  const sharpening::parameters&
  sharpening::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}

