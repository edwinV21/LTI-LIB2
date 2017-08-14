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
 * \file   ltiGlobalDescriptorExtraction.cpp
 *         Contains the class globalDescriptorExtraction,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   14.09.2008
 *
 * revisions ..: $Id: ltiGlobalDescriptorExtraction.cpp,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#include "ltiGlobalDescriptorExtraction.h"

namespace lti {
  // --------------------------------------------------
  // globalDescriptorExtraction::parameters
  // --------------------------------------------------

  // default constructor
  globalDescriptorExtraction::parameters::parameters()
    : descriptorExtraction::parameters() {
  }

  // copy constructor
  globalDescriptorExtraction::parameters::parameters(const parameters& other)
    : descriptorExtraction::parameters() {
    copy(other);
  }

  // destructor
  globalDescriptorExtraction::parameters::~parameters() {
  }

  // copy member

  globalDescriptorExtraction::parameters&
  globalDescriptorExtraction::parameters::copy(const parameters& other) {
    descriptorExtraction::parameters::copy(other);

    

    return *this;
  }

  // alias for copy method
  globalDescriptorExtraction::parameters&
  globalDescriptorExtraction::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& globalDescriptorExtraction::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  globalDescriptorExtraction::parameters*
  globalDescriptorExtraction::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  globalDescriptorExtraction::parameters*
  globalDescriptorExtraction::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool globalDescriptorExtraction::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
    }

    b = b && descriptorExtraction::parameters::write(handler,false);

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
  bool globalDescriptorExtraction::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && descriptorExtraction::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // globalDescriptorExtraction
  // --------------------------------------------------

  // default constructor
  globalDescriptorExtraction::globalDescriptorExtraction()
    : descriptorExtraction() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  globalDescriptorExtraction::globalDescriptorExtraction(const parameters& par)
    : descriptorExtraction() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  globalDescriptorExtraction::globalDescriptorExtraction(const globalDescriptorExtraction& other)
    : descriptorExtraction() {
    copy(other);
  }

  // destructor
  globalDescriptorExtraction::~globalDescriptorExtraction() {
  }

  // copy member
  globalDescriptorExtraction& 
  globalDescriptorExtraction::copy(const globalDescriptorExtraction& other) {
    descriptorExtraction::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  globalDescriptorExtraction&
  globalDescriptorExtraction::operator=(const globalDescriptorExtraction& other) {
    return (copy(other));
  }

  // class name
  const std::string& globalDescriptorExtraction::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  globalDescriptorExtraction* globalDescriptorExtraction::clone() const {
    return new globalDescriptorExtraction(*this);
  }

  // create a new instance
  globalDescriptorExtraction* globalDescriptorExtraction::newInstance() const {
    return new globalDescriptorExtraction();
  }

  // return parameters
  const globalDescriptorExtraction::parameters&
  globalDescriptorExtraction::getParameters() const {
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

  // On copy apply for type image!
  bool globalDescriptorExtraction::apply(const image&, 
                                        dvector& dest) const {
    
    dest.clear();
    setStatusString("Method not implemented yet");

    return false;
  }

  // On copy apply for type channel!
  bool globalDescriptorExtraction::apply(const channel&, 
                                        dvector& dest) const {

    dest.clear();
    setStatusString("Method not implemented yet");

    return false;
  }

  // On copy apply for type channel8!
  bool globalDescriptorExtraction::apply(const channel8&, 
                                        dvector& dest) const {
    dest.clear();
    setStatusString("Method not implemented yet");

    return false;
  }



}

