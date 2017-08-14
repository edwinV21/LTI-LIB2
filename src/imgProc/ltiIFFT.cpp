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
 * \file   ltiIFFT.cpp
 *         Contains the class lti::ifft, 
 *         which performs the Fast Fourier Transform
 * 
 * \author Stefan Syberichs
 * \date   06.12.99
 * \author Pablo Alvarado
 * \date   06.09.09
 *
 * revisions ..: $Id: ltiIFFT.cpp,v 1.2 2012-07-04 02:44:08 alvarado Exp $
 */

#include "ltiIFFT.h"
#include "ltiFFTinit.h"

namespace lti {

  // --------------------------------------------------
  // ifft
  // --------------------------------------------------

  // default constructor
  ifft::ifft()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  ifft::ifft(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  ifft::ifft(const ifft& other)
    : functor() {
    copy(other);
  }

  // destructor
  ifft::~ifft() {
  }

  // copy member
  ifft& 
  ifft::copy(const ifft& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  ifft&
  ifft::operator=(const ifft& other) {
    return (copy(other));
  }

  // class name
  const std::string& ifft::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  ifft* ifft::clone() const {
    return new ifft(*this);
  }

  // create a new instance
  ifft* ifft::newInstance() const {
    return new ifft();
  }

  // return parameters
  const ifft::parameters&
  ifft::getParameters() const {
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


  template<typename T>
  bool ifft::worker(const vector<T>& realInput,
                    const vector<T>& imagInput,
                    const eCoordinateSystem mode,
                    vector<T>& realOutput) const {

    int ldn, originalSize;
    int  k ;

    // buffers needed by rdft2d
    int *ip, n, m;
    T* w;

    if(realInput.size() != imagInput.size() ) {
      setStatusString("Input vector sizes do not match");
      return false;
    }

    // size must follow size = 2^n+1
    originalSize = realInput.size();

    // input data is just one half of the original spectrum
    // the original data are twice the size

    // n is always a power of 2
    ldn = static_cast<int>(ceil(log(double(originalSize-1))/log(2.0)));
    n = (1 << ldn); // n = 2^ldn

    // vector size 2x in space-domain
    realOutput.assign(n*2, T(0));

    const T factor = T(2)/T(n);

    // if input data polar:
    if(mode == lti::Polar) {

      realOutput[0] = factor*realInput[0]*cos(imagInput[0]);
      
      T sina, cosa;

      for (k =1; k < n; k++){
        const int k2 = (k << 1); // 2*k
        sincos(imagInput[k], sina, cosa);
        realOutput[k2]   = factor*realInput[k]*cosa;
        realOutput[k2+1] = factor*realInput[k]*sina;
      }

      realOutput[1] = factor*realInput[n]*cos(imagInput[n]);

    } else { // Cartesic
      realOutput[0] = factor*realInput[0];

      for (k =1; k < n; k++){
        const int k2 = (k << 1); // 2*k
        realOutput[k2]   = factor*realInput[k];
        realOutput[k2+1] = factor*imagInput[k];
      }

      realOutput[1] = factor*realInput[n];
    }

    // initialize fft
    ip = internal::fft::alloc_1d<int>(2 + (int) sqrt(2*n + 0.5));
    m = 2*n * 5 / 4 + (2*n) / 4;
    w = internal::fft::alloc_1d<T>(m);
    ip[0] = 0;

    // inverse FFT
    internal::fft::vrdft(2*n, -1, realOutput,  ip, w);

    // delete arrays
    internal::fft::free_1d<T>(w);
    internal::fft::free_1d<int>(ip);

    return true;

  } //apply for vectors


  /// apply Real Inv FFT to real vectors
  //  size of output is N !
  bool ifft::apply(const fvector& realInput,
                   const fvector& imagInput,
                   fvector& realOutput) const {
    return apply(realInput,imagInput,getParameters().mode,realOutput);
  }


  /// apply Real Inv FFT to real vectors
  //  size of output is N !
  bool ifft::apply(const dvector& realInput,
                   const dvector& imagInput,
                   dvector& realOutput) const {
    return worker(realInput,imagInput,getParameters().mode,realOutput);
  }

  bool ifft::apply(const fvector& realInput,
                   const fvector& imagInput,
                   const eCoordinateSystem mode,
                   fvector& realOutput) const {
    return worker(realInput,imagInput,mode,realOutput);
    
  }

  /// apply Real FFT to real vectors
  //  size of output is N/2+1 !
  bool ifft::apply(const dvector& realInput,
                   const dvector& imagInput,
                   const eCoordinateSystem mode,
                   dvector& realOutput) const{

    return worker(realInput,imagInput,mode,realOutput);

  } //apply for vectors


  bool ifft::apply(const fmatrix& realInput,
                   const fmatrix& imagInput,
                   fmatrix& realOutput) const {
    return apply(realInput,imagInput,getParameters().mode,realOutput);
  }

  /// apply Real FFT to real fmatrices
  bool ifft::apply(const fmatrix & realInput,
                   const fmatrix & imagInput,
                   const eCoordinateSystem mode,
                   fmatrix & realOutput) const{

    int originalXsize, originalYsize, ldn;

    // buffers needed by rdft2d
    int *ip, ny, nx, n, j1, j2;
    float *t, *w;

    if (realInput.rows() != imagInput.rows() ||
        realInput.columns() != imagInput.columns()) {
      setStatusString("Input fmatrices sizes do not match");
      return false;
    }

    // size must be always be 2^n
    originalXsize = realInput.columns();
    originalYsize = realInput.rows();

    // ny und nx are always powers of 2
    ldn = static_cast<int>(ceil(log(double(originalYsize))/log(2.0)));
    ny   = 1 << ldn; // n = 2^ldn

    ldn = static_cast<int>(ceil(log(double(originalXsize))/log(2.0)));
    nx   = 1 << ldn; // n = 2^ldn

    const float factor = 2.0f/(nx*ny);

    realOutput.assign(ny, nx, 0.0f);

    const int nyh = ny/2;
    const int nxh = nx/2;

    // for polar input data...
    if (mode != lti::Cartesian) {
      
      // Polar coordinates (this is very slow!)
      float sina, cosa;
      
      // First half of rows (except the first)
      for(j1=1; j1<nyh; j1++) {
        const fvector& reIn = realInput.getRow(j1);
        const fvector& imIn = imagInput.getRow(j1);
        fvector& out = realOutput[j1];
        
        for(j2=1; j2<nxh;j2++){
          const int j22 = (j2 << 1); // x2
          sincos(imIn[j2], sina, cosa);
          const float mag = factor*reIn[j2];
          out[j22]   = mag*cosa;
          out[j22+1] = mag*sina;
        }

        sincos(imIn[0], sina, cosa);
        const float mag = factor*reIn[0];
        out[0] = mag*cosa;
        out[1] = mag*sina;
        sincos(-imIn[nxh], sina, cosa);
        const float magh = factor*reIn[nxh];
        realOutput[ny-j1][1] = magh*cosa;
        realOutput[ny-j1][0] = magh*sina;
      }
      
      // keep on with the rows, without the last lines
      for(; j1<ny; j1++){
        const fvector& reIn = realInput.getRow(j1);
        const fvector& imIn = imagInput.getRow(j1);
        fvector& out = realOutput[j1];
        for(j2=1; j2<nxh;j2++){
          const int j22 = j2 << 1; // x2
          sincos(imIn[j2], sina, cosa);
          const float mag = factor*reIn[j2];
          out[j22]   = mag*cosa;
          out[j22+1] = mag*sina;
        }
      }
      
      // very first row
      for(j2=1; j2<nxh;j2++){
        const int j22 = j2 << 1; // x2
        sincos(imagInput[0][j2], sina, cosa);
        const float mag = factor*realInput[0][j2];
        realOutput[0][j22]   = mag*cosa;
        realOutput[0][j22+1] = mag*sina;
      }

      realOutput[0][0]   = factor*realInput[0][0]*cos(imagInput[0][0]);
      realOutput[0][1]   = factor*realInput[0][nxh]*cos(imagInput[0][nxh]);
      realOutput[nyh][0] = factor*realInput[nyh][0]*cos(imagInput[nyh][0]);
      realOutput[nyh][1] = factor*realInput[nyh][nxh]*cos(imagInput[nyh][nxh]);

    } else { //Cartesic

      // First half or the rows, except the first one
      for(j1=1; j1<nyh; j1++){
        const fvector& reIn = realInput.getRow(j1);
        const fvector& imIn = imagInput.getRow(j1);
        fvector& out = realOutput[j1];

        for(j2=1; j2<nxh;j2++){
          const int j22 = (j2 << 1); // x2
          out[j22]   = factor*reIn[j2]; // =  R[ny-j1][nx-j2],
          out[j22+1] = factor*imIn[j2]; // = -I[ny-j1][nx-j2],
        }

        out[0] = factor*reIn[0];// = realInput[ny-j1][0],
        out[1] = factor*imIn[0];// = -imagInput[ny-j1][0],
        const int cj1 = ny-j1;
        realOutput[cj1][1] =  factor*reIn[nxh];//=realInput[ny-j1][nxh],
        realOutput[cj1][0] = -factor*imIn[nxh];//=imagInput[ny-j1][nxh],
      }

      // next half of rows, without the last lines
      for(; j1<ny; j1++){
        const fvector& reIn = realInput.getRow(j1);
        const fvector& imIn = imagInput.getRow(j1);
        fvector& out = realOutput[j1];

        for(j2=1; j2<nxh;j2++){
          const int j22 = (j2 << 1); // x2
          out[j22]   = factor*reIn[j2];// = R[ny-j1][nx-j2],
          out[j22+1] = factor*imIn[j2];// = -I[ny-j1][nx-j2],
        }
      }

      // very first row
      for(j2=1; j2<nxh;j2++){
        const int j22 = (j2 << 1); // x2
        realOutput[0][j22]   = factor*realInput[0][j2]; // =  R[0][nx-j2],
        realOutput[0][j22+1] = factor*imagInput[0][j2]; // = -I[0][nx-j2],
      }

      realOutput[0][0]   = factor*realInput[0][0];
      realOutput[0][1]   = factor*realInput[0][nxh];
      realOutput[nyh][0] = factor*realInput[nyh][0];
      realOutput[nyh][1] = factor*realInput[nyh][nxh];
    }

    ////////////////////////////////////////////////////////////////////
    // initialize fft

    t = internal::fft::alloc_1d<float>(2 * ny);
    n = max(ny, nx / 2);
    ip = internal::fft::alloc_1d<int>(2 + (int) sqrt(n + 0.5));
    n = max(ny * 5 / 4, nx * 5 / 4) + nx / 4;
    w = internal::fft::alloc_1d<float>(n);
    ip[0] = 0; // flag to indicate "initialization required!"

    // inverse FFT
    internal::fft::rdft2d(ny, nx, -1, realOutput, t, ip, w);

    // delete arrays
    internal::fft::free_1d<float>(w);
    internal::fft::free_1d<int>(ip);
    internal::fft::free_1d<float>(t);

    return true;
  } //apply for fmatrices
  

}

