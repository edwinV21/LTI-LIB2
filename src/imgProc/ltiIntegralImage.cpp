/*
 * Copyright (C) 2007 by Pablo Alvarado
 * 
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiIntegralImage.cpp
 *         Contains the class lti::integralImage, 
 *         which is used as a preliminary stage of several other algorithms
 *         that need to convolve with boxes filled with a constant value.
 * 
 * \author Pablo Alvarado
 * \date   16.10.2007
 *
 * revisions ..: $Id: ltiIntegralImage.cpp,v 1.5 2007-10-29 01:38:32 alvarado Exp $
 */

#include "ltiIntegralImage.h"

namespace lti {
  // --------------------------------------------------
  // integralImage::parameters
  // --------------------------------------------------

  // default constructor
  integralImage::parameters::parameters()
    : functor::parameters() {
    
    boundaryType = Zero;
  }

  // copy constructor
  integralImage::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  integralImage::parameters::~parameters() {
  }

  // copy member

  integralImage::parameters&
  integralImage::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    boundaryType = other.boundaryType;

    return *this;
  }

  // alias for copy method
  integralImage::parameters&
  integralImage::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& integralImage::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  integralImage::parameters*
  integralImage::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  integralImage::parameters*
  integralImage::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool integralImage::parameters::write(ioHandler& handler,
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
  bool integralImage::parameters::read(ioHandler& handler,
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
  // integralImage
  // --------------------------------------------------

  // default constructor
  integralImage::integralImage()
    : functor()
#ifndef NDEBUG
      ,sumFloat_(0),sumInt_(0) 
#endif
  {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  integralImage::integralImage(const eBoundaryType boundaryType)
    : functor()
#ifndef NDEBUG
    ,sumFloat_(0),sumInt_(0) 
#endif
  {
    // create an instance of the parameters with the default values
    parameters params;
    params.boundaryType = boundaryType;
    setParameters(params);
  }

  // default constructor
  integralImage::integralImage(const parameters& par)
    : functor()
#ifndef NDEBUG
    ,sumFloat_(0),sumInt_(0) 
#endif
  {
    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  integralImage::integralImage(const integralImage& other)
    : functor()
#ifndef NDEBUG
    ,sumFloat_(0),sumInt_(0) 
#endif
  {
    copy(other);
  }

  // destructor
  integralImage::~integralImage() {
#ifndef NDEBUG
    delete sumFloat_;
    delete sumInt_;
#endif
  }

  // copy member
  integralImage& 
  integralImage::copy(const integralImage& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  integralImage&
  integralImage::operator=(const integralImage& other) {
    return (copy(other));
  }

  // class name
  const std::string& integralImage::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  integralImage* integralImage::clone() const {
    return new integralImage(*this);
  }

  // create a new instance
  integralImage* integralImage::newInstance() const {
    return new integralImage();
  }

  // return parameters
  const integralImage::parameters&
  integralImage::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool integralImage::updateParameters() {
    const parameters& par = getParameters();
    boundaryType_=par.boundaryType; // initialize shadow

#ifndef NDEBUG
    delete sumFloat_;
    delete sumInt_;

    switch(boundaryType_) {
      case NoBoundary:
        sumFloat_ = new sumNoBoundary<float>;
        sumInt_ = new sumNoBoundary<int32>;
        break;
      case Zero:
        sumFloat_ = new sumZero<float>;
        sumInt_ = new sumZero<int32>;
        break;
      case Constant:
        sumFloat_ = new sumConstant<float>;
        sumInt_ = new sumConstant<int32>;
        break;
      case Periodic:
        sumFloat_ = new sumPeriodic<float>;
        sumInt_ = new sumPeriodic<int32>;
        break;
      case Mirror:
        sumFloat_ = new sumMirror<float>;
        sumInt_ = new sumMirror<int32>;
        break;
      default:
        return false;
    }
#endif
    return true;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  template<typename T,typename U>
  bool integralImage::integrate(const matrix<T>& src,
                                      matrix<U>& dest) const {
    if (src.empty()) {
      dest.clear();
      return true;
    }

    dest.allocate(src.size());
    
    // first line is some special case:
    int y,ym1,x;
    U lineAcc; 
    lineAcc = dest.at(0,0) = static_cast<U>(src.at(0,0));
    
    for (x=1;x<src.columns();++x) {
      lineAcc += static_cast<U>(src.at(0,x));
      dest.at(0,x) = lineAcc;
    }

    //  we can proceed with the rest of the image,
    for (y=1,ym1=y-1;y<src.rows();++y,++ym1) {
      // the first pixel has no predecesor to the left
      lineAcc = static_cast<U>(src.at(y,0));
      dest.at(y,0) = dest.at(ym1,0) + lineAcc;

      // the remaining of the line
      for (x=1;x<src.columns();++x) {
        lineAcc += static_cast<U>(src.at(y,x));
        dest.at(y,x) = dest.at(ym1,x)+lineAcc;
      }
    }
    
    return true;
  }

  // The real methods

  // On copy apply for type matrix<ubyte>!
  bool integralImage::apply(const matrix<ubyte>& src,
                            matrix<int32>& dest) const {
    return integrate(src,dest);
  }

  // On copy apply for type fmatrix!
  bool integralImage::apply(const fmatrix& src, fmatrix& dest) const {
    return integrate(src,dest);
  }



}

