/*
 * Copyright (C) 2008
 * Jochen Wickel
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
 * \file   ltiIdentityCodec.h
 *         Contains the class lti::identityCodec, 
 *         which does nothing but to pass through the data.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiIdentityCodec.cpp,v 1.1 2008-05-16 21:27:13 alvarado Exp $
 */

#include "ltiIdentityCodec.h"
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>

namespace lti {

  // register in factory<dataCodec>
  _LTI_REGISTER_IN_FACTORY(dataCodec,identityCodec)

  // --------------------------------------------------
  // identityCodec
  // --------------------------------------------------

  // default constructor
  identityCodec::identityCodec()
    : dataCodec(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  identityCodec::identityCodec(const parameters& par)
    : dataCodec() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  identityCodec::identityCodec(const identityCodec& other)
    : dataCodec() {
    copy(other);
  }

  // destructor
  identityCodec::~identityCodec() {
  }

  // returns the name of this type
  const std::string& identityCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  identityCodec& identityCodec::copy(const identityCodec& other) {
      dataCodec::copy(other);
    return (*this);
  }

  // alias for copy member
  identityCodec& identityCodec::operator=(const identityCodec& other) {
    return (copy(other));
  }


  // clone member
  identityCodec* identityCodec::clone() const {
    return new identityCodec(*this);
  }

  identityCodec* identityCodec::newInstance() const {
    return new identityCodec;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool identityCodec::encodeImplementation(const buffer& src, buffer& dest,
                                           int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (nsrc > ndest) {
      ndest=NotEnoughSpace;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    // Ok, we have enough space
    ndest=0;
    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    for (int i=0; i<nsrc; i++) {
      *di++=*si++;
      ndest++;
    }
    return true;
  }


  bool identityCodec::decodeImplementation(const buffer& src, buffer& dest,
                                           int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (nsrc > ndest) {
      ndest=NotEnoughSpace;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    
    // Ok, we have enough space
    ndest=0;
    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();
    for (int i=0; i<nsrc; i++) {
      *di++=*si++;
      ndest++;
    }
    return true;
  }

}
