/*
 * Copyright (C) 2009
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
 * \file   ltiConjugateGradients.cpp
 *         Contains the class conjugateGradients,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiConjugateGradients.cpp,v 1.1 2009-05-26 00:49:13 alvarado Exp $
 */


#include "ltiConjugateGradients.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // conjugateGradients::parameters
  // --------------------------------------------------

  // default constructor
  conjugateGradients::parameters::parameters()
    : functor::parameters() {
    
    tolerance = sqrt(std::numeric_limits<float>::epsilon());
    maxIterations = 100;
    
  }

  // copy constructor
  conjugateGradients::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  conjugateGradients::parameters::~parameters() {
  }

  // copy member

  conjugateGradients::parameters&
  conjugateGradients::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    tolerance = other.tolerance;
    maxIterations = other.maxIterations;
    lineSearchParameters.copy(other.lineSearchParameters);

    return *this;
  }

  // alias for copy method
  conjugateGradients::parameters&
  conjugateGradients::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& conjugateGradients::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  conjugateGradients::parameters*
  conjugateGradients::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  conjugateGradients::parameters*
  conjugateGradients::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool conjugateGradients::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"tolerance",tolerance);
      lti::write(handler,"maxIterations",maxIterations);
      lti::write(handler,"lineSearchParameters",lineSearchParameters);
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
  bool conjugateGradients::parameters::read(ioHandler& handler,
                                    const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"tolerance",tolerance);
      lti::read(handler,"maxIterations",maxIterations);
      lti::read(handler,"lineSearchParameters",lineSearchParameters);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // conjugateGradients
  // --------------------------------------------------

  // default constructor
  conjugateGradients::conjugateGradients()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  conjugateGradients::conjugateGradients(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  conjugateGradients::conjugateGradients(const conjugateGradients& other)
    : functor() {
    copy(other);
  }

  // destructor
  conjugateGradients::~conjugateGradients() {
  }

  // copy member
  conjugateGradients& 
  conjugateGradients::copy(const conjugateGradients& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  conjugateGradients&
  conjugateGradients::operator=(const conjugateGradients& other) {
    return (copy(other));
  }

  // class name
  const std::string& conjugateGradients::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  conjugateGradients* conjugateGradients::clone() const {
    return new conjugateGradients(*this);
  }

  // create a new instance
  conjugateGradients* conjugateGradients::newInstance() const {
    return new conjugateGradients();
  }

  // return parameters
  const conjugateGradients::parameters&
  conjugateGradients::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}

