/*
 * Copyright (C) 1998 - 2004
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


/*
 * \file   ltiCholeskyDecomposition.cpp
 *         Explicit instantiations of lti::choleskyDecomposition for
 *         float and double
 * \author Peter Doerfler
 * \date   07.07.2003
 * 
 * $Id: ltiCholeskyDecomposition.cpp,v 1.1 2004-08-04 15:12:07 doerfler Exp $
 */

#include "ltiCholeskyDecomposition.h"
#include "ltiCholeskyDecomposition_template.h"

namespace lti {
  // explicit instantiations
  template class choleskyDecomposition<float>;
  template class choleskyDecomposition<double>;
} // namespace lti

