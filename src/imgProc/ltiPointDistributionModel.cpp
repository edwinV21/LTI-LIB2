/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009
 * Pedro Urenna / Escuela de Ing. Electronica
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
 * \file   ltiPointDistributionModel.cpp
 *         Contains the data structure used with the ASM.
 * \author Benjamin Winkler
 * \date   23.10.2001
 * \author Pedro Urenna
 * \date   01.07.2009
 * revisions ..: $Id: ltiPointDistributionModel.cpp,v 1.1 2009-08-17 01:21:44 alvarado Exp $
 */

#include "ltiPointDistributionModel.h"

namespace lti {

  // default constructor
  pointDistributionModel::pointDistributionModel() : ioObject() {
    meanShape.clear();
    eigenVectorMatrix.clear();
    varianceVector.clear();
  }

  // destructor
  pointDistributionModel::~pointDistributionModel() {
  }

  // copy data from other ioObject

  pointDistributionModel& 
  pointDistributionModel::copy(const pointDistributionModel& other) {
    // ioObject::copy(other);

    meanShape = other.meanShape;
    eigenVectorMatrix = other.eigenVectorMatrix;
    varianceVector = other.varianceVector;

    return *this;
  }

  // assigment operator (alias for copy(other)).
  pointDistributionModel& 
  pointDistributionModel::operator=(const pointDistributionModel& other) {
    return copy(other);
  }

  // copy constructor
  pointDistributionModel::
  pointDistributionModel(const pointDistributionModel& other)
    : ioObject() {
    copy(other);
  }

  //clone member
  pointDistributionModel* pointDistributionModel::clone() const {
    return new pointDistributionModel(*this);
  }

  //newInstance member function
  pointDistributionModel* pointDistributionModel::newInstance() const {
    return new pointDistributionModel();
  }

  // returns the name of this class
  const std::string& pointDistributionModel::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */

  bool pointDistributionModel::write(ioHandler& handler,
                                     const bool complete) const
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b = lti::write(handler, "meanShape", meanShape) && b;
      b = lti::write(handler, "eigenVectorMatrix", eigenVectorMatrix) && b;
      b = lti::write(handler, "varianceVector", varianceVector) && b;

    }

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
  bool pointDistributionModel::read(ioHandler& handler,
                                    const bool complete)
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b = lti::read(handler, "meanShape", meanShape) && b;
      b = lti::read(handler, "eigenVectorMatrix", eigenVectorMatrix) && b;
      b = lti::read(handler, "varianceVector", varianceVector) && b;

    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

}
