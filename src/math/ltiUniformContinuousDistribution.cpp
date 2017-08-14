/*
 * Copyright (C) 2007
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * \file   ltiUniformContinuousDistribution.cpp
 *         Generate uniformly distributed random integers in a given interval.
 * \author Pablo Alvarado
 * \date   25.09.2007
 *
 * revisions ..: $Id: ltiUniformContinuousDistribution.cpp,v 1.1 2007-09-26 21:21:52 alvarado Exp $
 */

#include "ltiUniformContinuousDistribution.h"
#include "ltiFactory.h"

namespace lti {
  // WARNING: this has to be in exactly one line: do not break it, or
  //          the scripts for automatic reference detection won't work.
  _LTI_REGISTER_IN_FACTORY(univariateContinuousDistribution,uniformContinuousDistribution)


  // --------------------------------------------------
  // uniformContinuousDistribution::parameters
  // --------------------------------------------------

  // default constructor
  uniformContinuousDistribution::parameters::parameters()
    : univariateContinuousDistribution::parameters() {
    
    min = 0.0;
    max = 1.0;
  }

  // copy constructor
  uniformContinuousDistribution::
  parameters::parameters(const parameters& other)
    : univariateContinuousDistribution::parameters() {
    copy(other);
  }

  // destructor
  uniformContinuousDistribution::parameters::~parameters() {
  }

  // copy member

  uniformContinuousDistribution::parameters&
  uniformContinuousDistribution::parameters::copy(const parameters& other) {
    univariateContinuousDistribution::parameters::copy(other);

    min = other.min;
    max = other.max;

    return *this;
  }

  // alias for copy method
  uniformContinuousDistribution::parameters&
  uniformContinuousDistribution::parameters::
  operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& uniformContinuousDistribution::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  uniformContinuousDistribution::parameters*
  uniformContinuousDistribution::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  uniformContinuousDistribution::parameters*
  uniformContinuousDistribution::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool uniformContinuousDistribution::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"min",min);
      lti::write(handler,"max",max);
    }

    b = b && 
      univariateContinuousDistribution::parameters::write(handler,false);

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
  bool uniformContinuousDistribution::parameters::read(ioHandler& handler,
                                                       const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"min",min);
      lti::read(handler,"max",max);
    }

    b = b && univariateContinuousDistribution::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // uniformContinuousDistribution
  // --------------------------------------------------

  // default constructor
  uniformContinuousDistribution::uniformContinuousDistribution()
    : univariateContinuousDistribution(),
      minimum_(0.0),maximum_(0.0),
      fminimum_(0.0),fmaximum_(0.0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  uniformContinuousDistribution::
  uniformContinuousDistribution(const double tmin,const double tmax)
    : univariateContinuousDistribution(),
      minimum_(0.0),maximum_(0.0),
      fminimum_(0.0),fmaximum_(0.0)  {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    defaultParameters.min=tmin;
    defaultParameters.max=tmax;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  uniformContinuousDistribution::
  uniformContinuousDistribution(const parameters& par)
    : univariateContinuousDistribution(),
      minimum_(0.0),maximum_(0.0),
      fminimum_(0.0),fmaximum_(0.0)  {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  uniformContinuousDistribution::
  uniformContinuousDistribution(const uniformContinuousDistribution& other)
    : univariateContinuousDistribution(),
      minimum_(0.0),maximum_(0.0),
      fminimum_(0.0),fmaximum_(0.0) {
    copy(other);
  }

  // destructor
  uniformContinuousDistribution::~uniformContinuousDistribution() {
  }

  // copy member
  uniformContinuousDistribution& 
  uniformContinuousDistribution::
  copy(const uniformContinuousDistribution& other) {
    univariateContinuousDistribution::copy(other);

    return (*this);
  }

  // alias for copy member
  uniformContinuousDistribution& uniformContinuousDistribution::
  operator=(const uniformContinuousDistribution& other) {
    return (copy(other));
  }

  // class name
  const std::string& uniformContinuousDistribution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  uniformContinuousDistribution* uniformContinuousDistribution::clone() const {
    return new uniformContinuousDistribution(*this);
  }

  // create a new instance
  uniformContinuousDistribution* 
  uniformContinuousDistribution::newInstance() const {
    return new uniformContinuousDistribution();
  }

  // return parameters
  const uniformContinuousDistribution::parameters&
  uniformContinuousDistribution::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool uniformContinuousDistribution::updateParameters() {
    if (univariateContinuousDistribution::updateParameters()) {
      const parameters& par = getParameters();

      if (par.min > par.max) {
        setStatusString("Parameter min has to be lower than max.");
        return false;
      }

      minimum_  = par.min;
      maximum_  = par.max;

      fminimum_ = static_cast<float>(par.min);
      fmaximum_ = static_cast<float>(par.max);

      delta_    = (maximum_ - minimum_)*dnorm_;
      fdelta_   = static_cast<float>(delta_);

      return true;
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type int!
  bool uniformContinuousDistribution::apply(double& rnd) {
    rnd = static_cast<double>(generator_->draw() * delta_) + minimum_;
    return true;
  }
  

  // On place apply for type int!
  double uniformContinuousDistribution::draw() {
    return static_cast<double>(generator_->draw() * delta_) + minimum_;
  }

  // On place apply for type int!
  double uniformContinuousDistribution::rand() {
    // this is maybe the fastest way, although maybe not so correct as
    // we are giving a small higher probability to the lowest
    // generator_->max() % delta_ values of the distribution.  We are
    // going to assume that the intervall is relatively small.
    return static_cast<double>(generator_->draw() * delta_) + minimum_;
  }

  // On place apply for type int!
  bool uniformContinuousDistribution::apply(float& rnd) {
    rnd = static_cast<float>(generator_->draw() * fdelta_) + fminimum_;
    return true;
  }

  // On place apply for type int!
  float uniformContinuousDistribution::fdraw() {
    return static_cast<float>(generator_->draw() * fdelta_) + fminimum_;
  }

  // On place apply for type int!
  float uniformContinuousDistribution::frand() {
    // this is maybe the fastest way, although maybe not so correct as
    // we are giving a small higher probability to the lowest
    // generator_->max() % delta_ values of the distribution.  We are
    // going to assume that the intervall is relatively small.
    return static_cast<float>(generator_->draw() * fdelta_) + fminimum_;
  }

  
  /*
   * Virtual method to obtain the maximum possible number
   * (inclusive) to be returned by this distribution
   */
  double uniformContinuousDistribution::max() {
    return maximum_;
  }

  /*
   * Virtual method to obtain the minimum possible number
   * (inclusive) to be returned by this distribution.
   */
  double uniformContinuousDistribution::min() {
    return minimum_;
  }

}

