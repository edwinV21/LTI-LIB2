/*
 * Copyright (C) 1998-2004
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2016
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
 * \file   ltiMath.cpp
 *         Definition of constants
 * \author Pablo Alvarado
 * \date   07.08.2016
 *
 * $Id: ltiMath.cpp,v 1.17 2012-11-17 14:49:05 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {

  /**
   * Constant Pi (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  const double Pi = 3.1415926535897932; //3.1415926535897932384626433832795;

  /**
   * Constant Not a Number (NaN) (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  const double NaN = std::numeric_limits<double>::quiet_NaN();

  /**
   * Constant Infinity (Inf) (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  const double Inf = std::numeric_limits<double>::infinity();

}
