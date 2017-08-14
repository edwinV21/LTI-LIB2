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
 * \file   ltiMorphology.h
 *         Contains the class lti::morphology, which implements basic general
 *         constructs for morphological operators.
 * \author Pablo Alvarado
 * \date   10.10.2009
 *
 * revisions ..: $Id: ltiMorphology.cpp,v 1.1 2009-10-12 02:47:15 alvarado Exp $
 */

#include "ltiMorphology.h"

#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiFactory.h"

namespace lti {

  // --------------------------------------------------
  // morphology::parameters
  // --------------------------------------------------

  // default constructor
  morphology::parameters::parameters()
    : functor::parameters(), mode(Binary),
      boundaryType(Zero),structuringElement_(0) {

  }

  // copy constructor
  morphology::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    structuringElement_ = 0;
    copy(other);
  }

  // destructor
  morphology::parameters::~parameters() {
    delete structuringElement_;
  }

  // get type name
  const std::string& morphology::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  morphology::parameters&
  morphology::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    delete structuringElement_;
    structuringElement_ = 0;

    if (notNull(other.structuringElement_)) {
      structuringElement_ = other.structuringElement_->clone();
    }

    mode = other.mode;
    boundaryType = other.boundaryType;

    return *this;
  }

  // clone member
  morphology::parameters* morphology::parameters::clone() const {
    return new parameters(*this);
  }

  morphology::parameters* morphology::parameters::newInstance() const {
    return new parameters;
  }

  const container& morphology::parameters::getStructuringElement() const {
    if (isNull(structuringElement_)) {
      throw invalidParametersException(name());
    }
    return *structuringElement_;
  }

  void morphology::parameters::setStructuringElement(const container& se) {
    delete structuringElement_;
    structuringElement_ = 0;
    structuringElement_ = se.clone();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool morphology::parameters::write(ioHandler& handler,
                                     const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b=b&&lti::write(handler,"mode",mode);

      if (isNull(structuringElement_)) {
        b=b&&lti::write(handler,"structuringElementType","none");
      } else {
        b=b&&lti::write(handler,"structuringElementType",
                        structuringElement_->name());
        b=b&&lti::write(handler,"structuringElement",getStructuringElement());
      }

      b=b&&lti::write(handler,"boundaryType",boundaryType);

    }

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
   * @return true if write was successful
   */
  bool morphology::parameters::read(ioHandler& handler,
                                    const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      std::string str;
      lti::read(handler,"mode",mode);

      b = b && lti::read(handler,"structuringElementType",str);
      delete structuringElement_;
      structuringElement_ = factory<container>::getFactory().newInstance(str);

      if (notNull(structuringElement_)) {
        if ((b = b && handler.readBegin())) {
          if ((b = b && handler.trySymbol("structuringElement"))) {
            b = b && handler.readKeyValueSeparator();
            b = b && structuringElement_->read(handler);
          }
          b = b && handler.readEnd();
        }
      }

      b = b && lti::read(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // morphology
  // --------------------------------------------------

  // default constructor
  morphology::morphology()
    : functor() {
    parameters tmpParam;
    setParameters(tmpParam);
  }

  morphology::morphology(const parameters& par)
    : functor() {
    setParameters(par);
  }

  // default constructor
  morphology::morphology(const container& se)
    : functor() {
    parameters tmpParam;
    tmpParam.setStructuringElement(se);
    setParameters(tmpParam);
  }


  // copy constructor
  morphology::morphology(const morphology& other)
    : functor(),
      matrixProcessingInterface<float>(),
      matrixProcessingInterface<ubyte>() {
    copy(other);
  }

  // destructor
  morphology::~morphology() {
  }


  // copy member
  morphology& morphology::copy(const morphology& other) {
    functor::copy(other);
    return (*this);
  }

  // return parameters
  const morphology::parameters& morphology::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  void morphology::setStructuringElement(const container& se) {
    parameters tmpParam(getParameters());
    tmpParam.setStructuringElement(se);
    setParameters(tmpParam);
  }

  // read function for eMorphologyMode.
  bool read(ioHandler& handler,morphology::eMorphologyMode& data) {

    std::string str;
    if (handler.read(str)) {
   
      if (str.find("inary") != std::string::npos) {
        data = morphology::Binary;
      } else if (str.find("yFlat") != std::string::npos) {
        data = morphology::GrayFlat;
      } else if (str.find("yNonFlat") != std::string::npos)  {
        data = morphology::GrayNonFlat;
      } else {
        data = morphology::Binary;
        handler.setStatusString("undefined eMorphologyMode");
        return false;
      }

      return true;
    }

    handler.setStatusString("Could not read std::string from stream");
    return false;
  }

  // write function for eMorphologyMode.
  bool write(ioHandler& handler,const morphology::eMorphologyMode& data) {
    bool b=false;
    switch(data) {
    case morphology::Binary:
      b=handler.write("Binary");
      break;
    case morphology::GrayFlat:
      b=handler.write("GrayFlat");
      break;
    case morphology::GrayNonFlat:
      b=handler.write("GrayNonFlat");
      break;
    default:
      b=false;
      handler.setStatusString("Undefined eMorphologyMode");
      handler.write("Unknown");
    }

    return b;
  }
}
