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
 * \file   ltiGaussKernels.h
 *         Contains the declarations for 1D and 2D Gauss kernels.
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * $Id: ltiGaussKernels.h,v 1.4 2005-05-20 15:42:33 doerfler Exp $
 */

#ifndef _LTI_GAUSSKERNELS_H_
#define _LTI_GAUSSKERNELS_H_

#include "ltiKernel1D.h"
#include "ltiSeparableKernel.h"

namespace lti {
  /**
   *  This class creates a one-dimensional gaussian filter kernel.
   *
   *  The area under the filter is normalized to one.
   *
   *  The one dimesional kernel is calculated with following equation:
   *
   *  \f$ g(x) = \frac{1}{\sigma \sqrt{2\pi}} \exp{-\frac{x^2}{2\sigma^2}}\f$
   *
   *  Example:
   *
   *  \code
   *  // the vector to be filtered:
   *  lti::vector<channel::value_type> data;
   *
   *  // ... initialize vector here ...
   *
   *  // gaussian filter kernel with 3 elements, and a variance of 0.72
   *  lti::gaussKernel1D<channel::value_type> kernel(3,0.72);
   *
   *  lti::convolution filter;                        // convolution operator
   *  lti::convolution::parameters param;             // parameters
   *  param.setKernel(kernel);                        // use the gauss kernel
   *  filter.setParameters(param);                    // set parameters
   *
   *  // filter the vector and leave the result there too
   *
   *  filter.apply(data);
   *  \endcode
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class gaussKernel1D : public kernel1D<T> {
  public:
    /**
     * constructor
     * @param size size of the kernel in one dimension
     * @param variance variance of the kernel.  If this argument is
     *        ommited or a negative value is given, the variance will
     *        be calculated such that the value at the index floor(size/2)
     *        is 1/(1+floor(size/2)) times the value at index 0.
     *        For example, for size==3, the value at 1 will be 1/2 the value
     *        at 0.
     *        Hence a 3 taps kernel will contain the elements (1/4,1/2,1/4).
     */
    gaussKernel1D(const int size = 3,
                  const double& variance = -1.0);

    /**
     * Constructs a Gaussian kernel with the given \a variance and a
     * size so that the outmost elements of the kernel are greater
     * equal \a maxError, which defaults to 1E-2. 
     *
     * E.g. for variance=0.7 this results in a kernel of size 7.
     *
     * @param variance variance of the kernel.
     * @param maxError outmost element of kernel is greater than this
     */
    gaussKernel1D(const double& variance,
                  const double& maxError = 1.E-2);

    /**
     * returns name of this type
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual gaussKernel1D<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual gaussKernel1D<T>* newInstance() const;

    /** initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     * @param variance variance of the kernel. If this argument is
     *        ommited or a negative value is given, the variance will
     *        be calculated such that the value at the index floor(size/2)
     *        is 1/(1+floor(size/2)) times the value at index 0.
     *        For example, for size==3, the value at 1 will be 1/2 the value
     *        at 0.
     *        Hence a 3 taps kernel will contain the elements (1/4,1/2,1/4).
     */
    void generate(const int size,
                  const double& variance=-1.0);

    /**
     * Generates a Gaussian kernel with the given \a variance and the
     * maximal size so that the outmost elements of the kernel are
     * greater equal \a maxError, which defaults to 1E-2.
     *
     * @param variance variance of the kernel.
     * @param maxError outmost element of kernel is greater than this
     */
    void generate(const double& variance, 
                  const double& maxError=1E-2);

    /**
     * Returns the variance actually used for building the
     * kernel. This value is computed automatically if a negative
     * value is supplied in the constructor or generate().
     *
     * @return variance used to create the kernel
     */
    const double& getActualVariance() const;

  protected:
    /**
     * This attribute is only needed to store an automatically set
     * variance for negative variance given by the user. It can be
     * retrieved with getActualVariance()
     */
    double variance;
  };

  /** 
   * Two-dimensional Gaussian filter kernel
   *
   * Gaussian kernels are separable, and will be created this way!
   * (@see  gaussKernel1D)
   *
   * You can create a "real" 2D kernel with the following code
   *
   * \code
   * lti::gaussKernel2D<float> gauss(5); // a kernel 5x5 with default variance
   * lti::kernel2D<float>      kern;     // a 2D kernel;
   * 
   * kern.castFrom(gauss);
   * \endcode
   *
   * but note that the convolution of this kernel with a channel is less
   * efficient than convolving its separable version.
   *
   * To convolve this filter with a channel follow the next example
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data;
   *
   * // ... initialize channel here ...
   *
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::channel::value_type> kernel(5,1.3);
   *
   * lti::convolution filter;                        // convolution operator
   * lti::convolution::parameters param;             // parameters
   * param.setKernel(kernel);                        // use the gauss kernel
   * filter.setParameters(param);                    // set parameters
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   * 
   * \endcode
   *
   * You can also use following shortcut, if you can use the default
   * boundary type for the convolution:
   *
   * \code
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::channel::value_type> kernel(5,1.3);
   *
   * lti::convolution filter(kernel);                // convolution operator
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   *
   * \endcode
   *
   * @ingroup gLinearFilters
   */
  template<class T>
  class gaussKernel2D : public separableKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     * @param variance variance of the kernel.  If negative, a default
     *             value from the given size will be computed 
     *             (see lti::gaussKernel1D<T>)
     */
    gaussKernel2D(const int size = 3,
                  const double& variance =  1.4426950409);

    /**
     * Generates a Gaussian kernel with the given \a variance and the
     * maximal size so that the outmost elements of the kernel are
     * greater equal \a maxError, which defaults to 1E-2.
     *
     * E.g. for variance=0.7 this results in a kernel of size 5.
     *
     * @param variance variance of the kernel.
     * @param maxError outmost element of kernel is greater than this
     */
    gaussKernel2D(const double& variance,
                  const double& maxError = 1.E-2);

    /**
     * returns name of this type
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual gaussKernel2D<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual gaussKernel2D<T>* newInstance() const;

    /** 
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     * @param variance variance of the kernel.  If negative, a default
     *                    value from the given size will be computed 
     *                    (see lti::gaussKernel1D<T>)
     */
    void generate(const int size,
                  const double& variance);

    /**
     * Constructs a Gaussian kernel with the given \a variance and the
     * maximal size so that the outmost elements of the kernel are
     * greater equal \a maxError, which defaults to 1E-2.
     *
     * @param variance variance of the kernel.
     * @param maxError outmost element of kernel is greater than this
     */
    void generate(const double& variance, 
                  const double& maxError=1E-2);

    /**
     * Returns the variance actually used for building the
     * kernel. This value is computed automatically if a negative
     * value is supplied in the constructor or generate().
     *
     * @return variance used to create the kernel
     */
    const double& getActualVariance() const;

  protected:
    /**
     * This attribute is only needed to store an automatically set
     * variance for negative variance given by the user. It can be
     * retrieved with getActualVariance()
     */
    double variance;
  };
}

#include "ltiGaussKernels_template.h"

#endif

