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
 * \file  ltiGradientKernels.h
 *        This file contains the declarations of linear kernels that can be
 *        used for calculating the gradient of a channel or other matrix type
 * \author Pablo Alvarado
 * \date 11.07.2000
 *
 * $Id: ltiGradientKernels.h,v 1.5 2007-10-24 03:37:55 alvarado Exp $
 */

#ifndef _LTI_GRADIENTKERNELS_H_
#define _LTI_GRADIENTKERNELS_H_

#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include <limits>

namespace lti {
  /**
   * Two-dimensional separable filter kernels for the gradient approximation
   *
   * This class will create a two-dimensional gradient filter kernel.
   *
   * There are three posibilities for the kernel dimensions 3x3, 4x4 or 5x5.
   * The approximation is done using the suggestions of Ando in his paper
   * "Consistent Gradient Operators" (IEEE PAMI, Vol. 22, No. 3, March 2000)
   *
   * These suggested kernels are separable and are of course implemented that
   * way (much faster!)
   *
   * This object will correspond to the derivative respect the x-Axis.
   * For the y-Axis just exchange the 1D filter kernels, or use the
   * lti::andoKernelY.
   *
   * The template parameter T should be the same as the value_type of
   * the matrix or channel to be filtered (channel::value_type, double, etc.)
   *
   * It is recomended to use this kernels with floating point matrices
   * or channels, due to precision aspects.
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class andoKernelX : public separableKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             The valid values are 3, 4 or 5.  If an invalid value is
     *             given, an empty kernel will be created.
     */
    andoKernelX(const int size = 3);

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    andoKernelX(const andoKernelX& other);

