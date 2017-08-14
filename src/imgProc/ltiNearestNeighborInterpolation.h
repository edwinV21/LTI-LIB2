/*
 * Copyright (C) 1998-2005
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
 * \file   ltiNearestNeighborInterpolation.h
 *         Contains the class lti::nearestNeighborInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiNearestNeighborInterpolation.h,v 1.6 2013-03-10 05:57:10 alvarado Exp $
 */


#ifndef _LTI_NEAREST_NEIGHBOR_INTERPOLATION_H_
#define _LTI_NEAREST_NEIGHBOR_INTERPOLATION_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiFixedGridInterpolation.h"
#include "ltiRound.h"

namespace lti {
  /**
   * This functor use nearestNeighbor interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class nearestNeighborInterpolation : public fixedGridInterpolation<T> {
  public:
    typedef typename fixedGridInterpolation<T>::parameters parameters;

    /**
     * default constructor
     */
    nearestNeighborInterpolation();

    /**
     * Construct an interpolator with the given boundary type.
     */
    nearestNeighborInterpolation(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    nearestNeighborInterpolation(const nearestNeighborInterpolation<T>& other);

    /**
     * destructor
     */
    virtual ~nearestNeighborInterpolation();


    /**
     * This method returns which pixel range around the interpolated postition
     * is considered by the interpolation functor.
     *
     * This is very useful for other functors to decide whether they should
     * call the interpolate() methods or the faster interpolateUnchk() methods.
     *
     * If the returned value is \f$a\f$ and the interpolated position is
     * \f$(x,y)\f$ all pixels with coordinates in 
     * \f$ [x-a .. x+a] \times [y-a .. y+a] \f$ 
     * may be considered by the interpolation functor.
     */
    int getRangeOfInfluence() const;

    /**
     * returns the name of this type ("nearestNeighborInterpolation")
     */
    virtual const std::string& name() const;

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    T apply(const vector<T>& src,const float& x) const;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    T apply(const float& x) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    T apply(const matrix<T>& src,
            const float& row,
            const float& col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position p.
     *
     * @param src matrix<T> with the source data.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    T apply(const matrix<T>& src,const point<float>& p) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    T apply(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     *
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    T apply(const point<float>& p) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).  This method is not virtual and can be used
     * if this interpolation type is used as template parameter in time
     * critical situations
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const matrix<T>& src,
                         const float row,
                         const float col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).  This method is not virtual and can be used
     * if this interpolation type is used as template parameter in time
     * critical situations.
     *
     * This method is used by classes which do not really require the
     * interpolation, but the boundary management (therefore the
     * integer type for the row and col parameters).
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const matrix<T>& src,
                         const int row,
                         const int col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * This method does not check if the given coordinates and the rest of
     * used points in the src matrix lie within the valid range.  This is
     * left to you.  
     *
     * This method is not virtual and can be used in time critical situations,
     * where you prefer to take the boundary control by yourself.
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const matrix<T>& src,
                              const float row,
                              const float col) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nearestNeighborInterpolation& copy(const nearestNeighborInterpolation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual nearestNeighborInterpolation<T>* clone() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual nearestNeighborInterpolation<T>* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };


}

#include "ltiNearestNeighborInterpolation_inline.h"

#endif

