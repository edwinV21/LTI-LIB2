/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
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
 * \file   ltiMedianFilter.h
 *         Contains the class lti::medianFilter
 * \author Guy Wafo Moudhe
 * \author Pablo Alvarado
 * \date   16.03.2001
 * \date   06.04.2009
 *
 * $Id: ltiMedianFilter.h,v 1.5 2009-12-15 11:33:28 alvarado Exp $
 */


#ifndef _LTI_MEDIAN_FILTER_H_
#define _LTI_MEDIAN_FILTER_H_

#include "ltiDenoising.h"
#include "ltiMatrix.h"
#include "ltiQuickMedian.h"
#include "ltiMatrixProcessingInterface.h"

namespace lti {

  /**
   * Median filter
   * 
   * This class implements the median filter.
   *
   * The median filter is a non-linear rank filter, which assign to
   * each pixel the median value in a window centered at the pixel.
   * The median is the value in the window which has exactly the same
   * number of elements larger or equal to itself, than number of elements
   * smaller or equal to itself.
   *
   * The median value of an n x n kernel window is left in its center
   * position.
   *
   * The algorithm for lti::fmatrix uses the straightforward method, and
   * is not as fast as the histogram-based method used for lti::matrix<ubyte>.
   *
   * \code 
   * matrix<ubyte> src;          //obtained from somewhere
   * medianFunctor func;    // default kernel size 3x3
   * func.setKernelSize(5); // now has a 5x5 kernel
   *
   * matrix<ubyte> dest;         // contains the median filtered matrix<ubyte>
   * if (!func.apply(src, dest)) {
   *   // print error message if anything goes wrong.
   *   std::cerr << func.getStatusString() << "\n";
   * }
   *
   * \endcode
   *
   * @see lti::quickMedian
   *
   * @ingroup gNonLinearFiltering
   */
  class medianFilter : public denoising,
                       public matrixProcessingInterface<ubyte>,
                       public matrixProcessingInterface<float> {
  public:
    /**
     * The parameters for the class medianFilter
     */
    class parameters : public denoising::parameters {
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
       * Returns a pointer to a new instance of the parameters
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
       * Write the parameters in the given ioHandler
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
       * Size of the kernel to be used.
       *
       * The kernel size should be an odd number. Otherwise the values
       * cannot be appropriately assigned to the center pixel.
       *
       * Default: 3 (which means a 3x3 kernel)
       */
      int kernelSize;

    };

    /**
     * Default constructor
     */
    medianFilter();

    /**
     * Constructor. Initialize the parameters with \p par
     */
    medianFilter(const parameters& par);

    /**
     * Constructor. Initialize the parameters with the given values
     *
     * @param kernelSize the kernel is square with the give size
     *                   on one side for 2D apply methods, or just
     *                   kernelSize size for 1D apply methods.
     * @param boundaryType optional value for the boundary type
     */
    medianFilter(const int kernelSize,
                 const eBoundaryType boundaryType=Constant);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    medianFilter(const medianFilter& other);

    /**
     * Destructor
     */
    virtual ~medianFilter();

    /**
     * Returns the name of this type ("medianFilter")
     */
    const std::string& name() const;

    /**
     * @name Media for matrices and channels
     */
    //@{

    /**
     * Operates on the given argument.
     * @param srcdest fmatrix with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fmatrix& srcdest) const;

    /**
     * Operates on the given argument.
     * @param srcdest matrix<ubyte> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src fmatrix with the source data.
     * @param dest fmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fmatrix& src,fmatrix& dest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const;

    /**
     * Operates on the given argument.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Operates on the given argument.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;
    //@{

    /**
     * @name Media for vectors
     */
    //@{
 
    /**
     * Operates on the given argument.
     * @param srcdest fvector with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fvector& srcdest) const;

    /**
     * Operates on the given argument.
     * @param srcdest vector<ubyte> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<ubyte>& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src fvector with the source data.
     * @param dest fvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fvector& src,fvector& dest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src vector<ubyte> with the source data.
     * @param dest vector<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<ubyte>& src,vector<ubyte>& dest) const;
    //@{

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    medianFilter& copy(const medianFilter& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual medianFilter* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual medianFilter* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Shortcut for resizing the kernel.  Following code using the shortcut
     *
     * \code
     * medianFilter theMedFilter;
     * theMedFilter.setKernelSize(5);
     * \endcode
     *
     * is equivalent to
     *
     * \code
     * medianFilter theMedFilter;
     *
     * medianFilter::parameters param(theMedFilter.getParameters());
     * param.kernelSize = 5;
     * theMedFilter.setParameters(param);
     * \endcode
     */
    void setKernelSize(int newKernelSize);
  protected:
    
    /**
     * @name Workers for matrices and channels
     */
    //@{

    /**
     * Runs inside the image src
     */
    void histogramMethodMiddle(const matrix<ubyte>& src,
                               matrix<ubyte>& dest,
                               ivector& histogram,
                               int& lessThanMedian,
                               int row,
                               int& col,
                               ubyte& median,
                               int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Zero
     */
    bool histogramMethodZero(const matrix<ubyte>& src,
                              matrix<ubyte>& dest,
                              int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Constant
     */
    bool histogramMethodConstant(const matrix<ubyte>& src,
                                 matrix<ubyte>& dest,
                                 int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Periodic
     */
    bool histogramMethodPeriodic(const matrix<ubyte>& src,
                                 matrix<ubyte>& dest,
                                 int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Mirror
     */
    bool histogramMethodMirror(const matrix<ubyte>& src,
                               matrix<ubyte>& dest,
                               int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary NoBoundary
     */
    bool histogramMethodNoBoundary(const matrix<ubyte>& src,
                                   matrix<ubyte>& dest,
                                   int sizeOfKernel) const;

    /**
     * the histogramMethod apply the median filter to a matrix
     * of type matrix<ubyte>.In difference with the function realMedian,
     * histogramMethod use a histogram,which avoid to sort each time
     * the kernel
     */
    bool histogramMethod(const matrix<ubyte>& src, matrix<ubyte>& dest) const;

    /**
     * realMedian is a template function which apply the median filter
     * to a matrix of type T.
     * The median filter does not depend on
     * the type of the elements of a matrix.
     */
    template <class T>
    bool realMedian(const matrix<T>& src, matrix<T>& dest) const;

    //@}

    /**
     * @name Workers for vectors
     */
    //@{

    /**
     * Filter the inner block
     */
    void histogramMethodMiddle(const vector<ubyte>& src,
                               vector<ubyte>& dest,
                               ivector& histogram,
                               int& lessThanMedian,
                               int& col,
                               ubyte& median,
                               const int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Zero
     */
    bool histogramMethodZero(const vector<ubyte>& src,
                              vector<ubyte>& dest,
                              int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Constant
     */
    bool histogramMethodConstant(const vector<ubyte>& src,
                                 vector<ubyte>& dest,
                                 int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Periodic
     */
    bool histogramMethodPeriodic(const vector<ubyte>& src,
                                 vector<ubyte>& dest,
                                 int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Mirror
     */
    bool histogramMethodMirror(const vector<ubyte>& src,
                               vector<ubyte>& dest,
                               int sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary NoBoundary
     */
    bool histogramMethodNoBoundary(const vector<ubyte>& src,
                                   vector<ubyte>& dest,
                                   int sizeOfKernel) const;

    /**
     * the histogramMethod apply the median filter to a vector
     * of type vector<ubyte>.In difference with the function realMedian,
     * histogramMethod use a histogram,which avoid to sort each time
     * the kernel
     */
    bool histogramMethod(const vector<ubyte>& src, vector<ubyte>& dest) const;

    /**
     * realMedian is a template function which apply the median filter
     * to a vector of type T.
     * The median filter does not depend on
     * the type of the elements of a vector.
     */
    template <class T>
    bool realMedian(const vector<T>& src, vector<T>& dest) const;

    //@}


  };
}

#endif
