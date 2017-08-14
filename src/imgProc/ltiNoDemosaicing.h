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
 * \file   ltiNoDemosaicing.h
 *         Contains the class lti::noDemosaicing, which does
 *         simple no interpolation of real colors for a Bayer
 *         pattern.
 * \author Pablo Alvarado
 * \date   26.10.2008
 *
 * revisions ..: $Id: ltiNoDemosaicing.h,v 1.1 2008-10-27 01:35:51 alvarado Exp $
 */

#ifndef _LTI_NO_DEMOSAICING_H_
#define _LTI_NO_DEMOSAICING_H_

#include "ltiMatrix.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiBayerDemosaicing.h"

namespace lti {

  /**
   * No Demosaicing
   *
   * This class is used in debugging of Bayer Patterns.
   * 
   * It just assigns to each pixel the only pixel value available, producing a
   * colorful pattern that simplifies following camera raw-patterns.
   *
   * Regard the following Bayer pattern, with green(G) and A and C being red
   * and blue or vice versa:
   *
   * \code
   * G01 C02 G03 C04 G05
   * A06 G07 A08 G09 A10
   * G11 C12 G13 C14 G15 
   * \endcode
   *
   * Then each pixel will get the only component available, setting the other
   * two to zero.
   *
   * @ingroup gAcquisition
   */
  class noDemosaicing : public bayerDemosaicing {
  public:
    /**
     * The parameters for the class noDemosaicing
     */
    class parameters : public bayerDemosaicing::parameters {
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
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
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
    noDemosaicing();

    /**
     * Construct a functor using the given parameters
     */
    noDemosaicing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    noDemosaicing(const noDemosaicing& other);

    /**
     * Destructor
     */
    virtual ~noDemosaicing();

    /**
     * Transforms a raw Bayer pattern image into an lti::image using
     * no interpolation as described in the class documentation.
     *
     * @param src contains a raw image of one of the parameters::eBayerPattern
     * @param dest the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src, image& dest) const;

    // make other apply member functions available
    using bayerDemosaicing::apply;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    noDemosaicing& copy(const noDemosaicing& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    noDemosaicing& operator=(const noDemosaicing& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual noDemosaicing* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual noDemosaicing* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Transform the pattern
     */
    bool pattern(const matrix<ubyte>& src,image& dest) const;
  };
}

#endif

