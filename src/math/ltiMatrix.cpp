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
 * \file   ltiMatrix.cpp
 * \author Pablo Alvarado
 * \date   18.05.2001
 *
 * $Id: ltiMatrix.cpp,v 1.2 2005-03-29 13:18:08 doerfler Exp $
 */

#include "ltiRGBPixel.h"
#include "ltiRGBAPixel.h"
#include "ltiPoint.h"
#include "ltiPoint3D.h"
#include "ltiComplex.h"
#include "ltiGenericMatrix.h" // provides implementation of generic matrix
#include "ltiMatrix.h"
#include "ltiMatrix_template.h"


namespace lti {
  // explicit instantiations for the parent class
  template class genericMatrix<ubyte>;
  template class genericMatrix<byte>;
  template class genericMatrix<char>;
  template class genericMatrix<long>;
  template class genericMatrix<int32>;
  template class genericMatrix<uint32>;
  template class genericMatrix<float>;
  template class genericMatrix<double>;
  template class genericMatrix<rgbaPixel>;
  template class genericMatrix<frgbPixel>;
  template class genericMatrix<ipoint>;
  template class genericMatrix<fpoint>;
  template class genericMatrix<dpoint>;
  template class genericMatrix<fcomplex>;
  template class genericMatrix<dcomplex>;
  template class genericMatrix<fpoint3D>;

  // explicit instantiation of the class
  template class matrix<ubyte>;
  template class matrix<byte>;
  template class matrix<char>;
  template class matrix<long>;
  template class matrix<int32>;
  template class matrix<uint32>;
  template class matrix<float>;
  template class matrix<double>;
  template class matrix<rgbaPixel>;
  template class matrix<frgbPixel>;
  template class matrix<ipoint>;
  template class matrix<fpoint>;
  template class matrix<dpoint>;
  template class matrix<fpoint3D>;
  template class matrix<fcomplex>;
  template class matrix<dcomplex>;
}

namespace std {
  // explicit instantiations for operator<<
  // for ubyte and byte there was already an specialization
  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::ubyte>&);
  
  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::byte>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<char>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<long>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::int32>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::uint32>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<float>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<double>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::rgbaPixel>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::frgbPixel>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::ipoint>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericMatrix<lti::fpoint>&);

  template ostream&
  operator<<(ostream& s,const lti::genericMatrix<lti::dpoint>&);

  template ostream&
  operator<<(ostream& s,const lti::genericMatrix<lti::fpoint3D>&);

  template ostream&
  operator<<(ostream& s,const lti::genericMatrix<lti::fcomplex>&);

  template ostream&
  operator<<(ostream& s,const lti::genericMatrix<lti::dcomplex>&);

}
