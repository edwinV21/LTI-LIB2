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
 * \file   ltiQuickPartialSort_template.h
 *         This file contains the functor quickPartialSort, which
 *         calculates the partialSort quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickPartialSort.cpp,v 1.2 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiQuickPartialSort.h"

namespace lti {

  // --------------------------------------------------
  // quickPartialSort
  // --------------------------------------------------

  // default constructor
  quickPartialSort::quickPartialSort()
    : functor(){
  }

  // copy constructor
  quickPartialSort::quickPartialSort(const quickPartialSort& other)
    : functor() {
    copy(other);
  }

  // destructor
  quickPartialSort::~quickPartialSort() {
  }

  // name
  const std::string& quickPartialSort::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy
  quickPartialSort& quickPartialSort::copy(const quickPartialSort& other) {
      functor::copy(other);
    return (*this);
  }

  // clone
  quickPartialSort* quickPartialSort::clone() const {
    return new quickPartialSort(*this);
  }

  // newInstance
  quickPartialSort* quickPartialSort::newInstance() const {
    return new quickPartialSort();
  }

}
