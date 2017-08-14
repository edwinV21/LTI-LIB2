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


/**
 * \file   ltiRectLocation.h
 *         Definition of lti::rectLocation which is a rectangular local region
 *         around an interest point.
 *
 *         Global read/write methods to iostream and ostream operators are
 *         provided as well.
 * \author Pablo Alvarado
 * \date   07.01.2002
 *
 * $Id: ltiRectLocation.h,v 1.3 2005-01-03 16:09:05 alvarado Exp $
 */

#ifndef _LTI_RECT_LOCATION_H_
#define _LTI_RECT_LOCATION_H_

#include "ltiPoint.h"
#include "ltiLocation.h"

namespace lti {
  /**
   * The rectLocation class specifies a small region in an image or
   * channel around an interest point.
   * The rectangular location is represented by its position in an
   * image or channel, an orientation, the length at the orientation
   * direction (maxLenght), and the length at the perpendicular
   * direction (minLength).
   *
   * The difference to lti::location is that the regions are considered
   * rectangular.  Two lengths are needed, where the orientation angle is
   * always given for the maxLength.
   *
   * You can get rectangular %locations with the lti::boundingBox %functor, and
   * use them to check if objects overlap or not.
   */
  class rectLocation {
  public:
    /**
     * Default constructor
     */
    rectLocation();

    /**
     * Constructor
     * @param pos position of the center point of the rectLocation
     * @param ang angle of the rectLocation (in radians)
     * @param maxLength length in pixels of the principal axis
     * @param minLength length in pixels of the second axis
     */
    rectLocation(const ipoint& pos,
                 const float& ang,
                 const float& maxLength,
                 const float& minLength);

    /**
     * Constructor
     * @param pos position of the center point of the rectLocation
     * @param ang angle of the rectLocation (in radians)
     * @param maxLength length in pixels of the principal axis
     * @param minLength length in pixels of the second axis
     */
    rectLocation(const fpoint& pos,
                 const float& ang,
                 const float& maxLength,
                 const float& minLength);

    /**
     * Copy constructor from a normal location
     */
    rectLocation(const location& loc);

    /**
     * Copy constructor
     */
    rectLocation(const rectLocation& other);

    /**
     * Copy operator
     */
    rectLocation& copy(const rectLocation& other);

    /**
     * Copy the other location into this rectLocation.  The radius of
     * the location will be assumed as the maxLength and minLength.
     */
    rectLocation& castFrom(const location& other);


    /**
     * Alias for copy operator
     */
    rectLocation& operator=(const rectLocation& other);

    /**
     * Operator ==
     */
    bool operator==(const rectLocation& p) const;

    /**
     * Operator !=
     */
    bool operator!=(const rectLocation& p) const;

    /**
     * Comparition operator.
     *
     * A rectLocation is smaller than another one if its area is smaller, or
     * in case of equal areas, if the position is smaller, i.e. if it has
     * smaller y-coordinate, or in case of equal y-position, if it has
     * smaller x-coordinate.
     */
    bool operator<(const rectLocation& other);

    /**
     * Comparition operator.
     *
     * A rectLocation is greater than another one if its area is greater, or
     * in case of equal radii, if the position is greater, i.e. if it has
     * greater y-coordinate, or in case of equal y-position, if it has
     * greater x-coordinate.
     */
    bool operator>(const rectLocation& other);

    /**
     * Multiply the lengths and position with the given factor
     */
    rectLocation& scale(const float& factor);

    /**
     * Multiply the other rectLocation's lengths and position with the
     * given factor and leave the result here.
     */
    rectLocation& scale(const rectLocation& other,
                        const float& factor);

    /**
     * Shift the rectLocation by the given point
     */
    rectLocation& shift(const ipoint& shft);

    /**
     * Shift the rectLocation by the given point
     */
    rectLocation& shift(const fpoint& shft);

    /**
     * Shift the other rectLocation by the given point and leave the
     * result here
     */
    rectLocation& shift(const rectLocation& other,const ipoint& shft);

    /**
     * Shift the other rectLocation by the given point and leave the
     * result here
     */
    rectLocation& shift(const rectLocation& other,const fpoint& shft);

    /**
     * Add the given angle in radians to the actual angle
     */
    rectLocation& rotate(const float& factor);

    /**
     * Add the given angle in radians to the angle of the other rectLocation and
     * leave the result here.
     */
    rectLocation& rotate(const rectLocation& other,const float& factor);

    /**
     * Check if the given point can be considered within the rectLocation.
     */
    bool contains(const ipoint& p) const;

    /**
     * Check if the given point can be considered within the rectLocation.
     */
    bool contains(const fpoint& p) const;

    /**
     * Returns the square of the distance between the borders of two
     * locations or zero if they overlap or if one of the locations
     * lies inside the other one.
     *
     * @param other the other rectLocation to be compared with
     * @return minimal distance to other location
     */
    float distanceSqr(const rectLocation& other);

    /**
     * Returns the square of the distance between the borders of two
     * locations or zero if they overlap or if one of the locations
     * lies inside the other one.
     *
     * @param other the other rectLocation to be compared with
     * @param pt point in the border of this location with the
     *           smallest distance.
     * @param po point in the border of the other location with the
     *           smallest distance.
     * @return minimal distance to other location
     */
    float distanceSqr(const rectLocation& other,
                      fpoint& pt,
                      fpoint& po);

    /**
     * Get the area of this location (maxLength*minLength)
     */
    float calculateArea() const;

    /**
     * Position of the rectLocation
     */
    fpoint position;

    /**
     * Angle in radians of the rectLocation.  It is usually given for
     * the image coordinates, i.e. for a left coordinate system, in which
     * positive angles are given clock-wise.
     */
    float angle;

    /**
     * Major length.
     *
     * The angle corresponds to the axis with the maxLength
     */
    float maxLength;

    /**
     * Minor length.
     *
     * Should be smaller than maximum length
     */
    float minLength;
  };

  /**
   * Global function to read a lti::rectLocation from an ioHandler.
   */
  bool read(ioHandler& handler,
            rectLocation& mat,
            const bool complete=true);

  /**
   * Global function to write a lti::rectLocation into an ioHandler.
   */
  bool write(ioHandler& handler,
             const rectLocation& mat,
             const bool complete=true);
}

namespace std {

  ostream& operator<<(ostream& s,
                      const lti::rectLocation& loc);

}

#endif


