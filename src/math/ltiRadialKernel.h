/*
 * Copyright (C) 2009
 * Jochen Wickel, Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiRadialKernel.h
 *         Contains the class lti::radialKernel,
 *         which implements a radial kernel with the kernelFunctor 
 *         interface.
 * \author Jochen Wickel
 * \author Pablo Alvarado
 * \date   08.04.2009
 *
 * revisions ..: $Id: ltiRadialKernel.h,v 1.4 2009-06-08 00:23:12 alvarado Exp $
 */

#ifndef _LTI_RADIAL_KERNEL_H_
#define _LTI_RADIAL_KERNEL_H_

#include "ltiFunctor.h"
#include "ltiKernelFunctorInterface.h"
#include <ltiEuclidianDistanceFunctor.h>

namespace lti {

  /**
   * Class radialKernel
   *
   * This class defines a radial kernel functor.  It is defined as
   * \f$ K(A,B) = \exp\left(-\frac{1}{2\sigma^2}\|A-B\|^2 \right)\f$.
   *
   * The width of the radial functions is defined through <code>sigma</code>
   * given in the parameters.
   *
   * @see radialKernel::parameters.
   *
   * @see lti::kernelFunctorInterface
   *
   * @ingroup gKernelMath
   */
  class radialKernel : public functor,
                       public kernelFunctorInterface<double>,
                       public kernelFunctorInterface<float> {
  public:

    /**
     * The parameters for the class radialKernel
     */
    class parameters : public functor::parameters {
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
       * Width of the radial function.
       *
       * This is equivalent to the standard deviation of the kernel. 
       *
       * Default value: 1
       */
      double sigma;
    };

    /**
     * Default constructor
     */
    radialKernel();

    /**
     * Construct a functor using the given parameters
     */
    radialKernel(const parameters& par);

    /**
     * Construct a functor using a parameters instance with the given
     * radial degree
     */
    radialKernel(const double sigma);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    radialKernel(const radialKernel& other);

    /**
     * Destructor
     */
    virtual ~radialKernel();

    /**
     * Compute the inner-product-like value of the two given vectors
     * and return the result in the given argument.
     *
     * @param first vector<T> working as first operand.
     * @param second vector<T> serving as second operand.
     * @param result T result of the replacement for inner product.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const fvector& first, 
                       const fvector& second,
                       float& result) const;

    /**
     * Compute the inner-product-like value of the two given vectors
     * and return the resulting value.
     *
     * @param first vector<T> working as first operand.
     * @param second vector<T> serving as second operand.
     * @return result of the replacement for inner product.
     */
    virtual float apply(const fvector& first, 
                        const fvector& second) const;

    /**
     * Compute the inner-product-like value of the two given vectors
     * and return the result in the given argument.
     *
     * @param first vector working as first operand.
     * @param second vector serving as second operand.
     * @param result of the replacement for inner product.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const dvector& first, 
                       const dvector& second,
                       double& result) const;

    /**
     * Compute the inner-product-like value of the two given vectors
     * and return the resulting value.
     *
     * @param first vector working as first operand.
     * @param second vector serving as second operand.
     * @return result of the replacement for inner product.
     */
    virtual double apply(const dvector& first,
                         const dvector& second) const; 

    /**
     * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
     * employed in some optimization processes.
     */
    virtual bool gradient(const fvector& x,
                          const fvector& y,
                          fvector& grad) const;

    /**
     * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
     * employed in some optimization processes.
     */
    virtual bool gradient(const fvector& x,
                          fvector& grad) const;

    /**
     * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
     * employed in some optimization processes.
     */
    virtual bool gradient(const dvector& x,
                          const dvector& y,
                          dvector& grad) const;

    /**
     * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
     * employed in some optimization processes.
     */
    virtual bool gradient(const dvector& x,
                          dvector& grad) const;

    /**
     * Non virtual method that computes the inner-product-like value of the 
     * two given vectors and return the resulting value.
     *
     * All inherited classes must override this method.
     *
     * @param first vector<T> working as first operand.
     * @param second vector<T> serving as second operand.
     * @return the result of the inner product replacement.
     */
    template<typename T>
    inline T inner(const vector<T>& first, 
                   const vector<T>& second) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    radialKernel& copy(const radialKernel& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    radialKernel& operator=(const radialKernel& other);

    /**
     * Returns the name of this type, which depends on the template parameters.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual radialKernel* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual radialKernel* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    virtual bool updateParameters();

  private:
    /**
     * Shadow of the radial sigma in the parameters
     */
    double sigma2_;

    /**
     * Compute the L2 distance
     */
    template<typename T>
    inline T distanceSqr(const vector<T>& first,
                         const vector<T>& second) const;

  };

  // The implementation of the two template members
  template<typename T>
  inline T radialKernel::distanceSqr(const vector<T>& first,
                                     const vector<T>& second) const {
    assert(first.size()==second.size());
    T dist(0);
    const int e=first.size();
    for (int i=0;i<e;++i) {
      dist+=sqr(first.at(i)-second.at(i));
    }
    return dist;
  }

  // The implementation of the one and only inline template member
  template<typename T>
  inline T radialKernel::inner(const vector<T>& first,
                               const vector<T>& second) const {
    return static_cast<T>(exp(distanceSqr(first,second)*sigma2_));
  }

}

#endif
