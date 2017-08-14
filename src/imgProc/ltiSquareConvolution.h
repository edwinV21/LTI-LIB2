/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiSquareConvolution.h
 *         Contains the class lti::squareConvolution to compute the convolution
 *         with a homogeneus square kernel in an efficient way.
 * \author Pablo Alvarado
 *
 * \date   13.10.2000
 *
 * revisions ..: $Id: ltiSquareConvolution.h,v 1.1 2008-12-27 20:14:56 alvarado Exp $
 */

#ifndef _LTI_SQUARE_CONVOLUTION_H_
#define _LTI_SQUARE_CONVOLUTION_H_

#include "ltiObject.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiBoundaryType.h"

namespace lti {
  /**
   * Square convolution
   *
   * In most cases you would prefer to use integral images rather 
   * than this class.
   *
   * This is an efficient implementation of the convolution with a
   * square or rectangular kernel.  The template parameter T defines
   * the type of the matrix or vector to be filtered, and A represents
   * the accumulator class, which allows different modi for the
   * convolution.
   *
   * The class A must implement following members:
   *  - constructor(int)          for initialization with the value 0.
   *  - operator+=(T x)           for accumulation,
   *  - operator-=(T x)           for "deaccumulation",
   *  - operator=(T x)            for assignation of a value of type T
   *  - operator/(T x,T y) const  returns f(x)/y instead of x/y.
   *                              For example for L2 norm f(x) = sqrt(x).
   *  - operator T() const        returns the contents casted to type T
   *
   * This way, you can use any simple-types (as int, char, double...)
   * as accumulator for a linear filter, or a user-defined class for a
   * region normalization with any norm!  For an example of an accumulator
   * class see lti::chrominanzHistogram::l2accumulator<T>.
   *
   * Note that for convolving a rectangular filter kernel with a channel8,
   * you shoud use an instance squareConvolution<ubyte,int>.  If you forget
   * the "int" parameter, some quite noisy overflowing effects should be
   * expected!
   *
   *
   * @see parameters
   *
   * @ingroup gLinearFilters
   */
  template<typename T,class A=T>
  class squareConvolution : public functor {
  public:
    
    /**
     * The parameters for the class squareConvolution
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       *
       * Initializes a kernel of k x k size centered at (0,0)
       */
      parameters(const int k = 3);
      
      /**
       * Copy constructor
       *
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
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Initialize the parameters for a symmetric square kernel sizeXsize,
       * where the sum of all elements equals one.
       * If the size is even, it will be "fixed" to the next odd number
       */
      void initSquare(const int size);

      /**
       * Write the parameters in the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

        
      /**
       * Read the parameters from the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete = true);

      /**
       * Type of boundary outside the image.
       * 
       * @see eBoundaryType
       *
       * Default value: Zero
       */
      eBoundaryType boundaryType;

      /**
       * Kernel size
       *
       * The rectangular kernel will have the value "1/kernelNorm"
       * between the limits specified by this rectangle.  When
       * convolving a vector, the "y" coordinates of this kernelSize
       * will be ignored.
       * Default value: rectangle(-1,-1,1,1), i.e. a 3x3 kernel
       */
      irectangle kernelSize;

      /**
       * Kernel norm
       *
       * The norm is the value which will be considered as "1.0" for the
       * elements of the kernel.  The default value is 9 (for a 3x3 kernel).
       * You can consider the square filter always as a kernel with nxn
       * "ones".
       * After the convolution, the resulting image will be divide by this
       * value.  The implementation combines the convolution with this
       * normalization!
       */
      T kernelNorm;
    };

    /**
     * Default constructor
     */
    squareConvolution();

    /**
     * Constructor to initialize the parameters with an square filter
     * of k x k size.
     */
    squareConvolution(const int k);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    squareConvolution(const squareConvolution& other);

    /**
     * Destructor
     */
    virtual ~squareConvolution();

    /**
     * Returns the name of this type ("squareConvolution")
     */
    virtual const std::string& name() const;

    /**
     * Operates on the given argument.
     *
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Operates on the given argument.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<T>& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<T>& src,vector<T>& dest) const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    squareConvolution<T,A>& copy(const squareConvolution& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual squareConvolution<T,A>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual squareConvolution<T,A>* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Applies the vertical part of the kernel to the matrix
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    matrix<T>& applyCol(const matrix<T>& src,matrix<T>& dest) const;

  private:
    /**
     * Calculate the real index in an vector of size srcSize for a
     * mirrored boundary
     */
    inline int getMirrorIndex(const int x,
                              const int srcSize,
                              bool& mirror) const;

  };



}

#include "ltiSquareConvolution_template.h"

#endif
