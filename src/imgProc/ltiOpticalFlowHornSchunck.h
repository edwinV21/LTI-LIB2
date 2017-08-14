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
 * \file   ltiOpticalFlowHornSchunck.h
 *         Contains the class lti::opticalFlowHornSchunck, 
 *         which implements the classic algorithm of Horn and Schunck for 
 *         global optical flow estimation.
 * 
 * \author Carlos Gomez
 * \author Pablo Alvarado
 * \date   28.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlowHornSchunck.h,v 1.5 2013-02-17 02:19:48 alvarado Exp $
 */

#ifndef _LTI_OPTICAL_FLOW_HORN_SCHUNCK_H_
#define _LTI_OPTICAL_FLOW_HORN_SCHUNCK_H_


#include "ltiGradientFunctor.h"
#include "ltiList.h"
#include "ltiLocation.h"
#include "ltiConvolution.h"

#include "ltiOpticalFlow.h"

namespace lti {

  
  /**
   * Class opticalFlowHornSchunck
   *
   * The method proposed by Berthold K.P. Horn and Brian G. Schunck estimates
   * optical flow with a global method which introduces a global constraint of
   * smoothness to solve the aperture problem.
   *
   * It is proposed in the paper:
   *
   * B.K.P. Horn and B.G. Schunck, "Determining optical flow." 
   * Artificial Intelligence, vol 17, pp 185-203, 1981
   *
   * @see opticalFlowHornSchunck::parameters.
   *
   * @ingroup gOpticalFlow
   */
  class opticalFlowHornSchunck : public opticalFlow {
  public:
    /**
     * The parameters for the class opticalFlowHornSchunck
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
       * Regularizarion constant (lambda).
       *
       * Default value: 0.4
       */
      float lambda;

      /**
       * Size of gaussian kernel
       *
       * Default value: 5
       */
      int kernelSize;
      
      /**
       * Variance of the gauss kernel
       *
       * Default value: 1.3
       */
      float variance;
      
      /**
       * Maximum number of iterations.
       *
       * Default value: 20
       */
      int maxIterations;
      
      /**
       * Maximum error permited.
       *
       * Default value: 0.01
       */
      float maxError;
      
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
    opticalFlowHornSchunck();

    /**
     * Construct a functor using the given parameters
     */
    opticalFlowHornSchunck(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    opticalFlowHornSchunck(const opticalFlowHornSchunck& other);

    /**
     * Destructor
     */
    virtual ~opticalFlowHornSchunck();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowHornSchunck& copy(const opticalFlowHornSchunck& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowHornSchunck& operator=(const opticalFlowHornSchunck& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual opticalFlowHornSchunck* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual opticalFlowHornSchunck* newInstance() const;

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
    virtual bool apply(const channel& next,
                       channel& u,channel& v);

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
    virtual bool apply(const channel& last,
                       const channel& next,
                       const list<location>& locs1,
                       channel& u,
                       channel& v,
                       std::vector<fpoint>& pts1f,
                       std::vector<fpoint>& pts2f);

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
     * Last velocity averages
     */
    channel u_,v_;
    

  protected:

    /**
     * Compute flow
     *
     * Computes the real flow assuming that the both images are already
     * checked to have the same size
     */
    
    virtual bool computeFlow(const channel& last, 
                             const channel& next,
                             channel& u,
                             channel& v);
	
    /**
     * Estimate the optical flow from the first frame to the second one just
     * for the locations given.
     *
     * @param chnl1 channel with the source data (frame 1).
     * @param chnl2 channel with the source data (frame 2).
     * @param locs1 locations where the optical flow must be computed.
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
                             channel& v);


    /**
     * Estimate the optical flow from the first frame to the second one just
     * for the locations given.
     *
     * @param chnl1 channel with the source data (frame 1).
     * @param chnl2 channel with the source data (frame 2).
     * @param locs1 locations where the optical flow must be computed.
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

	
  };
}

#endif

