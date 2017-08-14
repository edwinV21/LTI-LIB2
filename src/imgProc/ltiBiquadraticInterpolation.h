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
 * \file   ltiBiquadraticInterpolation.h
 *         The file contains the functor biquadraticInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiBiquadraticInterpolation.h,v 1.2 2005-05-06 15:43:35 arndh Exp $
 */

#ifndef _LTI_BIQUADRATIC_INTERPOLATION_H_
#define _LTI_BIQUADRATIC_INTERPOLATION_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiFixedGridInterpolation.h"

namespace lti {
  /**
   * This functor use biquadratic interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class biquadraticInterpolation : public fixedGridInterpolation<T> {
  public:

    typedef typename fixedGridInterpolation<T>::parameters parameters;

    /**
     * default constructor
     */
    biquadraticInterpolation();

    /**
     * Construct an interpolation with the given boundary type
     */
    biquadraticInterpolation(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    biquadraticInterpolation(const biquadraticInterpolation<T>& other);

    /**
     * destructor
     */
    virtual ~biquadraticInterpolation();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    T apply(const vector<T>& src, const float& x) const;

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
    T apply(const matrix<T>& src, const fpoint& p) const;

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
    T apply(const fpoint& p) const;

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
    T interpolate(const matrix<T>& src,
                  const float& row,
                  const float& col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * This method does not check if the given coordinates and the rest of
     * used points in the src matrix lie within the valid range.  This is
     * left to you.  Please consider that for the biquadratic interpolation
     * not only the point(trunc(col),trunc(row)) is used, but a 3x3 
     * neighborhood centered such that (col,row) is nearest to the center.
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
                              const float& row,
                              const float& col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const float& row,
                              const float& col) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    biquadraticInterpolation& copy(const biquadraticInterpolation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual biquadraticInterpolation* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual biquadraticInterpolation* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

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
     * Compute the biquadratic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fx fractional term between -0.5 and 0.5
     * @param y0 value for fx==-1.0 (note that is a value beyond the valid 
     *                               range of fx)
     * @param y1 value for fx==0.0
     * @param y2 value for fx==1.0  (note that is a value beyond the valid 
     *                               range of fx)
     *
     * @return interpolated value between the three values
     */
    inline T compute(const float& fx, const T& y0, 
                     const T& y1, const T& y2) const;
    
    /**
     * Compute the biquadratic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between -0.5 and 0.5
     * @param fx fractional term between -0.5 and 0.5
     * @param data array of 9 elements
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float& fy,
                     const float& fx,
                     const T *const  data) const;

    /**
     * Compute the biquadratic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between -0.5 and 0.5
     * @param fx fractional term between -0.5 and 0.5
     * @param y00 value for fx==-1 fy==-1
     * @param y01 value for fx==-1 fy== 0
     * @param y02 value for fx==-1 fy==+1
     * @param y10 value for fx== 0 fy==-1
     * @param y11 value for fx== 0 fy== 0
     * @param y12 value for fx== 0 fy==+1
     * @param y20 value for fx==+1 fy==-1
     * @param y21 value for fx==+1 fy== 0
     * @param y22 value for fx==+1 fy==+1
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float& fy,
                     const float& fx,
                     const T& y00, const T& y01, const T& y02,
                     const T& y10, const T& y11, const T& y12,
                     const T& y20, const T& y21, const T& y22) const;

  };

}

#include "ltiBiquadraticInterpolation_inline.h"

#endif

