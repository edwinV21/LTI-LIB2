/*
 * Copyright (C) 2007
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
 * \file   ltiUnsharpMaskingGaussianGenetics.cpp
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Miguel Angel Aguilar Ulloa
 * \date   19.11.07
 *
 * $Id: ltiUnsharpMaskingGaussianGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiUnsharpMaskingGaussianGenetics.h"
#include "ltiUnsharpMasking.h"
#include "ltiSharpeningKernels.h"
#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,unsharpMaskingGaussianGenetics)

  // -------------------------------------------------------------------------
  //   lti::UnsharpMaskingGaussianGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  unsharpMaskingGaussianGenetics::parameters::parameters()
    : sharpeningGenetics::parameters() {
    highBoostInterval.set(1.0,10.0);
    varianceInterval.set(0.0,100.0);
  }

  // copy constructor
  unsharpMaskingGaussianGenetics::parameters::parameters(const parameters& other)
    : sharpeningGenetics::parameters() {
    copy(other);
  }

  // destructor
  unsharpMaskingGaussianGenetics::parameters::~parameters() {
  }

  // copy member

  unsharpMaskingGaussianGenetics::parameters&
  unsharpMaskingGaussianGenetics::parameters::copy(const parameters& other) {
    sharpeningGenetics::parameters::copy(other);

    highBoostInterval = other.highBoostInterval;
    varianceInterval = other.varianceInterval;
    return *this;
  }

  // alias for copy method
  unsharpMaskingGaussianGenetics::parameters&
  unsharpMaskingGaussianGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& unsharpMaskingGaussianGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  unsharpMaskingGaussianGenetics::parameters*
  unsharpMaskingGaussianGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  unsharpMaskingGaussianGenetics::parameters*
  unsharpMaskingGaussianGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool unsharpMaskingGaussianGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"highBoostInterval",highBoostInterval);
    b = b && lti::write(handler,"varianceInterval",varianceInterval);
    b = b && sharpeningGenetics::parameters::write(handler,false);

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
  bool unsharpMaskingGaussianGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"highBoostInterval",highBoostInterval);
    b = b && lti::read(handler,"varianceInterval",varianceInterval);
    b = b && sharpeningGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------------
  //   lti::UnsharpMaskingGaussianGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  unsharpMaskingGaussianGenetics::unsharpMaskingGaussianGenetics() 
    : sharpeningGenetics() {
    
    // set the type of denoiser to be used
    unsharpMasking um;
    setSharpener(um);
  }

  unsharpMaskingGaussianGenetics::
  unsharpMaskingGaussianGenetics(const unsharpMaskingGaussianGenetics::parameters& p) 
    : sharpeningGenetics() {
    
    // set the type of sharpener to be used
    unsharpMasking um;
    setSharpener(um);

    setParameters(p);
  }

  unsharpMaskingGaussianGenetics::
  unsharpMaskingGaussianGenetics(const unsharpMaskingGaussianGenetics& other) 
    : sharpeningGenetics(other) {
  }

  /*
   * Destructor
   */
  unsharpMaskingGaussianGenetics::~unsharpMaskingGaussianGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& unsharpMaskingGaussianGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  unsharpMaskingGaussianGenetics* unsharpMaskingGaussianGenetics::clone() const {
    return new unsharpMaskingGaussianGenetics(*this);
  }

  /*
   * New instance method.
   */
  unsharpMaskingGaussianGenetics* 
    unsharpMaskingGaussianGenetics::newInstance() const {
    return new unsharpMaskingGaussianGenetics();
  }

  // return parameters
  const unsharpMaskingGaussianGenetics::parameters& 
  unsharpMaskingGaussianGenetics::getParameters() const {

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
  bool unsharpMaskingGaussianGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    unsharpMasking::parameters* phen = 
      dynamic_cast<unsharpMasking::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }
    
    const parameters& par = getParameters();

    // variables to hold the decoded values
    int ires;
    uint32 ures;
    double dres1,dres2;

    phen->kernelType = sharpeningKernels::Gaussian;

    // BitsForKernelSize,
    // encoded as the index (starting with zero) of the sequence {3,5,7,9,...}
    pos = binToUInt(genotype,pos,BitsForKernelSize,ures);
    phen->kernelSize = 3+ures*2; // we want just 3,5,7,9 masks

    // next bits for variance
    pos = binToDouble(genotype,pos,BitsForVariance,
                      par.varianceInterval.from,
                      par.varianceInterval.to,
                      dres1);
    phen->variance = dres1;


    // next bits for high boost constant
    pos = binToDouble(genotype,pos,BitsForHighBoost,
                      par.highBoostInterval.from,
                      par.highBoostInterval.to,
                      dres2);
    phen->A = static_cast<float>(dres2);

    // boundary type on the first BitsForBoundaryType bits
    pos = binToInt(genotype,pos,BitsForBoundaryType,ires);
    switch(ires) {
      case 0:
        phen->boundaryType = Zero;
        break;
      case 1:
        phen->boundaryType = Constant;
        break;
      case 2:
        phen->boundaryType = Periodic;
        break;
      case 3:
        phen->boundaryType = Mirror;
        break;
      default:
        phen->boundaryType = NoBoundary;
        break;
    }

    return true;
  }

  /*
   * Return a fresh allocated parameters for the evaluated functor, which is
   * equivalent to the given genotsqrt(par.lowerOutputLimitInterval.from),
                      sqrt(par.lowerOutputLimitInterval.to),
                      dres3);ype.
   *
   * There are some tools to convert binary chains into standard types:
   * - intToBin()
   * - uintToBin()
   * - doubleToBin()
     */
  unsharpMasking::parameters* unsharpMaskingGaussianGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    unsharpMasking::parameters* um = 
      new unsharpMasking::parameters;
    if (chromosomeToPhenotype(genotype,*um)) {
      return um;
    }
    delete um;
    return 0;
  }

  /*
   * Convert a valid parameters object (phenotype) into binary-chain
   * representation of a chromosome.
   */
  bool unsharpMaskingGaussianGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    const unsharpMasking::parameters* phen = 
      dynamic_cast<const unsharpMasking::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    const parameters& par = getParameters();

    // variables to hold the decoded values
    int ires;
    uint32 ures;
    double dres1, dres2;

     // the size of the kernel is encoded as 
    // the index (starting with zero) of the sequence {3,5,7,9,...}
    ures = static_cast<uint32>((phen->kernelSize - 3) / 2) & 
           static_cast<uint32>((0x1 << BitsForKernelSize) - 1);
    pos = uintToBin(ures,pos,BitsForKernelSize,genotype);

    // next bits for high boost constant
    dres1= phen->variance;
    pos = binToDouble(genotype,pos,BitsForVariance,
                      par.varianceInterval.from,
                      par.varianceInterval.to,
                      dres1);

    // next bits for high boost constant
    dres2= static_cast<double>(phen->A);
    pos = binToDouble(genotype,pos,BitsForHighBoost,
                      par.highBoostInterval.from,
                      par.highBoostInterval.to,
                      dres2);


    // boundary type BitsForBoundaryType 
    switch(phen->boundaryType) {
      case Zero:
        ires = 0;
        break;
      case Constant:
        ires = 1;
        break;
      case Periodic:
        ires = 2;
        break;
      case Mirror:
        ires = 3;
        break;
      default:
        ires = 0;
        break;
    }
    pos = intToBin(ires,pos,BitsForBoundaryType,genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int unsharpMaskingGaussianGenetics::getChromosomeSize() const {
    return BitsForKernelSize + BitsForVariance + 
           BitsForHighBoost + BitsForBoundaryType;
  }


}
