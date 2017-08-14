/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiCholeskyDecomposition_template.h
 *         Implements the class lti::choleskyDecomposition which decomposes a 
 *         matrix in a upper/lower triangular matrix.
 * \author Peter Doerfler
 * \date   07.07.2003
 * 
 * $Id: ltiCholeskyDecomposition_template.h,v 1.7 2007-12-26 04:49:23 alvarado Exp $
 */


#include "ltiCholeskyDecomposition.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // choleskyDecomposition::parameters
  // --------------------------------------------------

  //default constructor
  template<typename T>
  choleskyDecomposition<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters() {

    useLapack = false; //not available
    triangularMatrixType = Upper;
  }

  //copy constructor
  template<typename T>
  choleskyDecomposition<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  //destructor
  template<typename T>
  choleskyDecomposition<T>::parameters::~parameters() {
  }

  //copy the parameters
  template<typename T>
  typename choleskyDecomposition<T>::parameters&
  choleskyDecomposition<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    triangularMatrixType=other.triangularMatrixType;
    return *this;
  }

  //copy operator
  template<typename T>
  typename choleskyDecomposition<T>::parameters&
  choleskyDecomposition<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // return type name
  template<typename T>
  const std::string& choleskyDecomposition<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone parameters
  template<typename T>
  typename choleskyDecomposition<T>::parameters*
  choleskyDecomposition<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // create new instance
  template<typename T>
  typename choleskyDecomposition<T>::parameters*
  choleskyDecomposition<T>::parameters::newInstance() const {
    return new parameters();
  }

  // write the parameters
  template<typename T>
  bool
  choleskyDecomposition<T>::parameters::write(ioHandler& handler,
                                              const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    b = b && lti::write(handler,"triangularMatrixType",triangularMatrixType);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  //read the parameters
  template<typename T>
  bool
  choleskyDecomposition<T>::parameters::read(ioHandler& handler,
                                             const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    b = b && lti::read(handler,"triangularMatrixType",triangularMatrixType);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }


  // --------------------------------------------------
  // choleskyDecomposition
  // --------------------------------------------------

  // default constructor
  template<typename T>
  choleskyDecomposition<T>::choleskyDecomposition()
    : linearAlgebraFunctor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template<typename T>
  choleskyDecomposition<T>::choleskyDecomposition(const parameters& par)
    : linearAlgebraFunctor() {
    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<typename T>
  choleskyDecomposition<T>::choleskyDecomposition(const choleskyDecomposition<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template<typename T>
  choleskyDecomposition<T>::~choleskyDecomposition() {
  }

  // copy member
  template<typename T>
  choleskyDecomposition<T>&
  choleskyDecomposition<T>::copy(const choleskyDecomposition<T>& other) {
    linearAlgebraFunctor::copy(other);

    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  template<typename T>
  choleskyDecomposition<T>&
  choleskyDecomposition<T>::operator=(const choleskyDecomposition<T>& other) {
    return (copy(other));
  }

  // return type name
  template<typename T>
  const std::string& choleskyDecomposition<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template<typename T>
  choleskyDecomposition<T>* choleskyDecomposition<T>::clone() const {
    return new choleskyDecomposition<T>(*this);
  }

  // new instance of member
  template<typename T>
  choleskyDecomposition<T>* choleskyDecomposition<T>::newInstance() const {
    return new choleskyDecomposition<T>();
  }

  // return parameters
  template<typename T>
  const typename choleskyDecomposition<T>::parameters&
  choleskyDecomposition<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On place apply for type matrix<T>!
  template<typename T>
  bool choleskyDecomposition<T>::apply(matrix<T>& a) const {
    return apply(a, getParameters().triangularMatrixType);
  }

  template<typename T>
  bool choleskyDecomposition<T>::apply(matrix<T>& a,
                                   const eTriangularMatrixType& tType) const {

    const int sz=a.rows();
    int i,j,k;

#ifdef _LTI_DEBUG
    _lti_debug2("input matrix\n" << a << "\n");
#endif
    
    if (tType==Upper) {

#ifdef _LTI_DEBUG      
      _lti_debug2("Upper");
#endif

      for (k=0; k<sz; ++k) {
        if (a.at(k,k)<std::numeric_limits<T>::epsilon()) {
          setStatusString("Matrix is not positive definite\n");
          a.clear();
          return false;
        }
        a.at(k,k)=sqrt(a.at(k,k));
        for (i=k+1; i<sz; ++i) {
          a.at(k,i)/=a.at(k,k);
        }
        for (j=k+1; j<sz; ++j) {
          for (i=j; i<sz; ++i) {
            a.at(j,i)-=a.at(k,i)*a.at(k,j);
          }
        }
#ifdef _LTI_DEBUG
        _lti_debug4("matrix at k=" << k << "\n" << a << "\n");
#endif
      }
      
      //delete lower triangle
      for (i=0; i<sz; i++) {
        for (j=i+1; j<sz; j++) {
          a.at(j,i)=T(0);
        }
      }
      
#ifdef _LTI_DEBUG
      _lti_debug2("result matrix\n" << a << "\n");
#endif

      return true;
      
    } else if (tType==Lower) {
      
#ifdef _LTI_DEBUG
      _lti_debug2("Lower");
#endif

      for (k=0; k<sz; ++k) {
        if (a.at(k,k)<std::numeric_limits<T>::epsilon()) {
          setStatusString("Matrix is not positive definite\n");
          a.clear();
          return false;
        }
        a.at(k,k)=sqrt(a.at(k,k));
        for (i=k+1; i<sz; ++i) {
          a.at(i,k)/=a.at(k,k);
        }
        for (j=k+1; j<sz; ++j) {
          for (i=j; i<sz; ++i) {
            a.at(i,j)-=a.at(i,k)*a.at(j,k);
          }
        }
      }
      
      //delete upper  triangle
      for (i=0; i<sz; i++) {
        for (j=i+1; j<sz; j++) {
          a.at(i,j)=T(0);
        }
      }
      
#ifdef _LTI_DEBUG
      _lti_debug2("result matrix\n" << a << "\n");
#endif

      return true;

    } else {

      setStatusString("Illegal value of triangularMatrixType");
      a.clear();
      return false;
    }

  }

  // On copy apply for type matrix<T>!
  template<typename T>
  bool choleskyDecomposition<T>::apply(const matrix<T>& src,matrix<T>& dest,
                                    const eTriangularMatrixType& tType) const {
    dest.copy(src);
    return apply(dest,tType);
  }

  template<typename T>
  bool choleskyDecomposition<T>::apply(const matrix<T>& src,matrix<T>& dest) const {
    dest.copy(src);
    return apply(dest,getParameters().triangularMatrixType);
  }

}
