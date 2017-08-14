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
 * \file   ltiCWAGMSegmentation.h
 *         Segmentation method based on a graph merge approach.
 * \author Pablo Alvarado
 * \date   30.06.2003
 *
 * revisions ..: $Id: ltiCWAGMSegmentation.h,v 1.4 2009-09-24 14:37:41 alvarado Exp $
 */

#ifndef _LTI_CWAGM_SEGMENTATION_H_
#define _LTI_CWAGM_SEGMENTATION_H_

#include "ltiMacroSymbols.h"

#include "ltiSegmentation.h"
#include "ltiMedianFilter.h"
#include "ltiColorContrastGradient.h"
#include "ltiSplitImage.h"
#include "ltiRegionGraphMeans.h"
#include "ltiWatershedSegmentation.h"

#include <map>
#include <vector>

namespace lti {

  // ------------------------------------------------------------------------
  //
  // Color Watershed - Adjacency Graph Merge
  //
  // ------------------------------------------------------------------------


  /**
   * CWAGM Segmentation
   *
   * CWAGM means Color Watershed - Adjacency Graph Merge.  It is a
   * segmentation algortihms based on low level information introduced in
   * P. Alvarado "Segmentation of color images for interactive 3D object
   * retrieval".  Ph.D. Thesis. RWTH-Aachen, 2004.
   * http://www.bth.rwth-aachen.de/job/disslist.pl
   *
   * This watershed-based concept is based on the ideas of Haris et.al.
   * for a segmentation concept of gray-valued image, extended to color images.
   * It uses (optionally) a median filtering to reduce the noise in each color
   * channel, computes an over-segmentation with the watershed transformation
   * and merge the regions to minimize the mean square error of a piece-wise
   * constant image approximation.
   *
   * @see cwagmSegmentation::parameters
   *
   * @ingroup gSegmentation
   */
  class cwagmSegmentation : public segmentation {
  public:

    /**
     * The parameters for the class cwagmSegmentation
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
       * Returns name of this type
       */
      const std::string& name() const;

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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters
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
       * @name Preprocessing
       */
      //@{
      /**
       * A median filter with following parameters is used to denoise the image
       *
       * If you don't want any pre-smoothing, indicate a kernel size of 1 or
       * zero and nothing will be done.
       *
       * Default value: medianParam::kernelSize = 3;
       *                medianParam::boundaryType = Constant;
       */
      medianFilter::parameters medianParam;

      /**
       * Enhance contrast
       *
       * If set to \a true, the dynamic range of the channels will be
       * optimized.
       *
       * Since the RGB channels will be linearly expanded to map
       * min(R,G,B)-max(R,G,B) to the 0-255 range, the colors of the image
       * will not be severely altered.
       *
       * Default value: true
       */
      bool enhanceContrast;
      //@}

      /**
       * @name Split-Stage with Watersheds
       */
      //@{
      /**
       * The colorSplitter specifies the name of the color space splitting
       * functor used to extract the color components of the image, which are
       * employed to analyze the edges of the image.
       *
       * Default value: "XYZ"
       */
      std::string colorSplitter;

      /**
       * Parameters for the computations of the color contrast gradient
       *
       * The result is also the input for the watershed algorithm.
       */
      colorContrastGradient::parameters colorContrastParam;

      /**
       * Watershed transform parameters.
       *
       * Default: Rainfall method is selected as default and a 8-Neighborhood.
       *          The threshold is set to 255 to activate the automatic 
       *          threshold detection.
       */
      watershedSegmentation::parameters watershedParam;

      /**
       * To compute which threshold should be used in the watershed
       * segmentation, the accumulative histogram of gradient values will be
       * used as a probability distribution.  It will be assumed that the 
       * probability for a gradient value to be relevant must be greater 
       * than the given value.
       *
       * If you set a value close to zero, almost any gradient value will be
       * detected as edge, and an oversegmentation will be produced.
       *
       * If you set a value close to one, then only very strong gradient
       * values will be considered, causing an undersegmentation.
       * 
       * The automatic threshold computation will be done only if the user set
       * the watershedParam.threshold to 255.
       *
       * The value range must be between 0.0f and 1.0f
       * 
       * Default value: 0.45f
       */
      float minProbForWatershedThreshold;
      //@}
      
