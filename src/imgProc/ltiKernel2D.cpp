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
 * \file   ltiKernel2D.cpp
 *         explicit instantiations of kernel2D<ubyte>, kernel2D<int>,
 *         kernel2D<double>.
 * \author Pablo Alvarado
 * \date   28.10.00
 *
 * revisions ..: $Id: ltiKernel2D.cpp,v 1.4 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiComplex.h"
#include "ltiKernel2D.h"
#include "ltiKernel2D_template.h"
#include "ltiFactory.h"

namespace lti {

  _LTI_REGISTER_TEMPLATE_IN_FACTORY(ubyte,container,kernel2D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(int,container,kernel2D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(float,container,kernel2D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(double,container,kernel2D)
  // _LTI_REGISTER_TEMPLATE_IN_FACTORY(fcomplex,container,kernel1D)
  
  template class kernel2D<ubyte>;
  template class kernel2D<int>;
  template class kernel2D<float>;
  template class kernel2D<double>;
  //template class kernel2D<fcomplex>;
}
