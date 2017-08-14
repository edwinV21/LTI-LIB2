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
 * \file   ltiOpticalFlowLucasKanade.h
 *         Contains the class lti::opticalFlowLucasKanade, 
 *         which implements the classic algorithm of Lucas and Kanade.
 * 
 * \author Carlos Gomez
 * \author Pablo Alvarado
 * \date   28.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlowLucasKanade.h,v 1.4 2012-11-02 02:32:48 alvarado Exp $
 */

#ifndef _LTI_OPTICAL_FLOW_LUCAS_KANADE_H_
#define _LTI_OPTICAL_FLOW_LUCAS_KANADE_H_

#include "ltiList.h"
#include "ltiOpticalFlow.h"
#include "ltiGradientFunctor.h"
#include "ltiConvolution.h"
#include "ltiLocation.h"

namespace lti {

  /**
   * Class opticalFlowLucasKanade
   *
   * The method of Bruce D. Lucas and Takeo Kanade for the computation of
   * optical flow was proposed in
   *
   * B. D. Lucas and T. Kanade, An iterative image registration
   * technique with an application to stereo vision.  Proceedings of Imaging
   * Understanding Workshop, pages 121--130, 1981
   *
   * It assumes that the flow is essentially constant in a local neighbourhood
   * of the pixel under consideration, and solves the basic optical flow
   * equations for all the pixels in that neighbourhood, by the least squares
   * criterion.
   *
   * @see opticalFlowLucasKanade::parameters.
   *
   * @ingroup gOpticalFlow
   */
  class opticalFlowLucasKanade : public opticalFlow {
  public:
    /**
     * The parameters for the class opticalFlowLucasKanade
     */
    class parameters : public opticalFlow::parameters {
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
       * Variance of the gaussian filter used to give more importance to
       * the pixels close to the window center.
       *
       * Default value: 1e10 (i.e. aproximately a square filter)
       */
      float variance;

      /**
       * Size of the window side used to compute the flow.
       *
       * It is the size of the square window.
       *
       * Default value: 5
       */
      int kernelSize;

      /**
       * Gradient parameters
       *
       * The format is always overwritten to use cartesian coordinates.
       *
       * Default kernel: lti::gradientFunctor::Ando
       */
      lti::gradientFunctor::parameters gradient;

    };

    /**
     * Default constructor
     */
    opticalFlowLucasKanade();

    /**
     * Construct a functor using the given parameters
     */
    opticalFlowLucasKanade(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    opticalFlowLucasKanade(const opticalFlowLucasKanade& other);

    /**
     * Destructor
     */
    virtual ~opticalFlowLucasKanade();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowLucasKanade& copy(const opticalFlowLucasKanade& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowLucasKanade& operator=(const opticalFlowLucasKanade& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual opticalFlowLucasKanade* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual opticalFlowLucasKanade* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     *
     * This sets up all necessary functors to save time later on
     */
    virtual bool updateParameters();

  private:
    /**
     * Gradient functor
     */
    gradientFunctor grad_;

    /**
     * Convolution functor
     */
    convolution filter_;

  protected:
    /**
     * Compute flow
     *
     * Computes the real flow assuming that the both images are already
     * checked to have the same size
     */
    virtual bool computeFlow(const channel& chnl1, 
                             const channel& chnl2,
                             channel& u,
                             channel& v);

    /**
     * Compute flow
     *
     * Computes the real flow assuming that the both images are already
     * checked to have the same size
     */
    virtual bool computeFlow(const channel& chnl1, 
                             const channel& chnl2,
                             const list<location>& locs1,
                             channel& u,
                             channel& v);

    /**
     * Compute flow
     *
     * Computes the real flow assuming that the both images are already
     * checked to have the same size
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
    virtual bool computeFlow(const channel& chnl1, 
                             const channel& chnl2,
                             const list<location>& locs1,
                             channel& u,
                             channel& v,
                             std::vector<fpoint>& pts1f,
                             std::vector<fpoint>& pts2f);

    /**
     * Shadows of temporal channels to avoid memory allocation times
     */
    channel Iyy_,Ixx_,Ixy_,Ixt_,Iyt_;

  };
}

#endif

