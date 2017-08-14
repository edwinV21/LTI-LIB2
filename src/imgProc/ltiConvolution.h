/*
 * Copyright (C) 1998-2004
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
 * \file   ltiConvolution.h
 *         Contains the class convolution.
 * \author Pablo Alvarado
 * \date   19.04.99
 *
 * $Id: ltiConvolution.h,v 1.8 2007-09-29 00:37:11 alvarado Exp $
 */
#ifndef _LTI_CONVOLUTION_H_
#define _LTI_CONVOLUTION_H_

#include "ltiFunctor.h"
#include "ltiMatrixProcessingInterface.h"
#include "ltiBoundaryType.h"
#include "ltiArray.h"
#include "ltiMatrix.h"
#include "ltiTypes.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"

namespace lti {
  /**
   * Convolution %functor.
   *
   * This functor convolves a %filter kernel (given in the
   * convolution::parameters) with a vector or matrix.
   *
   * The kernel must be have of the kernel types provided by the library:
   * - lti::kernel1D<T> if you want to filter vectors
   * - lti::kernel2D<T> for non-separable kernels (to filter channels)
   * - lti::sepKernel<T> for separable kernels (to filter channels)
   *
   * @see lti::gaussKernel1D, lti::gaussKernel2D, lti::gaborKernel
   *
   * Even if the setKernel() method accepts lti::container objects, only
   * the previous ones are accepted.
   *
   * In the normal case, the type of the filter kernel has to be the
   * same as the type of the channel (or matrix) to be filter.  For
   * example, if you want to filter a channel (of floats) you will
   * require a kernel of floats.
   *
   * If you try to use different types for the kernel and the matrix, this
   * functor will try to cast the kernel to the proper type first (and this
   * will take some time).
   *
   * For the convolution of kernels and matrices (or channels) of
   * fixed point types (e.g. channel8), you must make use of the norm-term in
   * the kernel. (see kernel1D<T>::norm).
   *
   * \b Note that the convolution with sepKernels that are symmetric,
   * e.g gaussKernel2D or anti-symmetric like the gradient part of
   * sobelKernelX is optimized. It is assumed (by definition) that the
   * middle element of an anti-symmetric kernel is 0.
   *
   * @ingroup gLinearFilters
   *
   * Example using a gaussian kernel
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data,result;
   *
   * // ... initialize channel here ...
   *
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::channel::value_type> kernel(5,1.3);
   *
   * lti::convolution filter;                        // convolution operator
   * lti::convolution::parameters param;             // parameters
   * param.setKernel(kernel);                        // use the gauss kernel
   * filter.setParameters(param);                    // use given parameters
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   * \endcode
   *
   * You can also create the functor with a given filter kernel:
   *
   * \code
   * lti::convolution filter(lti::gaussKernel2D<lti::channel::value_type>(5,1.3);
   * filter.apply(data); // parameters already set in the constructor!
   * \endcode
   *
   * The filter kernel can also be change, changing the parameters or with
   * the shortcut setKernel():
   *
   * \code
   * param.setKernel(anotherKernel);
   * filter.setParamters(param);
   *
   * // this is equivalent to:
   *
   * filter.setKernel(anotherKernel);
   *
   * \endcode 
   */
  class convolution : public functor,
                      public matrixProcessingInterface<float>,
                      public matrixProcessingInterface<ubyte>,
                      public matrixProcessingInterface<double> {
  public:
    /**
     * Parameters of the lti::convolution functor
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
      virtual ~parameters();

      /**
       * Copy the contents of other parameter object
       * @param other the parameters object to be copied
       * @return a reference to this object
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of this parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of this parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Returns the kernel in use.  If it is not set yet, an
       * lti::invalidParameters exception will be thrown
       * @return a const reference to the filter kernel.
       */
      const container& getKernel() const;

      /**
       * Sets the filter kernel to be used.
       * A copy of the given argument will be made!
       * @param aKernel the filter kernel to be used
       */
      void setKernel(const container& aKernel);

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

      /**
       * Boundary Type
       *
       * How to treat the input channel or image outside the boundaries.
       *
       * Default value: eBoundaryType::Zero
       */
      eBoundaryType boundaryType;

    protected:

      /**
       * pointer to the filter kernel copy
       */
      container* kernel_;
    };

    /**
     * default constructor
     */
    convolution();

    /**
     * default constructor with parameters
     */
    convolution(const parameters& par);

    /**
     * construct a convolution functor with a parameters set
     * which includes the given filter kernel.
     *
     * @param aKernel kernel object with which you want to convolve.
     * @param boundary Boundary assumption (Zero, Mirror, Periodic, Constant or
     *                 NoBoundary).  @see eBoundaryType
     */
    convolution(const container& aKernel,
                const eBoundaryType& boundary = Zero);

    /**
     * copy constructor
     * @param other the other functor to be copied
     */
    convolution(const convolution& other);

    /**
     * destructor
     */
    virtual ~convolution();

    /**
     * operates on the given argument.
     * @param srcdest channel8 with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * operates on the given argument.
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fmatrix& srcdest) const;

    /**
     * operates on the given argument.
     * @param srcdest dmatrix with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dmatrix& srcdest) const;


    /**
     * operates on the given argument.
     * @param srcdest vector<ubyte> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<ubyte>& srcdest) const;

    /**
     * operates on the given argument.
     * @param srcdest vector<channel::value_type> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fvector& srcdest) const;

    /**
     * operates on the given argument.
     * @param srcdest dvector with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dvector& srcdest) const;


    /**
     * operates on a copy of the given parameters.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fmatrix& src,fmatrix& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<ubyte> with the source data.
     * @param dest vector<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<ubyte>& src,
                     vector<ubyte>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<channel::value_type> with the source data.
     * @param dest vector<channel::value_type> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fvector& src,
                     fvector& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dvector& src,
                     dvector& dest) const;

    /*
     * Split the given color image in its RGB components, filter each of them
     * (as lti::channel) and merge the results.
     * @param src image to be filtered.
     * @param dest resulting filtered image.
     * @return true if successful, false otherwise
     */
    //bool apply(const matrix<rgbaPixel>& src,
    //                 matrix<rgbaPixel>& dest) const;

    /*
     * Split the given color image in its RGB components, filter each of them
     * (as lti::channel) and merge the results.
     * @param srcdest image to be filtered.  The result is left here too.
     * @return true if successful, false otherwise
     */
    //bool apply(matrix<rgbaPixel>& srcdest) const;

    /**
     * Copy data of "other" functor.
     *
     * @param other The convolution functor to be copied.
     */
    convolution& copy(const convolution& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual convolution* clone() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual convolution* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut to set the filter kernel in the functor parameters.
     * The other parameters remain unchanged.
     */
    void setKernel(const container& aKernel);

  private:
    /**
     * This is the accumulator class needed by the convolution helper to
     * act as a linear convolution operator for gray valued images.
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class accumulator {
    public:
      /**
       * Default constructor
       */
      accumulator();

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
       * - src:                   srcL  *  middle  *  srcR
       * - filter:                *  *  *  middle  *  *  *
       * - used filter part:      *  *  *  middle
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
       * set norm
       */
      inline void setNorm(const T& norm);
    protected:
      /**
       * the accumulated value
       */
      U state_;

      /**
       * norm
       */
      T norm_;
    };
  };
}

#endif
