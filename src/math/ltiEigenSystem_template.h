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
 * \file   ltiEigenSystem_template.h
 *         Contains the template class eigenSystem<T>.
 * \author Pablo Alvarado
 * \date   10.10.10
 *
 * revisions ..: $Id: ltiEigenSystem_template.h,v 1.10 2010-10-14 13:43:46 alvarado Exp $
 */
#include "ltiSort2.h"
#include "ltiRound.h"
#include <limits>

namespace lti {
  
  //////////////////////////////////////////////////
  // eigenSystem::parameters
  //////////////////////////////////////////////////

  //constructor
  template <typename T>
  eigenSystem<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters(), sort(false), dimensions(0),
      leftVector(false) {
  }

  // copy constructor
  template <typename T>
  eigenSystem<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  //copy
  template <typename T>
  typename eigenSystem<T>::parameters& 
  eigenSystem<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    dimensions=other.dimensions;
    sort = other.sort;
    leftVector = other.leftVector;
    return (*this);
  }

  //name
  template <typename T>
  const std::string& eigenSystem<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  //clone
  template <typename T>
  typename eigenSystem<T>::parameters* 
  eigenSystem<T>::parameters::clone() const {
    return new parameters(*this);
  }

  //newInstance
  template <typename T>
  typename eigenSystem<T>::parameters* 
  eigenSystem<T>::parameters::newInstance() const {
    return new parameters();
  }

  /**
   * write the parameters in the given ioHandler
   */
  template<typename T>
  bool eigenSystem<T>::parameters::write(ioHandler& handler,
                                                  const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    b = b && lti::write(handler, "dimensions", dimensions);
    b = b && lti::write(handler, "sort", sort);
    b = b && lti::write(handler, "leftVector", leftVector);
    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the parameters from the given ioHandler
   */
  template<typename T>
  bool eigenSystem<T>::parameters::read(ioHandler& handler, 
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    b = b && lti::read(handler, "dimensions", dimensions);
    b = b && lti::read(handler, "sort", sort);
    b = b && lti::read(handler, "leftVector", leftVector);
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  //////////////////////////////
  // eigenSystem
  //////////////////////////////

  // constructor
  template <typename T>
  eigenSystem<T>::eigenSystem() : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , lapackInterface()
#endif
  {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  template <typename T>
  eigenSystem<T>::eigenSystem(const parameters& par) 
    : linearAlgebraFunctor()
#ifdef HAVE_LAPACK
    , lapackInterface()
#endif
  {
    setParameters(par);
  }

  // returns the current parameters
  template<typename T>
  const typename eigenSystem<T>::parameters& 
  eigenSystem<T>::getParameters() const {
  
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    
    if(params==0)
      throw invalidParametersException(name());

    return *params;
  }

  /**
   * Method to split a complex vector into its two components
   */
  template<typename T>
  void eigenSystem<T>::split(const vector< complex <T> >& cplx,
                             vector<T>& re,
                             vector<T>& im) const {
    re.allocate(cplx.size());
    im.allocate(cplx.size());
    for (int i=0;i<cplx.size();++i) {
      cplx.at(i).get(re.at(i),im.at(i));
    }
  }
  
  /**
   * Method to merge real and imag components into a complex one
   */
  template<typename T>
  void eigenSystem<T>::merge(const vector<T>& re,
                             const vector<T>& im,
                             vector< complex <T> >& cplx) const {
    assert(re.size()==im.size());
    cplx.allocate(re.size());
    for (int i=0;i<cplx.size();++i) {
      cplx.at(i).set(re.at(i),im.at(i));
    }
  }
  
  /**
   * Method to split a complex vector into its two components
   */
  template<typename T>
  void eigenSystem<T>::split(const matrix< complex <T> >& cplx,
                             matrix<T>& re,
                             matrix<T>& im) const {
    re.allocate(cplx.size());
    im.allocate(cplx.size());
    for (int i=0;i<cplx.rows();++i) {
      split(cplx.getRow(i),re.getRow(i),im.getRow(i));
    }
    
  }
  
  /**
   * Method to merge real and imag components into a complex one
   */
  template<typename T>
  void eigenSystem<T>::merge(const matrix<T>& re,
                             const matrix<T>& im,
                             matrix< complex <T> >& cplx) const {
    assert(re.size()==im.size());
    cplx.allocate(re.size());
    for (int i=0;i<cplx.rows();++i) {
      merge(re.getRow(i),im.getRow(i),cplx.getRow(i));
    }
  }
  
  
  // onCopy version of apply
  template<typename T>
  bool eigenSystem<T>::apply(const matrix<T>& theMatrix,
                             vector<T>& rEigenvalues,
                             vector<T>& iEigenvalues,
                             matrix<T>& rEigenvectors,
                             matrix<T>& iEigenvectors) const {
    
    vector< complex<T> > eigenvalues;
    matrix< complex<T> > eigenvectors;

    if (reducedEigenSystem(theMatrix,eigenvalues,eigenvectors,
                           getParameters().dimensions)) {
      split(eigenvalues,rEigenvalues,iEigenvalues);
      split(eigenvectors,rEigenvectors,iEigenvectors);
      return true;
    }
    return false;
  }

  template<typename T>
  bool eigenSystem<T>::apply(const matrix<T>& theMatrix,
                             vector< complex<T> >& eigenvalues,
                             matrix< complex<T> >& eigenvectors) const {

    return reducedEigenSystem(theMatrix,eigenvalues,eigenvectors,
                              getParameters().dimensions);
  }

  template<typename T>
  bool eigenSystem<T>::apply(const matrix<T>& theMatrix,
                             vector<T>& eigenvalues,
                             matrix<T>& eigenvectors) const {
    vector<T> iEigenvalues; // temporal dummy storage
    matrix<T> iEigenvectors;// temporal dummy storage
    return apply(theMatrix,eigenvalues,iEigenvalues,eigenvectors,iEigenvectors);
  }

  template<typename T>
  bool eigenSystem<T>::reducedEigenSystem(const matrix<T>& theMatrix,
                                          vector< complex<T> >& eigenvalues,
                                          matrix< complex<T> >& eigenvectors,
                                          const int dimensions) const{ 
    const int matrixDim = theMatrix.rows();

    if (matrixDim != theMatrix.columns()) {
      this->setStatusString("Matrix must be square");
      eigenvalues.clear();
      eigenvectors.clear();
      return false;
    }
    
    vector<T> rEigenvalues,iEigenvalues;
    matrix<T> rEigenvectors;
    
    if (solve(theMatrix,rEigenvalues,iEigenvalues,rEigenvectors)) {

      // reformat data in a direct way
      eigenvalues.allocate(rEigenvalues.size());
      eigenvectors.allocate(rEigenvectors.size());
      
      static const T eps = std::numeric_limits<T>::epsilon();
      int i;
      for (i=0;i<eigenvalues.lastIdx();++i) {
        
        // check if the current pair is complex conjugate:
        if ((abs(rEigenvalues.at(i)-rEigenvalues.at(i+1))<eps) &&
            (abs(iEigenvalues.at(i)+iEigenvalues.at(i+1))<eps)) {
 
          // yes, it seems that we have a complex conjugate pair
          eigenvalues.at(i).set(rEigenvalues.at(i),iEigenvalues.at(i));
          eigenvalues.at(i+1).set(rEigenvalues.at(i),-iEigenvalues.at(i));

          // construct complex conjugate vectors
          vector< complex<T> >& evi = eigenvectors.getRow(i);
          vector< complex<T> >& evi1= eigenvectors.getRow(i+1);
          const vector< T >& rei = rEigenvectors.getRow(i);
          const vector< T >& rei1= rEigenvectors.getRow(i+1);

          for (int j=0;j<eigenvectors.columns();++j) {
            const T re = rei.at(j);
            const T im = rei1.at(j);
            evi.at(j).set(re,im);
            evi1.at(j).set(re,-im);
          }
          
          ++i;
        } else {
          // no complex conjugate, so assume real
          eigenvalues.at(i).set(rEigenvalues.at(i),T(0));
          eigenvectors.getRow(i).castFrom(rEigenvectors.getRow(i));
        }
      }
      
      if (i<eigenvalues.size()) {
        // assume real
        eigenvalues.at(i).set(rEigenvalues.at(i),T(0));
        eigenvectors.getRow(i).castFrom(rEigenvectors.getRow(i));
      }
      
      const parameters& param = getParameters();
      const bool sort = param.sort || (dimensions>0);
      
      if (sort) {

        // LAPACK not always sorts
        
        // check if it is really necessary to sort
        bool reallySort = false;

        // use as key the magnitude of the eigenvalues
        vector<T> key(eigenvalues.size());
        for (i=0;i<key.size();++i) {
          key.at(i)=abs(eigenvalues.at(i));
          if (i>0) {
            reallySort = reallySort || (key.at(i)>key.at(i-1));
          }
        }

        if (reallySort) {
          // descending order and columns sorter
          sort2 sorter(Descending, sort2::Rows);
          vector<T> ukey(key);
          sorter.apply(ukey,eigenvalues);
          sorter.apply(key,eigenvectors);
        }
      }

      if ((dimensions>0) && 
          (dimensions<eigenvalues.size())) {

        // reduce the dimensionality of the vector and matrices as requested
        eigenvalues.resize(dimensions,complex<T>(0,0));
        eigenvectors.resize(dimensions,eigenvectors.columns(),complex<T>(0,0));

      }

      eigenvectors.transpose(); // ensure eigenvectors in columns
      
      return true;
    }

    return false;
  }

  template<class T>
  bool eigenSystem<T>::solve(const matrix<T>& theMatrix,
                             vector<T>& eigenvalues,
                             vector<T>& iEigenvalues,
                             matrix<T>& eigenvectors) const {
    
#ifdef HAVE_LAPACK
    integer n=theMatrix.columns();
    integer m=theMatrix.rows();

    bool left = getParameters().leftVector;

    integer ldvr=left ? 1 : n;
    integer ldvl=left ? n : 1;
    integer info=0;

    matrix<T> dest;
    dest.transpose(theMatrix); // LAPACK will overwrite dest!

    eigenvectors.allocate(n,n);
    eigenvalues.allocate(n);
    iEigenvalues.allocate(n);

    if (dest.getMode() != matrix<T>::Connected) {
      this->setStatusString("Something went wrong! Matrix not connected!");
      return false;
    }

    char jobvl= left ? 'V' : 'N'; // left eigenvectors of A are computed.
    char jobvr= left ? 'N' : 'V'; // right eigenvectors of A are not computed;

    // first call is done just to know which size the working buffer should
    // have.
    T twork[1];
    integer lwork=-1;

    geev(&jobvl,&jobvr,
         &n,dest.data(),&m,
         eigenvalues.data(),iEigenvalues.data(),
         eigenvectors.data(),&ldvl,
         eigenvectors.data(),&ldvr,
         &twork[0],&lwork,&info);

    lwork = max(static_cast<integer>(twork[0]),integer(4*n));
    T* work = new T[lwork];

    // Now, solve the eigensystem
    geev(&jobvl,&jobvr,
         &n,dest.data(),&m,
         eigenvalues.data(),iEigenvalues.data(),
         eigenvectors.data(),&ldvl,
         eigenvectors.data(),&ldvr,
         &work[0],&lwork,&info);

    delete[] work;

    if (info<0) {
      setStatusString("LAPACK method received wrong argument ");
      appendStatusString(static_cast<int>(-info));
      return false;
    }
    if (info>0) {
      setStatusString("LAPACK failed to compute all eigenvalues.");
      return false;
    }
    return true;
#else
    setStatusString("This functor requires LAPACK");
    return false;
#endif
  }

  /*
   * Returns the name of this class.
   */
  template<typename T>
  const std::string& eigenSystem<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * Returns a pointer to a clone of this functor.
   */
  template<typename T>
  eigenSystem<T>* eigenSystem<T>::clone() const {
    return new eigenSystem<T>(*this);
  }
  
  /*
   * Returns a pointer to a new instance of this functor.
   */
  template<typename T>
  eigenSystem<T>* eigenSystem<T>::newInstance() const {
    return new eigenSystem<T>();
  }


}
