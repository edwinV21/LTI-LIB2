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
 * \file   ltiAffineLocation.h
 *         Definition of lti::affineLocation which extends lti::location
 *         by a transformation matrix.
 *         Global read/write methods to iostream and ostream operators are
 *         provided as well.
 * \author Roland Neul
 * \date   07.06.2004
 *
 * $Id: ltiAffineLocation.h,v 1.2 2006-06-06 11:32:58 doerfler Exp $
 */


#ifndef _LTI_AFFINE_LOCATION_H_
#define _LTI_AFFINE_LOCATION_H_

#include <ltiMatrix.h>
#include <ltiLocation.h>

namespace lti {
  /**
   * The %affineLocation class is simply used to store the %parameters
   * associated with an affine %location. The only difference between
   * lti::affineLocation and lti::location is an additional transformation
   * Matrix associated with the affine %location.
   */
  class affineLocation : public location {
  public:
    /**
     * Default constructor.
     */
    affineLocation();

    /**
     * Copy constructor.
     * @param other the object to be copied
     */
    affineLocation(const affineLocation& other);

    /**
     * Destructor
     */
    ~affineLocation();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    affineLocation& copy(const affineLocation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    affineLocation& operator=(const affineLocation& other);
    
    /**
     * operator ==
     */
    bool operator==(const affineLocation& p);

    /**
     * operator !=
     */
    bool operator!=(const affineLocation& p);

    /** 
     * The affine transformation matrix 
     *
     * Default: identity
     */
    matrix<float> transMatrix;

    /**
     * Return the orientation of the major axis of shape description
     * (transMatrix). For a perfect circle 0 is returned. Due to the symmetry
     * the range is [0;Pi].
     */
    float affineOrientation() const;
    
  };
  
  /**
   * Read a affine %location from the ioHandler
   */
  bool read(ioHandler& handler,
            affineLocation& loc,
            const bool complete=true);

  /**
   * Write a affine %location into the ioHandler
   */
  bool write(ioHandler& handler,
             const affineLocation& loc,
             const bool complete=true);

}

namespace std {
  ostream& operator<<(ostream& s,
                      const lti::affineLocation& loc);
}

#endif

