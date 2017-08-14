/*
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
 * \file   ltiOpticalFlowPainter.h
 *         Contains the class lti::opticalFlowPainter,
 *         which has still to be documented by Pablo Alvarado
 *
 * \author Bernd Mussmann
 * \date   16.08.2000
 *
 * \author Suat Akyol
 * \date   16.08.2000
 *
 * \author Pablo Alvarado
 * \date   29.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlowPainter.h,v 1.5 2012-10-08 02:54:12 alvarado Exp $
 */

#ifndef _LTI_OPTICAL_FLOW_PAINTER_H_
#define _LTI_OPTICAL_FLOW_PAINTER_H_

#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiImage.h"

namespace lti {

  /**
   * Class opticalFlowPainter
   *
   * Paints a 2D vector field on an image.  The vector field is given by
   * two matrices, containing the x and y components of the vectors.  The
   * output is painted on an image.
   *
   * There are two possible modes for the output vector field representation:
   * an array of arrows, or a color encoded vector field as proposed in:
   *
   * Baker, S., Scharstein, D., Lewis, J.P., Roth, S., Black, M.J. and
   * Szeliski, R. "A Database and Evaluation Methodology for Optical Flow".
   * International Journal of Computer Vision. 92: 1-31, 2011
   *
   * The selection between both modes is controled by
   * opticalFlowPainter::parameters::vectorMode
   *
   * The color encoding code is based on the colorcode.cpp file authored by
   * David Scharstein.
   *
   * @see opticalFlowPainter::parameters.
   *
   * @ingroup gVisualization
   * @ingroup gOpticalFlow
   */
  class opticalFlowPainter : public functor {
  public:
    /**
     * Enumeration of overlay modes
     *
     * The overlay mode indicates what to use as background of the
     * vector field.
     */
    enum eOverlayMode {
      NoOverlay,  /*!< draws vector field on colored background */
      OnOutput,   /*!< draws vector field on given output picture, if it
                       has the proper size.  Otherwise equivalent to NoOverlay*/
      OnMagnitude,/*!< draws vector field on its magnitude  */
      OnPhase     /*!< draws vector field on its phase */
    };

    /**
     * Enumeration for vector display mode
     */
    enum eVectorMode {
      Arrows,     /*!< draws flow vectors as arrows*/
      Lines,      /*!< draws flow vectors as lines */
      Color       /*!< Use phase and magnitude encoded in hue and saturation
                    and intesity.  This mode disables the overlay selection */
    };

    /**
     * Enumeration for vector scaling mode
     */
    enum eScalingMode {
      NoScaling,  /*!< vectors will not be scaled in any way                */
      GridWidth,  /*!< longest vector will be scaled to gridWidth           */
      RecallMax   /*!< as GridWidth, but remember the max value among calls */
    };

    /**
     * The parameters for the class opticalFlowPainter
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
       * Zoom factor
       *
       * Tells, how much larger the vector field should be in comparison to the
       * input channel(s).
       *
       * Default value: 1
       */
      int zoom;

      /**
       * Input channels are sampled by this factor to paint each vector.
       *
       * Default value: 5
       */
      int samplingRate;

      /**
       * If the magnitude of the current sampled point is smaller than the
       * given fraction of the maximum value, the vector is not drawn.
       *
       * The valid value range is therefore from 0.0 to 1.0
       *
       * Default value: 0.1
       */
      float threshold;

      /**
       * Overlay mode
       *
       * Controls what should be used as background of the resulting image.
       *
       * Default value: NoOverlay
       *
       * @see eOverlayMode
       */
      eOverlayMode overlayMode;

      /**
       * Graphics mode for the vectors
       *
       * Controls if the vectors should be painted as arrows or as lines, or
       * using the color encoding proposed by David Scharstein.
       *
       * Default value: Arrows
       *
       * @see eVectorMode;
       */
      eVectorMode vectorMode;

      /**
       * Scaling mode
       *
       * Controls how the magnitude of the vectors should be scaled.
       *
       * Default value: GridWidth
       *
       * @see eScalingMode
       */
      eScalingMode scalingMode;

      /**
       * Color of flow line.
       *
       * Default value: lti::White
       */
      rgbaPixel lineColor;

      /**
       * Color of flow line tip.
       *
       * Default value: lti::White
       */
      rgbaPixel tipColor;

      /**
       * Background color
       *
       * Default value: lti::Black
       */
      rgbaPixel backgroundColor;
    };

    /**
     * Default constructor
     */
    opticalFlowPainter();

    /**
     * Construct a functor using the given parameters
     */
    opticalFlowPainter(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    opticalFlowPainter(const opticalFlowPainter& other);

    /**
     * Destructor
     */
    virtual ~opticalFlowPainter();

    /**
     * Paint the vector field with x and y components on the given canvas.
     * @see eOverlayMode
     *
     * @param x x component of the vector field
     * @param y y component of the vector field
     * @param canvas image where the vector field will be drawn
     * @return true if apply successful or false otherwise.
     */
    bool apply(const fmatrix& x,const fmatrix& y,image& canvas) const;

    /**
     * Paint the vector field with x and y components on the given canvas.
     * @see eOverlayMode
     *
     * @param x x component of the vector field
     * @param y y component of the vector field
     * @param canvas image where the vector field will be drawn
     * @param maxMag maximum magnitude of the speed vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const fmatrix& x,const fmatrix& y,
               image& canvas,float& maxMag) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowPainter& copy(const opticalFlowPainter& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowPainter& operator=(const opticalFlowPainter& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual opticalFlowPainter* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual opticalFlowPainter* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    virtual bool updateParameters();

    /**
     * Paint color chart.
     *
     * This method just copies the color chart being used.
     */
    void colorChart(image& chart) const;

    /**
     * Paint color chart
     *
     * Additionally to the other method, this one paints in black
     * the axes.
     */
    void colorChart(image& chart, const float maxMagnitude) const;



  private:

    /**
     * Paint color chart
     *
     * Renders the color chart
     */
    void renderColorChart(image& chart) const;

    /**
     * Color Flow encoding
     */
    bool colorFlow(const fmatrix& x,const fmatrix& y,
                   image& canvas,float& maxMag) const;

    /**
     * Shadow for the color chart when needed
     */
    image chart_;

    /**
     * Chart side
     */
    enum {
      ChartSide=511
    };

    /**
     * The maximum magnitude until now of the vector
     */
    mutable float maxMag_;

    /**
     * Color reference palette (angular) as proposed by David Scharstein
     */
    static palette colorWheel_;

    /**
     * Initialize the color wheel
     */
    void initColorWheel();
  };

  /**
   * Read a opticalFlowPainter::eOverlayMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            opticalFlowPainter::eOverlayMode& data);

  /**
   * Write a opticalFlowPainter::eOverlayMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const opticalFlowPainter::eOverlayMode& data);

  /**
   * Read a opticalFlowPainter::eVectorMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            opticalFlowPainter::eVectorMode& data);

  /**
   * Write a opticalFlowPainter::eVectorMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const opticalFlowPainter::eVectorMode& data);

  /**
   * Read a opticalFlowPainter::eScalingMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            opticalFlowPainter::eScalingMode& data);

  /**
   * Write a opticalFlowPainter::eScalingMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const opticalFlowPainter::eScalingMode& data);


}

#endif

