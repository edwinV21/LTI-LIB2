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
 * \file   ltiAsciiHexCodecFlate.h
 *         Contains the class lti::asciiHexCodec,
 *         which encodes a byte array to an array of hexadecimal digits.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiAsciiHexCodec.cpp,v 1.2 2008-05-20 21:40:48 alvarado Exp $
 */

#include "ltiAsciiHexCodec.h"
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>

namespace lti {

  // register in factory<dataCodec>
  _LTI_REGISTER_IN_FACTORY(dataCodec,asciiHexCodec)

  // --------------------------------------------------
  // asciiHexCodec
  // --------------------------------------------------

  const ubyte asciiHexCodec::digits[16]= {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'A',
    'B',
    'C',
    'D',
    'E',
    'F'
  };


  // default constructor
  asciiHexCodec::asciiHexCodec()
    : dataCodec(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  asciiHexCodec::asciiHexCodec(const parameters& par)
    : dataCodec() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  asciiHexCodec::asciiHexCodec(const asciiHexCodec& other)
    : dataCodec() {
    copy(other);
  }

  // destructor
  asciiHexCodec::~asciiHexCodec() {
  }

  // returns the name of this type
  const std::string& asciiHexCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  asciiHexCodec& asciiHexCodec::copy(const asciiHexCodec& other) {
      dataCodec::copy(other);

    return (*this);
  }

  // alias for copy member
  asciiHexCodec&
    asciiHexCodec::operator=(const asciiHexCodec& other) {
    return (copy(other));
  }


  // clone member
  asciiHexCodec* asciiHexCodec::clone() const {
    return new asciiHexCodec(*this);
  }

  asciiHexCodec* asciiHexCodec::newInstance() const {
    return new asciiHexCodec();
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool asciiHexCodec::encodeImplementation(const buffer& src, buffer& dest,
                                           int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (nsrc*2 > ndest) {
      ndest=NotEnoughSpace;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    // Ok, we have enough space
    ndest=0;
    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    for (int i=0; i<nsrc; i++,si++,ndest+=2) {
      ubyte highnibble=*si>>4;
      ubyte lownibble=*si&0x0f;

      *di++=digits[highnibble];
      *di++=digits[lownibble];
    }
    return true;
  }


  bool asciiHexCodec::decodeImplementation(const buffer& src, buffer& dest,
                                           int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (nsrc > ndest*2) {
      ndest=NotEnoughSpace;
      setStatusString(notEnoughSpaceMsg);
      return false;
    }
    if (nsrc%2 != 0) {
      setStatusString("Input data must have an even number of bytes");
      return false;
    }

    // Ok, we have enough space

    // initialize a LUT
    static const ubyte* lut=0;
    if (isNull(lut)) {
      static vector<ubyte> theLUT(256,ubyte(255));
      for (int i='0';i<='9';++i) {
        theLUT[i] = i-'0';
      }
      for (int i='a';i<='f';++i) {
        theLUT[i] = i-'a'+10;
      }
      for (int i='A';i<='F';++i) {
        theLUT[i] = i-'A'+10;
      }
      
      lut = theLUT.data();
    }

    ndest=0;
    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();
    for (int i=0; i<nsrc; i+=2,di++,ndest++) {
      const ubyte highnibble = lut[*si++];
      const ubyte lownibble  = lut[*si++];

      if ((highnibble == 255) || (lownibble == 255)) {
        char msg[80];
        sprintf(msg,"Illegal characters in input buffer at position %d",i);
        setStatusString(msg);
        return false;
      }

      *di=(highnibble<<4)|lownibble;
    }
    return true;
  }


  int asciiHexCodec::estimateEncodedSize(int old) const {
    return old*2;
  }


  int asciiHexCodec::estimateDecodedSize(int old) const {
    return old/2+old%2;
  }


}
