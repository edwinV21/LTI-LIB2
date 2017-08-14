/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is backported from the Computer Vision and Robotics Library
 * (CVR-Lib ) for the LTI-Lib-2
 *
 * The CVR-Lib is free software; you can redistribute it and/or
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
 * \file   ltiAffineTransformation2D.cpp
 *         Contains the class affineTransformation2D,
 *         which has still to be documented by Pablo Alvarado.
 *
 * \author Pablo Alvarado
 * \date   15.01.2008
 *
 * revisions ..: $Id: ltiAffineTransformation2D.cpp,v 1.1 2009-10-19 01:30:16 alvarado Exp $
 */

#include "ltiAffineTransformation2D.h"
#include "ltiMath.h"
#include "ltiConstants.h"
#include "ltiSVD.h"

namespace lti {
  // --------------------------------------------------
  // affineTransformation2D::parameters
  // --------------------------------------------------

  // default constructor
  affineTransformation2D::parameters::parameters()
    : functor::parameters() {

    translation.set(0.0,0.0);
    a00 = 1.0f;
    a01 = 0.0f;
    a10 = 0.0f;
    a11 = 1.0f;
  }

  // copy constructor
  affineTransformation2D::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  affineTransformation2D::parameters::~parameters() {
  }

  // copy member
  affineTransformation2D::parameters&
  affineTransformation2D::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    translation = other.translation;
    a00 = other.a00;
    a01 = other.a01;
    a10 = other.a10;
    a11 = other.a11;
    llsParameters.copy(other.llsParameters);
    return *this;
  }

  // alias for copy method
  affineTransformation2D::parameters&
  affineTransformation2D::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& affineTransformation2D::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  affineTransformation2D::parameters*
  affineTransformation2D::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  affineTransformation2D::parameters*
  affineTransformation2D::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool affineTransformation2D::parameters::write(ioHandler& handler,
                                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"translation",translation);
      lti::write(handler,"a00",a00);
      lti::write(handler,"a01",a01);
      lti::write(handler,"a10",a10);
      lti::write(handler,"a11",a11);
      lti::write(handler,"llsParameters",llsParameters);
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
  bool affineTransformation2D::parameters::read(ioHandler& handler,
                                                    const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"translation",translation);
      lti::read(handler,"a00",a00);
      lti::read(handler,"a01",a01);
      lti::read(handler,"a10",a10);
      lti::read(handler,"a11",a11);
      lti::read(handler,"llsParameters",llsParameters);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // affineTransformation2D
  // --------------------------------------------------

  // default constructor
  affineTransformation2D::affineTransformation2D()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  affineTransformation2D::affineTransformation2D(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  affineTransformation2D::
  affineTransformation2D(const affineTransformation2D& other)
    : functor() {
    copy(other);
  }

  // destructor
  affineTransformation2D::~affineTransformation2D() {
  }

  // copy member
  affineTransformation2D&
  affineTransformation2D::copy(const affineTransformation2D& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  affineTransformation2D&
  affineTransformation2D::operator=(const affineTransformation2D& other) {
    return (copy(other));
  }

  // class name
  const std::string& affineTransformation2D::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  affineTransformation2D* affineTransformation2D::clone() const {
    return new affineTransformation2D(*this);
  }

  // create a new instance
  affineTransformation2D* affineTransformation2D::newInstance() const {
    return new affineTransformation2D();
  }

  // return parameters
  const affineTransformation2D::parameters&
  affineTransformation2D::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  affineTransformation2D::parameters&
  affineTransformation2D::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool affineTransformation2D::updateParameters() {
    const parameters& par = getParameters();
    a00_ = par.a00;
    a01_ = par.a01;
    a10_ = par.a10;
    a11_ = par.a11;
    trans_.copy(par.translation);
    return lls_.setParameters(par.llsParameters);
  }

  int affineTransformation2D::dof() const {
    return 6;
  }

  void affineTransformation2D::generateMatrix(fmatrix& mat) const {
    mat.allocate(3,3);
    mat.at(0,0) =  a00_;
    mat.at(0,1) =  a01_;
    mat.at(0,2) =  trans_.x;

    mat.at(1,0) =  a10_;
    mat.at(1,1) =  a11_;
    mat.at(1,2) =  trans_.y;

    mat.at(2,0) =  0.0f;
    mat.at(2,1) =  0.0f;
    mat.at(2,2) =  1.0f;
  }

  void affineTransformation2D::decompose(float& theta,
                                         float& phi,
                                         float& lambda1,
                                         float& lambda2,
                                         float& tx,
                                         float& ty) const {
    tx = trans_.x;
    ty = trans_.y;
    
    svd::parameters svdPar;
    svdPar.sort = true;
    svdPar.transposeU = false;
    svdPar.transposeV = true;
    svdPar.useDivideAndConquer = true;
    svdPar.useMinDimensions = true;
    svd svdFunctor(svdPar);

    fmatrix Q;
    fmatrix U,Vt;
    fvector D;
    fmatrix A(2,2);

    A.at(0,0)= a00_;
    A.at(0,1)= a01_;
    A.at(1,0)= a10_;
    A.at(1,1)= a11_;

    if (svdFunctor.apply(A,U,D,Vt)) {
      fmatrix rtheta;
      rtheta.multiply(U,Vt);
      theta=atan2(rtheta.at(1,0),rtheta.at(0,0));
      lambda1 = D.at(0);
      lambda2 = D.at(1);
      if ((lambda1 == lambda2) && (Vt.at(0,0) == 0.0f)) {
        phi = 0.0f;
      } else {
        phi  =atan2(Vt.at(0,1),Vt.at(0,0));
      }
    } 
    
  }

  fmatrix affineTransformation2D::generateMatrix() const {
    fmatrix mat;
    generateMatrix(mat);
    return mat;
  }
}

