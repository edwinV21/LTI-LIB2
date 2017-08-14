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
 * \file   ltiNonLocalMeansDenoisingGenetics.cpp
 *         Evaluate Non-Local Means Denoising
 * \author Pablo Alvarado
 * \date   19.08.06
 *
 * $Id: ltiNonLocalMeansDenoisingGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiNonLocalMeansDenoisingGenetics.h"
#include "ltiNonLocalMeansDenoising.h"
#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,nonLocalMeansDenoisingGenetics)

  // -------------------------------------------------------------------------
  //   lti::NonLocalMeansDenoisingGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  nonLocalMeansDenoisingGenetics::parameters::parameters()
    : denoisingGenetics::parameters() {
    filterDegreeInterval.set(0.0,1.0);
    varianceInterval.set(0.01,100.0);

  }

  // copy constructor
  nonLocalMeansDenoisingGenetics::parameters::parameters(const parameters& other)
    : denoisingGenetics::parameters() {
    copy(other);
  }

  // destructor
  nonLocalMeansDenoisingGenetics::parameters::~parameters() {
  }

  // copy member

  nonLocalMeansDenoisingGenetics::parameters&
  nonLocalMeansDenoisingGenetics::parameters::copy(const parameters& other) {
    denoisingGenetics::parameters::copy(other);

    filterDegreeInterval = other.filterDegreeInterval;
    varianceInterval = other.varianceInterval;

    return *this;
  }

  // alias for copy method
  nonLocalMeansDenoisingGenetics::parameters&
  nonLocalMeansDenoisingGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& nonLocalMeansDenoisingGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  nonLocalMeansDenoisingGenetics::parameters*
  nonLocalMeansDenoisingGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  nonLocalMeansDenoisingGenetics::parameters*
  nonLocalMeansDenoisingGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool nonLocalMeansDenoisingGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"filterDegreeInterval",filterDegreeInterval);
    b = b && lti::write(handler,"varianceInterval",varianceInterval);
    b = b && denoisingGenetics::parameters::write(handler,false);

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
  bool nonLocalMeansDenoisingGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"filterDegreeInterval",filterDegreeInterval);
    b = b && lti::read(handler,"varianceInterval",varianceInterval);
    b = b && denoisingGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------------
  //   lti::NonLocalMeansDenoisingGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  nonLocalMeansDenoisingGenetics::nonLocalMeansDenoisingGenetics() 
    : denoisingGenetics() {
    
    // set the type of denoiser to be used
    nonLocalMeansDenoising nlmd;
    setDenoiser(nlmd);
  }

  nonLocalMeansDenoisingGenetics::
  nonLocalMeansDenoisingGenetics(const nonLocalMeansDenoisingGenetics::parameters& p) 
    : denoisingGenetics() {
    
    // set the type of denoiser to be used
    nonLocalMeansDenoising nlmd;
    setDenoiser(nlmd);

    setParameters(p);
  }

  nonLocalMeansDenoisingGenetics::
  nonLocalMeansDenoisingGenetics(const nonLocalMeansDenoisingGenetics& other) 
    : denoisingGenetics(other) {
  }

  /*
   * Destructor
   */
  nonLocalMeansDenoisingGenetics::~nonLocalMeansDenoisingGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& nonLocalMeansDenoisingGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  nonLocalMeansDenoisingGenetics* nonLocalMeansDenoisingGenetics::clone() const {
    return new nonLocalMeansDenoisingGenetics(*this);
  }

  /*
   * New instance method.
   */
  nonLocalMeansDenoisingGenetics* nonLocalMeansDenoisingGenetics::newInstance() const {
    return new nonLocalMeansDenoisingGenetics();
  }

  // return parameters
  const nonLocalMeansDenoisingGenetics::parameters& 
  nonLocalMeansDenoisingGenetics::getParameters() const {

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
  bool nonLocalMeansDenoisingGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    nonLocalMeansDenoising::parameters* phen = 
      dynamic_cast<nonLocalMeansDenoising::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }
    
    const parameters& par = getParameters();

    // variables to hold the decoded values
    int ires;
    uint32 ures1, ures2;
    double dres1, dres2;

    // boundary type on the first BitsForBoundaryType bits beginning at pos
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

    // the next BitsForWindowSize bits encode the similarity window size,
    // encoded as the index (starting with zero) of the sequence {3,5,7,9,...}
    pos = binToUInt(genotype,pos,BitsForWindowSize,ures1);
    phen->windowSize = 3+ures1*2; // we want just 3,5,7,9 masks

    // the next BitsForSubregionSize bits encode the subregion size,
    // encoded as the index (starting with zero) of the sequence {3,5,7,9,...}
    pos = binToUInt(genotype,pos,BitsForSubregionSize,ures2);
    phen->subregionSize = 3+ures2*2; // 3,5,7,9,11,15 subregion size

    // variance of gaussian kernel
    pos = binToDouble(genotype,pos,BitsForVariance,
                      par.varianceInterval.from,
                      par.varianceInterval.to,
                      dres1);
    phen->variance = dres1;

    // filer degree
    pos = binToDouble(genotype,pos,BitsForFilterDegree,
                      par.filterDegreeInterval.from,
                      par.filterDegreeInterval.to,
                      dres2);
    phen->filterDegree = static_cast<float>(dres2); 

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
  nonLocalMeansDenoising::parameters* nonLocalMeansDenoisingGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    nonLocalMeansDenoising::parameters* nlmdp = new nonLocalMeansDenoising::parameters;
    if (chromosomeToPhenotype(genotype,*nlmdp)) {
      return nlmdp;
    }
    delete nlmdp;
    return 0;
  }

  /*
   * Convert a valid parameters object (phenotype) into binary-chain
   * representation of a chromosome.
   */
  bool nonLocalMeansDenoisingGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    const nonLocalMeansDenoising::parameters* phen = 
      dynamic_cast<const nonLocalMeansDenoising::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    const parameters& par = getParameters();


    // variables to hold the decoded values
    int ires;
    uint32 ures1,ures2;
    double dres1,dres2;
    int pos = 0;

    // boundary type on the first BitsForBoundaryType bits beginning at pos
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
    std::cout<<phen->boundaryType<<std::endl;
    pos = intToBin(ires,pos,BitsForBoundaryType,genotype);
    
    // the size of the similarity window is encoded as 
    // the index (starting with zero) of the sequence {3,5,7,9,...}
    ures1 = static_cast<uint32>((phen->windowSize - 3) / 2);
           //static_cast<uint32>((phen->windowSize - 3) / 2) & 
           //static_cast<uint32>((0x1 << BitsForWindowSize) - 1);
    pos = uintToBin(ures1,pos,BitsForWindowSize,genotype);

    // the size of the subregion is encoded as 
    // the index (starting with zero) of the sequence {3,5,7,9,...}
    ures2 = static_cast<uint32>((phen->subregionSize - 3) / 2);
           //static_cast<uint32>((phen->subregionSize - 3) / 2) & 
           //static_cast<uint32>((0x1 << BitsForSubregionSize) - 1);
    pos = uintToBin(ures2,pos,BitsForSubregionSize,genotype);

    // variance for gaussian kernel
    dres1 = phen->variance;    
    pos = doubleToBin(dres1,pos,BitsForVariance,
                      par.varianceInterval.from,
                      par.varianceInterval.to,
                      genotype);

    // filter degree
    dres2 = static_cast<double>(phen->filterDegree);
    pos = doubleToBin(dres2,pos,BitsForFilterDegree,
                      par.filterDegreeInterval.from,
                      par.filterDegreeInterval.to,
                      genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int nonLocalMeansDenoisingGenetics::getChromosomeSize() const {
    return BitsForBoundaryType + BitsForWindowSize + BitsForSubregionSize 
             + BitsForVariance 
             + BitsForFilterDegree;
  }


}
