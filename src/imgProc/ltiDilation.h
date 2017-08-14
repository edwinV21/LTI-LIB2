/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
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
 * \file   ltiDilation.h
 *         Contains the class lti::dilation,
 *         which implements the morphological operator known as dilation.
 * \author Pablo Alvarado
 * \date   19.07.2000
 *
 * revisions ..: $Id: ltiDilation.h,v 1.2 2009-10-22 14:04:29 alvarado Exp $
 */


#ifndef _LTI_DILATION_H_
#define _LTI_DILATION_H_

#include "ltiObject.h"
#include "ltiMath.h"
#include "ltiImage.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiBoundaryType.h"
#include "ltiMatrixProcessingInterface.h"
#include "ltiConvolutionHelper.h"
#include "ltiTypes.h"
#include "ltiArray.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiMorphology.h"

namespace lti {
  /**
   * Dilation %functor.
   *
   * This functor implements the morphological operator "dilation".
   * Through the parameters a "binary" or "gray scale" modus can be choosed,
   * and the structuring element (represented by a linear filter kernel) can
   * be given.
   *
   * For mode Binary the destination image is set to 0 only if there are no
   * common pixels set in both, the structuring kernel and the source.  In other
   * words, if there is any overlapping pixels between the structuring element
   * and the source image then the pixel on which the structuring element is
   * centered will be set to the given non-zero value.
   *
   * The definitions for mode GrayFlat can be found in e.g. Gonzalez,
   * R. and Woods, R.  Digital Image Processing, 3rd Edition,
   * pp. 666--668, Prentice Hall, 2008
   *
   * \f[
   * \text{dest}(x,y) = \min_{(s,t)\in\text{kernel}}
   * \left\{src(x-s,y-t)\right\}
   * \f]
   *
   * The definitions for mode GrayNonFlat can be found in e.g. Gonzalez,
   * R. and Woods, R.  Digital Image Processing, 3rd Edition,
   * pp. 666--668, Prentice Hall, 2008
   *
   * \f[
   * \text{dest}(x,y) = \min_{(s,t)\in\text{kernel}}
   * \left\{src(x-s,y-t)+kernel(s,t)\right\}
   * \f]
   *
   * where the regions of the kernel and source overlap. Qualitatively the Gray
   * operation results in an enlightment of especially dark.  A value of 0 in
   * the kernel means that pixel does \b not belong to the structuring element.
   *
   * For channel8 the resulting values are clipped to be in the allowed range
   * of [0,255]. Note that for channels the kernel values should be much lower
   * than the default 1.f. Also note that when the kernel is separable
   * (separableKernel) the values of all column and row kernels are added. An
   * example is squareKernel.
   *
   * Example:
   *
   * \code
   * lti::dilation dilater;                   // the dilation functor
   * lti::dilation::parameters dilationParam; // the parameters
   *
   * lti::cityBlockKernel<float> kern(3);     // 3x3 diamond shaped kernel
   *
   * // binary dilation
   * dilationParam.mode = lti::dilation::Binary;
   * dilationParam.setStructuringElement(kern);
   *
   * // set the parameters
   * dilater.setParameters(dilationParam);
   *
   * // apply the dilation to a channel "chnlSrc" and leave the result in
   * // "chnlDest"
   *
   * dilater.apply(chnlSrc,chnlDest);
   * \endcode
   *
   * @see lti::dilation::parameters
   *
   * @ingroup gMorphology
   */
  class dilation : public morphology {
  public:
    /**
     * Alias for parent class parameters
     */
    typedef morphology::parameters parameters;

    /**
     * Default constructor
     */
    dilation();

    /**
     * Construct  with the given structuring element
     */
    dilation(const container& aKernel);

    /**
     * Construct with the given parameters
     */
    dilation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    dilation(const dilation& other);

    /**
     * Destructor
     */
    virtual ~dilation();

    /**
     * Returns the name of this type ("dilation")
     */
    virtual const std::string& name() const;

    /**
     * Operates on the given parameter.
     * @param srcdest fmatrix with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    bool apply(fmatrix& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param srcdest matrix<ubyte> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param srcdest fvector with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fvector& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param srcdest vector<ubyte> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<ubyte>& srcdest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param src fmatrix with the source data.
     * @param dest fmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fmatrix& src,fmatrix& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param src fvector with the source data.
     * @param dest fvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fvector& src,fvector& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param src vector<ubyte> with the source data.
     * @param dest vector<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<ubyte>& src,
                     vector<ubyte>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dilation& copy(const dilation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual dilation* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual dilation* newInstance() const;

  private:

    /**
     * This is the accumulator class needed by the convolution helper to
     * act as a dilation operator for gray valued images.
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class accumulatorGrayFlat {
    public:
      /**
       * Default constructor
       */
      accumulatorGrayFlat();

      /**
       * Accumulate the values of filter and src
       */
      inline void accumulate(const T& filter,const T& src);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Accumulate the values of filter and srcL and srcR
       * for symmetric filter kernel
       * src:               srcL  *  middle  *  srcR
       * filter:            *  *  *  middle  *  *  *
       * used filter part:  *  *  *  middle
       */
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of filter and src
       * for asymmetric filter kernel
       * src:               srcL  *  middle  *  srcR
       * filter:            *  *  *  middle  *  *  *
       * used filter part:  *  *  *  middle
       */
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * Set norm
       */
      inline void setNorm(const T& norm);
    protected:
      /**
       * The accumulated value
       */
      U state_;

      /**
       * The norm
       */
      T norm_;
    };

    /**
     * This is the accumulator class needed by the convolution helper to
     * act as a dilation operator for gray valued images.
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class accumulatorGrayNonFlat {
    public:
      /**
       * Default constructor
       */
      accumulatorGrayNonFlat();

      /**
       * Accumulate the values of filter and src
       */
      inline void accumulate(const T& filter,const T& src);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Accumulate the values of filter and srcL and srcR
       * for symmetric filter kernel
       * src:               srcL  *  middle  *  srcR
       * filter:            *  *  *  middle  *  *  *
       * used filter part:  *  *  *  middle
       */
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of filter and src
       * for asymmetric filter kernel
       * src:               srcL  *  middle  *  srcR
       * filter:            *  *  *  middle  *  *  *
       * used filter part:  *  *  *  middle
       */
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * Set norm
       */
      inline void setNorm(const T& norm);
    protected:
      /**
       * The accumulated value
       */
      U state_;

      /**
       * The norm
       */
      T norm_;
    };

    /**
     * This is the accumulator class needed by the convolution helper to
     * act as a dilation operator for binary valued images.
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class accumulatorBin {
    public:
      /**
       * Default constructor
       */
      accumulatorBin();

      /**
       * Accumulate the values of filter and src
       */
      inline void accumulate(const T& filter,const T& src);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Accumulate the values of filter and srcL and srcR
       * for symmetric filter kernel
       * src:                   srcL  *  middle  *  srcR
       * filter:                *  *  *  middle  *  *  *
       * used filter part:      *  *  *  middle
       */
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of filter and src
       * for asymmetric filter kernel
       * src:                   srcL  *  middle  *  srcR
       * filter:                *  *  *  middle  *  *  *
       * used filter part:      *  *  *  middle
       */
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);


      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * Set norm
       */
      inline void setNorm(const T& nrm);
    protected:
      /**
       * The accumulated value
       */
      U state_;

      /**
       * The norm
       */
      T norm_;
    };

  };
}

#endif
