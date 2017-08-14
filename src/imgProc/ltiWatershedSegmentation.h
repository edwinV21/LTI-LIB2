/* Copyright (C) 2003, 2004, 2005, 2006
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
 * \file   ltiWatershedSegmentation.h
 *         Watershed segmentation.
 * 
 * \author Benjamin Winkler
 * \author Axel Berner
 * \date   11.1.2001
 *
 * revisions ..: $Id: ltiWatershedSegmentation.h,v 1.4 2013-02-09 21:56:47 alvarado Exp $
 */


#ifndef _LTI_WATERSHED_SEGMENTATION_H_
#define _LTI_WATERSHED_SEGMENTATION_H_

#include "ltiFunctor.h"
#include "ltiPointList.h"
#include "ltiChannel8.h"
#include "ltiImage.h"

#include <queue>
#include <vector>     // lti::vector doesn't work here
#include <list>

namespace lti {

  /**
   * Watershed segmentation of a channel8
   *
   * Algorithms: 
   *
   * Luc Vincent and Pierre Soille.
   * "Watersheds in Digital Spaces: An Efficient Algorithm Based on Immersion
   *  Simulations".
   * IEEE transactions on pattern analysis and machine intelligence,
   * vol. 13, No. 6, June 1991, pp. 583-598
   *
   * and
   * 
   * Patrick De Smet and Rui Luis V.P.M. Pires
   * "Implementation and analysis of an optimized rainfalling watershed
   *  algorithm".
   * IS\&T\/SPIE's 12th Annual Symposium Electronic Imaging 2000,
   * January 2000, pp. 759-766
   *
   * Watershed segmentation is a morphological operator used to segment gray
   * valued images, based on viewing the gray image as a topographical map.
   * Valleys will be flooded with water until the water surpasses the sheds
   * separating them.  At the contact lines the "watershed lines" are created
   * and constitute the limits between the image regions.
   * 
   * The topological map is usually a gradient map (the magnitude of the
   * gradient) that can be obtained with the lti::gradientFunctor or
   * with the lti::colorContrastGradient, using the ltior directly using the
   * lti::gradientKernelX and lti::gradientKernelY and the lti::convolution
   * operator.
   *
   * Two kinds of apply() methods are provided: 
   *
   * - Two methods returning a channel8 will compute only two values:
   *   watershed or the catchment basin.  The specific values for each class
   *   are indicated in the parameters object.  This is useful for visualization
   *   of the results.
   * 
   * - The apply() method returning a matrix<int> return a labeled mask, where
   *   each catchment basin get its own id.  This method is more typical for a
   *   segmentation algorithm.
   */
  class watershedSegmentation : public functor {
  public:
    /**
     * the parameters for the class watershedSegmentation
     */
    class parameters : public functor::parameters {
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
      virtual const std::string& name() const;

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
       * Write the parameters in the given ioHandler
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
       * Defines the neighborhood of a pixel.
       *
       * If set to \c false (default), only the pixels horizontally and
       * vertically bordering are considered to be neighbors.
       *
       * If set to \c true, the four diagonally adjoining pixels are also taken
       * into account.
       */
      bool neighborhood8;

      /**
       * Gray value to be used for watersheds in the resulting channel8.
       *
       * Default value: 255
       */
      int watershedValue;

      /**
       * Gray value to be used for basins in the resulting channel8.
       *
       * Default value: 0
       */
      int basinValue;

      /**
       * Rainfalling Watersheds or Standard-Waterfall
       *
       * Default: true (Rainfall-Watersheds, they are much faster)
       */
      bool rainfall;

      /**
       * Threshold to eliminate noise in the src-image.
       *
       * Default value: 0
       *
       * To avoid oversegmentation, try higher values (for example 4).
       * Another usual measure to reduce oversegmentation is also to denoise
       * the input image, using median-filters, SUSAN denoiser or mean-shift
       * denoisers, among many other possibilities.
       */
      ubyte threshold;
    };

    /**
     * Default constructor
     */
    watershedSegmentation();

    /**
     * Constructor with parameters
     */
    watershedSegmentation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    watershedSegmentation(const watershedSegmentation& other);

    /**
     * Destructor
     */
    virtual ~watershedSegmentation();

    /**
     * Returns the name of this type ("watershedSegmentation")
     */
    virtual const std::string& name() const;

    /**
     * Creates a watershed mask on the given channel8.
     *
     * @param srcdest channel8 with the source data.  This is usually the
     *        gradient of an intensity image or a color contrast gradient.  The
     *        resulting watershed lines will be left here too.
     * @result true if successful, false otherwise
     */
    bool apply(channel8& srcdest);

