/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009, 2010
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
 * \file   ltiViewer1DPainter.h
 *         Contains the functor that the viewer1D uses to paint the data.
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   06.04.2006
 *
 * revisions ..: $Id: ltiViewer1DPainter.h,v 1.3 2012-07-01 00:36:13 alvarado Exp $
 */

#ifndef _LTI_VIEWER_1_D_PAINTER_H_
#define _LTI_VIEWER_1_D_PAINTER_H_

#include "ltiVector.h"
#include "ltiDrawBase.h"
#include "ltiFunctor.h"

#include "ltiImage.h"
#include "ltiDraw.h"
#include "ltiColors.h"

namespace lti {

  class histogramEqualization; // just not to include the header!

  /**
   * Class viewer1DPainter.
   *
   * Class used to paint vectors (1D data) as required by the lti::viewer1D.
   *
   * The apply() methods "render" a vector (or part of it) in an image that
   * will be visualized.  They are not "const" since the viewer1DPainter
   * administrates a cache for the statistics of the given data and some
   * precomputations, in order to allow the required speed in the viewer
   * classes.
   *
   * Therefore, the apply methods are faster if they are always invoqued with
   * the same source vector, as the statistics are computed only the first time
   * they are required.
   *
   * The "cache miss" is computed based on very simple heuristics: the
   * probability that two different images have exactly the same size and
   * located on the same memory block is relatively low, so that the signature
   * of the data is computed based on these "identifiers".  If you have an
   * application where you just fill always the same vector instance, then you
   * have to call invalidateCache() first, to ensure that the statistics will
   * be properly computed.
   *
   * @ingroup gVisualization
   */
  class viewer1DPainter : public functor {
  public:

    /**
     * Available styles for drawing the vector
     */
    enum eStyle {
      Dots,     /**< Just a single pixel at the value            */
      Boxes,    /**< Draw a box for each element                 */
      Steps,    /**< A short horizontal line (lids of the boxes) */
      Impulses, /**< One pixel vertical line per element         */
      Lines     /**< Connect the values with lines               */
    };

    /**
     * Parameters of the lti::viewer1DPainter class.
     */
    class parameters : virtual public parametersManager::parameters {
    public:

      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      virtual ~parameters();

      /**
       * Copy data of "other" parameters
       */
      parameters& copy(const parameters& other);

      /**
       * Copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const;


      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);
      
    public:
      //
      // Attributes
      //
      /**
       * @name Colors
       */
      //@{
      /**
       * Color of the background
       *
       * Default: Black
       */
      rgbaPixel backgroundColor;

      /**
       * Color of the x-axis if plotted
       *
       * Default: Grey75
       */
      rgbaPixel axisColor;

      /**
       * Color of the grid if plotted
       *
       * Default: Grey50
       */
      rgbaPixel gridColor;

      /**
       * Color of the lines used for plotting the data
       *
       * Default: White
       */
      rgbaPixel lineColor;

      /**
       * Color of the mean value line if plotted
       *
       * Default: Red
       */
      rgbaPixel meanColor;

      /**
       * Color of the markers if no specific color is specified.
       *
       * Default: Green
       */
      rgbaPixel markerColor;
      

      /**
       * Marker string used while in the Dot style.
       *
       * For a set of possible markers, please refer to
       * lti::drawBase::eMarkerType
       *
       * Default value: "."
       */
      std::string marker;
      //@}

      /**
       * @name General properties
       */
      //@{

      /**
       * Zoom factor.
       *
       * The zoom factor is simply a scale factor used on pixelsPerElement
       * (zoom.x) and vectorHeight (zoom.y). After multiplication with the
       * factor these properties are rounded to the nearest integer value > 1.
       *
       * Values greater than one indicate a zoom-in factor, values smaller
       * than one indicate a zoom-out factor.
       *
       * These values have to be positive and different than zero.
       *
       * When using this class directly it is recommended to set
       * pixelsPerElement and vectorHeight to the desired values directly
       * instead of using zoom. This parameter is useful for dynamic viewing,
       * though.
       *
       * Default value: (1.0,1.0) (no zoom)
       */
      fpoint zoom;

      /**
       * If true the horizontal or x-axis is drawn using axisColor if it is in
       * the displayed range. E.g. if the vector contains values in the range
       * [2, 4] and forceZero is false, the horizontal axis can't be shown.
       *
       * Default: true
       */
      bool drawHorizontalAxis;

      /**
       * If true a horizontal line is drawn at the mean value of the vector.
       *
       * Default: false
       */
      bool drawMeanValue;

      /**
       * If true a grid is drawn according to ticks.
       *
       * Default: false
       */
      bool drawGrid;

      /**
       * Ticks / gridlines in x and y direction. The outer frame is not drawn
       * since this is where the drawing area ends anyways. So a value of
       * (1,1) results in just a cross in the middle.
       *
       * You can set it to zero or less to disable the grid
       *
       * Default: (1,1)
       */
      ipoint ticks;

      /**
       * If true the zero value or x-axis is forced to be included in the
       * drawing. This might waste space but can be more informative.
       *
       * Default: false
       */
      bool forceZero;
      //@}

      /**
       * @name Element properties
       */
      //@{

      /**
       * Number of pixels used for each element in the vector. It is usually
       * advisable to use an odd number here.
       *
       * Default: 3
       */
      int pixelsPerElement;

      /**
       * Number of pixels used for the vector visualization in y direction.
       *
       * Default: 256
       */
      int vectorHeight;

      /**
       * Determines the eStyle used for displaying the vector data.
       *
       * Default: Boxes
       */
      eStyle style;
      //@}
    }; // end of parameters


