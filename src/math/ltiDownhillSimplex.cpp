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
 * \file   ltiDownhillSimplex.cpp
 *         Contains the class downhillSimplex,
 *         which is an optimization mathod that requires only function
 *         evaluations not relying wether on gradients nor in line search.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiDownhillSimplex.cpp,v 1.2 2010-03-21 02:59:45 alvarado Exp $
 */


#include "ltiDownhillSimplex.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // downhillSimplex::parameters
  // --------------------------------------------------

  // default constructor
  downhillSimplex::parameters::parameters()
    : functor::parameters() {
    
    tolerance = std::numeric_limits<double>::epsilon();
    maxIterations = 5000;
    
  }

  // copy constructor
  downhillSimplex::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  downhillSimplex::parameters::~parameters() {
  }

  // copy member

  downhillSimplex::parameters&
  downhillSimplex::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    tolerance = other.tolerance;
    maxIterations = other.maxIterations;

    return *this;
  }

  // alias for copy method
  downhillSimplex::parameters&
  downhillSimplex::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& downhillSimplex::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  downhillSimplex::parameters*
  downhillSimplex::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  downhillSimplex::parameters*
  downhillSimplex::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool downhillSimplex::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"tolerance",tolerance);
      lti::write(handler,"maxIterations",maxIterations);
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
  bool downhillSimplex::parameters::read(ioHandler& handler,
                                    const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"tolerance",tolerance);
      lti::read(handler,"maxIterations",maxIterations);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // downhillSimplex
  // --------------------------------------------------

  // default constructor
  downhillSimplex::downhillSimplex()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  downhillSimplex::downhillSimplex(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  downhillSimplex::downhillSimplex(const downhillSimplex& other)
    : functor() {
    copy(other);
  }

  // destructor
  downhillSimplex::~downhillSimplex() {
  }

  // copy member
  downhillSimplex& 
  downhillSimplex::copy(const downhillSimplex& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  downhillSimplex&
  downhillSimplex::operator=(const downhillSimplex& other) {
    return (copy(other));
  }

  // class name
  const std::string& downhillSimplex::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  downhillSimplex* downhillSimplex::clone() const {
    return new downhillSimplex(*this);
  }

  // create a new instance
  downhillSimplex* downhillSimplex::newInstance() const {
    return new downhillSimplex();
  }

  // return parameters
  const downhillSimplex::parameters&
  downhillSimplex::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}
