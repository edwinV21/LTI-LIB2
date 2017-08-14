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

/*
 * \file   ltiSquareKernel_template.h
 *         Contains the lti::squareKernel used with lti::morphology 
 *         inherited classes.
 * \author Pablo Alvarado
 * \date   12.10.2009
 *
 * revisions ..: $Id: ltiSquareKernel_template.h,v 1.2 2014-11-10 11:45:32 alvarado Exp $
 */

namespace lti {
  
  // constructor
  template<class T>
  squareKernel<T>::squareKernel(const int& size, const T& on)
    : separableKernel<T>() {
    generate(size,on);
  }
  
  // generate filter
  template<class T>
  void  squareKernel<T>::generate(const int& size, const T& on) {

    int realSize = size;

    if ((realSize % 2) == 0) {
      realSize++;
    }

    if (realSize < 3) {
      realSize = 3;
    }

    int border = realSize/2;

    kernel1D<T> oneD(-border,border,on);

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(oneD);
    this->getColFilter(0).copy(oneD);

    this->setNorm(typeInfo<T>::suggestedNorm());
  }

}

