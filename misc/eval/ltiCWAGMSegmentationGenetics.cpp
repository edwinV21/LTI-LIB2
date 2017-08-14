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
 * $Id: ltiCWAGMSegmentationGenetics.cpp,v 1.3 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiCWAGMSegmentationGenetics.h"
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
  _LTI_REGISTER_IN_FACTORY(genetics,cwagmSegmentationGenetics)

  // -------------------------------------------------------------------------
  //   lti::genetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  cwagmSegmentationGenetics::parameters::parameters()
    : segmentationGenetics::parameters() {

    //
    // Preprocessing
    //

    minValues.medianParam.kernelSize = 1;    
    maxValues.medianParam.kernelSize = 7;

    //
    // General split configuration
    //

    minValues.colorSplitter = "RGB";
    maxValues.colorSplitter = "XYZ";

    minValues.colorContrastParam.kernelType = 
      colorContrastGradient::Ando;
    maxValues.colorContrastParam.kernelType =
      colorContrastGradient::Kirsch;

    minValues.colorContrastParam.contrastType = 
      colorContrastGradient::MDD;
    maxValues.colorContrastParam.contrastType =
      colorContrastGradient::Maximum;

    //
    // Watershed configuration
    //

    minValues.watershedParam.neighborhood8 = false;
    maxValues.watershedParam.neighborhood8 = true;

    minValues.minProbForWatershedThreshold = 0.0;
    maxValues.minProbForWatershedThreshold = 1.0;

    minValues.harisRegionMergeParam.mergeThreshold = 0;
    maxValues.harisRegionMergeParam.mergeThreshold = 25;

    minValues.harisRegionMergeParam.minRegionNumber = 1;
    maxValues.harisRegionMergeParam.minRegionNumber = 25;

  }

  // copy constructor
  cwagmSegmentationGenetics::parameters::parameters(const parameters& other)
    : segmentationGenetics::parameters() {
    copy(other);
  }

  // destructor
  cwagmSegmentationGenetics::parameters::~parameters() {
  }

  // copy member

  cwagmSegmentationGenetics::parameters&
  cwagmSegmentationGenetics::parameters::copy(const parameters& other) {
    segmentationGenetics::parameters::copy(other);

    minValues = other.minValues;
    maxValues = other.maxValues;

    return *this;
  }

  // alias for copy method
  cwagmSegmentationGenetics::parameters&
  cwagmSegmentationGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& cwagmSegmentationGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  cwagmSegmentationGenetics::parameters*
  cwagmSegmentationGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  cwagmSegmentationGenetics::parameters*
  cwagmSegmentationGenetics::parameters::newInstance() const {
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
  cwagmSegmentationGenetics::parameters::write(ioHandler& handler,
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
  bool cwagmSegmentationGenetics::parameters::read(ioHandler& handler,
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
  //   lti::cwagmSegmentationGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  cwagmSegmentationGenetics::cwagmSegmentationGenetics() 
    : segmentationGenetics() {

    cwagmSegmentation segmenter;
    setSegmenter(segmenter);

    parameters defPar;
    setParameters(defPar);

  }

  /*
   * Default constructor.
   */
  cwagmSegmentationGenetics::cwagmSegmentationGenetics(const parameters& par) 
    : segmentationGenetics() {

    cwagmSegmentation segmenter;
    setSegmenter(segmenter);

    setParameters(par);
  }

  /*
   * Default constructor.
   */
  cwagmSegmentationGenetics::
  cwagmSegmentationGenetics(const cwagmSegmentationGenetics& other) 
    : segmentationGenetics() {

    copy(other);
  }

  /*
   * Destructor
   */
  cwagmSegmentationGenetics::~cwagmSegmentationGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& cwagmSegmentationGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  cwagmSegmentationGenetics& 
  cwagmSegmentationGenetics::copy(const cwagmSegmentationGenetics& other) {
    segmentationGenetics::copy(other);
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  cwagmSegmentationGenetics& 
  cwagmSegmentationGenetics::operator=(const cwagmSegmentationGenetics& other){
    return copy(other);
  }
  
  cwagmSegmentationGenetics* cwagmSegmentationGenetics::clone() const {
    return new cwagmSegmentationGenetics(*this);
  }

  cwagmSegmentationGenetics* cwagmSegmentationGenetics::newInstance() const {
    return new cwagmSegmentationGenetics();
  }

  // return parameters
  const cwagmSegmentationGenetics::parameters& 
  cwagmSegmentationGenetics::getParameters() const {
    
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
  struct cwagmSegmentationGenetics::bits {
    enum {
      MedianKernel                 = 2,
      ColorSplitter                = 3,
      GradientType                 = 3,
      ContrastFormat               = 2,  // __ 12 Bits => (MR 13.08)

      WatershedNeighborhood        = 1,
      WatershedThreshold           = 8,  // real threshold (usually 255)
      WatershedMinProbThreshold    = 8,  // flood factor
      WatershedHarisMerge          = 16, // merge threshold
      WatershedHarisMergeMode      = 1,  // fast/optimal
      WatershedHarisMinNumRegions  = 12  // __ 42 Bits => (MR 3.74)
                                         // (both groups: 54 => 2.9
    };

    static int total() {
      return (      MedianKernel                 +
                    ColorSplitter                +
                    GradientType                 +
                    ContrastFormat               +
                    
                    WatershedNeighborhood        +
                    WatershedThreshold           +
                    WatershedMinProbThreshold    +
                    WatershedHarisMerge          +
                    WatershedHarisMergeMode      +
                    WatershedHarisMinNumRegions   ); // 132 bits?
    }
  };

  segmentation::parameters* cwagmSegmentationGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    cwagmSegmentation::parameters* pars = new cwagmSegmentation::parameters;

    if (chromosomeToPhenotype(genotype,*pars)) {
      return pars;
    } 

    delete pars;
    return 0;
  }


  bool cwagmSegmentationGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                        functor::parameters& phenotype) const {
    
    const parameters& par = getParameters();
    cwagmSegmentation::parameters* phen = 
      dynamic_cast<cwagmSegmentation::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      return false;
    }

    int pos=0;
    int ires,theMin,theMax;
    double dres;

    // median kernel size
    pos = binToInt(genotype,pos,bits::MedianKernel,
                   (par.minValues.medianParam.kernelSize-1)/2,
                   (par.maxValues.medianParam.kernelSize-1)/2,ires);
    phen->medianParam.kernelSize = (ires*2+1);
    
    // color splitter
    pos = binToInt(genotype,pos,bits::ColorSplitter,0,7,ires);
   
    if (par.minValues.colorSplitter == par.maxValues.colorSplitter) {
      phen->colorSplitter = par.minValues.colorSplitter;
    } else {
      if (par.minValues.colorSplitter.find("XYZ") != std::string::npos){
        theMin=1;
      } else if (par.minValues.colorSplitter.find("xyY") != std::string::npos){
        theMin=2;
      } else if (par.minValues.colorSplitter.find("Luv") != std::string::npos){
        theMin=3;
      } else if (par.minValues.colorSplitter.find("rgI") != std::string::npos){
        theMin=4;
      } else if (par.minValues.colorSplitter.find("YUV") != std::string::npos){
        theMin=5;
      } else if (par.minValues.colorSplitter.find("YIQ") != std::string::npos){
        theMin=6;
      } else if (par.minValues.colorSplitter.find("OCP") != std::string::npos){
        theMin=7;
      } else { // (par.minValues.colorSplitter.find("RGB")!=std::string::npos)
        theMin=0; // RGB
      } 

      if (par.maxValues.colorSplitter.find("XYZ") != std::string::npos){
        theMax=1;
      } else if (par.maxValues.colorSplitter.find("xyY") != std::string::npos){
        theMax=2;
      } else if (par.maxValues.colorSplitter.find("Luv") != std::string::npos){
        theMax=3;
      } else if (par.maxValues.colorSplitter.find("rgI") != std::string::npos){
        theMax=4;
      } else if (par.maxValues.colorSplitter.find("YUV") != std::string::npos){
        theMax=5;
      } else if (par.maxValues.colorSplitter.find("YIQ") != std::string::npos){
        theMax=6;
      } else if (par.maxValues.colorSplitter.find("OCP") != std::string::npos){
        theMax=7;
      } else { // (par.maxValues.colorSplitter.find("RGB")!=std::string::npos)
        theMax=0; // RGB
      } 

      ires = max(theMin,min(theMax,ires));

      switch (ires) {
        case 0:
          phen->colorSplitter = "RGB";
          break;
        case 1:
          phen->colorSplitter = "XYZ";
          break;
        case 2:
          phen->colorSplitter = "xyY";
          break;
        case 3:
          phen->colorSplitter = "Luv";
          break;
        case 4:
          phen->colorSplitter = "rgI";
          break;
        case 5:
          phen->colorSplitter = "YUV";
          break;
        case 6:
          phen->colorSplitter = "YIQ";
          break;
        case 7:
          phen->colorSplitter = "OCP";
          break;
        default:
          phen->colorSplitter = "XYZ";
          break;
      }
    }
    
    // gradient kernel type
    pos = binToInt(genotype,pos,bits::GradientType,0,7,ires);
    if (par.minValues.colorContrastParam.kernelType == 
        par.maxValues.colorContrastParam.kernelType) {
      phen->colorContrastParam.kernelType =
        par.minValues.colorContrastParam.kernelType;
    } else {
      theMin = static_cast<int>(par.minValues.colorContrastParam.kernelType);
      theMax = static_cast<int>(par.maxValues.colorContrastParam.kernelType);
      minmax(theMin,theMax);
      ires = max(theMin,min(theMax,ires));

      phen->colorContrastParam.kernelType = 
        static_cast<gradientFunctor::eKernelType>(ires);
    }

    // gradient contrast format
    pos = binToInt(genotype,pos,bits::ContrastFormat,0,3,ires);
    if (par.minValues.colorContrastParam.contrastType == 
        par.maxValues.colorContrastParam.contrastType) {
      phen->colorContrastParam.contrastType =
        par.minValues.colorContrastParam.contrastType;
    } else {
      theMin=static_cast<int>(par.minValues.colorContrastParam.contrastType);
      theMax=static_cast<int>(par.maxValues.colorContrastParam.contrastType);
      minmax(theMin,theMax);
      ires = max(theMin,min(theMax,ires));

      phen->colorContrastParam.contrastType = 
        static_cast<colorContrastGradient::eContrastType>(ires);
    }

    // watershed configuration

    // neighborhood
    pos = binToInt(genotype,pos,bits::WatershedNeighborhood,
                   (par.minValues.watershedParam.neighborhood8) ? 1 : 0,
                   (par.maxValues.watershedParam.neighborhood8) ? 1 : 0,
                   ires);

    phen->watershedParam.neighborhood8 = (ires != 0);

    // WatershedThreshold
    pos = binToInt(genotype,pos,bits::WatershedThreshold,
                   par.minValues.watershedParam.threshold,
                   par.maxValues.watershedParam.threshold,ires);

    phen->watershedParam.threshold = static_cast<ubyte>(ires);

    // minProbForWatershedThreshold
    pos = binToDouble(genotype,pos,bits::WatershedMinProbThreshold,
                      par.minValues.minProbForWatershedThreshold,
                      par.maxValues.minProbForWatershedThreshold,dres);

    phen->minProbForWatershedThreshold = static_cast<float>(dres);

    // harisRegionMergeParam.mergeThreshold
    pos = binToDouble(genotype,pos,bits::WatershedHarisMerge,
                      par.minValues.harisRegionMergeParam.mergeThreshold,
                      par.maxValues.harisRegionMergeParam.mergeThreshold,dres);

    phen->harisRegionMergeParam.mergeThreshold = static_cast<float>(dres);

    // harisRegionMergeParam.mergeMode
    pos = binToInt(genotype,pos,bits::WatershedHarisMergeMode,
               static_cast<int>(par.minValues.harisRegionMergeParam.mergeMode),
               static_cast<int>(par.maxValues.harisRegionMergeParam.mergeMode),
                   ires);

    phen->harisRegionMergeParam.mergeMode =
      static_cast<regionGraphColorHaris::eMergeMode>(ires);

    // harisRegionMergeParam.minRegionNumber
    pos = binToDouble(genotype,pos,bits::WatershedHarisMinNumRegions,
                      par.minValues.harisRegionMergeParam.minRegionNumber,
                      par.maxValues.harisRegionMergeParam.minRegionNumber,
                      dres);

    phen->harisRegionMergeParam.minRegionNumber = iround(dres);

    // check in debug modus that everything is there!
    assert(pos == bits::total());

    return true;
  }

  bool cwagmSegmentationGenetics::phenotypeToChromosome(
                                         const functor::parameters& phenotype,
                                         chromosome& genotype) const {
    
    genotype.resize(bits::total());

    const parameters& par = getParameters();
    const cwagmSegmentation::parameters* phen =
      dynamic_cast<const cwagmSegmentation::parameters*>(&phenotype);

    if (isNull(phen)) {
      return false;
    }

    int pos=0;
    int ires;
    
    // median kernel size
    pos = intToBin((phen->medianParam.kernelSize-1)/2,pos,bits::MedianKernel,
                   (par.minValues.medianParam.kernelSize-1)/2,
                   (par.maxValues.medianParam.kernelSize-1)/2,
                   genotype);

    // color splitter
    if (phen->colorSplitter.find("XYZ") != std::string::npos) {
      ires=1;
    } else if (phen->colorSplitter.find("xyY") != std::string::npos) {
      ires=2;
    } else if (phen->colorSplitter.find("Luv") != std::string::npos) {
      ires=3;
    } else if (phen->colorSplitter.find("rgI") != std::string::npos) {
      ires=4;
    } else if (phen->colorSplitter.find("YUV") != std::string::npos) {
      ires=5;
    } else if (phen->colorSplitter.find("YIQ") != std::string::npos) {
      ires=6;
    } else if (phen->colorSplitter.find("OCP") != std::string::npos) {
      ires=7;
    } else { // (phen->colorSplitter.find("RGB")!=std::string::npos)
      ires=0; // RGB
    } 
    
    pos = intToBin(ires,pos,bits::ColorSplitter,0,7,genotype);

    // gradient kernel type
    ires = static_cast<int>(phen->colorContrastParam.kernelType);
    pos = intToBin(ires,pos,bits::GradientType,0,7,genotype);

    // gradient contrast format
    ires = static_cast<int>(phen->colorContrastParam.contrastType);
    pos = intToBin(ires,pos,bits::ContrastFormat,0,3,genotype);

    // watershed
    
    // neighborhood
    pos = intToBin((phen->watershedParam.neighborhood8) ? 1 : 0,
                   pos,bits::WatershedNeighborhood,
                   (par.minValues.watershedParam.neighborhood8) ? 1 : 0,
                   (par.maxValues.watershedParam.neighborhood8) ? 1 : 0,
                   genotype);

    // WatershedThreshold
    pos = intToBin(phen->watershedParam.threshold,
                   pos,bits::WatershedThreshold,
                   par.minValues.watershedParam.threshold,
                   par.maxValues.watershedParam.threshold,genotype);

    // minProbForWatershedThreshold
    pos = doubleToBin(phen->minProbForWatershedThreshold,
                      pos,bits::WatershedMinProbThreshold,
                      par.minValues.minProbForWatershedThreshold,
                      par.maxValues.minProbForWatershedThreshold,genotype);

    // harisRegionMergeParam.mergeThreshold
    pos = doubleToBin(phen->harisRegionMergeParam.mergeThreshold,
                      pos,bits::WatershedHarisMerge,
                      par.minValues.harisRegionMergeParam.mergeThreshold,
                      par.maxValues.harisRegionMergeParam.mergeThreshold,
                      genotype);

    // neighborhood
    pos = intToBin(static_cast<int>(phen->harisRegionMergeParam.mergeMode),
                   pos,bits::WatershedHarisMergeMode,
               static_cast<int>(par.minValues.harisRegionMergeParam.mergeMode),
               static_cast<int>(par.maxValues.harisRegionMergeParam.mergeMode),
                   genotype);

    // harisRegionMergeParam.minRegionNumber
    pos = doubleToBin(double(phen->harisRegionMergeParam.minRegionNumber),
                      pos,bits::WatershedHarisMinNumRegions,
                      par.minValues.harisRegionMergeParam.minRegionNumber,
                      par.maxValues.harisRegionMergeParam.minRegionNumber,
                      genotype);

    assert (pos == bits::total());
    
    return true;
    
  }

  int cwagmSegmentationGenetics::getChromosomeSize() const {
    return bits::total();
  }


}