      /**
       * Merging parameters for the graph representation.
       *
       * Watersheds over-segment images.  The high number of regions detected
       * slows down the rest of the algorithms.  Therefore a first "merging"
       * stage is done, using an adaption of the distance measure suggested by
       * Haris et.al. to also use color images.  All adjacent regions with
       * distances under the given threshold will be merged.
       *
       * A mergeThreshold value close to zero means that only very similar
       * regions have to be merged, while a large value indicates to be much
       * more tolerant while merging and many regions will be merged.
       *
       * The threshold indicated within this parameter objects indicate a
       * square distance between mean values in the color space weighted by
       * the area of the regions, so that this value will be between 0 and
       * about 1000000, depending on the regions detected.  You will usually
       * give values between 0 and 2000
       *
       * Default: harisRegionMergeParam::mergeThreshold = 1
       *          harisRegionMergeParam::minRegionNumber = 10
       */
      regionGraphColorHaris::parameters harisRegionMergeParam;
    };

    /**
     * Default constructor
     */
    cwagmSegmentation();

    /**
     * Construct a functor using the given parameters
     */
    cwagmSegmentation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    cwagmSegmentation(const cwagmSegmentation& other);

    /**
     * Destructor
     */
    virtual ~cwagmSegmentation();

    /**
     * Returns the name of this type ("cwagmSegmentation")
     */
    virtual const std::string& name() const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               imatrix& regions) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @param regionSizes sizes of each region in the \a regions mask.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     imatrix& regions,
                     ivector& regionSizes) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param preproc preprocessed image.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @param regionSizes sizes of each region in the \a regions mask.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     image& preproc,
                     imatrix& regions,
                     ivector& regionSizes) const;


    /**
     * Segmentate the image \a src and return many internal partial results
     * that can be used in further processing units.
     *
     * Note that there are several segmentation modes (like color ACA or
     * mean-shift segmentation) which do not require the gradient computation.
     * These modes will leave the given \a gradMag and \a gradArg untouched.
     * This way, the gradients won't be unnecessarily computed and you can
     * detect if they are computed or not: just give empty channels and if
     * they are still empty after calling this method, you will have to
     * compute them, if you need to.
     *
     * @param src image with the source data.
     * @param preproc resulting preprocessed image.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @param regionSizes sizes of each region in the \a regions mask.
     * @param gradMag magnitude of the color gradient
     * @param gradArg orientation of the color gradient
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     image& preproc,
                     imatrix& regions,
                     ivector& regionSizes,
                     channel& gradMag,
                     channel& gradArg) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cwagmSegmentation& copy(const cwagmSegmentation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cwagmSegmentation& operator=(const cwagmSegmentation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual cwagmSegmentation* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual cwagmSegmentation* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor's parameters.
     *
     * Update the internal state of the functor according to the parameters.
     * This special case the functor used to split the image into its color
     * componentes is initialized here.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();
    
  protected:

    /**
     * Preprocess
     * Denoise and Enhance Contrast
     */
    bool preprocess(const image& src,image& dest) const;

    /**
     * Split and merge with watersheds.
     *
     * @param orig original color image
     * @param preproc preprocessed image
     * @param regions labeld mask with initial partition
     * @param regionSizes number of pixels per region
     * @param gradM magnitude of the color gradient
     * @param gradA orientation of the color gradient
     * @param gradHisto histogram of the gradient values
     * @param maxGrad maximum gradient value which is also the last value in
     *                the histogram.
     */
    bool worker(const image& orig,
                const image& preproc,
                imatrix& regions,
                ivector& regionSizes,
                channel& gradM,
                channel& gradA,
                vector<float>& gradHisto,
                float& maxGrad) const;

    /**
     * The simplest contrast enhancement consists in mapping the input value
     * range of each channel into 0 to 255, each channel independently of
     * the others.
     *
     * src MUST be connected.  (see matrix<T>::mode())
     */
    bool enhanceContrast(image& src) const;
   
    /**
     * @name local functors
     */
    //@{
    /**
     * Median filter
     */
    medianFilter median_;

    /**
     * Pointer to splitter object
     */
    splitImage* splitter_;

    /**
     * Color contrast gradient functor used in edge detection
     */
    colorContrastGradient colorContrast_;

    //@}

  };
}

#endif
