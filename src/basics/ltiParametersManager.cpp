/*
 * Copyright (C) 1998-2005
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
 * \file   ltiParametersManager.cpp
 * \author Pablo Alvarado
 * \date   06.01.2005
 *
 * $Id: ltiParametersManager.cpp,v 1.7 2009-04-19 04:05:08 alvarado Exp $
 */

#include "ltiParametersManager.h"
#include <sstream>

namespace lti {
 

  // --------------------------------------------------
  // parametersManager::parameters
  // --------------------------------------------------
  parametersManager::parameters::parameters()
    : ioObject() {
  }

  parametersManager::parameters::parameters(const parameters& other)
    : ioObject() {
    copy(other);
  }


  parametersManager::parameters::~parameters() {
  }

  parametersManager::parameters& 
  parametersManager::parameters::copy(const parameters&) {
    return (*this);
  }

  parametersManager::parameters&
  parametersManager::parameters::operator=(const parameters&){
    std::string errorMsg;

    errorMsg  = "operator= not implemented for " + name() + ". ";
    errorMsg += "Please use the copy() member instead or implement the ";
    errorMsg += "operator=() properly.";

    throw exception(errorMsg);

    return (*this);
  }

  bool parametersManager::parameters::write(ioHandler&,
					    const bool) const {
    return true;
  }

  bool parametersManager::parameters::read(ioHandler&,
					   const bool) {
    return true;
  }

  // ----------------------------
  // parametersManager
  // ----------------------------

  // constructor
  parametersManager::parametersManager()
    : params_(0),ownParams_(true) {
  }

  // copy constructor
  parametersManager::parametersManager(const parametersManager& other)
    : params_(0),ownParams_(true) {
    copy(other);
  }

  // destructor
  parametersManager::~parametersManager() {
    if (ownParams_) {
      delete params_;
      params_ = 0;
    }
    ownParams_=true;
  }

//   const std::string& parametersManager::name() const {
//     _LTI_RETURN_CLASS_NAME
//   }

  // copy constructor

  /* sets the parametersManager's parameters.
     The parametersManager keeps its own copy of the given parameters.*/
  bool parametersManager::setParameters(const parameters& theParams) {


    if (&theParams != params_) {
      // if the given instance is different than the one in use create
      // a copy
      if (ownParams_) {
        delete params_;
        params_ = 0;
      }
      params_ = theParams.clone();
    } else if (!ownParams_) {
      // if the given instance is the same one in use, but it do not
      // belong to us, then create a copy
      params_ = theParams.clone();
    } 
    // if the given instance is the same one in use and it is ours, then
    // just continue using it
    
    ownParams_ = true;

    return updateParameters();
  }

  bool parametersManager::useParameters(parameters& theParams) {
    // avoid removing the parameters if they do not belong to us or
    // if they are one and the same instance!
    if (ownParams_ && (&theParams != params_)) {
      delete params_;
      params_ = 0;
    }
    params_ = &theParams;
    ownParams_ = false;

    return updateParameters();
  }


  bool parametersManager::attachParameters(parameters& theParams) {
    // avoid removing the parameters if they do not belong to us or
    // if they are one and the same instance!
    if (ownParams_ && (&theParams != params_)) {
      delete params_;
      params_ = 0;
    }
    params_ = &theParams;
    ownParams_ = true;

    return updateParameters();
  }

  // updateParameters
  bool parametersManager::updateParameters() {
    // return true only if the parameters are valid
    return validParameters();
  }

  /*
   * returns current parameters.
   */
  const parametersManager::parameters&
  parametersManager::getParameters() const {
    return *params_;
  }

  parametersManager::parameters& parametersManager::getRWParameters() {
    return *params_;
  }

  /*
   * returns true if the parameters are valid
   */
  bool parametersManager::validParameters() const {
    return notNull(params_);
  }

  // copy data of "other" parametersManager.
  parametersManager& parametersManager::copy(const parametersManager& other) {
    if(other.validParameters()) {
      setParameters(other.getParameters());
    }
    else {
      delete params_;
      params_ = 0;
      ownParams_=true;
    }
    return (*this);
  }

  // alias for copy
  parametersManager& 
  parametersManager::operator=(const parametersManager&) {
    std::string str;
    // str = "operator=() not defined for class " + name() + ". ";
    str = "Please use copy() method instead.";
    throw exception(str);
    return (*this);
  }

  /*
   * Read the parametersManager (its parameters)
   */
  bool parametersManager::read(ioHandler& handler, const bool complete) {
    if (notNull(params_)) {
      if ( params_->read(handler,complete) ) {
        return updateParameters();
      }
      return false;
    } else {
      throw exception("Tried to read <NULL> parameters object");
      return false;
    }
  }

  /*
   * Write the parametersManager (its parameters)
   */
  bool parametersManager::write(ioHandler& handler, const bool complete) const{
    if (notNull(params_)) {
      return params_->write(handler,complete);
    } else {
      throw exception("Tried to write <NULL> parameters object");
      return false;
    }
  }

} // namespace lti

