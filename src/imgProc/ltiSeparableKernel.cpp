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
 * \file   ltiSeperableKernel.cpp
 *         Contains explicit instantiations of separableKernel.
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * revisions ..: $Id: ltiSeparableKernel.cpp,v 1.3 2009-10-12 02:45:00 alvarado Exp $
 */
#include "ltiSeparableKernel.h"
#include "ltiSeparableKernel_template.h"
#include "ltiFactory.h"

namespace lti {

  _LTI_REGISTER_TEMPLATE_IN_FACTORY(ubyte,container,separableKernel)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(int,container,separableKernel)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(float,container,separableKernel)
  _LTI_REGISTER_TEMPLATE_IN_FACTORY(double,container,separableKernel)
  //_LTI_REGISTER_TEMPLATE_IN_FACTORY(fcomplex,container,separableKernel)


  template class separableKernel<ubyte>;
  template class separableKernel<int>;
  template class separableKernel<float>;
  template class separableKernel<double>;

  template std::ostream& 
  operator<<<ubyte>(std::ostream& s, const separableKernel<ubyte>& kern);
  template std::ostream& 
  operator<<<int>(std::ostream& s, const separableKernel<int>& kern);
  template std::ostream& 
  operator<<<float>(std::ostream& s, const separableKernel<float>& kern);
  template std::ostream& 
  operator<<<double>(std::ostream& s, const separableKernel<double>& kern);

}
