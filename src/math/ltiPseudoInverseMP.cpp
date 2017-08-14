/*
 * Copyright (C) 2008
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
 * \file   ltiPseudoInverseMP.cpp
 *         Contains the class pseudoInverseMP,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   24.03.2008
 *
 * revisions ..: $Id: ltiPseudoInverseMP.cpp,v 1.1 2008-03-25 17:02:18 alvarado Exp $
 */

#include "ltiPseudoInverseMP.h"
#include "ltiMatrixInversion.h"

namespace lti {
  // --------------------------------------------------
  // pseudoInverseMP::parameters
  // --------------------------------------------------

  // default constructor
  pseudoInverseMP::parameters::parameters()
    : linearAlgebraFunctor::parameters() {
    
    lambda = double(0.01);
  }

  // copy constructor
  pseudoInverseMP::parameters::parameters(const parameters& other)
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  // destructor
  pseudoInverseMP::parameters::~parameters() {
  }

  // copy member

  pseudoInverseMP::parameters&
  pseudoInverseMP::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);

    
    lambda = other.lambda;

    return *this;
  }

  // alias for copy method
  pseudoInverseMP::parameters&
  pseudoInverseMP::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& pseudoInverseMP::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  pseudoInverseMP::parameters*
  pseudoInverseMP::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  pseudoInverseMP::parameters*
  pseudoInverseMP::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool pseudoInverseMP::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"lambda",lambda);
    }

    b = b && linearAlgebraFunctor::parameters::write(handler,false);

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
  bool pseudoInverseMP::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"lambda",lambda);
    }

    b = b && linearAlgebraFunctor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // pseudoInverseMP
  // --------------------------------------------------

  // default constructor
  pseudoInverseMP::pseudoInverseMP()
    : linearAlgebraFunctor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  pseudoInverseMP::pseudoInverseMP(const double& lambda)
    : linearAlgebraFunctor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    defaultParameters.lambda = lambda;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  pseudoInverseMP::pseudoInverseMP(const parameters& par)
    : linearAlgebraFunctor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  pseudoInverseMP::pseudoInverseMP(const pseudoInverseMP& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  pseudoInverseMP::~pseudoInverseMP() {
  }

  // copy member
  pseudoInverseMP& 
  pseudoInverseMP::copy(const pseudoInverseMP& other) {
    linearAlgebraFunctor::copy(other);

    return (*this);
  }

  // alias for copy member
  pseudoInverseMP&
  pseudoInverseMP::operator=(const pseudoInverseMP& other) {
    return (copy(other));
  }

  // class name
  const std::string& pseudoInverseMP::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  pseudoInverseMP* pseudoInverseMP::clone() const {
    return new pseudoInverseMP(*this);
  }

  // create a new instance
  pseudoInverseMP* pseudoInverseMP::newInstance() const {
    return new pseudoInverseMP();
  }

  // return parameters
  const pseudoInverseMP::parameters&
  pseudoInverseMP::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------


}

