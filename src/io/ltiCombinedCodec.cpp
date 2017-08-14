/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiCombinedCodec.cpp
 *         Contains the class lti::combinedCodec
 * 
 * \author Jochen Wickel
 * \date   05.11.2002
 *
 * revisions ..: $Id: ltiCombinedCodec.cpp,v 1.2 2012-09-16 04:58:33 alvarado Exp $
 */

#include "ltiCombinedCodec.h"
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>

namespace lti {


  // --------------------------------------------------
  // combinedCodec::parameters
  // --------------------------------------------------

  // default constructor
  combinedCodec::parameters::parameters()
    : dataCodec::parameters() {

    codec1 = "lti::runLengthCodec";
    codec2 = "lti::identityCodec";
  }

  // copy constructor
  combinedCodec::parameters::parameters(const parameters& other)
    : dataCodec::parameters()  {
    copy(other);
  }

  // destructor
  combinedCodec::parameters::~parameters() {
  }

  // get type name
  const std::string& combinedCodec::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  combinedCodec::parameters&
  combinedCodec::parameters::copy(const parameters& other) {
    dataCodec::parameters::copy(other);

    codec1 = other.codec1;
    codec2 = other.codec2;

    return *this;
  }

  // alias for copy member
  combinedCodec::parameters&
  combinedCodec::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  combinedCodec::parameters* combinedCodec::parameters::clone() const {
    return new parameters(*this);
  }

  combinedCodec::parameters* 
  combinedCodec::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool combinedCodec::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b=b && lti::write(handler, "codec1", codec1);
      b=b && lti::write(handler, "codec2", codec2);
    }

    b = b && dataCodec::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool combinedCodec::parameters::read(ioHandler& handler,
                                           const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b=b && lti::read(handler, "codec1", codec1);
      b=b && lti::read(handler, "codec2", codec2);
    }

    b = b && dataCodec::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // combinedCodec
  // --------------------------------------------------

  combinedCodec::combinedCodec()
    : dataCodec() {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // default constructor
  combinedCodec::combinedCodec(const dataCodec& c1, const dataCodec& c2)
    : dataCodec() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    defaultParameters.codec1=c1.name();
    defaultParameters.codec2=c2.name();

    // set the default parameters
    setParameters(defaultParameters);

    codec1->setParameters(c1.getParameters());
    codec2->setParameters(c2.getParameters());
  }

  // default constructor
  combinedCodec::combinedCodec(const parameters& par)
    : dataCodec() {

    setParameters(par);
  }


  // copy constructor
  combinedCodec::combinedCodec(const combinedCodec& other) : dataCodec() {
    copy(other);
  }

  // destructor
  combinedCodec::~combinedCodec() {
    delete codec1;
    delete codec2;
  }

  // returns the name of this type
  const std::string& combinedCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  combinedCodec&
    combinedCodec::copy(const combinedCodec& other) {
      dataCodec::copy(other);

    return (*this);
  }

  // alias for copy member
  combinedCodec&
    combinedCodec::operator=(const combinedCodec& other) {
    return (copy(other));
  }


  // clone member
  combinedCodec* combinedCodec::clone() const {
    return new combinedCodec(*this);
  }

  // clone member
  combinedCodec* combinedCodec::newInstance() const {
    return new combinedCodec();
  }

  // return parameters
  const combinedCodec::parameters& combinedCodec::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&dataCodec::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool combinedCodec::updateParameters() {
    if (dataCodec::updateParameters()) {
      const parameters pars = getParameters();
      if ( (notNull(codec1) && (codec1->name() != pars.codec1)) ||
           (isNull(codec1)) ) {
        delete codec1;
        codec1 = factory<dataCodec>::getFactory().newInstance(pars.codec1);
      }
      
      if ( (notNull(codec2) && (codec2->name() != pars.codec2)) ||
           (isNull(codec2)) ) {
        delete codec2;
        codec2 = factory<dataCodec>::getFactory().newInstance(pars.codec2);
      }

      return (notNull(codec1) && notNull(codec2));
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool combinedCodec::encodeImplementation(const buffer& src, buffer& dest,
                                           int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (isNull(codec1)) {
      setStatusString("First codec not set.  Name in parameters ok?");
      return false;
    }

    if (isNull(codec2)) {
      setStatusString("Second codec not set.  Name in parameters ok?");
      return false;
    }

    int ntmp=codec1->estimateEncodedSize(nsrc);
    buffer tmp(ntmp);
    bool result;

    result=codec1->encode(src.data(),nsrc,tmp.data(),ntmp);
    assert(ntmp != NotEnoughSpace);

    result=result && codec2->encode(tmp.data(),ntmp,dest.data(),ndest);
    return result;
  }

  bool combinedCodec::decodeImplementation(const buffer& src, buffer& dest,
                                           int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (isNull(codec1)) {
      setStatusString("First codec not set.  Name in parameters ok?");
      return false;
    }

    if (isNull(codec2)) {
      setStatusString("Second codec not set.  Name in parameters ok?");
      return false;
    }


    int ntmp=codec2->estimateDecodedSize(nsrc);
    buffer tmp(ntmp);
    bool result;

    result=codec2->decode(src.data(),nsrc,tmp.data(),ntmp);
    assert(ntmp != NotEnoughSpace);

    result=result && codec1->decode(tmp.data(),ntmp,dest.data(),ndest);
    return result;
  }



  int combinedCodec::estimateEncodedSize(int old) const {
    if (isNull(codec1)) {
      setStatusString("First codec not set.  Name in parameters ok?");
      return -1;
    }

    if (isNull(codec2)) {
      setStatusString("Second codec not set.  Name in parameters ok?");
      return -1;
    }

    int i1=codec1->estimateEncodedSize(old);
    return codec2->estimateEncodedSize(i1);
  }

  int combinedCodec::estimateDecodedSize(int old) const {
    if (isNull(codec1)) {
      setStatusString("First codec not set.  Name in parameters ok?");
      return -1;
    }

    if (isNull(codec2)) {
      setStatusString("Second codec not set.  Name in parameters ok?");
      return -1;
    }

    int i1=codec2->estimateDecodedSize(old);
    return codec1->estimateDecodedSize(i1);
  }


}
