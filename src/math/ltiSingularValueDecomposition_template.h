/*
 * Copyright (C) 1998, 1999, 2000, 2001
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

/*
 * \file   ltiSingularValueDecomposition_template.h
 *         Compute the SVD of a matrix
 * \author Xin Gu
 * \date   11.01.2001
 *
 * $Id: ltiSingularValueDecomposition_template.h,v 1.23 2012-01-03 02:22:25 alvarado Exp $
 */

#ifdef _LTI_WIN32
  #undef min
  #undef max
#endif

#include <cstdio>
#include <limits>

#include "ltiMath.h"
#include "ltiSort2.h"
#include "ltiValidator.h"

#ifdef _LTI_DEBUG
#include <iostream>
using std::cout;
#endif

#include "ltiDebug.h"

namespace lti {

#ifdef _LTI_DEBUG
  // only tmp:
  template<typename T>
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

  template<typename T>
  void printVecX(vector<T> &v) {
    int i;
    for (i = 0; i < v.size(); i++) {
      cout << v[i] << "\t";
    }
    cout << endl;
  }
#endif

  // --------------------------------------------------
  // singularValueDecomposition
  // --------------------------------------------------

  /**
   * default constructor
   */
  template<typename T>
  singularValueDecomposition<T>::parameters::parameters() 
    : linearAlgebraFunctor::parameters() {
    sort = false;
    transposeU = false;
    transposeV = false;
    useDC = true;
    useMinDimensions = true;
  }
  
  /**
   * Copy constructor
   */
  template<typename T>
  singularValueDecomposition<T>::parameters::parameters(const parameters& o) 
    : linearAlgebraFunctor::parameters() {
    copy(o);
  }

  /**
   * Destructor
   */
  template<typename T>
  singularValueDecomposition<T>::parameters::~parameters() {
  }
  
  /**
   * Copy the contents of a parameters object
   */
  template<typename T>
  typename singularValueDecomposition<T>::parameters& 
  singularValueDecomposition<T>::parameters::copy(const parameters& other)  {
    linearAlgebraFunctor::parameters::copy(other);
    
    sort=other.sort;
    transposeU = other.transposeU;
    transposeV = other.transposeV;
    useDC = other.useDC;
    useMinDimensions = other.useMinDimensions;
    
    return *this;
  }

  /**
   * Assigns the contents of the other object to this object
   */
  template<typename T>
  typename singularValueDecomposition<T>::parameters& 
  singularValueDecomposition<T>::parameters::operator=(const parameters& other) {
    copy(other);
    return *this;
  }
  
  /**
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& singularValueDecomposition<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /**
   * Returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename singularValueDecomposition<T>::parameters* 
  singularValueDecomposition<T>::parameters::clone() const {
    return new parameters(*this);
  }
  
  /**
   * Returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename singularValueDecomposition<T>::parameters* 
  singularValueDecomposition<T>::parameters::newInstance() const {
    return new parameters();
  }
  
  /**
   * Read the parameters from the given ioHandler
   */
  template<typename T>
  bool singularValueDecomposition<T>::parameters::read(ioHandler &handler, const bool complete)
  {
    bool b=true;
    
    if (complete) {
      b=handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"sort",sort);
      lti::read(handler,"transposeU",transposeU);
      lti::read(handler,"transposeV",transposeV);
      lti::read(handler,"useDC",useDC);
      lti::read(handler,"useMinDimensions",useMinDimensions);
      
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    
    if (complete) {
      b=b && handler.readEnd();
    }
    
    return b;
  }
  
  /**
   * Write the parameters in the given ioHandler
   */
  template<typename T>
  bool singularValueDecomposition<T>::parameters::write(ioHandler& handler,const bool complete) const
  {
    bool b=true;
    
    if (complete) {
      b=handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"sort",sort);
      lti::write(handler,"transposeU",transposeU);
      lti::write(handler,"transposeV",transposeV);
      lti::write(handler,"useDC",useDC);
      lti::write(handler,"useMinDimensions",useMinDimensions);
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    
    if (complete) {
      b=b && handler.writeEnd();
    }
    
    return b;
  }
  
