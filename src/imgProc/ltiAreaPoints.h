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


/**
 * \file   ltiAreaPoints.h
 *         Declares areaPoints
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiAreaPoints.h,v 1.4 2007-09-24 04:01:11 alvarado Exp $
 */

#ifndef _LTI_AREA_POINTS_H
#define _LTI_AREA_POINTS_H

#include "ltiIoHandler.h"
#include "ltiPointList.h"
#include "ltiBorderPoints.h"
#include "ltiIoPoints.h"
#include "ltiMatrix.h"
#include "ltiRectangle.h"

namespace lti {

  class ioPoints;
  class borderPoints;

  /**
   * List of all points within an area.
   *
   * areaPoints is a ipointList that contains all points that belong to
   * a specified area.
   *
   * For the explanation of the contour description in this class, see
   * following image:
   *
   * \code
   *   -- 00000000001111111111222222222233
   *   -- 01234567890123456789012345678901
   *   00 --------------------------------
   *   01 --------------------------------
   *   02 --------------------------------
   *   03 --------BBBB------BBBB----------
   *   04 -------B****BBB----B**B---------
   *   05 -------B*******B---B***B--------
   *   06 ------B*******B-----B*B---------
   *   07 -------B*******BBBBB**B---------
   *   08 ---------B*************B--------
   *   09 --------B**----*********B-------
   *   10 --------B**-----********B-------
   *   11 -------B**-----*******BB--------
   *   12 ------B**-----*******B----------
   *   13 ------B**-------******BB--------
   *   14 -----B**---------*******B-------
   *   15 -----B**--------*********B------
   *   16 ----B**-------**********BB------
   *   17 ---B***----*******----BB--------
   *   18 ----BBBBBBBBB***  --*B----------
   *   19 -------------BBBBBBBB-----------
   *   20 --------------------------------
   *   21 --------------------------------
   *   22 --------------------------------
   *   23 --------------------------------
   *
   *  "-" means background and the rest is part of the object.
   *  "B" indicates a borderpoint.
   *  \endcode
   *
   *  This contour class allows three representations of a contour:
   *  - ioPoints. It contains all input and output points for
   *    each line.  For example, for the previous image:
   *    (8,3)(11,3)(18,3)(21,3)(7,4)(14,4)(19,4)(22,4)...
   *    Note that every IO-point is a border point, but not all border
   *    points are IO-points.
   *  - borderPoints.  It contains a list of the points at the border.
   *    Beginning with the point at (8,3) the chain code for our example
   *    image is:
   *    (8,3)(9,3)(10,3)(11,3)(12,4)(13,4)(14,4)(15,5)(14,6)...
   *  - areaPoints.  It contains all points in the object.
   *
   *  @see lti::ioPoints, lti::borderPoints
   *
   *  @ingroup gAggregate
   *  @ingroup gShape
   */
  class areaPoints : public ipointList {

  public:
    /**
     * Default constructor. empty areaPoints
     */
    areaPoints();

    /**
     * Copy constructor
     */
    areaPoints(const areaPoints& other);

    /**
     * Destructor
     */
    virtual ~areaPoints();

    /**
     * Extracts areaPoints from borderPoints
     */
    areaPoints& castFrom(const borderPoints& theBorderPoints);

    /**
     * Extracts areaPoints from ioPoints
     */
    areaPoints& castFrom(const ioPoints& theIoPoints);

    /**
     * Extracts areaPoints from the given polygonPoints
     */
//     areaPoints& castFrom(const polygonPoints& thePolyPoints);

    /**
     * Copy \c other areaPoints into these.
     */
    areaPoints& copy(const areaPoints& other);

    /**
     * Assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    inline areaPoints& operator=(const areaPoints& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of the areaPoints
     *
     * @return a pointer to a copy of the areaPoints.
     */
    virtual areaPoints* clone() const;

    /**
     * Create a new instance of areaPoints.
     *
     * @return a pointer to a copy of the areaPoints
     */
    virtual areaPoints* newInstance() const;

    /**
     * Extracts areaPoints from the given mask.
     *
     * This function assumes that the mask contains JUST ONE connected
     * object.  To get the biggest object on the mask see
     * lti::objectsFromMask or lti::fastRelabeling.
     */
    bool extractFromMask(const matrix<ubyte>& mask);

    /**
     * Generate mask from the areaPoints.
     *
     * The dimensions of the resulting mask are the smallest rectangle to
     * contain the point (0,0) (optional) and all points in this list, plus
     * 1 pixel in both width and heigth.  If the given mask is bigger than
     * this size, its dimensions will be kept.
     *
     * If the boundary of the point list is not up to date, it can
     * be calculated if specified by the arguments:
     * @param mask The calculated mask will be stored here
     * @param computeBoundary If false, the internal boundary of the
     *    point list will be used, otherwise the boundary
     *    will be calculated (but NOT updated!!!).
     * @param exactBoundaryDimensions If true, the dimensions of the resulting
     *    mask will be the smallest rectangle to contain only the points in
     *    this list. The origin (0,0) may not be included, therefore a 1:1
     *    correspondence of coordinates will generally not be given.
     * @param keepMaskData if false, the mask will be initialized with 
     *                     zero before getting the area mask.  If true,
     *                     the previous mask data is not deleted, but
     *                     the mask will be resized if necessary.
     * @return true if successful, false otherwise.
     */
    bool generateMask(matrix<ubyte>& mask,
                      const bool computeBoundary=true,
                      const bool exactBoundaryDimensions=false,
                      const bool keepMaskData=false) const;

    /**
     * Compute intersection area with a rectangle
     */
    int intersectionArea(const irectangle& rect) const;
  };

}

#include "ltiAreaPoints_inline.h"

#endif



