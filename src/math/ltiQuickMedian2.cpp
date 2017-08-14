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
 * $Id: ltiQuickMedian2.cpp,v 1.2 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiQuickMedian2.h"

namespace lti {

  // --------------------------------------------------
  // quickMedian2
  // --------------------------------------------------

  // default constructor
  quickMedian2::quickMedian2()
    : functor() {
    parameters par;
    setParameters(par);
  }

  quickMedian2::quickMedian2(const parameters& par)
    : functor(){
    setParameters(par);
  }

  // default constructor
  quickMedian2::quickMedian2(const eMedianEvenCase medianEvenCase)
    : functor(){
    parameters param;
    param.medianEvenCase = medianEvenCase;
    setParameters(param);
  }

  // copy constructor
  quickMedian2::quickMedian2(const quickMedian2& other)
    : functor() {
    copy(other);
  }

  // destructor
  quickMedian2::~quickMedian2() {
  }

  // name
  const std::string& quickMedian2::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  quickMedian2* quickMedian2::clone() const {
    return new quickMedian2(*this);
  }

  // newInstance member
  quickMedian2* quickMedian2::newInstance() const {
    return new quickMedian2();
  }

  // return parameters
  const quickMedian2::parameters&
  quickMedian2::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}
