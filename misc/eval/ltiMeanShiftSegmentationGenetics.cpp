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
 * \file   ltiSegmentationGenetics.h
 *         Defines the abstract parent class of the genetic evaluation of
 *         segmentation algorithms.
 * \author Pablo Alvarado
 * \date   21.05.08
 *
 * $Id: ltiMeanShiftSegmentationGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiMeanShiftSegmentationGenetics.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiIOImage.h"
#include "ltiLoadImageList.h"
#include "ltiTimer.h"
#include "ltiConstants.h"
#include "ltiList.h"
#include "ltiFactory.h"
 
#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#define _LTI_SAVE_PARAM 1

#include "ltiViewer2D.h"
#include "ltiDraw.h"
#include "ltiLispStreamHandler.h"
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

#endif

namespace lti {
  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,meanShiftSegmentationGenetics)

  // -------------------------------------------------------------------------
  //   lti::genetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  meanShiftSegmentationGenetics::parameters::parameters()
    : segmentationGenetics::parameters() {

    minValues.maxTrial = 1;    
    maxValues.maxTrial = 10;

    minValues.multivariateNormalKernel = false;    
    maxValues.multivariateNormalKernel = true;

    minValues.speedup = meanShiftSegmentation::NoSpeedup;
    maxValues.speedup = meanShiftSegmentation::HighSpeedup;

    minValues.sigmaS = 1;    
    maxValues.sigmaS = 10;

    minValues.sigmaR = 1;    
    maxValues.sigmaR = 10;

    minValues.maxNeighbourColorDistance = 1;    
    maxValues.maxNeighbourColorDistance = 9;

    minValues.minRegionSize = 1;    
    maxValues.minRegionSize = 1024;

    minValues.thresholdConverged = 0.01;    
    maxValues.thresholdConverged = 0.2;
  }

  // copy constructor
  meanShiftSegmentationGenetics::parameters::parameters(const parameters& other)
    : segmentationGenetics::parameters() {
    copy(other);
  }

  // destructor
  meanShiftSegmentationGenetics::parameters::~parameters() {
  }

  // copy member

  meanShiftSegmentationGenetics::parameters&
  meanShiftSegmentationGenetics::parameters::copy(const parameters& other) {
    segmentationGenetics::parameters::copy(other);

    minValues = other.minValues;
    maxValues = other.maxValues;

    return *this;
  }

  // alias for copy method
  meanShiftSegmentationGenetics::parameters&
  meanShiftSegmentationGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& meanShiftSegmentationGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  meanShiftSegmentationGenetics::parameters*
  meanShiftSegmentationGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  meanShiftSegmentationGenetics::parameters*
  meanShiftSegmentationGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool 
  meanShiftSegmentationGenetics::parameters::write(ioHandler& handler,
                                               const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"minValues",minValues);
      lti::write(handler,"maxValues",maxValues);
    }

    b = b && segmentationGenetics::parameters::write(handler,false);

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
  bool meanShiftSegmentationGenetics::parameters::read(ioHandler& handler,
                                                   const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"minValues",minValues);
      lti::read(handler,"maxValues",maxValues);    
    }

    b = b && segmentationGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  //   lti::meanShiftSegmentationGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  meanShiftSegmentationGenetics::meanShiftSegmentationGenetics() 
    : segmentationGenetics() {

    meanShiftSegmentation segmenter;
    setSegmenter(segmenter);

    parameters defPar;
    setParameters(defPar);

  }

  /*
   * Default constructor.
   */
  meanShiftSegmentationGenetics::meanShiftSegmentationGenetics(const parameters& par) 
    : segmentationGenetics() {

    meanShiftSegmentation segmenter;
    setSegmenter(segmenter);

    setParameters(par);
  }

  /*
   * Default constructor.
   */
  meanShiftSegmentationGenetics::
  meanShiftSegmentationGenetics(const meanShiftSegmentationGenetics& other) 
    : segmentationGenetics() {

    copy(other);
  }

  /*
   * Destructor
   */
  meanShiftSegmentationGenetics::~meanShiftSegmentationGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& meanShiftSegmentationGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  meanShiftSegmentationGenetics& 
  meanShiftSegmentationGenetics::copy(const meanShiftSegmentationGenetics& other) {
    segmentationGenetics::copy(other);
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  meanShiftSegmentationGenetics& 
  meanShiftSegmentationGenetics::operator=(const meanShiftSegmentationGenetics& other){
    return copy(other);
  }
  
  meanShiftSegmentationGenetics* meanShiftSegmentationGenetics::clone() const {
    return new meanShiftSegmentationGenetics(*this);
  }

  meanShiftSegmentationGenetics* meanShiftSegmentationGenetics::newInstance() const {
    return new meanShiftSegmentationGenetics();
  }

  // return parameters
  const meanShiftSegmentationGenetics::parameters& 
  meanShiftSegmentationGenetics::getParameters() const {
    
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }


  //
  // global private enum, with the number of pixels per parameter
  // it is global to define this constants for all method that need them,
  //
  struct meanShiftSegmentationGenetics::bits {
    enum {
      MaxTrial = 5,
      MultivariateNormalKernel = 1,
      Speedup = 2,
      SigmaS = 8,
      SigmaR = 8,
      MaxNeighbourColorDistance = 8,
      MinRegionSize = 5,
      ThresholdConverged = 10
    };

    static int total() {
      return (MaxTrial                  +
              MultivariateNormalKernel  +
              Speedup                   +
              SigmaS                    +
              SigmaR                    +
              MaxNeighbourColorDistance +
              MinRegionSize             +
              ThresholdConverged);
    }
  };

  segmentation::parameters* meanShiftSegmentationGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    meanShiftSegmentation::parameters* pars = new meanShiftSegmentation::parameters;

    if (chromosomeToPhenotype(genotype,*pars)) {
      return pars;
    } 

    delete pars;
    return 0;
  }


  bool meanShiftSegmentationGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                        functor::parameters& phenotype) const {
    
    const parameters& par = getParameters();
    meanShiftSegmentation::parameters* phen = 
      dynamic_cast<meanShiftSegmentation::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      return false;
    }

    int pos=0;
    int ires;

    // maxTrial
    pos = binToInt(genotype,pos,bits::MaxTrial,
                   par.minValues.maxTrial,
                   par.maxValues.maxTrial,phen->maxTrial);
    
    // multivariateNormalKernel
    pos = binToInt(genotype,pos,bits::MultivariateNormalKernel,
                   par.minValues.multivariateNormalKernel,
                   par.maxValues.multivariateNormalKernel,ires);
   
    phen->multivariateNormalKernel = (ires != 0);
    
    // speedup
    pos = binToInt(genotype,pos,bits::Speedup,
                   par.minValues.speedup,
                   par.maxValues.speedup,
                   ires);

    switch(ires) {
    case meanShiftSegmentation::NoSpeedup:
      phen->speedup = meanShiftSegmentation::NoSpeedup;
      break;
    case meanShiftSegmentation::MediumSpeedup:
      phen->speedup = meanShiftSegmentation::MediumSpeedup;
      break;
    case meanShiftSegmentation::HighSpeedup:
      phen->speedup = meanShiftSegmentation::HighSpeedup;
      break;
    default:
      phen->speedup = meanShiftSegmentation::MediumSpeedup;
      break;
    }

    // sigmaS
    pos = binToDouble(genotype,pos,bits::SigmaS,
                      par.minValues.sigmaS,
                      par.maxValues.sigmaS,phen->sigmaS);

    // sigmaR
    pos = binToDouble(genotype,pos,bits::SigmaR,
                      par.minValues.sigmaR,
                      par.maxValues.sigmaR,phen->sigmaR);

    // maxNeighbourColorDistance
    pos = binToDouble(genotype,pos,bits::MaxNeighbourColorDistance,
                      par.minValues.maxNeighbourColorDistance,
                      par.maxValues.maxNeighbourColorDistance,
                      phen->maxNeighbourColorDistance);

    if (phen->maxNeighbourColorDistance > phen->sigmaR) {
      phen->maxNeighbourColorDistance = phen->sigmaR;
    }

    // minRegionSize
    pos = binToInt(genotype,pos,bits::MinRegionSize,
                   par.minValues.minRegionSize,
                   par.maxValues.minRegionSize,ires);

    phen->minRegionSize = sqr(ires);

    // thresholdConverged
    pos = binToDouble(genotype,pos,bits::ThresholdConverged,
                      par.minValues.thresholdConverged,
                      par.maxValues.thresholdConverged,
                      phen->thresholdConverged);

    // check in debug modus that everything is there!
    assert(pos == bits::total());

    return true;
  }

  bool meanShiftSegmentationGenetics::phenotypeToChromosome(
                                         const functor::parameters& phenotype,
                                         chromosome& genotype) const {
    
    genotype.resize(bits::total());

    const parameters& par = getParameters();
    const meanShiftSegmentation::parameters* phen =
      dynamic_cast<const meanShiftSegmentation::parameters*>(&phenotype);

    if (isNull(phen)) {
      return false;
    }

    int pos=0;
    
    // maxTrial
    pos = intToBin(phen->maxTrial,pos,bits::MaxTrial,
                   par.minValues.maxTrial,
                   par.maxValues.maxTrial,
                   genotype);

    // multivariateNormalKernel
    pos = intToBin((phen->multivariateNormalKernel) ? 1 : 0,
                   pos,bits::MultivariateNormalKernel,
                   par.minValues.multivariateNormalKernel,
                   par.maxValues.multivariateNormalKernel,
                   genotype);

    // speedup
    pos = intToBin((phen->speedup) ? 1 : 0,
                   pos,bits::Speedup,
                   par.minValues.speedup,
                   par.maxValues.speedup,
                   genotype);

    // sigmaS
    pos = doubleToBin(phen->sigmaS,
                      pos,bits::SigmaS,
                      par.minValues.sigmaS,
                      par.maxValues.sigmaS,genotype);

    // sigmaR
    pos = doubleToBin(phen->sigmaR,
                      pos,bits::SigmaR,
                      par.minValues.sigmaR,
                      par.maxValues.sigmaR,genotype);

    // maxNeighbourColorDistance
    pos = doubleToBin(phen->maxNeighbourColorDistance,
                      pos,bits::MaxNeighbourColorDistance,
                      par.minValues.maxNeighbourColorDistance,
                      par.maxValues.maxNeighbourColorDistance,genotype);

    // minRegionSize
    pos = intToBin(iround(sqrt(phen->minRegionSize)),
                   pos,bits::MinRegionSize,
                   par.minValues.minRegionSize,
                   par.maxValues.minRegionSize,
                   genotype);

    // thresholdConverged
    pos = doubleToBin(phen->thresholdConverged,
                      pos,bits::ThresholdConverged,
                      par.minValues.thresholdConverged,
                      par.maxValues.thresholdConverged,genotype);

    assert (pos == bits::total());
    
    return true;
    
  }

  int meanShiftSegmentationGenetics::getChromosomeSize() const {
    return bits::total();
  }


}
