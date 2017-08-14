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
 * \file   ltiDataTransformer.h
 *         Contains the class lti::dataTransformer, 
 *         which has still to be documented by Jochen Wickel
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiDataTransformer.h,v 1.6 2008-05-16 21:27:13 alvarado Exp $
 */

#ifndef _LTI_DATA_TRANSFORMER_H_
#define _LTI_DATA_TRANSFORMER_H_

#include "ltiVector.h"
#include "ltiFunctor.h"

namespace lti {

  /**
   * Class dataTransformer
   *
   * This is the low-level base class for data transformers. Most notable
   * examples for such transformers are coding or encryption functors.
   * This is an abstract class.
   *
   * @ingroup gUnassigned
   */
  class dataTransformer : public functor {
  public:

    enum {
      NotEnoughSpace=-1
    };

    /**
     * The buffers with the data to be transformed (or the transformed data)
     * are manipulated in container instances of the buffer type.
     */
    class buffer : public vector<ubyte> {
    public:
      /**
       * Construct a buffer with n elements
       */
      buffer(const int& n);

      /**
       * Construct an element with n elements, with a copy of the given data
       */
      buffer(const int& n, ubyte data[], const eConstantReference constRef);

      /**
       * Save this buffer as a raw data block to the given file.
       */
      bool saveRaw(const std::string& filename) const;

      /**
       * Load this buffer as a raw data block from the given file.
       * The buffer is resized to fit the size of the file.
       */
      bool loadRaw(const std::string& filename);
    };


    /**
     * Default constructor
     */
    dataTransformer();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    dataTransformer(const dataTransformer& other);

    /**
     * Destructor
     */
    virtual ~dataTransformer();


    /**
     * Operates on the given arguments.
     * @param srcdest pointer to an array of ubyte. The input
     *        array must be allocated and have enough space for
     *        input as well as output data.
     * @param nsrc the number of input bytes that should be transformed.
     * @param ndest The caller must set this to the number of bytes
     *        of the buffer that can be used for the result.
     *        After returning, it will contain the number of bytes
     *        that are actually used.
     *        If buffer was too small, it will contain notEnoughSpace.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(ubyte* srcdest, int nsrc, int& ndest) const=0;

    /**
     * Operates on the given argument.
     * @param srcdest vector with the input data which will
     *        also receive the transformed data. The
     *        vector will be resized to fit the transformed data.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(buffer& srcdest) const=0;

    /**
     * Operates on a copy of the given arguments.
     * @param src pointer to an array of ubyte which contains the
     *            source data
     * @param nsrc the number of input bytes that should be transformed.
     * @param dest pointer to an array of ubyte which will receive
     *             the transformed data. The array must be allocated
     *             by the caller.
     * @param ndest When called, must contain the size of the output
     *              buffer. When the function returns, it contains the
     *              number of bytes actually used, or -1 if the buffer
     *              was too small.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const ubyte* src,
                       int nsrc, ubyte* dest, int& ndest) const=0;

    /**
     * Operates on the given argument.
     * @param src vector with the input data. All elements of the vector
     *        are transformed.
     * @param dest vector which will receive the transformed data. The
     *        vector is resized so that it fits the data.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const buffer& src, buffer& dest) const=0;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dataTransformer& copy(const dataTransformer& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dataTransformer& operator=(const dataTransformer& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual dataTransformer* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual dataTransformer* newInstance() const = 0;
    
    /**
     * Commonly used message
     */
    static const char *const notEnoughSpaceMsg;
  };
}

#endif

