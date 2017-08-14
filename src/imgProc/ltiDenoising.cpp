/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiDenoising.cpp
 *         Contains the class denoising,
 *         which is the parent class of all denoising functors like
 *         lti::median, lti::susanDenoising, etc.
 * \author Pablo Alvarado
 * \date   14.09.2007
 *
 * revisions ..: $Id: ltiDenoising.cpp,v 1.2 2007-10-14 02:51:00 alvarado Exp $
 */

#include "ltiDenoising.h"

namespace lti {
  // --------------------------------------------------
  // denoising::parameters
  // --------------------------------------------------

  // default constructor
  denoising::parameters::parameters()
    : functor::parameters() {
    
    boundaryType = Zero;
  }

  // copy constructor
  denoising::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  denoising::parameters::~parameters() {
  }

  // copy member

  denoising::parameters&
  denoising::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    boundaryType = other.boundaryType;

    return *this;
  }

  // alias for copy method
  denoising::parameters&
  denoising::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& denoising::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  denoising::parameters*
  denoising::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  denoising::parameters*
  denoising::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool denoising::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"boundaryType",boundaryType);
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
  bool denoising::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // denoising
  // --------------------------------------------------

  // default constructor
  denoising::denoising()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  denoising::denoising(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  denoising::denoising(const denoising& other)
    : functor(),
      channelProcessingInterface<float>(),
      channelProcessingInterface<ubyte>() {
    copy(other);
  }

  // destructor
  denoising::~denoising() {
  }

  // copy member
  denoising& 
  denoising::copy(const denoising& other) {
    functor::copy(other);
    return (*this);
  }

  // alias for copy member
  denoising&
  denoising::operator=(const denoising& other) {
    return (copy(other));
  }

  // class name
  const std::string& denoising::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  const denoising::parameters&
  denoising::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type channel8!
  bool denoising::apply(channel8& srcdest) const {
    channel8 tmp;
    return apply(srcdest,tmp);
  }

  // On place apply for type channel!
  bool denoising::apply(channel& srcdest) const {
    channel tmp;
    return apply(srcdest,tmp);
  }

}

