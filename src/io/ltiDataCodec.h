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
 * \file   ltiDataCodec.h
 *         Contains the class lti::dataCodec, 
 *         which is the parent abstract class of all encoders/decoders.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiDataCodec.h,v 1.6 2008-05-19 16:53:38 alvarado Exp $
 */

#ifndef _LTI_DATA_CODEC_H_
#define _LTI_DATA_CODEC_H_

#include "ltiDataTransformer.h"

namespace lti {

  /**
   * Class dataCodec
   *
   * This is the base class for data encoding and decoding.
   * This an abstract class.
   *
   * @see dataCodec::parameters.
   *
   * @ingroup gCodecs
   */
  class dataCodec : public dataTransformer {
  public:

    /**
     * The coding direction can either be encode or decode.
     */
    enum eCodingDirection {
      Encode, /*< encode the data */
      Decode  /*< decode the data */
    };

    /**
     * The parameters for the class dataCodec
     */
    class parameters : public dataTransformer::parameters {
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
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
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
       * The direction of the coding.
       *
       * Default value: Decode
       */
      eCodingDirection codingDirection;
    };

    /**
     * Default constructor
     */
    dataCodec();

    /**
     * Construct a functor using the given parameters
     */
    dataCodec(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    dataCodec(const dataCodec& other);

    /**
     * Destructor
     */
    virtual ~dataCodec();
    
    /**
     * Encodes data in place.
     */
    virtual bool encode(ubyte* srcdest, int nsrc, int& ndest) const;

    /**
     * Encodes data in place.
     */
    virtual bool encode(buffer& srcdest) const;

    /**
     * Encodes data on copy.
     */
    virtual bool encode(const ubyte* src, int nsrc,
                        ubyte* dest, int& ndest) const;

    /**
     * Encodes data on copy.
     */
    virtual bool encode(const buffer& src, buffer& dest) const;


    /**
     * Decodes data in place.
     */
    virtual bool decode(ubyte* srcdest, int nsrc, int& ndest) const;

    /**
     * Decodes data in place.
     */
    virtual bool decode(buffer& srcdest) const;

    /**
     * Decodes data on copy.
     */
    virtual bool decode(const ubyte* src, int nsrc,
                        ubyte* dest, int& ndest) const;

    /**
     * Decodes data on copy.
     */
    virtual bool decode(const buffer& src, buffer& dest) const;

    /**
     * Operates on the given arguments. If the data is encoded or
     * decoded depends on the parameters::codingDirection parameter.
     * @param srcdest pointer to an array of ubyte. The input
     *        array must be allocated and have enough space for
     *        input as well as output data.
     * @param nsrc the number of input bytes that should be transformed.
     * @param ndest the number of bytes of the transformed data. If the
     *        buffer was too small, it will contain -1 when the function
     *        returns.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(ubyte* srcdest, int nsrc, int& ndest) const;

    /**
     * Operates on the given argument. If the data is encoded or
     * decoded depends on the parameters::codingDirection parameter.
     * @param srcdest vector with the input data which will
     *        also receive the transformed data.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(buffer& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src pointer to an array of ubyte which contains the
     *            source data
     * @param nsrc the number of input bytes that should be transformed.
     * @param dest pointer to an array of ubyte which will receive
     *             the transformed data. The array must be allocated
     *             by the caller
     * @param ndest When called, must contain the size of the output
     *              buffer. When the function returns, it contains the
     *              number of bytes actually used, or -1 if the buffer
     *              was too small.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const ubyte* src, int nsrc,
                       ubyte* dest, int& ndest) const;

    /**
     * Operates on the given argument.
     * @param src vector with the input data
     * @param dest vector with the encoded or decoded data
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const buffer& src, buffer& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dataCodec& copy(const dataCodec& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dataCodec& operator=(const dataCodec& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual dataCodec* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual dataCodec* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

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
                                      int nsrc, int& ndest) const=0;

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
                                      int nsrc, int& ndest) const=0;


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

  protected:
    /**
     * Computes a new reasonable buffer size for encoded data
     * if the given buffer size is too small. The default is
     * 2*orginialSize;
     * @param originalSize the size of the data buffer
     * @return the recommended new size of the data buffer
     */
    virtual int getNewEncodingBufferSize(int originalSize) const;

    /**
     * Computes a new reasonable buffer size for encoded data
     * if the given buffer size is too small. The default is
     * 2*orginialSize;
     * @param originalSize the size of the data buffer
     * @return the recommended new size of the data buffer
     */
    virtual int getNewDecodingBufferSize(int originalSize) const;

  };

  /**
   * Read a dataCodec::eCodingDirection
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            dataCodec::eCodingDirection& data);

  /**
   * Write a dataCodec::eCodingDirection
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const dataCodec::eCodingDirection& data);
}

#endif

