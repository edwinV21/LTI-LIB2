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
 * \file   ltiThresholding.h
 *         Contains the class lti::thresholding, used to perform non-adaptive
 *         thresholding segmentation and some adaptive methods.
 * 
 * \author Pablo Alvarado
 * \date   26.10.2009
 *
 * revisions ..: $Id: ltiThresholding.h,v 1.3 2009-11-12 03:53:47 alvarado Exp $
 */

#ifndef _LTI_THRESHOLDING_H_
#define _LTI_THRESHOLDING_H_

#include "ltiInterval.h"
#include "ltiMatrix.h"
#include "ltiSegmentation.h"

namespace lti {

  /**
   * Class thresholding
   *
   * Thresholding is the simplest method of segmentation, in which each pixel
   * of a gray valued image is assigned whether to the background or to the
   * foreground, generating a binary image.  
   *
   * This class assumes as background the pixels with gray values falling
   * outside the foreground interval given in the parameters, and as foreground
   * the pixels with values falling within that interval.
   *
   * You can directly provide the threshold, or use one of several adaptive
   * methods.
   *
   * This class additionally provides the functionality of keeping the
   * background or the foreground values of the thresholded image.
   *
   * @see thresholding::parameters.
   *
   * @ingroup gSegmentation
   */
  class thresholding : public segmentation {
  public:
    /**
     * Method for thresholding
     */
    enum eMethod {
      Direct, /**< Direct method uses the threshold values as given in the 
               * parameters
               */

      Otsu,   /**< Otsu method.  Maximizes the between-class variance.  Just
               *   one threshold value is computed for the full range of the
               *   gray values.  All values below the threshold are considered
               *   background and the rest foreground.
               */

      OtsuInterval,  /**< Otsu method.  Maximizes the between-class variance.
                      *   Just one threshold value is computed for a window of
                      *   gray values specified through the
                      *   parameters::foreground.  For the floating point input
                      *   channels, the given window represents the section of
                      *   the histogram to be considered, which represent a
                      *   diferent range of gray values for each image as the
                      *   histogram is computed for the interval of actual
                      *   values found in the images, i.e.  the bin with index
                      *   0 represent the smallest value in the channel and the
                      *   last histogram bin represents the highest values
                      *   present in the channel.  All values below the
                      *   threshold are considered background and the rest
                      *   foreground.
                      */

      Simple, /**< Simple means-based adaptive method.  This method is
               *   explained in the books of Sonka et.al. and in
               *   Gonzalez-Woods, based on the average of the mean values for
               *   each class.
               */

      SimpleInterval, /**< Simple means-based adaptive method.  This method is
                       *   explained in the books of Sonka et.al. and in
                       *   Gonzalez-Woods, based on the average of the mean
                       *   values for each class.  The threshold value is
                       *   computed for a window of gray values specified
                       *   through the parameters::foreground.  For the
                       *   floating point input channels, the given window
                       *   represents the section of the histogram to be
                       *   considered, which represent a diferent range of gray
                       *   values for each image as the histogram is computed
                       *   for the interval of actual values found in the
                       *   images, i.e.  the bin with index 0 represent the
                       *   smallest value in the channel and the last histogram
                       *   bin represents the highest values present in the
                       *   channel.  All values below the threshold are
                       *   considered background and the rest foreground.
                       */

      Relative  /**< The threshold values represent the percentage of pixels
                 *   rather than intensities.  For instance, if the interval
                 *   parameters::foreground contains [0.5,0.9] then the
                 *   lowest threshold is adjusted so that 50% of
                 *   all pixels, the ones with the lowest intensities will
                 *   fall into the background, and the highest threshold
                 *   will be chosed such that 10% of the pixels have
                 *   higher intensity values than the finally chosen threshold.
                 */
    };

    /**
     * The parameters for the class thresholding
     */
    class parameters : public segmentation::parameters {
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
       * Normalized foreground value on the end result.
       *
       * For lti::channel (lti::matrix<float>) this value is directly used.
       * For lti::channel8 (lti::matrix<ubyte>) this value is multiplied
       * first by 255.
       *
       * Default value: 1.0
       */
      float foregroundValue;

      /**
       * Normalized background value on the end result.
       *
       * For lti::channel (lti::matrix<float>) this value is directly used.
       * For lti::channel8 (lti::matrix<ubyte>) this value is multiplied
       * first by 255.
       *
       * Default value: 0.0
       */
      float backgroundValue;

