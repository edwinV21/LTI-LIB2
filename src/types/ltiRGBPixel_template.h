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
 * \file   ltiRGBPixel_template.h
 *         Contains the basic template class to represent RGB pixels
 *         (without alpha channel)
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiRGBPixel_template.h,v 1.4 2006-03-23 12:19:35 doerfler Exp $
 */

namespace lti {

  template<typename T>
  rgbPixel<T>::rgbPixel() {
  }

  template<typename T>
  rgbPixel<T>::rgbPixel(const T r,
                        const T g,
                        const T b) {
    red   = r;
    green = g;
    blue  = b;
  }

  template<typename T>
  rgbPixel<T>::rgbPixel(const rgbaPixel& other) {
    red = static_cast<T>(other.red);
    green = static_cast<T>(other.green);
    blue = static_cast<T>(other.blue);
  }

  template<typename T>
  bool read(ioHandler& handler,rgbPixel<T>& p,const bool) {
    bool b = true;

    // the begin and end tokens are mandatory here! ignore the complete flag...
    handler.readBegin();

    b = b && handler.read(p.red);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.green);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.blue);

    b = b && handler.readEnd();

    return b;
  }

  template<typename T>
  bool write(ioHandler& handler,
             const rgbPixel<T>& p,
             const bool){
    bool b = true;

    // the begin token is mandatory here, so ignore the complete flag
    b = b && handler.writeBegin();

    b = b && handler.write(p.getRed());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.getGreen());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.getBlue());
    b = b && handler.writeEnd();

    return b;
  }
}

namespace std {
  template<typename T>
  ostream& operator<<(ostream& s,const lti::rgbPixel<T>& p) {
    s << "(" 
      << p.red   << ","
      << p.green << ","
      << p.blue  << ")";
    return s;
  }

  template<>
  ostream& operator<<(ostream& s,const lti::rgbPixel<lti::byte>& p) {
    s << "(" 
      << static_cast<int>(p.red)   << ","
      << static_cast<int>(p.green) << ","
      << static_cast<int>(p.blue)  << ")";
    return s;
  }

  template<>
  ostream& operator<<(ostream& s,const lti::rgbPixel<lti::ubyte>& p) {
    s << "(" 
      << static_cast<int>(p.red)   << ","
      << static_cast<int>(p.green) << ","
      << static_cast<int>(p.blue)  << ")";
    return s;
  }

  //inline ostream& operator>>(istream& s,const lti::rgbPixel& p);
  template<typename T>
  istream& operator>>(istream& s,lti::rgbPixel<T>& p) {
    char c;
    s >> c
      >> p.red >> c
      >> p.green >> c
      >> p.blue >> c;

    return s;
  }

  template<>
  istream& operator>>(istream& s,lti::rgbPixel<lti::byte>& p) {
    char c;
    int r,g,b;
    s >> c
      >> r >> c
      >> g >> c
      >> b >> c;

    p.set(static_cast<lti::byte>(r),
          static_cast<lti::byte>(g),
          static_cast<lti::byte>(b));

    return s;
  }

  template<>
  istream& operator>>(istream& s,lti::rgbPixel<lti::ubyte>& p) {
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




