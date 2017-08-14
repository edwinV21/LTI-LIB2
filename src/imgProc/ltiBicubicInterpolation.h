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
 * \file   ltiBicubicInterpolation.h
 *         This file contains the functor bicubicInterpolation.
 * \author Jens Rietzschel
 * \date   04.10.2001
 *
 * $Id: ltiBicubicInterpolation.h,v 1.3 2005-05-06 15:43:35 arndh Exp $
 */

#ifndef _LTI_BICUBIC_INTERPOLATION_H_
#define _LTI_BICUBIC_INTERPOLATION_H_

#include "ltiFixedGridInterpolation.h"


namespace lti {
  /**
   * This functor uses bicubic interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template<class T>
  class bicubicInterpolation : public fixedGridInterpolation<T> {
  public:

    /**
     * default constructor
     */
    bicubicInterpolation();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    bicubicInterpolation(const bicubicInterpolation& other);

    /**
     * destructor
     */
    virtual ~bicubicInterpolation();

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    virtual T apply(const vector<T>& src, const float& x) const;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    virtual T apply(const float& x) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param srcdest matrix<T> with the source and destination data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& srcdest,
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
    inline T apply(const matrix<T>& src, const fpoint& p) const {
      return apply(src,p.y,p.x);
    };


    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    inline T apply(const fpoint& p) const {
      return apply(p.y,p.x);
    };

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    virtual T apply(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the gradient of the matrix src
     * at the real valued position (row,col).
     * dest.x row-direction  dest.y column-direction
     * @param src matrix with the source data
     * @param row which row
     * @param col which column
     * @param dest gradient
     */
    bool getGradient(const matrix<T>& src, const float& row, const float& col,
                     point<T>& dest) const;

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bicubicInterpolation& copy(const bicubicInterpolation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bicubicInterpolation<T>& operator=(const bicubicInterpolation<T>& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual bicubicInterpolation<T>* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual bicubicInterpolation<T>* newInstance() const;

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

 private:

    /**
     * The grid square containing the desired point for interpolation
     * is considered here. The four points are numbered
     * counterclockwise starting at the low left corner.Within the
     * loop the function value, the gradients, and the cross
     * derivative are determined and stored in
     * y[4],y1[4],y2[4],y12[4].
     *
     * This is all the information needed from the given matrix to
     * compute the interpolation value(done in bicubicInterpolation ).
     */
    void centeredDifferencing(const float& row, const float& col,
                              float& t,float& u,
                              float y[4],float y1[4], float y2[4],
                              float y12[4], const matrix<T>& src) const;


    /**
     * like centeredDifferencing but used at the border of the matrix
     */
    void centeredDifferencingBorder(const float& row, const float& col,
                                    float& t, float& u,
                                    float y[4], float y1[4], float y2[4],
                                    float y12[4], const matrix<T>& src) const;


    /**
     * This method computes the interpolation value(only for
     * matrixes).  The value is returned in result, whereas gradient1 and
     * gradient2 give the interpolated gradients
     */
    void bicubic(float y[4],float y1[4],float y2[4],float y12[4],
                 const float& t, const float& u,
                 T& result,
                 float& gradient1,
                 float& gradient2) const;


    /**
     * This method computes the interpolation value for vectors using
     * the newton polynom of second degree.The values next to the
     * desired point and one to the left are used for the polynom
     */
    T newtonInterpolation(const int x[3], const T y[3],
                          const float& toinpol) const;

    /**
     * get bilinear interpolated value
     */
    void bilinearInterpolation(const matrix<T>& src,
                               const float& row, const float& col,
                               T& result)const;

    /**
     * extrapolation is used for interpolation beyond the borders. The
     * boundaryType is decisive for the result
     */
    void extrapolation(const float& row, const float& col,
                       const matrix<T>& src, T& result,
                       float& gradient1, float& gradient2) const;
  };

}

#endif

