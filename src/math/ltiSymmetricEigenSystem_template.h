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
 * \file   ltiSymmetricEigenSystem_template.h
 *         Contains the template class symmetricEigenSystem<T>.
 * \author Thomas Rusert
 * \date   16.06.99
 *
 * revisions ..: $Id: ltiSymmetricEigenSystem_template.h,v 1.10 2009-07-28 16:53:00 alvarado Exp $
 */
#include "ltiSort2.h"
#include <cstdio>

#ifdef HAVE_LAPACK
#  include "ltiValidator.h"
#  include "ltiRound.h"
#endif

namespace lti {
  
  //////////////////////////////////////////////////
  // symmetricEigenSystem::parameters
  //////////////////////////////////////////////////

  //constructor
  template <typename T>
  symmetricEigenSystem<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters(), sort(false), dimensions(0) {
  }

  // copy constructor
  template <typename T>
  symmetricEigenSystem<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  //copy
  template <typename T>
  typename symmetricEigenSystem<T>::parameters& 
  symmetricEigenSystem<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    dimensions=other.dimensions;
    sort = other.sort;
    return (*this);
  }

  //name
  template <typename T>
  const std::string& symmetricEigenSystem<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  //clone
  template <typename T>
  typename symmetricEigenSystem<T>::parameters* 
  symmetricEigenSystem<T>::parameters::clone() const {
    return new parameters(*this);
  }

  //newInstance
  template <typename T>
  typename symmetricEigenSystem<T>::parameters* 
  symmetricEigenSystem<T>::parameters::newInstance() const {
    return new parameters();
  }

  /**
   * write the parameters in the given ioHandler
   */
  template<typename T>
  bool symmetricEigenSystem<T>::parameters::write(ioHandler& handler,
                                                  const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    b = b && lti::write(handler, "dimensions", dimensions);
    b = b && lti::write(handler, "sort", sort);
    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the parameters from the given ioHandler
   */
  template<typename T>
  bool symmetricEigenSystem<T>::parameters::read(ioHandler& handler, const bool complete){
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    b = b && lti::read(handler, "dimensions", dimensions);
    b = b && lti::read(handler, "sort", sort);
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  //////////////////////////////
  // symmetricEigenSystem
  //////////////////////////////

  // constructor
  template <typename T>
  symmetricEigenSystem<T>::symmetricEigenSystem() : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
                                                    , lapackInterface()
#endif
  {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  template <typename T>
  symmetricEigenSystem<T>::symmetricEigenSystem(const parameters& par) 
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , lapackInterface()
#endif
  {
    setParameters(par);
  }

  // returns the current parameters
  template<typename T>
  const typename symmetricEigenSystem<T>::parameters& 
  symmetricEigenSystem<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    
    if(params==0)
      throw invalidParametersException(name());

    return *params;
  }


  template<typename T>
  inline void symmetricEigenSystem<T>::rotate(double& g,double& h,
                                              matrix<double>& a,
                                              const int i,const int j,
                                              const int k,const int l,
                                              const double s,
                                              const double tau) const {
    g=a[i][j];
    h=a[k][l];
    a[i][j]=g-s*(h+g*tau);
    a[k][l]=h+s*(g-h*tau);
  }

  template<typename T>
  inline void symmetricEigenSystem<T>::rotateT(double& g,double& h,
                                               matrix<T>& a,
                                               const int i,const int j,
                                               const int k,const int l,
                                               const double s,
                                               const double tau) const {
    g=a[i][j];
    h=a[k][l];
    a[i][j]=static_cast<T>(g-s*(h+g*tau));
    a[k][l]=static_cast<T>(h+s*(g-h*tau));
  }


  // onCopy version of apply
  template<typename T>
  bool symmetricEigenSystem<T>::apply(const matrix<T>& theMatrix,
                                      vector<T>& eigenvalues,
                                      matrix<T>& eigenvectors) const {
    return reducedEigenSystem(theMatrix,eigenvalues,eigenvectors,
                              getParameters().dimensions);
  }


  template<typename T>
  bool symmetricEigenSystem<T>::reducedEigenSystem(const matrix<T>& theMatrix,
                                                   vector<T>& eigenvalues,
                                                   matrix<T>& eigenvectors,
                                                   const int dimensions) const{ 
    const int matrixDim = theMatrix.rows();

    if (matrixDim != theMatrix.columns()) {
      this->setStatusString("Matrix must be square and symmetric");
      eigenvalues.clear();
      eigenvectors.clear();
      return false;
    }

#ifdef HAVE_LAPACK
    if (getParameters().useLapack) {
      return applyLapack(theMatrix,eigenvalues,eigenvectors,dimensions);
    }
#endif    


    static const int maxIter=100;
    
    const parameters& tmpParam = getParameters();

    const bool sort = tmpParam.sort || (dimensions>0);

    int j,iq,ip,i;
    double tresh, theta, tau, sm, g, h;
    T c,s,t;

    matrix<double> cloneMatrix; 
    cloneMatrix.castFrom(theMatrix);
    vector<double> b(matrixDim);
    vector<double> z(matrixDim, 0.0);

    eigenvalues.allocate(matrixDim);
    eigenvectors.assign(matrixDim,matrixDim,T(0));

    for (ip=0;ip<matrixDim;ip++) {
      eigenvectors[ip][ip]=static_cast<T>(1);
      eigenvalues[ip]=static_cast<T>(b[ip]=cloneMatrix[ip][ip]);
    }

    for (i=0;i<maxIter;i++) {
      sm=0.0;
      for (ip=0;ip<matrixDim;ip++) {
        for(iq=ip+1;iq<matrixDim;iq++) {
          sm += fabs(cloneMatrix[ip][iq]);
        }
      }

      if (sm == 0.0) {   // normal return!
        if(sort) {
          // descending order and columns sorter
          sort2 sorter(Descending, sort2::Columns);
          sorter.apply(eigenvalues,eigenvectors);
        }
        
        if ((dimensions>0) && 
            (dimensions<eigenvalues.size())) {
          // reduce the dimensionality of the vector and matrices as requested
          eigenvalues.resize(dimensions,T(),Copy);
          eigenvectors.resize(eigenvectors.rows(),dimensions,T(),
                              Copy);
        }

        return true;
      }

      if (i < 3) {
        tresh= 0.2*sm/(matrixDim*matrixDim);
      } else {
        tresh=0.0;
      }

      for (ip=0;ip<matrixDim-1;ip++) {
        for (iq=ip+1;iq<matrixDim;iq++) {
          g = 100.0*fabs(cloneMatrix[ip][iq]);
          if ((i > 3) && ((fabs(eigenvalues[ip])+g) == fabs(eigenvalues[ip]))
              && ((fabs(eigenvalues[iq])+g) == fabs(eigenvalues[iq]))) {
            cloneMatrix[ip][iq]=0.0;
          }
          else if (fabs(cloneMatrix[ip][iq]) > tresh) {
            h=eigenvalues[iq]-eigenvalues[ip];
            if ((fabs(h)+g) == fabs(h)) {
              t=static_cast<T>((cloneMatrix[ip][iq])/h);
            } else {
              theta = 0.5*h/cloneMatrix[ip][iq];
              t = static_cast<T>(1.0/(fabs(theta)+sqrt(1.0+theta*theta)));
              if (theta < 0.0) {
                t = -t;
              }
            }
            c = static_cast<T>(1)/sqrt(1+t*t);
            s = t*c;
            tau = s/(c+1);
            h=t*cloneMatrix[ip][iq];
            z[ip] -= h;
            z[iq] += h;
            eigenvalues[ip] -= static_cast<T>(h);
            eigenvalues[iq] += static_cast<T>(h);
            cloneMatrix[ip][iq]=0.0;
            for (j=0;j<ip;j++)  {
              rotate(g,h,cloneMatrix,j,ip,j,iq,s,tau);
            }
            for (j=ip+1;j<iq;j++) {
              rotate(g,h,cloneMatrix,ip,j,j,iq,s,tau);
            }
            for (j=iq+1;j<matrixDim;j++) {
              rotate(g,h,cloneMatrix,ip,j,iq,j,s,tau);
            }
            for (j=0;j<matrixDim;j++) {
              rotateT(g,h,eigenvectors,j,ip,j,iq,s,tau);
            }
          }
        }
      }
      for (ip=0;ip<matrixDim;ip++) {
        b[ip] += z[ip];
        eigenvalues[ip] = static_cast<T>(b[ip]);
        z[ip] = 0.0;
      }
    }

    this->setStatusString("Jacobi Method did not converge after ");
    this->appendStatusString(maxIter);
    this->appendStatusString(" iterations\n");

    return false;
  }

  /*
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& symmetricEigenSystem<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * Returns a pointer to a clone of this functor.
   */
  template<typename T>
  symmetricEigenSystem<T>* symmetricEigenSystem<T>::clone() const {
    return new symmetricEigenSystem<T>(*this);
  }
  
  /*
   * Returns a pointer to a new instance of this functor.
   */
  template<typename T>
  symmetricEigenSystem<T>* symmetricEigenSystem<T>::newInstance() const {
    return new symmetricEigenSystem<T>();
  }

#ifdef HAVE_LAPACK
  template <typename T>
  bool symmetricEigenSystem<T>::applyLapack(const matrix<T>& theMatrix,
                                            vector<T>& eigenvalues,
                                            matrix<T>& eigenvectors,
                                            const int dimensions) const {

    if (theMatrix.empty()) {
      eigenvalues.clear();
      eigenvectors.clear();
      return true; // nothing to do, matrix is empty!
    }

    bool success;

    lti::validator validate;

    assert(validate.apply(theMatrix));

    // RANGE  (input) CHARACTER*1
    //        = 'A':  all eigenvalues will be found.
    //        = 'V':  all eigenvalues in the half-open interval
    //                (VL,VU] will be found.
    //        = 'I':  the  IL-th  through IU-th eigenvalues
    //                will be found.
    char range='A'; 

    // JOBZ   (input) CHARACTER*1
    //        = 'N':  Compute eigenvalues only;
    //        = 'V':  Compute eigenvalues and eigenvectors.
    char jobz='V';  

    // UPLO   (input) CHARACTER*1
    //        = 'U':  Upper triangle of A is stored;
    //        = 'L':  Lower triangle of A is stored.
    char uplo='U';  
    
    // N      (input) INTEGER
    //        The order of the matrix A.  N >= 0.
    integer n = theMatrix.columns(); 

    // A  (input/output)  DOUBLE PRECISION array, dimension (LDA, N)
    //    On  entry, the symmetric matrix A.  If UPLO = 'U',
    //    the leading N-by-N upper triangular part of A con­
    //    tains  the  upper triangular part of the matrix A.
    //    If UPLO = 'L', the leading N-by-N lower triangular
    //    part  of  A  contains the lower triangular part of
    //    the matrix A.  On exit,  the  lower  triangle  (if
    //    UPLO='L')  or  the upper triangle (if UPLO='U') of
    //    A, including the diagonal, is destroyed.
    
    // we have to copy the data, because it will be modified!
    matrix<T> a(theMatrix);
    T* apointer=&a.at(0,0);

    // LDA    (input) INTEGER
    //        The leading dimension of  the  array  A.   LDA  >= max(1,N).
    integer lda = n;

    // VL     (input) DOUBLE PRECISION
    // VU     (input) DOUBLE PRECISION
    //        If RANGE='V', the lower and upper bounds of the
    //        interval to be searched for eigenvalues.
    //        VL < VU. 
    //        Not referenced if RANGE = 'A' or 'I'.    
    T vl=T(0);
    T vu=T(0);

    // IL     (input) INTEGER
    // IU     (input) INTEGER If RANGE='I', the  indices
    //        (in  ascending  order) of the smallest and 
    //        largest eigenvalues to be returned.
    //        1 <= IL <= IU  <=  N,if  N > 0;
    //        IL = 1 and IU = 0 if N = 0.
    //        Not referenced if RANGE = 'A' or 'V'.
    integer il=1;
    integer iu=n;      

    // ABSTOL (input) DOUBLE PRECISION
    //        The absolute error tolerance for the
    //        eigenvalues.  
    //        An approximate eigenvalue is accepted as
    //        converged when it is determined to lie in an
    //        interval [a,b] of width less than or equal to
    //
    //               ABSTOL + EPS *   max( |a|,|b| ) ,
    // 
    //        where EPS is the machine precision.  If ABSTOL
    //        is less than or equal to zero, then EPS*|T|
    //        will be used in its place, where |T| is the
    //        1-norm of the tridiagonal matrix obtained by
    //        reducing A to tridiagonal form.
    char cmach='S'; // Safe Minimum
    T abstol;
    lamch(&cmach,abstol);

    // M      (output) INTEGER
    //        The  total number of eigenvalues found. 
    //        0 <= M <= N.
    //        If RANGE = 'A', M = N     , and 
    //        if RANGE = 'I', M = IU-IL+1.
    integer m=0;

    // LDZ    (input) INTEGER
    //        The  leading  dimension of the array Z.
    //        LDZ >= 1, and if JOBZ = 'V', LDZ >= max(1,N).
    integer ldz=n;

    // ISUPPZ  (output) INTEGER array, dimension ( 2*max(1,M) )
    //         The support of the eigenvectors in  Z,  i.e.,  the
    //         indices  indicating the nonzero elements in Z. The
    //         i-th  eigenvector  is  nonzero  only  in  elements
    //         ISUPPZ( 2*i-1 ) through ISUPPZ( 2*i ).
    vector<integer> isuppz(2*n); 

    // INFO   (output) INTEGER
    //        = 0:  successful exit
    //        < 0:  if INFO = -i, the i-th argument had an illegal value
    //        > 0:  Internal error
    integer info=0;

    if (dimensions>0 && dimensions<n) {
      il=n-dimensions+1;
      range='I';
      isuppz.allocate(2*dimensions);
    }

    // Z      (output) DOUBLE PRECISION array,  dimension  (LDZ,max(1,M))
    //        If JOBZ = 'V', then if INFO = 0, the first M columns of Z
    //        contain the orthonormal eigenvectors of the matrix A
    //        corresponding to the selected eigenvalues, with the i-th column
    //        of Z holding the eigenvector associated with W(i).  If JOBZ =
    //        'N', then Z is not referenced.  Note: the user must ensure that
    //        at least max(1,M) columns are supplied in the array Z; if RANGE
    //        = 'V', the exact value of M is not known in advance and an upper
    //        bound must be used.
    eigenvectors.allocate(iu-il+1,ldz);

    // W       (output) DOUBLE PRECISION array, dimension (N)
    //         The  first  M elements contain the selected eigen­
    //         values in ascending order.
    vector<T> w;
    w.allocate(n);

    // LWORK   (input) INTEGER
    //         The  dimension  of  the  array  WORK.   LWORK >= max(1,26*N).
    //         For  optimal  efficiency,  LWORK >= (NB+6)*N, where NB is the
    //         max of the blocksize for DSYTRD and DORMTR returned by ILAENV.
    //
    //         If  LWORK = -1, then a workspace query is assumed;
    //         the routine only calculates the  optimal  size  of
    //         the  WORK  array,  returns this value as the first
    //         entry of the WORK  array,  and  no  error  message
    //         related to LWORK is issued by XERBLA.
    integer lwork=-1;

    //  WORK   (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
    //         On  exit, if INFO = 0, WORK(1) returns the optimal LWORK.
    T twork[1];


    // IWORK   (workspace/output)   INTEGER   array,  dimension (LIWORK)
    //         On exit, if INFO = 0, IWORK(1) returns the optimal LWORK.
    integer tiwork[1];

    // LIWORK  (input) INTEGER
    //         The dimension of the array IWORK.   LIWORK >= max(1,10*N).
    //
    //         If LIWORK = -1, then a workspace query is assumed;
    //         the routine only calculates the  optimal  size  of
    //         the  IWORK  array, returns this value as the first
    //         entry of the IWORK array,  and  no  error  message
    //         related to LIWORK is issued by XERBLA.
    integer liwork=-1;

    // this first call is supposed to only compute the sizes for optimal
    // workspaces. 
    // The results are the tiwork and twork.
    evr(&jobz, &range, &uplo,
//         &n, &a.at(0,0), &lda,
        &n, apointer, &lda,
        &vl, &vu, &il, &iu, &abstol,
        &m, w.data(), &eigenvectors.at(0,0), &ldz, isuppz.data(),
        &twork[0],&lwork,&tiwork[0],&liwork,&info);

    // reserve the proper workspace arrays
    lwork =max(integer(iround(twork[0])),26*n);
    T* work = new T[lwork];

    liwork=max(tiwork[0],10*n);
    integer* iwork = new integer[liwork];

    // now the proper eigenvalues can be computed with the correct help 
    // workspaces
    evr(&jobz, &range, &uplo,
//         &n, &a.at(0,0), &n,
        &n, apointer, &n,
        &vl, &vu, &il, &iu, &abstol,
        &m, w.data(), &eigenvectors.at(0,0), &ldz, isuppz.data(),
        &work[0],&lwork,&iwork[0],&liwork,&info);

    //transpose and sort in descending order
    int tmpRows,tmpCols,i,j;
    tmpRows = eigenvectors.rows();
    tmpCols = eigenvectors.columns();

    matrix<T> temp;

    if (eigenvectors.getMode() == matrix<T>::Connected) {
      eigenvectors.detach(temp); // transport data to 'temp'
    }
    else {
      temp.copy(eigenvectors);
    }

    eigenvectors.allocate(tmpCols,tmpRows);
    for(i=0;i<tmpRows;++i) {
      for(j=0;j<tmpCols;++j) {
        eigenvectors.at(j,i)=temp.at(tmpRows-1-i,j);
      }
    }

    eigenvalues.allocate(tmpRows);

    //swap eigenvalues
    for(i=0; i<tmpRows; i++) {
      eigenvalues.at(i) = w.at(tmpRows-1-i);
    }

    success=(info==0);

    delete[] work;
    delete[] iwork;

    if (!success) {
      // INFO   (output) INTEGER
      //        = 0:  successful exit
      //        < 0:  if INFO = -i, the i-th argument had an illegal value
      //        > 0:  Internal error
      if (info<0) {
        this->setStatusString("Fast eigensystem got an LAPACK error: ");
        this->appendStatusString(static_cast<int>(-info));
        this->appendStatusString("-th argument seems to be wrong");
      } else {
        this->setStatusString("Fast eigensystem got an internal LAPACK error");
      }
    }

    return success;
  }
  
#endif //HAVE_LAPACK


}
