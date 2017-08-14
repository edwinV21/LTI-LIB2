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
 * \file   ltiAscii85Codec.h
 *         Contains the class lti::ascii85Codec,
 *         which encodes a byte array to an array of numbers in base 85.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiAscii85Codec.cpp,v 1.2 2008-05-20 21:40:48 alvarado Exp $
 */

#include "ltiAscii85Codec.h"
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>

namespace lti {

  // --------------------------------------------------
  // ascii85Codec
  // --------------------------------------------------

  // register in factory<dataCodec>
  _LTI_REGISTER_IN_FACTORY(dataCodec,ascii85Codec)

  const ubyte ascii85Codec::offset_=static_cast<ubyte>('!');

  // default constructor
  ascii85Codec::ascii85Codec()
    : dataCodec(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  ascii85Codec::ascii85Codec(const parameters& par)
    : dataCodec() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  ascii85Codec::ascii85Codec(const ascii85Codec& other)
    : dataCodec() {
    copy(other);
  }

  // destructor
  ascii85Codec::~ascii85Codec() {
  }

  // returns the name of this type
  const std::string& ascii85Codec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  ascii85Codec& ascii85Codec::copy(const ascii85Codec& other) {
      dataCodec::copy(other);

    return (*this);
  }

  // alias for copy member
  ascii85Codec& ascii85Codec::operator=(const ascii85Codec& other) {
    return (copy(other));
  }

  // clone member
  ascii85Codec* ascii85Codec::clone() const {
    return new ascii85Codec(*this);
  }

  ascii85Codec* ascii85Codec::newInstance() const {
    return new ascii85Codec();
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool ascii85Codec::encodeImplementation(const buffer& src, buffer& dest,
                                          int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    int rem=nsrc%4;
    if (rem == 0) {
      if (nsrc*5 > ndest*4) {
        ndest=NotEnoughSpace;
        setStatusString(notEnoughSpaceMsg);
        return false;
      }
    } else {
      if ((nsrc/4)*5+rem+1 > ndest) {
        ndest=NotEnoughSpace;
        setStatusString(notEnoughSpaceMsg);
        return false;
      }
    }
    // Ok, we have enough space
    ndest=0;

    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    for (int i=0; i<nsrc-rem; i+=4) {
      uint32 b1=*si++;
      uint32 b2=*si++;
      uint32 b3=*si++;
      uint32 b4=*si++;

      // build 4-byte number
      uint32 tmp=(b1<<24)|(b2<<16)|(b3<<8)|b4;
      // now build a base-85 representation
      bool z=true;

      buffer::iterator tmpi=di+5;
      for (int j=0; j<5; j++) {
        uint32 d=tmp%85;
        *--tmpi=static_cast<ubyte>(d)+offset_;
        tmp/=85;
        z=z && (d == 0);
      }
      // handle the special case if all digits are zero
      if (z) {
        *di='z';
        di++;
        ndest++;
      } else {
        di+=5;
        ndest+=5;
      }
    }
    if (rem != 0) {
      // we have some extra bytes
      int n=rem;
      uint32 tmp=0;
      int i,j;

      for (i=0; i<n; i++) {
        tmp=tmp<<8;
        tmp|=*si++;
      }
      for (i=n; i<4; i++) {
        tmp=tmp<<8;
      }
      for (j=5; j>n+1; j--) {
        tmp/=85;
      }
      buffer::iterator tmpi=di;
      tmpi+=n+1;
      for (j=n+1; j>0; j--) {
        uint32 d=tmp%85;
        *--tmpi=static_cast<ubyte>(d)+offset_;
        tmp/=85;
      }
      ndest+=n+1;
    }

    return true;
  }


  bool ascii85Codec::decodeImplementation(const buffer& src, buffer& dest,
                                          int nsrc, int& ndest) const {

    assert((src.size() >= nsrc) && (dest.size() >= ndest));
    int rem=nsrc%5;

    if (rem == 0) {
      if (nsrc*4 > ndest*5) {
        ndest=NotEnoughSpace;
        setStatusString(notEnoughSpaceMsg);
        return false;
      }
    } else {
      if ((nsrc/5)*4+rem-1 > ndest) {
        ndest=NotEnoughSpace;
        setStatusString(notEnoughSpaceMsg);
        return false;
      }
    }

    buffer::const_iterator si=src.begin();
    buffer::iterator di=dest.begin();

    // Ok, we have enough space
    ndest=0;
    for (int i=0; i<nsrc-rem; ndest+=4) {
      int32 c1=*si;
      if (c1 == 'z') {
        *di++=0;
        *di++=0;
        *di++=0;
        *di++=0;
        si++;
        i++;
      } else {
        c1-=offset_;
        int32 c2=(*++si)-offset_;
        int32 c3=(*++si)-offset_;
        int32 c4=(*++si)-offset_;
        int32 c5=(*++si)-offset_;
        ++si;
        if (c1 < 0 || c1 >= 85 || c2 < 0 || c2 >= 85 ||
            c3 < 0 || c3 >= 85 || c3 < 0 || c3 >= 85 ||
            c4 < 0 || c4 >= 85) {
          char msg[80];
          sprintf(msg,"Illegal characters in input buffer at position %d",i);
          setStatusString(msg);
          return false;
        }
        uint32 tmp=85*(85*(85*(85*c1+c2)+c3)+c4)+c5;

        *di++=static_cast<ubyte>(tmp>>24       );
        *di++=static_cast<ubyte>(tmp>>16 & 0xff);
        *di++=static_cast<ubyte>(tmp>>8  & 0xff);
        *di++=static_cast<ubyte>(tmp     & 0xff);

        i+=5;
      }
    }
    if (rem != 0) {
      int n=rem;
      uint32 tmp=0;
      int i,j;
      for (i=0; i<n; i++) {
        tmp *= 85;
        tmp += *si++ - offset_;
      }
      for (i=n; i<5; i++) {
        tmp*=85;
      }
      for (j=4; j>n-1; j--) {
        tmp=tmp>>8;
      }
      buffer::iterator tmpi=di;
      tmpi+=n-1;
      for (j=n-1; j>0; j--) {
        *--tmpi=static_cast<ubyte>(tmp&0xff);
        tmp=tmp>>8;
      }
      // for some reason, the last char is always one too small
      // therefore, simply add one
      tmpi=di;
      tmpi+=n-2;
      (*tmpi)++;
      ndest+=n-1;
    }

    return true;
  }


  int ascii85Codec::estimateEncodedSize(int old) const {
    if (old%4 == 0) {
      return 5*old/4;
    } else {
      return (old/4)*5+old%4+1;
    }
  }


  int ascii85Codec::estimateDecodedSize(int old) const {
    if (old%5 == 0) {
      return 4*old/5;
    } else {
      return (old/5)*4+old%5-1;
    }
  }


}
