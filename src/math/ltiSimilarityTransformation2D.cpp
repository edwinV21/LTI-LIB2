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
 * \file   ltiSimilarityTransformation2D.cpp
 *         Contains the class similarityTransformation2D,
 *         which has still to be documented by Pablo Alvarado.
 *
 * \author Pablo Alvarado
 * \date   15.01.2008
 *
 * revisions ..: $Id: ltiSimilarityTransformation2D.cpp,v 1.1 2009-10-19 01:14:05 alvarado Exp $
 */

#include "ltiSimilarityTransformation2D.h"
#include "ltiMath.h"
#include "ltiConstants.h"

namespace lti {
  // --------------------------------------------------
  // similarityTransformation2D::parameters
  // --------------------------------------------------

  // default constructor
  similarityTransformation2D::parameters::parameters()
    : functor::parameters() {

    translation.set(0.0,0.0);
    angle = 0.0f;
    scaling = 1.0f;
  }

  // copy constructor
  similarityTransformation2D::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  similarityTransformation2D::parameters::~parameters() {
  }

  // copy member
  similarityTransformation2D::parameters&
  similarityTransformation2D::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    translation = other.translation;
    angle = other.angle;
    scaling = other.scaling;
    llsParameters.copy(other.llsParameters);
    return *this;
  }

  // alias for copy method
  similarityTransformation2D::parameters&
  similarityTransformation2D::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& similarityTransformation2D::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  similarityTransformation2D::parameters*
  similarityTransformation2D::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  similarityTransformation2D::parameters*
  similarityTransformation2D::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool similarityTransformation2D::parameters::write(ioHandler& handler,
                                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"translation",translation);
      lti::write(handler,"angle",angle);
      lti::write(handler,"scaling",scaling);
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
  bool similarityTransformation2D::parameters::read(ioHandler& handler,
                                                    const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"translation",translation);
      lti::read(handler,"angle",angle);
      lti::read(handler,"scaling",scaling);
      lti::read(handler,"llsParameters",llsParameters);    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // similarityTransformation2D
  // --------------------------------------------------

  // default constructor
  similarityTransformation2D::similarityTransformation2D()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  similarityTransformation2D::similarityTransformation2D(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  similarityTransformation2D::
  similarityTransformation2D(const similarityTransformation2D& other)
    : functor() {
    copy(other);
  }

  // destructor
  similarityTransformation2D::~similarityTransformation2D() {
  }

  // copy member
  similarityTransformation2D&
  similarityTransformation2D::copy(const similarityTransformation2D& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  similarityTransformation2D&
  similarityTransformation2D::operator=(const similarityTransformation2D& other) {
    return (copy(other));
  }

  // class name
  const std::string& similarityTransformation2D::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  similarityTransformation2D* similarityTransformation2D::clone() const {
    return new similarityTransformation2D(*this);
  }

  // create a new instance
  similarityTransformation2D* similarityTransformation2D::newInstance() const {
    return new similarityTransformation2D();
  }

  // return parameters
  const similarityTransformation2D::parameters&
  similarityTransformation2D::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  similarityTransformation2D::parameters&
  similarityTransformation2D::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool similarityTransformation2D::updateParameters() {
    const parameters& par = getParameters();
    sincos( ((par.angle > constants<float>::twoPi()) ?
             degToRad(par.angle) :
             par.angle),
            ssina_,scosa_);
    ssina_*=par.scaling;
    scosa_*=par.scaling;
    trans_.copy(par.translation);
    return lls_.setParameters(par.llsParameters);
  }

  int similarityTransformation2D::dof() const {
    return 4;
  }

  void similarityTransformation2D::generateMatrix(fmatrix& mat) const {
    mat.allocate(3,3);
    mat.at(0,0) =  scosa_;
    mat.at(0,1) =  ssina_;
    mat.at(0,2) =  trans_.x;

    mat.at(1,0) = -ssina_;
    mat.at(1,1) =  scosa_;
    mat.at(1,2) =  trans_.y;

    mat.at(2,0) =  0.0f;
    mat.at(2,1) =  0.0f;
    mat.at(2,2) =  1.0f;
  }

  fmatrix similarityTransformation2D::generateMatrix() const {
    fmatrix mat;
    generateMatrix(mat);
    return mat;
  }
}

