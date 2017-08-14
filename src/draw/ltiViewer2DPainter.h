/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/** 
 * \file   ltiViewer2DPainter.h
 *         Contains the functor that the viewer2D uses to paint the data.
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewer2DPainter.h,v 1.14 2010-01-02 04:35:18 alvarado Exp $
 */

#ifndef _LTI_VIEWER_2_D_PAINTER_H_
#define _LTI_VIEWER_2_D_PAINTER_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiRGBPixel.h"
#include "ltiSplitImageToHSI.h"
#include "ltiMergeHSIToImage.h"
#include "ltiSplitImageToRGB.h"
#include "ltiMergeRGBToImage.h"


namespace lti {

  class histogramEqualization; // just not to include the header!

  /**
   * Class viewer2DPainter.
   *
   * Class used to paint the 2D data as required by the lti::viewer2D.
   *
   * The apply() methods "render" an image corresponding to a window of the
   * data to be visualized.  They are not "const" since the viewer2DPainter
   * administrates a cache for the statistics of the given images and some
   * precomputations, in order to allow the required speed in the viewer
   * classes.
   *
   * Therefore, the apply methods are faster if they are always invoqued with
   * the same source image, as the statistics are computed only the first time
   * they are required.
   *
   * The "cache miss" is computed based on very simple heuristics: the
   * probability that two different images have exactly the same size and
   * located on the same memory block is relatively low, so that the signature
   * of the data is computed based on these "identifiers".  If you have an
   * application where you just fill always the same lti::matrix (or
   * derived) instance, then you have to call invalidateCache() first, to
   * ensure that the statistics will be properly computed.
   *
   * @ingroup gVisualization
   */
  class viewer2DPainter : public functor {
  public:

    /**
     * Enumeration used to indicate the type of gray-scale/brightness mapping
     * used.
     */
    enum eMappingType {
      Original,    /**< Original image, without modifications */
      OffsetSlope, /**< Use a linear mapping with the given Offset and Slope */
      MinToBlack,  /**< Map always the minimum value to black */
      MaxToWhite,  /**< Map always the maximum value to white */
      Optimal,     /**< Map the minimum to black and maximum to white */
      HistogramEq, /**< Equalize the brightness histogram */
      Log,         /**< Logarithmic curve log(x+1)/log(256) where x is assumed
                      to be between 0 a 255.  For channels it is used x*255.*/
      OptimalLog,  /**< Like log, but an Optimal map is applied first */
      UserDefMap   /**< Use a user given LUT */
    };
    
    /**
     * Enumeration used to indicate which palette should be used when
     * displaying channels, or on which channel should be applied the mapping
     * if showing color images.
     */
    enum ePaletteType {
      GrayPalette,  /**< Gray palette or intensity channel */
      RedPalette,   /**< Red palette or red channel */
      GreenPalette, /**< Green palette or green channel */
      BluePalette,  /**< Blue palette or blue channel */
      HuePalette,   /**< Hue palette or hue channel*/
      UserDefPalette /**< A palette defined by the user or all RGB channels*/
    };

    /**
     * Enumeration type used to indicate the type of neighborhood used in
     * the adjacency analysis.
     */
    enum eNeighborhoodType {
      FourNeighborhood,  /**< 4-Neighborhood */
      EightNeighborhood  /**< 8-Neighborhood */
    };

    /**
     * Limit for the biggest allowable label in a channel32 for the
     * label adjacency analysis.
     */
    enum {
      BiggestLabel = 307200  /**< assume that a mask of 640x480 has 1 label per
                              *   pixel, which would produce in the worst case
                              *   LUT of 1.2MB. 
                              */
    };

    /**
     * Parameters of the lti::viewer2DPainter class.
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
       * Zoom factor.
       *
       * The zoom functor is simply a scale factor used for each pixel.
       * You can interpret it as the number of pixels used per pixel.
       *
       * Values greater than one indicate a zoom-in factor, where the value
       * indicates the number of displayed pixels for each image pixel.
       *
       * Values smaller than one indicate a zoom-out factor, where only each
       * 1/zoom pixel is shown.
       *
       * This values have to be positive and different than zero.
       *
       * Default value: 1.0
       */
      float zoom;

      /**
       * Type of mapping used for the intensity of the data.
       *
       * Default value: Original
       */
      eMappingType mappingType;

