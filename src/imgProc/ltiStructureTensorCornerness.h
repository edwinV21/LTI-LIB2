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
 * \file   ltiStructureTensorCornerness.h
 *         Contains the class lti::structureTensorCornerness, a
 *         cornernessFunctor which uses the structure tensor
 * \author Peter Doerfler
 * \date   13.1.2005
 *
 * revisions ..: $Id: ltiStructureTensorCornerness.h,v 1.5 2013-03-10 05:57:46 alvarado Exp $
 */

#ifndef _LTI_STRUCTURE_TENSOR_CORNERNESS_H_
#define _LTI_STRUCTURE_TENSOR_CORNERNESS_H_

#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiImage.h"
#include "ltiConvolution.h"
#include "ltiCornernessFunctor.h"
#include "ltiGradientFunctor.h"
#include "ltiColorContrastGradient.h"

namespace lti {

  /**
   * Class structureTensorCornerness.
   *
   * This functor computes a cornerness from the structure tensor as
   * introduces by Harris. Let I_x and I_y be the gradient of an image
   * I in x and y direction then:
   *
   * \f[ I_{\mathit{temp}} = G(x,y,\sigma) * 
   *                          \begin{bmatrix}
   *                          \left(I_x(x,y)\right)^2 & I_x(x,y)I_y(x,y) \\
   *                          I_x(x,y)I_y(x,y) & \left(I_y(x,y)\right)^2 \\
   *                          \end{bmatrix} \f]
   *
   * \f[ \mathit{cornerness} = \mathrm{det}(I_{\mathit{temp}}) - 
   *        \alpha \cdot \left(\mathrm{trace}(I_{\mathit{temp}})\right)^2 \f]
   *
   * Here, G(x,y,sigma) is the Gaussian with variance sigma and * is
   * the convolution. The Gaussian is configured via
   * parameters::integralKernelSize and
   * parameters::integralVariance. Further, the second term with
   * factor parameters::alpha reduces the influence of edges in the
   * image. Typical values are in [0.04; 0.06].
   *
   * Take care that the kernel size for the integration should always
   * be larger than the kernel size (or 'influence region') of the
   * gradient.
   *
   * @ingroup gEdgeCorner
   */
  class structureTensorCornerness : public cornernessFunctor {
  public:
    /**
     * The parameters for the class structureTensorCornerness
     */
    class parameters : public cornernessFunctor::parameters {
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
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
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

      /**
       * @name parameters for the Gaussian intergration kernel
       */
      //@{
      /**
       * Size of the Gaussian kernel used for integration
       *
       * Default value: 7
       */
      int integrationKernelSize;

      /**
       * Variance of the Gaussian kernel used for intergration. The
       * value of -1 sets the value automatically fittion the kernel
       * size. See lti::gaussKernel1D<float>::generate() for further
       * details.
       *
       * Default value: -1
       */
      float integrationVariance;
      //@}

      /**
       * Influence of the trace of the structure tensor. See
       * structureTensorCornerness for details.
       *
       * Default value: 0.04
       */
      float alpha;

      /**
       * Parameters of the gradientFunctor used for
       * differentiation. Originally the harrisKernelX was used. But
       * Schmid and Mohr showed that it is much better to use an
       * oriented Gaussian derivative. This is also set as
       * default. Other values are as they are default in
       * gradientFunctor::parameters. Of course, the outputFormat is
       * always set to Cartesian.
       */
      gradientFunctor::parameters gradientParameters;

      /**
       * Parameters for the colorContrastGradient used for
       * 'differentiation' of color images. The same options as in
       * gradientParameters are present. Additionally, the method for
       * color handling has to be chosen.
       *
       * As for gradientParameters the default kernel used is OGD, and
       * outputFormat is always changed to Cartesian. All other
       * parametes are default from colorContrastGradient::parameters.
       */
      colorContrastGradient::parameters colorGradientParameters;

    };

    /**
     * Default constructor
     */
    structureTensorCornerness();

    /**
     * Construct a functor using the given parameters
     */
    structureTensorCornerness(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    structureTensorCornerness(const structureTensorCornerness& other);

    /**
     * Destructor
     */
    virtual ~structureTensorCornerness();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    structureTensorCornerness& copy(const structureTensorCornerness& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    structureTensorCornerness& 
    operator=(const structureTensorCornerness& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual structureTensorCornerness* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual structureTensorCornerness* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     *
     * The gradientFunctorParameters are checked for compliancy with
     * the definitions above. See
     * parameters::gradientParameters.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  protected:

    /**
     * Calculates the cornerness of \a src and leaves it in \a
     * dest. The minimal and maximal cornerness values are returned as
     * well.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value found
     * @param maxCornerness maximum cornerness value found
     * @return true if apply successful or false otherwise.
     */
    virtual bool worker(const channel& src, channel& dest,
                        float& minCornerness, float& maxCornerness) const;

    /**
     * Calculates the cornerness of \a src and leaves it in \a dest.
     *
     * Due to the different nature of grey and color images a
     * different but similar method will be used here. This can be
     * done by e.g. using a colorGradient instead of a normal
     * gradient.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value found
     * @param maxCornerness maximum cornerness value found
     * @return true if apply successful or false otherwise.
     */
    virtual bool worker(const image& src, channel& dest,
                        float& minCornerness, float& maxCornerness) const;


    /**
     * Computes the structure tensor and integrates over the elements
     * with the Gaussian kernel.
     *
     * @param src the original image
     * @param gx2 integrated squared gradient in x direction
     * @param gy2 integrated squared gradient in y direction
     * @param gxgy integrated product of gradients in x and y direction
     * @return true when successfull
     */
    bool computeStructureTensor(const channel& src,
                                channel& gx2,
                                channel& gy2,
                                channel& gxgy) const;

    /**
     * Computes the structure tensor and integrates over the elements
     * with the Gaussian kernel.
     *
     * @param src the original image
     * @param gx2 integrated squared gradient in x direction
     * @param gy2 integrated squared gradient in y direction
     * @param gxgy integrated product of gradients in x and y direction
     * @return true when successfull
     */
    bool computeStructureTensor(const image& src,
                                channel& gx2,
                                channel& gy2,
                                channel& gxgy) const;

    /**
     * Calculates the cornerness from the structure tensor.
     *
     * @param gx2 result from computeStructureTensor()
     * @param gy2 result from computeStructureTensor()
     * @param gxgy result from computeStructureTensor()
     * @param cornerness the cornerness
     * @param minCornerness smallest cornerness value
     * @param maxCornerness largest cornerness value
     * @return true when successful
     */
    bool computeCornerness(const channel& gx2,
                           const channel& gy2,
                           const channel& gxgy,
                           channel& cornerness,
                           float& minCornerness,
                           float& maxCornerness) const;

    /**
     * gradientFunctor used for finding gradients of channels
     */
    gradientFunctor gradient_;

    /**
     * colorContrastGradient used for finding gradients of channels
     */
    colorContrastGradient colorGradient_;

    /**
     * This convolution is used for the integration with a Gauss kernel
     */
    convolution gaussFilter_;

  };
}

#endif

