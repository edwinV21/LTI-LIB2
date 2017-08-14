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
 * \file   ltiRansacEstimation.cpp
 *         Contains the template class ransacEstimation,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   13.02.2008
 *
 * revisions ..: $Id: ltiRansacEstimation_template.h,v 1.2 2012-10-04 16:45:30 alvarado Exp $
 */

#include "ltiEuclidianDistantor.h"
#include "ltiRound.h"

namespace lti {
  // --------------------------------------------------
  // ransacEstimation<E>::parameters
  // --------------------------------------------------

  // default constructor
  template<class E>
  ransacEstimation<E>::parameters::parameters()
    : functor::parameters() {
    
    numberOfIterations = 50;
    numberOfCorrespondences = -1;
    confidence = 0.99f;
    contamination = 0.5f;
    maxError = 0.8f;
    adaptiveContamination = false;
    
  }

  // copy constructor
  template<class E>
  ransacEstimation<E>::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  template<class E>
  ransacEstimation<E>::parameters::~parameters() {
  }

  // copy member

  template<class E>
  typename ransacEstimation<E>::parameters&
  ransacEstimation<E>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    numberOfIterations = other.numberOfIterations;
    numberOfCorrespondences = other.numberOfCorrespondences;
    confidence = other.confidence;
    contamination = other.contamination;
    maxError = other.maxError;
    adaptiveContamination = other.adaptiveContamination;
    initialEstimationParameters = other.initialEstimationParameters;
    rndParameters = other.rndParameters;
    return *this;
  }

  // alias for copy method
  template<class E>
  typename ransacEstimation<E>::parameters&
  ransacEstimation<E>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  template<class E>
  const std::string&
  ransacEstimation<E>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  template<class E>
  typename ransacEstimation<E>::parameters*
  ransacEstimation<E>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template<class E>
  typename ransacEstimation<E>::parameters* 
  ransacEstimation<E>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template<class E>
  bool ransacEstimation<E>::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"numberOfIterations",numberOfIterations);
      lti::write(handler,"numberOfCorrespondences",numberOfCorrespondences);
      lti::write(handler,"confidence",confidence);
      lti::write(handler,"contamination",contamination);
      lti::write(handler,"maxError",maxError);
      lti::write(handler,"adaptiveContamination",adaptiveContamination);
      lti::write(handler,"initialEstimationParameters",
                 initialEstimationParameters);
      lti::write(handler,"rndParameters",rndParameters);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  template<class E>
  bool ransacEstimation<E>::parameters::read(ioHandler& handler,
                                           const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"numberOfIterations",numberOfIterations);     
      lti::read(handler,"numberOfCorrespondences",numberOfCorrespondences);
      lti::read(handler,"confidence",confidence);
      lti::read(handler,"contamination",contamination);
      lti::read(handler,"maxError",maxError);
      lti::read(handler,"adaptiveContamination",adaptiveContamination);
      lti::read(handler,"initialEstimationParameters",
                initialEstimationParameters);
      lti::read(handler,"rndParameters",rndParameters);      
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // ransacEstimation
  // --------------------------------------------------

  // default constructor
  template<class E>
  ransacEstimation<E>::ransacEstimation()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template<class E>
  ransacEstimation<E>::ransacEstimation(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class E>
  ransacEstimation<E>::ransacEstimation(const ransacEstimation<E>& other)
    : functor() {
    copy(other);
  }

  // destructor
  template<class E>
  ransacEstimation<E>::~ransacEstimation<E>() {
  }

  // copy member
  template<class E>
  ransacEstimation<E>& 
  ransacEstimation<E>::copy(const ransacEstimation<E>& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class E>
  ransacEstimation<E>&
  ransacEstimation<E>::operator=(const ransacEstimation<E>& other) {
    return (copy(other));
  }

  // class name
  template<class E>
  const std::string&
  ransacEstimation<E>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template<class E>
  ransacEstimation<E>* ransacEstimation<E>::clone() const {
    return new ransacEstimation<E>(*this);
  }

  // newInstance member
  template<class E>
  ransacEstimation<E>* ransacEstimation<E>::newInstance() const {
    return new ransacEstimation<E>();
  }

  // return parameters
  template<class E>
  const typename ransacEstimation<E>::parameters&
  ransacEstimation<E>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  template<class E>
  bool ransacEstimation<E>::updateParameters() {
    E dummyEstimator;
    const parameters& par = getParameters();
    numPointsPerTrial_    = max(par.numberOfCorrespondences,
                                (dummyEstimator.dof()+1)/2);
    logConfidence_ = log10(1.0-par.confidence);
    return true;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  template<class E>
  void ransacEstimation<E>::getNRandom(uniformDiscreteDistribution& rnd,
                                       const int n,
                                       ivector& idx) const {
    for (int i=0;i<n;++i) {
      int j = rnd.rand();
      swap(idx.at(j),idx.at(i));
    }
  }

    /**
     * Take the points in setA, transform them with transformer into a
     * estB which are compare with each point in setB.  The squared distances
     * between estB and setB are stored in residual.
     */
  template<class E>
  template<class P>
  void ransacEstimation<E>::computeResidual(E& transformer,
                                            const std::vector<P>& setA, 
                                            const std::vector<P>& setB,
                                            const float maxError,
                                            fvector& residual,
                                            float& average,
                                            ivector& inliers,
                                            int& numInliers) const {
    residual.allocate(setA.size());
    inliers.allocate(setA.size());
    euclidianSqrDistantor<P> dist;
    P est;
    average = 0.0f;
    numInliers=0;
    for (int i=0;i<residual.size();++i) {
      transformer.apply(setA[i],est);
      const float d = static_cast<float>(dist(est,setB[i]));
      residual.at(i)=d;
      // is an inlier?
      if (d<maxError) {
        inliers.at(numInliers)=i;
        numInliers++;
        average+=d;
      }
    }
    average/=residual.size();
  }
  
  // On copy apply for type std::vector<P>!
  template<class E>
  template<class P>
  bool ransacEstimation<E>::apply(const std::vector<P>& setA, 
                                  const std::vector<P>& setB,
                                        typename E::parameters& result) const {
    ivector inliers;
    return apply(setA,setB,inliers,result);
  }

  // On copy apply for type std::vector<P>!
  template<class E>
  template<class P>
  bool ransacEstimation<E>::apply(const std::vector<P>& setA, 
                                  const std::vector<P>& setB,
                                        ivector& inliers,
                                        typename E::parameters& result) const {
    if (setA.size() != setB.size()) {
      setStatusString("Sets of points have different sizes.");
      return false;
    }

    if (static_cast<int>(setA.size()) < numPointsPerTrial_) {
      setStatusString("Not enough points for estimation. " \
                      "Check parameters::numberOfCorrespondences");
      return false;
    }

    const parameters& par = getParameters();
    //const int numberOfInliers = iround(setA.size()*(1.0-par.contamination));

    ivector idx(setA.size());
    for (int i=0;i<idx.size();++i) {
      idx.at(i)=i;
    }

    uniformDiscreteDistribution::parameters rndPar(par.rndParameters);
    rndPar.min = 0;
    rndPar.max = idx.lastIdx();

    uniformDiscreteDistribution rnd(rndPar);
    E estimator(par.initialEstimationParameters);
    ivector wnd;
    fvector residuals;
    float average;
    ivector winnerInliers;
    int numInliers;
    int winningInliers(0);
    float winningAverage(0.0f);
    int numIterations = par.numberOfIterations;

    for (int trial=0;trial<numIterations;++trial) {
      getNRandom(rnd,numPointsPerTrial_,idx);
      wnd.useExternData(numPointsPerTrial_,idx.data());
      if (estimator.estimateLLS(wnd,setA,setB)) {
        computeResidual(estimator,setA,setB,sqrt(par.maxError),
                        residuals,average,inliers,numInliers);
        if ( (winningInliers < numInliers) ||
             ((winningInliers == numInliers) && (winningAverage>average)) ) {

          winningInliers = numInliers;
          winningAverage = average;
          inliers.swap(winnerInliers);

          if (par.adaptiveContamination) {
            const float inlierProb=static_cast<float>(numInliers)/setA.size();
            // never increase the number of iterations, only decrease them
            const float tmp = pow(inlierProb,numPointsPerTrial_);
            int suggest;
            if ( abs(tmp) < std::numeric_limits<float>::epsilon() ) {
              suggest=std::numeric_limits<int>::max();
            } else {
              suggest = iround( logConfidence_ / log10(1.0f-tmp) );
            }
            numIterations = min(numIterations,suggest);
          }
        }
      }
    }

    // reestimate the transform just with the inliers
    if (winningInliers > 0) {
      wnd.useExternData(winningInliers,winnerInliers.data());
      if (estimator.estimateLLS(wnd,setA,setB)) {
        result.copy(estimator.getParameters());
        inliers.copy(wnd);
        return true;
      }
    } else {
      setStatusString("No inliers found!  Fix your parameters");
    }

    return false;
  }

}
