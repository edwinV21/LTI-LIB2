/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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

/*
 * \file   ltiSVD_template.h
 *         Singular Value Decomposition
 * \author Xin Gu
 * \author Pablo Alvarado
 * \date   11.01.2001
 *
 * revisions ..: $Id: ltiSVD_template.h,v 1.3 2012-01-03 02:22:25 alvarado Exp $
 */

#include <cstdio>
#include <limits>

#include "ltiMath.h"
#include "ltiSort2.h"
#include "ltiValidator.h"

#ifdef HAVE_LAPACK
#include "clapack.h"

#undef min
#undef max

#endif

#undef _LTI_DEBUG
// #define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include <iostream>
using std::cout;
#endif

namespace lti {

#ifdef _LTI_DEBUG
  // nur tmp:
  template<class T>
  void printMatX(matrix<T> &mat) {
    int rows = mat.rows();
    int cols = mat.columns();

    int row,col;

    for (row = 0; row < rows; row++) {
      for (col = 0; col < cols; col++) {
        cout << mat[row][col] << "\t";
      }
      cout << endl;
    }
  }

  template<class T>
  void printVecX(vector<T> &v) {
    int i;
    for (i = 0; i < v.size(); i++) {
      cout << v[i] << "\t";
    }
    cout << endl;
  }
#endif

  // --------------------------------------------------
  // svd::helper<T>
  // --------------------------------------------------
  template<typename T>
  svd::helper<T>::helper(const parameters& thePars) :
#ifdef HAVE_LAPACK
    lapackInterface(),
#endif
    params_(thePars) {
  }
  
  template<typename T>
  bool svd::helper<T>::decomposition(matrix<T>& src,
                                     vector<T>& w,
                                     matrix<T>& v) const {
#ifdef HAVE_LAPACK
    if (params_.useLapack) {
      return lapackApply(src,w,v);
    } else {
      return decompositionLocal(src,w,v);
    }
#else
    return decompositionLocal(src,w,v);
#endif
  }

  template<typename T>
  bool svd::helper<T>::decomposition(const matrix<T>& src,
                                     matrix<T>& u,
                                     vector<T>& w,
                                     matrix<T>& v) const {
#ifdef HAVE_LAPACK
    if (params_.useLapack) {
      return lapackApply(src,u,w,v);
    } else {
      u.copy(src);
      return decompositionLocal(u,w,v);
    }
#else
    u.copy(src);
    return decompositionLocal(u,w,v);
#endif
  }

