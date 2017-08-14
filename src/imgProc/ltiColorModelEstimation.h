/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
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
 * \file   ltiColorModelEstimation.h
 *         Contains the class lti::colorModelEstimation, which computes 3D
 *         color histograms.
 * \author Pablo Alvarado
 * \date   12.03.2002
 *
 * revisions ..: $Id: ltiColorModelEstimation.h,v 1.3 2009-09-19 00:17:39 alvarado Exp $
 */


#ifndef _LTI_COLOR_MODEL_ESTIMATOR_H_
#define _LTI_COLOR_MODEL_ESTIMATOR_H_

#include <list>
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

#include "ltiHistogram.h"
#include "ltiRGBAPixel.h"
#include "ltiRGBPixel.h"
#include "ltiInterval.h"

#include "ltiFunctor.h"

namespace lti {

  /**
   * Color model estimator.
   *
   * This functor is used to create three-dimensional histograms for
   * the colors of images.  The histograms are used as color models in
   * functors like lti::probabilityMap.
   *
   * It is possible to provide whole images (usefull to create histograms
   * of backgrounds, if they can be captured as a whole image), or
   * images with corresponding masks, that allow to consider only those
   * pixels with a mask value different than zero.
   *
   * It is also possible to provide directly a list of images.
   *
   * This functor generates also simple statistics for a multivariate gaussian
   * model for the color (mean color and covariance matrix).
   *
   * @ingroup gColor
   */
  class colorModelEstimation : public functor {
  public:
    /**
     * The parameters for the class colorModelEstimation
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
       * Dimensions for the 3D color model.
       *
       * The alpha channel will be ignored.
       *
       * Default: (32,32,32)
       */
      rgbaPixel histogramDimensions;

      /**
       * Mask ignore interval.
       *
       * When considering images using also a mask, only the values strictly
       * above the \c maskIgnore.to attribute in the mask will mark valid
       * pixels to be considered into the model.
       *
       * If you want to simultaneously compute a non-object or background
       * model, then only the pixels with a corresponding mask value stricktly
       * below the \c maskIgnore.from will be assumed to be part of that model.
       *
       * This allows you to create masks that indicate which pixels to consider
       * as part of the 'object', which pixels to consider as part of the
       * 'non-object' (or background) and which pixels should be ignored, all
       * at once.
       *
       * The default value is chosen such that a mask value of exactly 0 marks
       * the background, and everything greater than 0 is part of the object.
       *
       * Default value: [1,0]
       */
      iinterval maskIgnore;

    };

    /**
     * Default constructor.
     */
    colorModelEstimation();

    /**
     * Constructor with parameters
     */
    colorModelEstimation(const parameters& par);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    colorModelEstimation(const colorModelEstimation& other);

    /**
     * Destructor
     */
    virtual ~colorModelEstimation();

    /**
     * Returns the name of this type ("colorModelEstimation")
     */
    virtual const std::string& name() const;

    /**
     * Reset the internal histogram to consider new elements.
     * The internal model will be empty, but with the correct size.
     */
    bool reset();

    /**
     * @name Methods to add pixels into the statistics of one model
     */
    //@{

    /**
     * Consider a pixel into the model.
     *
     * @param pix pixel to be considered
     * @param times how many times should the pixel be considered
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const rgbaPixel& pix,const int times=1);

    /**
     * Consider a pixel for the model.
     *
     * This more time consuming \c consider method will not only add 1 to
     * the "winner" bin, but will also distribute the "1" vote using 
     * tri-linear interpolation among the eight neighbors.
     *
     * @param pix pixel to be considered
     * @param times how many times should the pixel be considered
     *
     * @return true if successful or false otherwise.
     */
    bool smoothConsider(const rgbaPixel& pix,const int times=1);

    /**
     * Consider a three dimensional pixel value given through its three
     * components.
     *
     * It is assumed that the value range of all channels is between 0.0 and
     * 1.0.
     *
     * @param c1 first color component.
     * @param c2 second color component.
     * @param c3 third color component.
     *
     * @param times number of times the given color point must be consider.
     */
    bool consider(const float& c1,const float& c2,const float& c3,
                  const int times=1);

    /**
     * Consider all colors in a palette into the model.
     *
     * @param pal the color palette to be considered.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const palette& pal);

    /**
     * Consider all pixels in the image into the model.
     *
     * @param img image with colors to be considered
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img);

    /**
     * Consider all pixels defined by the correspondent elements in the three
     * given channels.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3);

    /**
     * Consider all the images in the given list.
     *
     * @param imgs list of images to be considered
     * @return true if successful or false otherwise.
     */
    bool consider(const std::list<image>& imgs);
    //@}

