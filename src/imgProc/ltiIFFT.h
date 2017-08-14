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
 * \file   ltiIFFT.h
 *         Contains the class lti::fft, 
 *         which performs the Fast Fourier Transform
 * 
 * \author Stefan Syberichs
 * \date   06.12.99
 * \author Pablo Alvarado
 * \date   06.09.09
 *
 * revisions ..: $Id: ltiIFFT.h,v 1.2 2012-07-04 02:44:08 alvarado Exp $
 */

#ifndef _LTI_IFFT_H_
#define _LTI_IFFT_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiCoordinateSystem.h"
#include "ltiFFT.h"

namespace lti {

  /**
   * Class ifft for the inverse IFFT
   *
   * lti::ifft is a class to perform the Inverse Fast Fourier Transforms on
   * lti::vectors and lti::channels with a format coincident with the lti::fft
   * output.  The input can be either in polar or in cartesic format,
   * specified by the parameter \c mode.
   *
   * The iFFT for vectors expects only one half of the Fourier coefficients per
   * dimension (real and imaginary parts), since the other half of the
   * coefficients are hermitian symmetric due to the fact that the input signal
   * is real.
   *
   * The iFFT on channels expects two full-sized input matrices (i.e the size
   * of the output data) even though only half of the size would be required.
   *
   * In both versions (for vector and matrix), the input data must be of a
   * power of 2 size.
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
   * \warning Note that the cartesic input (real and imaginary parts) computes
   *          faster.
   *
   * \warning This iFFT corresponds to an normalized DFT.
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
   * @see fft::parameters.
   *
   * @ingroup gLinearAlgebra
   */
  class ifft : public functor {
  public:

    /**
     * The parameters for the class fft
     */
    typedef fft::parameters parameters;

    /**
     * Default constructor
     */
    ifft();

    /**
     * Construct a functor using the given parameters
     */
    ifft(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    ifft(const ifft& other);

    /**
     * Destructor
     */
    virtual ~ifft();
    
    /**
     * Compute the inverse FFT for the complex values given by real and imag 
     * (or magnitude and angle inf the polar mode is used)
     * vectors into the dest.
     *
     * The size of the input data is half of the input data.
     *
     * @param real the real output data (e.g. IFFT: the real part)
     * @param imag the imaginary output data (e.g. IFFT: the imaginary part)
     * @param dest the real input data (constant) 
     *
     */
    bool apply(const fvector& real,
               const fvector& imag,
               fvector& dest) const;

    /**
     * Compute the inverse FFT for the complex values given by real and imag 
     * (or magnitude and angle inf the polar mode is used)
     * vectors into the dest.
     *
     * The size of the input data is half of the input data.
     *
     * @param real the real output data (e.g. IFFT: the real part)
     * @param imag the imaginary output data (e.g. IFFT: the imaginary part)
     * @param dest the real input data (constant) 
     *
     */
    bool apply(const dvector& real,
               const dvector& imag,
               dvector& dest) const;

    /**
     * Compute the inverse FFT for the complex values given by real and imag 
     * (or magnitude and angle inf the polar mode is used)
     * channels into the dest.
     *
     * The size of the input data is half of the input data.
     *
     * @param real the real output data (e.g. IFFT: the real part)
     * @param imag the imaginary output data (e.g. IFFT: the imaginary part)
     * @param dest the real input data (constant) 
     *
     */
    bool apply(const fmatrix& real,
               const fmatrix& imag,
               fmatrix& dest) const;

    /**
     * Compute the inverse FFT for the complex values given by real and imag 
     * (or magnitude and angle inf the polar mode is used)
     * vectors into the dest.
     *
     * The size of the input data is half of the input data.
     *
     * @param real the real output data (e.g. IFFT: the real part)
     * @param imag the imaginary output data (e.g. IFFT: the imaginary part)
     * @param mode indicates if the previous two arguments are in lti::Cartesian
     *             or in lti::Polar.
     * @param dest the real input data (constant) 
     *
     */
    bool apply(const fvector& real,
               const fvector& imag,
               const eCoordinateSystem mode,
               fvector& dest) const;

    /**
     * Compute the inverse FFT for the complex values given by real and imag 
     * (or magnitude and angle inf the polar mode is used)
     * vectors into the dest.
     *
     * The size of the input data is half of the input data.
     *
     * @param real the real output data (e.g. IFFT: the real part)
     * @param imag the imaginary output data (e.g. IFFT: the imaginary part)
     * @param mode indicates if the previous two arguments are in lti::Cartesian
     *             or in lti::Polar.
     * @param dest the real input data (constant) 
     *
     */
    bool apply(const dvector& real,
               const dvector& imag,
               const eCoordinateSystem mode,
               dvector& dest) const;

    /**
     * Compute the inverse FFT for the complex values given by real and imag 
     * (or magnitude and angle inf the polar mode is used)
     * channels into the dest.
     *
     * The size of the input data is half of the input data.
     *
     * @param real the real output data (e.g. IFFT: the real part)
     * @param imag the imaginary output data (e.g. IFFT: the imaginary part)
     * @param mode indicates if the previous two arguments are in lti::Cartesian
     *             or in lti::Polar.
     * @param dest the real input data (constant) 
     *
     */
    bool apply(const fmatrix& real,
               const fmatrix& imag,
               const eCoordinateSystem mode,
               fmatrix& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ifft& copy(const ifft& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ifft& operator=(const ifft& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual ifft* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ifft* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * Worker does the real job
     */
    template<typename T>
    bool worker(const vector<T>& real,
                const vector<T>& imag,
                const eCoordinateSystem mode,
                vector<T>& dest) const;

  };
}

#endif

