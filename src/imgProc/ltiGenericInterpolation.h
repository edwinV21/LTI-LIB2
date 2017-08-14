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
 * \file   ltiGenericInterpolation.h
 *         Contains the functor genericInterpolation which uses LUT
 *         for interpolation kernels
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiGenericInterpolation.h,v 1.5 2007-11-17 20:03:13 alvarado Exp $
 */

#ifndef _LTI_GENERIC_INTERPOLATION_H_
#define _LTI_GENERIC_INTERPOLATION_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiFixedGridInterpolation.h"

namespace lti {

  /**
   * This functor uses a generic interpolation concept based on look-up-tables 
   * for the interpolation kernels.
   *
   * The LUTs allow to considerably accelerate the computation times, but a
   * little bit precision is lost due to the unavoidable quantization present
   * in the LUT.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class genericInterpolation : public fixedGridInterpolation<T> {
  public:

    /**
     * Types for interpolation kernels.
     */
    enum eInterpolationKernelType {
      BilinearKernel, /**< Bilinear interpolation kernel.
                       * The neighborhood size is always 2x2.
                       * This implementation
                       * is slower than the optimized version in the functor
                       * lti::bilinearInterpolation, but is provided for
                       * completeness.
                       */
      BicubicKernel,  /**< Bicubic interpolation.
                       * The neighborhood size is always 4x4.
                       * The kernel provided here is after Sonka et.al. 
                       * pp. 67
                       *
                       * \f[ h(x)=\begin{cases}
                       * 1-2|x|^2+|x|^3 & \text{for $0\leq|x|\leq 1$} \\
                       * 4-8|x|+5|x|^2-|x|^3 & \text{for $1\leq|x|\leq 2$}\\
                       * 0 & \text{otherwise}
                       * \end{cases} \f]
                       */
      GenericKernel   /**< Generic interpolation.
                       * The generic interpolation uses the kernelLUT given
                       * in the parameters.
                       */
    };

    /**
     * The parameters for the class genericInterpolation
     */
    class parameters : public fixedGridInterpolation<T>::parameters {
    public:

      /**
       * default constructor
       */
      parameters();

      /**
       * Constructor to set explicitly the desired boundaryType
       */
      parameters(const eBoundaryType btype);

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);
      
      /**
       * destructor
       */
      ~parameters();

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler, const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Interpolation type to be used.
       *
       * Default: Bicubic
       */
      eInterpolationKernelType kernelType;

      /**
       * Number of samples in the interpolation table for a unit interval.
       * 
       * The total number of elements in the LUT will be this value multiplied
       * by the number of unit intervals of the interpolation kernel.
       *
       * Default value: 256
       */
      int numSamplesPerInterval;

