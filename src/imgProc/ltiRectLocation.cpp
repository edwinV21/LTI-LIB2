/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiRectLocation.h
 *         Implementation of lti::rectLocation which is a rectangular
 *         local region around an interest point.
 * \author Pablo Alvarado
 * \date   07.01.2002
 *
 * $Id: ltiRectLocation.cpp,v 1.5 2008-08-06 03:40:57 alvarado Exp $
 */


#include "ltiMath.h"
#include "ltiGeometry.h"
#include "ltiRectLocation.h"

namespace lti {

  /*
   * default constructor
   */
  rectLocation::rectLocation()
    : position(0,0),angle(0),maxLength(0),minLength(0) {
  }

  /*
   * Constructor
   * @param pos position of the middle point of the rectLocation
   * @param ang angle of the rectLocation (in radians)
   * @param radi radius or size of the rectLocation
   */
  rectLocation::rectLocation(const ipoint& pos,
                             const float& ang,
                             const float& maxL,
                             const float& minL)
    : position(static_cast<float>(pos.x),
               static_cast<float>(pos.y)),
      angle(ang), maxLength(maxL), minLength(minL) {
  }

  /*
   * Constructor
   * @param pos position of the middle point of the rectLocation
   * @param ang angle of the rectLocation (in radians)
   * @param radi radius or size of the rectLocation
   */
  rectLocation::rectLocation(const fpoint& pos,
                             const float& ang,
                             const float& maxL,
                             const float& minL)
    : position(pos),angle(ang),maxLength(maxL),minLength(minL) {
  }

  /*
   * copy constructor
   */
  rectLocation::rectLocation(const location& other) {
    castFrom(other);
  }


  /*
   * copy constructor
   */
  rectLocation::rectLocation(const rectLocation& other) {
    copy(other);
  }

  /*
   * copy operator
   */
  rectLocation& rectLocation::copy(const rectLocation& other) {
    position = other.position;
    angle = other.angle;
    maxLength = other.maxLength;
    minLength = other.minLength;
    return *this;
  }

  /*
   * copy operator
   */
  rectLocation& rectLocation::castFrom(const location& other) {
    position = other.position;
    angle = other.angle;
    maxLength = other.radius;
    minLength = other.radius;
    return *this;
  }

  /*
   * alias for copy operator
   */
  rectLocation& rectLocation::operator=(const rectLocation& other) {
    return copy(other);
  }

  /*
   * operator ==
   */
  bool rectLocation::operator==(const rectLocation& other) const {
    return ((position == other.position) &&
            (angle == other.angle) &&
            (maxLength == other.maxLength) &&
            (minLength == other.minLength));
  }

  /*
   * operator !=
   */
  bool rectLocation::operator!=(const rectLocation& other) const {
    return ((position != other.position) ||
            (angle != other.angle) ||
            (maxLength != other.maxLength) ||
            (minLength != other.minLength));
  }

  float rectLocation::calculateArea() const {
    return maxLength*minLength;
  }

  /*
   * Comparison operator.
   *
   * A rectLocation is smaller than another one if its radius is smaller, or
   * in case of equal radii, if the position is smaller, i.e. if it has
   * smaller y-coordinate, or in case of equal y-position, if it has
   * smaller x-coordinate.
   */
  bool rectLocation::operator<(const rectLocation& other) {
    float athis = calculateArea();
    float o = other.calculateArea();
    if (athis < o) {
      return true;
    } else if (athis == o) {
      if (position < other.position) {
        return true;
      } else if (position == other.position) {
        return (angle < other.angle);
      }
    }

    return false;
  }

  /*
   * Comparison operator.
   *
   * A rectLocation is greater than another one if its radius is greater, or
   * in case of equal radii, if the position is greater, i.e. if it has
   * greater y-coordinate, or in case of equal y-position, if it has
   * greater x-coordinate.
   */
  bool rectLocation::operator>(const rectLocation& other) {
    float athis = calculateArea();
    float o = other.calculateArea();
    if (athis > o) {
      return true;
    } else if (athis == o) {
      if (position < other.position) {
        return false;
      } else if (position == other.position) {
        return (angle > other.angle);
      } else {
        return true;
      }
    }

    return false;
  }

  /*
   * Multiply the radius with the given factor
   */
  rectLocation& rectLocation::scale(const float& factor) {
    maxLength *= factor;
    minLength *= factor;
    position.multiply(factor);
    return *this;
  }

