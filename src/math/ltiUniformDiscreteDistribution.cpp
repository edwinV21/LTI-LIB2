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
 * \file   ltiUniformDiscreteDistribution.cpp
 *         Generate uniformly distributed random integers in a given interval.
 * \author Pablo Alvarado
 * \date   25.09.2007
 *
 * revisions ..: $Id: ltiUniformDiscreteDistribution.cpp,v 1.3 2009-04-19 04:06:10 alvarado Exp $
 */

#include "ltiUniformDiscreteDistribution.h"
#include "ltiFactory.h"

namespace lti {

  // WARNING: this has to be in exactly one line: do not break it, or
  //          the scripts for automatic reference detection won't work.
  _LTI_REGISTER_IN_FACTORY(univariateDiscreteDistribution,uniformDiscreteDistribution)


  // --------------------------------------------------
  // uniformDiscreteDistribution::parameters
  // --------------------------------------------------

  // default constructor
  uniformDiscreteDistribution::parameters::parameters()
    : univariateDiscreteDistribution::parameters() {
    
    min = 0;
    max = 100;
  }

  // copy constructor
  uniformDiscreteDistribution::parameters::parameters(const parameters& other)
    : univariateDiscreteDistribution::parameters() {
    copy(other);
  }

  // destructor
  uniformDiscreteDistribution::parameters::~parameters() {
  }

  // copy member

  uniformDiscreteDistribution::parameters&
  uniformDiscreteDistribution::parameters::copy(const parameters& other) {
    univariateDiscreteDistribution::parameters::copy(other);

    min = other.min;
    max = other.max;

    return *this;
  }

  // alias for copy method
  uniformDiscreteDistribution::parameters&
  uniformDiscreteDistribution::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& uniformDiscreteDistribution::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  uniformDiscreteDistribution::parameters*
  uniformDiscreteDistribution::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  uniformDiscreteDistribution::parameters*
  uniformDiscreteDistribution::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool uniformDiscreteDistribution::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"min",min);
      lti::write(handler,"max",max);
    }

    b = b && univariateDiscreteDistribution::parameters::write(handler,false);

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
  bool uniformDiscreteDistribution::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"min",min);
      lti::read(handler,"max",max);
    }

    b = b && univariateDiscreteDistribution::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // uniformDiscreteDistribution
  // --------------------------------------------------

  // default constructor
  uniformDiscreteDistribution::uniformDiscreteDistribution()
    : univariateDiscreteDistribution() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  uniformDiscreteDistribution::uniformDiscreteDistribution(const int tmin,
                                                           const int tmax)
    : univariateDiscreteDistribution() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;

    defaultParameters.min=tmin;
    defaultParameters.max=tmax;

    // set the default parameters
    setParameters(defaultParameters);
    
  }

  // default constructor
  uniformDiscreteDistribution::
  uniformDiscreteDistribution(const parameters& par)
    : univariateDiscreteDistribution() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  uniformDiscreteDistribution::
  uniformDiscreteDistribution(const uniformDiscreteDistribution& other)
    : univariateDiscreteDistribution() {
    copy(other);
  }

  // destructor
  uniformDiscreteDistribution::~uniformDiscreteDistribution() {
  }

  // copy member
  uniformDiscreteDistribution& 
  uniformDiscreteDistribution::copy(const uniformDiscreteDistribution& other) {
    univariateDiscreteDistribution::copy(other);

    return (*this);
  }

  // alias for copy member
  uniformDiscreteDistribution& uniformDiscreteDistribution::
  operator=(const uniformDiscreteDistribution& other) {
    return (copy(other));
  }

  // class name
  const std::string& uniformDiscreteDistribution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  uniformDiscreteDistribution* uniformDiscreteDistribution::clone() const {
    return new uniformDiscreteDistribution(*this);
  }

  // create a new instance
  uniformDiscreteDistribution* 
  uniformDiscreteDistribution::newInstance() const {
    return new uniformDiscreteDistribution();
  }

  // return parameters
  const uniformDiscreteDistribution::parameters&
  uniformDiscreteDistribution::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  uniformDiscreteDistribution::parameters&
  uniformDiscreteDistribution::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool uniformDiscreteDistribution::updateParameters() {
    if (univariateDiscreteDistribution::updateParameters()) {
      const parameters& par = getParameters();

      if (par.min > par.max) {
        setStatusString("Parameter min has to be lower than max.");
        return false;
      }

      minimum_ = par.min;
      maximum_ = par.max;
      delta_   = maximum_ - minimum_ + 1;

      return true;
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // On place apply for type int!
  int uniformDiscreteDistribution::rand() {
    // this is maybe the fastest way, although maybe not so correct as
    // we are giving a small higher probability to the lowest
    // generator_->max() % delta_ values of the distribution.  We are
    // going to assume that the intervall is relatively small.
    return static_cast<int>(generator_->draw() % delta_) + minimum_;
  }
  
  // On place apply for type int!
  bool uniformDiscreteDistribution::apply(int& rnd) {
    rnd = rand();
    return true;
  }

  // On place apply for type int!
  int uniformDiscreteDistribution::draw() {
    return rand();
  }
  
  /*
   * Virtual method to obtain the maximum possible number
   * (inclusive) to be returned by this distribution
   */
  int uniformDiscreteDistribution::max() const {
    return maximum_;
  }

  /*
   * Virtual method to obtain the minimum possible number
   * (inclusive) to be returned by this distribution.
   */
  int uniformDiscreteDistribution::min() const {
    return minimum_;
  }

  /*
   * Change the interval, modifying the parameters as well
   */
  bool uniformDiscreteDistribution::
  setInterval(const int lower,const int upper) {
    parameters& par = getRWParameters();
    if ( (par.min != lower) || (par.max != upper) ) {
      par.min = lower;
      par.max = upper;
      return updateParameters();
    }

    return true;
  }

}

