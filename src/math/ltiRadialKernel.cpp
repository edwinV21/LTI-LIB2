/*
 * Copyright (C) 2009
 * Jochen Wickel, Pablo Alvarado
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
 * \file   ltiRadialKernel.cpp
 *         Contains the class lti::radialKernel
 *         which implements the dot product with the kernelFunctorInterfaces
 *         for double and float
 * 
 * \author Jochen Wickel
 * \author Pablo Alvarado
 * \date   08.04.2009
 *
 * revisions ..: $Id: ltiRadialKernel.cpp,v 1.3 2009-09-08 21:35:08 alvarado Exp $
 */

#include "ltiRadialKernel.h"
#include "ltiFactory.h"
#include "ltiMath.h"

namespace lti {

  // register in factory<functor>
  _LTI_REGISTER_IN_FACTORY(functor,radialKernel)

  // --------------------------------------------------
  // radialKernel::parameters
  // --------------------------------------------------

  // default constructor
  radialKernel::parameters::parameters()
    : functor::parameters() {
    sigma = 1.0;
  }

  // copy constructor
  radialKernel::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  radialKernel::parameters::~parameters() {
  }

  // copy member

  radialKernel::parameters&
  radialKernel::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    sigma = other.sigma;

    return *this;
  }

  // alias for copy method
  radialKernel::parameters&
  radialKernel::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& radialKernel::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  radialKernel::parameters*
  radialKernel::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  radialKernel::parameters*
  radialKernel::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool radialKernel::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b = lti::write(handler,"sigma",sigma);
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
  bool radialKernel::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b = lti::read(handler,"sigma",sigma);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // radialKernel
  // --------------------------------------------------

  // default constructor
  radialKernel::radialKernel()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  radialKernel::radialKernel(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }

  // default constructor
  radialKernel::radialKernel(const double sigma)
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    defaultParameters.sigma = sigma;

    // set the default parameters
    setParameters(defaultParameters);
  }


  // copy constructor
  radialKernel::radialKernel(const radialKernel& other)
    : functor(),
      kernelFunctorInterface<double>(),
      kernelFunctorInterface<float>() {
    copy(other);
  }

  // destructor
  radialKernel::~radialKernel() {
  }

  // copy member
  radialKernel& 
  radialKernel::copy(const radialKernel& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  radialKernel& radialKernel::operator=(const radialKernel& other) {
    return (copy(other));
  }

  // class name
  const std::string& radialKernel::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  radialKernel* radialKernel::clone() const {
    return new radialKernel(*this);
  }

  // create a new instance
  radialKernel* radialKernel::newInstance() const {
    return new radialKernel();
  }

  // return parameters
  const radialKernel::parameters&
  radialKernel::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool radialKernel::updateParameters() {
    sigma2_ = -1.0/(0.5*sqr(getParameters().sigma));
    return true;
  }

  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the result in the given argument.
   */
  bool radialKernel::apply(const fvector& first, 
                           const fvector& second,
                                 float& result) const {
    result = inner(first,second);
    return (result!=NaN);
  }

  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the resulting value.
   */
  float radialKernel::apply(const fvector& first,
                            const fvector& second) const {
    return inner(first,second);
  }

  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the result in the given argument.
   */
  bool radialKernel::apply(const dvector& first, 
                           const dvector& second,
                                 double& result) const {
    result = inner(first,second);
    return (result!=NaN);
  }

  
  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the resulting value.
   */
  double radialKernel::apply(const dvector& first, 
                             const dvector& second) const {
    return inner(first,second);
  }


  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool radialKernel::gradient(const fvector& x,
                              const fvector& y,
                                    fvector& grad) const {
    grad.subtract(x,y);
    grad.multiply(inner(x,y)*static_cast<float>(2*sigma2_));
    return true;
  }

  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool radialKernel::gradient(const fvector& x,
                                    fvector& grad) const {
    
    grad.assign(x.size(),0.0f);
    return true;
  }


  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool radialKernel::gradient(const dvector& x,
                              const dvector& y,
                                    dvector& grad) const {
    grad.subtract(x,y);
    grad.multiply(inner(x,y)*2.0*sigma2_);
    return true;
  }

  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool radialKernel::gradient(const dvector& x,
                                    dvector& grad) const {
    grad.assign(x.size(),0.0);
    return true;
  }
  
}