    /**
     * returns name of this type ("andoKernelX")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual andoKernelX<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual andoKernelX<T>* newInstance() const;

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     */
    void generate(const int size);
  };

  /**
   * Two-dimensional separable filter kernels for the gradient approximation
   *
   * This class will create a two-dimensional gradientian filter kernel.
   *
   * There are three posibilities for the kernel dimensions 3x3, 4x4 or 5x5.
   * The approximation is done using the suggestions of Ando in its paper
   * "Consistent Gradient Operators" (IEEE PAMI, Vol. 22, No. 3, March 2000)
   *
   * These suggested kernels are separable and are of course implemented that
   * way (much faster!)
   *
   * This object will correspond to the derivative respect the y-Axis.
   * For the x-Axis just exchange the 1D filter kernels, or use the
   * lti::andoKernelX .
   *
   * The template parameter T should be the same as the value_type of
   * the matrix or channel to be filtered (channel::value_type, double, etc.)
   *
   * It is recomended to use this kernels with floating point matrices
   * or channels, due to precision aspects.
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class andoKernelY : public separableKernel<T> {
  public:
    /**
     * constructor
     *
     * @param size is the dimension of the one dimensional part
     *             (i.e. the filter kern is a size x size kernel!)  The valid
     *             values are 3, 4 or 5.  If an invalid value is given, an
     *             empty kernel will be created.
     */
    andoKernelY(const int size = 3);

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    andoKernelY(const andoKernelY& other);

    /**
     * returns name of this type ("andoKernelY")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual andoKernelY<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual andoKernelY<T>* newInstance() const;

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension.
     */
    void generate(const int size);
  };

  /**
   * Sobel Kernel X
   *
   * Define as a 3x3 separable kernel:
   *
   * \code
   * -1  0  1
   * -2  0  2
   * -1  0  1
   * \endcode
   *
   * For the y direction see sobelKernelY
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class sobelKernelX : public separableKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/4.
     */
    sobelKernelX(const bool normalized = false);

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    sobelKernelX(const sobelKernelX& other);

    /**
     * returns name of this type ("sobelKernelX")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual sobelKernelX<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual sobelKernelX<T>* newInstance() const;
  };

  /**
   * Sobel Kernel Y
   *
   * Defined as a 3x3 separable kernel:
   *
   * \code
   * -1 -2 -1
   *  0  0  0
   *  1  2  1
   * \endcode
   *
   * For the x direction see sobelKernelX
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class sobelKernelY : public separableKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/4.
     */
    sobelKernelY(const bool normalized = false);

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    sobelKernelY(const sobelKernelY& other);

    /**
     * returns name of this type ("sobelKernelY")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual sobelKernelY<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual sobelKernelY<T>* newInstance() const;
  };

  /**
   * Prewitt Kernel X
   *
   * Define as a 3x3 separable kernel:
   *
   * \code
   * -1  0  1
   * -1  0  1
   * -1  0  1
   * \endcode
   *
   * For the y direction see prewittKernelY
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class prewittKernelX : public separableKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/3.
     */
    prewittKernelX(const bool normalized = false);

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    prewittKernelX(const prewittKernelX& other);

    /**
     * returns name of this type ("prewittKernelX")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual prewittKernelX<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual prewittKernelX<T>* newInstance() const;
  };

  /**
   * Prewitt Kernel Y
   *
   * Defined as a 3x3 separable kernel:
   *
   * \code
   * -1 -1 -1
   *  0  0  0
   *  1  1  1
   * \endcode
   *
   * For the x direction see prewittKernelX
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class prewittKernelY : public separableKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/3.
     */
    prewittKernelY(const bool normalized = false);

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    prewittKernelY(const prewittKernelY& other);

    /**
     * returns name of this type ("prewittKernelY")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual prewittKernelY<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual prewittKernelY<T>* newInstance() const;
  };

  /**
   * Harris Kernel X
   *
   * This is the kernel used for the famed Harris Corner Detector
   * introduced in:
   *
   * C. Harris and M. Stephens, "A Combined Corner and Edge Detector",
   * in Proc. 4th Alvey Vision Conference, pp. 147-151, 1988
   *
   * Define as a 1x5 separable kernel:
   *
   * \code
   * -2 -1  0  1 2
   * \endcode
   *
   * For the y direction see harrisKernelY
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class harrisKernelX : public separableKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other gradient kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelX();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    harrisKernelX(const harrisKernelX& other);

    /**
     * returns name of this type ("harrisKernelX")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual harrisKernelX<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual harrisKernelX<T>* newInstance() const;
  };

  /**
   * Harris Kernel Y
   *
   * This is the kernel used for the famed Harris Corner Detector
   * introduced in:
   *
   * C. Harris and M. Stephens, "A Combined Corner and Edge Detector",
   * in Proc. 4th Alvey Vision Conference, pp. 147-151, 1988
   *
   * Define as a 5x1 separable kernel:
   *
   * \code
   * [-2 -1  0  1 2] transposed
   * \endcode
   *
   * For the x direction see harrisKernelX
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class harrisKernelY : public separableKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other gradient kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelY();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    harrisKernelY(const harrisKernelY& other);

    /**
     * returns name of this type ("harrisKernelY")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual harrisKernelY<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual harrisKernelY<T>* newInstance() const;
  };

  /**
   * Robinson Kernel X
   *
   * Define as a 3x3 2D (non-separable) kernel:
   *
   * \code
   * -1   1  1
   * -1  -2  1
   * -1   1  1
   * \endcode
   *
   * For the y direction see robinsonKernelY
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class robinsonKernelX : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelX();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    robinsonKernelX(const robinsonKernelX& other);

    /**
     * returns name of this type ("robinsonKernelX")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual robinsonKernelX<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual robinsonKernelX<T>* newInstance() const;
  };

  /**
   * Robinson Kernel Y
   *
   * Defined as a 3x3 2D (non separable) kernel:
   *
   * \code
   * -1 -1 -1
   *  1 -2  1
   *  1  1  1
   * \endcode
   *
   * For the x direction see robinsonKernelX
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class robinsonKernelY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelY();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    robinsonKernelY(const robinsonKernelY& other);

    /**
     * returns name of this type ("robinsonKernelY")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual robinsonKernelY<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual robinsonKernelY<T>* newInstance() const;
  };

  /**
   * Kirsch Kernel X
   *
   * Defined as a 3x3 2D (non-separable) kernel:
   *
   * \code
   * -5   3  3
   * -5   0  3
   * -5   3  3
   * \endcode
   *
   * For the y direction see kirschKernelY
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class kirschKernelX : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelX();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    kirschKernelX(const kirschKernelX& other);

    /**
     * returns name of this type ("kirschKernelX")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual kirschKernelX<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual kirschKernelX<T>* newInstance() const;
  };

  /**
   * Kirsch Kernel Y
   *
   * Defined as a 3x3 2D (non-separable) kernel:
   *
   * \code
   * -5 -5 -5
   *  3  0  3
   *  3  3  3
   * \endcode
   *
   * For the x direction see kirschKernelX
   *
   * @ingroup gLinearFilters
   */
  template<typename T>
  class kirschKernelY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelY();

    /**
     * copy constructor
     * @param other the kernel to be copied.
     */
    kirschKernelY(const kirschKernelY& other);

    /**
     * returns name of this type ("kirschKernelY")
     */
    virtual const std::string& name() const;

    /**
     * clone member
     * @return a pointer to a copy of this instance
     */
    virtual kirschKernelY<T>* clone() const;
    
    /**
     * newInstance member
     * @return a pointer to a new instance
     */
    virtual kirschKernelY<T>* newInstance() const;
  };

}

#include "ltiGradientKernels_template.h"

#endif