  /*
   * Multiply the other rectLocation with the given factor and leave the
   * result here.
   */
  rectLocation& rectLocation::scale(const rectLocation& other,
                                    const float& factor) {
    position = other.position*factor;
    angle = other.angle;
    maxLength = other.maxLength*factor;
    minLength = other.minLength*factor;
    return *this;
  }

  /*
   * Shift the rectLocation by the given point
   */
  rectLocation& rectLocation::shift(const ipoint& shft) {
    position.x += shft.x;
    position.y += shft.y;
    return *this;
  }

  /*
   * Shift the rectLocation by the given point
   */
  rectLocation& rectLocation::shift(const fpoint& shft) {
    position.add(shft);
    return *this;
  }

  /*
   * Shift the other rectLocation by the given point and leave the result here
   */
  rectLocation& rectLocation::shift(const rectLocation& other,
                                    const ipoint& shft) {
    position.x = other.position.x+shft.x;
    position.y = other.position.y+shft.y;
    angle = other.angle;
    maxLength = other.maxLength;
    minLength = other.minLength;
    return *this;
  }

  /*
   * Shift the other rectLocation by the given point and leave the result here
   */
  rectLocation& rectLocation::shift(const rectLocation& other,
                                    const fpoint& shft) {

    position.x = other.position.x+shft.x;
    position.y = other.position.y+shft.y;
    angle = other.angle;
    maxLength = other.maxLength;
    minLength = other.minLength;
    return *this;
  }

  /*
   * Add the given angle in radius to the actual angle
   */
  rectLocation& rectLocation::rotate(const float& factor) {
    angle+=factor;
    return *this;
  }

  /*
   * Add the given angle in radius to the angle of the other rectLocation and
   * leave the result here.
   */
  rectLocation& rectLocation::rotate(const rectLocation& other,
                                     const float& factor) {
    position  = other.position;
    angle     = other.angle+factor;
    maxLength = other.maxLength;
    minLength = other.minLength;
    return *this;
  }

  /*
   * returns the distance between the borders of two locations or zero
   * if they overlap or if one of the locations lies inside the other one.
   */
  float rectLocation::distanceSqr(const rectLocation& other) {
    fpoint pt,po;
    return distanceSqr(other,pt,po);
  }

