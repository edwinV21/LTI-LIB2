/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Torsten Kaemper
 *
 * Copyright (C) 2009
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
 * \file   ltiMeanShiftTracker.h
 *         Contains the class lti::meanShiftTracker, used to track
 *         rectangular windows in image sequences.
 * 
 * \author Torsten Kaemper
 * \date   24.09.2001
 * \author Pablo Alvarado
 * \date   12.09.2009
 *
 * revisions ..: $Id: ltiMeanShiftTracker.h,v 1.1 2009-09-13 21:51:49 alvarado Exp $
 */

#ifndef _LTI_MEAN_SHIFT_TRACKER_H_
#define _LTI_MEAN_SHIFT_TRACKER_H_

#include "ltiFunctor.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiImage.h"
#include "ltiHistogram.h"

namespace lti {

  /**
   * Class meanShiftTracker
   *
   * This is the implementation of the Mean-Shift Tracking algorithm as
   * described in: Comaniciu, Ramesh and Meer, "Real-Time Tracking of Non-Rigid
   * Objects using the Mean Shift", IEEE Workshop on Applic.Comp.Vis.,2000, that
   * can be found at 
   * http://www.caip.rutgers.edu/riul/research/papers/pdf/trackmo.pdf
   *
   * It tracks a rectangular search window (the target)
   * in an image by its color distribution. Therefore
   * it uses an iterative gradient ascent algorithm, known as
   * mean shift, that finds a similarity peak between target
   * and candidate positions. The similarity is measured with
   * a distance metric, which can be obtained after each \c apply().
   *
   * Use this by calling an apply on subsequent images and thereby
   * passing the last returned search rectangle. Initialization is
   * done with first use or after a call of the reset() method.
   *
   * @see meanShiftTracker::parameters.
   *
   * @ingroup gTracker
   */
  class meanShiftTracker : public functor {
  public:
    /**
     * Kernel Type.
     */
    enum eKernelType {
      Normal,           /**< Normal Kernel */
      Epanechnikov      /**< Epanechnikov Kernel */
    };

    /**
     * The parameters for the class meanShiftTracker
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
       * Kernel type used in the estimation process.
       *
       * Available kernels are \c Normal or \c Epanechnikov 
       *
       * Default value: Normal
       */
      eKernelType kernelType;

      /**
       * Threshold for tracker validity.
       *
       * The valid range is from 0.0 to 1.0.
       *
       * If color distribution distance after apply is below this
       * threshold, then the tracker simply doesn't react.
       *
       * Default value: 0.8
       */
      float threshold;

      /**
       * Precision for iterations.
       * Use 0.1 for deep iteration , 10.0 for a fast runtime.
       *
       * Allowed range is [0.1, ... ]
       * 
       * Default value: 2.0
       */
      float precision;

      /**
       * The size adaption ratio tells how strong the size adaptation
       * may be within one call of an apply() method.
       *
       * Valid range is from 0.0 to 1.0 (reasonable values lie between 0.0 and
       * 0.5).
       *
       * Default value: 0.1
       */
      float sizeAdaptRatio;
    };

    /**
     * Default constructor
     */
    meanShiftTracker();

