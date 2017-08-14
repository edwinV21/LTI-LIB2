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
 * revisions ..: $Id: ltiNormalDistribution.cpp,v 1.1 2007-09-26 21:21:52 alvarado Exp $
 */

#include "ltiNormalDistribution.h"
#include "ltiMath.h"
#include "ltiFactory.h"

namespace lti {
  // WARNING: this has to be in exactly one line: do not break it, or
  //          the scripts for automatic reference detection won't work.
  _LTI_REGISTER_IN_FACTORY(univariateContinuousDistribution,normalDistribution)

  template<typename T>
  normalDistribution::shadows<T>::shadows() 
    : sigma(T(1)), mu(T()), precomputed(T()), precomputationAvailable(false) {
  }


  // --------------------------------------------------
  // normalDistribution::parameters
  // --------------------------------------------------

  // default constructor
  normalDistribution::parameters::parameters()
    : univariateContinuousDistribution::parameters() {
    
    mean = 0.0;
    sigma = 1.0;
  }

  // copy constructor
  normalDistribution::
  parameters::parameters(const parameters& other)
    : univariateContinuousDistribution::parameters() {
    copy(other);
  }

  // destructor
  normalDistribution::parameters::~parameters() {
  }

  // copy member

  normalDistribution::parameters&
  normalDistribution::parameters::copy(const parameters& other) {
    univariateContinuousDistribution::parameters::copy(other);

    mean  = other.mean;
    sigma = other.sigma;

    return *this;
  }

  // alias for copy method
  normalDistribution::parameters&
  normalDistribution::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& normalDistribution::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  normalDistribution::parameters*
  normalDistribution::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  normalDistribution::parameters*
  normalDistribution::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool normalDistribution::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"mean",mean);
      lti::write(handler,"sigma",sigma);
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
  bool normalDistribution::parameters::read(ioHandler& handler,
                                            const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"mean",mean);
      lti::read(handler,"sigma",sigma);
    }

    b = b && univariateContinuousDistribution::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // normalDistribution
  // --------------------------------------------------

  // default constructor
  normalDistribution::normalDistribution()
    : univariateContinuousDistribution() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  normalDistribution::
  normalDistribution(const double mean,const double sigma)
    : univariateContinuousDistribution() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    defaultParameters.mean=mean;
    defaultParameters.sigma=sigma;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  normalDistribution::
  normalDistribution(const parameters& par)
    : univariateContinuousDistribution() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  normalDistribution::
  normalDistribution(const normalDistribution& other)
    : univariateContinuousDistribution() {
    copy(other);
  }

  // destructor
  normalDistribution::~normalDistribution() {
  }

  // copy member
  normalDistribution& 
  normalDistribution::
  copy(const normalDistribution& other) {
    univariateContinuousDistribution::copy(other);

    return (*this);
  }

  // alias for copy member
  normalDistribution& normalDistribution::
  operator=(const normalDistribution& other) {
    return (copy(other));
  }

  // class name
  const std::string& normalDistribution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  normalDistribution* normalDistribution::clone() const {
    return new normalDistribution(*this);
  }

  // create a new instance
  normalDistribution* 
  normalDistribution::newInstance() const {
    return new normalDistribution();
  }

  // return parameters
  const normalDistribution::parameters&
  normalDistribution::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool normalDistribution::updateParameters() {
    if (univariateContinuousDistribution::updateParameters()) {
      const parameters& par = getParameters();

      fshadow_.mu    = static_cast<float>(par.mean);
      fshadow_.sigma = static_cast<float>(par.sigma);
      fshadow_.precomputationAvailable = false;
      fshadow_.norm  = 2.0f*fnorm_;
      
      dshadow_.mu    = static_cast<double>(par.mean);
      dshadow_.sigma = static_cast<double>(par.sigma);
      dshadow_.precomputationAvailable = false;
      dshadow_.norm  = 2.0*dnorm_;

      return true;
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type int!
  inline double normalDistribution::rand() {
    double ret;
    
    if (dshadow_.precomputationAvailable) {
      dshadow_.precomputationAvailable = false;
      ret = dshadow_.precomputed;
    } else {
      double x, y, r2;
      do {
        x  = dshadow_.norm * static_cast<double>(generator_->draw()) - 1.0;
        y  = dshadow_.norm * static_cast<double>(generator_->draw()) - 1.0;
        r2 = x * x + y * y;
      }
      while (r2 > 1.0 || r2 == 0.0);
      
      const double mult = sqrt(-2.0 * log(r2) / r2);
      dshadow_.precomputed = x * mult;
      dshadow_.precomputationAvailable = true;
      ret = y * mult;
    }
    
    return (ret * dshadow_.sigma + dshadow_.mu);
  }

  // On place apply for type int!
  bool normalDistribution::apply(double& rnd) {
    rnd = rand();
    return true;
  }
  

  // On place apply for type int!
  double normalDistribution::draw() {
    return rand();
  }

  // On place apply for type int!
  float normalDistribution::frand() {
    float ret;
    
    if (fshadow_.precomputationAvailable) {
      fshadow_.precomputationAvailable = false;
      ret = fshadow_.precomputed;
    } else {
      float x, y, r2;
      do {
        x = fshadow_.norm * static_cast<float>(generator_->draw()) - 1.0f;
        y = fshadow_.norm * static_cast<float>(generator_->draw()) - 1.0f;
        r2 = x * x + y * y;
      }
      while (r2 > 1.0f || r2 == 0.0f);
      
      const float mult = sqrt(-2.0f * log(r2) / r2);
      fshadow_.precomputed = x * mult;
      fshadow_.precomputationAvailable = true;
      ret = y * mult;
    }
    
    return (ret * fshadow_.sigma + fshadow_.mu);
  }

  // On place apply for type int!
  bool normalDistribution::apply(float& rnd) {
    rnd = frand();
    return true;
  }

  // On place apply for type int!
  float normalDistribution::fdraw() {
    return frand();
  }
    

}

