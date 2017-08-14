/*
 * Copyright (C) 2010
 * Pedro Elías Alpízar Salas
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
 * \file   ltiCoherenceEnhancingDiffusion.h
 *         Contains the class lti::coherenceEnhancingDiffusion,
 *         which has still to be documented by Pedro Elías Alpízar Salas
 *
 * \author Pedro Elías Alpízar Salas
 * \date   23.08.2010
 *
 * revisions ..: $Id: ltiCoherenceEnhancingDiffusion.h,v 1.5 2012-07-05 17:39:04 alvarado Exp $
 */

#ifndef _LTI_COHERENCE_ENHANCING_DIFFUSION_H_
#define _LTI_COHERENCE_ENHANCING_DIFFUSION_H_

#include "ltiFunctor.h"
#include "ltiGradientFunctor.h"
#include "ltiBoundaryType.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

namespace lti {

  /**
   * Class coherenceEnhancingDiffusion
   *
   * The class lti:coherenceEnhanchingDiffusion make a diffusion of the image
   * ussing the natural flux of the image.
   *
   * More information about can be found in the paper:
   *
   * Martin Welk, Joachim Weickert and Gabriele Steidl. From Tensor-Driven
   * Diffusion to Anisotropic Wavelet Shrinkage, 2006. Available at:
   *
   * www.mia.uni-saarland.de/Publications/welk-eccv06.pdf
   *
   * @see coherenceEnhancingDiffusion::parameters.
   *
   *
   * @ingroup gNonLinearFilters
   */
  class coherenceEnhancingDiffusion : public functor {
  public:
    /**
     * The parameters for the class coherenceEnhancingDiffusion
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
       * Type of gradient kernel to be used.
       *
       * Default value: Difference
       *
       * @see gradientFunctor::eKernelType
       */
      gradientFunctor::eKernelType gradientKernelType;

      /**
       * Size of the kernel used.
       *
       * This attribute is used only for the Optimal and OGD Kernel types.
       *
       * For the Optimal kernel type, the value \b must be 3, 4, or 5.
       * For the OGD odd values greater or equal 3 should be used.
       *
       * Default value: 3
       */
      int gradientKernelSize;

      /**
       * Variance used for the Gaussian.
       *
       * This argument has effect only if kernelType is OGD.
       *
       * It is the variance use for the Gaussian to be derived as approximation
       * for a gradient kernel.
       *
       * Default value: -1 (i.e. the variance will be computed from the size
       *                    of the kernel (\a gradientKernelSize) )
       */
      float ogdVariance;

      /**
       * Type of the boundary used to do the Gaussian diffussion of the image
       * and the Jacobian.
       *
       * Default value: Zero
       *
       * @see lti::eBoundaryType
       */
      eBoundaryType smootBoundary;

      /**
       * The sigma value for the Gaussian kernel to smoot the image.
       *
       * Default value: 1
       *
       */
      float gaussSmoothing;

      /**
       * The sigma value for the Gaussian kernel to smoot the Jacobian.
       *
       * Default value: 1
       *
       */
      float gaussSmoothingHessian;

      /**
       * Put it true if you whant to filer the gradient with some angle.
       * The filter use a rectangular window with a cosein function at the end.
       *
       * Default value: false
       *
       */
      bool  phaseFilter;

      /**
       * The value where is center the filter of values of the gradient.
       *
       * If the given value is greater than Pi then it is assumed to be given
       * in degrees.
       *
       * Default value: 0
       *
       */
      float centerPhaseWindow;

      /**
       * The width of the rectangular window centered in the centerPhaseWindow
       * value.
       * If the given value is greater than Pi/2 then it is assumed to be given
       * in degrees.
       *
       * Default value: \f$ \frac{\pi}{2} \f$
       *
       */
      float phaseWindowWidth;

      /**
       * The width of the cosein function at the end of the rectangular window.
       *
       * If the given value is greater than Pi/2 then it is assumed to be given
       * in degrees.
       *
       * Default value: \f$ \frac{\pi}{2} \f$
       *
       */
      float phaseCosWidth;

      /**
       * Is the importance of the diffusion on the surce image.
       * Must be a positive value less than one
       *
       * Default value: 0.15
       *
       */
      float stepDiffusionTime;

      /**
       * The functions iterate n times until
       * \f$ n*stepDiffusionTime > totalDiffusionTime\f$
       *
       * Default value: 5
       *
       */
      float totalDiffusionTime;

      /**
       * Constant value in the formula for calculate the eigenvector values.
       *
       * Default value: 0.001
       *
       */
      float smallRegularisation;

      /**
       * Multiply the exponet in the creation of the eigenvectors values.
       *
       * Default value: 1
       *
       */
      float diffTensorExponent;