  /*
   * returns the distance between the borders of two locations or zero
   * if they overlap or if one of the locations lies inside the other one.
   */
  float rectLocation::distanceSqr(const rectLocation& other,
                                  fpoint& pt,
                                  fpoint& po) {
    
    static const int I   = 0;
    static const int II  = 1;
    static const int III = 2;
    static const int IV  = 3;

    fpoint d1,p1[4],d2,p2[4];
    fpoint co1,co2;
    int a1,b1,c1,a2,b2,c2;

    const float
      cosa1(cos(angle)),
      sina1(-sin(angle)),
      cosa2(cos(other.angle)),
      sina2(-sin(other.angle));

    const float m(maxLength/2.0f);
    const float l(minLength/2.0f);
    const float mo(other.maxLength/2.0f);
    const float lo(other.minLength/2.0f);

    // select the two lines of the first location that could be the
    // nearest to the next one.
    // rotation matrix:
    //
    // | cos(a)  sin(a) | | m |
    // |-sin(a)  cos(a) | | l |

    d1.subtract(other.position,position);
    d2.subtract(position,other.position);

    co1.x = cosa1*d1.x - sina1*d1.y;
    co1.y = sina1*d1.x + cosa1*d1.y;

    co2.x = cosa2*d2.x - sina2*d2.y;
    co2.y = sina2*d2.x + cosa2*d2.y;

    // the corners of this location:
    p1[I].x =  m*cosa1 + l*sina1 + position.x; // I
    p1[I].y = -m*sina1 + l*cosa1 + position.y;

    p1[II].x = -m*cosa1 + l*sina1 + position.x; // II
    p1[II].y =  m*sina1 + l*cosa1 + position.y;

    p1[III].x = -m*cosa1 - l*sina1 + position.x; // III
    p1[III].y =  m*sina1 - l*cosa1 + position.y;

    p1[IV].x =  m*cosa1 - l*sina1 + position.x; // IV
    p1[IV].y = -m*sina1 - l*cosa1 + position.y;

    // the corners of the other location:
    p2[I].x =  mo*cosa2 + lo*sina2 + other.position.x; // I
    p2[I].y = -mo*sina2 + lo*cosa2 + other.position.y;

    p2[II].x = -mo*cosa2 + lo*sina2 + other.position.x; // II
    p2[II].y =  mo*sina2 + lo*cosa2 + other.position.y;

    p2[III].x = -mo*cosa2 - lo*sina2 + other.position.x; // III
    p2[III].y =  mo*sina2 - lo*cosa2 + other.position.y;

    p2[IV].x =  mo*cosa2 - lo*sina2 + other.position.x; // IV
    p2[IV].y = -mo*sina2 - lo*cosa2 + other.position.y;

    if (co1.x >= 0) {
      if (co1.y >= 0) {
        // 1st quadrant
        a1 = I; b1 = IV; c1 = II;
      } else {
        // 4th quadrant
        a1 = IV; b1 = III; c1 = I;
      }
    } else {
      if (co1.y >= 0) {
        // 2nd quadrant
        a1 = II; b1 = I; c1 = III;
      } else {
        // 3rd quadrant
        a1 = III; b1 = II; c1 = IV;
      }
    }

    // select the two lines of the first location that could be the
    // nearest to the next one.
    if (co2.x >= 0) {
      if (co2.y >= 0) {
        // 1st quadrant
        a2 = I; b2 = IV; c2 = II;
      } else {
        // 4th quadrant
        a2 = IV; b2 = III; c2 = I;
      }
    } else {
      if (co2.y >= 0) {
        // 2nd quadrant
        a2 = II; b2 = I; c2 = III;
      } else {
        // 3rd quadrant
        a2 = III; b2 = II; c2 = IV;
      }
    }

    fpoint dpa[4];
    fpoint dpb[4];
    float d[4];

    d[0] = minDistanceSqr(p1[a1],p1[b1],p2[a2],p2[b2],dpa[0],dpb[0]);
    d[1] = minDistanceSqr(p1[a1],p1[b1],p2[a2],p2[c2],dpa[1],dpb[1]);
    d[2] = minDistanceSqr(p1[a1],p1[c1],p2[a2],p2[b2],dpa[2],dpb[2]);
    d[3] = minDistanceSqr(p1[a1],p1[c1],p2[a2],p2[c2],dpa[3],dpb[3]);

    int idx(0);
    for (int i=1;i<4;++i) {
      if (d[i] < d[idx]) {
        idx = i;
      }
    }

    pt = dpa[idx];
    po = dpb[idx];

    return d[idx];
  }

  /*
   * Check if the given point can be considered within the rectLocation, i.e.
   * if ||p-position|| < radius
   */
  bool rectLocation::contains(const ipoint& p) const {
    const float sina(cos(angle)),cosa(cos(angle));
    return (((2*(abs(cosa*p.x+sina*p.y))) <= maxLength) &&
            ((2*(abs(cosa*p.y-sina*p.x))) <= minLength));
  }

  /*
   * Check if the given point can be considered within the rectLocation, i.e.
   * if ||p-position|| < radius
   */
  bool rectLocation::contains(const fpoint& p) const {
    const float sina(cos(angle)),cosa(cos(angle));
    return (((2*(abs(cosa*p.x+sina*p.y))) <= maxLength) &&
            ((2*(abs(cosa*p.y-sina*p.x))) <= minLength));
  }

  /*
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  bool read(ioHandler& handler,rectLocation& loc,const bool complete) {
    bool b(true);

    if (complete) {
      b = handler.readBegin();
    }

    b = b && read(handler,loc.position);
    b = b && handler.readDataSeparator();
    b = b && read(handler,loc.angle);
    b = b && handler.readDataSeparator();
    b = b && read(handler,loc.maxLength);
    b = b && handler.readDataSeparator();
    b = b && read(handler,loc.minLength);

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
             const rectLocation& loc,
             const bool complete) {

    bool b(true);

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && write(handler,loc.position);
    b = b && handler.writeDataSeparator();
    b = b && write(handler,loc.angle);
    b = b && handler.writeDataSeparator();
    b = b && write(handler,loc.maxLength);
    b = b && handler.writeDataSeparator();
    b = b && write(handler,loc.minLength);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }



}

namespace std {

  ostream& operator<<(ostream& s,const lti::rectLocation& loc) {
    s << "(" << loc.position << ","
      << loc.angle  << ","
      << loc.maxLength << ","
      << loc.minLength << ")";
    return s;
  }
}


