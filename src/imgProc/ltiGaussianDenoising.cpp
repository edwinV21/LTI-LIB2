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
 * \file   ltiGaussianDenoising.cpp
 *         Contains the class gaussianDenoising,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   05.10.2007
 *
 * revisions ..: $Id: ltiGaussianDenoising.cpp,v 1.2 2007-10-14 02:51:00 alvarado Exp $
 */

#include "ltiGaussianDenoising.h"
#include "ltiGaussKernels.h"

namespace lti {
  // --------------------------------------------------
  // gaussianDenoising::parameters
  // --------------------------------------------------

  // default constructor
  gaussianDenoising::parameters::parameters()
    : denoising::parameters() {
    variance = 1.4426950409;
    kernelSize = 3;
  }

  // copy constructor
  gaussianDenoising::parameters::parameters(const parameters& other)
    : denoising::parameters() {
    copy(other);
  }

  // destructor
  gaussianDenoising::parameters::~parameters() {
  }

  // copy member

  gaussianDenoising::parameters&
  gaussianDenoising::parameters::copy(const parameters& other) {
    denoising::parameters::copy(other);

    
    variance = other.variance;
    kernelSize = other.kernelSize;

    return *this;
  }

  // alias for copy method
  gaussianDenoising::parameters&
  gaussianDenoising::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& gaussianDenoising::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  gaussianDenoising::parameters*
  gaussianDenoising::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  gaussianDenoising::parameters*
  gaussianDenoising::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool gaussianDenoising::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"variance",variance);
      lti::write(handler,"kernelSize",kernelSize);
    }

    b = b && denoising::parameters::write(handler,false);

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
  bool gaussianDenoising::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"variance",variance);
      lti::read(handler,"kernelSize",kernelSize);
    }

    b = b && denoising::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // gaussianDenoising
  // --------------------------------------------------

  // default constructor
  gaussianDenoising::gaussianDenoising()
    : denoising() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  gaussianDenoising::gaussianDenoising(const parameters& par)
    : denoising() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  gaussianDenoising::gaussianDenoising(const gaussianDenoising& other)
    : denoising() {
    copy(other);
  }

  // destructor
  gaussianDenoising::~gaussianDenoising() {
  }

  // copy member
  gaussianDenoising& 
  gaussianDenoising::copy(const gaussianDenoising& other) {
    denoising::copy(other);

    return (*this);
  }

  // alias for copy member
  gaussianDenoising&
  gaussianDenoising::operator=(const gaussianDenoising& other) {
    return (copy(other));
  }

  // class name
  const std::string& gaussianDenoising::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  gaussianDenoising* gaussianDenoising::clone() const {
    return new gaussianDenoising(*this);
  }

  // create a new instance
  gaussianDenoising* gaussianDenoising::newInstance() const {
    return new gaussianDenoising();
  }

  // return parameters
  const gaussianDenoising::parameters&
  gaussianDenoising::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool gaussianDenoising::updateParameters() {
    const parameters& par = getParameters();
    bool b;

    convolution::parameters cpar;
    gaussKernel2D<ubyte> ugk(par.kernelSize,par.variance);
    gaussKernel2D<float> fgk(par.kernelSize,par.variance);
    cpar.boundaryType = par.boundaryType;
    cpar.setKernel(ugk);
    b = uconv_.setParameters(cpar);
    cpar.setKernel(fgk);
    return b && fconv_.setParameters(cpar);
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type channel!
  bool gaussianDenoising::apply(channel& srcdest) const {
    return fconv_.apply(srcdest);
  }

  // On place apply for type channel8!
  bool gaussianDenoising::apply(channel8& srcdest) const {
    return uconv_.apply(srcdest);
  }

  // On copy apply for type channel!
  bool gaussianDenoising::apply(const channel& src, channel& dest) const {
    return fconv_.apply(src,dest);
  }

  // On copy apply for type channel8!
  bool gaussianDenoising::apply(const channel8& src,
                                      channel8& dest) const {

    return uconv_.apply(src,dest);
  }
}