  template<typename T>
  bool svd::helper<T>::decompositionLocal(matrix<T>& src,
                                          vector<T>& w,
                                          matrix<T>& v) const {

    //get the rows of the matrix
    const int rows=src.rows();
    //get the columns of the matrix
    const int cols=src.columns();

    //int max=rows > cols ? rows : cols;

    static const int maxIter=150;

    w.assign(cols,T(0));
    v.assign(cols,cols,T(0));

    int i(-1),its(-1),j(-1),jj(-1),k(-1),nm(0);
    int ppi(0);
    bool aFlag;
    T anorm(0),c(0),f(0),g(0),h(0),s(0),scale(0),x(0),y(0),z(0);

    // Householder reduction to bidiagonal form
    vector<T> rv1(cols,T(0));

    g=scale=anorm=T(0);
    for (i=0; i < cols; i++){
      ppi=i+1;
      rv1.at(i)=scale*g;
      g=s=scale=T(0);
      if (i < rows ) {
        // compute the sum of the i-th column, starting from the i-th row
        scale=sumOfAbsColumnPart(src,i,i);
        //cerr << "scale = " << scale << endl;
        if (notZero(scale)) {
          // multiply the i-th column by 1.0/scale, start from the i-th element
          multiplyColumn(src,i,static_cast<T>(1)/scale,i);
          // sum of squares of column i, start from the i-th element
          s=dotOfColumns(src,i,i,i);
          f=src.at(i,i);  // f is the diag elem
          g=-sign(sqrt(s),f);
          h=f*g-s;
          src.at(i,i)=f-g;
          for (j=ppi; j < cols; j++) {
            // dot product of columns i and j, starting from the i-th row
            s=dotOfColumns(src,i,j,i);
            f=s/h;
            // copy the scaled i-th column into the j-th column
            for (k=i;k<rows;k++) {
              src.at(k,j)+=f*src.at(k,i);
            }
          }
          multiplyColumn(src,i,scale,i);
        }
      }
      // save singular value
      w.at(i)=scale*g;
      g=s=scale=T(0);
      if ((i < rows) && (i != cols-1)) {
        // sum of row i, start from columns i+1
        scale=sumOfAbsRowPart(src,i,ppi);
        //cerr << "scale2 = " << scale << endl;
        if (notZero(scale)) {
          multiplyRow(src,i,static_cast<T>(1)/scale,ppi);
          s=dotOfRows(src,i,i,ppi);
          f=src.at(i,ppi);
          g=-sign(sqrt(s),f); //<---- do something
          h=f*g-s;
          src.at(i,ppi)=f-g;
          for ( k=ppi; k < cols; k++) {
            rv1.at(k)=src.at(i,k)/h;
          }
          for ( j=ppi; j < rows; j++) {
            s=dotOfRows(src,i,j,ppi);
            for ( k=ppi; k < cols; k++) {
              src.at(j,k)+=s*rv1.at(k);
            }
          }
          multiplyRow(src,i,scale,ppi);
        }
      }
      anorm=max(anorm,abs(w.at(i))+abs(rv1.at(i)));
    }

    // Accumulation of right-hand transformation
    for (i=cols-1; i>=0; i--) {
      //cerr << "\n";
      if ( i < cols-1 ) {
        //cerr << "g = " << g; // << endl;
        if (notZero(g)) {
          //cerr << "A";
          for ( j=ppi; j < cols; j++) {
            v.at(j,i)=(src.at(i,j)/src.at(i,ppi))/g;
          }
          //cerr << "B";
          for ( j=ppi; j < cols; j++) {
            for (s=T(0),k=ppi;k < cols;k++) {
              s+=src.at(i,k)*v.at(k,j);
            }
            for ( k=ppi;k < cols;k++) {
              v.at(k,j)+=s*v.at(k,i);
            }
          }
        }
        //cerr << "C";
        fillRow(v,i,0,ppi);
        fillColumn(v,i,0,ppi);
      }
      //cerr << "D";
      v.at(i,i)=T(1);
      g=rv1.at(i);
      ppi--;
    }
    //cerr << "E\n";

    // Accumulation of left-hand transformation
    for ( i=min(rows,cols)-1; i>=0; i--) {      //<---- do something
      ppi=i+1;
      g=w.at(i);
      //cerr << "F";
      fillRow(src,i,0,ppi);
      //cerr << "G";
      if (notZero(g)) {
        g=T(1)/g;
        //cerr << "H";
        for ( j=ppi; j < cols; j++) {
          s=dotOfColumns(src,i,j,ppi);
          f=(s/src.at(i,i))*g;
          for ( k=i;k < rows;k++) {
            src.at(k,j)+=f*src.at(k,i);
          }
        }
        //cerr << "I";
        multiplyColumn(src,i,g,i);
      } else {
        //cerr << "J";
        fillColumn(src,i,T(0),i);
      }
      ++src.at(i,i);
    }

    // Diagonalization of the bidiagonal form:
    // Loop over singular values,and over allowed iterations
    for ( k=cols-1; k >= 0; k--) {
      //cerr << "B";
      for (its=1; its <= maxIter; its++) {
        aFlag=true;
        //Test for splitting. Note that rv1[i] is always 0
        for (ppi=k; ppi >= 0; ppi--) {
          nm=ppi-1;
          if ((abs(rv1.at(ppi))+anorm) == anorm) {
            aFlag=false;
            break;
          }
          if ((abs(w.at(nm)+anorm) == anorm)) {
            break;
          }
        }

        //Cancellation of rv1[ppi],if ppi>1.
        if (aFlag) {
          c=T(0);
          s=T(1);
          for ( i=ppi; i <= k ;i++) {
            f=s*rv1.at(i);
            rv1.at(i)=c*rv1.at(i);
            if ((abs(f)+anorm) == anorm) {
              break;
            }
            g=w.at(i);
            h=pythag(f,g);


            w.at(i)=h;

            h=T(1)/h;
            c=g*h;
            s=-f*h;
            for ( j=0;j < rows; j++) {
              y=src.at(j,nm);
              z=src.at(j,i);
              src.at(j,nm)=y*c+z*s;
              src.at(j,i)=z*c-y*s;
            }
          }
        }
        z=w.at(k);

        // Convergence. Singular value is made nonnegative.
        if (ppi == k) {
          if (z < T(0)) {
            w.at(k)=-z;
            for (j=0; j < cols; j++) {
              v.at(j,k)=-v.at(j,k);
            }
          }
          break;
        }

        if (its == maxIter) {
          // make all singular values zero -> rank 0
          w.fill(T(0));
          return false;
        }
        //cerr << "E";
        // shift from bottom 2-by-2 minor
        x=w.at(ppi);
        nm=k-1;
        y=w.at(nm);
        g=rv1.at(nm);
        h=rv1.at(k);


        f=((y-z)*(y+z)+(g-h)*(g+h))/(T(2)*h*y);

        if (abs(f) >= std::numeric_limits<T>::max()) {
         f = std::numeric_limits<T>::max();
        }

        g = pythag(f,T(1));
        f = ((x-z)*(x+z)+h*((y/(f+sign(g,f)))-h))/x;

        // deviation from numerical receipes
        if (abs(f) >= std::numeric_limits<T>::max()) {
          f = std::numeric_limits<T>::epsilon();
        }

        //Next QR transformation
        c=s=T(1);
        for ( j=ppi; j<=nm ;j++){
          //cerr << "F";

          i=j+1;
          g=rv1.at(i);
          y=w.at(i);
          h=s*g;
          g=c*g;
          z=pythag(f,h);
          rv1.at(j)=z;


          c=f/z;
          s=h/z;

          f=x*c+g*s;
          g=g*c-x*s;
          h=y*s;
          y*=c;
          for (jj=0; jj<cols ;jj++){
            x=v.at(jj,j);
            z=v.at(jj,i);
            v.at(jj,j)=x*c+z*s;
            v.at(jj,i)=z*c-x*s;
          }
          z=pythag(f,h);
          // Rotation can be arbitrary if z=0
          w.at(j)=z;

          if (notZero(z)) {
            z=T(1)/z;
            c=f*z;
            s=h*z;
          }
          f=c*g+s*y;
          x=c*y-s*g;
          for (jj=0; jj<rows; jj++){
            y=src.at(jj,j);
            z=src.at(jj,i);
            src.at(jj,j)=y*c+z*s;
            src.at(jj,i)=z*c-y*s;
          }
        }
        rv1.at(ppi)=T(0);
        rv1.at(k)=f;
        w.at(k)=x;

      }
    }

    if (params_.sort) {
      const int sz=w.size();
      ivector idx(sz);
      for (i=0;i<sz;++i) {
        idx.at(i)=i;
      }

      sort2 sorter(Descending);

      if (!sorter.apply(w,idx)) {
        return false;
      }

      int r=src.rows();
      matrix<T> tsrc(r,sz);
      matrix<T> tv(sz,sz);

      for (i=0;i<sz;++i) {
        const int& ix=idx.at(i);
        for (j=0; j<r; ++j) {
          tsrc.at(j,i)=src.at(j,ix);
        }
        for (j=0; j<sz; ++j) {
          tv.at(j,i)=v.at(j,ix);
        }
      }

      tsrc.detach(src);
      tv.detach(v);

    }

    if (params_.transposeU) {
      src.transpose();
    }

    if (params_.transposeV) {
      v.transpose();
    }

    return true;
  }

#ifdef HAVE_LAPACK

