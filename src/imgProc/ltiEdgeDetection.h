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
 * \file   ltiEdgeDetection.h
 *         Contains the class lti::edgeDetection, 
 *         which has still to be documented by Pablo Alvarado
 * 
 * \author Pablo Alvarado
 * \date   22.12.2008
 *
 * revisions ..: $Id: ltiEdgeDetection.h,v 1.1 2008-12-23 03:39:27 alvarado Exp $
 */

#ifndef _LTI_EDGE_DETECTION_H_
#define _LTI_EDGE_DETECTION_H_


#include "ltiFunctor.h"
#include "ltiChannel8.h"
#include "ltiChannel.h"

namespace lti {

  /**
   * Class edgeDetection
   *
   * Parent abstract class for all edge detectors.
   *
   * An edge detector finds the edges in an gray-valued image.  The
   * definition of "edge" is usually related to an abrupt change in
   * the intensity value of pixels.
   *
   * The result will be usually a channel8 containing only the two
   * values specified in the parameter to denote edge and no-edge.
   *
   * @see edgeDetection::parameters.
   *
   * @ingroup gEdgeCorner
   */
  class edgeDetection : public functor {
  public:
    /**
     * The parameters for the class edgeDetection
     */
    class parameters : public functor::parameters {
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
       * Value used to denote a no-edge pixel.
       *
       * The value must be between 0 and 255.  For channels the used
       * value will be scaled by 255.
       *
       * Default value: 0
       */
      ubyte noEdgeValue;

      /**
       * Value used to denote an edge pixel.
       *
       * The value must be between 0 and 255.  For channels the used
       * value will be scaled by 255.
       *
       * Default value: 255
       */
      ubyte edgeValue;

    };

    /**
     * Default constructor
     */
    edgeDetection();

    /**
     * Construct a functor using the given parameters
     */
    edgeDetection(const parameters& par);

    /**
     * Destructor
     */
    virtual ~edgeDetection();

    /**
     * Find the edges on the \c srcdest channel, and leave them in the same
     * container.
     *
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * Find the edges on the \c srcdest channel, and leave them in the same
     * container.
     *
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * Find the edges on the \c src channel, and leave them in the containter
     * \c dest.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const = 0;

    /**
     * Find the edges on the \c src channel, and leave them in the containter
     * \c dest.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const = 0;

    /**
     * Find the edges on the \c src channel, and leave them in the containter
     * \c dest.
     *
     * @param src channel with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel8& dest) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    edgeDetection& copy(const edgeDetection& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    edgeDetection& operator=(const edgeDetection& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual edgeDetection* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual edgeDetection* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

