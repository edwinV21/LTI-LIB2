/*
 * Copyright (C) 2007
 * Instituto Tecnol�gico de Costa Rica
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
 * \file   ltiSharpeningKernels.h
 *         Contains a Two-dimensional kernels for image sharpening
 * \author Miguel Angel Aguilar Ulloa
 * \date   16.10.2007
 *
 * $Id: ltiSharpeningKernels.h,v 1.3 2012-06-15 13:39:21 alvarado Exp $
 */


#ifndef _LTI_SHARPENING_KERNELS_H_
#define _LTI_SHARPENING_KERNELS_H_

#include "ltiKernel2D.h"

namespace lti {
  /**
   * This class contains a Two-dimensional kernels for image sharpening, base
   * on "Unsharp mask and high-boost filtering".
   *
   * A common technique for sharp images is "unsharp mask", it consist of 
   * subtracting a blurred version of an image from the image itself. This
   * is expressed as
   *
   * \f[f_{s}(x,y)=f(x,y)-\bar{f}(x,y)\f]
   *
   * where \f$ f_{s}(x,y)\f$ denotes the sharpened image obteined by unsharp 
   * masking, and \f$ \bar{f}(x,y)\f$ is a blurred version of \f$ f(x,y) \f$.
   *
   * A generalization of upsharp masking is called high-boost filtering, that
   * is defined as
   *
   * \f[f_{hb}(x,y)=Af(x,y)-\bar{f}(x,y)\f]
   *
   * where \f$ A \ge 1 \f$. Reordening we obtain,
   *
   * \f[f_{hb}(x,y)=f(x,y)*A-f(x,y)*g(x,y)\f]
   * \f[f_{hb}(x,y)=f(x,y)*[A\delta(x,y)-g(x,y)]\f]
   *
   * This means that to obtain a sharp image in needed to convolve the image
   * with \f$ [A\delta(x,y)-g(x,y)] \f$, where \f$ g(x,y)\f$ is a laplacian or
   * gaussian kernel.
   *
   * This class implements the kernel define by \f$ [A\delta(x,y)-g(x,y)] \f$.
   * The parameters define the size, the constan A, the type (laplacian or
   * gaussian) and the type of neighbor (four or eight).
   * 
   * This is an example of a 3x3 four neighbor laplacian sharpening kernel:
   * \code
   *  0    -1    0
   * -1    A+4  -1
   *  0    -1    0
   * \endcode
   *
   * This is an example of a 3x3 eight neigbor laplacian sharpening kernel:
   * \code
   * -1    -1   -1
   * -1    A+8  -1
   * -1    -1   -1
   * \endcode
   *
   * To convolve a mask based on laplacian kernel with a channel follow the 
   * next example
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data;
   *
   * // ... initialize channel here ...
   *
   * // laplacian filter kernel with dimensions 3x3, high boost constant 
   * // and four neighbor type
   * lti::sharpeningKernels kernel(3,1.1,sharpeningKernels::Laplacian,
   * sharpeningKernels::FourNeighbor);
   *
   * // convolution operator
   * lti::convolution filter(kernel);                       
   *
   * // filter the channel to sharp it
   * filter.apply(data);
   * \endcode
   *
   * To convolve a mask based on gaussian kernel with a channel follow the 
   * next example
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data;
   *
   * // ... initialize channel here ...
   *
   * // laplacian filter kernel with dimensions 3x3, high boost constant 1.1
   * // and variance 5.0
   * lti::sharpening kernel(3,1.1,sharpeningKernels::Gaussian,5.0);
   *
   * // convolution operator
   * lti::convolution filter(kernel);                       
   *
   * // filter the channel to sharp it
   * filter.apply(data);
   * \endcode
   *
   * Note: The convolution with this kernels could gives values under zero 
   * or above the top limit of the type (>1 in floats or >255 in ubytes).
   * Thus, you have to crop it!.
   *
   * @ingroup gLinearFilters
   */
  class sharpeningKernels : public kernel2D<float> {
  public:
    /**
     * Kernel type.  This type indicates the type of the
     * kernel: laplacian or gaussian.
     */
    enum eKernelType {
      Laplacian,   /**< Laplacian kernel */
      Gaussian     /**< Gaussian kernel */
    };

    /**
     * Neighbor type.  This type indicates the type of the
     * of neighbor of the kernel: FourNeighbor or EightNeighbor.
     */
    enum eNeighborType {
      FourNeighbor,     /**< A four neighbor kernel type */
      EightNeighbor     /**< A eight neighbor kernel type */
    };

    /**
     * Constructor.
     *
     * Initialize this kernel with the specified values. If an invalid value 
     * is given, an empty kernel will be created.  
     *
     * @param kernelSize   size of the kernel in one dimension.
     * @param A            high boost constant.
     * @param kernelType   type of the kernel (Laplacian or Gaussian).
     * @param neighborType type of neighbor of the kernel, this is valid only
     *                     for laplacian Kernel.
     * @param variance     variance of the kernel. This is valid only for
     *                     gaussian kernel.
     */
    sharpeningKernels(const int kernelSize = 3, const float A = 1.0f, 
                      const eKernelType kernelType = Laplacian, 
                      const eNeighborType neighborType = FourNeighbor,
                      const double& variance = 1.3);

    /**
     * Constructor.
     *
     * Initialize this kernel with the specified values in case of gaussian 
     * kernel. If an invalid value is given, an empty kernel will be created.  
     *
     * @param kernelSize   size of the kernel in one dimension.
     * @param A            high boost constant.
     * @param kernelType   type of the kernel (Laplacian or Gaussian).
     * @param variance     variance of the kernel. This is valid only for
     *                     gaussian kernel.
     */
    sharpeningKernels(const int kernelSize, const float A, 
                      const eKernelType kernelType, const double& variance);

    /**
     * Initialize this kernel with the specified values. If an invalid value 
     * is given, an empty kernel will be created. 
     *
     * @param kernelSize   size of the kernel in one dimension.
     * @param A            high boost constant.
     * @param kernelType   type of the kernel (Laplacian or Gaussian).
     * @param neighborType type of neighbor of the kernel.
     * @param variance     variance of the kernel. This is valid only for
     *                     gaussian kernel.
     */
    void generate(const int kernelSize, const float A, 
                  const eKernelType kernelType,
                  const eNeighborType neighborType, const double& variance);

  };
}
#endif
