/*
 * Copyright (C) 2007 by Pablo Alvarado
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
 * \file   ltiIntegralImage.h
 *         Contains the class lti::integralImage, 
 *         which is used as a preliminary stage of several other algorithms
 *         that need to convolve with boxes filled with a constant value.
 * \author Pablo Alvarado
 * \date   16.10.2007
 *
 * revisions ..: $Id: ltiIntegralImage.h,v 1.10 2008-01-16 21:52:26 alvarado Exp $
 */

#ifndef _LTI_INTEGRAL_IMAGE_H_
#define _LTI_INTEGRAL_IMAGE_H_

#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiBoundaryType.h"

namespace lti {

  /**
   * Class integralImage
   *
   * The integral image, (which in the LTI-Lib should it be better called the
   * integral channel, but for consistency with the literature we leave it as
   * is) is the image \f$I_\Sigma(x,y)\f$ obtained from the channel
   * \f$I(x,y)\f$ with the following convention:
   * \f[
   *   I_\Sigma(x,y) = \sum_{i=0}^{x}\sum_{j=0}^{y} I(i,j)
   * \f]
   *
   * From this integral image, the computation of any sum of intensities over
   * any upright, rectangular area can be done with only four additions,
   * independent of the area size.
   *
   * The computation of the integral image is very efficient.  It needs just
   * one pass of the orignal image to be obtained, with just two additions per
   * pixel.
   *
   * The class provides a collection of useful methods to compute the sum of
   * elements in a rectangular window, for which the parameter boundaryType
   * makes a lot of sense.
   *
   * @see integralImage::parameters.
   *
   * @ingroup gLinearFilters
   */
  class integralImage : public functor {
  public:
    /**
     * The parameters for the class integralImage
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
       * Boundary type.
       *
       * Default value: Zero
       */
      eBoundaryType boundaryType;

    };

    /**
     * Default constructor
     */
    integralImage();

    /**
     * Construct with the given boundary type
     */
    integralImage(const eBoundaryType boundaryType);

