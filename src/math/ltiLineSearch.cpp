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
 * \file   ltiLineSearch.cpp
 *         Contains the class lineSearch,
 *         which is used to find the minimum of an objective function along
 *         a given direction.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiLineSearch.cpp,v 1.2 2009-05-26 03:45:32 alvarado Exp $
 */


#include "ltiLineSearch.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // lineSearch::parameters
  // --------------------------------------------------

  // default constructor
  lineSearch::parameters::parameters()
    : functor::parameters() {
    
    epsilon = sqrt(std::numeric_limits<float>::epsilon());
    maxIterations = 100;
    maxBracketStepMagnification = 100.0;
    
  }

  // copy constructor
  lineSearch::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  lineSearch::parameters::~parameters() {
  }

  // copy member

  lineSearch::parameters&
  lineSearch::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    epsilon = other.epsilon;
    maxIterations = other.maxIterations;
    maxBracketStepMagnification = other.maxBracketStepMagnification;

    return *this;
  }

  // alias for copy method
  lineSearch::parameters&
  lineSearch::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& lineSearch::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  lineSearch::parameters*
  lineSearch::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  lineSearch::parameters*
  lineSearch::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool lineSearch::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"epsilon",epsilon);
      lti::write(handler,"maxIterations",maxIterations);
      lti::write(handler,"maxBracketStepMagnification",
                 maxBracketStepMagnification);
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
  bool lineSearch::parameters::read(ioHandler& handler,
                                    const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"epsilon",epsilon);
      lti::read(handler,"maxIterations",maxIterations);
      lti::read(handler,"maxBracketStepMagnification",
                maxBracketStepMagnification);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // lineSearch
  // --------------------------------------------------

  // default constructor
  lineSearch::lineSearch()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  lineSearch::lineSearch(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  lineSearch::lineSearch(const lineSearch& other)
    : functor() {
    copy(other);
  }

  // destructor
  lineSearch::~lineSearch() {
  }

  // copy member
  lineSearch& 
  lineSearch::copy(const lineSearch& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  lineSearch&
  lineSearch::operator=(const lineSearch& other) {
    return (copy(other));
  }

  // class name
  const std::string& lineSearch::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  lineSearch* lineSearch::clone() const {
    return new lineSearch(*this);
  }

  // create a new instance
  lineSearch* lineSearch::newInstance() const {
    return new lineSearch();
  }

  // return parameters
  const lineSearch::parameters&
  lineSearch::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


}

