/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2012
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
 * \file   ltiMergeHSVToImage.h
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.99
 * 
 * $Id: ltiMergeHSVToImage.h,v 1.1 2012-09-09 02:26:29 alvarado Exp $
 */

#ifndef _LTI_MERGE_HSV_TO_IMAGE_H_
#define _LTI_MERGE_HSV_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge HSV (Hue Saturation Value) channels.
   *
   * @see lti::splitImageToHSV
   *
   * @ingroup gColor
   */
  class mergeHSVToImage : public mergeImage {
  public:

    /**
     * Constructor
     */
    mergeHSVToImage(void);

    /**
     * return the name of this type
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual mergeHSVToImage* clone() const;

    /**
     * returns a pointer to a new instance of the functor.
     */
    virtual mergeHSVToImage* newInstance() const;

    /**
     * merge hue channel H, saturation S and value
     * channel V to an image
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& H,
                       const matrix<float>& S,
                       const matrix<float>& V,
                       image& img) const;


    /**
     * merge hue channel H, saturation S and value
     * channel V to an image
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<ubyte>& H,
                       const matrix<ubyte>& S,
                       const matrix<ubyte>& V,
                       image& img) const;

    /**
     * merge the hue value H, saturation S and value value V
     * to a pixel
     * @param H the hue value
     * @param S the saturation value
     * @param V the value
     * @param pixel the merged pixel
     */
    virtual bool apply(const float& H,
                       const float& S,
                       const float& V,
                       rgbaPixel& pixel) const;

    /**
     * merge the hue value H, saturation S and value V
     * to a pixel
     * @param H the hue value
     * @param S the saturation value
     * @param V the value
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& H,
                       const ubyte& S,
                       const ubyte& V,
                       rgbaPixel& pixel) const;
    

  };

}

#endif

