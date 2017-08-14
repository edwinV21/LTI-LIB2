/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2008
 * Pablo Alvarado
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

/*
 * \file   ltiSVD_template.h
 *         Singular Value Decomposition
 * \author Xin Gu
 * \author Pablo Alvarado
 * \date   11.01.2001
 *
 * revisions ..: $Id: ltiSVD.cpp,v 1.1 2008-05-07 15:18:35 alvarado Exp $
 */


#include "ltiMacroSymbols.h"

#include "ltiSVD.h"
#include "ltiSVD_template.h"

namespace lti {
  // explicit instantiations
  // --------------------------------------------------
  // svd::parameters
  // --------------------------------------------------

  /*
   * default constructor
   */
  svd::parameters::parameters() : linearAlgebraFunctor::parameters() {
    sort = false;
    transposeU = false;
    transposeV = false;

    useDivideAndConquer = true;
    useMinDimensions = true;
  }

  /*
   * copy constructor
   * @param other the parameters object to be copied
   */
  svd::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }
  
  /*
   * destructor
   */
  svd::parameters::~parameters() {
  }
  
  /*
   * returns name of this type
   */
  const std::string& svd::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  svd::parameters& svd::parameters::copy(const parameters& other)  {
    linearAlgebraFunctor::parameters::copy(other);
    
    sort=other.sort;
    transposeU = other.transposeU;
    transposeV = other.transposeV;
    
    useDivideAndConquer = other.useDivideAndConquer;
    useMinDimensions = other.useMinDimensions;

    return *this;
  }
  
  /*
   * returns a pointer to a clone of the parameters
   */
  svd::parameters* svd::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * returns a pointer to a clone of the parameters
   */
  svd::parameters* svd::parameters::newInstance() const {
    return new parameters();
  }

  bool svd::parameters::read(ioHandler &handler, const bool complete) {
    bool b=true;
    
    if (complete) {
      b=handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"sort",sort);
      lti::read(handler,"transposeU",transposeU);
      lti::read(handler,"transposeV",transposeV);

      lti::read(handler,"useDivideAndConquer",useDivideAndConquer);
      lti::read(handler,"useMinDimensions",useMinDimensions);

    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);

    if (complete) {
      b=b && handler.readEnd();
    }

    return b;
  }

  bool svd::parameters::write(ioHandler& handler,const bool complete) const {
    bool b=true;
    
    if (complete) {
      b=handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"sort",sort);
      lti::write(handler,"transposeU",transposeU);
      lti::write(handler,"transposeV",transposeV);

      lti::write(handler,"useDivideAndConquer",useDivideAndConquer);
      lti::write(handler,"useMinDimensions",useMinDimensions);
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);

    if (complete) {
      b=b && handler.writeEnd();
    }
    
    return b;
  }
  

  // --------------------------------------------------
  // svd
  // --------------------------------------------------
  svd::svd() {
    parameters defaultParam;
    setParameters(defaultParam);
  }

  svd::svd(const parameters& par) {
    setParameters(par);
  }

  svd::svd(bool sort) {
    parameters defaultParam;
    defaultParam.sort=sort;
    setParameters(defaultParam);
  }

  /*
   * copy constructor
   */
  svd::svd(const svd& other) : linearAlgebraFunctor() {
    copy(other);
  }

  /*
   * destructor
   */
  svd::~svd() {
  }


  // returns the name of this type
  const std::string& svd::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  svd& svd::copy(const svd& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  // clone member
  svd* svd::clone() const {
    return new svd(*this);
  }

  svd* svd::newInstance() const {
    return new svd();
  }

  bool svd::decomposition(dmatrix& src,dvector& w,dmatrix& v) const {
    helper<double> h(getParameters());
    return h.decomposition(src,w,v);
  }

  bool svd::decomposition(fmatrix& src,fvector& w,fmatrix& v) const {
    helper<float> h(getParameters());
    return h.decomposition(src,w,v);
  }

  bool svd::apply(dmatrix& src,dvector& w,dmatrix& v) const {
    helper<double> h(getParameters());
    return h.decomposition(src,w,v);
  }

  bool svd::apply(fmatrix& src,fvector& w,fmatrix& v) const {
    helper<float> h(getParameters());
    return h.decomposition(src,w,v);
  }

  bool svd::apply(const dmatrix& src, 
                  dmatrix& u,
                  dvector& w, 
                  dmatrix& v) const {
    helper<double> h(getParameters());
    return h.decomposition(src,u,w,v);
  }

  bool svd::apply(const fmatrix& src, 
                  fmatrix& u,
                  fvector& w, 
                  fmatrix& v) const {
    helper<float> h(getParameters());
    return h.decomposition(src,u,w,v);
  }

  const svd::parameters& svd::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if (isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }
}

