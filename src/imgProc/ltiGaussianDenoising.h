/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiGaussianDenoising.h
 *         Contains the class lti::gaussianDenoising, 
 *         which has still a simple wrapper class with denoising interface for
 *         the convolution with a gaussian filter.
 * 
 * \author Pablo Alvarado
 * \date   05.10.2007
 *
 * revisions ..: $Id: ltiGaussianDenoising.h,v 1.2 2007-10-14 02:51:00 alvarado Exp $
 */

#ifndef _LTI_GAUSSIAN_DENOISING_H_
#define _LTI_GAUSSIAN_DENOISING_H_

#include "ltiDenoising.h"
#include "ltiConvolution.h"

namespace lti {

  /**
   * Class gaussianDenoising
   *
   * This is a simple wrapper class for a convolution with a gaussian
   * kernel, which provides the denoising interface in case denoising
   * polymorphism is desired.
   *
   * @see gaussianDenoising::parameters.
   *
   * @ingroup gDenoising
   */
  class gaussianDenoising : public denoising {
  public:
    /**
     * The parameters for the class gaussianDenoising
     */
    class parameters : public denoising::parameters {
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
       * Kernel variance.
       *
       * The gaussian kernel will have this variance (the square of
       * the standard deviation).
       *
       * Default value: 1.4426950409
       */
      double variance;

      /**
       * The kernel size
       *
       * Each side of the square kernel mask is kernelSize pixels long.
       *
       * Default value: 3
       */
      int kernelSize;

    };

    /**
     * Default constructor
     */
    gaussianDenoising();

    /**
     * Construct a functor using the given parameters
     */
    gaussianDenoising(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    gaussianDenoising(const gaussianDenoising& other);

    /**
     * Destructor
     */
    virtual ~gaussianDenoising();

    /**
     * Operates on the given argument.
     *
     * @param srcdest channel with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Operates on the given argument.
     *
     * @param srcdest channel8 with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, channel& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, channel8& dest) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gaussianDenoising& copy(const gaussianDenoising& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gaussianDenoising& operator=(const gaussianDenoising& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual gaussianDenoising* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual gaussianDenoising* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

  protected:
    /**
     * Convolution functor for ubytes.
     *
     * It is stored to save the creation time when applying
     */
    convolution uconv_;
    
    /**
     * Convolution functor for float.
     *
     * It is stored to save the creation time when applying
     */
    convolution fconv_;
  };
}

#endif

