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
 * revisions ..: $Id: ltiIdentityCodec.h,v 1.2 2008-05-19 16:53:38 alvarado Exp $
 */

#ifndef _LTI_IDENTITY_CODEC_H_
#define _LTI_IDENTITY_CODEC_H_

#include "ltiDataCodec.h"

namespace lti {
  /**
   * Class identityCodec.
   *
   * This is a dummy codec that does absolutely nothing. It is convenient
   * for using "no compression" as just one of many compression methods.
   *
   * @see dataCodec::parameters.
   *
   * @ingroup gCodecs
   */
  class identityCodec : public dataCodec {
  public:

    /**
     * Default constructor
     */
    identityCodec();

    /**
     * Construct a functor using the given parameters
     */
    identityCodec(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    identityCodec(const identityCodec& other);

    /**
     * Destructor
     */
    virtual ~identityCodec();

    /**
     * Return the name of this type ("identityCodec")
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    identityCodec& copy(const identityCodec& other);

    /**
     * Alias for copy member.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    identityCodec& operator=(const identityCodec& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual identityCodec* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual identityCodec* newInstance() const;

  protected:

    virtual bool decodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc, int& ndest) const;

    virtual bool encodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc, int& ndest) const;


  };
}

#endif
