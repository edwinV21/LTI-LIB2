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
 * \file   ltiFFTinit.h
 *         FFT routines based on modified code by Takuya Ooura with kind
 *         permission ooura@kurims.kyoto-u.ac.jp or ooura@mmm.t.u-tokyo.ac.jp)
 * \author Stefan Syberichs
 * \author Takuyka Ooura
 * \date   06.12.99
 * revisions ..: $Id: ltiFFTinit.h,v 1.3 2012-09-16 04:58:33 alvarado Exp $
 */

#ifndef _LTI_FFTINIT_H
#define _LTI_FFTINIT_H

namespace lti{

  namespace internal {

    /**
     * Namespace with all FFT and inverse FFT routines based on code by
     * Takuya Ooura with kind permission ooura@kurims.kyoto-u.ac.jp or
     * ooura@mmm.t.u-tokyo.ac.jp).  The code has been adapted to use C++
     * and LTI-lib conventions.  This functions will be used by the
     * functors lti::realFFT and lti::realInvFFT
     */
    namespace fft {
      // prototypes for c-functions

      /**
       * Reserve memory for a n-dimensional vector of elements with type T
       * @param n1 dimension of the vector
       * @return pointer to the elements of the vector
       */
      template<class T>
      T* alloc_1d(const int& n1);

      /**
       * Free memory of 1d array of elements with type T
       */
      template<class T>
      void free_1d(T* i);

      /**
       * Reserve memory for a n1xn2-matrix of elementes of type T
       * @param n1 number of rows
       * @param n2 number of columns
       * @return pointer to the elements of the vector
       */
      template<class T>
      T** alloc_2d(const int& n1, const int& n2);

      /**
       * Free memory of 2d array of elements with type T
       */
      template<class T>
      void free_2d(T** ii);

      /**
       * Real discrete Fourier transform for lti::vectors
       * @param n data length (must be power of 2)
       * @param isgn transform direction: +1 for transform and -1 for inverse.
       * @param a the input/output data.  In the frecuency domain the real
       *          parts will be stored in the even locations and the imaginary
       *          parts in the odd locations.  a[1] = real[n/2]
       * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
       *           cos/sin table
       * @param w cos/sin table.
       *3
       * w[] and ip[] will be initialized if ip[0] == 0
       */
      template<class T>
      void vrdft(int n, int isgn, vector<T>& a, int *ip, T *w);

      /**
       * Real discrete Fourier transform for 2D
       * @param ny number of rows (must be power of 2)
       * @param nx number of columns (must be power of 2)
       * @param isgn transform direction: +1 for transform and -1 for inverse.
       * @param a the input/output data.  In the frecuency domain the real
       *          parts will be stored in the even locations and the imaginary
       *          parts in the odd locations.  
       *          \code
       *          Output data if isgn=+1
       *            a[k1][2*k2] = R[k1][k2] = R[n1-k1][n2-k2], 
       *            a[k1][2*k2+1] = I[k1][k2] = -I[n1-k1][n2-k2], 
       *                0<k1<n1, 0<k2<n2/2, 
       *            a[0][2*k2] = R[0][k2] = R[0][n2-k2], 
       *            a[0][2*k2+1] = I[0][k2] = -I[0][n2-k2], 
       *                0<k2<n2/2, 
       *            a[k1][0] = R[k1][0] = R[n1-k1][0], 
       *            a[k1][1] = I[k1][0] = -I[n1-k1][0], 
       *            a[n1-k1][1] = R[k1][n2/2] = R[n1-k1][n2/2], 
       *            a[n1-k1][0] = -I[k1][n2/2] = I[n1-k1][n2/2], 
       *                0<k1<n1/2, 
       *            a[0][0] = R[0][0], 
       *            a[0][1] = R[0][n2/2], 
       *            a[n1/2][0] = R[n1/2][0], 
       *            a[n1/2][1] = R[n1/2][n2/2]
       *          Input data if isgn=-1
       *            a[j1][2*j2] = R[j1][j2] = R[n1-j1][n2-j2], 
       *            a[j1][2*j2+1] = I[j1][j2] = -I[n1-j1][n2-j2], 
       *               0<j1<n1, 0<j2<n2/2, 
       *            a[0][2*j2] = R[0][j2] = R[0][n2-j2], 
       *            a[0][2*j2+1] = I[0][j2] = -I[0][n2-j2], 
       *               0<j2<n2/2, 
       *            a[j1][0] = R[j1][0] = R[n1-j1][0], 
       *            a[j1][1] = I[j1][0] = -I[n1-j1][0], 
       *            a[n1-j1][1] = R[j1][n2/2] = R[n1-j1][n2/2], 
       *            a[n1-j1][0] = -I[j1][n2/2] = I[n1-j1][n2/2], 
       *               0<j1<n1/2, 
       *            a[0][0] = R[0][0], 
       *            a[0][1] = R[0][n2/2], 
       *            a[n1/2][0] = R[n1/2][0], 
       *            a[n1/2][1] = R[n1/2][n2/2]
       *          \endcode
       * @param t work area
       * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
       *           cos/sin table
       * @param w cos/sin table. Length of w >= max(n1/2, n2/4) + n2/4.
       *
       * w[] and ip[] will be initialized if ip[0] == 0
       */
      template<class T>
      void rdft2d(int ny, int nx, int isgn, matrix<T>& a, T *t, int *ip, T *w);

      template<class T>
      void makewt(int nw, int *ip, T *w);

      template<class T>
      void makect(int nc, int *ip, T *c);

      template<class T>
      void bitrv2(int n, int *ip, T *a);

      template<class T>
      void cftfsub(int n, T *a, T *w);

      template<class T>
      void cftbsub(int n, T *a, T *w);

      template<class T>
      void rftfsub(int n, T *a, int nc, T *c);

      template<class T>
      void rftbsub(int n, T *a, int nc, T *c);

      /**
       * Real discrete Fourier transform
       * @param n data length (must be power of 2)
       * @param isgn transform direction: +1 for transform and -1 for inverse.
       * @param a the input/output data.  In the frecuency domain the real
       *          parts will be stored in the even locations and the imaginary
       *          parts in the odd locations.  a[1] = real[n/2]
       * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
       *           cos/sin table
       * @param w cos/sin table.
       *
       * w[] and ip[] will be initialized if ip[0] == 0
       */
      template<class T>
      void rdft(int n, int isgn, T *a, int *ip, T *w);

      /**
       * Complex direct Fourier transform
       * @param n data length (must be power of 2)
       * @param isgn transform direction: +1 for transform and -1 for inverse.
       * @param a the input/output data.  In the frecuency domain the real
       *          parts will be stored in the even locations and the imaginary
       *          parts in the odd locations.  a[1] = real[n/2]
       * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
       *           cos/sin table
       * @param w cos/sin table.
       *
       * w[] and ip[] will be initialized if ip[0] == 0
       */
      template<class T>
      void cdft(int n, int isgn, T *a, int *ip, T *w);

      template<class T>
      void cft1st(int n, T *a, T *w);

      template<class T>
      void cftmdl(int n, int l, T *a, T *w);

      template<class T>
      void bitrv2conj(int n, int *ip, T *a);
    }
  }
}

#endif
