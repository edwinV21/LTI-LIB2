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
 * \file   ltiRGBAPixel.cpp
 *         Contains the basic template class to represent RGB pixels with
 *         an alpha channel
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBAPixel.cpp,v 1.7 2006-09-21 08:33:14 doerfler Exp $
 */

#include "ltiRGBAPixel.h"
#include <iostream>

namespace lti {

  // DO NOT INITIALIZE VALUE HERE!
  // this would make constructing lti::image too expensive
  rgbaPixel::rgbaPixel() {
  }

  rgbaPixel::rgbaPixel(const uint32 val) : value(val)  {
  }

  rgbaPixel::rgbaPixel(const rgbaPixel& other) : value(other.value) {
  }

  rgbaPixel::rgbaPixel(const ubyte r,
                       const ubyte g,
                       const ubyte b,
                       const ubyte d)
    : blue(b),
      green(g),
      red(r),
      alpha(d) {
  }

  bool rgbaPixel::read(ioHandler& handler,const bool) {
    int tmp;
    bool b = true;

    // the begin and end tokens are mandatory here! ignore the complete flag...
    handler.readBegin();

    b = b && handler.read(tmp);
    setRed(static_cast<ubyte>(tmp));

    b = b && handler.readDataSeparator();

    b = b && handler.read(tmp);
    setGreen(static_cast<ubyte>(tmp));

    b = b && handler.readDataSeparator();

    b = b && handler.read(tmp);
    setBlue(static_cast<ubyte>(tmp));
    
    if (!handler.tryEnd()) {
      // the new rgbaPixel writes also the alpha value.  To allow
      // old stuff to be readed, the alpha value is optionally readed
      b = b && handler.readDataSeparator();

      b = b && handler.read(tmp);
      setAlpha(static_cast<ubyte>(tmp));

      // the begin and end tokens are mandatory here! ignore the
      // complete flag...
      b = b && handler.readEnd();
    } else {
      setAlpha(0);
    }

    return b;
  }

  bool rgbaPixel::write(ioHandler& handler,const bool) const {
    bool b = true;

    // the begin token is mandatory here, so ignore the complete flag
    b = b && handler.writeBegin();

    b = b && handler.write(static_cast<int>(red));
    b = b && handler.writeDataSeparator();
    b = b && handler.write(static_cast<int>(green));
    b = b && handler.writeDataSeparator();
    b = b && handler.write(static_cast<int>(blue));
    b = b && handler.writeDataSeparator();
    b = b && handler.write(static_cast<int>(alpha));
    b = b && handler.writeEnd();

    return b;
  }

  bool read(ioHandler& handler,rgbaPixel& p,const bool complete) {
    return p.read(handler,complete);
  }

  bool write(ioHandler& handler,const rgbaPixel& p,const bool complete) {
    return p.write(handler,complete);
  }
  
}

namespace std {
  ostream& operator<<(ostream& s,const lti::rgbaPixel& p) {
    s << "("; 
    s << static_cast<int>(p.red);
    s << ",";
    s << static_cast<int>(p.green);
    s << ",";
    s << static_cast<int>(p.blue);
    s << ")";
    return s;
  }

  //inline ostream& operator>>(istream& s,const lti::rgbaPixel& p);
  istream& operator>>(istream& s,lti::rgbaPixel& p) {
    char c;
    int r,g,b;
    s >> c
      >> r >> c
      >> g >> c
      >> b >> c;
    p.set(static_cast<lti::ubyte>(r),
          static_cast<lti::ubyte>(g),
          static_cast<lti::ubyte>(b));

    return s;
  }
}