      /**
       * Generic interpolation kernel.
       * 
       * If the kernelType is GenericKernel then this kernel LUT will be used.
       * To be a valid kernel, this vector must have a size \a n times
       * numSamplesPerInterval, with \a n even.  If the validity of the
       * kernel LUT cannot be confirmed, a Bilinear Kernel will be used
       * instead, but setParameters will return false.
       *
       * Default value: empty kernel
       */
      fvector kernelLUT;
    };

    /**
     * default constructor
     */
    genericInterpolation();

    /**
     * Construct an interpolator with the given parameters
     */
    genericInterpolation(const parameters& params);

    /**
     * Construct an interpolator with the given boundary type
     */
    genericInterpolation(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    genericInterpolation(const genericInterpolation<T>& other);

    /**
     * destructor
     */
    virtual ~genericInterpolation();

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
    T apply(const matrix<T>& src,const fpoint& p) const;

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
    inline T interpolate(const float row, const float col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * This method does not check if the given coordinates and the rest of
     * used points in the src matrix lie within the valid range.  This is
     * left to you.  Please consider that for the generic interpolation
     * not only the point(trunc(col),trunc(row)) is used, but a nxn 
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
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    genericInterpolation& copy(const genericInterpolation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    genericInterpolation<T>& operator=(const genericInterpolation<T>& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual genericInterpolation* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual genericInterpolation* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * returns used parameters
     */
    virtual bool updateParameters();


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
     * Return a read only reference to the last computed LUT.
     *
     * This method is mainly used for debugging purposes.
     */
    inline const vector<float>& getLUT() const;

    /**
     * Compute the generic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fx fractional term between 0.0f (inclusive) and 1.0f (exclusive)
     * @param data pointer to an array with the support samples.  It has at
     *             least \a n elements, with \a n the number of unit intervals
     *             in the interpolation kernel.
     *             Their position will be assumed to be from 
     *             -(n/2-1) to n/2, so that the
     *             interval 0..1 lies exaclty in the middle.
     *
     * @return interpolated value between the three values
     */
    inline T compute(const float fx,const T* data) const;
    
    /**
     * Compute the generic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between 0.0 and 1.0
     * @param fx fractional term between 0.0 and 1.0
     * @param data pointer to an array of arrays with the support samples.
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float fy,
                     const float fx,
                     const T** data) const;

  protected:
    /** 
     * range of influence
     */
    int rangeOfInfluence_;

    /**
     * The interpolation kernel is traversed in revers order than
     * the data.  To save the first computation time, we store the
     * sample index for the first element of the last interval
     */
    int lastIntervalFirstSample_;

    /**
     * Number of intervals really used.
     *
     * The value given in the parameters might be ignored depending on the
     * chosen kernel type.
     */
    int numberOfIntervals_;

    /**
     * This is the number that need to be substracted  to a position point
     * to get the first valid support sample.
     * It is usually numberOfIntervals/2 - 1;
     */
    int firstSampleOffset_;
     
    /**
     * Interval size taken from the parameters numSamplesPerInterval;
     */
    int intervalSize_;

    /**
     * @name Buffers
     */
    /**
     * For the two dimensional interpolation, this vector will administrate
     * the memory array required.
     */
    vector<T> column_;

    /**
     * This pointer will always point to the first element of column
     */
    T* firstElemColumn_;

    /**
     * This is a small window that can be used to save temporarily the
     * sample supporting points
     */
    mutable matrix<T> buffer_;

    /**
     * This pointer to arrays is initialized to have the proper size, but
     * you have to set the second pointers by yourself
     */
    mutable const T** rows_;

    /**
     * This is initialized to point a the buffer's rows.
     */
    const T** bufferRows_;

    //@}

    /**
     * @name Kernel initialization routines
     */
    //@{
    /**
     * Interpolation kernel.
     *
     * This is initialized by the initLUT
     */
    vector<float> interpolationKernel_;

    /**
     * Initialize the interpolation kernel LUT based on the parameters
     * settings.
     *
     * The LUT has only positive entries, but represents the interval from
     * -halfSize+1 to halfSize.  No use of the symmetrical properties of the
     * kernel are exploited to improve efficiency, since less comparisons are
     * required.
     */
    bool initLUT();

    /**
     * Initialization of the bilinear kernel.
     * Two unit intervals wlll be initialized.
     */
    bool initBilinearLUT();

    /**
     * Initialization of the bicubic kernel.
     * Four unit intervals wlll be initialized.
     * The kernel provided here is after Sonka et.al. page 67:
     * \code
     *           /     
     *          |  1-2|x|^2 + |x|^3           for 0<=x<=1
     *  h(x) = <   4-8|x|^2 + 5|x|^2 - |x|^3  for 1<x<=2
     *          |  0                          otherwise 
     *           \
     * \endcode
     */
    bool initBicubicLUT();

    /**
     * Generic kernel initialization.
     * 
     * This method checks that the kernelLUT in the parameters is a valid
     * kernel and sets all required attributes.
     */
    bool initGenericLUT();

    //@}
  };
}

#include "ltiGenericInterpolation_inline.h"

#endif

