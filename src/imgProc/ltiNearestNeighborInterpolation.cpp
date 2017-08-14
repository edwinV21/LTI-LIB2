/*
 * Copyright (C) 1998-2005
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
 * \file   ltiNearestNeighborInterpolation.cpp
 *         Contains the intantiations of lti::nearestNeighborInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiNearestNeighborInterpolation.cpp,v 1.1 2005-04-08 14:34:45 arndh Exp $
 */

#include "ltiMacroSymbols.h"

#include "ltiNearestNeighborInterpolation.h"
#include "ltiNearestNeighborInterpolation_template.h"
#include "ltiRGBAPixel.h"

namespace lti {
  template class nearestNeighborInterpolation<ubyte>;
  template class nearestNeighborInterpolation<int>;
  template class nearestNeighborInterpolation<float>;
  template class nearestNeighborInterpolation<double>;
  template class nearestNeighborInterpolation<rgbaPixel>;
}

