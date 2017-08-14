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
 * \file   ltiConvolutionHelper.h
 *         Contains a helper class for convolution.
 * \author Pablo Alvarado
 * \date   19.07.2000
 *
 * $Id: ltiConvolutionHelper.h,v 1.9 2010-04-25 05:30:27 alvarado Exp $
 */
#ifndef _LTI_CONVOLUTION_HELPER_H_
#define _LTI_CONVOLUTION_HELPER_H_

#include "ltiObject.h"
#include "ltiMath.h"
#include "ltiImage.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiKernelSymmetry.h"
#include "ltiBoundaryType.h"
#include "ltiInvalidParametersException.h"
#include "ltiPointList.h"
#include "ltiAreaPoints.h"
#include "ltiInvalidParametersException.h"

namespace lti {
  /**
   * Pre-convolution algorithm for a vector * kernel1D
   * The type T is the type of the elements of the vector and kernel1D
   * The class A determines how the values should be accumulated.
   * The class A must have following  methods:
   *
   *  - accumulate(T kernelElement,T source)
   *    accumulate the result of applying some function to one element
   *    of the kernel and one element of the source channel.
   *  - accumulateSym(T kernelElement,T source1, T source2)
   *    accumulate the result of applying some function to one element of a
   *    symmetric kernel and the corresponding elements of the source
   *    channel.
   *  - accumulateASym(T kernelElement,T source1, T source2)
   *    accumulate the result of applying some function to one element of a
   *    anti-symmetric kernel (e.g. a gradient) and the corresponding elements
   *    of the source channel.
   *  - accumulateZero(T source) 
   *    accumulate the result of applying some function with kernel value
   *    zero to the source channel. For efficient implementation of 
   *    accumulateASym() where middle kernel value is zero by definition.
   *    For filters this is an empty function, however, not for others.
   *  - getResult()
   *    returns the accumulated value
   *  - reset()
   *    reset the accumulated value
   *  - setNorm()
   *    set the normalization constant used by the getResult() method
   *
   * Examples of the accumulator class can be found in:
   * lti::dilation::accumulatorGray, lti::dilation::accumulatorBin,
   * lti::erosion::accumulatorGray, lti::erosion::accumulatorBin,
   * lti::convolution::accumulator
   *
   * The implementation of these methods allows the efficient use
   * of this class for the implementation of convolution, dilation and
   * erosion.
   *
   * Please note that this class is intended to be used just as a helper class.
   * The correctness of the parameters and input data must be ensured by the
   * "helped" classes before these methods are called.
   *
   */
  template <class T,class A>
  class convHelper1D {
  public:
    /**
     * Default constructor
     */
    convHelper1D();

    /**
     * Destructor
     */
    ~convHelper1D();

    /**
     * Try to get a Kernel from the parameters.
     * If it is not possible to get it, return false;
     * This will not copy the kernel, it just will keep a reference to it!
     * @param kern the container with the kernel.
     * @param mirrored force the kernel to be mirrored before setting it
     * @return true if kernel could be set and false otherwise
     */
    bool setKernel(const container* kern, const bool mirrored=false);

    /**
     * Convolve the vector src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src vector to be filtered
     * @param dest destination vector for the result
     * @param boundaryType boundary type
     */
    void apply(const vector<T>& src,
                     vector<T>& dest,
               const eBoundaryType& boundaryType);

    /**
     * Convolve the columns of matrix src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param boundaryType boundary type
     */
    void applyCol(const matrix<T>& src,
                        matrix<T>& dest,
                  const eBoundaryType& boundaryType);

    /**
     * Convolve the rows of matrix src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param boundaryType boundary type
     */
    void applyRow(const matrix<T>& src,
                        matrix<T>& dest,
                  const eBoundaryType& boundaryType);

    /**
     * Returns a reference to the accumulator object being used
     */
    A& getAccumulator();

    /**
     * Returns a reference to the accumulator object being used
     */
    const A& getAccumulator() const;

  protected:
    /**
     * Pointer to a kernel casted to the proper type.
     * 
     * It is a pointer to an external copy or the tmpKernel used
     * as temporal holder 
     */
    const kernel1D<T>* kernel_;

    /**
     * Cast kernel.
     * 
     * Returns false if the given container cannot be casted to a valid
     * kernel1D<T>.
     */
    template<typename U>
    bool cast(const container* other,bool mirrored);

    /**
     * Convolve any kernel1D to vector
     */
    void applyAny(const vector<T>& src,
                  vector<T>& dest,
                  const eBoundaryType& boundaryType);
    
    /**
     * Convolve symmetric or asymmetric kernel1D to vector
     */
    void applySym(const vector<T>& src,
                        vector<T>& dest,
                        const eBoundaryType& boundaryType);

    /**
     * Boundary conditions for kernels larger than the input vector
     */
    void applyHuge(const vector<T>& src,
                   vector<T>& dest,
                   const eBoundaryType& boundaryType);


    /**
     * Convolve the columns of matrix src with any filter kernel and
     * leave the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param boundaryType boundary type
     */
    void applyAnyCol(const matrix<T>& src,
                           matrix<T>& dest,
                     const eBoundaryType& boundaryType);

