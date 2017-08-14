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
 * \file   ltiFlateCodec.h
 *         Contains the class lti::flateCodec, 
 *         which encodes a byte array to an array of hexadecimal digits.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiFlateCodec.h,v 1.3 2008-05-20 21:40:48 alvarado Exp $
 */


#ifndef _LTI_FLATE_CODEC_H_
#define _LTI_FLATE_CODEC_H_

#include "ltiConfig.h"
#include "ltiDataCodec.h"

#ifdef HAVE_LIBZ

namespace lti {

  /**
   * Class flateCodec
   *
   * Encodes a byte array to an array of hexadecimal digits or
   * performs the corresponding decoding method, depending on the parameters.
   *
   * @see dataCodec::parameters.
   *
   * @ingroup gCodecs
   */
  class flateCodec : public dataCodec {
  public:

    /**
     * Default constructor.
     */
    flateCodec();

    /**
     * Construct a functor using the given parameters.
     */
    flateCodec(const parameters& par);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    flateCodec(const flateCodec& other);

    /**
     * destructor
     */
    virtual ~flateCodec();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    flateCodec& copy(const flateCodec& other);

    /**
     * Alias for copy member.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    flateCodec& operator=(const flateCodec& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual flateCodec* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual flateCodec* newInstance() const;

    /**
     * Computes the default buffer size for encoded data. The
     * source of the original data buffer is given as argument.
     * The default is to return the given value.
     * @param originalSize the size of the data buffer to be encoded
     * @return the estimated size of the encoded data.
     */
    virtual int estimateEncodedSize(int originalSize) const;

    /**
     * Computes the default buffer size for decoded data. The
     * source of the original data buffer is given as argument.
     * The default is to return the given value.
     * @param originalSize the size of the data buffer to be decoded
     * @return the estimated size of the encoded data.
     */
    virtual int estimateDecodedSize(int originalSize) const;

  protected:
    virtual bool decodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc,  int& ndest) const;

    virtual bool encodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc, int& ndest) const;
  };
}

#else

#include "ltiRunLengthCodec.h"

#ifdef _LTI_MSC_VER
#pragma message("zlib not found. Using runLengthCodec instead of flateCodec.")
#else
#warning "zlib not found."
#endif

#endif


#endif
