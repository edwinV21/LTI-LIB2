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
 * \file   ltiEdgeDetection.cpp
 *         Contains the class edgeDetection,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   22.12.2008
 *
 * revisions ..: $Id: ltiEdgeDetection.cpp,v 1.1 2008-12-23 03:39:27 alvarado Exp $
 */

//TODO: include files
#include "ltiEdgeDetection.h"

namespace lti {
  // --------------------------------------------------
  // edgeDetection::parameters
  // --------------------------------------------------

  // default constructor
  edgeDetection::parameters::parameters()
    : functor::parameters() {
    noEdgeValue = ubyte(0);
    edgeValue = ubyte(255);
  }

  // copy constructor
  edgeDetection::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  edgeDetection::parameters::~parameters() {
  }

  // copy member

  edgeDetection::parameters&
  edgeDetection::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    noEdgeValue = other.noEdgeValue;
    edgeValue = other.edgeValue;

    return *this;
  }

  // alias for copy method
  edgeDetection::parameters&
  edgeDetection::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& edgeDetection::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  edgeDetection::parameters*
  edgeDetection::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  edgeDetection::parameters*
  edgeDetection::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool edgeDetection::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"noEdgeValue",noEdgeValue);
      lti::write(handler,"edgeValue",edgeValue);
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
  bool edgeDetection::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"noEdgeValue",noEdgeValue);
      lti::read(handler,"edgeValue",edgeValue);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // edgeDetection
  // --------------------------------------------------

  // default constructor
  edgeDetection::edgeDetection()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  edgeDetection::edgeDetection(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }

  // destructor
  edgeDetection::~edgeDetection() {
  }

  // copy member
  edgeDetection& 
  edgeDetection::copy(const edgeDetection& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  edgeDetection&
  edgeDetection::operator=(const edgeDetection& other) {
    return (copy(other));
  }

  // return parameters
  const edgeDetection::parameters&
  edgeDetection::getParameters() const {
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

  // On place apply for type channel8!
  bool edgeDetection::apply(channel8& srcdest) const {

    channel8 tmp;
    if (apply(srcdest,tmp)) {
      // can we just use the result as new image?
      if (srcdest.getMode() == channel8::Connected) {
        // yes: just transfer the data
        tmp.detach(srcdest);
      } else {
        // no: srcdest is part of another bigger image, just copy
        // the data.
        srcdest.fill(tmp);
      }
      return true;
    }


    return false;
  }

  // On place apply for type channel!
  bool edgeDetection::apply(channel& srcdest) const {
    channel tmp;
    if (apply(srcdest,tmp)) {
      // can we just use the result as new image?
      if (srcdest.getMode() == channel::Connected) {
        // yes: just transfer the data
        tmp.detach(srcdest);
      } else {
        // no: srcdest is part of another bigger image, just copy
        // the data.
        srcdest.fill(tmp);
      }
      return true;
    }

    return false;
  }


}

