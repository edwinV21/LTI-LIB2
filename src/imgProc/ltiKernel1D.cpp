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
 * \file   ltiKernel1D.cpp
 *         explicit instantiations of kernel1D<ubyte>, kernel1D<int>,
 *         kernel1D<double>.
 * \author Pablo Alvarado
 * \date   28.10.00
 *
 * revisions ..: $Id: ltiKernel1D.cpp,v 1.4 2012-09-16 05:22:59 alvarado Exp $
 */


#include "ltiKernel1D.h"
#include "ltiKernel1D_template.h"
#include "ltiComplex.h"
#include "ltiFactory.h"

namespace lti {

  _LTI_REGISTER_TEMPLATE_IN_FACTORY(ubyte,container,kernel1D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(int,container,kernel1D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(float,container,kernel1D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(double,container,kernel1D)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(fcomplex,container,kernel1D)
 
  template class kernel1D<ubyte>;
  template class kernel1D<int>;
  template class kernel1D<float>;
  template class kernel1D<double>;
  template class kernel1D<fcomplex>;
}
