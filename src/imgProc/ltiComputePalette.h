/*
 * Copyright (C) 2003, 2004, 2005, 2006
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
 * \file   ltiComputePalette.h
 *         Compute a palette based on a color image and a labeled mask.
 * \author Pablo Alvarado
 * \date   11.3.2002
 *
 * revisions ..: $Id: ltiComputePalette.h,v 1.2 2008-12-26 17:51:46 alvarado Exp $
 */

#ifndef _LTI_COMPUTE_PALETTE_H_
#define _LTI_COMPUTE_PALETTE_H_

#include "ltiImage.h"
#include "ltiFunctor.h"
#include "ltiRGBPixel.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

namespace lti {
  /**
   * Compute a color palette.
   *
   * This class is used to compute a palette given a color image and an index
   * mask.  The entries of the palette will correspond to the average values of
   * all pixels sharing the same label in the labeled mask.
   *
   * The size of the resulting palette will be the maximal value in the index
   * mask plus one.
   *
   * @see usePalette
   */
  class computePalette : public functor {
  public:
    /**
     * Default constructor
     */
    computePalette();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    computePalette(const computePalette& other);

    /**
     * Destructor
     */
    virtual ~computePalette();

    /**
     * Return the name of this type ("computePalette")
     */
    virtual const std::string& name() const;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,const channel8& mask, palette& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,const matrix<int>& mask, palette& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel8& mask,
               palette& dest,
               vector<rgbPixel<float> >& palStdDev) const;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               palette& dest,
               vector<rgbPixel<float> >& palStdDev) const;

    /**
     * Computes the mean values for each label present in the image.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param means mean value of all elements with the same label.
     * @param n number of elements for each label.
     * @param numLabels this optional parameter can be used to save some time
     *                  if you give the number of labels in the mask, or to
     *                  be more specific the greatest label in mask + 1.  If
     *                  ommited, the mask.maximum() will be called to find out
     *                  how many elements the vectors n and means must have,
     *                  which costs one iteration on the whole mask.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               vector<rgbPixel<float> >& means,
               ivector& n,
               const int numLabels = -1) const;


    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel8& mask,
               palette& dest,
               vector<rgbPixel<float> >& palStdDev,
               vector<int>& n) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).
     * @param palStdDev standard deviation for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               palette& dest,
               vector<rgbPixel<float> >& palStdDev,
               vector<int>& n) const;

    /**
     * Generate multivariate statistics for each entry in the palette, at it
     * takes more time than the other ones.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).  Note that this apply
     *                               will compute with floats)
     * @param palCovar covariance matrices for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               vector<rgbPixel<float> >& dest,
               std::vector<matrix<float> >& palCovar,
               vector<int>& n) const;

    /**
     * Generate multivariate statistics for each entry in the palette.  It
     * takes more time than the other ones.  The image will be assumed to be
     * composed by the given three channels.
     *
     * @param ch1 first channel of the image
     * @param ch2 second channel of the image
     * @param ch3 third channel of the image
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).  Note that this apply
     *                               will compute with floats)
     * @param palCovar covariance matrices for each palette entry.
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& ch1,
               const channel& ch2,
               const channel& ch3,
               const matrix<int>& mask,
               vector<rgbPixel<float> >& dest,
               std::vector<matrix<float> >& palCovar,
               vector<int>& n) const;

    /**
     * Generate multivariate statistics for each entry in the palette.  It
     * takes more time than the other ones.  The image will be assumed to be
     * composed by the given three channels.
     *
     * @param ch1 first channel of the image
     * @param ch2 second channel of the image
     * @param ch3 third channel of the image
     * @param mask mask with palette entry indices.
     * @param dest computed palette (mean value of all elements with the
     *                               same index).  Note that this apply
     *                               will compute with floats)
     * @param n number of pixels found for each entry
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& ch1,
               const channel& ch2,
               const channel& ch3,
               const matrix<int>& mask,
               vector<rgbPixel<float> >& dest,
               vector<int>& n) const;

    /**
     * This apply method computes the mean values and variances of the
     * regions indicated by the mask for the given channel.
     *
     * @param ch channel for which the region statistics will be computed
     * @param mask labeled mask indicating the regions
     * @param means mean values for each region
     * @param vars variances for each region
     * @param n number of pixels per region
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& ch,
               const matrix<int>& mask,
               vector<float>& means,
               vector<float>& vars,
               vector<int>& n) const;


    /**
     * Compute the mean value and covariance matrix for just one label in the
     * mask.
     *
     * @param src image with the source data.
     * @param mask mask with palette entry indices.
     * @param label label to be analyzed.
     * @param mean mean color of the given label.
     * @param covar covariance matrix for the given label.
     * @param n number of pixels found with the given label.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const matrix<int>& mask,
               const int label,
               rgbPixel<float>& mean,
               matrix<float>& covar,
               int& n) const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    computePalette& copy(const computePalette& other);

    /**
     * Alias for copy member.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    computePalette& operator=(const computePalette& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual computePalette* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual computePalette* newInstance() const;

    /**
     * Returns used parameters.
     */
    const parameters& getParameters() const;

  private:
    /**
     * Outer product of two colors.
     */
    void outer(const rgbPixel<float>& a,
               const rgbPixel<float>& b,
                     matrix<double>& mat) const;
  };
}

#endif
