/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiDownsampling.h
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   11.05.2000
 * 
 * $Id: ltiDownsampling.h,v 1.2 2006-09-15 15:02:55 doerfler Exp $
 */

#ifndef _LTI_DOWNSAMPLING_H_
#define _LTI_DOWNSAMPLING_H_

#include "ltiObject.h"
#include "ltiModifier.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h" 
namespace lti {
  /** 
   * This functor is used to downsample images.
   *
   * The downsampling::parameters specify which kind of filter should be
   * used and which downsampling factor.  If you do not want or need
   * to low-pass filter the image first, than take a look at lti::decimation.
   *
   * @see lti::decimation, lti::upsampling, lti::filledUpsampling
   *
   * @ingroup gLinearFilters
   *
   * Example:
   * \code
   *  
   * #include "ltiDownsampling.h"
   * #include "ltiGaussKernel2D.h"
   *   
   * lti::channel chnl1,chnl2;
   *   
   * ...
   *   
   * lti::downsampling downsampler;       // downsampling functor
   * lti::downsampling::parameters param; // downsampling parameters
   *   
   * // a gaussian kernel to use before the downsampling
   * lti::gaussKernel2D<lti::channel::value_type> gkernel;
   *   
   * param.setKernel(gkernel);  // use the gaussian kernel
   *   
   * downsampler.setParameters(param);  // use the given parameters
   *
   * downsampler.apply(chnl1,chnl2);  // channel 1 will be downsampled an the
   *                                  // result will be left on channel 2.
   *
   * \endcode
   */
  class downsampling : public modifier {
  public:
    /**
     * the parameters for the class downsampling
     */
    class parameters : public modifier::parameters {
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
       * Returns the name of this type.
       */
      virtual const std::string& name() const;
      
      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;
      
      /**
       * Returns a pointer to a new instance of this functor.
       */
      virtual parameters* newInstance() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * returns the kernel in use.  If it is not set yet, an
       * lti::invalidParameters exception will be thrown
       * @return a const reference to the filter kernel.
       */
      const container& getKernel() const;

      /**
       * sets the filter kernel to be used.
       * A copy of the given parameter will be made!
       *
       * Default value: see attribute lti::downsampling::parameters::kernel
       * @param aKernel the filter kernel to be used
       */
      void setKernel(const container& aKernel);

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

      /**
       * The filter to be used.
       *
       * This should be a separable kernel of the same type as the object
       * to be decimated, with only one filter pair.  If not, some time will
       * be required to construct an adequate filter kernel!
       *
       * The default value for the kernel is a 3x3 separable kernel, with
       * row/column kernel of the form (0.25 0.5 0.25)x(0.25 0.5 0.25).
       */
      container* kernel;

      /**
       * the downsampling factor
       *
       * Only the pixels with coordinates x=n*factor.x or y=m*factor.y,
       * (n and m integers) will be taken.
       *
       * The default value is (2,2).
       */
      ipoint factor;
    };

    /**
     * default constructor
     */
    downsampling();

    /**
     * Constructor with parameters object
     */
    downsampling(const parameters& par);

    /**
     * create a downsampling functor with a parameter set which includes
     * the given filter kernel
     */
    downsampling(const container& aKernel);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    downsampling(const downsampling& other);

    /**
     * destructor
     */
    virtual ~downsampling();
    
    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;
      
    /**
     * Returns a pointer to a clone of the parameters
     */
    virtual downsampling* clone() const;
      
    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual downsampling* newInstance() const;
    

    /**
     * operates on the given parameter.
     * @param srcdest channel8 with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<float>& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest dmatrix with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dmatrix& srcdest) const;

    /**
     * downsample a color image by splitting it into the RGB components,
     * downsampling each channel and merging the results
     * @param srcdest image with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(image& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest vector<channel8::value_type> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<channel8::value_type>& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest vector<channel::value_type> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<channel::value_type>& srcdest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<float>& src,matrix<float>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * downsample a color image by splitting it into the RGB components,
     * downsampling each channel and merging the results
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,image& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<channel8::value_type> with the source data.
     * @param dest vector<channel8::value_type> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<channel8::value_type>& src,
                    vector<channel8::value_type>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<channel::value_type> with the source data.
     * @param dest vector<channel::value_type> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<channel::value_type>& src,
                     vector<channel::value_type>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    downsampling& copy(const downsampling& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut to set the filter kernel
     * A copy of the given parameter will be made!
     * @param aKernel the filter kernel to be used
     */
    void setKernel(const container& aKernel);

