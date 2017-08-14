/*
 * Copyright (C) 2002 2003 2004 2005 2006
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
 * \file   ltiCannyEdges.h
 *         Contains the class lti::cannyEdges, 
 *         which implements the classical Canny Edge Detector.
 * 
 * \author Christian Harte
 * \author Pablo Alvarado
 * \date   19.07.2002
 *
 * revisions ..: $Id: ltiCannyEdges.h,v 1.2 2012-07-25 23:33:30 alvarado Exp $
 */

#ifndef _LTI_CANNY_EDGES_H_
#define _LTI_CANNY_EDGES_H_

#include "ltiEdgeDetection.h"
#include "ltiNonMaximaSuppression.h"
#include "ltiColorContrastGradient.h"
#include "ltiArctanLUT.h"

namespace lti {
  /**
   * Canny Edge Detector
   *
   * The Canny Edge Detector is a standard algorithm, designed to detect
   * "optimal" edges.  Almost every image processing book (e.g. Sonka et.al
   * Image Processing, Analysis and Machine Vision) explains the details for
   * the algorithm.
   *
   * The values used for the edges and background are specified in the
   * parameters (see lti::edgeDetection::parameters)
   *
   * The construction of the first cannyEdges functor in your
   * application will take some time to initialize a Look-Up-Table
   * that allows a much faster edge detection later  (about 0.13 seconds
   * on a Pentium III, 450MHz).  The LUT will require about 1MB.
   * Please see lti::arctanLUT for more information.
   *
   * You can also choose between the "classic" simple gradient operator,
   * which considers only two neighbors at each axis \c x and \c y or the more
   * accurate gradient kernels (see lti::gradientKernelX).  In the latter
   * mode, the edge detection is about a factor 2 slower.
   *
   * For color images the color contrast gradient is used.
   *
   * @see lti::nonMaximumSuppression
   * @see lti::colorContrastGradient
   * @see lti::edgeDetection
   *
   * @ingroup gEdgeCorner
   */
  class cannyEdges : public edgeDetection {
  public:
    /**
     * the parameters for the class cannyEdges
     */
    class parameters : public edgeDetection::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor.
       *
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns the name of this class
       */
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler.
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler.
       *
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
       * Variance for the Gaussian to smooth out the image.
       * (see also gaussKernel1D<ubyte>::gaussKernel1D())
       *
       * Default value: 1.0
       */
      float variance;

      /**
       * Size of the Gaussian kernel used to smooth out the image
       *(see also variance).
       *
       * Set to zero if you don't want any smoothing.
       *
       * Default value: 7
       */
      int kernelSize;

      /**
       * If a pixel is detected as part of an edge (a response higher
       * than thresholdMax), its neighbors are consider also edges if their
       * values are higher than the given percentage of thresholdMax
       * (i.e. higher than thresholdMax*thresholdMin)
       *
       * This value must be between 0.0 and 1.0
       *
       * Default Value: 0.5
       */
      float thresholdMin;

      /**
       * If an edge response is higher than this value, those pixels will be
       * definite an edge.  This value MUST be between 0 and 1.0.
       *
       * Please note that the older value range between 0 and 255 is 
       * deprecated.
       *
       * Default Value: 0.04f;
       */
      float thresholdMax;

      /**
       * Parameters for gradient computation. 
       *
       * To provide more alternatives to the gradient computation, the
       * lti::gradientFunctor is used.  Here you can select all options
       * available for that functor.
       *
       * The default behaviour is to use the
       * lti::gradientFunctor::parameters::Difference kernel, which is the
       * fastest but also the less precise.  You can use the optimal
       * kernels if you require a more stable orientation detection.
       *
       * Default value: Difference mode (fast gradient computation)
       *
       * @see lti::gradientFunctor
       */
      colorContrastGradient::parameters gradientParameters;
    };

    /**
     * Default constructor
     */
    cannyEdges();

    /**
     * Constructor with parameters
     */
    cannyEdges(const parameters& thePars);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    cannyEdges(const cannyEdges& other);

