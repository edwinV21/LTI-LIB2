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

/**
 * \file   ltiGridSampling.cpp
 *         Contains the class gridSampling, which is 
 *         a local sampling functor.
 * \author Arnd Hannemann
 * \date   12.8.2004
 *
 * revisions ..: $Id: ltiGridSampling.cpp,v 1.5 2008-03-18 21:00:46 alvarado Exp $
 */

#include "ltiGridSampling.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

#include "ltiFactory.h"

namespace lti {
  //register in localSampling factory
  _LTI_REGISTER_IN_FACTORY(localSampling,gridSampling)

  // --------------------------------------------------
  // gridSampling::parameters
  // --------------------------------------------------

  // default constructor
  
  gridSampling::parameters::parameters()
    : localSampling::parameters() {
    
    useOrientation = true;
    gridRadius = 3;
  }

  // copy constructor
  
  gridSampling::parameters::parameters(const parameters& other)
    : localSampling::parameters() {
    copy(other);
  }

  // destructor
  
  gridSampling::parameters::~parameters() {
  }

  // get type name
  
  const std::string& gridSampling::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  gridSampling::parameters&
  gridSampling::parameters::copy(const parameters& other) {
    localSampling::parameters::copy(other);

    
      useOrientation = other.useOrientation;
      gridRadius = other.gridRadius;

    return *this;
  }

  // alias for copy method 
   gridSampling::parameters&
  gridSampling::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone method
   gridSampling::parameters*
  gridSampling::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
   gridSampling::parameters* 
  gridSampling::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */  
  bool gridSampling::parameters::write(ioHandler& handler,
                                            bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"useOrientation",useOrientation);
      lti::write(handler,"gridRadius",gridRadius);
    }

    b = b && localSampling::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool gridSampling::parameters::read(ioHandler& handler,
                                      bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"useOrientation",useOrientation);
      lti::read(handler,"gridRadius",gridRadius);
    }

    b = b && localSampling::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // gridSampling
  // --------------------------------------------------

  // default constructor  
  gridSampling::gridSampling()
    : localSampling() {

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

  // default constructor
  
  gridSampling::gridSampling(const parameters& par)
    : localSampling() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  
  gridSampling::gridSampling(const gridSampling& other)
    : localSampling() {
    copy(other);
  }

  // destructor
  
  gridSampling::~gridSampling() {
  }

  // returns the name of this type
  
  const std::string& gridSampling::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  
  gridSampling& 
  gridSampling::copy(const gridSampling& other) {
    localSampling::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  
  gridSampling&
  gridSampling::operator=(const gridSampling& other) {
    return (copy(other));
  }

  // clone member
  
  gridSampling* gridSampling::clone() const {
    return new gridSampling(*this);
  }

  // clone member
  
  gridSampling* gridSampling::newInstance() const {
    return new gridSampling();
  }

  // return parameters
  const gridSampling::parameters&
  gridSampling::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // ------------
  // own updateParameters() method
  // ------------
  bool gridSampling::updateParameters() {
    const parameters& par = getParameters();
    // initialize mask
    return initCircle_.apply(par.gridRadius,circPoints_);
  }

}
#include "ltiUndebug.h" 
