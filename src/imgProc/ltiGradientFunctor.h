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
 * \file   ltiGradientFunctor.h
 *         This file contains the gradientFunctor which is a
 *         convenient wrapper for convolution of a channel with a
 *         gradient kernel.
 * \author Pablo Alvarado
 * \date   01.06.2003
 *
 * $Id: ltiGradientFunctor.h,v 1.7 2010-09-03 15:59:59 alvarado Exp $
 */

#ifndef _LTI_GRADIENT_FUNCTOR_H_
#define _LTI_GRADIENT_FUNCTOR_H_

#include "ltiFunctor.h"
#include "ltiArctanLUT.h"
#include "ltiChannel.h"

namespace lti {
  /**
   * The gradient functor is a simple wrapper for the convolution functor with
   * some convenience parameterization to choose between different common
   * gradient kernels.
   *
   * Not only the classical simple difference computation (right minus
   * left for the x direction or bottom minus top for the y direction)
   * and the classical Sobel, Prewitt, Robinson, Roberts and Kirsch
   * kernels can be used, but the more sophisticated Ando optimal
   * kernels (see lti::gradientKernelX) and the approximation using
   * oriented gaussian derivatives can be used.
   *
   * The functor computes the gradient for lti::channel %objects only.  For
   * color images you can use one of the derived classes.
   *
   * Example:
   * \code
   * lti::gradientFunctor::parameters gradPar;
   * gradPar.format = lti::gradientFunctor::Polar; // use Polar format
   * gradPar.kernelType = lti::gradientFunctor::Sobel; // use Sobel kernels
   *
   * lti::gradientFunctor grad(gradPar);
   * lti::channel in,mag,arg;
   * // Fill the in with whatever you need to
   * // ...
   * grad.apply(in,mag,arg);
   * \endcode
   *
   * @see ltiGradientKernels.h, lti::colorContrastGradient
   *
   * @ingroup gLinearFilters
   */
  class gradientFunctor : public functor {
  public:

    /**
     * Format types
     */
    enum eOutputFormat {
      Polar,     /**< Polar coordinates first magnitud, then angle */
      Cartesian /**< Cartesian coordinates first x, then y        */
    };

    /**
     * Kernel Type
     */
    enum eKernelType {
      Ando,    /**< Use the optimal kernel gradients of Ando
                *   (see lti::andoKernelX).
                */
      OGD,     /**< Use oriented Gaussian derivatives, see lti::ogd1Kernel */
      Difference, /**<
                   * Simplest (and fastest) kernel just right-left,
                   * top-bottom
                   */
      Roberts,    /**<
                   * Also very simple, but has a location-bias of half pixel.
                   * These are 2x2 kernels,
                   */
      Sobel,      /**<
                   * 3x3 separable kernel with a 1 2 1, -1 0 1 sub-kernels
                   * (see lti::sobelKernelX)
                   */
      Prewitt,    /**<
                   * 3x3 separable kernel with a 1 1 1, -1 0 1 sub-kernels
                   * (see lti::prewittKernelX)
                   */
      Robinson,   /**<
                   * 3x3 non-separable kernel (see lti::robinsonKernelX)
                   */
      Kirsch,      /**<
                    * 3x3 non-separable kernel (see lti::kirschKernelX)
                    */
      Harris      /**<
                   * 1x5 separable kernel -2 -1 0 1 2
                   * (see lti::harrisKernelX)
                   */
    };

    /**
     * the parameters for the class gradientFunctor
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

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
       * Returns name of this type ("lti::gradientFunctor::parameters")
       */
      const std::string& name() const;

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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
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
       * The output of the apply methods can be in polar or cartesic
       * coordinates.  You can specify here which format you prefer.
       *
       * Default value: Polar
       */
      eOutputFormat format;

      /**
       * Type of gradient kernel to be used.
       *
       * Default value: Ando
       */
      eKernelType kernelType;

      /**
       * Size of the kernel used.
       *
       * This attribute is used only for the Optimal and OGD Kernel types.
       *
       * For the Optimal kernel type, the value \b must be 3, 4, or 5.
       * For the OGD odd values greater or equal 3 should be used.
       *
       * Default value: 3
       */
      int gradientKernelSize;

