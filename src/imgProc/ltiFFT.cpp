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
 * \file   ltiFFT.cpp
 *         Contains the class lti::fft, 
 *         which performs the Fast Fourier Transform
 * 
 * \author Stefan Syberichs
 * \date   06.12.99
 * \author Pablo Alvarado
 * \date   03.07.12
 *
 * revisions ..: $Id: ltiFFT.cpp,v 1.3 2012-07-04 02:43:36 alvarado Exp $
 */

#include "ltiFFT.h"
#include "ltiFFTinit.h"

namespace lti {
  // --------------------------------------------------
  // fft::parameters
  // --------------------------------------------------

  // default constructor
  fft::parameters::parameters()
    : functor::parameters() {
    
    mode = Cartesian;
  }

  // copy constructor
  fft::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  fft::parameters::~parameters() {
  }

  // copy member

  fft::parameters&
  fft::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    mode = other.mode;

    return *this;
  }

  // alias for copy method
  fft::parameters&
  fft::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& fft::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  fft::parameters*
  fft::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  fft::parameters*
  fft::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool fft::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"mode",mode);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool fft::parameters::read(ioHandler& handler,
                             const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"mode",mode);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // fft
  // --------------------------------------------------

  // default constructor
  fft::fft()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  fft::fft(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  fft::fft(const fft& other)
    : functor() {
    copy(other);
  }

  // destructor
  fft::~fft() {
  }

  // copy member
  fft& 
  fft::copy(const fft& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  fft&
  fft::operator=(const fft& other) {
    return (copy(other));
  }

  // class name
  const std::string& fft::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  fft* fft::clone() const {
    return new fft(*this);
  }

  // create a new instance
  fft* fft::newInstance() const {
    return new fft();
  }

  // return parameters
  const fft::parameters&
  fft::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  /// apply real FFT to real vectors
  //  size of output is N/2+1 !
  template<typename T>
  bool fft::worker(const vector<T>& realInput,
                   const eCoordinateSystem mode,
                   vector<T>& realOutput,
                   vector<T>& imagOutput) const {
    int originalSize, ldn;
    int *ip,  n, m, k;
    T  *w;
    vector<T> help;

    // size must have a size with the size = 2^n
    originalSize = realInput.size();

    ldn = static_cast<int>(ceil(log(double(originalSize))/log(2.0)));
    n   = 1 << ldn;
    const int n2h = n/2;

    if (n!=originalSize) {
      help.assign(n, T(0));
      help.fill(realInput, (n-originalSize)/2, n);
    } else {
      help.copy(realInput);
    }

    // initialize fft

    // ip: work area for bit reversal
    ip = internal::fft::alloc_1d<int>(2 + (int) sqrt(n + 0.5));
    m = n * 5 / 4 + n / 4;

    // w: cos/sin table
    w = internal::fft::alloc_1d<T>(m);
    ip[0] = 0; // flag to indicate initialization

    //resize output data
    realOutput.allocate(1+n2h);
    imagOutput.allocate(1+n2h);

    // FFT
    internal::fft::vrdft(n, 1, help, ip, w); // FFT

    // generate output data
    if(mode != lti::Cartesian) {
      // POLAR
      realOutput[0] = help[0];
      imagOutput[0] = 0.0f; // the angle at w=0 must always be zero (is odd)

      for(k=1; k<n2h; k++) {
        const int k2 = (k << 1); // k*2
        const T& re = help[k2];
        const T& im = help[k2+1];

        realOutput[k] = sqrt(re*re+im*im);
        imagOutput[k] = atan2(im,re);
      }

      realOutput[n2h] = abs(help[1]);
      imagOutput[n2h] = (help[1] < 0) ? static_cast<T>(Pi) : 0.0f;
    } else {
      // CARTESIC
      realOutput[0] = help[0];
      imagOutput[0] = 0.0f;

      for(k=1; k<n2h; k++){
        const int k2 = (k << 1); // k*2
        realOutput[k] = help[k2];
        imagOutput[k] = help[k2+1];
      }

      realOutput[n2h] = help[1];
      imagOutput[n2h] = 0.0f;
    }

    // delete memory in the heap!

    internal::fft::free_1d<T>(w);
    internal::fft::free_1d<int>(ip);

    return true;
  } //apply for vectors

  bool fft::apply(const fvector& realInput,
                  fvector& realOutput,
                  fvector& imagOutput) const {
    return worker(realInput,getParameters().mode,realOutput,imagOutput);
  }

  bool fft::apply(const fvector& realInput,
                  const eCoordinateSystem mode,
                  fvector& realOutput,
                  fvector& imagOutput) const{
    return worker(realInput,mode,realOutput,imagOutput);
  }

  bool fft::apply(const dvector& realInput,
                  dvector& realOutput,
                  dvector& imagOutput) const {
    return worker(realInput,getParameters().mode,realOutput,imagOutput);    
  }

  /// apply real FFT to real vectors
  //  size of output is N/2+1 !
  bool fft::apply(const dvector& realInput,
                  const eCoordinateSystem mode,
                  dvector& realOutput,
                  dvector& imagOutput) const{
    return worker(realInput,mode,realOutput,imagOutput);
  } //apply for vectors

  /// apply real FFT to real matrix<float>s
  bool fft::apply(const fmatrix& realInput,
                  fmatrix& realOutput,
                  fmatrix& imagOutput) const {
    return apply(realInput,getParameters().mode,realOutput,imagOutput);
  }


  /// apply real FFT to real matrix<float>s
  bool fft::apply(const fmatrix& realInput,
                  const eCoordinateSystem mode,
                  fmatrix& realOutput,
                  fmatrix& imagOutput) const{

    int originalXsize, originalYsize, ldn;
    int k1, k2 ;

    // buffers needed by rdft2d
    int *ip, ny, nx, n;
    float *t, *w;
    lti::matrix<float> help;

    // size must always be as 2^n
    originalXsize = realInput.columns();
    originalYsize = realInput.rows();

    // ny and nx always are powers of two
    ldn = static_cast<int>(ceil(log(double(originalYsize))/log(2.0)));
    ny   = 1 << ldn; // n = 2^ldn

    ldn = static_cast<int>(ceil(log(double(originalXsize))/log(2.0)));
    nx   = 1 << ldn; // n = 2^ldn

    if ((ny != help.rows()) || (nx != help.columns())) {
      help.assign(ny, nx, 0.0f);
      help.fill(realInput,0,0); // copy the original image at(0,0)
    } else {
      help.copy(realInput);
    }

    // initialize fft
    const int nxh = nx/2;
    const int nyh = ny/2;

    // twice the height to store complex values
    t = internal::fft::alloc_1d<float>(2 * ny);

    n = lti::max(ny, nxh);
    ip = internal::fft::alloc_1d<int>(2 + (int) sqrt(n + 0.5));
    n = lti::max(ny * 5 / 4, nx * 5 / 4) + nx / 4;
    w = internal::fft::alloc_1d<float>(n);

    // resize output buffer
    realOutput.allocate(ny, nx);
    imagOutput.allocate(ny, nx);

    // FFT
    ip[0] = 0; // flag for initialization required of ip and w
    internal::fft::rdft2d(ny, nx, 1, help, t, ip, w); // FFT

    // generate output matrices
    if(mode != lti::Cartesian) {

      // Polar coordinates
      float im,re;

      // first half rows (except the first)
      for(k1=1; k1 < nyh; k1++) {
        const int mny = ny-k1;

        const fvector& h = help.getRow(k1);
        fvector& reOut = realOutput.getRow(k1);
        fvector& imOut = imagOutput.getRow(k1);
        fvector& creOut = realOutput.getRow(mny);
        fvector& cimOut = imagOutput.getRow(mny);

        for (k2 = 1; k2 < nxh; k2++) {
          const int k22 = (k2 << 1); // x2
          const int ck2 = nx-k2;
          re = h[k22];
          im = h[k22+1];

          creOut[ck2] =   reOut[k2] = sqrt(re*re+im*im);
          cimOut[ck2] = -(imOut[k2] = atan2(im,re));
        }

        re = h[0];
        im = h[1];
        reOut[0]  = creOut[0] = sqrt(re*re+im*im);
        cimOut[0] = -(imOut[0] = atan2(im,re));

        re = help[mny][1];
        im = help[mny][0];
        reOut[nxh] = creOut[nxh] = sqrt(re*re+im*im);
        imOut[nxh] = -(cimOut[nxh] = atan2(im,re));
      }

      // next half rows without the last four columns
      for(; k1 < ny; k1++) {
        const int mny = ny-k1;

        const fvector& h = help.getRow(k1);
        fvector& reOut = realOutput.getRow(k1);
        fvector& imOut = imagOutput.getRow(k1);
        fvector& creOut = realOutput.getRow(mny);
        fvector& cimOut = imagOutput.getRow(mny);

        for(k2 = 1; k2 < nxh; k2++) {
          const int k22 = (k2 << 1); // x2
          const int ck2 = nx-k2;

          re = h[k22];
          im = h[k22+1];

          creOut[ck2] =   reOut[k2] = sqrt(re*re+im*im);
          cimOut[ck2] = -(imOut[k2] = atan2(im,re));
        }
      }

      // very first row
      for(k2=1; k2 < nxh; k2++){
        const int k22 = (k2 << 1); // x2
        const int ck2 = nx-k2;

        re = help[0][k22];
        im = help[0][k22+1];
        realOutput[0][k2]  =   realOutput[0][ck2] = sqrt(re*re+im*im);
        imagOutput[0][ck2] = -(imagOutput[0][k2]  = atan2(im,re));
      }

      realOutput[0][0] = abs(help[0][0]);
      imagOutput[0][0] = (help[0][0]<0) ? static_cast<float>(Pi) : 0.0f;

      realOutput[0][nxh] = abs(help[0][1]);
      imagOutput[0][nxh] = (help[0][1]<0) ? static_cast<float>(Pi) : 0.0f;

      realOutput[nyh][0] = abs(help[nyh][0]);
      imagOutput[nyh][0] = (help[nyh][0]<0) ? static_cast<float>(Pi) : 0.0f;

      realOutput[nyh][nxh] = abs(help[nyh][1]);
      imagOutput[nyh][nxh] = (help[nyh][1]<0) ? static_cast<float>(Pi) : 0.0f;

    } else { //Cartesic

      // the first half of the rows (except the first)
      for(k1=1; k1 < nyh; k1++) {
        const int mny = ny-k1;
        const fvector& h= help.getRow(k1);
        fvector& reOut  = realOutput.getRow(k1);
        fvector& imOut  = imagOutput.getRow(k1);
        fvector& creOut = realOutput.getRow(mny);
        fvector& cimOut = imagOutput.getRow(mny);

        /*
         * a[k1][2*k2] = R[k1][k2] = R[n1-k1][n2-k2], 
         * a[k1][2*k2+1] = I[k1][k2] = -I[n1-k1][n2-k2], 
         *    0<k1<n1, 0<k2<n2/2, 
         */
        for(k2 = 1; k2 < nxh; k2++) {
          const int k22 = (k2 << 1); // x2
          const int ck2 = nx-k2;
          creOut[ck2] =   reOut[k2] = h[k22];
          cimOut[ck2] = -(imOut[k2] = h[k22+1]);
        }

        reOut[0]   =   creOut[0]   = h[0];
        cimOut[0]  = -(imOut[0]    = h[1]);
        reOut[nxh] =   creOut[nxh] = help[mny][1];
        imOut[nxh] = -(cimOut[nxh] = help[mny][0]);
      }

      // the next half (the last four lines of the previous half not required)
      for(; k1 < ny; k1++) {
        const int mny = ny-k1;

        const fvector& h = help.getRow(k1);
        fvector& reOut  = realOutput.getRow(k1);
        fvector& imOut  = imagOutput.getRow(k1);
        fvector& creOut = realOutput.getRow(mny);
        fvector& cimOut = imagOutput.getRow(mny);

        for(k2 = 1; k2 < nxh; k2++) {
          const int k22 = (k2 << 1); // x2
          const int ck2 = nx-k2;

          creOut[ck2] =   reOut[k2] = h[k22];
          cimOut[ck2] = -(imOut[k2] = h[k22+1]);
        }
      }

      // The very first row
      for(k2=1; k2 < nxh; k2++){
        const int k22 = (k2 << 1); // x2
        const int ck2 = nx-k2;

        realOutput[0][k2]  =   realOutput[0][ck2] = help[0][k22];
        imagOutput[0][ck2] = -(imagOutput[0][k2]  = help[0][k22+1]);
      }

      realOutput[0][0] = help[0][0];
      imagOutput[0][0] = 0;

      realOutput[0][nxh] = help[0][1];
      imagOutput[0][nxh] = 0;

      realOutput[nyh][0] = help[nyh][0];
      imagOutput[nyh][0] = 0;

      realOutput[nyh][nxh] = help[nyh][1];
      imagOutput[nyh][nxh] = 0;

    } // end else if polar

    // Delete arrays

    internal::fft::free_1d<float>(w);
    internal::fft::free_1d<int>(ip);
    internal::fft::free_1d<float>(t);

    return true;

  } //apply for matrix<float>

  

}

