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
 * \file ltiKernelFunctorInterface.h Contains the pure abstract interface
 *         lti::kernelFunctorInterface, which is the base class of all
 *         kernel functors used by SVM, KPCA, etc.
 * 
 * \author Jochen Wickel
 * \author Pablo Alvarado
 * \date   08.04.2009
 *
 * revisions ..: $Id: ltiKernelFunctorInterface.h,v 1.3 2009-06-08 00:23:12 alvarado Exp $
 */

#ifndef _LTI_KERNEL_FUNCTOR_INTERFACE_H_
#define _LTI_KERNEL_FUNCTOR_INTERFACE_H_

#include "ltiVector.h"

namespace lti {

  /**
   * Class kernelFunctorInterface
   *
   * This class defines a kernel functor interface.
   *
   * A kernel is sort of an inner product, but needs not be linear. It
   * takes two vectors and returns a scalar. The most simple kernel is
   * the canonical scalar product.
   *
   * The standard apply() method returns the result of the inner product
   * replacement through an argument passed per reference, leaving the
   * boolean return value to report success or failure, which is usually to
   * check if the result is a valid number.
   *
   * The non-virtual method inner() does usually the real job, 
   * and if small enough, it should be implemented.
   *
   * @see linearKernel, sigmoidKernel, radialKernel, polynomialKernel
   *
   * For more information on kernel machines see
   * <A HREF="http://www.kernel-machines.org">here</A>.
   *
   * @ingroup gKernelMath
   */
  template<typename T>
  class kernelFunctorInterface {
  public:
    /**
     * Destructor
     */
    virtual ~kernelFunctorInterface() {};

    /**
     * Compute the inner-product-like value of the two given vectors
     * and return the result in the given argument.
     *
     * @param first vector<T> working as first operand.
     * @param second vector<T> serving as second operand.
     * @param result T result of the replacement for inner product.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const vector<T>& first, 
                       const vector<T>& second,
                       T& result) const = 0;

    /**
     * Compute the inner-product-like value of the two given vectors
     * and return the resulting value.
     *
     * @param first vector<T> working as first operand.
     * @param second vector<T> serving as second operand.
     * @return result of the replacement for inner product.
     */
    virtual T apply(const vector<T>& first, 
                    const vector<T>& second) const = 0; 

    /**
     * Compute the gradient value \f$\dfrac{\partial k(x,y)}{\partial x}\f$
     * employed in some optimization processes.
     */
    virtual bool gradient(const vector<T>& x,
                          const vector<T>& y,
                          vector<T>& grad) const = 0;

    /**
     * Compute the gradient value \f$\dfrac{\partial k(x,x)}{\partial x}\f$
     * employed in some optimization processes.
     */
    virtual bool gradient(const vector<T>& x,
                          vector<T>& grad) const = 0;

  };

}

#endif

