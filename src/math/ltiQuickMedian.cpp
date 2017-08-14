/*
 * Copyright (C) 1998 - 2005
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiQuickMedian_template.h
 *         This file contains the functor quickMedian, which
 *         calculates the median quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickMedian.cpp,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiQuickMedian.h"

namespace lti {

  // --------------------------------------------------
  // quickMedian::parameters
  // --------------------------------------------------

  // default constructor
  quickMedian::parameters::parameters()
    : functor::parameters() {
    medianEvenCase = TakeLower;
  }

  // copy constructor
  quickMedian::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }

  // destructor
  quickMedian::parameters::~parameters() {
  }

  // name
  const std::string& quickMedian::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  //copy
  quickMedian::parameters&
  quickMedian::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    medianEvenCase = other.medianEvenCase;
    
    return *this;
  }

  // alias for copy
  quickMedian::parameters& 
  quickMedian::parameters::operator=(const parameters& other) {
    return copy(other);
  }
  
  //clone
  quickMedian::parameters* 
  quickMedian::parameters::clone() const {
    return new parameters(*this);
  }

  //newInstance
 quickMedian::parameters* 
  quickMedian::parameters::newInstance() const {
    return new parameters();
  }

  // write
  bool quickMedian::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b(true);
    
    if (complete) {
      b = b && handler.writeBegin();
    }
    
    b = b && lti::write(handler, "medianEvenCase", medianEvenCase);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  // read
  bool quickMedian::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b(true);
    
    if (complete) {
      b = b && handler.readBegin();
    }
    
    b = b && lti::read(handler, "medianEvenCase", medianEvenCase);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  // --------------------------------------------------
  // quickMedian
  // --------------------------------------------------

  // default constructor
  quickMedian::quickMedian()
    : functor(){
    parameters param;
    setParameters(param);
  }

  quickMedian::quickMedian(const parameters& par)
    : functor(){
    setParameters(par);
  }

  // default constructor
  quickMedian::quickMedian(const eMedianEvenCase medianEvenCase)
    : functor(){
    parameters param;
    param.medianEvenCase = medianEvenCase;
    setParameters(param);
  }

  // copy constructor
  quickMedian::quickMedian(const quickMedian& other)
    : functor() {
    copy(other);
  }

  // destructor
  quickMedian::~quickMedian() {
  }

  // name
  const std::string& quickMedian::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy
  quickMedian& quickMedian::copy(const quickMedian& other) {
      functor::copy(other);
    return (*this);
  }

  // clone
  quickMedian* quickMedian::clone() const {
    return new quickMedian(*this);
  }

  // newInstance
  quickMedian* quickMedian::newInstance() const {
    return new quickMedian();
  }

  // return parameters
  const quickMedian::parameters& quickMedian::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}