      /**
       * Offset used if the linear mapping is chosen.
       *
       * This is the \a b value in a \a mx+b equation.
       *
       * The value given here is normalized, which means that a value of 1
       * indicates an offset of the whole value range (1.0 for lti::channel and
       * 255 for channel8 or images).  In other words, you want here usually
       * values between -1.0 and 1.0.
       *
       * Default value: 0
       */
      float offset;

      /**
       * Slope used if the linear mapping is chosen.
       *
       * This is the \a m value in a \a mx+b equation.
       *
       * Default value: 1
       */
      float slope;

      /**
       * User defined curve, given as a LUT
       *
       * Default value: a linear mapping
       */
      vector<ubyte> userMapping;

      /**
       * Should the data in the status bar be displayed in hexadecimal or
       * in decimal notation.
       *
       * Default value: false
       */ 
      bool hexadecimal;

      /**
       * Color used for an underflow.
       *
       * The linear maps used can produce data outside the expected ranges.  If
       * the values are above the highest valid value, this color will be used
       * instead.
       *
       * Default value: Blue
       */
      rgbaPixel underflowColor;

      /**
       * Color used for an overflow
       *
       * The linear maps used can produce data outside the expected ranges.  If
       * the values are below the lowest valid value, this color will be used
       * instead.
       *
       * Default value: Red
       */
      rgbaPixel overflowColor;

      /**
       * Palette type to be used or channel to be mapped.
       *
       * If you 
       *
       * Default value: GrayPalette
       */
      ePaletteType paletteType;

      /**
       * Used defined color palette.  It is used as a palette for
       * representation of channels (ubyte, float and int), but also as
       * the palette for the label adjacency representations.
       *
       * Default value: labelAdjacencyMap::defaultPalette
       */
      palette userPalette;

      /**
       * Label adjacency analysis
       *
       * If true, then the channel8 or channel32 being displayed will be
       * assumed to be a labeled mask, and an adjacency analysis will be done
       * to improve the visualization of adjacent regions.
       *
       * Default value: false
       */
      bool labelAdjacency;

      /**
       * Neighborhood type for the adjacency analysis.
       *
       * Default value: FourNeighborhood
       */
      eNeighborhoodType adjacencyNeighborhood;

      /**
       * If labelAdjacency is true, this flag indicates if the number of 
       * used colors should be minimal (the first items in the userPalette) or
       * all colors in userPalette can be used.
       *
       * Default value: true;
       */
      bool minAdjacencyColors;
    }; // end of parameters


    /**
     * Default constructor
     */
    viewer2DPainter();

    /**
     * Construct a functor using the given parameters
     */
    viewer2DPainter(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    viewer2DPainter(const viewer2DPainter& other);

    /**
     * Destructor
     */
    virtual ~viewer2DPainter();

    /**
     * @name Apply methods to paint the whole image
     */
    //@{
    /**
     * Generate a new image considering all viewer parameterization and store
     * it in dest.
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<rgbaPixel>& src, image& dest);

    /**
     * Generate a new image considering all viewer parameterization and store
     * it in dest.
     *
     * @param src channel with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src, image& dest);

    /**
     * Generate a new image considering all viewer parameterization and store
     * it in dest.
     *
     * @param src channel with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src, image& dest);

    /**
     * Generate a new image considering all viewer parameterization and store
     * it in dest.
     *
     * @param src channel with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int32>& src, image& dest);
    //@}

    /**
     * @name Apply methods to partially paint an image.
     *
     * The view port is considered to denote the dimensions of an area in a
     * canvas where the src data is to be painted on.  Please note that, due to
     * the chosen zoom factor, such canvas may have a different size than the
     * input data itself.  The apply methods have to take care of all
     * conversions of coordinate systems, as the viewport denote the canvas
     * coordinates and not the src image coordinates.
     *
     * If you are overloading these methods, you may find the method 
     * analyzeViewport() useful.
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
    virtual bool apply(const matrix<rgbaPixel>& src,
                       const irectangle& viewport,
                       image& dest);

    /**
     * Generate a new image.
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
    virtual bool apply(const matrix<float>& src,
                       const irectangle& viewport,
                       image& dest);

    /**
     * Generate a new image.
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
    virtual bool apply(const matrix<ubyte>& src,
                       const irectangle& viewport,
                       image& dest);

    /**
     * Generate a new image.
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
    virtual bool apply(const matrix<int32>& src,
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
    viewer2DPainter& copy(const viewer2DPainter& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewer2DPainter& operator=(const viewer2DPainter& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual viewer2DPainter* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual viewer2DPainter* newInstance() const;

    /**
     * Set the zoom parameter to the value indicated.
     *
     * The painter can be used with GUI tools that take care of the zoom,
     * so it is handy to have a method that allows a direct change of the
     * parameters object to set only the zoom value.
     */
    void setZoom(const float zoomFactor);

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
     * Histogram equalization functor.
     *
     * It is initialized only at construction time
     */
    histogramEqualization* histEq_;

    /**
     * @name Splitters and mergers
     */
    //@{
    const splitImageToHSI splitHSI_;
    const mergeHSIToImage mergeHSI_;
    const splitImageToRGB splitRGB_;
    const mergeRGBToImage mergeRGB_;
    //@}

    /**
     * @name Palettes
     * 
     * The channel representations can be colored using one of the following
     * palettes
     */
    //@{
    /**
     * Initialize all palettes
     */
    void initPalettes();

    /**
     * Gray palette
     */
    static palette gray_;
    
    /**
     * palette
     */
    static palette red_;

    /**
     * palette
     */
    static palette green_;

    /**
     * palette
     */
    static palette blue_;

    /**
     * palette
     */
    static palette hue_;

    /**
     * user palette that depends on the parameters
     */
    palette user_;
    //@}

    /**
     * @name Cached data statistics
     */
    //@{

    // Anonymous constants
    enum eChannelType {
      IntensityCache = 0,
      HueCache       = 1,
      RedCache       = 2,
      GreenCache     = 3,
      BlueCache      = 4,
      NumCacheBlocks = 5
    };

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
       * Flag to indicate that the histogram equalization LUT are up-to-date.
       */
      bool histEqOk_;

      /**
       * Min value
       */
      float min_;

      /**
       * Max value
       */
      float max_;

      /**
       * LUT for histogram equalization
       */
      vector<float> lut_;
    };

