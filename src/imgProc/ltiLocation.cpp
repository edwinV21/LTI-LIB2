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
 * \file   ltiLocation.cpp
 *         Implementation of lti::location which is an interest point in
 *         an image and circular neighborhood. 
 * \author Pablo Alvarado
 * \date   07.01.2002
 *
 * $Id: ltiLocation.cpp,v 1.5 2007-12-24 02:58:22 alvarado Exp $
 */


#include "ltiMath.h"
#include "ltiGeometry.h"
#include "ltiLocation.h"

namespace lti {
  /*
   * Default constructor
   *
   * This initializes the position, angle and radius with zero values.
   */
  location::location() : position(0,0), angle(0), radius(0) {
  }

  /*
   * Constructor
   * @param pos position of the middle point of the location
   * @param ang angle of the location (in radians)
   * @param radi radius or size of the location
   */
  location::location(const ipoint& pos, const float& ang,const float& rad)
    : position(float(pos.x),float(pos.y)),angle(ang),radius(rad) {
  }

  /*
   * Constructor
   * @param pos position of the middle point of the location
   * @param ang angle of the location (in radians)
   * @param radi radius or size of the location
   */
  location::location(const fpoint& pos,
                     const float& ang,
                     const float& rad)
    : position(pos),angle(ang),radius(rad) {
  }

  /*
   * copy constructor
   */
  location::location(const location& other) {
    copy(other);
  }

  /*
   * copy operator
   */
  location& location::copy(const location& other) {
    position = other.position;
    angle = other.angle;
    radius = other.radius;
    return *this;
  }

  /*
   * alias for copy operator
   */
  location& location::operator=(const location& other) {
    return copy(other);
  }

  /*
   * operator ==
   */
  bool location::operator==(const location& other) const {
    return ((position == other.position) &&
            (angle == other.angle) &&
            (radius == other.radius));
  }

  /*
   * operator !=
   */
  bool location::operator!=(const location& other) const {
    return ((position != other.position) ||
            (angle != other.angle) ||
            (radius != other.radius));
  }

  /*
   * Comparison operator.
   *
   * A location is smaller than another one if its radius is smaller, or
   * in case of equal radii, if the position is smaller, i.e. if it has
   * smaller y-coordinate, or in case of equal y-position, if it has
   * smaller x-coordinate.
   */
  bool location::operator<(const location& other) {
    if (radius < other.radius) {
      return true;
    } else if (radius == other.radius) {
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
   * A location is greater than another one if its radius is greater, or
   * in case of equal radii, if the position is greater, i.e. if it has
   * greater y-coordinate, or in case of equal y-position, if it has
   * greater x-coordinate.
   */
  bool location::operator>(const location& other) {
    if (radius > other.radius) {
      return true;
    } else if (radius == other.radius) {
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
  location& location::scale(const float& factor) {
    radius *= factor;
    position.multiply(factor);
    return *this;
  }

  /*
   * Multiply the other location with the given factor and leave the
   * result here.
   */
  location& location::scale(const location& other,const float& factor) {
    position = other.position*factor;
    angle = other.angle;
    radius = other.radius*factor;
    return *this;
  }

  /*
   * Shift the location by the given point
   */
  location& location::shift(const ipoint& shft) {
    position.x += shft.x;
    position.y += shft.y;
    return *this;
  }

  /*
   * Shift the location by the given point
   */
  location& location::shift(const fpoint& shft) {
    position.add(shft);
    return *this;
  }

  /*
   * Shift the other location by the given point and leave the result here
   */
  location& location::shift(const location& other,const ipoint& shft) {
    position.x = other.position.x+shft.x;
    position.y = other.position.y+shft.y;
    angle = other.angle;
    radius = other.radius;
    return *this;
  }

  /*
   * Shift the other location by the given point and leave the result here
   */
  location& location::shift(const location& other,const fpoint& shft) {
    position.x = other.position.x+shft.x;
    position.y = other.position.y+shft.y;
    angle = other.angle;
    radius = other.radius;
    return *this;
  }

  /*
   * Add the given angle in radius to the actual angle
   */
  location& location::rotate(const float& factor) {
    angle+=factor;
    return *this;
  }

  /*
   * Add the given angle in radius to the angle of the other location and
   * leave the result here.
   */
  location& location::rotate(const location& other,const float& factor) {
    position = other.position;
    angle    = other.angle+factor;
    radius   = other.radius;
    return *this;
  }

  /*
   * returns the square of the distance between the borders of two
   * locations or zero if they overlap or if one of the locations
   * lies inside the other one.
   */
  float location::distance(const location& other) {

    float d;
    d = sqrt(position.distanceSqr(other.position)) - (other.radius+radius);

    if (d<0.f) {
      d = 0.f;
    }

    return d;
  }


  /*
   * Check if the given point can be considered within the location, i.e.
   * if ||p-position|| < radius
   */
  bool location::contains(const ipoint& p) const {
    float dx,dy;
    dx = (p.x - position.x);
    dy = (p.y - position.y);

    return ((dx*dx+dy*dy) < radius*radius);
  }

  /*
   * Check if the given point can be considered within the location, i.e.
   * if ||p-position|| < radius
   */
  bool location::contains(const fpoint& p) const {
    float dx,dy;
    dx = (p.x - position.x);
    dy = (p.y - position.y);

    return ((dx*dx+dy*dy) < radius*radius);
  }

  /*
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  bool read(ioHandler& handler,location& loc,const bool complete) {
    bool b(true);

    if (complete) {
      b = handler.readBegin();
    }

    b = b && read(handler,loc.position);
    b = b && handler.readDataSeparator();
    b = b && read(handler,loc.angle);
    b = b && handler.readDataSeparator();
    b = b && read(handler,loc.radius);

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
             const location& loc,
             const bool complete) {

    bool b(true);

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && write(handler,loc.position);
    b = b && handler.writeDataSeparator();
    b = b && write(handler,loc.angle);
    b = b && handler.writeDataSeparator();
    b = b && write(handler,loc.radius);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

}

namespace std {

  ostream& operator<<(ostream& s,const lti::location& loc) {
    s << "(" << loc.position << ","
      << loc.angle  << ","
      << loc.radius << ")";
    return s;
  }


}