    /**
     * Default constructor
     */
    viewer1DPainter();

    /**
     * Construct a functor using the given parameters
     */
    viewer1DPainter(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    viewer1DPainter(const viewer1DPainter& other);

    /**
     * Destructor
     */
    virtual ~viewer1DPainter();

    /**
     * @name Apply methods to paint the whole image
     */
    //@{
    /**
     * Draw a graph of the given data \p src into \p canvas
     *
     * @param src vector which contains the data to be visualized
     * @param canvas data is visualized here
     * @return true if apply successful or false otherwise.
     */
    template <class V>
    bool apply(const V& src, image& canvas);

    /**
     * Draw a graph of the given data \p src into \p canvas
     *
     * @param src vector which contains the data to be visualized
     * @param painter data is visualized with this
     * @return true if apply successful or false otherwise.
     */
    template <class V>
    bool apply(const V& src, drawBase<rgbaPixel>& painter);
    //@}

    /**
     * @name Apply methods to partially paint a vector
     *
     * The view port is considered to denote the a window in a larger canvas
     * where the src data is to be painted on.  Please note that, due to
     * the chosen zoom factor, such canvas may have a different size than the
     * input data itself.  The apply methods have to take care of all
     * conversions of coordinate systems, as the viewport denote the canvas
     * coordinates and not the src image coordinates.
     */
    //@{

    /** 
     * Generate a new image to the given viewport.
     *
     * @param src Original image
     *
     * @param viewport section of the destination image to be rendered.  The
     *  meaning of this rectangle depends on the zoom factor configured by the
     *  user.
     *
     * @param dest Canvas where the region will be painted.
     * 
     * @return true if the rendering was possible, false if not.
     */
    template<typename V>
    bool apply(const V& src,
               const irectangle& viewport,
                     image& dest);
    //@}

    /**
     * Ensure that the next apply called will force the computation of the
     * required statistics.
     */
    virtual void invalidateCache();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewer1DPainter& copy(const viewer1DPainter& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewer1DPainter& operator=(const viewer1DPainter& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual viewer1DPainter* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual viewer1DPainter* newInstance() const;

    /**
     * Set the zoom parameter to the value indicated.
     *
     * The painter can be used with GUI tools that take care of the zoom,
     * so it is handy to have a method that allows a direct change of the
     * parameters object to set only the zoom value.
     */
    void setZoom(const fpoint& zoomFactor);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Compute a correct user palette from the parameters
     */
    virtual bool updateParameters();


  protected:
    /**
     * Returns a read-write reference to the used parameters
     */
    parameters& getRWParameters();

    /**
     * @name Cached data statistics
     */
    //@{

    /**
     * Data computed for a channel and flags indicating if stats were already
     * computed
     */
    class cacheBlock {
    public:
      /**
       * Constructor 
       */
      cacheBlock();

      /**
       * Invalidate cache block
       */
      void invalidate();
      
      /**
       * Flag to indicate that the min-max statistics are up-to-date.
       */
      bool minMaxOk_;

      /**
       * Min value
       */
      double min_;

      /**
       * Max value
       */
      double max_;

      /**
       * Mean value
       */
      double mean_;
    };

    /**
     * A manager for all statistic blocks
     */
    class cacheManager {
    public:
      /**
       * Constructor
       */
      cacheManager();

      /**
       * Update the internal cache blocks if necessary
       *
       * @param vct the vector for which the statistics are required.  It
       *             has to be exactly of the same size than the original
       *             data, or the statistics will be wrong!
       *
       * @return false if nothing had to be updated, or true if changes were
       *         done.
       */
      template<class V>
      bool update(const V& vct);

      /**
       * Update the internal signature to the given matrix
       */
      template<class V>
      bool updateSignature(const V& data);

      /**
       * Verify if there will be a cache miss with the given data
       */
      template<class T>
      bool checkCacheMiss(const T* data,
                          const int& size) const;

      /**
       * Since not all vectors are lti::vectors, then we need to 
       * reimplement the search for min and max values
       */
      template<class V>
      void findExtremes(const V& vct,
                        typename V::value_type& theMin,
                        typename V::value_type& theMax,
                        double& theMean) const;
      
      /**
       * Invalidate the cache
       */
      void invalidate();

    protected:
      /**
       * @name Signature data
       */
      //@{
      /**
       * Pointer to the data block of the matrix.
       *
       * Note that this is the pointer to the data block itself, and NOT to the
       * container matrix, i.e. it will be a rgbaPixel*, int*, ubyte* or
       * float*.
       */
      const void* dataPtr_;

      /**
       * Size of the vector
       *
       * Even if dataPtr_ is almost perfect for the task, a second check cannot
       * hurt!
       */
      int dataSize_;
      //@}

    public:
      /**
       * All cached blocks
       */
      cacheBlock blocks;

    };

    /**
     * Cache instance
     */
    cacheManager cache_;

    /**
     * Flag to indicate that the parameters may have changed
     * (the user called in the mean time setParameters(), useParameters(), etc.
     */
    bool haveNewParameters_;

    //@}

    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest.
     */
    template <class V>
    bool render(const V& src,
                const irectangle& viewPort,
                drawBase<rgbaPixel>& painter);


    /**
     * The actual (zoomed) vectorHeight
     */
    int vectorHeight_;

    /**
     * The actual (zoomed) pixelsPerElement
     */
    int pixelsPerElement_;
  };

  /**
   * Read a viewer1DPainter::eStyle
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            viewer1DPainter::eStyle& data);

  /**
   * Write a viewer1DPainter::eStyle
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const viewer1DPainter::eStyle& data);

  
}

#include "ltiViewer1DPainter_template.h"

#endif

