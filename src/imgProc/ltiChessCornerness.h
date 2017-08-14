/*
 * Copyright (C) 2013
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
 * \file   ltiChessCornerness.h
 *         Contains the class lti::chessCornerness, which implements
 *         the ChESS corner detector proposed by Stuart Bennett and
 *         Joan Lasenby
 * 
 * \author Pablo Alvarado
 * \date   10.03.2013
 *
 * revisions ..: $Id: ltiChessCornerness.h,v 1.2 2013-03-10 14:10:26 alvarado Exp $
 */

#ifndef _LTI_CHESS_CORNERNESS_H_
#define _LTI_CHESS_CORNERNESS_H_

#include "ltiCornernessFunctor.h"
#include "ltiBoundaryType.h"
#include "ltiNearestNeighborInterpolation.h"

namespace lti {

  /**
   * Class chessCornerness
   *
   * The ChESS feature detector (Chess-board Extraction by Subtraction
   * and Summation) has been proposed in
   *
   * Bennett, S. and Lasenby, J. "ChESS - Quick and Robust Detection of 
   *   Chess-board Features". arXiv:1301.5491v1, January 2013
   *
   * It is a relatively simple feature detector specially designed for the
   * detection of chess-board corners.  It is frequently used in
   * camera calibration tasks.
   *
   * The method produces a cornerness-like output for each pixel, that
   * can be further processed by each application or by lti::localExtremes.
   *
   * It provides the standard interface of lti::cornernessFunctor.  For
   * the color images, the cornerness is computed for the intensity channel
   * of a HSI implementation and multiplied by (1-saturation) to enhance
   * black and white corners above colored ones, which is a common task.
   *
   *
   * Example:
   * \code
   * lti::image img; // input image loaded/created/captured somehow...
   * 
   * lti::channel cornerness; // chess feature value
   *
   * lti::chessCornerness detector; // computes the ChESS feature
   *
   * detector.apply(img,cornerness); // compute for every pixel the likelihood
   *                                 // that it belongs to a chess-board corner
   * 
   * // Now compute where are the maxima:
   * lti::localExtremes::parameters lePar;
   * lePar.relativeThreshold = 0.5; // just get the strongest ones
   * lePar.kernelSize = 11;         // CheSS uses anyway 5 pixel radius
   *
   * lti::localExtremes ext(lePar);
   * lti::ipointList corners;
   * ext.apply(cornerness,corners);
   *
   * // Paint the corners for visualization
   * lti::channel canvas(cornerness.size(),0.0f);
   * lti::ipointList::iterator it;
   * for (it=corners.begin();it!=corners.end();++it) {
   *   canvas.at(*it)=cornerness.at(*it);
   * }
   * lti::viewer2D viewCorners("Corners");
   * viewCorners.show(canvas);
   *
   * \endcode
   *
   * @see chessCornerness::parameters.
   *
   * @ingroup gEdgeCorner
   */
  class chessCornerness : public cornernessFunctor {
  public:
    /**
     * The parameters for the class chessCornerness
     */
    class parameters : public cornernessFunctor::parameters {
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
       * Boundary Type
       *
       * Specify how the boundaries will be computed
       *
       * See the documentation for lti::eBoundaryType for more information.
       *
       * The default value is set to be compatible with the paper original
       * proposal and its reference code, where the boundary is simply ignored.
       *
       * Default: lti::NoBoundary
       */
      eBoundaryType boundaryType;

      /**
       * The pure definition of the ChESS cornerness allows for negative
       * values, but usually they are useless.
       *
       * You can reactive the presentation of the negative values by
       * setting this parameter to false.
       *
       * Default: true
       */
      bool suppressNegatives;
    };

    /**
     * Default constructor
     */
    chessCornerness();

    /**
     * Construct a functor using the given parameters
     */
    chessCornerness(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    chessCornerness(const chessCornerness& other);

    /**
     * Destructor
     */
    virtual ~chessCornerness();

    /**
     * Compute the ChESS response just for a single point.
     *
     * Do not use this function to compute the cornerness of a complete
     * channel, since it would be too slow!  For that case use the inherited
     * apply() methods.
     *
     * This method checks for the boundary type, and therefore is too
     * slow for a the computation of a large set of points.
     *
     * However, if the cornerness is required for just a few points,
     * then the use of this method is more efficient as the complete
     * image computation.
     */
    float chess(const channel& src,const ipoint& pos) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chessCornerness& copy(const chessCornerness& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    chessCornerness& operator=(const chessCornerness& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual chessCornerness* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual chessCornerness* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * The interpolator used is configured appropriatelly
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();


  protected:
    /**
     * Does the actual calculation of the cornerness of \a src and leaves it
     * in \a dest.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool worker(const channel& src, channel& dest,
                        float& minCornerness, float& maxCornerness) const;

    /**
     * Does the actual calculation of the cornerness of \a src and leaves it
     * in \a dest.
     *
     * Due to the different nature of grey and color images a
     * different but similar method will be used here. This can be
     * done by e.g. using a colorGradient instead of a normal
     * gradient.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool worker(const image& src, channel& dest,
                        float& minCornerness, float& maxCornerness) const;

    /**
     * Does the paper chess implementation with a radius 5 ring
     *
     * This assumes src is connected.
     *
     * In the future other radii may be implemented
     */
    bool chess5(const channel& src, channel& dest,
		float& minCornerness, float& maxCornerness) const;
    
    /**
     * Almost the same chess implementation but on the borders
     *
     * This assumes src is connected.
     */
    bool border5(const channel& src,
		 const irectangle& region, channel& dest,
		 float& minCornerness, float& maxCornerness) const;

    /**
     * Interpolator used for managing the boundaries
     */
    nearestNeighborInterpolation<float> nni_;

    /**
     * Shadow of the boundary type
     */
    eBoundaryType boundaryType_;

    /**
     * Shadow of negative suppression
     */
    bool suppressNegatives_;

  };
}

#endif

