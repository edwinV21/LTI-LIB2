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
 * \file   ltiRectangularKernel_template.h
 *         Contains the lti::rectangularKernel used with lti::morphology 
 *         inherited classes.
 * \author Pablo Alvarado
 * \date   12.10.2009
 *
 * revisions ..: $Id: ltiRectangularKernel_template.h,v 1.1 2010-05-24 01:50:26 alvarado Exp $
 */

namespace lti {
  
  // constructor
  template<class T>
  rectangularKernel<T>::rectangularKernel(const ipoint& size, const T& on)
    : separableKernel<T>() {
    generate(size,on);
  }
  
  // generate filter
  template<class T>
  void rectangularKernel<T>::generate(const ipoint& size, const T& on) {

    int realSizeX = size.x;
    int realSizeY = size.y;

    if ((realSizeX % 2) == 0) {
      realSizeX++;
    }

    if (realSizeX < 3) {
      realSizeX = 3;
    }

    if ((realSizeY % 2) == 0) {
      realSizeY++;
    }

    if (realSizeY < 3) {
      realSizeY = 3;
    }

    int borderX = realSizeX/2;
    int borderY = realSizeY/2;

    kernel1D<T> oneDX(-borderX,borderX,on);
    kernel1D<T> oneDY(-borderY,borderY,on);

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(oneDX);
    this->getColFilter(0).copy(oneDY);

    setNorm(typeInfo<T>::suggestedNorm());
  }

}

