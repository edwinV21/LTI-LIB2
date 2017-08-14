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
 * \file   ltiVector.cpp
 * \author Pablo Alvarado
 * \date   18.05.2001
 *
 * $Id: ltiVector.cpp,v 1.1 2004-07-28 14:18:49 alvarado Exp $
 */

#include "ltiTypes.h"
#include "ltiRGBPixel.h"
#include "ltiRGBAPixel.h"
#include "ltiPoint.h"
#include "ltiPoint3D.h"
#include "ltiComplex.h"
#include "ltiGenericVector.h" // provides implementation of generic vector
#include "ltiVector.h"
#include "ltiVector_template.h"

namespace lti {
  // explicit instantiations for the parent class, which were explicitely
  // deactivated until now
  template class genericVector<ubyte>;
  template class genericVector<byte>;
  template class genericVector<char>;
  template class genericVector<uint16>;
  template class genericVector<int16>;
  template class genericVector<int32>;
  template class genericVector<uint32>;
  template class genericVector<long>;
  template class genericVector<float>;
  template class genericVector<double>;
  template class genericVector<rgbaPixel>;
  template class genericVector<frgbPixel>;
  template class genericVector<ipoint>;
  template class genericVector<fpoint>;
  template class genericVector<dpoint>;
  template class genericVector<ipoint3D>;
  template class genericVector<fpoint3D>;
  template class genericVector<dpoint3D>;
  template class genericVector<fcomplex>;
  template class genericVector<dcomplex>;

  // explicit instantiations for the class, which were explicitely
  // deactivated until now
  template class vector<ubyte>;
  template class vector<byte>;
  template class vector<char>;
  template class vector<uint16>;
  template class vector<int16>;
  template class vector<int32>;
  template class vector<uint32>;
  template class vector<long>;
  template class vector<float>;
  template class vector<double>;
  template class vector<rgbaPixel>;
  template class vector<frgbPixel>;
  template class vector<ipoint>;
  template class vector<fpoint>;
  template class vector<dpoint>;
  template class vector<ipoint3D>;
  template class vector<fpoint3D>;
  template class vector<dpoint3D>;
  template class vector<fcomplex>;
  template class vector<dcomplex>;
}

namespace std {
  // explicit instantiations for operator<<

  // for ubyte and byte there was already an specialization
  template ostream& 
  operator<<(ostream& s,const lti::genericVector<lti::ubyte>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericVector<lti::byte>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<char>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::int16>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::uint16>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::int32>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::uint32>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<long>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<float>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericVector<double>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::rgbaPixel>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::frgbPixel>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericVector<lti::ipoint>&);

  template ostream& 
  operator<<(ostream& s,const lti::genericVector<lti::fpoint>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::dpoint>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::ipoint3D>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::fpoint3D>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::dpoint3D>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::fcomplex>&);

  template ostream&
  operator<<(ostream& s,const lti::genericVector<lti::dcomplex>&);

}
