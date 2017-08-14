/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2009
 * Stefan Syberichs, Pablo Alvarado
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
 * \file   ltiFFT.h
 *         Contains the class lti::fft, 
 *         which performs the Fast Fourier Transform
 * 
 * \author Stefan Syberichs
 * \date   06.12.99
 * \author Pablo Alvarado
 * \date   03.07.12
 *
 * revisions ..: $Id: ltiFFT.h,v 1.2 2012-07-04 02:43:36 alvarado Exp $
 */

#ifndef _LTI_FFT_H_
#define _LTI_FFT_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiCoordinateSystem.h"

namespace lti {

  /**
   * Class fft
   *
   * lti::fft is a class to perform Fast Fourier Transforms on lti::vectors and
   * lti::channels.  The output can be either in polar or in cartesic format,
   * specified by the parameter \c mode. 
   *
   * The vector FFT generates only one half of the Fourier coefficients per
   * dimension (real and imaginary parts), since the other half of the
   * coefficients are hermitian symmetric due to the fact that the input signal
   * is real.
   *
   * The FFT on channels produces two full-sized output matrices (i.e the size
   * of the input data) even though only half of the size is required.  This is
   * done this way to simplify visualization of the spectrum.
   *
   * In both versions (for vector and matrix), the output data is automatically
   * padded to a power of 2.
   *
   * In the case of channels, the DC component of the signal is in the
   * upper-left corner of the two-dimensional FFT, and in the case of vectors
   * it is placed at index 0.
   *
   * The apply-methods are based on fast FFT-routines written by Takuya Ooura
   * (the original code can be found 
   * <a href="http://www.kurims.kyoto-u.ac.jp/~ooura/fft.html">here</a>) that
   * have been adapted for the use on lti::vectors and channels. 
   *
   * \warning Note that the cartesic output computes faster.
   *
   * \warning This FFT corresponds to an unnormalized DFT.  The normalization
   *          is computed in the lti::ifft.
   *
   * Usage:
   *  \code
   *  #include "ltiFFT.h"
   *  #include "ltiIFFT.h"
   *
   *  lti::fft fft2d;     // for 2-dimensional FFT
   *  lti::ifft ifft2d;   // for 2-dimensional inverse FFT
   *
   *  lti::fft::parameters par2d;
   *  lti::ifft::parameters ipar2d;
   *
   *  par2d.mode = lti::fft::Polar;
   *
   *  ifft2d.setParameters(par2d);
   *  fft2d.setParameters(par2d);
   *
   *  lti::channel R(256,256,0.0f);
   *  lti::channel back;
   *  lti::channel re,im;
   *
   *  R.at(127,127)=1.0f;
   *
   *  fft2d.apply(R, re, im);       // the actual FFT
   *  ifft2d.apply(re, im, back);   // inverse FFT
   *  \endcode
   *
   * \warning The interface of this class will change, as it is been modified
   * to make use of the faster fftw library.
   *
   * @see fft::parameters, ifft
   *
   * @ingroup gLinearAlgebra
   */
  class fft : public functor {
  public:

    /**
     * The parameters for the class fft
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
       * Format for the output vectors or channels.
       * 
       * It can be either lti::Cartesian or lti::Polar.  
       * Note that cartesian mode is faster, as it is computed directly.  The
       * polar version is constructed from the cartesian.
       *
       * Default: Cartesian
       */
      eCoordinateSystem mode;

    };

    /**
     * Default constructor
     */
    fft();

    /**
     * Construct a functor using the given parameters
     */
    fft(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fft(const fft& other);

    /**
     * Destructor
     */
    virtual ~fft();

    
    /**
     * Transform the \c src vector into the two components FFT (real/imaginary
     * or magnitude/angle), depending on parameters settings.
     *
     * The size of the output data is half of the input data (possibly padded
     * to power of 2).  The output vectors will have the dimension of the
     * padded input divided by 2 plus one.
     *
     * @param src the real input data (constant) 
     * @param real the real output data (e.g. FFT: the real part)
     * @param imag the imaginary output data (e.g. FFT: the imaginary part)
     *
     */
    bool apply(const fvector& src,fvector& real,fvector& imag) const;

    /**
     * Transform the \c src vector into the two components FFT (real/imaginary
     * or magnitude/angle), depending on parameters settings.
     *
     * The size of the output data is half of the input data (possibly padded
     * to power of 2).  The output vectors will have the dimension of the
     * padded input divided by 2 plus one.
     *
     * @param src the real input data (constant) 
     * @param real the real output data (e.g. FFT: the real part)
     * @param imag the imaginary output data (e.g. FFT: the imaginary part)
     *
     */
    bool apply(const dvector& src,dvector& real,dvector& imag) const;


    /**
     * Transform the \c src channel into the two components FFT (real/imaginary
     * or magnitude/angle), depending on parameters settings.
     *
     * The size of the output data is half of the input data (possibly padded
     * to power of 2).  The output vectors will have the dimension of the
     * padded input divided by 2 plus one.
     *
     * @param src the real input data (constant) 
     * @param real the real output data (e.g. FFT: the real part)
     * @param imag the imaginary output data (e.g. FFT: the imaginary part)
     *
     */
    bool apply(const fmatrix& src,fmatrix& real,fmatrix& imag) const;

    /**
     * Transform the \c src vector into the two components FFT (real/imaginary
     * or magnitude/angle), depending on parameters settings.
     *
     * The size of the output data is half of the input data (possibly padded
     * to power of 2).  The output vectors will have the dimension of the
     * padded input divided by 2 plus one.
     *
     * @param src the real input data (constant) 
     * @param mode lti::Cartesian or lti::Polar, overrides the value given in
     *             the parameters.
     * @param real the real output data (e.g. FFT: the real part)
     * @param imag the imaginary output data (e.g. FFT: the imaginary part)
     *
     */
    bool apply(const fvector& src,
               const eCoordinateSystem mode,
               fvector& real,
               fvector& imag) const;

    /**
     * Transform the \c src vector into the two components FFT (real/imaginary
     * or magnitude/angle), depending on parameters settings.
     *
     * The size of the output data is half of the input data (possibly padded
     * to power of 2).  The output vectors will have the dimension of the
     * padded input divided by 2 plus one.
     *
     * @param src the real input data (constant) 
     * @param mode lti::Cartesian or lti::Polar, overrides the value given in
     *             the parameters.
     * @param real the real output data (e.g. FFT: the real part)
     * @param imag the imaginary output data (e.g. FFT: the imaginary part)
     *
     */
    bool apply(const dvector& src,
               const eCoordinateSystem mode,
                     dvector& real,
                     dvector& imag) const;


    /**
     * Transform the \c src channel into the two components FFT (real/imaginary
     * or magnitude/angle), depending on parameters settings.
     *
     * The size of the output data is half of the input data (possibly padded
     * to power of 2).  The output vectors will have the dimension of the
     * padded input divided by 2 plus one.
     *
     * @param src the real input data (constant) 
     * @param mode lti::Cartesian or lti::Polar, overrides the value given in
     *             the parameters.
     * @param real the real output data (e.g. FFT: the real part)
     * @param imag the imaginary output data (e.g. FFT: the imaginary part)
     *
     */
    bool apply(const fmatrix& src,
               const eCoordinateSystem mode,
                     fmatrix& real,
                     fmatrix& imag) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fft& copy(const fft& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fft& operator=(const fft& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual fft* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual fft* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * This worker does the real job for vectors
     */
    template <typename T>
    bool worker(const vector<T>& src,
                const eCoordinateSystem mode,
                vector<T>& real,
                vector<T>& imag) const;
  };
}

#endif

