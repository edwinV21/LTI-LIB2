/*
 * Copyright (C) 2006
 * Pablo Alvarado, ITCR
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
 * \file   ltiFastHessianDetectionGenetics.cpp
 *         Defines the evaluation class for lti::fastHessianDetection 
 *         algorithm.
 * \author Pablo Alvarado
 * \date   04.12.07
 *
 * $Id: ltiFastHessianDetectionGenetics.cpp,v 1.3 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiFastHessianDetectionGenetics.h"
#include "ltiFastHessianDetection.h"
#include "ltiConstants.h"

#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,fastHessianDetectionGenetics)

  // -------------------------------------------------------------------------
  //   lti::fastHessianDetectionGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  fastHessianDetectionGenetics::parameters::parameters()
    : locationDetectionGenetics::parameters() {

    numberOfLevels.set(3,34);
    initialKernelSize.set(3,45);
    levelSelectionMethod.set(fastHessianDetection::Blocks,
                             fastHessianDetection::Exponential);
    initialKernelStep.set(6,48);
    levelGroupSize.set(2,17);
    normPower.set(3.5f,4.5f);
    extrema.set(fastHessianDetection::Minima,fastHessianDetection::Both);
    locationSelectionMode.set(fastHessianDetection::All,
                              fastHessianDetection::Number);
    threshold.set(0.0,1.0);
    subsampleLevels.set(false,true);
    initialSamplingStep.set(1,32);
    computeOrientation.set(false,true);
    orientationNeighborhoodFactor.set(1,32);
    orientationSamplingStepFactor.set(1,16);
    orientationWaveletSizeFactor.set(1,16);
    orientationGaussianFactor.set(0.1f,10.0f);
    orientationWindowWidth.set(360.0f/256.0f,360.0f);
    orientationNumberOfWindows.set(1,64);
  }

  // copy constructor
  fastHessianDetectionGenetics::parameters::parameters(const parameters& other)
    : locationDetectionGenetics::parameters() {
    copy(other);
  }

  // destructor
  fastHessianDetectionGenetics::parameters::~parameters() {
  }

  // copy member

  fastHessianDetectionGenetics::parameters&
  fastHessianDetectionGenetics::parameters::copy(const parameters& other) {
    locationDetectionGenetics::parameters::copy(other);


    numberOfLevels.copy(other.numberOfLevels);
    initialKernelSize.copy(other.initialKernelSize);
    levelSelectionMethod.copy(other.levelSelectionMethod);
    initialKernelStep.copy(other.initialKernelStep);
    levelGroupSize.copy(other.levelGroupSize);
    normPower.copy(other.normPower);
    extrema.copy(other.extrema);
    locationSelectionMode.copy(other.locationSelectionMode);
    threshold.copy(other.threshold);
    subsampleLevels.copy(other.subsampleLevels);
    initialSamplingStep.copy(other.initialSamplingStep);
    computeOrientation.copy(other.computeOrientation);

    orientationNeighborhoodFactor.copy(other.orientationNeighborhoodFactor);
    orientationSamplingStepFactor.copy(other.orientationSamplingStepFactor);
    orientationWaveletSizeFactor.copy(other.orientationWaveletSizeFactor);
    orientationGaussianFactor.copy(other.orientationGaussianFactor);
    orientationWindowWidth.copy(other.orientationWindowWidth);
    orientationNumberOfWindows.copy(other.orientationNumberOfWindows);

    return *this;
  }

  // alias for copy method
  fastHessianDetectionGenetics::parameters&
  fastHessianDetectionGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& fastHessianDetectionGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  fastHessianDetectionGenetics::parameters*
  fastHessianDetectionGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  fastHessianDetectionGenetics::parameters*
  fastHessianDetectionGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool fastHessianDetectionGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"numberOfLevels",numberOfLevels);
    b = b && lti::write(handler,"initialKernelSize",initialKernelSize);
    b = b && lti::write(handler,"levelSelectionMethod",levelSelectionMethod);
    b = b && lti::write(handler,"initialKernelStep",initialKernelStep);
    b = b && lti::write(handler,"levelGroupSize",levelGroupSize);
    b = b && lti::write(handler,"normPower",normPower);
    b = b && lti::write(handler,"extrema",extrema);
    b = b && lti::write(handler,"locationSelectionMode",locationSelectionMode);
    b = b && lti::write(handler,"threshold",threshold);
    b = b && lti::write(handler,"subsampleLevels",subsampleLevels);
    b = b && lti::write(handler,"initialSamplingStep",initialSamplingStep);
    b = b && lti::write(handler,"computeOrientation",computeOrientation);
    b = b && lti::write(handler,"orientationNeighborhoodFactor",
                        orientationNeighborhoodFactor);
    b = b && lti::write(handler,"orientationSamplingStepFactor",
                        orientationSamplingStepFactor);
    b = b && lti::write(handler,"orientationWaveletSizeFactor",
                        orientationWaveletSizeFactor);
    b = b && lti::write(handler,"orientationGaussianFactor",
                        orientationGaussianFactor);
    b = b && lti::write(handler,"orientationWindowWidth",
                        orientationWindowWidth);
    b = b && lti::write(handler,"orientationNumberOfWindows",
                        orientationNumberOfWindows);

    b = b && locationDetectionGenetics::parameters::write(handler,false);

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
  bool fastHessianDetectionGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"numberOfLevels",numberOfLevels);
    b = b && lti::read(handler,"initialKernelSize",initialKernelSize);
    b = b && lti::read(handler,"levelSelectionMethod",levelSelectionMethod);
    b = b && lti::read(handler,"initialKernelStep",initialKernelStep);
    b = b && lti::read(handler,"levelGroupSize",levelGroupSize);
    b = b && lti::read(handler,"normPower",normPower);
    b = b && lti::read(handler,"extrema",extrema);
    b = b && lti::read(handler,"locationSelectionMode",locationSelectionMode);
    b = b && lti::read(handler,"threshold",threshold);
    b = b && lti::read(handler,"subsampleLevels",subsampleLevels);
    b = b && lti::read(handler,"initialSamplingStep",initialSamplingStep);
    b = b && lti::read(handler,"computeOrientation",computeOrientation);
    b = b && lti::read(handler,"orientationNeighborhoodFactor",
                        orientationNeighborhoodFactor);
    b = b && lti::read(handler,"orientationSamplingStepFactor",
                        orientationSamplingStepFactor);
    b = b && lti::read(handler,"orientationWaveletSizeFactor",
                        orientationWaveletSizeFactor);
    b = b && lti::read(handler,"orientationGaussianFactor",
                        orientationGaussianFactor);
    b = b && lti::read(handler,"orientationWindowWidth",
                        orientationWindowWidth);
    b = b && lti::read(handler,"orientationNumberOfWindows",
                        orientationNumberOfWindows);

    b = b && locationDetectionGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // -------------------------------------------------------------------------
  //   lti::locationDetectionGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  fastHessianDetectionGenetics::fastHessianDetectionGenetics() 
    : locationDetectionGenetics() {
    
    // set the type of denoiser to be used
    fastHessianDetection fhd;
    setDetector(fhd);

    parameters defaultParams;
    setParameters(defaultParams);
  }

  fastHessianDetectionGenetics::
  fastHessianDetectionGenetics(const fastHessianDetectionGenetics::parameters&
                               par) 
    : locationDetectionGenetics() {
    
    // set the type of denoiser to be used
    fastHessianDetection fhd;
    setDetector(fhd);

    setParameters(par);
  }

  fastHessianDetectionGenetics::
  fastHessianDetectionGenetics(const fastHessianDetectionGenetics& other) 
    : locationDetectionGenetics(other) {
  }

  /*
   * Destructor
   */
  fastHessianDetectionGenetics::~fastHessianDetectionGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& fastHessianDetectionGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  fastHessianDetectionGenetics* fastHessianDetectionGenetics::clone() const {
    return new fastHessianDetectionGenetics(*this);
  }

  /*
   * New instance method.
   */
  fastHessianDetectionGenetics* 
  fastHessianDetectionGenetics::newInstance() const {
    return new fastHessianDetectionGenetics();
  }

  // return parameters
  const fastHessianDetectionGenetics::parameters& 
  fastHessianDetectionGenetics::getParameters() const {

    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }


  /*
   * Convert a binary-chain representation of a chromosome to a valid
   * parameter object.
   *
   * There are some tools to convert standard types into some binary chains,
   * which can be used by all derived classes:
   * - binToInt()
   * - binToUInt()
   * - binToDouble()
   */
  bool fastHessianDetectionGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    fastHessianDetection::parameters* phen = 
      dynamic_cast<fastHessianDetection::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }
    
    const parameters& par = getParameters();

    // variables to hold the decoded values
    uint32 ures;
    double dres;

    // number of levels
    pos = binToUInt(genotype,pos,int(BitsForNumberOfLevels),
                    par.numberOfLevels.from,
                    par.numberOfLevels.to,
                    ures);

    phen->numberOfLevels = static_cast<int>(ures);

    // Initial kernel size
    pos = binToUInt(genotype,pos,BitsForInitialKernelSize,
                    (par.initialKernelSize.from-3)/6,
                    (par.initialKernelSize.to-3)/6,
                    ures);

    phen->initialKernelSize = 3+(ures*6);

    // Level selection method
    pos = binToUInt(genotype,pos,BitsForLevelSelectionMethod,
                    (par.levelSelectionMethod.from == 
                     fastHessianDetection::Blocks) ? 0 : 1,
                    (par.levelSelectionMethod.to   == 
                     fastHessianDetection::Blocks) ? 0 : 1,
                    ures);

    phen->levelSelectionMethod = (ures == 0) ? 
      fastHessianDetection::Blocks :
      fastHessianDetection::Exponential;
    
    // Initial Kernel Step
    pos = binToUInt(genotype,pos,BitsForInitialKernelStep,
                    par.initialKernelStep.from/6,
                    par.initialKernelStep.to/6,
                    ures);

    phen->initialKernelStep = 6*ures;

    // Level Group Size
    pos = binToUInt(genotype,pos,BitsForLevelGroupSize,
                    par.levelGroupSize.from,
                    par.levelGroupSize.to,
                    ures);
    
    phen->levelGroupSize = ures;

    // Norm Power
    pos = binToDouble(genotype,pos,BitsForNormPower,
                      par.normPower.from,
                      par.normPower.to,
                      dres);

    phen->normPower = static_cast<float>(dres);

    // Extrema
    pos = binToUInt(genotype,pos,BitsForExtrema,
                    static_cast<uint32>(par.extrema.from),
                    static_cast<uint32>(par.extrema.to),
                    ures);

    phen->extrema = 
      static_cast<fastHessianDetection::eExtremaType>(ures);

     // Location selection mode
    pos = binToUInt(genotype,pos,BitsForLocationSelectionMode,
                    static_cast<uint32>(par.locationSelectionMode.from),
                    static_cast<uint32>(par.locationSelectionMode.to),
                    ures);

    phen->locationSelectionMode = 
      static_cast<fastHessianDetection::eLocationSelectionMode>(ures);

    double scale = 1.0;

    switch(phen->locationSelectionMode) {
      case fastHessianDetection::Absolute:
      case fastHessianDetection::Number:
        scale = 1000.0;
        break;
      case fastHessianDetection::Conspicuous:
        scale = 4.0;
        break;
      default:
        scale = 1.0;
    }
    
    // Threshold
    pos = binToDouble(genotype,pos,BitsForThreshold,
                      static_cast<double>(par.threshold.from),
                      static_cast<double>(par.threshold.to),
                      dres);

    phen->threshold = static_cast<float>(dres*scale);

    // Subsample Levels
    pos = binToUInt(genotype,pos,BitsForSubsampleLevels,
                    par.subsampleLevels.from ? 1 : 0,
                    par.subsampleLevels.to   ? 1 : 0,
                    ures);

    phen->subsampleLevels = (ures == 0) ? false : true;

    // Initial Sampling Step
    pos = binToUInt(genotype,pos,BitsForInitialSamplingStep,
                    static_cast<uint32>(par.initialSamplingStep.from),
                    static_cast<uint32>(par.initialSamplingStep.to),
                    ures);

    phen->initialSamplingStep = static_cast<int>(ures);

    // Compute Orientation
    pos = binToUInt(genotype,pos,BitsForComputeOrientation,
                    par.computeOrientation.from ? 1 : 0,
                    par.computeOrientation.to   ? 1 : 0,
                    ures);

    phen->computeOrientation = (ures == 0) ? false : true;

    // Neighborhood Factor
    pos=binToUInt(genotype,pos,BitsForOrientationNeighborhoodFactor,
                  static_cast<uint32>(par.orientationNeighborhoodFactor.from),
                  static_cast<uint32>(par.orientationNeighborhoodFactor.to),
                  ures);

    phen->orientationNeighborhoodFactor = static_cast<int>(ures);

    // Orientation Sampling Step Factor
    pos=binToUInt(genotype,pos,BitsForOrientationSamplingStepFactor,
                  static_cast<uint32>(par.orientationSamplingStepFactor.from),
                  static_cast<uint32>(par.orientationSamplingStepFactor.to),
                  ures);

    phen->orientationSamplingStepFactor = static_cast<int>(ures);

    // Orientation: Wavelet Size Factor
    pos = binToUInt(genotype,pos,BitsForOrientationWaveletSizeFactor,
                    static_cast<uint32>(par.orientationWaveletSizeFactor.from),
                    static_cast<uint32>(par.orientationWaveletSizeFactor.to),
                    ures);

    phen->orientationWaveletSizeFactor = static_cast<int>(ures);

    // Orientation: Gaussian Factor
    pos = binToDouble(genotype,pos,BitsForOrientationGaussianFactor,
                      static_cast<double>(par.orientationGaussianFactor.from),
                      static_cast<double>(par.orientationGaussianFactor.to),
                      dres);

    phen->orientationGaussianFactor = static_cast<float>(dres);

    // Orientation: Window Width
    pos = binToUInt(genotype,pos,BitsForOrientationWindowWidth,
                    static_cast<uint32>(360.0f/
                                        par.orientationWindowWidth.from),
                    static_cast<uint32>(360.0f/
                                        par.orientationWindowWidth.to),
                    ures);

    phen->orientationWindowWidth = static_cast<float>(360.0f/max(1U,ures));
    if (phen->orientationWindowWidth < constants<float>::twoPi()) {
      // ups, we have a value that will be considered in radiants.  Fix it!
      phen->orientationWindowWidth = degToRad(phen->orientationWindowWidth);
    }

    // Orientation: Number Of Windows
    pos = binToUInt(genotype,pos,BitsForOrientationNumberOfWindows,
                    static_cast<uint32>(par.orientationNumberOfWindows.from),
                    static_cast<uint32>(par.orientationNumberOfWindows.to),
                    ures);

    phen->orientationNumberOfWindows = static_cast<int>(ures);

    return true;
  }

  /*
   * Return a fresh allocated parameters for the evaluated functor, which is
   * equivalent to the given genotype.
   *
   * There are some tools to convert binary chains into standard types:
   * - intToBin()
   * - uintToBin()
   * - doubleToBin()
     */
  fastHessianDetection::parameters* fastHessianDetectionGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    fastHessianDetection::parameters* gdp =
      new fastHessianDetection::parameters;
    if (chromosomeToPhenotype(genotype,*gdp)) {
      return gdp;
    }
    delete gdp;
    return 0;
  }

  /*
   * Convert a valid parameters object (phenotype) into binary-chain
   * representation of a chromosome.
   */
  bool fastHessianDetectionGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    const fastHessianDetection::parameters* phen = 
      dynamic_cast<const fastHessianDetection::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    const parameters& par = getParameters();


    // variables to hold the decoded values
    uint32 ures;
    double dres;
    int pos = 0;


    // number of levels
    ures = static_cast<uint32>(phen->numberOfLevels);
    pos = uintToBin(ures,pos,BitsForNumberOfLevels,
                    par.numberOfLevels.from,
                    par.numberOfLevels.to,
                    genotype);


    // Initial kernel size
    ures = static_cast<uint32>((phen->initialKernelSize-3)/6);
    pos = uintToBin(ures,pos,BitsForInitialKernelSize,
                    (par.initialKernelSize.from-3)/6,
                    (par.initialKernelSize.to-3)/6,
                    genotype);


    // Level selection method
    ures = (phen->levelSelectionMethod ==
            fastHessianDetection::Blocks) ? 0 : 1;
    pos = uintToBin(ures,pos,BitsForLevelSelectionMethod,
                    (par.levelSelectionMethod.from == 
                     fastHessianDetection::Blocks) ? 0 : 1,
                    (par.levelSelectionMethod.to   == 
                     fastHessianDetection::Blocks) ? 0 : 1,
                    genotype);

    
    // Initial Kernel Step
    ures = static_cast<uint32>(phen->initialKernelStep/6);
    pos = uintToBin(ures,pos,BitsForInitialKernelStep,
                    par.initialKernelStep.from/6,
                    par.initialKernelStep.to/6,
                    genotype);


    // Level Group Size
    ures = phen->levelGroupSize;
    pos = uintToBin(ures,pos,BitsForLevelGroupSize,
                    par.levelGroupSize.from,
                    par.levelGroupSize.to,
                    genotype);
    

    // Norm Power
    dres = phen->normPower;
    pos = doubleToBin(dres,pos,BitsForNormPower,
                      par.normPower.from,
                      par.normPower.to,
                      genotype);


    // Extrema
    ures = static_cast<uint32>(phen->extrema);
    pos = uintToBin(ures,pos,BitsForExtrema,
                    static_cast<uint32>(par.extrema.from),
                    static_cast<uint32>(par.extrema.to),
                    genotype);

     // Location selection mode
    ures = static_cast<uint32>(phen->locationSelectionMode);
    pos = uintToBin(ures,pos,BitsForLocationSelectionMode,
                    static_cast<uint32>(par.locationSelectionMode.from),
                    static_cast<uint32>(par.locationSelectionMode.to),
                    genotype);

    double scale = 1.0;

    switch(phen->locationSelectionMode) {
      case fastHessianDetection::Absolute:
      case fastHessianDetection::Number:
        scale = 1000.0;
        break;
      case fastHessianDetection::Conspicuous:
        scale = 4.0;
        break;
      default:
        scale = 1.0;
    }
    
    // Threshold
    dres = static_cast<double>(phen->threshold)/scale;
    pos = doubleToBin(dres,pos,BitsForThreshold,
                      static_cast<double>(par.threshold.from),
                      static_cast<double>(par.threshold.to),
                      genotype);


    // Subsample Levels
    ures = (phen->subsampleLevels) ? 1 : 0;
    pos = uintToBin(ures,pos,BitsForSubsampleLevels,
                    par.subsampleLevels.from ? 1 : 0,
                    par.subsampleLevels.to   ? 1 : 0,
                    genotype);


    // Initial Sampling Step
    ures = static_cast<uint32>(phen->initialSamplingStep);
    pos = uintToBin(ures,pos,BitsForInitialSamplingStep,
                    static_cast<uint32>(par.initialSamplingStep.from),
                    static_cast<uint32>(par.initialSamplingStep.to),
                    genotype);

    // Compute Orientation
    ures = (phen->computeOrientation)? 1 : 0;
    pos = uintToBin(ures,pos,BitsForComputeOrientation,
                    par.computeOrientation.from ? 1 : 0,
                    par.computeOrientation.to   ? 1 : 0,
                    genotype);

    // Neighborhood Factor
    ures = static_cast<uint32>(phen->orientationNeighborhoodFactor);
    pos=uintToBin(ures,pos,BitsForOrientationNeighborhoodFactor,
                  static_cast<uint32>(par.orientationNeighborhoodFactor.from),
                  static_cast<uint32>(par.orientationNeighborhoodFactor.to),
                  genotype);

    // Orientation Sampling Step Factor
    ures = static_cast<uint32>(phen->orientationSamplingStepFactor);
    pos=uintToBin(ures,pos,BitsForOrientationSamplingStepFactor,
                  static_cast<uint32>(par.orientationSamplingStepFactor.from),
                  static_cast<uint32>(par.orientationSamplingStepFactor.to),
                  genotype);

    // Orientation: Wavelet Size Factor
    ures = static_cast<uint32>(phen->orientationWaveletSizeFactor);
    pos = uintToBin(ures,pos,BitsForOrientationWaveletSizeFactor,
                    static_cast<uint32>(par.orientationWaveletSizeFactor.from),
                    static_cast<uint32>(par.orientationWaveletSizeFactor.to),
                    genotype);

    // Orientation: Gaussian Factor
    dres = static_cast<double>(phen->orientationGaussianFactor);
    pos = doubleToBin(dres,pos,BitsForOrientationGaussianFactor,
                      static_cast<double>(par.orientationGaussianFactor.from),
                      static_cast<double>(par.orientationGaussianFactor.to),
                      genotype);

    // Orientation: Window Width
    double angle = phen->orientationWindowWidth;
    if (angle < constants<double>::twoPi()) {
      angle = radToDeg(angle);
    }
    ures = static_cast<uint32>(360.0f/angle);
    pos = uintToBin(ures,pos,BitsForOrientationWindowWidth,
                    static_cast<uint32>(360.0f/
                                        par.orientationWindowWidth.from),
                    static_cast<uint32>(360.0f/
                                        par.orientationWindowWidth.to),
                    genotype);

    // Orientation: Number Of Windows
    ures = static_cast<uint32>(phen->orientationNumberOfWindows);
    pos = uintToBin(ures,pos,BitsForOrientationNumberOfWindows,
                    static_cast<uint32>(par.orientationNumberOfWindows.from),
                    static_cast<uint32>(par.orientationNumberOfWindows.to),
                    genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int fastHessianDetectionGenetics::getChromosomeSize() const {
    static const int size = (BitsForNumberOfLevels +
                             BitsForInitialKernelSize +
                             BitsForLevelSelectionMethod +
                             BitsForInitialKernelStep +
                             BitsForLevelGroupSize +
                             BitsForNormPower +
                             BitsForExtrema +
                             BitsForLocationSelectionMode +
                             BitsForThreshold +
                             BitsForSubsampleLevels +
                             BitsForInitialSamplingStep +
                             BitsForComputeOrientation +
                             BitsForOrientationNeighborhoodFactor +
                             BitsForOrientationSamplingStepFactor +
                             BitsForOrientationWaveletSizeFactor +
                             BitsForOrientationGaussianFactor +
                             BitsForOrientationWindowWidth +
                             BitsForOrientationNumberOfWindows);
    return size;
  }


}
