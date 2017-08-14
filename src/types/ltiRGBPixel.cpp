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
 * \file   ltiRGBPixel.cpp
 *         Contains the basic template class to represent RGB pixels (without
 *         an alpha channel)
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBPixel.cpp,v 1.2 2004-08-11 11:01:56 doerfler Exp $
 */


#include <iostream>

#include "ltiRGBPixel.h"
#include "ltiRGBPixel_template.h"

namespace lti {
  // make instances of all classes and global functions for the eight
  // supported types.


  template class rgbPixel<byte>;
  template class rgbPixel<ubyte>;
  template class rgbPixel<int16>;
  template class rgbPixel<uint16>;
  template class rgbPixel<int32>;
  template class rgbPixel<uint32>;
  template class rgbPixel<float>;
  template class rgbPixel<double>;

  template bool read(ioHandler& handler,
                     rgbPixel<byte>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<ubyte>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<int16>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<uint16>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<int32>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<uint32>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<float>& p,
                     const bool complete);
  template bool read(ioHandler& handler,
                     rgbPixel<double>& p,
                     const bool complete);
  
  template bool write(ioHandler& handler,
                      const rgbPixel<byte>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<ubyte>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<int16>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<uint16>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<int32>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<uint32>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<float>& p,
                      const bool complete);
  template bool write(ioHandler& handler,
                      const rgbPixel<double>& p,
                      const bool complete);
}

namespace std {
  //  template ostream& operator<<(ostream& s,const lti::rgbPixel<lti::byte>&)
  //  template ostream& operator<<(ostream& s,const lti::rgbPixel<lti::ubyte>&);
  template ostream& operator<<(ostream& s,const lti::rgbPixel<lti::int16>&);
  template ostream& operator<<(ostream& s,const lti::rgbPixel<lti::uint16>&);
  template ostream& operator<<(ostream& s,const lti::rgbPixel<lti::int32>&);
  template ostream& operator<<(ostream& s,const lti::rgbPixel<lti::uint32>&);
  template ostream& operator<<(ostream& s,const lti::rgbPixel<float>&);
  template ostream& operator<<(ostream& s,const lti::rgbPixel<double>&);

  //  template istream& operator>>(istream& s,lti::rgbPixel<lti::byte>&);
  //  template istream& operator>>(istream& s,lti::rgbPixel<lti::ubyte>&);
  template istream& operator>>(istream& s,lti::rgbPixel<lti::int16>&);
  template istream& operator>>(istream& s,lti::rgbPixel<lti::uint16>&);
  template istream& operator>>(istream& s,lti::rgbPixel<lti::int32>&);
  template istream& operator>>(istream& s,lti::rgbPixel<lti::uint32>&);
  template istream& operator>>(istream& s,lti::rgbPixel<float>&);
  template istream& operator>>(istream& s,lti::rgbPixel<double>&);
}
