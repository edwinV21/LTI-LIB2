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
 * \file   ltiSegmentation.h
 *         Parent class of all segmentation algorithms.
 * 
 * \author Pablo Alvarado
 * \date   24.08.99
 *
 * revisions ..: $Id: ltiSegmentation.h,v 1.4 2013-10-25 14:43:34 alvarado Exp $
 */

#ifndef _LTI_SEGMENTATION_H_
#define _LTI_SEGMENTATION_H_

#include "ltiObject.h"
#include "ltiFunctor.h"
#include "ltiException.h"
#include "ltiTypes.h"
#include "ltiImage.h"

namespace lti  {
  /**
   * Base class for all segmentation algorithms
   *
   * @ingroup gSegmentation
   */
  class segmentation : public functor {
  public:

    /**
     * Default constructor
     */
    segmentation();

    /**
     * Copy constructor
     */
    segmentation(const segmentation& other);

    /**
     * Destructor
     */
    virtual ~segmentation();

    /**
     * Copy data of "other" functor.
     */
    segmentation& copy(const segmentation& other);

    /**
     * Clone member
     */
    virtual segmentation* clone() const=0;

    /**
     * New instance member
     */
    virtual segmentation* newInstance() const=0;

    /**
     * Returns the name of this type
     */
    virtual const std::string& name() const=0;

    /**
     * Segment the given image and leave a labeled mask into the given matrix.
     *
     * Inherited classes may provide additional functionality, but all
     * segmentation algorithms have to provide at least this interface for
     * color images.  
     *
     * If inherited classes can only cope with gray valued channels,
     * then this method extracts first the intensity from the given
     * color image.
     *
     * @param img input color image
     * @param mask output labeled mask.  Each detected region will have its own
     *        label.
     */
    virtual bool apply(const image& img,imatrix& mask) const = 0;

    /**
     * Utility method to overlay on the input image the boundaries of all mask
     * labels
     *
     * @param mask the output mask from the apply method 
     * @param img input image, on which the boundaries will be
     *        overlayed
     * @param neighborhood the neighborhood used (should be 8 or 4 or
     *        '8' or '4')
     * @return true if successfull, false otherwise
     */
    bool overlayBoundaries(const imatrix& mask,
			   image& img,
			   const rgbaPixel& color=rgbaPixel(0,0,0),
			   const char neighborhood='4') const;
  };
} // namespace lti

#endif
