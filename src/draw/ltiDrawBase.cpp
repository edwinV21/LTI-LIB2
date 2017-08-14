/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiDrawBase.cpp
 * \author Pablo Alvarado
 * \date   16.01.2003
 * 
 * $Id: ltiDrawBase.cpp,v 1.2 2006-08-04 08:50:43 doerfler Exp $
 */

#include "ltiMacroSymbols.h"

#include "ltiDrawBase.h"
#include "ltiDrawBase_template.h"

#include "ltiRGBAPixel.h"

namespace lti {

  // explicit instantiations

  template class drawBase<float>;     // channels
  template class drawBase<ubyte>;     // channel8
  template class drawBase<rgbaPixel>;  // image 
  template class drawBase<double>;    // matrices of doubles
  template class drawBase<int>;       // matrices of integers
  // template class drawBase<cmykColor>; // for EPS drawing in CMYK
} // namespace lti

