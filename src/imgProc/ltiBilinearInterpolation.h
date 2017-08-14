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
 * \file   ltiBilinearInterpolation.h
 *         Contains the template class lti::bilinearInterpolation<T>, which is
 *         a functor, which uses bilinear interpolation to approximate values
 *         between the pixels or elements of vectors and matrices.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiBilinearInterpolation.h,v 1.3 2007-09-10 02:41:51 alvarado Exp $
 */

#ifndef _LTI_BILINEAR_INTERPOLATION_H_
#define _LTI_BILINEAR_INTERPOLATION_H_


#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiFixedGridInterpolation.h"

namespace lti {

  /**
   * This functor use bilinear interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   *
   * @ingroup gInterpolator
   */
  template<typename T>
  class bilinearInterpolation : public fixedGridInterpolation<T> {
  public:

    /**
     * The parameters for the class bilinearInterpolation
     */
    class parameters : public fixedGridInterpolation<T>::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Constructor parameters with given with boundary type
       */
      parameters(const eBoundaryType btype);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
    };

    /**
     * Default constructor
     */
    bilinearInterpolation();

    /**
     * Construct an interpolation with the given boundary type
     */
    bilinearInterpolation(const eBoundaryType boundaryType);

    /**
     * Construct a functor using the given parameters
     */
    bilinearInterpolation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    bilinearInterpolation(const bilinearInterpolation<T>& other);

    /**
     * Destructor
     */
    virtual ~bilinearInterpolation();

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
    virtual T apply(const vector<T>& src,const float& x) const;

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
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& src,
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
    virtual T apply(const matrix<T>& src,const point<float>& p) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    virtual T apply(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     *
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const point<float>& p) const;

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
                  const float row,
                  const float col) const;

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
    inline T interpolate(const float row,
                         const float col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * This method does not check if the given coordinates and the rest of
     * used points in the src matrix lie within the valid range.  This is
     * left to you.  Please consider that for the bilinear interpolation
     * not only the point(trunc(col),trunc(row)) is used, but also its three
     * "next" neighbors ((col,row+1),(col+1,row),(col+1,row+1)).
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
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const float row,
                              const float col) const;

    /**
     * Compute the bilinear interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between 0 and 1
     * @param fx fractional term between 0 and 1
     * @param syx value for fx==0 fy==0
     * @param syx1 value for fx==1 fy==0
     * @param sy1x value for fx==0 fy==1
     * @param sy1x1 value for fx==1 fy==1
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float fy, 
                     const float fx, 
                     const T syx, const T syx1,
                     const T sy1x, const T sy1x1) const;

    /**
     * Compute the linear interpolated value for the given coefficients
     * and values
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fx fractional term between 0 and 1
     * @param sx value for sx==0
     * @param sx1 value for sx==1
     *
     * @return interpolated value between the two extremes.
     */
    inline T compute(const float fx, 
                     const T sx, const T sx1) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearInterpolation<T>& copy(const bilinearInterpolation<T>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearInterpolation<T>& operator=(const bilinearInterpolation<T>& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bilinearInterpolation<T>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bilinearInterpolation<T>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * This method returns which pixel range around the interpolated postition
     * is considered by the interpolation functor.
     *
     * This is very useful for other functors to decide whether they should
     * call the interpolate() methods or the faster interpolateUnchk() methods.
     *
     * If the returned value is a and the interpolated position is (x,y) all
     * pixels with coordinates in \f$ [x-a .. x+a] \times [y-a .. y+a] \f$ 
     * may be considered by the interpolation functor.
     */
    int getRangeOfInfluence() const;

  };
}

#include "ltiBilinearInterpolation_inline.h"

#endif

