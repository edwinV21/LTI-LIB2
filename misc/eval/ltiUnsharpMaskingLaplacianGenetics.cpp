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
 * \file   ltiUnsharpMaskingLaplacianGenetics.cpp
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Miguel Angel Aguilar Ulloa
 * \date   19.11.07
 *
 * $Id: ltiUnsharpMaskingLaplacianGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiUnsharpMaskingLaplacianGenetics.h"
#include "ltiUnsharpMasking.h"
#include "ltiSharpeningKernels.h"
#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,unsharpMaskingLaplacianGenetics)

  // -------------------------------------------------------------------------
  //   lti::UnsharpMaskingLaplacianGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  unsharpMaskingLaplacianGenetics::parameters::parameters()
    : sharpeningGenetics::parameters() {
    highBoostInterval.set(1.0,10.0);
  }

  // copy constructor
  unsharpMaskingLaplacianGenetics::parameters::parameters(const parameters& other)
    : sharpeningGenetics::parameters() {
    copy(other);
  }

  // destructor
  unsharpMaskingLaplacianGenetics::parameters::~parameters() {
  }

  // copy member

  unsharpMaskingLaplacianGenetics::parameters&
  unsharpMaskingLaplacianGenetics::parameters::copy(const parameters& other) {
    sharpeningGenetics::parameters::copy(other);

    highBoostInterval = other.highBoostInterval;

    return *this;
  }

  // alias for copy method
  unsharpMaskingLaplacianGenetics::parameters&
  unsharpMaskingLaplacianGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& unsharpMaskingLaplacianGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  unsharpMaskingLaplacianGenetics::parameters*
  unsharpMaskingLaplacianGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  unsharpMaskingLaplacianGenetics::parameters*
  unsharpMaskingLaplacianGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool unsharpMaskingLaplacianGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"highBoostInterval",highBoostInterval);
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
  bool unsharpMaskingLaplacianGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"highBoostInterval",highBoostInterval);
    b = b && sharpeningGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------------
  //   lti::UnsharpMaskingLaplacianGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  unsharpMaskingLaplacianGenetics::unsharpMaskingLaplacianGenetics() 
    : sharpeningGenetics() {
    
    // set the type of denoiser to be used
    unsharpMasking um;
    setSharpener(um);
  }

  unsharpMaskingLaplacianGenetics::
  unsharpMaskingLaplacianGenetics(const unsharpMaskingLaplacianGenetics::parameters& p) 
    : sharpeningGenetics() {
    
    // set the type of sharpener to be used
    unsharpMasking um;
    setSharpener(um);

    setParameters(p);
  }

  unsharpMaskingLaplacianGenetics::
  unsharpMaskingLaplacianGenetics(const unsharpMaskingLaplacianGenetics& other) 
    : sharpeningGenetics(other) {
  }

  /*
   * Destructor
   */
  unsharpMaskingLaplacianGenetics::~unsharpMaskingLaplacianGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& unsharpMaskingLaplacianGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  unsharpMaskingLaplacianGenetics* unsharpMaskingLaplacianGenetics::clone() const {
    return new unsharpMaskingLaplacianGenetics(*this);
  }

  /*
   * New instance method.
   */
  unsharpMaskingLaplacianGenetics* 
    unsharpMaskingLaplacianGenetics::newInstance() const {
    return new unsharpMaskingLaplacianGenetics();
  }

  // return parameters
  const unsharpMaskingLaplacianGenetics::parameters& 
  unsharpMaskingLaplacianGenetics::getParameters() const {

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
  bool unsharpMaskingLaplacianGenetics::
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
    int ires1,ires2;
    uint32 ures;
    double dres;

    // BitsForKernelSize,
    // encoded as the index (starting with zero) of the sequence {3,5,7,9,...}
    pos = binToUInt(genotype,pos,BitsForKernelSize,ures);
    phen->kernelSize = 3+ures*2; // we want just 3,5,7,9 masks

    // neighbor type for BitsForNeighborType bits
    pos = binToInt(genotype,pos,BitsForNeighborType,ires1);
    switch(ires1) {
      case 0:
        phen->neighborType = sharpeningKernels::FourNeighbor;
        break;
      case 1:
        phen->neighborType = sharpeningKernels::EightNeighbor;
        break;
      default:
        phen->neighborType = sharpeningKernels::FourNeighbor;
        break;
    }

    // next bits for high boost constant
    pos = binToDouble(genotype,pos,BitsForHighBoost,
                      par.highBoostInterval.from,
                      par.highBoostInterval.to,
                      dres);
    phen->A = static_cast<float>(dres);

    // boundary type on the first BitsForBoundaryType bits
    pos = binToInt(genotype,pos,BitsForBoundaryType,ires2);
    switch(ires2) {
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
  unsharpMasking::parameters* unsharpMaskingLaplacianGenetics::
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
  bool unsharpMaskingLaplacianGenetics::
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
    int ires1,ires2;
    uint32 ures;
    double dres;

     // the size of the kernel is encoded as 
    // the index (starting with zero) of the sequence {3,5,7,9,...}
    ures = static_cast<uint32>((phen->kernelSize - 3) / 2) & 
           static_cast<uint32>((0x1 << BitsForKernelSize) - 1);
    pos = uintToBin(ures,pos,BitsForKernelSize,genotype);

    // neighbor type for BitsForNeighborType bits
    switch(phen->neighborType) {
      case sharpeningKernels::FourNeighbor:
        ires1 = 0;
        break;
      case sharpeningKernels::EightNeighbor:
        ires1 = 1;
        break;
      default:
        ires1 = 0;
        break;
    }
    pos = intToBin(ires1,pos,BitsForNeighborType,genotype);

    // next bits for high boost constant
    dres= static_cast<double>(phen->A);
    pos = binToDouble(genotype,pos,BitsForHighBoost,
                      par.highBoostInterval.from,
                      par.highBoostInterval.to,
                      dres);


    // boundary type BitsForBoundaryType 
    switch(phen->boundaryType) {
      case Zero:
        ires2 = 0;
        break;
      case Constant:
        ires2 = 1;
        break;
      case Periodic:
        ires2 = 2;
        break;
      case Mirror:
        ires2 = 3;
        break;
      default:
        ires2 = 0;
        break;
    }
    pos = intToBin(ires2,pos,BitsForBoundaryType,genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int unsharpMaskingLaplacianGenetics::getChromosomeSize() const {
    return BitsForKernelSize + BitsForNeighborType + 
           BitsForHighBoost + BitsForBoundaryType;
  }


}
