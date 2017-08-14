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
 * \file   ltiBinarizationCodec.cpp
 *         Contains the class lti::binarizationCodec, 
 *         which encodes a byte array to an array of bits.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiBinarizationCodec.cpp,v 1.3 2008-08-06 03:40:58 alvarado Exp $
 */

#include "ltiBinarizationCodec.h"
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>
#include <limits>


namespace lti {

  // register in factory<dataCodec>
  _LTI_REGISTER_IN_FACTORY(dataCodec,binarizationCodec)



  // --------------------------------------------------
  // binarizationCodec::parameters
  // --------------------------------------------------

  // default constructor
  binarizationCodec::parameters::parameters()
    : dataCodec::parameters() {

    maxValue=std::numeric_limits<ubyte>::max();
    threshold=maxValue/2+1;
  }

  // copy constructor
  binarizationCodec::parameters::parameters(const parameters& other)
    : dataCodec::parameters()  {
    copy(other);
  }

  // destructor
  binarizationCodec::parameters::~parameters() {
  }

  // get type name
  const std::string& binarizationCodec::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  binarizationCodec::parameters&
  binarizationCodec::parameters::copy(const parameters& other) {
    dataCodec::parameters::copy(other);

    threshold=other.threshold;
    maxValue=other.maxValue;

    return *this;
  }

  // alias for copy member
  binarizationCodec::parameters&
  binarizationCodec::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  binarizationCodec::parameters* binarizationCodec::parameters::clone() const {
    return new parameters(*this);
  }

  binarizationCodec::parameters* 
  binarizationCodec::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool binarizationCodec::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      b=b && lti::write(handler, "threshold", threshold);
      b=b && lti::write(handler, "maxValue", maxValue);
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
  bool binarizationCodec::parameters::read(ioHandler& handler,
                                           const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b=b && lti::read(handler, "threshold", threshold);
      b=b && lti::read(handler, "maxValue", maxValue);
    }

    b = b && dataCodec::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // binarizationCodec
  // --------------------------------------------------

  // default constructor
  binarizationCodec::binarizationCodec()
    : dataCodec(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  binarizationCodec::binarizationCodec(const parameters& par)
    : dataCodec() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  binarizationCodec::binarizationCodec(const binarizationCodec& other)
    : dataCodec() {
    copy(other);
  }

  // destructor
  binarizationCodec::~binarizationCodec() {
  }

  // returns the name of this type
  const std::string& binarizationCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  binarizationCodec&
  binarizationCodec::copy(const binarizationCodec& other) {
    dataCodec::copy(other);

    return (*this);
  }

  // alias for copy member
  binarizationCodec&
  binarizationCodec::operator=(const binarizationCodec& other) {
    return (copy(other));
  }


  // clone member
  binarizationCodec* binarizationCodec::clone() const {
    return new binarizationCodec(*this);
  }

  binarizationCodec* binarizationCodec::newInstance() const {
    return new binarizationCodec();
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool binarizationCodec::encodeImplementation(const buffer& src, buffer& dest,
                                               int nsrc, int& ndest) const {

    assert((src.size() >= nsrc) && (dest.size() >= ndest));

    int count=0;

    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    int i=0;

    const ubyte threshold=getParameters().threshold;
    *di=0;
    while ((i < nsrc) && (count < ndest)) {
      *di=(*di << 1) | ((*si++ >= threshold) ? 1 : 0);
      i++;
      if (i%(sizeof(ubyte)*8) == 0) {
        di++;
        count++;
        *di=0;
      }
    }
    if ((count >= ndest) && (i < nsrc)) {
      ndest=-1;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    ndest=count;
    return true;
  }

  bool binarizationCodec::decodeImplementation(const buffer& src, buffer& dest,
                                               int nsrc, int& ndest) const {

    assert((src.size() >= nsrc) && (dest.size() >= ndest));

    int count=0;
		
    static int* threshs(0);
    if (isNull(threshs)) {
      // let the compiler take control of the static memory management
      static int tmpThreshs[sizeof(ubyte)*8]; 
      
      // init threshold array
      for (unsigned int b=0; b<sizeof(ubyte)*8; b++) {
        tmpThreshs[b]=1<<(sizeof(ubyte)*8-b-1);
      }

      threshs=tmpThreshs;
    }

    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    const ubyte maxValue=getParameters().maxValue;
    int i=0;

    while ((i < nsrc) && (count < ndest)) {
      int tmp=*si;
      for (unsigned int j=0; j<sizeof(ubyte)*8; j++) {			 
        if ((tmp & threshs[j]) != 0) {
          *di++=maxValue;
        } else {
          *di++=0;
        }
        count++;
      }
      si++;
      i++;
    }

    if ((count >= ndest) && (i < nsrc)) {
      ndest=-1;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    ndest=count;
    return true;
  }


  int binarizationCodec::estimateEncodedSize(int old) const {
    // this is the absolute worst case
    return old/8+1;
  }


  int binarizationCodec::estimateDecodedSize(int old) const {
    return old*8;
  }

  // return parameters
  const binarizationCodec::parameters&
  binarizationCodec::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&dataCodec::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }



}
