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
 * \file   ltiFlateCodec.cpp
 *         Contains the class lti::flateCodec, 
 *         which encodes a byte array to an array of hexadecimal digits.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiFlateCodec.cpp,v 1.1 2008-05-16 21:27:13 alvarado Exp $
 */

#include "ltiFlateCodec.h"
#if HAVE_LIBZ
#include "ltiFactory.h"
#include <cctype>
#include <cstdio>
#include <zlib.h>


namespace lti {
  // register in factory<dataCodec>
  _LTI_REGISTER_IN_FACTORY(dataCodec,flateCodec)

  // --------------------------------------------------
  // flateCodec
  // --------------------------------------------------

  // default constructor
  flateCodec::flateCodec()
    : dataCodec(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  flateCodec::flateCodec(const parameters& par)
    : dataCodec() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  flateCodec::flateCodec(const flateCodec& other)
    : dataCodec() {
    copy(other);
  }

  // destructor
  flateCodec::~flateCodec() {
  }

  // returns the name of this type
  const std::string& flateCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  flateCodec&
    flateCodec::copy(const flateCodec& other) {
      dataCodec::copy(other);

    return (*this);
  }

  // alias for copy member
  flateCodec& flateCodec::operator=(const flateCodec& other) {
    return (copy(other));
  }


  // clone member
  flateCodec* flateCodec::clone() const {
    return new flateCodec(*this);
  }

  flateCodec* flateCodec::newInstance() const {
    return new flateCodec;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool flateCodec::encodeImplementation(const buffer& src, buffer& dest,
                                        int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    if (float(nsrc)*1.01f+12 > ndest) {
      ndest=NotEnoughSpace;
      setStatusString(notEnoughSpaceMsg);
    }

    z_stream zs;
    zs.next_in=const_cast<ubyte*>(src.data());
    zs.avail_in=nsrc;
    zs.next_out=dest.data();
    zs.avail_out=ndest;
    zs.zalloc=Z_NULL;
    zs.zfree=Z_NULL;
    zs.opaque=Z_NULL;

    // must return Z_OK
    int errc=deflateInit(&zs,Z_DEFAULT_COMPRESSION);
    if (errc != Z_OK) {
      setStatusString(notNull(zs.msg) ? zs.msg : "deflateInit failed.");
      return false;
    }
    // must return Z_STREAM_END
    errc=deflate(&zs,Z_FINISH);
    if (errc != Z_STREAM_END) {
      setStatusString(notNull(zs.msg) ? zs.msg : "deflate failed.");
      return false;
    }
    ndest=static_cast<int>(zs.next_out-dest.data());
    // must return Z_OK
    errc=deflateEnd(&zs);
    if (errc != Z_OK) {
      setStatusString(notNull(zs.msg) ? zs.msg : "deflateEnd failed.");
      return false;
    }

    return true;
  }


  bool flateCodec::decodeImplementation(const buffer& src, buffer& dest,
                                        int nsrc, int& ndest) const {

    assert(src.size() >= nsrc && dest.size() >= ndest);

    z_stream zs;
    zs.next_in=const_cast<ubyte*>(src.data());
    zs.avail_in=nsrc;
    zs.next_out=dest.data();
    zs.avail_out=ndest;
    zs.zalloc=Z_NULL;
    zs.zfree=Z_NULL;
    zs.opaque=Z_NULL;

    int errc;
    // must return Z_OK
    errc=inflateInit(&zs);
    if (errc != Z_OK) {
      setStatusString(notNull(zs.msg) ? zs.msg : "inflateInit failed.");
      return false;
    }
    // must return Z_STREAM_END
    errc=inflate(&zs,Z_FINISH);
    if (errc != Z_STREAM_END) {
      if (zs.avail_out == 0 && errc == Z_OK) {
        ndest=NotEnoughSpace;
        setStatusString(notEnoughSpaceMsg);
      } else {
        setStatusString(notNull(zs.msg) ? zs.msg : "inflate failed.");
      }
      return false;
    }
    ndest=static_cast<int>(zs.next_out-dest.data());
    // must return Z_OK
    errc=inflateEnd(&zs);
    if (errc != Z_OK) {
      setStatusString(notNull(zs.msg) ? zs.msg : "inflateEnd failed.");
      return false;
    }

    return true;
  }


  int flateCodec::estimateEncodedSize(int old) const {
    return static_cast<int>(float(old)*1.01f+0.5f+12);
  }


  int flateCodec::estimateDecodedSize(int old) const {
    return old*10;
  }

}

#endif