  private:
    /**
     * convolution algorithms (private classes of lti::downsampling)
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template <class T,class U>
      class convolve {
      protected:
      /**
       * accumulates src*filter in result
       */
      inline void accumulate(const T& filter,const T& src, U& result);

      /**
       * get the result with the proper typecasting and normalization
       */
      inline T getResult(const U& result);

      /**
       * set normalization
       */
      inline void setNorm(const T& theNorm);

      /**
       * normalization constant.  Taken from filter kernel
       */
      T norm_;

    };

    /**
     * convolution algorithm for a vector * kernel1D
     * The type T is the type of the elements of the vector and kernel1D
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template <class T,class U>
      class convolve1D : public convolve<T,U> {
      public:
      /**
       * default constructor
       */
      convolve1D();

      /**
       * destructor
       */
      ~convolve1D();

      /**
       * try to get a Kernel from the parameters.
       * If it is not possible to get it, return false;
       * This will not copy the kernel, it just will keep a reference to it!
       * @param kern the container with the kernel.
       * @return true if kernel could be set and false otherwise
       */
      bool setKernel(const container* kern);

      /**
       * convolves the vector src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src vector to be filtered
       * @param kern filter kernel
       * @param dest destination vector for the result
       */
      void apply(const vector<T>& src,
                       vector<T>& dest,
                 const downsampling::parameters& param);

      /**
       * convolves the columns of matrix src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src matrix to be filtered
       * @param kern filter kernel
       * @param dest destination matrix for the result
       *
       * @param allLines if true, all lines will be filtered, otherwise only
       *                 the lines needed for the downsampling specified in the
       *                 parameters will be filtered
       */
      void applyCol(const matrix<T>& src,
                          matrix<T>& dest,
                    const downsampling::parameters& param);

      /**
       * convolves the rows of matrix src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src matrix to be filtered
       * @param kern filter kernel
       * @param dest destination matrix for the result
       */
      void applyRow(const matrix<T>& src,
                          matrix<T>& dest,
                    const downsampling::parameters& param);

      protected:
      const kernel1D<T>* kernel_;

      private:
      kernel1D<T>* tmpKernel_;
    };

    /**
     * convolution algorithm for a matrix * kernel2D
     * The type T is the type of the elements of the vector an kernel1D
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template <class T,class U=T>
    class convolve2D : public convolve<T,U> {
    public:
      /**
       * default constructor
       */
      convolve2D();
      
      /**
       * destructor
       */
      ~convolve2D();
      
      /** 
       * try to get a Kernel from the parameters.
       * If it is not possible to get it, return false;
       * This will not copy the kernel, it just will keep a reference to it!
       * @param kern the container with the kernel.
       * @return true if kernel could be set and false otherwise
       */
      bool setKernel(const container* kern);
      
      /**
       * get kernel type
       */
      inline bool isKernelSeparable() const;

      /**
       * convolves the matrix src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src matrix to be filtered
       * @param kern filter kernel
       * @param dest result
       */
      void apply(const matrix<T>& src,
                       matrix<T>& dest,
                 const downsampling::parameters& param);

      /**
       * convolves the matrix src with the separable filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src matrix to be filtered
       * @param kern filter kernel
       * @param dest result
       */
      void applySep(const matrix<T>& src,
                          matrix<T>& dest,
                    const downsampling::parameters& param);

    protected:
      const kernel2D<T>* kernel_;
      const separableKernel<T>* sKernel_;
    private:
      kernel2D<T>* tmpKernel_;
      separableKernel<T>* tmpSKernel_;
    };
  };
}

#endif