  template<>
  int svd::helper<float>::svd(char* jobu, char* jobvt,
                              integer* m, integer* n, float* a, integer* lda,
                              float* s, float* u, integer* ldu, 
                              float* vt, integer* ldvt,
                              float* work, integer* lwork,
                              integer* info) const {
    this->lockInterface();
    int tmp=LA_SGESVD(jobu,jobvt,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,info);
    this->unlockInterface();
    return tmp;
  }

  template<>
  int svd::helper<double>::svd(char* jobu, char* jobvt,
                               integer* m, integer* n, double* a, integer* lda,
                               double* s, double* u, integer* ldu,
                               double* vt, integer* ldvt,
                               double* work, integer* lwork,
                               integer* info) const {
    this->lockInterface();
    int tmp=LA_DGESVD(jobu,jobvt,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,info);
    this->unlockInterface();
    return tmp;
  }

  template<>
  int svd::helper<float>::sdd(char* jobz, integer* m, integer* n, 
                              float* a, integer* lda,
                              float* s, float* u, integer* ldu, 
                              float* vt, integer* ldvt,
                              float* work, integer* lwork, integer* iwork,
                              integer* info) const {
    this->lockInterface();
    int tmp=LA_SGESDD(jobz,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,iwork,info);
    this->unlockInterface();
    return tmp;
  }

  template<>
  int svd::helper<double>::sdd(char* jobz, integer* m, integer* n, 
                               double* a, integer* lda,
                               double* s, double* u, integer* ldu,
                               double* vt, integer* ldvt,
                               double* work, integer* lwork, integer* iwork,
                               integer* info) const {
    this->lockInterface();
    int tmp=LA_DGESDD(jobz,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,iwork,info);
    this->unlockInterface();
    return tmp;
  }