      /**
       * Constant value in the formula for calculate the eigenvector values.
       *
       *
       * Default value: 1e-10
       *
       */
      float contrast;


    };

    /**
     * Default constructor
     */
    coherenceEnhancingDiffusion();

    /**
     * Construct a functor using the given parameters
     */
    coherenceEnhancingDiffusion(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    coherenceEnhancingDiffusion(const coherenceEnhancingDiffusion& other);

    /**
     * Destructor
     */
    virtual ~coherenceEnhancingDiffusion();


    /**
     * Computes a CED on the given argument.
     *
     * @param srcdest channel with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Computes a CED on the given argument.
     *
     * @param srcdest channel8 with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Computes a CED on the given arguments.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, channel& dest)const;

    /**
     * Computes a CED on the given arguments.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, channel8& dest) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    coherenceEnhancingDiffusion& copy(const coherenceEnhancingDiffusion& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    coherenceEnhancingDiffusion& operator=
                                    (const coherenceEnhancingDiffusion& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual coherenceEnhancingDiffusion* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual coherenceEnhancingDiffusion* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

    /**
     * Apply the phase filter over the magnitude of the gradient
     * @param mag Surce magnitude
     * @param phase Phase of the gradient
     * @param destMag Filtered magnitude
     * @return true if successful, false otherwise
     */
    bool phaseFilterFunction(const float& mag,
                             float        phase,
                             float&       destMag) const;


  protected:
    /**
     * Take the smoot image and compute the Jacobian for that image
     * @param gaussSrc smoot source channel
     * @param destJXX xx component of the Jacobian
     * @param destJXY xy component of the Jacobian
     * @param destJYY yy component of the Jacobian
     * @return true if successful, false otherwise
     */
    bool structureTensor(const channel& gaussSrc,
                               channel&       destJXX,
                               channel&       destJXY,
                               channel&       destJYY) const;

    /**
     * Take the Jacobian of a point and compute the eigenvectors to it
     * @param jXX Jacobian value
     * @param jXY Jacobian value
     * @param jYY Jacobian value
     * @param mu1 First eigen value
     * @param mu2 Second eigen value
     * @param v1X X componente of the first eigenvector
     * @param v1Y Y componente of the first eigenvector
     * @param v2X X componente of the second eigenvector
     * @param v2Y Y componente of the second eigenvector
     * @return true if successful, false otherwise
     */
    bool eigenVector(const channel::value_type& jXX,
                            const channel::value_type& jXY,
                            const channel::value_type& jYY,
                            float&                     mu1,
                            float&                     mu2,
                            float&                     v1X,
                            float&                     v1Y,
                            float&                     v2X,
                            float&                     v2Y) const;


    /**
     * Compute the diffusion tensor
     * @param jXX xx Jacobian channel
     * @param jXY xy Jacobian channel
     * @param jYY yy Jacobian channel
     * @param dXX Result xx Tensor channel
     * @param dXY Result xy Tensor channel
     * @param dYY Result yy Tensor channel
     * @return true if successful, false otherwise
     */
    bool structDiffusionTensor(const channel& jXX,
                               const channel& jXY,
                               const channel& jYY,
                               channel&       dXX,
                               channel&       dXY,
                               channel&       dYY) const;

    /**
     * Compute the diffusion tensor with diferent exponent
     * @param jXX xx Jacobian channel
     * @param jXY xy Jacobian channel
     * @param jYY yy Jacobian channel
     * @param tensorExpo tensor exponent
     * @param dXX Result xx Tensor channel
     * @param dXY Result xy Tensor channel
     * @param dYY Result yy Tensor channel
     * @return true if successful, false otherwise
     */
    bool structDiffusionTensor(const channel& jXX,
                               const channel& jXY,
                               const channel& jYY,
                               const float    tensorExpo,
                               channel&       dXX,
                               channel&       dXY,
                               channel&       dYY) const;

    /**
     * Compute the diffusion and apply to the image
     * @param src Source channel
     * @param dXX xx Tensor channel
     * @param dXY xy Tensor channel
     * @param dYY yy Tensor channel
     * @param dest Destination channel
     * @return true if successful, false otherwise
     */
    bool diffusionSchemeRotInvariance(const channel& src,
                                      const channel& dXX,
                                      const channel& dXY,
                                      const channel& dYY,
                                      channel&       dest) const;



    float centerPhaseWindow_;


    float phaseWindowWidth_;

    float phaseCosWidth_;

    float maxWinPh_;
    float minWinPh_;
    float maxFilPh_;
    float minFilPh_;

    float filterTable_ [1024];
  };
}

#endif

