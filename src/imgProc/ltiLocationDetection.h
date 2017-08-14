/*
 * Copyright (C) 2007
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
 * \file   ltiLocationDetection.h
 *         Contains the abstract class lti::locationDetection, which 
 *         is the parent class of all local detection classes.
 * 
 * \author Pablo Alvarado
 * \date   13.12.2007
 *
 * revisions ..: $Id: ltiLocationDetection.h,v 1.1 2007-12-19 02:54:36 alvarado Exp $
 */

#ifndef _LTI_LOCATION_DETECTION_H_
#define _LTI_LOCATION_DETECTION_H_

#include "ltiChannel.h"
#include "ltiLocation.h"
#include "ltiChannel8.h"
#include "ltiList.h"
#include "ltiBoundaryType.h"
#include "ltiFunctor.h"

namespace lti {

  /**
   * Abstract class locationDetection
   *
   * The location detection abstract class is the parent class of all location
   * detectors in the LTI-Lib.
   *
   * @see locationDetection::parameters.
   *
   * @ingroup gFeatureExtr
   */
  class locationDetection : public functor {
  public:
    /**
     * The parameters for the class locationDetection
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
       * Boundary Type
       *
       * Specify how to consider the regions outside the given image.
       *
       * Default value: Constant
       */
      eBoundaryType boundaryType;
    };

    /**
     * Default constructor
     */
    locationDetection();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    locationDetection(const locationDetection& other);

    /**
     * Destructor
     */
    virtual ~locationDetection();

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, list<location>& locs) const = 0;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src, list<location>& locs) const = 0;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * This method provides numLocs = locs.size() directly.  The reason is
     * simple: speed.  The size() method needs to count the elements, and
     * this is usually done in the process.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param numLocs number of locations detected.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, 
                       list<location>& locs,
                       int& numLocs) const = 0;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * This method provides numLocs = locs.size() directly.  The reason is
     * simple: speed.  The size() method needs to count the elements, and
     * this is usually done in the process.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param numLocs number of locations detected.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                       list<location>& locs,
                       int& numLocs) const = 0;

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual locationDetection* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual locationDetection* newInstance() const = 0;

    /**
     * Copy member
     */
    locationDetection& copy(const locationDetection& other);
  
    /**
     * Alias for copy member
     */
    locationDetection& operator=(const locationDetection& other);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

