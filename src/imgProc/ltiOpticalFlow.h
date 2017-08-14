/*
 * Copyright (C) 2012
 * Carlos Gomez, Pablo Alvarado
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
 * \file   ltiOpticalFlow.h
 *         Contains the abstract class lti::opticalFlow, 
 *         which defines the interface of all optical flow classes
 * 
 * \author Carlos Gomez
 * \author Pablo Alvarado
 * \date   11.10.2012
 *
 * revisions ..: $Id: ltiOpticalFlow.h,v 1.3 2012-11-02 02:32:48 alvarado Exp $
 */

#ifndef _LTI_OPTICAL_FLOW_H_
#define _LTI_OPTICAL_FLOW_H_

#include "ltiFunctor.h"
#include "ltiLocation.h"
#include "ltiList.h"
#include "ltiChannel.h"

namespace lti {

  /**
   * Class opticalFlow
   *
   * This is an abstract class, which should be used as parent of all
   * optical flow methods.
   *
   * @ingroup gOpticalFlow
   */
  class opticalFlow : public functor {
  public:

    typedef functor::parameters parameters;

    /**
     * Default constructor
     */
    opticalFlow();

    /**
     * Destructor
     */
    virtual ~opticalFlow();

    /**
     * Operates on the given argument.
     *
     * @param chnl1OrU channel with the source data (frame 1). The result
     *                will be left here too (u velocity vector).
     * @param chnl2OrV channel with the source data (frame 2). The result
     *                will be left here too (v velocity vector).
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& chnl1OrU, channel& chnl2OrV);

    /**
     * Estimate the optical flow from the first frame to the second one.
     *
     * @param chnl1 channel with the source data (frame 1).
     * @param chnl2 channel with the source data (frame 2).
     * @param u channel where the result will be left (u velocity vector).
     * @param v channel where the result will be left (v velocity vector).
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& chnl1, const channel& chnl2,
                       channel& u, channel& v);

    /**
     * Estimate the optical flow with the last given channel and the
     * next one
     *
     * You should call first the apply method that expects two input frames for
     * the functor to be properly initialized.  If this method is called first,
     * then false will be returned the first time, as no previous frame is
     * known
     *
     * @param next channel with the next frame
     * @param u channel where the result will be left (u velocity vector).
     * @param v channel where the result will be left (v velocity vector).
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& next,channel& u,channel& v);

    /**
     * Estimate the optical flow from the first frame to the second one only
     * for the locations given.
     *
     * @param last channel with the source data (previous frame).
     * @param next channel with the source data (next frame).
     * @param locs1 Locations in where the optical flow must be computed.
     * @param u channel where the result will be left (u velocity vector).
     * @param v channel where the result will be left (v velocity vector).
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& last,
                       const channel& next,
                       const list<location>& locs1,
                       channel& u,
                       channel& v);

    /**
     * Estimate the optical flow from the first frame to the second one only
     * for the locations given.
     *
     * This method generates two vectors containing the positions of the given
     * locations (locs1) stored in the first vector, and the tracked locations
     * in the "next" channel, stored in the second vector.  These vectors can
     * used in estimators like lti::ransacEstimation.
     *
     * @param last channel with the source data (previous frame).
     * @param next channel with the source data (next frame).
     * @param locs1 Locations in where the optical flow must be computed.
     * @param u channel where the result will be left (u velocity vector).
     * @param v channel where the result will be left (v velocity vector).
     * @param pts1f vector of positions of the locations in the channel "last"
     * @param pts2f vector of corresponding positions in the channel "next",
     *              computed with the optical flow
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& last,
                       const channel& next,
                       const list<location>& locs1,
                       channel& u,
                       channel& v,
                       std::vector<fpoint>& pts1f,
                       std::vector<fpoint>& pts2f);


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlow& copy(const opticalFlow& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlow& operator=(const opticalFlow& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual opticalFlow* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual opticalFlow* newInstance() const = 0;

  protected:

    /**
     * Compute flow
     *
     * Computes the real flow assuming that both images are already
     * checked to have the same size
     */
    virtual bool computeFlow(const channel& chnl1, 
                             const channel& chnl2,
                             channel& u,
                             channel& v) = 0;

    /**
     * Estimate the optical flow from the first frame to the second one only
     * for the locations given.
     *
     * @param last channel with the source data (previous frame).
     * @param next channel with the source data (next frame).
     * @param locs1 Locations in where the optical flow must be computed.
     * @param u channel where the result will be left (u velocity vector).
     * @param v channel where the result will be left (v velocity vector).
     * @return true if apply successful or false otherwise.
     */
    virtual bool computeFlow(const channel& last,
                             const channel& next,
                             const list<location>& locs1,
                             channel& u,
                             channel& v) = 0;

    /**
     * Estimate the optical flow from the first frame to the second one only
     * for the locations given.
     *
     * @param last channel with the source data (previous frame).
     * @param next channel with the source data (next frame).
     * @param locs1 Locations in where the optical flow must be computed.
     * @param u channel where the result will be left (u velocity vector).
     * @param v channel where the result will be left (v velocity vector).
     * @param pts1f vector of positions of the locations in the channel "last"
     * @param pts2f vector of corresponding positions in the channel "next",
     *              computed with the optical flow
     * @return true if apply successful or false otherwise.
     */
    virtual bool computeFlow(const channel& last,
                             const channel& next,
                             const list<location>& locs1,
                             channel& u,
                             channel& v,
                             std::vector<fpoint>& pts1f,
                             std::vector<fpoint>& pts2f) = 0;

    /**
     * Last frame
     */
    channel last_;


  };
}

#endif

