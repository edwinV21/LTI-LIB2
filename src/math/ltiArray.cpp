/*
 * Copyright (C) 1998-2004
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
 * \file   ltiArray.cpp
 *         Contains implicit instantiations of array.
 * \author Pablo Alvarado
 * \date   09.04.99
 *
 * revisions ..: $Id: ltiArray.cpp,v 1.4 2012-09-16 05:22:59 alvarado Exp $
 */
#include "ltiArray.h"
#include "ltiArray_template.h"
#include "ltiPoint.h"
#include "ltiRGBPixel.h"
#include "ltiTypes.h"
#include "ltiComplex.h"

namespace lti {
  // explicit instantiations

  template class array< ubyte >;
  template class array< byte >;
  template class array< int32 >;
  template class array< uint32 >;
  template class array< float >;
  template class array< double >;
  template class array< rgbaPixel >;
  template class array< ipoint >;
  template class array< fcomplex >;
  template class array< dcomplex >;

}
