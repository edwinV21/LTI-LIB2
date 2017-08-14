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
 * \file   ltiValidator.cpp
 *         Contains a class that checks the validity of the elements of
 *         matrices and vectors of floating point types (e.g. float and double). * \author Jochen Wickel
 * \date   05.12.2001
 * 
 * $Id: ltiValidator.cpp,v 1.7 2007-04-05 22:13:26 alvarado Exp $
 */

#include "ltiConfig.h"
#include "ltiObject.h"
#include "ltiMacroSymbols.h"

#ifdef _LTI_GNUC
#  define _GNU_SOURCE 1 // used by <cfloat> to activate some features
                        // (see glibc's include/features.h)
#  ifdef _GNU_SOURCE    // a silly question, but avoids warning!
#  include <cfloat>
#  endif
#  include "ltiMath.h"
#else
#  include <cfloat>
#endif

#include "ltiValidator.h"
#include "ltiClassName.h"

namespace lti {
  // --------------------------------------------------
  // validator::parameters
  // --------------------------------------------------

  // default constructor
  validator::parameters::parameters()
    : functor::parameters() {
    
    isDoubleValid=&defaultValidateDouble;
    isFloatValid=&defaultValidateFloat;
  }

  // copy constructor
  validator::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  validator::parameters::~parameters() {
  }

  // copy member

  validator::parameters&
  validator::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    isDoubleValid=other.isDoubleValid;
    isFloatValid=other.isFloatValid;

    return *this;
  }

  // alias for copy member
  validator::parameters&
    validator::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& validator::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  functor::parameters* validator::parameters::clone() const {
    return new parameters(*this);
  }
 
  // new instance member
  functor::parameters* validator::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool validator::parameters::write(ioHandler& handler,
				    const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool validator::parameters::read(ioHandler& handler,
				   const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  bool validator::parameters::defaultValidateDouble(double x) {
    //return !((isnan(x) != 0) || (isinf(x) != 0));
    return (::finite(x) != 0);
  }

  bool validator::parameters::defaultValidateFloat(float x) {
    //return !((isnan(x) != 0) || (isinf(x) != 0));
    return (::finite(x) != 0);
  }


  // --------------------------------------------------
  // validator
  // --------------------------------------------------

  // default constructor
  validator::validator()
    : functor(){

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // copy constructor
  validator::validator(const validator& other)
    : functor()  {
    copy(other);
  }

  // destructor
  validator::~validator() {
  }

  // copy member
  validator& validator::copy(const validator& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  validator& validator::operator=(const validator& other) {
    return (copy(other));
  }

  // class name
  const std::string& validator::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  functor* validator::clone() const {
    return new validator(*this);
  }

  // new instance member
  functor* validator::newInstance() const {
    return new validator;
  }

  // return parameters
  const validator::parameters&
    validator::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(className::get(this));
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type vector<double>!
  bool validator::apply(const vector<double>& src) const {
    bool valid=true;

    doubleValidator& isvalid=*getParameters().isDoubleValid;

    vector<double>::const_iterator it=src.begin();
    const vector<double>::const_iterator itend=src.end();
    while (it != itend && valid) {
      valid=(valid && isvalid(*it++));
    }

    return valid;
  }

  // On place apply for type vector<float>!
  bool validator::apply(const vector<float>& src) const {
    bool valid=true;

    floatValidator& isvalid=*getParameters().isFloatValid;

    vector<float>::const_iterator it=src.begin();
    const vector<float>::const_iterator itend=src.end();
    while (it != itend && valid) {
      valid=(valid && isvalid(*it++));
    }

    return valid;
  }

  // On place apply for type matrix<double>!
  bool validator::apply(const matrix<double>& src) const {
    bool valid=true;

    doubleValidator& isvalid=*getParameters().isDoubleValid;

    matrix<double>::const_iterator it=src.begin();
    const matrix<double>::const_iterator itend=src.end();
    while (it != itend && valid) {
      valid=(valid && isvalid(*it++));
    }

    return valid;
  }

  // On place apply for type matrix<float>!
  bool validator::apply(const matrix<float>& src) const {
    bool valid=true;

    floatValidator& isvalid=*getParameters().isFloatValid;

    matrix<float>::const_iterator it=src.begin();
    const matrix<float>::const_iterator itend=src.end();
    while (it != itend && valid) {
      valid=(valid && isvalid(*it++));
    }

    return valid;
  }


}




