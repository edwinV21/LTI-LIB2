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
 * \file   ltiPolynomialKernel.cpp
 *         Contains the class lti::polynomialKernel
 *         which implements the dot product with the kernelFunctorInterfaces
 *         for double and float
 * 
 * \author Jochen Wickel
 * \author Pablo Alvarado
 * \date   08.04.2009
 *
 * revisions ..: $Id: ltiPolynomialKernel.cpp,v 1.3 2009-09-08 21:35:08 alvarado Exp $
 */

#include "ltiPolynomialKernel.h"
#include "ltiFactory.h"
#include "ltiMath.h"

namespace lti {

  // register in factory<functor>
  _LTI_REGISTER_IN_FACTORY(functor,polynomialKernel)

  // --------------------------------------------------
  // polynomialKernel::parameters
  // --------------------------------------------------

  // default constructor
  polynomialKernel::parameters::parameters()
    : functor::parameters() {
    degree = 2;
  }

  // copy constructor
  polynomialKernel::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  polynomialKernel::parameters::~parameters() {
  }

  // copy member

  polynomialKernel::parameters&
  polynomialKernel::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    degree = other.degree;

    return *this;
  }

  // alias for copy method
  polynomialKernel::parameters&
  polynomialKernel::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& polynomialKernel::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  polynomialKernel::parameters*
  polynomialKernel::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  polynomialKernel::parameters*
  polynomialKernel::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool polynomialKernel::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b = lti::write(handler,"degree",degree);
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
  bool polynomialKernel::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b = lti::read(handler,"degree",degree);      
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // polynomialKernel
  // --------------------------------------------------

  // default constructor
  polynomialKernel::polynomialKernel()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  polynomialKernel::polynomialKernel(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }

  // default constructor
  polynomialKernel::polynomialKernel(const int degree)
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    defaultParameters.degree = degree;

    // set the default parameters
    setParameters(defaultParameters);
  }


  // copy constructor
  polynomialKernel::polynomialKernel(const polynomialKernel& other)
    : functor(),
      kernelFunctorInterface<double>(),
      kernelFunctorInterface<float>() {
    copy(other);
  }

  // destructor
  polynomialKernel::~polynomialKernel() {
  }

  // copy member
  polynomialKernel& 
  polynomialKernel::copy(const polynomialKernel& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  polynomialKernel&
  polynomialKernel::operator=(const polynomialKernel& other) {
    return (copy(other));
  }

  // class name
  const std::string& polynomialKernel::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  polynomialKernel* polynomialKernel::clone() const {
    return new polynomialKernel(*this);
  }

  // create a new instance
  polynomialKernel* polynomialKernel::newInstance() const {
    return new polynomialKernel();
  }

  // return parameters
  const polynomialKernel::parameters&
  polynomialKernel::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool polynomialKernel::updateParameters() {
    d_ = getParameters().degree;
    return true;
  }

  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the result in the given argument.
   */
  bool polynomialKernel::apply(const fvector& first, 
                           const fvector& second,
                           float& result) const {
    result = inner(first,second);
    return (result!=NaN);
  }

  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the resulting value.
   */
  float polynomialKernel::apply(const fvector& first,
                            const fvector& second) const {
    return inner(first,second);
  }

  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the result in the given argument.
   */
  bool polynomialKernel::apply(const dvector& first, 
                           const dvector& second,
                           double& result) const {
    result = inner(first,second);
    return (result!=NaN);
  }

  
  /*
   * Compute the inner-product-like value of the two given vectors
   * and return the resulting value.
   */
  double polynomialKernel::apply(const dvector& first, 
                             const dvector& second) const {
    return inner(first,second);
  }


  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool polynomialKernel::gradient(const fvector& x,
                                  const fvector& y,
                                        fvector& grad) const {
    grad.multiply(y,d_*(1.0f+x.dot(y)));
    return true;
  }

  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool polynomialKernel::gradient(const fvector& x,
                                        fvector& grad) const {
    
    grad.multiply(x,2.0f*d_*(1.0f+x.dot(x)));
    return true;
  }


  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool polynomialKernel::gradient(const dvector& x,
                                  const dvector& y,
                                        dvector& grad) const {
    grad.multiply(y,d_*(1.0f+x.dot(y)));
    return true;
  }

  /*
   * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
   * employed in some optimization processes.
   */
  bool polynomialKernel::gradient(const dvector& x,
                                        dvector& grad) const {
    grad.multiply(x,2.0f*d_*(1.0f+x.dot(x)));
    return true;
  }


  
}

