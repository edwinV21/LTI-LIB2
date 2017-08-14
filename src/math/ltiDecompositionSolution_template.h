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

/**
 * \file   ltiDecompositionSolution_template.h
 * \author Thomas Rusert
 * \date   02.06.1999
 *
 * $Id: ltiDecompositionSolution_template.h,v 1.2 2006-08-29 12:27:43 doerfler Exp $
 */

namespace lti {

  //-----------------------------------------
  //--- decompositionSolution::parameters ---
  //-----------------------------------------

  // constructor
  template<class T>
  decompositionSolution<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters(),
    systemMatrix(0,0) {
  }

  // copy constructor
  template<class T>
  decompositionSolution<T>::parameters::parameters(const typename decompositionSolution<T>::parameters& other)
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  // copy member
  template<class T>
  typename decompositionSolution<T>::parameters& 
  decompositionSolution<T>::parameters::copy
  (const typename decompositionSolution<T>::parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    
    // if other has its own data, make a copy of it
    if (other.systemMatrix.ownsData()) {
      systemMatrix.copy(other.systemMatrix);
    } else {
      // if other does not own its data, continue referencing the original
      // data (this is used in case of huge matrices (> 1GB!)
      systemMatrix.useExternData(other.systemMatrix.rows(),
                                 other.systemMatrix.columns(),
                                 const_cast<T*>(&other.systemMatrix.at(0,0)));
    }
    
    return (*this);
  }

  // name
  template<class T>
  const std::string& decompositionSolution<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template <class T>
  typename decompositionSolution<T>::parameters* 
  decompositionSolution<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance
  template <class T>
  typename decompositionSolution<T>::parameters* 
  decompositionSolution<T>::parameters::newInstance() const {
    return new parameters();
  }


  //-----------------------------
  //--- decompositionSolution ---
  //-----------------------------

  // construction
  template<class T>
  decompositionSolution<T>::decompositionSolution() {
    decomposed_=false;
  }

  template<class T>
  decompositionSolution<T>::decompositionSolution(const parameters& theParams){
    setParameters(theParams);
  }

  template<class T>
  decompositionSolution<T>::decompositionSolution(const matrix<T>& theMatrix) {
    parameters tmpParam;
    tmpParam.systemMatrix = theMatrix;
    setParameters(tmpParam);
  }

  // sets the functor's parameters.
  template<class T>
  bool decompositionSolution<T>::updateParameters() {
    decomposed_=false;
    return true;
  }

  // returns the current parameters
  template<class T>
  const typename decompositionSolution<T>::parameters&
  decompositionSolution<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(params)) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  // copy data of "other" functor.
  template<class T>
  decompositionSolution<T>&
  decompositionSolution<T>::copy(const decompositionSolution<T>& other) {
    linearAlgebraFunctor::copy(other);
    decomposed_=other.decomposed_;
    dcmpMat_=other.dcmpMat_;

    return (*this);
  }

  template<class T>
  const std::string& decompositionSolution<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
}

