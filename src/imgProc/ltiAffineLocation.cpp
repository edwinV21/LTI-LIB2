/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiAffineLocation.cpp
 *         Implementation of lti::affineLocation which extends lti::location
 *         by a transformation matrix.
 *         Global read/write methods to iostream and ostream operators are
 *         provided as well.
 * \author Roland Neul
 * \date   07.06.2004
 *
 * $Id: ltiAffineLocation.cpp,v 1.2 2006-06-06 11:32:58 doerfler Exp $
 */


#include "ltiAffineLocation.h"

#include <limits>

namespace lti {
  // --------------------------------------------------
  // affineLocation
  // --------------------------------------------------

  // default constructor
  affineLocation::affineLocation()
    : location(){
    
    // initialize transformation to identity
    transMatrix.resize(2, 2, 0.0f, Init);
    transMatrix.setIdentity();
  }

  // copy constructor
  affineLocation::affineLocation(const affineLocation& other)
    : location() {
    copy(other);
  }

  // destructor
  affineLocation::~affineLocation() {
  }

  // copy member
  affineLocation&
    affineLocation::copy(const affineLocation& other) {
      location::copy(other);

    transMatrix = other.transMatrix;

    return (*this);
  }

  // alias for copy member
  affineLocation&
    affineLocation::operator=(const affineLocation& other) {
    return (copy(other));
  }

  bool affineLocation::operator==(const affineLocation& p) {
    bool equals = fabs(position.x - p.position.x) <
                  std::numeric_limits<float>::epsilon();
    equals = equals && fabs(position.y - p.position.y) <
                       std::numeric_limits<float>::epsilon();
    equals = equals && fabs(radius - p.radius) <
                       std::numeric_limits<float>::epsilon();
    equals = equals && fabs(angle - p.angle) <
                       std::numeric_limits<float>::epsilon();
    equals = equals && fabs(transMatrix.at(0, 0) - p.transMatrix.at(0, 0)) <
                       std::numeric_limits<float>::epsilon();
    equals = equals && fabs(transMatrix.at(0, 1) - p.transMatrix.at(0, 1)) <
                       std::numeric_limits<float>::epsilon();
    equals = equals && fabs(transMatrix.at(1, 0) - p.transMatrix.at(1, 0)) <
                       std::numeric_limits<float>::epsilon();
    equals = equals && fabs(transMatrix.at(1, 1) - p.transMatrix.at(1, 1)) <
                       std::numeric_limits<float>::epsilon();
    return equals;
  }

  bool affineLocation::operator!=(const affineLocation& p) {
    return (!(p.transMatrix == transMatrix)) || (!location::operator==(p));
  }

  float affineLocation::affineOrientation() const {
    assert (transMatrix.at(0,1)==transMatrix.at(1,0));

    const float& tm00 = transMatrix.at(0,0);
    const float& tm01 = transMatrix.at(0,1);
    const float& tm11 = transMatrix.at(1,1);

    if (closeToZero(abs(tm00-tm11)+abs(tm01))) {
      return 0.f;
    } else {
      return 0.5f*atan2(2.f*tm01, tm00+tm11);
    }
  }

  /*
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  bool read(ioHandler& handler,affineLocation& loc,const bool complete) {
    bool b(true);

    if (complete) {
      b = handler.readBegin();
    }

    // read original location
    b = b && read(handler, static_cast<location&>(loc), false);
    b = b && handler.readDataSeparator();

    // and the additional transformation matrix
    b = b && read(handler, loc.transMatrix, false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  /*
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool write(ioHandler& handler,
             const affineLocation& loc,
             const bool complete) {

    bool b(true);

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && write(handler, static_cast<location>(loc), false);
    b = b && handler.writeDataSeparator();
    b = b && write(handler, loc.transMatrix, false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

}

namespace std {

  ostream& operator<<(ostream& s,const lti::affineLocation& loc) {
    s << "(" << static_cast<lti::location>(loc)
      << "," << loc.transMatrix << ")";
    return s;
  }

}