    /**
     * Construct a functor using the given parameters
     */
    meanShiftTracker(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    meanShiftTracker(const meanShiftTracker& other);

    /**
     * Destructor
     */
    virtual ~meanShiftTracker();

    /**
     * Tracks a target color distribution.
     *
     * The initial target is specified by "window" on first usage or after a
     * reset() (i.e. tracker is not initialized).  Subsequent calls of this
     * method perform tracking by translating and scaling the "window" to
     * optimally fit the target color distribution.
     *
     * @param src input image
     * @param window output rectangle
     * @return true if successful, of false otherwise
     */
    bool apply(const image& src,irectangle& window);

    /**
     * Tells, if the tracker has already been initialized with a
     * target color distribution.
     *
     * @return true if successful, of false otherwise
     */
    bool isInitialized() const;

    /**
     * Explicitly initialize the tracker with a given region in the image.
     *
     * This is called automatically when the apply() method is called and the 
     * tracker has not been initialised; i.e. calling initialize() is
     * the same as subsequently calling reset() and apply().
     *
     * @param src input image
     * @param window output rectangle
     * @return true if successful, of false otherwise
     */
    void initialize(const image& src,irectangle& window);

    /**
     * Explicitly initialize the tracker with a given region in the image.
     *
     * Additionally weight all pixels with the value in mask.
     * You must call this explicitly, if you want to initialize
     * with a weighting mask.
     *
     * @param src input image
     * @param mask weights mask
     * @param window output rectangle
     * @return true if successful, of false otherwise
     */
    void initialize(const image& src,                 
                    const channel8& mask,
                    irectangle& window);

    /**
     * Explicitly initialize tracker with given region in image.
     *
     * Additionally weight all pixels with the value in mask.
     * You must call this explicitly, if you want to initialize
     * with a weighting mask.
     */
    void initialize(const image& src,
                    const channel& mask,
                    irectangle& window);

    /**
     * Resets the tracker. 
     *
     * Next call of apply initializes it again.
     */
    void reset();

    /**
     * Tells if returned window is valid. This depends on the threshold
     * parameter. If the color distribution distance between target and
     * candidate window is above threshold, then this method returns
     * false.
     */
    bool isValid() const;

    /**
     * Returns the distance between the color distribution of the
     * initial target and the last returned window.
     */
    float getDistance() const;

    /**
     * Returns the center of the tracked candidate distribution.
     *
     * In general, this is the returned search windows center,
     * except that the winwod is at the fringe of the image.
     */
    fpoint getCenter() const;

    /**
     * Returns a const reference to the histogram of the target.
     *
     * The histogram only changes on initialization.
     */
    const fhistogram& getTargetHistogram() const;

    /**
     * Returns a const reference to the histogram of the tracked candidate.
     *
     * The histogram changes after each call of an apply method.
     */
    const fhistogram& getCandidateHistogram() const;


    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    meanShiftTracker& copy(const meanShiftTracker& other);

    /**
     * Alias for copy member.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    meanShiftTracker& operator=(const meanShiftTracker& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual meanShiftTracker* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual meanShiftTracker* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();
    
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
    
  private:

    enum {
      Dim = 3,
      CellsPerDim = 32
    };

    /**
     * Struct for storing the tracker data after call of any apply method
     */
    class trackerState : public ioObject {
    public:
      /**
       * Constructor
       */
      trackerState();

      /**
       * Copy Constructor
       */
      trackerState(const trackerState& other);

      /**
       * Destructor
       */
      ~trackerState();

      /**
       * Clear class contents
       */
      void clear();

      /**
       * Copy the state
       */
      trackerState& copy(const trackerState& other);

      /**
       * Alias for copy
       */
      trackerState& operator=(const trackerState& other);

      /**
       * Final center position y1 (after iterations)
       */
      fpoint y1;

      /**
       * Final Bhattacharyya coefficient for position y1
       */
      float bhat1;

      /**
       * Target color probability
       */
      fhistogram targetProb;

      /** 
       * Candidate color probability for pos y1
       */      
      fhistogram candProb;

      /**
       * Distance and height to width ratio of tracking box
       */
      float distance, hwRatio;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
      
      /**
       * Returns a copy of this object
       */
      virtual ioObject* clone() const;
      
      /**
       * Returns a copy of this object
       */
      virtual ioObject* newInstance() const;
      
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
    };

    /**
     * calculates the bhattacharyya metric for measuring
     * similarity of two color distributions
     */
    float calcBhatCoef(const fhistogram& targetProb,
                       const fhistogram& candProb) const;

    /**
     * fills the histogram "prob" with the colors inside the
     * rectangle "canvas" in the image. Each color is weighted
     * with a kernel function. Note: the histogramm is not
     * normalized to 1, thus use prob.getProbability() to obtain
     * probabilities.
     */
    void calcProb(fhistogram& prob,
                  const irectangle& canvas,
                  const image& src) const;

    /**
     * fills the histogram "prob" with the colors inside the
     * rectangle "canvas" in the image. Each color is weighted
     * with a kernel function AND the weight given in mask.
     * Note: the histogramm is not normalized to 1, thus use
     * prob.getProbability() to obtain probabilities.
     */
    void calcProb(fhistogram& prob,
                  const irectangle& canvas,
                  const image& src,
                  const channel8& mask) const;

    /**
     * Fills the histogram "prob" with the colors inside the
     * rectangle "canvas" in the image. 
     *
     * Each color is weighted with a kernel function AND the weight given in
     * mask.
     *
     * Note: the histogramm is not normalized to 1, thus use
     * prob.getProbability() to obtain probabilities.
     */
    void calcProb(fhistogram& prob,
                  const irectangle& canvas,
                  const image& src,
                  const channel& mask) const;


    /**
     * Returns the Normal or Epanechikov kernel for the given position
     */
    float kernel(const ipoint& cen,
                 const ipoint& pos,
                 const ipoint& h) const;

    /**
     * Calculates the color distribution distance between two histogramms
     */
    inline float calcDistance(const fhistogram& targetProb,
                              const fhistogram& candProb) const;

    /**
     * Shifts and clips rect to fit into given canvas
     */
    void correctRect(irectangle& rect,
                     const irectangle& canvas) const;

    /**
     * Returns the derivate of the Epanechikov kernel
     */
    float derivateKernel(const ipoint& cen,
                         const ipoint& pos,
                         const ipoint& h) const;

    /**
     * Stores results of last call of one apply method (the state of the
     * tracker)
     */
    trackerState td_;

    /**
     * indicates if the tracker is initialized
     */
    bool initialized_;

    /**
     * indicates if the last apply was a valid tracking step
     */
    bool valid_;

    /**
     * Shadow for the parameter kernelType
     */
    eKernelType kernelType_;
  };

  /**
   * Read function for meanShiftTracker::eKernelType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,meanShiftTracker::eKernelType& data);

  /**
   * Write function for meanShiftTracker::eKernelType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const meanShiftTracker::eKernelType& data);


}

#endif