    /**
     * Destructor
     */
    virtual ~cannyEdges();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Compute the Canny edges of the \c srcdest channel and leave the result
     * there too.
     *
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * Compute the Canny edges of the \c srcdest channel and leave the result
     * there too.
     *
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * Compute the Canny edges of the \c src channel and leave the result
     * in the \c dest channel.
     *
     * This is the fastest apply method because it does not require to cast the
     * channels to any other type.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * Compute the Canny edges for the given source channel.
     *
     * The found edges will be left on the \c edges channel, and the
     * orientation angle of the edges (that can be used in other functors, like
     * the lti::houghLineTransform) is left on the \c orientation channel.
     *
     * @param src channel8 with the source data.
     * @param edges channel8 where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,
                             channel8& edges,
                             channel& orientation) const;

    /**
     * Compute the Canny edges for the given source channel.
     *
     * The found edges will be left on the \c edges channel, and the
     * orientation angle of the edges (that can be used in other functors, like
     * the lti::houghLineTransform) is left on the \c orientation channel.
     * You might also need the strength channel, associated in Canny with
     * the magnitude of the gradient.
     *
     * @param src channel8 with the source data.
     * @param edges channel8 where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @param strength edge strength, associated with the magnitude of the 
     *        gradient
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,
                             channel8& edges,
                             channel& orientation,
                             channel& strength) const;

    /**
     * Compute the Canny edges of the \c src channel and leave the result
     * in the \c dest channel.
     *
     * Some channel casts are required, hence this method is slower than other
     * ones.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;


    /**
     * Compute the Canny edges of the \c src channel and leave the result
     * in the \c dest channel.
     *
     * Some channel casts are required, hence this method is slower than other
     * ones.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel8& dest) const;

    /**
     * Compute the Canny edges for the given source channel.
     *
     * The found edges will be left on the \c edges channel, and the
     * orientation angle of the edges (that can be used in other functors, like
     * the lti::houghLineTransform) is left on the \c orientation channel.
     *
     * @param src channel with the source data.
     * @param edges channel8 where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                             channel8& edges,
                             channel& orientation) const;


    /**
     * Compute the Canny edges for the given source channel.
     *
     * The found edges will be left on the \c edges channel, and the
     * orientation angle of the edges (that can be used in other functors, like
     * the lti::houghLineTransform) is left on the \c orientation channel.
     *
     * @param src channel with the source data.
     * @param edges channel8 where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @param strength edge strength, associated with the magnitude of the 
     *        gradient
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                             channel8& edges,
                             channel& orientation,
                             channel& strength) const;

    /**
     * Color edges.
     *
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * The problem lies in the definition of the "gradient" for a
     * color channel.  Here, the approach introduced in A. Cumani,
     * "Edge Detection in Multispectral Images", Technical Report,
     * Istituto Elettrotecnico Nazionale "Galileo Ferraris", 1989 is
     * followed.
     *
     * With this approach, instead of the usual gradient, the maxima of the
     * contrast function are searched.  The contrast function defines
     * the direction in the (x,y) plane at which the contrast change is
     * maximal.  See lti::colorContrastGradient for more information.
     *
     * The Canny-typical non-maxima suppression procedure is applied on the
     * contrast result.
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,channel8& dest) const;

    /**
     * Color edges.
     *
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * @see apply(const image&,channel8&)
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                             channel8& dest,
                             channel& orientation) const;


    /**
     * Color edges.
     *
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * @see apply(const image&,channel8&)
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @param strength edge strength, associated with the magnitude of the 
     *        gradient
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                             channel8& dest,
                             channel& orientation,
                             channel& strength) const;

    /**
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * The problem lies in the definition of the "gradient" for a
     * color channel.  Here, the approach introduced in A. Cumani,
     * "Edge Detection in Multispectral Images", Technical Report,
     * Istituto Elettrotecnico Nazionale "Galileo Ferraris", 1989 is
     * followed.
     *
     * With this approach, instead of the usual gradient, the maxima of the
     * contrast function are searched.  The contrast function defines
     * the direction in the (x,y) plane at which the contrast change is
     * maximal.  See lti::colorContrastGradient for more information.
     *
     * The Canny-typical non-maxima suppression procedure is applied on the
     * contrast result.
     *
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     * @param edges the found edges
     * @param orientation orientation of the found angles.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& c1,
                       const channel& c2,
                       const channel& c3,
                       channel8& edges,
                       channel& orientation) const;

    /**
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * The problem lies in the definition of the "gradient" for a
     * color channel.  Here, the approach introduced in A. Cumani,
     * "Edge Detection in Multispectral Images", Technical Report,
     * Istituto Elettrotecnico Nazionale "Galileo Ferraris", 1989 is
     * followed.
     *
     * With this approach, instead of the usual gradient, the maxima of the
     * contrast function are searched.  The contrast function defines
     * the direction in the (x,y) plane at which the contrast change is
     * maximal.  See lti::colorContrastGradient for more information.
     *
     * The Canny-typical non-maxima suppression procedure is applied on the
     * contrast result.
     *
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     * @param edges the found edges
     * @param orientation orientation of the found angles.
     * @param strength edge strength, associated with the magnitude of the 
     *        gradient
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& c1,
                       const channel& c2,
                       const channel& c3,
                       channel8& edges,
                       channel& orientation,
                       channel& strength) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cannyEdges& copy(const cannyEdges& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cannyEdges& operator=(const cannyEdges& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual cannyEdges* clone() const;
    
    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual cannyEdges* newInstance() const;

    /**
     * Update functor's parameters.
     *
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    
    /**
     * Non maxima suppression functor
     */
    nonMaximaSuppression suppressor_;

    /**
     * Look-Up Table for the computations of atan2
     */
    arctanLUT atan2_;

    /**
     * convert a pair of values in cartesic system into a pair in
     * polar system
     *
     * The LUT takes care that the returned value lies between 0 and 2*Pi
     */
    inline void cartToPolar(const int dx,const int dy,
                            float& mag,float& angle) const;

    /**
     * The gradient functor used
     */
    colorContrastGradient grad_;

  };
}

#endif