      /**
       * Variance used for the Gaussian.
       *
       * This argument has effect only if kernelType is OGD.
       *
       * It is the variance use for the Gaussian to be derived as approximation
       * for a gradient kernel.
       *
       * Default value: -1 (i.e. the variance will be computed from the size
       *                    of the kernel (\a gradientKernelSize) )
       */
      float ogdVariance;
    };

    /**
     * Default constructor
     */
    gradientFunctor(const bool initializeParameters = true);

    /**
     * Construct a functor using gradient kernels with the specified values.
     *
     * @param format specify the format to be use: Cartesic or Polar
     * @param gradKernelSize size for the gradient kernel to be used.
     */
    gradientFunctor(const eOutputFormat& format,
                    const int gradKernelSize=0);

    /**
     * Construct a functor using the given parameters
     */
    gradientFunctor(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    gradientFunctor(const gradientFunctor& other);

    /**
     * destructor
     */
    virtual ~gradientFunctor();

    /**
     * Returns the name of this type ("lti::gradientFunctor")
     */
    virtual const std::string& name() const;

    /**
     * Computes the gradient of the given channel.
     *
     * @param src channel with the source data.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
                     channel& xOrMag,
                     channel& yOrArg) const;

    /**
     * Computes the gradient magnitude of the given channel.
     *
     * @param src channel with the source data.
     * @param mag the resulting magnitude of the gradient will be left here.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
                     channel& mag) const;

    /**
     * Computes the gradient magnitude of the given channel.
     *
     * @param srcdest channel with source data, the resulting magnitude
     *                will be left here too.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;


    /**
     * Computes the gradient of the given channel.
     *
     * @param src channel8 with the source data.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
                     channel& xOrMag,
                     channel& yOrArg) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gradientFunctor& copy(const gradientFunctor& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gradientFunctor& operator=(const gradientFunctor& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual gradientFunctor* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual gradientFunctor* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * @name One channel gradient computation
     */
    //@{
    /**
     * compute the gradient of the src channel8 using the adecuate
     * method xyDifferentiateXYZ
     */
    bool computeGradientCart(const channel& src,
                                   channel& dx,
                                   channel& dy) const;

    /**
     * differentiate the image in X and Y directions using operator (-1 0 1)
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCart(const channel& src,
                                  channel& dx,
                                  channel& dy) const;

    /**
     * differentiate the image in X and Y directions using optimal operators.
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCartOpt(const channel& src,
                                     channel& dx,
                                     channel& dy) const;

    /**
     * differentiate the image in X and Y directions using optimal operators.
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCartClassicSep(const channel& src,
                                            channel& dx,
                                            channel& dy) const;

    /**
     * differentiate the image in X and Y directions using classic gradients.
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCartClassic2D(const channel& src,
                                           channel& dx,
                                           channel& dy) const;

    /**
     * Roberts operator
     *
     * calculates the gradient at the center of a 2x2 mask. The position marked
     * with "S" is the position where the gradient is stored at in the output
     * channels:
     *
     * \code
     * ( S  +)
     * ( +  +)
     * \endcode
     *
     * (The origin (0,0) is at the bottom left of the image)
     * @param src the already smoothed image
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return returns NULL if successful, a textual error message otherwise
     */
    bool xyDifferentiateImageCartInterPixel(const channel& src,
                                            channel& dx,
                                            channel& dy) const;

    /**
     * Differentiate the image in X and Y directions using operator (-1 0 1)
     * The result will be directly in polar coordinates
     * @param src source channel
     * @param gradAbs magnitude of the gradient.
     * @param gradPhi argument of the gradient (angle).
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImage(const channel8& src,
                              channel& gradAbs,
                              channel& gradPhi) const;

    //@}

    /**
     * Look-Up Table for computation of arc tangent
     */
    arctanLUT atan2_;

    /**
     * convert a pair of values in cartesic system into a pair in
     * polar system
     *
     * The LUT takes care that the returned value lies between 0 and 2*Pi
     */
    inline void cartToPolar(const int dx,const int dy,
                            float& mag,float& angle) const;

  };

  /**
   * Read a gradientFunctor::eKernelType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, gradientFunctor::eKernelType& data);

  /**
   * Write a gradientFunctor::eKernelType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const gradientFunctor::eKernelType& data);


  /**
   * Read a gradientFunctor::eOutputFormat
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, gradientFunctor::eOutputFormat& data);



  /**
   * Write a gradientFunctor::eOutputFormat
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const gradientFunctor::eOutputFormat& data);
}

#endif