  template<typename T>
  singularValueDecomposition<T>::singularValueDecomposition() 
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
  {
    parameters defaultParam;
    setParameters(defaultParam);
  }

  template<typename T>
  singularValueDecomposition<T>
  ::singularValueDecomposition(const parameters& par)
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
  {
    setParameters(par);
  }

  template<typename T>
  singularValueDecomposition<T>::singularValueDecomposition(bool sort)
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
  {
    parameters defaultParam;
    defaultParam.sort=sort;
    setParameters(defaultParam);
  }

  /*
   * Copy constructor
   */
  template<typename T>
  singularValueDecomposition<T>::
  singularValueDecomposition(const singularValueDecomposition<T>& other) 
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
      , lapackInterface()
#endif
  {
    copy(other);
  }

  /*
   * Destructor
   */
  template<typename T>
  singularValueDecomposition<T>::~singularValueDecomposition() {
  }

  /*
   * Copy member
   */
  template <typename T>
  singularValueDecomposition<T>& singularValueDecomposition<T>::
  copy(const singularValueDecomposition<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  /*
   * class name
   */
  template<typename T>
  const std::string& singularValueDecomposition<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * clone member
   */
  template<typename T>
  singularValueDecomposition<T>* singularValueDecomposition<T>::clone() const {
    return new singularValueDecomposition<T>(*this);
  }

  /*
   * new instance
   */
  template<typename T>
  singularValueDecomposition<T>* singularValueDecomposition<T>::newInstance() const {
    return new singularValueDecomposition<T>();
  }

  /**
   * Sign
   */
  template<typename T>
  inline T singularValueDecomposition<T>::sign(const T a,const T b) const {
    return (b >= T(0)) ? abs(a) : -abs(a);
  }
  
  template<typename T>
  inline bool singularValueDecomposition<T>::isZero(const T x) const {
    return (abs(x) < std::numeric_limits<T>::epsilon());
  }

  template<typename T>
  inline bool singularValueDecomposition<T>::notZero(const T x) const {
    return !isZero(x);
  }

  /*
   * Function for the Implementation of the SVD-algorithm
   */
  template<typename T>
  bool singularValueDecomposition<T>::decomposition(matrix<T>& src,
                                                    vector<T>& w,
                                                    matrix<T>& v) const {

    const int rows=src.rows();
    const int cols=src.columns();

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

//    cout << " src1 " << endl;
//    printMatX(src);

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
    //cerr << "A\n";


    // Diagonalization of the bidiagonal form:
    // Loop over singular values,and over allowed iterations
    for ( k=cols-1; k >= 0; k--) {
      //cerr << "B";
      for (its=1; its <= maxIter; its++) {
        aFlag=true;
        //Test for splitting. Note that rv1[i] is always 0
        for (ppi=k; ppi >= 0; ppi--) {
          nm=ppi-1;
//           if ((abs(rv1.at(ppi))+anorm) == anorm) {
          if (closeToZero(rv1.at(ppi))) {
            aFlag=false;
            break;
          }
//           if ((abs(w.at(nm)+anorm) == anorm)) {
          if (closeToZero(w.at(nm))) {
            break;
          }
        }
        //cerr << "C";

        //Cancellation of rv1[ppi],if ppi>1.
        if (aFlag) {
          c=T(0);
          s=T(1);
          for ( i=ppi; i <= k ;i++) {
            f=s*rv1.at(i);
            rv1.at(i)=c*rv1.at(i);
//             if ((abs(f)+anorm) == anorm) {
            if (closeToZero(f)) {
              break;
            }
            g=w.at(i);
            h=pythag(f,g);


            w.at(i)=h;

//cout << "H = " << h << endl;
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
        //cerr << "D";

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
          char tmpmsg[80];
          sprintf(tmpmsg,"SVD did not converge after %d iterations!",maxIter);
          setStatusString(tmpmsg);
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

//        cout << f << endl;


        if (abs(f) >= std::numeric_limits<T>::max())
      {
         //cout << "overflowAAAAAAAAAAAXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx0"  << endl;
         f = std::numeric_limits<T>::max();
       }

       g = pythag(f,T(1));
       f = ((x-z)*(x+z)+h*((y/(f+sign(g,f)))-h))/x;

       // deviation from numerical receipes
       if (abs(f) >= std::numeric_limits<T>::max())
       {
         //cout << "overflow1DDDDXXXXXXXXXXXXXXXXXXX, w(ppi):" << w[ppi]  << endl;
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


/*
          // deviation from numerical receipes, maybe not necessary
          if (isZero(z)){
            cout << "underflowXXXXXXXXXXXXXXXXXXXXXXXXXx2"  << endl;
            c = T(0);
            s = T(0);
          }
          else if (tabs(z) >= std::numeric_limits<T>::max()){
            cout << "underflowXXXXXXXXXXXXXXXXXXXXXXXXXX21"  << endl;
            z =  std::numeric_limits<T>::max();
            T sgn = f > 0 ? 1 : -1;
            f =  sgn * std::numeric_limits<T>::max();           // check sign
            c=f/z;
            s=h/z;
          }
          else */
          {
            c=f/z;
            s=h/z;
          }

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
          //cerr << "G";

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

    if (getParameters().sort) {
      int sz=w.size();
      ivector idx(sz);
      for (i=0;i<sz;++i) {
        idx.at(i)=i;
      }

      sort2 sorter(Ascending);
      if (!sorter.apply(w,idx)) {
        setStatusString(sorter.getStatusString());
        return false;
      }

      int r=src.rows();
      matrix<T> tsrc(r,sz);
      matrix<T> tv(sz,sz);

      for (i=0;i<sz;++i) {
        const int ix=idx.at(i);
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

    if (getParameters().transposeU) {
      src.transpose();
    }

    if (getParameters().transposeV) {
      v.transpose();
    }

    return true;
  }

  template <typename T>
  bool singularValueDecomposition<T>::apply(matrix<T>& src, vector<T>& w,
                                            matrix<T>& v) const {

#ifdef HAVE_LAPACK

    if (getParameters().useLapack) {
      //this is ok, since the first argument is copied (by transform) 
      //right away.
      return applyLapack(src,src,w,v);
    }
    
#endif
    return decomposition(src,w,v);
  }

  template <typename T>
  bool singularValueDecomposition<T>::apply(const matrix<T>& src, matrix<T>& u,
                                            vector<T>& w, matrix<T>& v) const {

#ifdef HAVE_LAPACK   
    
    if (getParameters().useLapack) {
      return applyLapack(src,u,w,v);
    }
#endif

    u=src;
    return decomposition(u,w,v);
  }

#ifdef HAVE_LAPACK   
  template <typename T>
  bool singularValueDecomposition<T>::applyLapack(const matrix<T>& src,
                                                  matrix<T>& u,
                                                  vector<T>& w,
                                                  matrix<T>& v) const {

    // do things here
    const parameters& param = getParameters();
    
    lti::validator validate;
    
    if (!validate.apply(src)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("data matrix has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }
    
    matrix<T> a;
    a.transpose(src);
    
    char jobz,jobu,jobvt;
    integer m=src.rows();
    integer n=src.columns();
    integer minMN=min(m,n);
    integer ldu=m;
    integer ldvt=minMN;
    integer info=0;
    
    _lti_debug3("m=" << m << " n=" << n << " minMN=" << minMN << "\n");
    
    
    if (param.useMinDimensions) {
      _lti_debug2("Using min dimensions");
      jobz='S';
      jobu='S';
      jobvt='S';
      u.allocate(minMN,m);
      v.allocate(n,minMN);
    } else {
      _lti_debug2("Using all dimensions");
      jobz='A';
      jobu='A';
      jobvt='A';
      u.allocate(m,m);
      v.allocate(n,n);
      ldvt=n;
    }
    
    w.allocate(minMN);
    if (a.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }
    
    T twork[1];
    integer lwork=-1;
    
    if (param.useDC) {
      
      integer* iwork = new integer[8*minMN];
      
      gesdd(&jobz, &m, &n, a.data(), &m,
	    w.data(), u.data(), &ldu,
	    v.data(), &ldvt,
	    &twork[0], &lwork, &iwork[0], &info);
      
      lwork=int(twork[0]);
      T* work = new T[lwork];
      
      gesdd(&jobz, &m, &n, a.data(), &m,
	    w.data(), u.data(), &ldu,
	    v.data(), &ldvt,
	    &work[0], &lwork,  &iwork[0], &info);
      
      delete[] iwork;
      delete[] work;
      
    } else {
      
      gesvd(&jobu, &jobvt, &m, &n, a.data(), &m,
	    w.data(), u.data(), &ldu,
	    v.data(), &ldvt,
	    &twork[0], &lwork, &info);
      
      lwork=int(twork[0]);
      T* work = new T[lwork];
      
      gesvd(&jobu, &jobvt, &m, &n, a.data(), &m,
	    w.data(), u.data(), &ldu,
	    v.data(), &ldvt,
	    &work[0], &lwork, &info);
      
      delete[] work;
    }
    
    
    _lti_debug3("param.transposeU=" << param.transposeU
                << "  param.transposeV=" << param.transposeV <<"\n");
    
    //transpose
    if (!param.transposeU) {
      u.transpose();
    }
    
    if (param.transposeV) {
      v.transpose();
    }
    
    return (info==0);

  }
#endif

  /*
   * Help method: computes (a^2+b^2)^0.5 without destructive underflow
   * or overflow
   */
  template<typename T>
  T singularValueDecomposition<T>::pythag(const T a,const T b) const {

    T tmp1,tmp2;
    tmp1=abs(a);
    tmp2=abs(b);
    if (tmp1 > tmp2)
      return static_cast<T>(tmp1*sqrt(1.0+sqr(tmp2/tmp1)));
    else
      return ( closeToZero(tmp1) ? T(0) :
               static_cast<T>(tmp2*sqrt(1.0+sqr(tmp1/tmp2))));
  }

  /**
   * Compute the dot product of a part of two matrix rows
   */
  template<typename T>
  T singularValueDecomposition<T>::dotOfRows(const matrix<T>& data,
                                             const int row1, const int row2,
                                             int lowCol, const int highCol) const {
    

    T sum=T(0);
    const vector<T>& rtmp1=data.getRow(row1);
    const vector<T>& rtmp2=data.getRow(row2);
    const typename vector<T>::const_iterator ie1=rtmp1.end();
    const typename vector<T>::const_iterator ie2=rtmp2.end();
    
    typename vector<T>::const_iterator i1=rtmp1.begin();
    typename vector<T>::const_iterator i2=rtmp2.begin();
    
    i1+=lowCol;
    i2+=lowCol;
    
    while (lowCol++ <= highCol && i1 != ie1 && i2 != ie2) {
      sum+=*i1++**i2++;
    }
    return sum;
  }
    
  /**
   * Compute the dot product of a part of two matrix columns
   */
  template<typename T>
  T singularValueDecomposition<T>::dotOfColumns(const matrix<T>& data,
                                                const int col1, const int col2,
                                                int lowRow, int highRow) const {
    T sum=T(0);
    highRow=min(highRow,data.rows()-1);
    while (lowRow <= highRow) {
      sum+=data.at(lowRow,col1)*data.at(lowRow,col2);
      lowRow++;
    }
    return sum;
  }
  
  /**
   * Compute the sum of a part of a matrix row
   */
  template<typename T>
  T singularValueDecomposition<T>::sumOfRowPart(const matrix<T>& data,
                                                const int row,
                                                int lowCol, const int highCol) const {
    T sum=T(0);
    const vector<T>& rtmp=data.getRow(row);
    const typename vector<T>::const_iterator ie=rtmp.end();
    
    typename vector<T>::const_iterator i=rtmp.begin();
    i+=lowCol;
    while (lowCol++ <= highCol && i != ie) {
      sum+=*i++;
    }
    return sum;
  }
  
  /**
   * Compute the sum of a part of a matrix column
   */
  template<typename T>
  T singularValueDecomposition<T>::sumOfColumnPart(const matrix<T>& data,
                                                   const int col,
                                                   int lowRow, int highRow) const {
    T sum=T(0);
    highRow=min(highRow,data.rows()-1);
    while (lowRow <= highRow) {
      sum+=data.at(lowRow,col);
      lowRow++;
    }
    return sum;
  }
  
  /**
   * Compute the sum of the absolute value of a part of a matrix row
   */
  template<typename T>
  T singularValueDecomposition<T>::sumOfAbsRowPart(const matrix<T>& data,
                                                   const int row,
                                                   int lowCol, const int highCol) const {
    T sum=T(0);
    const vector<T>& rtmp=data.getRow(row);
    const typename vector<T>::const_iterator ie=rtmp.end();
    
    typename vector<T>::const_iterator i=rtmp.begin();
    i+=lowCol;
    while (lowCol <= highCol && i != ie) {
      sum+=abs(*i++);
      lowCol++;
    }
    return sum;
  }
  
  /**
   * Compute the sum of the absolute values of a part of a matrix column
   */
  template<typename T>
  T singularValueDecomposition<T>::sumOfAbsColumnPart(const matrix<T>& data,
                                                      const int col,
                                                      int lowRow, int highRow) const {
    T sum=T(0);
    highRow=min(highRow,data.rows()-1);
    while (lowRow <= highRow) {
      sum+=abs(data.at(lowRow,col));
      lowRow++;
    }
    return sum;
  }

  template<typename T>
  void singularValueDecomposition<T>::multiplyColumn(matrix<T>& data, const int col, 
                                                     const T factor,
                                                     int lowRow, int highRow) const {
    highRow=min(highRow,data.rows()-1);
    for (int i=lowRow; i<=highRow; i++) {
      data.at(lowRow++,col)*=factor;
    }
  }
  
  template<typename T>
  void singularValueDecomposition<T>::multiplyRow(matrix<T>& data, const int row, 
                                                  const T factor,
                                                  int lowCol, const int highCol) const {
    vector<T>& rtmp=data.getRow(row);
    typename vector<T>::iterator i=rtmp.begin();
    const typename vector<T>::iterator ie=rtmp.end();
    i+=lowCol;
    while (lowCol++ <= highCol && i != ie) {
      *i++*=factor;
    }
  }
  
  template<typename T>
  void singularValueDecomposition<T>::fillColumn(matrix<T>& data, const int col, const T value,
                                                 int lowRow, int highRow) const {
    highRow=min(highRow,data.rows()-1);
    for (int i=lowRow; i<=highRow; i++) {
      data.at(lowRow++,col)=value;
    }
  }
  
  template<typename T>
  void singularValueDecomposition<T>::fillRow(matrix<T>& data, const int row, const T value,
                                              int lowCol, const int highCol) const {
    vector<T>& rtmp=data.getRow(row);
      typename vector<T>::iterator i=rtmp.begin();
      const typename vector<T>::iterator ie=rtmp.end();
      i+=lowCol;
      while (lowCol++ <= highCol && i != ie) {
        *i++=value;
      }
  }
  
  
//layout the matrix
//   template<typename T>
//   void singularValueDecomposition<T>::sort(matrix<T>& u,
//                                     vector<T>& w,
//                                     matrix<T>& v) const{

//     const int m=w.size();
//     int tmp;
//     double max;
//     //sortieren
//     for (int i=0; i<m-1; i++){
//       max=w.at(i);
//       tmp=i;
//       for(int j=i+1; j<m; j++){
//         if ( max<w.at(j) ){
//           tmp=j;
//           max=w.at(j);
//         }
//       }
//       if ( tmp != i ){
//         w.at(tmp)=w.at(i);
//         w.at(i)=max;
//         swapColumns(u,v,i,tmp);
//       }
//     }
//   }

  template<typename T>
  const typename singularValueDecomposition<T>::parameters&
  singularValueDecomposition<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException();
    }
    return *par;
  }

  // swap columns(i,j)
//   template<typename T>
//   void singularValueDecomposition<T>::swapColumns(matrix<T>& u,
//                                            matrix<T>& v,
//                                            int& i,
//                                            int& j) const {
//     vector<T> h;
//     u.getColumnCopy(i,h);

//     u.setColumn(i,v.getColumnCopy(j));
//     v.setColumn(j,h);
//   }

}
