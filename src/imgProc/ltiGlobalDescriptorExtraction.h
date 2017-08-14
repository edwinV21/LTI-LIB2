/*
 * Copyright (C) 2008
 * Pablo Alvarado
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
 * \file   ltiGlobalDescriptorExtraction.h
 *         Contains the class lti::globalDescriptorExtraction, 
 *         which has still to be documented by Pablo Alvarado
 * 
 * \author Pablo Alvarado
 * \date   14.09.2008
 *
 * revisions ..: $Id: ltiGlobalDescriptorExtraction.h,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#ifndef _LTI_GLOBAL_DESCRIPTOR_EXTRACTION_H_
#define _LTI_GLOBAL_DESCRIPTOR_EXTRACTION_H_

#include "ltiChannel8.h"
#include "ltiChannel.h"
#include "ltiImage.h"
#include "ltiDescriptorExtraction.h"

namespace lti {

  /**
   * Class globalDescriptorExtraction
   *
   * The feature extractions are functors which extract information from
   * the given images or channels, and leave this information in the form
   * of vectors of double.  This class is the parent class for all
   * implemented feature extractions that work on whole images.
   *
   * @see globalDescriptorExtraction::parameters.
   *
   * @ingroup gFeatureExtr
   */
  class globalDescriptorExtraction : public descriptorExtraction {
  public:
    /**
     * The parameters for the class globalDescriptorExtraction
     */
    class parameters : public descriptorExtraction::parameters {
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

    };

    /**
     * Default constructor
     */
    globalDescriptorExtraction();

    /**
     * Construct a functor using the given parameters
     */
    globalDescriptorExtraction(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    globalDescriptorExtraction(const globalDescriptorExtraction& other);

    /**
     * Destructor
     */
    virtual ~globalDescriptorExtraction();
    
    /**
     * Generates feature vector for the src channel, and leaves the result
     * in the dest vector.
     *
     * @param src channel with the source data.
     * @param dest dvector where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual bool apply(const channel& src,dvector& dest) const;

    /**
     * Generates feature vector for the src channel, and leaves the result
     * in the dest vector.
     *
     * @param src channel8 with the source data.
     * @param dest vector<double> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual bool apply(const channel8& src,dvector& dest) const;

    /**
     * Operates on a copy of the given parameters.
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual bool apply(const image& src,dvector& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    globalDescriptorExtraction& copy(const globalDescriptorExtraction& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    globalDescriptorExtraction& 
    operator=(const globalDescriptorExtraction& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual globalDescriptorExtraction* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual globalDescriptorExtraction* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