    /**
     * A manager for all statistic blocks, computed (only if necessary) for
     * the red, green, blue, hue and intensity channels of the data.
     */
    class cacheManager {
    public:
      /**
       * Constructor
       */
      cacheManager(histogramEqualization*& histoEq);

      /**
       * Update the internal cache blocks if necessary
       *
       * @param data pointer to the data block from which chnl was extracted.
       * @param chnlType type of color channel being analyzed (red, hue, etc).
       * @param chnl the channel for which the statistics are required.  It
       *             has to be exactly of the same size than the original
       *             data, or the statistics will be wrong!
       * @param mapping the transformation desired is used to detect which
       *        statistics have to be computed.
       *
       * @return false if nothing had to be updated, or true if changes were
       *         done.
       */
      bool update(const void* data,
                  const eChannelType chnlType,
                  const matrix<float>& chnl,
                  const eMappingType mapping);

      /**
       * Update the internal cache blocks if necessary
       *
       * @param data pointer to the data block from which chnl was extracted.
       * @param chnlType type of color channel being analyzed (red, hue, etc).
       * @param chnl the channel for which the statistics are required.  It
       *             has to be exactly of the same size than the original
       *             data, or the statistics will be wrong!
       * @param mapping the transformation desired is used to detect which
       *        statistics have to be computed.
       *
       * @return false if nothing had to be updated, or true if changes were
       *         done.
       */
      bool update(const ubyte* data,
                  const eChannelType chnlType,
                  const matrix<ubyte>& chnl,
                  const eMappingType mapping);


      /**
       * Update the internal cache blocks if necessary
       *
       * @param data pointer to the data block from which chnl32 was extracted.
       * @param chnlType type of color channel being analyzed (red, hue, etc).
       * @param chnl the channel for which the statistics are required.  It
       *             has to be exactly of the same size than the original
       *             data, or the statistics will be wrong!
       * @param mapping the transformation desired is used to detect which
       *        statistics have to be computed.
       *
       * @return false if nothing had to be updated, or true if changes were
       *         done.
       */
      bool update(const int32* data,
                  const eChannelType chnlType,
                  const matrix<int32>& chnl,
                  const eMappingType mapping);

      /**
       * Update the internal signature to the given matrix
       */
      bool updateSignature(const matrix<ubyte>& data);

      /**
       * Update the internal signature to the given matrix
       */
      bool updateSignature(const matrix<int32>& data);

      /**
       * Verify if there will be a cache miss with the given data.
       *
       * Mapping is used to determine if the statistics have to be recomputed
       * or are not necessary at all.
       */
      bool checkCacheMiss(const void* data,
                          const eChannelType chnlType,
                          const ipoint& size,
                          const eMappingType mapping) const;