    /**
     * Saves a watershed mask of src in dest.
     *
     * @param src channel8 with the source data.  This is usually the gradient
     *            of an intensity image or a color contrast gradient.
     * @param dest channel8 where the watershed lines will be written.
     */
    bool apply(const channel8& src,channel8& dest);

    /**
     * Creates a region mask on the given matrix.
     *
     * Only exact watersheds are marked 0, regions are numbered 1,2,...
     * @param src channel8 with the source data.  This is usually the gradient
     *            of an intensity image or a color contrast gradient.
     * @param result matrix<int> with the resulting region information, with
     *               a label per basin.
     */
    bool apply(const channel8& src, matrix<int>& result);

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    watershedSegmentation& copy(const watershedSegmentation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual watershedSegmentation* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual watershedSegmentation* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:

    // for both --------------------------------------------------------
    /**
     * Fill neighborhood pointlist according to parameter neighborhood8.
     * if set to true, the full 8-neighborhood will be generated
     * otherwise, only the 4 horizontally and vertically neighboring pixels
     * will be taken into account.
     */
    void createNeighborhood(const int colms,const bool neigh8);

   /**
     * Converts the resulting matrix to the given channel8
     * by using watershedValue and basinValue
     */
    void copyMatrixToChannel8(const matrix<int>& src,
                                    channel8& dest);
    /**
     * List of neighborhood points (relative).  Is an 4 or 8 length vector
     * containing the offsets to get the neighbor pixels from the current one.
     */
    vector<int> neigh_;

    /**
     * Stores the size of the image in pixels
     */
    int imgSize_;

    /**
     * Border LUT contains 0 for all non-border pixel and 255 for
     * all border pixels.  It is used to accelerate detection of a border
     * position.
     */
    channel8 borderLUT_;

    /**
     * Return true if the given point corresponds to a valid neighbor point
     * of the given current point.
     */
    inline bool invalidNeighbor(const int currentPoint,
                                const int currentNeighbor) const {
      return (currentNeighbor<0 || currentNeighbor>=imgSize_ ||
              ((borderLUT_.elem(currentPoint)!=0) && 
               (abs((currentPoint%borderLUT_.columns()) - 
                    (currentNeighbor%borderLUT_.columns())) > 1)));
    }

    /**
     * Return true if the given point corresponds to a valid neighbor point
     * of the given current point
     */
    inline bool validNeighbor(const int currentPoint,
                              const int currentNeighbor) const {
      return (currentNeighbor>=0 && currentNeighbor<imgSize_ &&
              ((borderLUT_.elem(currentPoint)==0) || 
               (abs((currentPoint%borderLUT_.columns()) - 
                    (currentNeighbor%borderLUT_.columns())) <= 1)));
    }


  private:
    // rainfalling-------------------------------------------------------
    /**
     * Find for all points(if there is) a neigh, which have a lower level
     */
    void findLowerNeigh(const channel8& src,
      matrix<int>& downPos,
      channel8& thresSrc8);

    /**
     * Number serialy all minimas (lakes,or only points),
     */
    void markMinimas(const matrix<int>& downPos,
         const channel8& src,
         matrix<int>& dest);

    /**
     * Look in which minima a raindrop would flow
     */
    void letsRain(const matrix<int>& downPos, matrix<int>& dest);

  private:
    /**
     * Following type can be replaced for a faster list for small objects
     * in the near future
     */
    typedef std::list<int> list_type;

    // standard-----------------------------------------------------------

    /**
     * Initialize a border LUT to save time detecting if a pixel is in
     * the border.
     */
    void initBorderLUT(const ipoint& size,
                       channel8& borderLUT) const;

    /**
     * Creates a kind of histogram and stores all points, belonging to
     * a grayvalue in a list.
     *
     * It assumes that \a sortedPoints is empty!
     */
    void sortPixels(const channel8& src,
                    std::vector<list_type>& sortedPoints) const;
    
    /**
     * Set all new pixel (caused by waterlevel raising) MASK
     */
    void maskCurrLevelPoints  (const list_type& currentPointList,
                               vector<int>& distance,
                               std::queue<int>& fifoQueue,
                               matrix<int>& result) const;
    /**
     * Find out to which minima(lake) the MASK-pixel belog to
     */
    void assignCurrLevelPoints(vector<int>& distance,
                               std::queue<int>& fifoQueue,
                               matrix<int>& result) const;

    /**
     * Define all pixel, which are not assigned to a minima, as a new minima
     */
    void checkForMins          (const list_type& currentPointList,
                                vector<int>& distance,
                                std::queue<int>& fifoQueue,
                                matrix<int>& result,
                                int& currentLabel) const;
    /**
     * Raise the waterlevel, and look what happen
     */
    void raiseWaterLevel(const std::vector<list_type>& sortedPoints,
                         matrix<int>& result) const;

  };

}

#endif
