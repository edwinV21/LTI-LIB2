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
 * \file   ltiLocalSampling.cpp
 *         Contains the class localSampling.
 * \author Arnd Hannemann
 * \date   10.8.2004
 *
 * revisions ..: $Id: ltiLocalSampling.cpp,v 1.12 2009-08-30 04:30:29 alvarado Exp $
 */

//header and template implementation are included separetely
//to avoid forward class declarations
#include "ltiLocalSampling.h"
#include "ltiLocalSampling_template.h"

namespace lti {


  // --------------------------------------------------
  // localSampling::parameters
  // --------------------------------------------------

  // default constructor
  localSampling::parameters::parameters()
    : functor::parameters() {
    keyIsOrientation = false;
  }

  // copy constructor
  localSampling::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  localSampling::parameters::~parameters() {
  }

  // copy member
  localSampling::parameters&
  localSampling::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    keyIsOrientation = other.keyIsOrientation;
    return *this;
  }

  // alias for copy method
  localSampling::parameters&
  localSampling::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& localSampling::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  localSampling::parameters* localSampling::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  localSampling::parameters* localSampling::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool localSampling::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"keyIsOrientation",keyIsOrientation);
    }

    b = b && functor::parameters::write(handler,false);

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
  bool localSampling::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"keyIsOrientation",keyIsOrientation);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // localSampling
  // --------------------------------------------------

  // default constructor
  localSampling::localSampling()
    : functor() {
    
    // no last cast
    last_ = 0;

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  localSampling::localSampling(const parameters& par)
    : functor() {

    // no last cast
    last_ = 0;

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  localSampling::localSampling(const localSampling& other)
    : functor() {
    copy(other);
  }

  // destructor
  localSampling::~localSampling() {
  }

  // copy member
  localSampling& 
  localSampling::copy(const localSampling& other) {
    functor::copy(other);

    last_ = other.last_;

    return (*this);
  }

  // alias for copy member
  localSampling&
  localSampling::operator=(const localSampling& other) {
    return (copy(other));
  }

  // class name
  const std::string& localSampling::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  localSampling* localSampling::clone() const {
    return new localSampling(*this);
  }

  // clone member
  localSampling* localSampling::newInstance() const {
    return new localSampling();
  }

  // return parameters
  const localSampling::parameters&
  localSampling::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  int localSampling::getNumberOfRegions() const {
    return 1;
  }

  localSampling::eResultOrder localSampling::getResultOrder() const {
    return Sequential;
  }

  void localSampling::switchOrder(dvector& srcdest) const {
    // pointer on vector memory
    double* p = &(srcdest[0]);
    int r = getNumberOfRegions();
    int c = srcdest.size()/r;
    matrix<double> tmp;
    if (getResultOrder()==Interleaved) {
      tmp.useExternData(c,r,p);
    } else {
      tmp.useExternData(r,c,p);
    }
    tmp.transpose();
    // if no new memory was allocated we MUST NOT detach
    if (&(tmp[0][0]) == p) return;
    tmp.detach(srcdest);
  }
}

