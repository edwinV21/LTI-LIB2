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
 * \file   ltiBiquadraticInterpolation.cpp
 *         The file contains explicit instantiations for
 *         biquadraticInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiBiquadraticInterpolation.cpp,v 1.1 2005-01-11 15:18:22 doerfler Exp $
 */

#include "ltiBiquadraticInterpolation.h"
#include "ltiBiquadraticInterpolation_template.h"

namespace lti {
  template class biquadraticInterpolation<ubyte>;
  template class biquadraticInterpolation<int>;
  template class biquadraticInterpolation<float>;
  template class biquadraticInterpolation<double>;
  template class biquadraticInterpolation<rgbaPixel>;
}

