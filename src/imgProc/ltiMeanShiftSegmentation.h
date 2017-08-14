/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiMeanShiftSegmentation.h
 *         Contains the class lti::meanShiftSegmentation, 
 *         which implements the well known image segmentation algorithm.
 * \author Axel Berner
 * \author Jens Rietzschel
 * \author Pablo Alvarado
 * \date   22.6.2001
 *
 * revisions ..: $Id: ltiMeanShiftSegmentation.h,v 1.2 2009-09-24 14:37:41 alvarado Exp $
 */

#ifndef _LTI_MEAN_SHIFT_SEGMENTATION_H_
#define _LTI_MEAN_SHIFT_SEGMENTATION_H_

#include "ltiSegmentation.h"
#include "ltiImage.h"
#include "ltiChannel8.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Mean Shift Segmentation Algorithm.
   *
   * This is the implementation of the mean shift segmentation algorithm.
   * It is described in
   *
   * D. Comaniciu, P. Meer,
   * "Mean Shift: A Robust Approach toward Feature Space Analysis"
   * appeared in IEEE Trans. Pattern Analysis Machine Intell. Vol. 24, No. 5,
   * 603-619, 2002
   * (original link <a href="http://www.caip.rutgers.edu/~comanici/">
   * here </a>)
   *
   * http://www.caip.rutgers.edu/~comanici/Papers/MsRobustApproach.pdf
   *
   * The code here is based on the code of the original authors, modified
   * extensively for the use with the LTI-Lib.  It has changed a lot since
   * then to provide more functionality and configuration options.
   *
   * The present implementation is an adaption of Comaniciu and Meer's EDISON
   * code (http://www.caip.rutgers.edu/riul/research/robust.html).  Decisive
   * for the result of the new implementation is the choice of the parameters
   * parameters::sigmaR, parameters::sigmaS,
   * parameters::maxNeighbourColorDistance, and the parameters::speedup.
   *
   * @ingroup gSegmentation
   *
   * @see meanShiftSegmentation::parameters
   */
  class meanShiftSegmentation : public segmentation {
  public:
    /**
     * Three types of speed-up techniques
     */
    enum eSpeedUpType{
      NoSpeedup,     /**< Filters the image applying mean shift to each
                      *   point.
                      *
                      *   Advantage: most accurate
                      *
                      *   Disadvantage : time expensive
                      */
      MediumSpeedup, /**< Filters the image using previous mode information
                      *   to avoid re-applying mean shift to some data
                      *   points (is default)
                      *
                      *   Advantage: maintains high level of accuracy,
                      *              large speed up compared to
                      *              non-optimized version
                      *
                      *   Disadvantage: possibly not as accurate as
                      *                 non-optimized version
                      */
      HighSpeedup    /**< Filters the image using previous mode information
                      *   and window traversals to avoid re-applying mean
                      *   shift to some data points
                      *
                      *   Advantage: huge speed up  maintains accuracy good
                      *              enough for segmentation
                      *
                      *   Disadvantage: not as accurate as previous filters
                      */
    };

    /**
     * The parameters for the class meanShiftSegmentation
     */
    class parameters : public segmentation::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
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
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
       * Maximal number of trials for choosing a valid representing color.
       *
       * Default: 10
       */
      int maxTrial;

      /**
       * Using the  multivariate normal kernel yields better results
       * but is more computational intensive
       *
       * Default: false
       */
      bool multivariateNormalKernel;

      /**
       * Higher speedup level causes loss of acuracy
       *
       * Default: MediumSpeedup
       */
      eSpeedUpType speedup;

      /**
       * The spatial radius of the mean shift sphere.
       * (the radius in grid space)
       *
       * Higher values cause longer computation times, and smoother region
       * boundaries.
       *
       * Default value: 5
       */
      double sigmaS;

      /**
       * The range radius of the mean shift sphere.
       * (the radius in color space)
       *
       * Higher values result in less regions
       *
       * Default value: 5
       */
      double sigmaR;

      /**
       * Regions having a color difference less than this parameter are joined
       * together (by the method fuseRegions()).
       *
       * It should be smaller than sigmaR.
       *
       * Default value: 3
       */
      double maxNeighbourColorDistance;

      /**
       * Set the minSize (in Pixel) a region must have.
       *
       * Default value: 15
       */
      int minRegionSize;

      /**
       * If the magnitude of the mean-shift vector is under this
       * threshold, it is considered as converged.
       *
       * Default value: 0.1
       */
      double thresholdConverged;
    };

    /**
     * Default constructor
     */
    meanShiftSegmentation();

    /**
     * Default constructor
     */
    meanShiftSegmentation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    meanShiftSegmentation(const meanShiftSegmentation& other);

    /**
     * Destructor
     */
    virtual ~meanShiftSegmentation();

    /**
     * Returns the name of this type ("meanShiftSegmentation")
     */
    virtual const std::string& name() const;

    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param dest image where the result (segmented image) will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src, image& dest) const;

    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param dest imatrix where the result (region-labels) will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,imatrix& dest) const;

    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param labels imatrix where the result (region-labels) will be left.
     * @param colorMap the color palette used
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               imatrix& labels,
               palette& colorMap) const;

    /**
     * Apply the mean shift segmentation algorithm to a color image, which
     * has been already split into three channels.  The algorithm will work
     * with these channels without converting them into the Luv color space.
     * This allows the use of the mean shift segmentation in other color
     * spaces than Luv.
     *
     * @param chnl1 first channel of the source color image.
     * @param chnl2 second channel of the source color image.
     * @param chnl3 third channel of the source color image.
     * @param dest imatrix where the result (region-labels) will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& chnl1,
               const channel8& chnl2,
               const channel8& chnl3,
               imatrix& dest) const;


    /**
     * Apply the mean shift segmentation algorithm to the given image.
     *
     * This image will be internally transformed into the Luv color space, as
     * proposed by Comaniciu et.al. in their original paper.
     *
     * @param src image with the source data.
     * @param destFiltered the meanshift filtered image (not
     *        available for classic algorithm)
     * @param destSegmented the segmented image
     * @param destLabels label associating each pixel to a
     *        mode and thus a color to each pixel
     * @param destColorMap the colors of the found modes
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
	       image& destFiltered,
	       image& destSegmented,
	       imatrix& destLabels,
	       palette& destColorMap) const;

    /**
     * Filter the image with meanshift algorithm
     *
     * @param src the image to be filtered
     * @param dest the result will be left here
     * @return true if apply successful or false otherwise.
     */
    bool filter(const image& src, image& dest) const;

    /**
     * Filter the image with meanshift algorithm
     *
     * @param srcdest the image to be filtered
     * @return true if apply successful or false otherwise.
     */
    bool filter(image& srcdest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    meanShiftSegmentation& copy(const meanShiftSegmentation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual meanShiftSegmentation* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual meanShiftSegmentation* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Like rgbaPixel for RGB-ColorSpace,
     * is luvPixel  for LUV-ColorSpace
     */
    struct luvPixel {
      float l,u,v;
    };

  private:

    //-----------------------------------------------------------------------
    // members used for classic algorithm - not used in new implementation
    //-----------------------------------------------------------------------
    /**
     * @name Classic implementation
     */
    //@{

    /**
     * Define region adjacency list class which has one entry for each
     * neighbour region of this (one) region the first entry represents the
     * region itself and not a neighbour
     */
    class regionAdjacencyList {
    public:

      /**
       * The labels of this region
       */
      int label;

      /**
       * The next neighbour of the considered region
       */
      regionAdjacencyList* next;

      /**
       * Constructor
       */
      regionAdjacencyList();

      /** 
       * Insert a region node into the region adjecency list.
       *
       * @return \c true indicates that the node was inserted, \c false
       * indicates that the node was not inserted because this neighbour region
       * is already in the list.
       */
      bool insert(regionAdjacencyList* entry);	

    private:

      /**
       * pointer to current and previous entry
       */
      regionAdjacencyList *cur_, *prev_;
    };

    /**
     * The following attributes are packed into a class, to make it
     * possible to keep the const attribute of the apply and filter methods.
     */
    class internals {
    public:
      /**
       * Constructor
       */
      internals();

      /**
       * Destructor
       */
      ~internals();

      /**
       * Width and height of original image
       */
      int width, height;
      
      /**
       * number of pixel of original image
       */
      int imageSize;
      
      /**
       * The range dimension is the number of dimensions of color space
       */
      int dimensionRange;
      
      /**
       * The spatial dimension is the number of dimensions of the image lattice
       */
      int dimensionSpace;
      
      /**
       * The dimension of the feature space =  dimensionRange + dimensionSpace
       */
      int dimensionFeatureSpace;
      
      /**
       * Represents the original image in luv space
       */
      float* imageLuvOrgF;
      
      /**
       * Represents the meanshift filtered image in luv space
       */
      float* imageLuvFilteredF;
      
      /** 
       * Represents the meanshift filtered image in luv space (rounded
       * integers)
       */
      int* imageLuvFilteredI;
      
      /**
       * Contains for each pixel a weight factor
       */
      float* weightMap;
      
      /**
       * Assigns a label to each data point associating it to a mode in modes
       * e.g. a data point having label i has mode modes[i]
       */
      int* labels;
      
      /**
       * Contains all found modes as luv values
       * (l1,u1,v1,l2,u2,v2,l3,u3,v3,...)
       */
      vector<float> modes;

      /**
       * Counts the points associated to each mode
       */
      vector<int> modePointCounts;
      
      /**
       * Number of found regions
       */
      int regionCount;
      
      /**
       * For debug and analysis information only: counts how many pixels
       * converged in one step, two steps etc...
       */
      vector<int> vecTrialsToConverge;

      /**
       * Used in fuseRegions()
       */
      regionAdjacencyList* raMatrix, *raPool;

      
      /**
       * Initialize members and allocate memory.
       *
       * This method assumes the height and weight are already set.
       *
       * @param pars Parameters
       * @param size Size of the image
       * @param dimRange dimension range
       * @param dimSpace dimension space
       */
      void initialize(const parameters& pars,
                      const ipoint& size,
                      const int dimRange,
                      const int dimSpace);

      /**
       * Free memory allocated by initialize()
       */
      void freeMemory();
    };


    /**
     * Filters the image applying mean shift to each point advantage: most
     * accurate disadvantage : time expensive
     */
    void nonOptimizedFilter(internals& data) const;

    /**
     * Filters the image using previous mode information to avoid re-applying
     * mean shift to some data points speedup depends on parameter speed
     */
    void optimizedFilter(internals& data) const;

    /**
     * Connect neighbouring pixels having the same color values to a region
     * by giving them the same label in labels_[]
     * -input: imageLuvFilteredI_
     * -output: labels_, modes_, regionCount_, modePointCounts_
     */
    void connect(internals& data) const;


    /**
     * Fuse regions with similar color (modes within sphere)
     * -input: labels_, modes_, regionCount_, modePointCounts_
     * -output: labels_, modes_, regionCount_, modePointCounts_
     */
    void fuseRegions(internals& data) const;

    /**
     * Prune regions that have less than minRegionSize pixels
     * -input: labels_, modes_, regionCount_, modePointCounts_
     * -output: labels, modes, regionCount_, modePointCounts_
     */
    void pruneRegions(internals& data) const;

    /**
     * Build an array of regionAdjacencyLists (raMatrix_) containing all
     * neighbour regions for each region (used in fuseRegions())
     * -input: labels_, regionCount_
     * -output: raMatrix_
     */
    void buildRegionAdjacencyMatrix(internals& data) const;

    /**
     * Convert image from rgb (src) to luv (imageLuvOrgF_)
     */
    void rgbToLuv(const image& src,internals& data) const;

    /**
     * Convert image from luv (src) to rgb (dest)
     */
    void luvToRgb(float* src, image& dest,internals& data) const;

    /**
     * Convert one luv pixel(src) to one rgb pixel(dest)
     */
    void luvToRgb(luvPixel src, rgbaPixel& dest) const;

  };

  /**
   * Read a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            meanShiftSegmentation::eSpeedUpType& data);

  /**
   * Write a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const meanShiftSegmentation::eSpeedUpType& data);
  


}

#endif
