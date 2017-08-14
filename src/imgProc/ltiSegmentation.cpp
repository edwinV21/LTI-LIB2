/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
 * Copyright (C) 2008
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
 * \file   ltiSegmentation.cpp
 *         Parent class of all segmentation algorithms.
 * 
 * \author Pablo Alvarado
 * \date   24.08.99
 *
 * revisions ..: $Id: ltiSegmentation.cpp,v 1.3 2013-10-25 14:53:03 alvarado Exp $
 */

#include "ltiSegmentation.h"

namespace lti {
  segmentation::segmentation() 
    : functor() {
  }

  segmentation::segmentation(const segmentation& other) 
    : functor() {
    copy(other);
  }

  // destructor
  segmentation::~segmentation() {
  }

  // copy data of "other" functor.
  segmentation& segmentation::copy(const segmentation& other) {
    functor::copy(other);
    return (*this);
  }

  /**
   * Utility method to overlay on the input image the boundaries of all mask
   * labels
   *
   * @param mask the output mask from the apply method 
   *
   * @param img input image, on which the boundaries will be
   *        overlayed
   *
   * @param neighborhood the neighborhood used (should be 8 or 4 or
   *        '8' or '4')
   * @return true if successfull, false otherwise
   */
  bool segmentation::overlayBoundaries(const imatrix& mask,
                                       image& img,
                                       const rgbaPixel& color,
                                       const char neighborhood) const {

    if (img.size() != mask.size()) {
      setStatusString("Error, labels mask and image must have the same size");
      return false;
    }

    bool n8=!((neighborhood=='4') || (neighborhood==4));


    const int lasty=mask.rows()-1;
    const int lastx=mask.columns()-1;

    for (int y=0;y<lasty;++y) {
      const int ny=y+1;

      // loop unrolling for x=0, since for n8 is with x=0 different
      if (mask.at(y,0) != mask.at(ny,0)) {
	img.at(y,0)=img.at(ny,0)=color;
      }
      if (mask.at(y,0) != mask.at(y,1)) {
	img.at(y,0)=img.at(y,1)=color;
      }
      if (n8 && (mask.at(y,0) != mask.at(ny,1))) {
	img.at(y,0)=img.at(ny,1)=color;
      }
      
      for (int x=1;x<lastx;++x) {
	const int current = mask.at(y,x);
        const int nx=x+1;

        if (current != mask.at(ny,x)) {
	  img.at(y,x)=img.at(ny,x)=color;
	}
	if (current != mask.at(y,nx)) {
	  img.at(y,x)=img.at(y,nx)=color;
	}
        if (n8) {
	  if (current != mask.at(ny,nx)) {
	    img.at(y,x)=img.at(ny,nx)=color;
	  }
	  const int px=x-1;
	  if (current != mask.at(ny,px)) {
	    img.at(y,x)=img.at(ny,px)=color;
	  }
	}
      }
    }
    return true;
  }


} // namespace lti
