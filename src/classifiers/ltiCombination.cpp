/*
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

/** 
 * \file ltiCombination.cpp
 * Contains the class lti::combination, which
 * allows to combine classification results from lti::classifier instances.
 * \author Pablo Alvarado
 * \date 05.10.2008
 *
 * revisions ..: $Id: ltiCombination.cpp,v 1.1 2008-10-07 00:11:28 alvarado Exp $
 */


#include "ltiCombination.h"

namespace lti {
  // --------------------------------------------------
  // combination::parameters
  // --------------------------------------------------

  // default constructor
  combination::parameters::parameters() : functor::parameters() {

    strategy = Linear;
    normalize           = true;
    normalizeInputs     = false;
    useWeights          = false;
    weights             = dvector(0);
  }

  // copy constructor
  combination::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  combination::parameters::~parameters() {
  }

  // get type name
  const std::string& combination::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  combination::parameters&
  combination::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    strategy  = other.strategy;
    normalize            = other.normalize;
    normalizeInputs      = other.normalizeInputs;
    useWeights           = other.useWeights;
    weights              = other.weights;

    return *this;
  }

  // alias for copy member
  combination::parameters&
    combination::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  combination::parameters* combination::parameters::clone() const {
    return new parameters(*this);
  }

  combination::parameters* combination::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool combination::parameters::write(ioHandler& handler,
                                      const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"strategy",strategy);
      lti::write(handler,"normalize",normalize);
      lti::write(handler,"normalizeInputs",normalizeInputs);
      lti::write(handler,"useWeights",useWeights);

      if (useWeights) {
        lti::write(handler,"weights",weights);
      }
    }

    b = b && functor::parameters::write(handler,false);

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
  bool combination::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"strategy",strategy);

      lti::read(handler,"normalize",normalize);
      lti::read(handler,"normalizeInputs",normalizeInputs);
      lti::read(handler,"useWeights",useWeights);

      if (useWeights) {
        lti::read(handler,"weights",weights);
      }

    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // combination
  // --------------------------------------------------

  // default constructor
  combination::combination()
    : functor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // copy constructor
  combination::combination(const combination& other)
    : functor()  {
    copy(other);
  }

  // constructor
  combination::combination(const parameters& params)
    : functor()  {
    setParameters(params);
  }

  // destructor
  combination::~combination() {
  }

  // returns the name of this type
  const std::string& combination::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  combination& combination::copy(const combination& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  combination& combination::operator=(const combination& other) {
    return (copy(other));
  }


  // clone member
  combination* combination::clone() const {
    return new combination(*this);
  }

  // clone member
  combination* combination::newInstance() const {
    return new combination;
  }

  // return parameters
  const combination::parameters&
    combination::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  bool combination::apply(const std::list<classifier::result>& src,
                          classifier::result& dest) const {

    bool b=true;

    // If src is empty there is nothing to combine
    if (src.empty()) {
      dest=classifier::result();
      setStatusString("Empty input-list in combination");
      return false;
    }

    // To use weighted combination call other apply method
    // If useWeights is true but weights is empty or 0 return false but
    // continue with processing as if useWeights was false
    const parameters& param = getParameters();
    if (param.useWeights) {
      if (!param.weights.empty()) {
        return apply(src, param.weights, dest);
      } else {
        dest=classifier::result();
        setStatusString
          ("Combination: Empty weights vector although useWeights is true");
        b=false;
      }
    }


    const std::list<classifier::result>* inputs=0;
    std::list<classifier::result> tlist;
    if (param.normalizeInputs) {
      classifier::result tvec;
      std::list<classifier::result>::const_iterator it=src.begin();
      while (it!=src.end()) {
        tvec=*it++;
        tvec.makeProbDistribution();
        tlist.push_back(tvec);
      }
      inputs=&tlist;
    } else {
      inputs=&src;
    }

    const eStrategy mode = param.strategy;

    switch(mode) {

    case Linear:
      b = sumOfVectors(*inputs, dest) && b;
      break;

    case ProbabilisticBordaCount:
      b = probBordaCount(*inputs, dest) && b;
      break;

    default:
      b = sumOfVectors(*inputs, dest) && b;
    }

    if (param.normalize) {
      dest.makeProbDistribution();
    }

    return b;
  }

  bool combination::apply(const std::list<classifier::result>& src,
                          const dvector& weights,
                          classifier::result& dest) const {

    bool b=true;

    // If src is empty there is nothing to combine
    if (src.empty()) {
      dest=classifier::result();
      setStatusString("Empty input-list in combination");
      return false;
    }

    const parameters& param = getParameters();
    const eStrategy mode = param.strategy;

    switch(mode) {

    case Linear:
      b = sumOfVectors(src, weights, dest) && b;
      break;

    case ProbabilisticBordaCount:
      b = probBordaCount(src, weights, dest) && b;
      break;

    default:
      b = sumOfVectors(src, weights, dest) && b;
    }

    if (param.normalize) {
      dest.makeProbDistribution();
    }

    return b;
  }

  // -------------------------------------------------------------------
  // The combination-methods!
  // -------------------------------------------------------------------


  bool combination::sumOfVectors(const std::list<classifier::result> src,
                                 classifier::result& dest) const {

#ifdef _DEBUG
    // Checks whether it is possible to combine the vectors in src.
    // This is quite time consuming and thus only executed when in debug mode
    if (!noMultAndCompatible(src)) {
      return false;
    }
#endif

    std::list<classifier::result>::const_iterator it;

    it=src.begin();
    dest=*it;
    ++it;

    while (it!=src.end()) {
      dest.add(*it);
      ++it;
    }

    return true;

  }

  bool combination::sumOfVectors(const std::list<classifier::result> src,
                                 const dvector& weights,
                                 classifier::result& dest) const {

#ifdef _DEBUG
    // Checks whether it is possible to combine the vectors in src.
    // This is quite time consuming and thus only executed when in debug mode
    if (!noMultAndCompatible(src)) {
      dest=classifier::result();
      return false;
    }
    if (src.size()!=(unsigned int)weights.size()) {
      setStatusString("Combination: Unequal number of vectors and weights");
      dest=classifier::result();
      return false;
    }
#endif

    std::list<classifier::result>::const_iterator it;

    it=src.begin();
    dest=*it;
    ++it;
    dest.mul(weights.at(0));
    int i=1;
    while (it!=src.end()) {
      dest.addScaled(*it, weights.at(i));
      ++it;
      ++i;
    }

    return true;
  }


  bool
  combination::probBordaCount(const std::list<classifier::result> src,
                              classifier::result& dest) const {

#ifdef _DEBUG
    // Checks whether it is possible to combine the vectors in src.
    // This is quite time consuming and thus only executed when in debug mode
    if (!noMultAndCompatible(src)) {
      dest=classifier::result();
      return false;
    }
#endif

    int i;
    std::list<classifier::result>::const_iterator it;
    it=src.begin();

    dest=classifier::result(*it);
    dest.setValues(dvector(it->size()));
    it++;
    while (it!=src.end()) {

      int    curr;
      double currPbc;
      double currP;
      double resP;
      double prevPbc=0.;
      double prevP;
      it->getValueByPosition(0, prevP);
      double d=0.f;
      for (i=1; i<it->size(); i++, d++) {
        it->getPair(i, curr, currP);
        dest.getValueById(curr, resP);
        currPbc=d*(currP-prevP)+prevPbc;
        dest.setValueByPosition(curr, resP + currPbc);
        prevPbc=currPbc;
        prevP=currP;
      }
      it++;
    }
    return true;
  }

  bool
  combination::probBordaCount(const std::list<classifier::result> src,
                              const dvector& weights,
                              classifier::result& dest) const {

#ifdef _DEBUG
    // Checks whether it is possible to combine the vectors in src.
    // This is quite time consuming and thus only executed when in debug mode
    if (!noMultAndCompatible(src)) {
      dest=classifier::result();
      return false;
    }
    if (src.size()!=(unsigned int)weights.size()) {
      setStatusString("Combination: Unequal number of vectors and weights");
      dest=classifier::result();
      return false;
    }
#endif

    int i, count;
    std::list<classifier::result>::const_iterator it;
    it=src.begin();

    dest=classifier::result(*it);
    dest.setValues(dvector(it->size()));

    it++;
    count=0;
    while (it!=src.end()) {
      int    curr;
      double currPbc;
      double currP;
      double resP;
      double prevPbc=0.;
      double prevP;
      it->getValueByPosition(0,prevP);
      double d=0.f;
      for (i=1; i<it->size(); i++, d++) {
        it->getPair(i, curr, currP);
        dest.getValueById(curr, resP);
        currPbc=d*(currP-prevP)+prevPbc;
        dest.setValueByPosition(curr, resP + weights[count++]*currPbc);
        prevPbc=currPbc;
        prevP=currP;
      }
    }
    return true;
  }


  // -------------------------------------------------------------------
  // tools!
  // -------------------------------------------------------------------

  bool combination::
  noMultAndCompatible(const std::list<classifier::result> src) const {

    std::list<classifier::result>::const_iterator it;
    it=src.begin();
    classifier::result first=*it++;
    if (!first.noMultipleIds()) {
      return false;
    }
    for (; it!=src.end(); it++) {
      if (!it->noMultipleIds()) {
        setStatusString("Combination: One vector in the list contained " \
                        "a label more than once.");
        return false;
      }
      if (!it->isCompatible(first)) {
        setStatusString("Combination: Two vectors of the list are not " \
                        "compatible");
        return false;
      }
    }
    return true;
  }


  /**
   * Write combination::eStrategy
   */
  bool write(ioHandler& handler,const combination::eStrategy val) {
    switch(val) {
    case combination::Linear:
      return handler.write("Linear");
      break;
    case combination::ProbabilisticBordaCount:
      return handler.write("ProbabilisticBordaCount");
      break;
    default:
      handler.write("Unknown");
      handler.setStatusString("Unknown combination::eStrategy value");
    }
    return false;
  }

  /**
   * Read combination::eStrategy
   */
  bool read(ioHandler& handler,combination::eStrategy& val) {
    std::string tmp;
    if (handler.read(tmp)) {
      if (tmp.find("Linear") != std::string::npos) {
        val = combination::Linear;
      } else if (tmp.find("Borda") != std::string::npos) {
        val = combination::ProbabilisticBordaCount;
      } else {
        val = combination::Linear;
        return false;
      }
      return true;
    }
    return false;
  }


}
