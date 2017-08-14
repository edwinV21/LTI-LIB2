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
 * \file   ltiRunLengthCodec.cpp
 *         Contains the class lti::runLenghtCodec, 
 *         which does a classical run length encoding/decoding.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiRunLengthCodec.cpp,v 1.1 2008-05-19 16:53:38 alvarado Exp $
 */

#include "ltiRunLengthCodec.h"
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>


namespace lti {

  _LTI_REGISTER_IN_FACTORY(dataCodec,runLengthCodec)

  // --------------------------------------------------
  // runLengthCodec
  // --------------------------------------------------

  // default constructor
  runLengthCodec::runLengthCodec()
    : dataCodec(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  runLengthCodec::runLengthCodec(const parameters& par)
    : dataCodec() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  runLengthCodec::runLengthCodec(const runLengthCodec& other)
    : dataCodec() {
    copy(other);
  }

  // destructor
  runLengthCodec::~runLengthCodec() {
  }

  // returns the name of this type
  const std::string& runLengthCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  runLengthCodec&
    runLengthCodec::copy(const runLengthCodec& other) {
      dataCodec::copy(other);

    return (*this);
  }

  // alias for copy member
  runLengthCodec&
    runLengthCodec::operator=(const runLengthCodec& other) {
    return (copy(other));
  }


  // clone member
  runLengthCodec* runLengthCodec::clone() const {
    return new runLengthCodec(*this);
  }

  runLengthCodec* runLengthCodec::newInstance() const {
    return new runLengthCodec();
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool runLengthCodec::encodeImplementation(const buffer& src, buffer& dest,
                                        int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    int count=0;

    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    int i=0;

    ubyte old;

    while (i < nsrc && count < ndest) {
      old=*si;
      buffer::const_iterator tmpi=si;

      // detect run
      int n=1;
      int tmpj=i+1;
      tmpi++;
      while (tmpj < nsrc && *tmpi == old && n < 128) {
        tmpi++;
        tmpj++;
        n++;
      }

      // check number of identical bytes
      if (n > 1) {
        // have at least two copies; write length byte of n
        *di++=static_cast<ubyte>(257-n);
        // write data byte
        *di++=old;
        si+=n;
        count+=2;
      } else {
        // we do not have a run; detect number of different elements
        if (tmpj < nsrc) {
          old=*tmpi;
          tmpi++;
          tmpj++;
          while (tmpj < nsrc && *tmpi != old && n < 128) {
            old=*tmpi;
            tmpi++;
            tmpj++;
            n++;
          }
          // we now have n bytes that differ
          *di++=static_cast<ubyte>(n-1);
          for (int j=0; j<n; j++) {
            *di++=*si++;
          }
          count+=n+1;
        } else {
          // special case: We have a single byte at the end of the
          // stream
          *di++=static_cast<ubyte>(0);
          *di++=old;
          count+=2;
          si++;
        }
      }
      i+=n;
    }
    if (count >= ndest && i < nsrc) {
      ndest=-1;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    ndest=count;
    return true;
  }

  bool runLengthCodec::decodeImplementation(const buffer& src, buffer& dest,
                                            int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    int count=0;

    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    int i=0;

    while (i < nsrc && count < ndest) {
      ubyte tmp=*si++;
      if (tmp < 128) {
        // copy next tmp+1 bytes
        tmp++;
        for (int j=0; j<tmp; j++) {
          *di++=*si++;
        }
        i+=tmp+1;
      } else if (tmp > 128) {
        // we have a run
        ubyte data=*si++;
        tmp=257-tmp;
        for (int j=0; j<tmp; j++) {
          *di++=data;
        }
        i+=2;
      } else {
        // end of data
        setStatusString("End of data occured in the middle of the buffer");
        ndest=count;
        return false;
      }
      count+=tmp;
    }

    if (count >= ndest && i < nsrc) {
      ndest=-1;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    ndest=count;
    return true;
  }


  int runLengthCodec::estimateEncodedSize(int old) const {
    // this is the absolute worst case
    return 2*old;
  }


  int runLengthCodec::estimateDecodedSize(int old) const {
    return old*2;
  }

}
