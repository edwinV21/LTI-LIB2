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
 * \file    ltiDraw.cpp
 * \author Pablo Alvarado
 * \date    25.5.1998
 * 
 * $Id: ltiDraw.cpp,v 1.2 2012-09-16 05:05:59 alvarado Exp $
 */

#include "ltiMacroSymbols.h"

#ifdef _LTI_MSC_6
// a weird warning from MSVC++ is a false alarm!
#pragma warning(disable:4660)
#endif

#include "ltiDraw.h"
#include "ltiDraw_template.h"

namespace lti {

  // explicit instantiations

  template class draw<float>;     // channels
  template class draw<ubyte>;     // channel8
  template class draw<rgbaPixel>;  // image
  template class draw<double>;    // matrices of doubles
  template class draw<int>;       // matrices of integers
} // namespace lti