      /**
       * Verify if there will be a cache miss with the given data
       * of a channel8 or equivalent matrix.
       */
      bool checkCacheMiss(const ubyte* data,
                          const ipoint& size,
                          const int cacheIdx) const;

      /**
       * Verify if there will be a cache miss with the given data
       * of a channel32 or equivalent matrix.
       */
      bool checkCacheMiss(const int* data,
                          const ipoint& size,
                          const int cacheIdx) const;

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
       * Size of the matrix.
       *
       * Even if dataPtr_ is almost perfect for the task, a second check cannot
       * hurt!
       */
      ipoint dataSize_;
      //@}

      /**
       * histogram equalizer
       */
      histogramEqualization*& histoEq_;

    public:
      /**
       * All cached blocks
       */
      cacheBlock blocks_[NumCacheBlocks];

      enum {
        ClutSize = UserDefMap+5
      };

      /**
       * Cached palettes used by channel8 and channel32
       * The boolean is a flag to indicate that the palette has been
       * computed.
       *
       * The first (UserDefMap+1) entries are for normal display and the last
       * 4 are used by the label adjacency LUT
       */
      std::pair<ivector,bool> clut_[ClutSize];

      /**
       * Flag that indicates if a labeled channel32 has too many labels
       */
      bool tooManyLabels_;
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
     * Get the start and end coordinates of the viewport but in
     * the coordinate system of the original image.
     *
     * If the viewport area corresponds to a region that exceeds the boundaries
     * in the original image, then the returned startPx and endPx coordinates
     * will be truncated to not exceed the area between (0,0) and
     * dataSize-(1,1).
     * 
     * @param viewport section of the canvas that has to be rendered
     * @param dataSize size of the original data
     * @param zoom zoom factor
     * @param startPx coordinates of the upper-left point in the original
     *                data equivalent to the requested viewport.
     * @param endPx   coordinates of the bottom-write point in the
     *                original data (inclusive)
     */
    void analyzeViewport(const irectangle& viewport,
                         const ipoint& dataSize,
                         const float& zoom,
                         ipoint& startPx,
                         ipoint& endPx) const;

    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest.
     *
     * The dest image is always connected and owns its data.
     */
    bool render(const matrix<rgbaPixel>& src,
                const ipoint from,
                const ipoint to,
                image& dest);

    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest.
     *
     * The dest image is always connected and owns its data.
     */
    bool render(const matrix<float>& src,
                const ipoint from,
                const ipoint to,
                image& dest);


    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest.
     *
     * The dest image is always connected and owns its data.
     */
    bool render(const matrix<ubyte>& src,
                const ipoint from,
                const ipoint to,
                image& dest);

    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest, assuming
     * that src is a labeled mask
     *
     * The dest image is always connected and owns its data.
     */
    bool renderLabels(const matrix<ubyte>& src,
                      const ipoint from,
                      const ipoint to,
                      image& dest);


    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest, assuming
     * that src is a labeled mask
     *
     * The dest image is always connected and owns its data.
     */
    bool renderLabels(const matrix<int32>& src,
                      const ipoint from,
                      const ipoint to,
                      image& dest);

    /**
     * Changes pixelwise the appearance of src but only the pixels from
     * the point "from" to "to", and leaves the result in dest.
     *
     * The dest image is always connected and owns its data.
     */
    bool render(const matrix<int32>& src,
                const ipoint from,
                const ipoint to,
                image& dest);

    /**
     * Computes on-place the intensity mapping.
     *
     * It is assumed that the normal range is always from 0.0 to 1.0
     */
    bool mapping(matrix<float>& srcDest,
                 const eChannelType type) const;

    /**
     * Computes on-place the intensity mapping.
     *
     * It is assumed that the normal range is always from 0 to 255
     * The normal output will be in that range too.
     */
    bool mapping(matrix<int32>& srcDest,
                 const eChannelType type) const;

    /**
     * Computes a LUT for a channel8 (or similar)
     */
    bool computeC8LUT(ivector& lut) const;

    /**
     * Zoom in or out the given image by the given factor
     * leaving the result in dest and considering the "from" point that
     * may indicate a partial part of the pixels.
     *
     * @param src a reference to the source image, which is not const since
     *        it can be detached to the dest if it is possible.  It has to
     *        be connected!
     * @param viewport coordinates of the viewport
     * @param zoom zoom factor
     * @param dest destination image, which must have the desired size already!
     *
     * @return true if successful or false otherwise.
     */
    virtual bool zoom(image& src,
                      const irectangle& viewport,
                      const float zoom,
                      image& dest) const;



  };
  
}

#endif

