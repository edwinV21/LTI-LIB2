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
 * \file   ltiLocation.h
 *         Definition of lti::location which is an interest point in
 *         an image and circular neighborhood. 
 *
 *         Global read/write methods to iostream and ostream operators are
 *         provided as well.
 * \author Pablo Alvarado
 * \date   07.01.2002
 *
 * $Id: ltiLocation.h,v 1.4 2005-01-03 16:09:05 alvarado Exp $
 */

#ifndef _LTI_LOCATION_H_
#define _LTI_LOCATION_H_

#include "ltiPoint.h"

namespace lti {

  /**
   * The location class specifies an interest point and a small
   * surrounding region in an image or channel.  It is mainly used in
   * the extraction of local descriptors.  There are some functors to
   * extract the important or salient locations in an image, for
   * example lti::pyramidLocationSearch.
   *
   * A locations is represented by its position in an image or channel,
   * an orientation and a radius.
   *
   */
  class location {
  public:
    /**
     * default constructor
     */
    location();

    /**
     * Constructor
     * @param pos position of the center point of the location
     * @param ang angle of the location (in radians)
     * @param rad radius or size of the location
     */
    location(const ipoint& pos,const float& ang,const float& rad);

    /**
     * Constructor
     * @param pos position of the center point of the location
     * @param ang angle of the location (in radians)
     * @param rad radius or size of the location
     */
    location(const fpoint& pos,const float& ang,const float& rad);

    /**
     * copy constructor
     */
    location(const location& other);

    /**
     * copy operator
     */
    location& copy(const location& other);

    /**
     * alias for copy operator
     */
    location& operator=(const location& other);

    /**
     * operator ==
     */
    bool operator==(const location& p) const;

    /**
     * operator !=
     */
    bool operator!=(const location& p) const;

    /**
     * Comparison operator: less than.
     *
     * A location is smaller than another one if its radius is smaller, or
     * in case of equal radii, if the position is smaller, i.e. if it has
     * smaller y-coordinate, or in case of equal y-position, if it has
     * smaller x-coordinate.
     */
    bool operator<(const location& other);

    /**
     * Comparison operator: greater than.
     *
     * A location is greater than another one if its radius is greater, or
     * in case of equal radii, if the position is greater, i.e. if it has
     * greater y-coordinate, or in case of equal y-position, if it has
     * greater x-coordinate.
     */
    bool operator>(const location& other);

    /**
     * Multiply the radius and position with the given factor
     *
     * @param factor scaling factor for radius and position.
     */
    location& scale(const float& factor);

    /**
     * Multiply the other location's radius and position with the
     * given factor and leave the result here.
     *
     * @param other location to be scaled
     * @param factor scaling factor for radius and position.
     */
    location& scale(const location& other,const float& factor);

    /**
     * Shift the location by the given point.
     *
     * @param shft shift the position by this offset
     */
    location& shift(const ipoint& shft);

    /**
     * Shift the location by the given point
     *
     * @param shft shift the position by this offset
     */
    location& shift(const fpoint& shft);

    /**
     * Shift the other location by the given point and leave the result here
     *
     * @param other location to be shifted
     * @param shft shift the position of \c other by this offset
     */
    location& shift(const location& other,const ipoint& shft);

    /**
     * Shift the other location by the given point and leave the result here
     *
     * @param other location to be shifted
     * @param shft shift the position of \c other by this offset
     */
    location& shift(const location& other,const fpoint& shft);

    /**
     * Rotate the location by adding the given angle in radians to the
     * actual angle.
     *
     * @param ang rotation angle
     */
    location& rotate(const float& ang);

    /**
     * Rotate the location by adding the given angle in radians to the
     * angle of the other location and leave the result here.
     *
     * @param other location to be rotated
     * @param ang rotation angle
     */
    location& rotate(const location& other,const float& ang);

    /**
     * Check if the given point can be considered within the location, i.e.
     * if ||p-position||_L2 < radius
     * 
     * @param p query point
     */
    bool contains(const ipoint& p) const;

    /**
     * Check if the given point can be considered within the location, i.e.
     * if ||p-position||_L2 < radius
     * 
     * @param p query point
     */
    bool contains(const fpoint& p) const;

    /**
     * returns the distance between the borders of two
     * locations or zero if they overlap or one of the locations
     * lies inside the other one.
     */
    float distance(const location& other);

    /**
     * Position of the location.
     */
    fpoint position;

    /**
     * Angle in radians of the location.  It is usually given for
     * the image coordinates, i.e. for a left coordinate system, in which
     * positive angles are given clock-wise.
     */
    float angle;

    /**
     * Radius or size of the location
     */
    float radius;
  };

  /**
   * Read a location from the ioHandler
   */
  bool read(ioHandler& handler,
            location& mat,
            const bool complete=true);

  /**
   * Write a location into the ioHandler
   */
  bool write(ioHandler& handler,
             const location& mat,
             const bool complete=true);

}

namespace std {
  ostream& operator<<(ostream& s,
                      const lti::location& loc);
}

#endif


