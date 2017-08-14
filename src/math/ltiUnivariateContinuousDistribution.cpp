/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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
 * \file   ltiUnivariateContinuousDistribution.cpp
 *         Contains the class univariateContinuousDistribution,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiUnivariateContinuousDistribution.cpp,v 1.1 2007-09-26 21:21:52 alvarado Exp $
 */

#include "ltiUnivariateContinuousDistribution.h"
#include "ltiFactory.h"

namespace lti {

  // WARNING: this has to be in exactly one line: do not break it, or
  //          the scripts for automatic reference detection won't work.
  _LTI_REGISTER_IN_FACTORY(univariateContinuousDistribution,univariateContinuousDistribution)

  // --------------------------------------------------
  // univariateContinuousDistribution
  // --------------------------------------------------

  // default constructor
  univariateContinuousDistribution::
  univariateContinuousDistribution(const bool initParams)
    : randomDistribution() {

    if (initParams) {
      // create an instance of the parameters with the default values
      parameters defaultParameters;
      // set the default parameters
      setParameters(defaultParameters);
    }
  }

  // default constructor
  univariateContinuousDistribution::
  univariateContinuousDistribution(const parameters& par)
    : randomDistribution() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  univariateContinuousDistribution::
  univariateContinuousDistribution(const univariateContinuousDistribution& o)
    : randomDistribution() {
    copy(o);
  }

  // destructor
  univariateContinuousDistribution::~univariateContinuousDistribution() {
  }

  // copy member
  univariateContinuousDistribution& univariateContinuousDistribution::
  copy(const univariateContinuousDistribution& other) {
    randomDistribution::copy(other);

    return (*this);
  }

  // alias for copy member
  univariateContinuousDistribution& univariateContinuousDistribution::
  operator=(const univariateContinuousDistribution& other) {
    return (copy(other));
  }

  // class name
  const std::string& univariateContinuousDistribution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  univariateContinuousDistribution* univariateContinuousDistribution::
  clone() const {
    return new univariateContinuousDistribution(*this);
  }

  // create a new instance
  univariateContinuousDistribution* univariateContinuousDistribution::
  newInstance() const {
    return new univariateContinuousDistribution();
  }

  // return parameters
  bool univariateContinuousDistribution::updateParameters() {
    if (randomDistribution::updateParameters()) {
      dnorm_ = 1.0/(static_cast<double>(max_)+1.0);
      fnorm_ =static_cast<float>(dnorm_);
      return true;
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type float!
  bool univariateContinuousDistribution::apply(float& rnd)  {
    rnd = generator_->draw() * fnorm_;
    return true;
  }

  // On place apply for type double!
  bool univariateContinuousDistribution::apply(double& rnd)  {
    rnd = generator_->draw() * dnorm_;
    return true;
  }

  // On place apply for type float!
  float univariateContinuousDistribution::fdraw()  {
    return generator_->draw() * fnorm_;
  }

  // On place apply for type double!
  double univariateContinuousDistribution::draw()  {
    return generator_->draw() * dnorm_;
  }

  // On place apply for type float!
  float univariateContinuousDistribution::frand()  {
    return generator_->draw() * fnorm_;
  }

  // On place apply for type double!
  double univariateContinuousDistribution::rand()  {
    return generator_->draw() * dnorm_;
  }




}