  template <class T>
  bool svd::helper<T>::lapackApply(const matrix<T>& theMatrix,
                                   matrix<T>& leftSV,
                                   vector<T>& singularValues,
                                   matrix<T>& rightSVtrans) const
  {

    // do things here
    lti::validator validate;

    if (!validate.apply(theMatrix)) {
      return false;
    } else {
      _lti_debug("+\n");
    }

    matrix<T> a;

    char jobz,jobu,jobvt;
    integer m=theMatrix.rows();
    integer n=theMatrix.columns();
    integer minMN=min(m,n);
    integer ldu=m;
    integer ldvt=minMN;
    integer info=0;

    _lti_debug3("m=" << m << " n=" << n << " minMN=" << minMN << "\n");


    if (params_.useMinDimensions) {
      _lti_debug2("Using min dimensions");
      jobz='S';
      jobu='S';
      jobvt='S';
      leftSV.resize(minMN,m);
      rightSVtrans.resize(n,minMN);
    } else {
      _lti_debug2("Using all dimensions");
      jobz='A';
      jobu='A';
      jobvt='A';
      leftSV.resize(m,m);
      rightSVtrans.resize(n,n);
      ldvt=n;
    }

    a.transpose(theMatrix);
    singularValues.resize(minMN);
    if (a.getMode() != matrix<T>::Connected) {
      return false;
    }

    T twork[1];
    integer lwork=-1;

    if (params_.useDivideAndConquer) {

      integer* iwork = new integer[8*minMN];

      sdd(&jobz, &m, &n, &a.at(0,0), &m,
          singularValues.data(), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &iwork[0], &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      sdd(&jobz, &m, &n, &a.at(0,0), &m,
          singularValues.data(), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork,  &iwork[0], &info);

      delete[] iwork;
      delete[] work;

    } else {

      svd(&jobu, &jobvt, &m, &n, &a.at(0,0), &m,
          singularValues.data(), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      svd(&jobu, &jobvt, &m, &n, &a.at(0,0), &m,
          singularValues.data(), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork, &info);

      delete[] work;
    }


    _lti_debug3("param.transposeU=" << params_.transposeU
                << "  param.transposeV=" << params_.transposeV <<"\n");

    //transpose
    if (!params_.transposeU) {
      leftSV.transpose();
    }

    if (params_.transposeV) {
      rightSVtrans.transpose();
    }

    return (info==0);
  }

  template <class T>
  bool svd::helper<T>::lapackApply(matrix<T>& theMatrix,
                                   vector<T>& singularValues,
                                   matrix<T>& rightSVtrans) const {

    // do things here
    lti::validator validate;

    if (!validate.apply(theMatrix)) {
      return false;
    } else {
      _lti_debug("+\n");
    }


    char jobz,jobu,jobvt;
    integer m=theMatrix.rows();
    integer n=theMatrix.columns();
    integer minMN=min(m,n);
    integer ldu=m;
    integer ldvt=minMN;
    integer info=0;

    theMatrix.transpose();

    //int mode=0;
    jobu='O';

    if (m>=n) {
      //mode=1;
      jobz='O';
      jobvt='A';
      rightSVtrans.resize(n,n);
      ldu=0;
    } else if (params_.useMinDimensions) {
      //mode=2;
      jobz='S';
      jobvt='S';
      rightSVtrans.resize(n,minMN);
    } else {
      //mode=3;
      jobz='A';
      jobvt='A';
      rightSVtrans.resize(n,n);
      ldvt=n;
    }


    singularValues.resize(minMN);
    if (theMatrix.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }

    T twork[1];
    integer lwork=-1;

    if (params_.useDivideAndConquer) {

      T* uTemp = new T[ldu*m];
      if (ldu==0) ldu=1;
      integer* iwork = new integer[8*minMN];

      sdd(&jobz, &m, &n, &theMatrix.at(0,0), &m,
          singularValues.data(), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &iwork[0], &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      sdd(&jobz, &m, &n, &theMatrix.at(0,0), &m,
          singularValues.data(), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork,  &iwork[0], &info);

      if (jobz!='O') {
        theMatrix.fill(uTemp,0,0,m-1,m-1);
      }

      delete[] iwork;
      delete[] work;
      delete[] uTemp;

    } else {

      T* uTemp = new T[ldu*m];
      if (ldu==0) ldu=1;

      svd(&jobu, &jobvt, &m, &n, &theMatrix.at(0,0), &m,
          singularValues.data(), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      svd(&jobu, &jobvt, &m, &n, &theMatrix.at(0,0), &m,
          singularValues.data(), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork, &info);

      delete[] uTemp;
      delete[] work;
    }


    //transpose

    if (!params_.transposeU) {
      theMatrix.transpose();
    }

    if (params_.transposeV) {
      rightSVtrans.transpose();
    }

    return (info==0);

  }
  


#endif


}
