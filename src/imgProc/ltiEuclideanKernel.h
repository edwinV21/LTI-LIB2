/*
 * Copyright (C) 2009
 * Pablo Alvarado
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
 * \file   ltiEuclideanKernel.h
 *         Contains the lti::euclideanKernel used with lti::morphology 
 *         inherited classes.
 * \author Pablo Alvarado
 * \date   12.10.2009
 *
 * revisions ..: $Id: ltiEuclideanKernel.h,v 1.1 2009-10-13 01:16:56 alvarado Exp $
 */


#ifndef _LTI_EUCLIDEAN_KERNEL_H_
#define _LTI_EUCLIDEAN_KERNEL_H_

#include "ltiKernel2D.h"

namespace lti {
  /**
   * Class lti::euclideanKernel
   *
   * This binary filter kernel (values only 0 or \p onValue) is used as a
   * structuring element for erosion/dilation operations.  It is equivalent to
   * a discretized circle.
   *
   * The onValue is important if e.g. erosion::GrayNonFlat mode is
   * used. For erosion::Binary mode the exact value of onValue is not
   * relevant. 
   *
   * Default is typeInfo<T>::suggestedNorm()/255, i.e. 1 for ubyte and 0.003921
   * for float.
   *
   * The possible sizes are all odd values greater/equal 3.
   *
   * The kernel norm is set to the typeInfo<T>::suggestedNorm() so the
   * result is the expected black and white image.
   *
    * An example of an euclidean kernel of size nine:
   *
   * \code
   *          - - - - 1 - - - -
   *          - - 1 1 1 1 1 - -
   *          - 1 1 1 1 1 1 1 -
   *          - 1 1 1 1 1 1 1 -
   *          1 1 1 1 1 1 1 1 1
   *          - 1 1 1 1 1 1 1 -
   *          - 1 1 1 1 1 1 1 -
   *          - - 1 1 1 1 1 - -
   *          - - - - 1 - - - -
   * \endcode
   *
   * @ingroup gMorphology
   */
  template<class T>
  class euclideanKernel : public kernel2D<T> {
  public:
    /**
     * Constructor
     *
     * @param size is the dimension of one side (i.e. the filter kernel is a
     *             size x size kernel!)  This must be an odd value
     *             greater/equal 3.  If another value is given, the next valid
     *             odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    euclideanKernel(const int& size = 3,
                    const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));

    /**
     * Initialize this kernel with the specified values.
     *
     * @param size is the dimension of one side (i.e. the filter kernel is a
     *             size x size kernel!)  This must be an odd value
     *             greater/equal 3.  If another value is given, the next valid
     *             odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    void generate(const int& size,
                  const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));
  };

}

#include "ltiEuclideanKernel_template.h"

#endif