    /**
     * Construct a functor using the given parameters
     */
    integralImage(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    integralImage(const integralImage& other);

    /**
     * Destructor
     */
    virtual ~integralImage();

    /**
     * Compute the integral image of a ubyte matrix.  
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src, matrix<int32>& dest) const;

    /**
     * Compute the integral image of an fmatrix.
     *
     * @param src fmatrix with the source data.
     * @param dest fmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const fmatrix& src, fmatrix& dest) const;

    /**
     * @name Convenience window sums with boundary considerations.
     *
     * This methods used the computed integral image to produce the sum of
     * elements of the originary channel falling inside a given window.
     *
     * These methods are relatively slow, since they check if a part of the
     * window falls outside the image and take the necessary measures depending
     * on the boundary type selected.  The methods are designed to produce
     * valid results if and only if at least a part of the window falls over
     * the image.
     *
     * If in your application you know for sure that the window falls
     * completely inside the image, and at least with one pixel free on the top
     * and left boundaries, then you may want to use the unchecked
     * internalSum() methods.
     *
     * The NoBoundary type will return 0 if any part of the given window falls
     * outside the image.
     *
     * You have to ensure that the \c from arguments correspond to the top-left
     * corner or the window, and that the \c to arguments correspond to the
     * bottom-right one.
     *
     * \warning Invalid results are provided if the complete window falls
     *          outside the image.  You must ensure that the image is not empty
     *          when you call these methods.
     */
    //@{
    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param rect the delimiter of a window whose pixels have to be sumed.
     */
    inline int32 sum(const matrix<int32>& intImage,
                     const irectangle& rect) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param from initial point of area to be summed.
     * @param to final point of area to be summed.
     *
     * @return the computed sum
     */
    inline int32 sum(const matrix<int32>& intImage,
                     const ipoint& from,
                     const ipoint& to) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param fromX initial x coordinate of area to be summed.
     * @param fromY initial y coordinate of area to be summed.
     * @param toX final x coordinate of area to be summed.
     * @param toY final y coordinate of area to be summed.
     *
     * @return the computed sum
     */
    inline int32 sum(const matrix<int32>& intImage,
                     const int fromX, const int fromY,
                     const int toX, const int toY) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param rect the delimiter of a window whose pixels have to be sumed.
     *
     * @return the computed sum
     */
    inline float sum(const fmatrix& intImage,
                     const irectangle& rect) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param from initial point of area to be summed.
     * @param to final point of area to be summed.
     *
     * @return the computed sum
     */
    inline float sum(const fmatrix& intImage,
                     const ipoint& from,
                     const ipoint& to) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param fromX initial x coordinate of area to be summed.
     * @param fromY initial y coordinate of area to be summed.
     * @param toX final x coordinate of area to be summed.
     * @param toY final y coordinate of area to be summed.
     *
     * @return the computed sum
     */
    inline float sum(const matrix<float>& intImage,
                     const int fromX, const int fromY,
                     const int toX, const int toY) const;
    //@}

    /**
     * @name Convenience window sums without boundary considerations.
     *
     * These methods use the computed integral image to produce the sum of
     * elements of the originary channel falling inside a given window.
     *
     * They are relatively fast, since they assume that the complete window
     * falls into the image and at least one pixel border has been left on the
     * top and on the left.  If this is not the case, you have to use the sum()
     * methods instead.
     *
     * \warning If this condition is not met, the debug version will report an
     * assert when accessing the matrix at invalid indices, and the release
     * version probably will crash due to a segmentation fault.
     *
     * This method is used when you know for sure that the window falls
     * completely inside the image, and leaves at least one pixel free on the
     * top and left boundaries.
     */

    //@{
    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param rect the delimiter of a window whose pixels have to be sumed.
     */
    inline int32 internalSum(const matrix<int32>& intImage,
                             const irectangle& rect) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param from initial point of area to be summed.
     * @param to final point of area to be summed.
     *
     * @return the computed sum
     */
    inline int32 internalSum(const matrix<int32>& intImage,
                             const ipoint& from,
                             const ipoint& to) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param fromX initial x coordinate of area to be summed.
     * @param fromY initial y coordinate of area to be summed.
     * @param toX final x coordinate of area to be summed.
     * @param toY final y coordinate of area to be summed.
     *
     * @return the computed sum
     */
    inline int32 internalSum(const matrix<int32>& intImage,
                             const int fromX, const int fromY,
                             const int toX, const int toY) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param rect the delimiter of a window whose pixels have to be sumed.
     *
     * @return the computed sum
     */
    inline float internalSum(const fmatrix& intImage,
                             const irectangle& rect) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param from initial point of area to be summed.
     * @param to final point of area to be summed.
     *
     * @return the computed sum
     */
    inline float internalSum(const fmatrix& intImage,
                             const ipoint& from,
                             const ipoint& to) const;

    /**
     * Convenience method to use the computed integral image to produce the
     * sum of the results in the provided rectangle.
     *
     * @param intImage an integral image computed with apply()
     * @param fromX initial x coordinate of area to be summed.
     * @param fromY initial y coordinate of area to be summed.
     * @param toX final x coordinate of area to be summed.
     * @param toY final y coordinate of area to be summed.
     *
     * @return the computed sum
     */
    inline float internalSum(const matrix<float>& intImage,
                             const int fromX, const int fromY,
                             const int toX, const int toY) const;

    //@}

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    integralImage& copy(const integralImage& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    integralImage& operator=(const integralImage& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual integralImage* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual integralImage* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

  private:
    /**
     * General implementation of the integration
     */
    template<typename T,typename U>
    bool integrate(const matrix<T>& src,matrix<U>& dest) const;

#   ifndef NDEBUG

    // Implementation in debug mode is accelerated by avoiding a switch
    // which costs too much without optimizations

    /**
     * Internal base class for all boundary-bounded sum methods
     * This is faster than a switch of boundary types
     */
    template<typename T>
    class dispatcher {
    public:
      /**
       * Constructor
       */
      dispatcher();
      /**
       * Desttructor
       */
      virtual ~dispatcher();

      /**
       * The virtual method for everybody
       *
       * The default implementation returns the zero boundary case
       */
      virtual T sumBox(const matrix<T>& intImage,
                    const int fromX,const int fromY,
                    const int toX,const int toY) const;

    protected:
      /**
       * Convenience method to use the computed integral image to produce the
       * sum of the results in the provided rectangle.
       *
       * @param intImage an integral image computed with apply()
       * @param from initial point of area to be summed.
       * @param to final point of area to be summed.
       */
    inline T internalSum(const matrix<T>& intImage,
                         const int fromX,const int fromY,
                         const int toX,const int toY) const;

    };   

    /**
     * Internal class no-boundary
     */
    template<typename T>
    class sumNoBoundary : public dispatcher<T> {
     public:
     /** 
       * Constructor
       */
      sumNoBoundary();
 

      /**
       * The virtual method for everybody
       */
      virtual T sumBox(const matrix<T>& intImage,
                    const int fromX,const int fromY,
                    const int toX,const int toY) const;
    };   

    /**
     * Internal class for zero
     */
    template<typename T>
    class sumZero : public dispatcher<T> {
    public:
      /**
       * Constructor
       */
      sumZero();
    };   

    /**
     * Internal class for constant
     */
    template<typename T>
    class sumConstant : public dispatcher<T> {
    public:
     /**
       * Constructor
       */
      sumConstant();


      /**
       * The virtual method for everybody
       */
      virtual T sumBox(const matrix<T>& intImage,
                    const int fromX,const int fromY,
                    const int toX,const int toY) const;
    };   

    /**
     * Internal class for periodic
     */
    template<typename T>
    class sumPeriodic : public dispatcher<T> {
    public:
      /**
       * Constructor
       */
      sumPeriodic();

      /**
       * The virtual method for everybody
       */
      virtual T sumBox(const matrix<T>& intImage,
                    const int fromX,const int fromY,
                    const int toX,const int toY) const;
    };   

    /**
     * Internal class for mirror
     */
    template<typename T>
    class sumMirror : public dispatcher<T> {
    public:
      /**
       * Constructor
       */
      sumMirror();


      /**
       * The virtual method for everybody
       */
      virtual T sumBox(const matrix<T>& intImage,
                    const int fromX,const int fromY,
                    const int toX,const int toY) const;
    };   

    /**
     * Dispatcher for sum methods
     */
    dispatcher<float>* sumFloat_;

    /**
     * Dispatcher for sum methods
     */
    dispatcher<int32>* sumInt_;

# else 


    // Implementation in release mode is based on a switch, which is
    // efficiently optimized by the compiler
    
    /**
     * The sum dispatcher
     */

    template<typename T>
    inline T sumDispatcher(const matrix<T>& intImage,
                           const int fromX,const int fromY,
                           const int toX,const int toY) const;

    /**
     * SumBox considers the zero boundary, and is used by the other
     * methods, to be compatible with the debug dispatcher method we keep
     * just this name
     */
    template<typename T>
    T sumBox(const matrix<T>& intImage,
             const int fromX,const int fromY,
             const int toX,const int toY) const;
    
    template<typename T>
    T sumNoBoundary(const matrix<T>& intImage,
                    const int fromX,const int fromY,
                    const int toX,const int toY) const;

    template<typename T>
    T sumConstant(const matrix<T>& intImage,
                  const int fromX,const int fromY,
                  const int toX,const int toY) const;
    
    template<typename T>
    T sumPeriodic(const matrix<T>& intImage,
                  const int fromX,const int fromY,
                  const int toX,const int toY) const;

    template<typename T>
    T sumMirror(const matrix<T>& intImage,
                const int fromX,const int fromY,
                const int toX,const int toY) const;

#endif

    /**
     * Shadow of the parameters to save some critical time in the
     * sum methods
     */
    eBoundaryType boundaryType_;

  };
}

#include "ltiIntegralImage_inline.h"

#endif

