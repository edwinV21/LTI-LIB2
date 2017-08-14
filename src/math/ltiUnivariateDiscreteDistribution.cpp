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
 * \file   ltiUnivariateDiscreteDistribution.cpp
 *         Contains the class lti::univariateDiscreteDistribution, 
 *         parent of all univariate discrete random distributions.
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiUnivariateDiscreteDistribution.cpp,v 1.1 2007-09-26 21:21:52 alvarado Exp $
 */

#include "ltiUnivariateDiscreteDistribution.h"

namespace lti {

  // --------------------------------------------------
  // univariateDiscreteDistribution
  // --------------------------------------------------

  // default constructor
  univariateDiscreteDistribution::univariateDiscreteDistribution()
    : randomDistribution() {
  }

  // copy constructor
  univariateDiscreteDistribution::
  univariateDiscreteDistribution(const univariateDiscreteDistribution& other)
    : randomDistribution() {
    copy(other);
  }

  // destructor
  univariateDiscreteDistribution::~univariateDiscreteDistribution() {
  }

  // Default implementation for rand()
  int univariateDiscreteDistribution::rand() {
    return draw();
  }

}

