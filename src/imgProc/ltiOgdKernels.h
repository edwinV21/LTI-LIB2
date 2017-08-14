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
 * \file   ltiOgdKernels.h
 *         This file contains the declarations of first and second
 *         order Oriented Gaussian Derivative (OGD) kernels.
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * $Id: ltiOgdKernels.h,v 1.5 2005-04-28 09:42:07 doerfler Exp $
 */

#ifndef _LTI_OGDKERNELS_H_
#define _LTI_OGDKERNELS_H_

#include "ltiSeparableKernel.h"

namespace lti {

  /** 
   * First order Oriented Gaussian Derivative
   *
   * OGD kernels are separable (one filter pair per basis kernel)!
   *
   * You can create a "real" 2D kernel with the following code
   *
   * \code
   * lti::ogd1Kernel<float> ogd(5); // a kernel 5x5 with default variance
   *                                 // and angle
   * lti::kernel2D<float>   kern;    // a 2D kernel;
   *
   * kern.outerProduct(ogd.getRowFilter(0),ogd.getColFilter(0));
   *
   * \endcode
   *
   * The first basis filter of the first order OGD is:
   *
   * \f$ b_0(x,y) = -g(x,y) \frac{x}{\sigma^2}\f$
   *
   * and the second:
   *
   * \f$ b_1(x,y) = -g(x,y) \frac{y}{\sigma^2}\f$
   *
   * The interpolation functions are \f$ \cos\theta\f$ and
   * \f$ \sin\theta\f$.
   *
   * If you need to calculate several channels with different
   * directions, calculate first the basis images and then use
   * lti::ogd1Filter to combine them.  This method is faster than to
   * convolve each time the channel with the separable kernel.
   *
   * @ingroup gLinearFilters
   */
  template<class T>
  class ogd1Kernel : public separableKernel<T> {
  public:

    /**
     * There are two directions for base kernels for first order
     * derivatives: X and Y
     */
    enum eGradientDirection {
      X, /**< first base kernel: gradient in x direction,
            corresponding to an angle of 0*/
      Y  /**< second base kernel: gradient in y direction,
            corresponding to an angle of Pi/2*/
    };

    /** 
     * Default constructor
     *
     * Creates an empty kernel.  To initialize use generate() or
     * generateBasisKernel().
     */
    ogd1Kernel();

    /**
     * Copy constructor
     */
    ogd1Kernel(const ogd1Kernel& other);

    /**
     * Constructor.
     * 
     * Creates a base kernel (X or Y) with the given \a size and \a
     * variance. For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param gradientDirection specifies which base kernel to construct
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param variance variance of the kernel
     */
    ogd1Kernel(const eGradientDirection gradientDirection,
               const int size,
               const double& variance);

    /** 
     * Constructor that creates an oriented kernel.
     *
     * For a negative variance value the variance for the Gaussian is
     * calculated based on the kernel size. See lti::gaussKernel1D for
     * more information.
     *
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param variance variance of the kernel
     * @param angle used angle (in radians)
     */
    ogd1Kernel(const int size,
               const double& variance =  1.4426950409,
               const double& angle = 0.0);

    /**
     * returns name of this type
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual ogd1Kernel<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual ogd1Kernel<T>* newInstance() const;

    /** 
     * Initialize this kernel with the specified values.
     *
     * For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param size is the dimension of the one dimensional part (i.e. the
     *              kernel has dimensions size x size !)
     * @param variance variance of the kernel
     * @param angle used angle (in radians)
     */
    void generate(const int size,
                  const double& variance,
                  const double& angle);

    /** 
     * Initialize this kernel with the specified values
     *
     * For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param gradientDirection specifies which base kernel to generate
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param variance variance of the kernel
     */
    void generateBasisKernel(const eGradientDirection gradientDirection,
                             const int size,
                             const double& variance);

  };

  /**
   * Second order Oriented Gaussian Derivative
   *
   * OGD kernels are separable! (one filter pair per basis kernel)
   *
   * The first basis filter of the second order OGD is:
   *
   * \f$ b_0(x,y) = g(x,y) \frac{x^2 - \sigma^2}{\sigma^4}\f$
   *
   * and the second:
   *
   * \f$ b_1(x,y) = g(x,y) \frac{y^2 - \sigma^2}{\sigma^4}\f$
   *
   * and the third:
   *
   * \f$ b_2(x,y) = g(x,y) \frac{xy}{\sigma^4}\f$
   *
   * The used interpolation functions are \f$ \cos^2\theta\f$,
   * \f$ \sin^2\theta\f$ and \f$2\cos\theta \sin\theta\f$.
   *
   * @ingroup gLinearFilters
   */
  template<class T>
  class ogd2Kernel : public separableKernel<T> {
  public:

    /**
     * There are three directions for base kernels for second order
     * derivatives: XX, YY, and XY
     */
    enum eSecondDerivativeDirection {
      XX, /**< first base kernel: second derivative in xx direction,
             corresponding to an angle of 0*/
      YY, /**< second base kernel: second derivative in yy direction,
             corresponding to an angle of Pi/2*/
      XY /**< third base kernel: second derivative in xy direction,
             corresponding to an angle of Pi/4*/
    };

    /** 
     * Default constructor.
     *
     * Creates an empty kernel.  To initialize use generate() or
     * generateBasisKernel().
     */
    ogd2Kernel();

    /**
     * Copy constructor.
     */
    ogd2Kernel(const ogd2Kernel& other);

    /**
     * Constructor.
     * 
     * Creates a base kernel (XX, YY, or XY) with the given \a size
     * and \a variance. See eSecondDerivativeDirection for further docu.
     *
     * For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param secondDerivativeDirection specifies which base kernel to
     *        construct
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param variance variance of the kernel
     */
    ogd2Kernel(const eSecondDerivativeDirection secondDerivativeDirection,
               const int size = 3,
               const double& variance = 1.4426950409);

    /**
     * Constructor creating an oriented filter.
     *
     * For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param variance variance of the kernel
     * @param angle used angle (in radians)
     */
    ogd2Kernel(const int size,
               const double& variance =  1.4426950409,
               const double& angle = 0.0);

    /**
     * returns name of this type
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual ogd2Kernel<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual ogd2Kernel<T>* newInstance() const;

    /**
     * Initialize this kernel with the specified values
     *
     * For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param variance variance of the kernel
     * @param angle used angle (in radians)
     */
    void generate(const int size,
                  const double& variance,
                  const double& angle);

    /*
     * Initialize this kernel with the specified values.
     *
     * For a negative variance value the variance for the
     * Gaussian is calculated based on the kernel size. See
     * lti::gaussKernel1D for more information.
     *
     * @param secondDerivativeDirection specifies which base kernel to
     *        construct
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !
     * @param variance variance of the kernel
     */
    void generateBasisKernel(const eSecondDerivativeDirection secondDerivativeDirection,
                             const int size,
                             const double& variance);

  };

}

#include "ltiOgdKernels_template.h"

#endif