      /**
       * Normalized interval of the input image representing the foreground.
       *
       * If the \c Direct method is chosen, then the values of the input image
       * falling into this closed interval are considered part of the \b
       * foreground.  All values falling outside this interval are considered
       * \b background.
       *
       * The exact meaning of this interval depends on the method chosen: 
       * - thresholding::Direct : if the input value_type is ubyte, then the
       *   limits of the interval are computed by multiplying the values of
       *   this parameter times 255.  If the value_type of the input is float,
       *   then the values are used as is.
       * - thresholding::Relative : this represent the percentage of pixels
       *   that have to be under the chosen threshold.
       * - thresholding::Otsu : this parameter is ignored
       * - thresholding::OtsuInterval : this parameter represents
       *   which section of the histogram has to be considered for the 
       *   computation of the threshold.
       * - thresholding::Simple: this parameter is ignored
       * - thresholding::SimpleInterval : this parameter represents
       *   which section of the histogram has to be considered for the 
       *   computation of the threshold.
       *
       * Default value: [0.5f,1.0f]
       */
      finterval foreground;

      /**
       * Method used for thresholding.
       *
       * You can choose between a direct, non-adaptive method, or one of
       * the adaptive methods.
       *
       * @see eMethod
       *
       * Default value: Direct
       */
      eMethod method;

      /**
       * Keep the original background pixels (if set to \c true), or
       * replace the background pixels by the backgroundValue.
       *
       * Default value: false
       */
      bool keepBackground;

      /**
       * Keep the original foreground pixels (if set to \c true), or
       * replace the foreground pixels by the foregroundValue.
       *
       * Default value: false
       */
      bool keepForeground;

      /**
       * Number of histogram bins (or entries).
       *
       * This value is only used while creating the histograms for float valued
       * channels.  If the analyzed structure is a channel8, (or
       * lti::matrix<ubyte>), then the histogram will always use 256 entries.
       * 
       * Default value: 1024
       */
      int histogramBins;

      /**
       * Minimum changed used to stop adaption in adaptive methods.
       *
       * The Simple method stops, when the new computed threshold change is 
       * less than or equal to this value, in histogram steps.
       *
       * Default value: 1
       */
      int deltaT;
      
    };

    /**
     * Default constructor
     */
    thresholding();

    /**
     * Construct a functor using the given parameters
     */
    thresholding(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    thresholding(const thresholding& other);

    /**
     * Destructor
     */
    virtual ~thresholding();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    thresholding& copy(const thresholding& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    thresholding& operator=(const thresholding& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual thresholding* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual thresholding* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    virtual bool updateParameters();

    /**
     * Threshold the \a src channel and leave the result on \a dest.
     *
     * @param src matrix with the source data.
     * @param dest matrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src, matrix<ubyte>& dest) const;

    /**
     * Threshold the \a src channel and leave the result on \a dest.
     *
     * @param src matrix with the source data.
     * @param dest matrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src, matrix<ubyte>& dest) const;

    /**
     * Threshold the \a src channel and leave the result on \a dest.
     *
     * @param src matrix with the source data.
     * @param dest matrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src, matrix<float>& dest) const;

    /**
     * Apply the thresholding segmentation to the intensity channel
     *
     * @param src image with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src, imatrix& dest) const;

  protected:
    /**
     * Compute the histogram of the given matrix<ubyte>
     */
    bool computeHistogram(const matrix<ubyte>& in,
                          ivector& hist) const;

    /**
     * Compute the histogram of the given matrix<float>
     */
    bool computeHistogram(const matrix<float>& in,
                          ivector& hist,
                          float& minVal,
                          float& maxVal) const;


    /**
     * Threshold the \a src channel and leave the result on \a dest.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param fg foreground interval
     * @return true if apply successful or false otherwise.
     */
    bool direct(const matrix<ubyte>& src, 
                      matrix<ubyte>& dest,
                const iinterval& fg) const;

    /**
     * Threshold the \a src channel and leave the result on \a dest.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param fg foreground interval
     * @return true if apply successful or false otherwise.
     */
    bool direct(const matrix<float>& src,
                      matrix<ubyte>& dest,
                const finterval& fg) const;

    /**
     * Threshold the \a src channel and leave the result on \a dest.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param fg foreground interval
     * @return true if apply successful or false otherwise.
     */
    bool direct(const matrix<float>& src, 
                      matrix<float>& dest,
                const finterval& fg) const;


    /**
     * Given the histogram, compute the between-class variance if
     * the threshold is set to k
     */
    bool otsu(const ivector& hist,int& kstar) const;

    /**
     * Given the histogram, compute the between-class variance if
     * the threshold is set to k
     */
    bool simple(const ivector& hist,int& k) const;

    /**
     * Compute relative thresholds
     */
    bool relative(const matrix<float>& src,
                  finterval& fg) const;

    /**
     * Compute relative thresholds
     */
    bool relative(const matrix<ubyte>& src,
                  iinterval& fg) const;

    
    /**
     * Shadow for foreground interval
     */
    finterval foreground_;

    /**
     * Shadow for method
     */
    eMethod method_;

    /**
     * Shadow for histogram bins
     */
    int bins_;

    /**
     * Shadow delta threshold
     */
    int deltaT_;
  };

  /**
   * Read function for thresholding::eMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,thresholding::eMethod& data);

  /**
   * Write function for thresholding::eMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const thresholding::eMethod& data);

  
}

#endif