    /**
     * @name Methods to include only pixels in a mask in the statistics
     */
    //@{

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size, otherwise \c false will be returned.
     *
     * @param img image with colors to be considered
     * @param mask channel8 with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const channel8& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size, otherwise \c false will be returned.
     *
     * @param img image with colors to be considered
     * @param mask imatrix with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const imatrix& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size, otherwise \c false will be returned.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @param mask channel8 with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3,
                  const channel8& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size, otherwise \c false will be returned.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @param mask imatrix with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3,
                  const imatrix& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask contains the value given by label.
     * The mask and the image must have the same size.
     *
     * This is seldom used.  See lti::computePalette, maybe that is
     * what you are looking for!
     *
     * @param img image with colors to be considered
     * @param mask channel8 with the mask.  
     * @param label only those pixel whose value in the mask are equal to
     *              this parameters will be considered.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const channel8& mask,const ubyte label);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask contains the value given by label.
     * The mask and the image must have the same size.
     *
     * This is seldom used.  See lti::computePalette, maybe that is
     * what you are looking for!
     *
     * @param img image with colors to be considered
     * @param mask imatrix with the mask.  
     * @param label only those pixel whose value in the mask are equal to
     *              this parameters will be considered.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const imatrix& mask,const int label);

    /**
     * Consider all the images in the given list.  The mask list must have
     * the same size, and the corresponding images must also have the same
     * size.
     *
     * @param imgs list of images to be considered.
     * @param masks list of all masks.
     * @return true if successful or false otherwise.
     */
    bool consider(const std::list<image>& imgs,
                  const std::list<channel8>& masks);

    //@}

    /**
     * @name Methods to consider only the pixels in a mask for this model,
     *       and the rest in an additional model
     */
    //@{
    /**
     * Consider the pixels in the image, for which the corresponding pixel in
     * the mask is strictly greater than the parameters::maskIgnore.to value.
     *
     * Additionally the rest model estimator will consider only the values
     * strictly below the parameters::maskIgnore.from value.
     *
     * The mask and the image must have the same size, otherwise \c false will
     * be returned.
     *
     * @param img image with colors to be considered
     * @param mask channel8 with the mask.  A value strictly greater than the
     *             parameters::maskIgnore.to means that the corresponding
     *             pixel in the image must be considered in this model
     * @param rest the pixels with corresponding mask value strictly below the
     *             parameters::maskIgnore.from will be taken into
     *             account in this estimator.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,
                  const channel8& mask,
                  colorModelEstimation& rest);

    /**
     * Consider the pixels in the image, for which the corresponding pixel in
     * the mask is strictly greater than the parameters::maskIgnore.to value.
     *
     * Additionally the rest model estimator will consider only the values
     * strictly below the parameters::maskIgnore.from value.
     *
     * The mask and the image must have the same size, otherwise \c false will
     * be returned.
     *
     * @param img image with colors to be considered
     * @param mask imatrix with the mask.  A value strictly greater than the
     *             parameters::maskIgnore.to means that the corresponding
     *             pixel in the image must be considered in this model
     * @param rest the pixels with corresponding mask value strictly below the
     *             parameters::maskIgnore.from will be taken into
     *             account in this estimator.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,
                  const imatrix& mask,
                  colorModelEstimation& rest);

    /**
     * Consider the colors constructed from the three given channels, for which
     * the corresponding pixel in the mask is strictly greater than the
     * parameters::maskIgnore.to value.
     *
     * Additionally the rest model estimator will consider only the values
     * strictly below the parameters::maskIgnore.from value.
     *
     * The mask and the image must have the same size, otherwise \c false will
     * be returned.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @param mask channel8 with the mask.  A value strictly greater than the
     *             parameters::maskIgnore.to means that the corresponding
     *             pixel in the image must be considered in this model
     * @param rest the pixels with corresponding mask value strictly below the
     *             parameters::maskIgnore.from will be taken into
     *             account in this estimator.
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3,
                  const channel8& mask,
                  colorModelEstimation& rest);

    /**
     * Consider the colors constructed from the three given channels, for which
     * the corresponding pixel in the mask is strictly greater than the
     * parameters::maskIgnore.to value.
     *
     * Additionally the rest model estimator will consider only the values
     * strictly below the parameters::maskIgnore.from value.
     *
     * The mask and the image must have the same size, otherwise \c false will
     * be returned.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @param mask imatrix with the mask.  A value strictly greater than the
     *             parameters::maskIgnore.to means that the corresponding
     *             pixel in the image must be considered in this model
     * @param rest the pixels with corresponding mask value strictly below the
     *             parameters::maskIgnore.from will be taken into
     *             account in this estimator.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3,
                  const imatrix& mask,
                  colorModelEstimation& rest);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask contains the value given by label.
     * The mask and the image must have the same size.
     *
     * This is seldom used.  See lti::computePalette, maybe that is
     * what you are looking for!
     *
     * @param img image with colors to be considered
     * @param mask channel8 with the mask.  
     * @param label only those pixel whose value in the mask are equal to
     *              this parameters will be considered.
     * @param rest the pixels with a mask value different than the label will
     *             be taken into account in this estimator.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,
                  const channel8& mask,
                  const ubyte label,
                  colorModelEstimation& rest);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask contains the value given by label.
     * The mask and the image must have the same size.
     *
     * This is seldom used.  See lti::computePalette, maybe that is
     * what you are looking for!
     *
     * @param img image with colors to be considered
     * @param mask imatrix with the mask.  
     * @param label only those pixel whose value in the mask are equal to
     *              this parameters will be considered.
     * @param rest the pixels with a mask value different than the label will
     *             be taken into account in this estimator.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,
                  const imatrix& mask,
                  const int label,
                  colorModelEstimation& rest);

    /**
     * Consider the pixels in the given list of images image, for which the
     * corresponding pixel in the corresponding mask is strictly greater than
     * the parameters::maskIgnore.to value.
     *
     * Additionally the rest model estimator will consider only the values
     * strictly below the parameters::maskIgnore.from value.
     *
     * The mask and the image must have the same size, otherwise \c false will
     * be returned.
     *
     * @param imgs list of images to be considered.
     * @param masks list of all masks.  A value strictly greater than the
     *             parameters::maskIgnore.to means that the corresponding
     *             pixel in the image must be considered in this model
     * @param rest the pixels with corresponding mask value strictly below the
     *             parameters::maskIgnore.from will be taken into
     *             account in this estimator.
     *
     * @return true if successful or false otherwise.
     */
    bool consider(const std::list<image>& imgs,
                  const std::list<channel8>& masks,
                  colorModelEstimation& rest);
    //@}

    /**
     * Get a constant reference to the internal model
     */
    const dhistogram& getColorModel() const;

    /**
     * Get a copy of the internal model in the given %parameter.
     * @param model the color model (as 3D histogram) computed until now
     * @return true if apply successful or false otherwise.
     */
    bool apply(dhistogram& model) const;

    /**
     * Get the mean color and covariance matrix of the internal model
     * @param mean the mean color
     * @param covar covariance matrix of the color (3x3 matrix)
     * @return true if apply successful, false otherwise.
     */
    bool apply(rgbPixel<double>& mean,
               dmatrix& covar) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorModelEstimation& copy(const colorModelEstimation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorModelEstimation& operator=(const colorModelEstimation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual colorModelEstimation* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual colorModelEstimation* newInstance() const;

    /**
     * Set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     *
     * This overloading will initialize the size of the internal color model
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * The color model used.  If empty, it is assumed to be uninitialized.
     */
    dhistogram theModel_;

    /**
     * Sum of colors until now
     */
    rgbPixel<double> sum_;

    /**
     * Sum of outer-product of the colors until now (required for the
     * computation of the covariance matrix.
     */
    dmatrix sumOuter_;

    /**
     * Number of pixels considered until now
     */
    int n_;

    /**
     * Outer product of two colors.  The matrix MUST be 3x3 before calling
     * this method.
     */
    void outer(const rgbPixel<double>& a,
               const rgbPixel<double>& b,
               dmatrix& mat) const;

  private:
    /**
     * Iterator for dmatrices
     */
    dmatrix::iterator omit_;

    /**
     * Index vector.  Attribute to reduce creation time...
     */
    ivector index_;

    /**
     * Dimensions of the histogram (red)
     */
    int hdimRed_;

    /**
     * Dimensions of the histogram (green)
     */
    int hdimGreen_;

    /**
     * Dimensions of the histogram (blue)
     */
    int hdimBlue_;

    /**
     * Equivalent to hdimRed-1;
     */
    int lastIdxRed_;

    /**
     * Equivalent to hdimGreen-1;
     */
    int lastIdxGreen_;

    /**
     * Equivalent to hdimBlue-1;
     */
    int lastIdxBlue_;

    /**
     * Low threshold
     */
    iinterval ignoreInterval_;
  };
}

#endif
