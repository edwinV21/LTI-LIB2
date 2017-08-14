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
 * \file   ltiCombinedCodec.h
 *         Contains the class lti::combinedCodec, 
 *         which is wrapper class for cascading two codecs.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiCombinedCodec.h,v 1.1 2008-05-19 16:53:38 alvarado Exp $
 */

#ifndef _LTI_COMBINED_CODEC_H_
#define _LTI_COMBINED_CODEC_H_

#include "ltiDataCodec.h"

namespace lti {
  /**
   * Class combinedCodec.
   *
   * This is a wrapper class for cascading two codecs given at
   * construction time.
   *
   * @see dataCodec::parameters.
   *
   * @ingroup gCodecs
   */
  class combinedCodec : public dataCodec {
  public:
    /**
     * The parameters for the class combinedCodec.
     */
    class parameters : public dataCodec::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Name of the class used for the first codec.
       *
       * Default value: "lti::runLengthCodec"
       */
      std::string codec1;

      /**
       * Name of the class used for the second codec.
       *
       * Default value: "lti::identityCodec"
       */
      std::string codec2;
    };

    /**
     * Constructor.
     */
    combinedCodec();

    /**
     * Constructor.
     */
    combinedCodec(const dataCodec& c1, const dataCodec& c2);

    /**
     * Construct a functor using the given parameters.
     */
    combinedCodec(const parameters& par);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    combinedCodec(const combinedCodec& other);

    /**
     * Destructor.
     */
    virtual ~combinedCodec();

    /**
     * Return the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Copy member.
     */
    combinedCodec& copy(const combinedCodec& other);

    /**
     * Alias for copy member.
     */
    combinedCodec& operator=(const combinedCodec& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual combinedCodec* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual combinedCodec* newInstance() const;

    /**
     * Return used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

  protected:
    /**
     * Implementation of on-copy data decoder. <b>This method
     * must not, under no circumstances, resize the destination vector.</b>
     * @return true if the decoding could be performed successfully.
     * @param src the source of the data.
     * @param dest the destination for the decoded data.
     * @param nsrc tells how many elements of the src vector
     *        must be decoded.
     * @param ndest tells how many elements of the destination vector
     *        can be used for the decoded data.
     *        When the method returns,
     *        this value tells how many elements were actually used,
     *        or notEnoughSpace if not enough elements were available.
     */
    virtual bool decodeImplementation(const buffer& src,
                                      buffer& dest,
                                      int nsrc, int& ndest) const;

    /**
     * Implementation of on-copy data encoder. <b>This method
     * must not, under no circumstances, resize the destination vector.</b>
     * @return true if the decoding could be performed successfully.
     * @param src the source of the data.
     * @param dest the destination for the encoded data.
     * @param nsrc tells how many elements of the src vector
     *        must be decoded.
     * @param ndest tells how many elements of the destination vector
     *        can be used for the decoded data.
     *        When the method returns,
     *        this value tells how many elements were actually used,
     *        or notEnoughSpace if not enough elements were available.
     */
    virtual bool encodeImplementation(const buffer& src,
                                      buffer& dest,
                                      int nsrc, int& ndest) const;


  public:
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
     * source of the encoded data buffer is given as argument.
     * The default is to return the given value.
     * @param encodedSize the size of the data buffer to be decoded
     * @return the estimated size of the decoded data.
     */
    virtual int estimateDecodedSize(int encodedSize) const;

  private:

    /**
     * Codec 1
     */
    dataCodec* codec1;

    /**
     * Codec 2
     */
    dataCodec* codec2;

  };
}

#endif