    /**
     * Convolve the columns of matrix src with a symmetric or asymmetric filter
     * kernel and leave the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param boundaryType boundary type
     */
    void applySymCol(const matrix<T>& src,
                           matrix<T>& dest,
                     const eBoundaryType& boundaryType);

    /**
     * Convolve the columns of matrix src with a filter kernel larger than
     * the number of rows and leave the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param boundaryType boundary type
     */
    void applyHugeCol(const matrix<T>& src,
                           matrix<T>& dest,
                     const eBoundaryType& boundaryType);

    /**
     * Accumulator used
     */
    A lresult_;

    /**
     * Estimated kernel symmetry.  This attribute is set in setKernel()
     */
    eKernelSymmetry kernelSymmetry_;
    
  private:

    /**
     * Temporal kernel used to hold a casted copy of the given kernel.
     * 
     * This helds valid data only if a cast was required.  Otherwise,
     * an externa kernel is employed.
     */
    kernel1D<T>* tmpKernel_;
  };

  /**
   * Pre-convolution algorithm for a matrix * kernel2D
   * The type T is the type of the elements of the vector an kernel1D
   * The class A determines how the values should be accumulated.
   * The class A must have following methods:
   *
   *  - accumulate(T kernelElement,T source)
   *    accumulate the result of applying some function to one element
   *    of the kernel and one element of the source channel.
   *  - getResult()
   *    returns the accumulated value
   *  - reset()
   *    reset the accumulated value
   *  - setNorm()
   *    set the normalization constant used by the getResult() method
   *
   * Examples of the accumulator class can be found in:
   * lti::dilation::accumulatorGray, lti::dilation::accumulatorBin,
   * lti::erosion::accumulatorGray, lti::erosion::accumulatorBin,
   * lti::convolution::accumulator
   *
   * The implementation of these methods allows the efficient use
   * of this class for the implementation of convolution, dilation and
   * erosion.
   *
   * Please note that this class is intended to be used just as a helper class.
   * The correctness of the parameters and input data must be ensured by the
   * "helped" classes before these methods are called.
   *
   */
  template <class T,class A>
  class convHelper2D  {
  public:
    /**
     * Default constructor
     */
    convHelper2D();

    /**
     * Destructor
     */
    ~convHelper2D();

    /**
     * Try to get a Kernel from the parameters.
     * If it is not possible to get it, return false;
     * This will not copy the kernel, it just will keep a reference to it!
     * @param kern the container with the kernel.
     * @param mirrored force the kernel to be mirrored before setting it
     * @return true if kernel could be set and false otherwise
     */
    bool setKernel(const container* kern, const bool mirrored=false);
  
    /**
     * Get kernel type
     */
    inline bool isKernelSeparable() const;

    /**
     * Convolve the matrix src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest result
     * @param boundaryType boundary type
     */
    void apply(const matrix<T>& src,
               matrix<T>& dest,
               const eBoundaryType& boundaryType);

    /**
     * Convolve the matrix src with the separable filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest result
     * @param boundaryType boundary type
     */
    void applySep(const matrix<T>& src,
                  matrix<T>& dest,
                  const eBoundaryType& boundaryType);

    /*
     * Convolve the matrix src with the object depicted in the filter
     * kernel and described by the mask. Leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest result
     * @param boundaryType boundary type
     */
    void applyMask(const matrix<T>& src,
                   matrix<T>& dest,
                   const eBoundaryType& boundaryType);

    /**
     * Set the mask to be used with the kernel.
     */
    void setMask(const matrix<ubyte>& mask);

    /**
     * Returns a pointer to the current mask.
     */
    const matrix<ubyte>* getMask() const;

    /**
     * Returns a reference to the accumulator object being used
     */
    A& getAccumulator();

    /**
     * Returns a reference to the accumulator object being used
     */
    const A& getAccumulator() const;

  protected:

    /** 
     * Just a pointer to the kernel being used, which can be extern or to
     * tmKernel_
     */
    const kernel2D<T>* kernel_;

    /**
     * Just a pointer to the kernel being used, which can be extern or to
     * tmSKernel_
     */
    const separableKernel<T>* sKernel_;

    /**
     * Points to the mask used to convolve only specified (or masked) parts of
     * a channel
     */
    const matrix<ubyte>* mask_;

    /**
     * Accumulator
     */
    A lresult_;

  private:
    /**
     * Temporal object that holds casted or mirrored kernels
     */
    kernel2D<T>* tmpKernel_;

    /**
     * Temporal object that holds casted or mirrored separable kernels
     */
    separableKernel<T>* tmpSKernel_;
    
    /**
     * Cast kernel2D.
     * 
     * Returns false if the given container cannot be casted to a valid
     * kernel2D<T>.
     */
    template<typename U>
    bool cast2(const container* other,bool mirrored);

    /**
     * Cast separableKernel.
     * 
     * Returns false if the given container cannot be casted to a valid
     * separableKernel<T>.
     */
    template<typename U>
    bool castS(const container* other,bool mirrored);

  };
}

#include "ltiConvolutionHelper_template.h"

#endif

