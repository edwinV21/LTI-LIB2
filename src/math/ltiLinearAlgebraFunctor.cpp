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
 * \file   ltiLinearAlgebraFunctor.cpp
 *         Contains the class lti::linearAlgebraFunctor which is the
 *         base class of all linear algebra functors.
 * \author Thomas Rusert
 * \date   01.06.1999
 * 
 * $Id: ltiLinearAlgebraFunctor.cpp,v 1.6 2006-08-29 12:27:44 doerfler Exp $
 */

#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  // =================================
  // ========== parameters ===========
  // =================================

  // default constructor
  linearAlgebraFunctor::parameters::parameters() : functor::parameters() {
#ifdef HAVE_LAPACK
    useLapack = true;
#else
    useLapack = false;
#endif
  }

  // copy constructor
  linearAlgebraFunctor::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  linearAlgebraFunctor::parameters::~parameters() {
  }

  //copy member
  linearAlgebraFunctor::parameters& 
  linearAlgebraFunctor::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    useLapack = other.useLapack;

    return *this;
  }

  // alias for copy method
  linearAlgebraFunctor::parameters&
  linearAlgebraFunctor::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  const std::string&
  linearAlgebraFunctor::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  linearAlgebraFunctor::parameters*
  linearAlgebraFunctor::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  linearAlgebraFunctor::parameters*
  linearAlgebraFunctor::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool linearAlgebraFunctor::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && functor::parameters::write(handler,false);
    b = b && lti::write(handler, "useLapack", useLapack);

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
  bool linearAlgebraFunctor::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && functor::parameters::read(handler,false);
    b = b && lti::read(handler, "useLapack", useLapack);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // linearAlgebraFunctor
  // --------------------------------------------------

  // default constructor
  linearAlgebraFunctor::linearAlgebraFunctor()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  linearAlgebraFunctor::linearAlgebraFunctor(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  linearAlgebraFunctor::linearAlgebraFunctor(const linearAlgebraFunctor& other)
    : functor() {
    copy(other);
  }

  // destructor
  linearAlgebraFunctor::~linearAlgebraFunctor() {
  }

  // copy member
  linearAlgebraFunctor& 
  linearAlgebraFunctor::copy(const linearAlgebraFunctor& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  linearAlgebraFunctor&
  linearAlgebraFunctor::operator=(const linearAlgebraFunctor& other) {
    return (copy(other));
  }

  // return parameters
  const linearAlgebraFunctor::parameters&
  linearAlgebraFunctor::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }
}


