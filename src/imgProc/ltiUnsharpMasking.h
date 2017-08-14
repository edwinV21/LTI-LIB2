/*
 * Copyright (C) 2007
 * Instituto Tecnológico de Costa Rica
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
 * \file   ltiUnsharpMasking.h
 *         Contains a class for image sharpening 
 * 
 * \author Miguel Angel Aguilar Ulloa
 * \date   17/10/2007
 *
 * revisions ..: $Id: ltiUnsharpMasking.h,v 1.1 2008-02-18 00:17:23 alvarado Exp $
 */

#ifndef _LTI_UNSHARP_MASKING_H_
#define _LTI_UNSHARP_MASKING_H_

#include "ltiSharpeningKernels.h"
#include "ltiSharpening.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

namespace lti {

  /**
   * This class convolve one of two types of kernels with an image for sharp it, 
   * base on "Unsharp mask and high-boost filtering".
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
   * gaussian kernel. See lti::sharpeningKernels for further information 
   * about kernels.
   *
   * The parameters define the size, the  constan A, the type (laplacian or 
   * gaussian) and the type of neighbor (four or eight).
   *
   * @ingroup gSharpening
   */
  class unsharpMasking : public sharpening {
  public:
    /**
     * The parameters for the class unsharpMasking
     */
    class parameters : public sharpening::parameters {
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
       * Size of the kernel in one dimension.
       *
       * Default value: 3x3
       *
       */
      int kernelSize;

      /**
       * High boost constant
       *
       * Default value: 1
       */
      float A;

      /**
       * Type of the kernel for the unsharp mask 
       * (Laplacian or Gaussian).
       *
       * Default value: sharpeningKernels::Laplacian
       */
      sharpeningKernels::eKernelType kernelType;

      /**
       * This is the type of the neighbor consider in
       * the laplacian kernel case. 
       *
       * Default value: sharpeningKernels::FourNeighbor
       */
      sharpeningKernels::eNeighborType neighborType;

      /**
       * Variance of the gassian kernel in case it use
       *
       * Default value: 5.0
       */
      double variance;
    };

    /**
     * Default constructor
     */
    unsharpMasking();

    /**
     * Construct a sharpening using the given parameters
     */
    unsharpMasking(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    unsharpMasking(const unsharpMasking& other);

    /**
     * Destructor
     */
    virtual ~unsharpMasking();

    /**
     * Operates on the given argument.
     *
     * @param srcdest channel8 with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Operates on the given argument.
     *
     * @param srcdest channel with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, channel8& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, channel& dest) const;


    /**
     * Copy data of "other" sharpening.
     * @param other the sharpening to be copied
     * @return a reference to this sharpening object
     */
    unsharpMasking& copy(const unsharpMasking& other);

    /**
     * Alias for copy member
     * @param other the sharpening to be copied
     * @return a reference to this sharpening object
     */
    unsharpMasking& operator=(const unsharpMasking& other);

    /**
     * Returns the complete name of the sharpening class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this sharpening.
     */
    virtual unsharpMasking* clone() const;

    /**
     * Returns a pointer to a new instance of this sharpening.
     */
    virtual unsharpMasking* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
   /**
    * Convolve the input channel with the selected kernel
    * and return the result in dest
    *
    * @param src channel with the source data.
    * @param dest channel where the result will be left, sharp image.
    * @return true if apply successful or false otherwise.
    */
    bool convolve(channel& srcdest) const;

   /**
    * Crop the given value to the limits 0 - 1
    * on floats 
    */
    static float crop (const float value) ;
  };
}

#endif

