/*
 * Copyright (C) 2008
 * Claudia Goenner
 *
 * This file is backported from the Computer Vision and Robotics Library
 * (CVR-Lib ) for the LTI-Lib-2
 *
 * The CVR-Lib is free software; you can redistribute it and/or
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
 * \file   ltiRansacEstimation.h
 *         Contains the template class cvr::ransacEstimation<E>, used to
 *         estimate any transformation E with help of the RANSAC algorithm.
 *
 * \author Claudia Goenner (Original LTI-Lib class)
 * \author Pablo Alvarado (Adaptation to CVR-Lib model)
 * \date   13.02.2008
 *
 * revisions ..: $Id: ltiRansacEstimation.h,v 1.2 2009-11-12 03:54:46 alvarado Exp $
 */

#ifndef _LTI_RANSAC_ESTIMATION_H_
#define _LTI_RANSAC_ESTIMATION_H_


#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiUniformDiscreteDistribution.h"

namespace lti {

  /**
   * Template Class ransacEstimation<E>
   *
   * This class estimates a transform E using the RANSAC algorithm, which is a
   * robust estimation approach that maximizes the number of inliers. At each
   * iteration a subset of points/correspondences is drawn from which the
   * transform is computed.
   *
   * Theoretically the RANSAC algorithm copes with up to 90% outliers. It is
   * advised though, to verify the estimated transform and repeat the
   * estimation in case the result is not satisfactory.
   *
   * The template type E corresponds to a basic estimation class like:
   * - lti::euclideanTransformation2D
   * - lti::similarityTransformation2D
   * - lti::affineTransformation2D
   *
   * In any case, it is expected for that class E to provide the methods
   * - estimateLLS(const ivector&,const std::vector<P>&,const std::vector<P>&)
   * - dof()
   *
   * @see ransacEstimation::parameters.
   *
   * @ingroup gGeometricTrans
   */
  template<class E>
  class ransacEstimation : public functor {
  public:
    /**
     * The parameters for the class ransacEstimation
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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Returns the name of this parameter class.
       */
      virtual const std::string& name() const;

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

      /**
       * Maximal number of iterations used while trying to converge to a
       * solution.
       * 
       * Default: 50
       */
      int numberOfIterations;

      /**
       * Flag for automatic adjustment of the degree of contamination after
       * each successfull guess.
       *
       * The contamination is only decreased and never increased.  This
       * parameter has a direct effect on the number of iterations performed.
       * The functor will always terminate after at most the maximum
       * numberOfIterations is reached, though.
       *
       * If adaptive contamination is turned on, the apply-methods return true
       * even if the detected inliers suggest a contamination above the
       * parametrized contamination.
       *
       * Default: false.
       */
      bool adaptiveContamination;

      /**
       * The number of correspondences drawn at each trial to estimate the
       * transformation. Literature advises to use the minimum number
       * correspondences that are required which is proved optimal under a
       * statistical context.
       *
       * The minimum number of correspondences required to estimate a
       * transformation can be obtained with C=(dof()+1)/2 (assuming integer
       * division), where dof() is a method provided in the interface of the
       * estimators.  If the number provided here is less than that minimum C,
       * then C is used instead.
       *
       * Default: -1 (always use the minimum number of correspondences)
       * 
       */
      int numberOfCorrespondences;

      /**
       * The number of trials in adaptive mode depends on the estimated
       * contamination and the confidence, under which the result is correct.
       *
       * Default: .99
       */
      float confidence;

      /**
       * The expected degree of contamination.  This is the part of the total
       * correspondences that is assumed to be contaminated.
       *
       * Default: .5
       */
      float contamination;

      /**
       * The maximum error for a single correspondence or the averaged
       * size of the residual.
       *
       * Default: .8f
       */
      float maxError;

      /**
       * Some estimators allow configurations that you may want to control
       * explicitelly.  These are the parameters set to the basic estimator
       * before any computations are made.
       */
      typename E::parameters initialEstimationParameters;

      /**
       * Parameters for uniform discrete distribution.
       */
      uniformDiscreteDistribution::parameters rndParameters;
    };

    /**
     * Default constructor
     */
    ransacEstimation();

    /**
     * Construct a functor using the given parameters
     */
    ransacEstimation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    ransacEstimation(const ransacEstimation<E>& other);

    /**
     * Destructor
     */
    virtual ~ransacEstimation();

    /**
     * Estimates the transformation for the given set of correspondences.
     *
     * It will be assumed that for any valid index i the point setA[i] has a
     * correspondent point setB[i].
     *
     * @param setA first set of points of type P.
     * @param setB second set of points of type P.
     * @param result transformation parameters where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class P>
    bool apply(const std::vector<P>& setA,
               const std::vector<P>& setB,
                     typename E::parameters& result) const;

    /**
     * Estimates the transformation for the given set of correspondences.
     *
     * It will be assumed that for any valid index i the point setA[i] has a
     * correspondent point setB[i].
     *
     * @param setA first set of points of type P.
     * @param setB second set of points of type P.
     * @param inliers output container with the indices of the inliers detected
     * @param result transformation parameters where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class P>
    bool apply(const std::vector<P>& setA,
               const std::vector<P>& setB,
                     ivector& inliers,
                     typename E::parameters& result) const;
    
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ransacEstimation<E>& copy(const ransacEstimation<E>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ransacEstimation<E>& operator=(const ransacEstimation<E>& other);

    /**
     * Returns the name of this type, which depends on the template parameters.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual ransacEstimation<E>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ransacEstimation<E>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    virtual bool updateParameters();
  private:

    /**
     * Shadow of parameters (but fixed)
     */
    int numPointsPerTrial_; 

    /**
     * Shadow of parameters but logarithmic
     */
    double logConfidence_;
    
    /**
     * Get n random numbers.
     * It assumes that idx contains each of its own valid indices exactly once.
     */
    void getNRandom(uniformDiscreteDistribution& rnd,
                    const int n,
                    ivector& idx) const;

    /**
     * Take the points in setA, transform them with transformer into a
     * estB which are compare with each point in setB.  The squared distances
     * between estB and setB are stored in residual.
     */
    template<class P>
    void computeResidual(E& transformer,
                         const std::vector<P>& setA, 
                         const std::vector<P>& setB,
                         const float maxError,
                         fvector& residuals,
                         float& average,
                         ivector& inliers,
                         int& numInliers) const;
  };
}

#include "ltiRansacEstimation_template.h"

#endif

