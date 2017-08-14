/*
 * Copyright (C) 2007
 * Instituto Tecnológico de Costa Rica
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
 * \file   ltiUnsharpMasking.cpp
 *         Contains a class for image sharpening 
 * 
 * \author Miguel Angel Aguilar Ulloa
 * \date   17/10/2007
 *
 * revisions ..: $Id: ltiUnsharpMasking.cpp,v 1.1 2008-02-18 00:17:23 alvarado Exp $
 */

#include "ltiUnsharpMasking.h"
#include "ltiConvolution.h"

namespace lti {
  // --------------------------------------------------
  // unsharpMasking::parameters
  // --------------------------------------------------

  // default constructor
  unsharpMasking::parameters::parameters()
    : sharpening::parameters() {
    kernelType = sharpeningKernels::Laplacian;
    A = 1.0f;
    variance = 5.0;
    kernelSize = 3;
    neighborType = sharpeningKernels::FourNeighbor;
    boundaryType = Periodic;
  }

  // copy constructor
  unsharpMasking::parameters::parameters(const parameters& other)
    : sharpening::parameters() {
    copy(other);
  }

  // destructor
  unsharpMasking::parameters::~parameters() {
  }

  // copy member

  unsharpMasking::parameters&
  unsharpMasking::parameters::copy(const parameters& other) {
    sharpening::parameters::copy(other);
    
    kernelType = other.kernelType;
    A = other.A;
    variance = other.variance;
    kernelSize = other.kernelSize;
    neighborType = other.neighborType;

    return *this;
  }

  // alias for copy method
  unsharpMasking::parameters&
  unsharpMasking::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& unsharpMasking::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  unsharpMasking::parameters*
  unsharpMasking::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  unsharpMasking::parameters*
  unsharpMasking::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool unsharpMasking::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"kernelSize",kernelSize);
      lti::write(handler,"A",A);
      lti::write(handler,"kernelType",kernelType);
      lti::write(handler,"neighborType",neighborType);
      lti::write(handler,"variance",variance);
    }

    b = b && sharpening::parameters::write(handler,false);

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
  bool unsharpMasking::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"kernelSize",kernelSize);
      lti::read(handler,"A",A);
//       lti::read(handler,"kernelType",kernelType);
//       lti::read(handler,"neighborType",neighborType);
      lti::read(handler,"variance",variance);
    }

    b = b && sharpening::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // unsharpMasking
  // --------------------------------------------------

  // default constructor
  unsharpMasking::unsharpMasking()
    : sharpening() {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  unsharpMasking::unsharpMasking(const parameters& par)
    : sharpening() {
    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  unsharpMasking::unsharpMasking(const unsharpMasking& other)
    : sharpening() {
    copy(other);
  }

  // destructor
  unsharpMasking::~unsharpMasking() {
  }

  // copy member
  unsharpMasking& 
  unsharpMasking::copy(const unsharpMasking& other) {
    sharpening::copy(other);
    return (*this);
  }

  // alias for copy member
  unsharpMasking&
  unsharpMasking::operator=(const unsharpMasking& other) {
    return (copy(other));
  }

  // class name
  const std::string& unsharpMasking::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  unsharpMasking* unsharpMasking::clone() const {
    return new unsharpMasking(*this);
  }

  // create a new instance
  unsharpMasking* unsharpMasking::newInstance() const {
    return new unsharpMasking();
  }

  // return parameters
  const unsharpMasking::parameters&
  unsharpMasking::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&sharpening::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------
   
  // crop
  float unsharpMasking::crop (const float value) {
    return within(value,0.0f,1.0f);
  }

  // convolve the input channel with the selected kernel
  bool unsharpMasking::convolve(channel& srcdest) const {
    
    // get parameters
    const parameters& param = getParameters();
 
    // kernel size
    const int kernelSize =  param.kernelSize +
      ((param.kernelSize%2 == 0) ? 1:0);

    // high-boost constant
    const float A = param.A;

    // kernel type
    const sharpeningKernels::eKernelType kernelType = param.kernelType;
 
    // neighbor type
    const sharpeningKernels::eNeighborType neighborType = param.neighborType;
 
    // variance
    const double variance = param.variance;

    // boudary type
    const eBoundaryType boundaryType = param.boundaryType;

    // define the kernel
    sharpeningKernels kernel(kernelSize,A,kernelType,neighborType,variance);

    // convolution operator
    convolution filter(kernel,boundaryType);

    // apply the kernel to the src image to sharp it, 
    // and left the result in dest
    filter.apply(srcdest);

    // crop 
    srcdest.apply(crop);

    return true;
  }
 
  // On place apply for type channel!
  bool unsharpMasking::apply(channel8& srcdest) const {
    
    channel srcdestTmp;
    srcdestTmp.castFrom(srcdest);
    if(convolve(srcdestTmp)) {
      srcdest.castFrom(srcdestTmp);
      return true;
    }
    return false;
  }

  // On place apply for type channel!
  bool unsharpMasking::apply(channel& srcdest) const {

    if(convolve(srcdest)) {
      return true;
    }
    return false;
  }

  // On copy apply for type channel8!
  bool unsharpMasking::apply(const channel8& src, channel8& dest) const {
    
    channel tmp;
    tmp.castFrom(src);
    if (convolve(tmp)) {
      dest.castFrom(tmp);
      return true;
    }
    return false;
  }

  // On copy apply for type channel!
  bool unsharpMasking::apply(const channel& src, channel& dest) const {
    
    channel srcTmp;
    srcTmp.copy(src);
    if (convolve(srcTmp)) {
      srcTmp.detach(dest);
      return true;
    }
    return false;
  }
}

