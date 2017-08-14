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
 * \file   ltiMedianFilterGenetics.cpp
 *         Evaluate median filter algorithm
 *
 * \author Miguel Angel Aguilar Ulloa
 * \date   21.10.07
 *
 * $Id: ltiMedianFilterGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiMedianFilterGenetics.h"
#include "ltiMedianFilter.h"
#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,medianFilterGenetics)

  // -------------------------------------------------------------------------
  //   lti::MedianFilterGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  medianFilterGenetics::parameters::parameters()
    : denoisingGenetics::parameters() {

  }

  // copy constructor
  medianFilterGenetics::parameters::parameters(const parameters& other)
    : denoisingGenetics::parameters() {
    copy(other);
  }

  // destructor
  medianFilterGenetics::parameters::~parameters() {
  }

  // copy member

  medianFilterGenetics::parameters&
  medianFilterGenetics::parameters::copy(const parameters& other) {
    denoisingGenetics::parameters::copy(other);

    return *this;
  }

  // alias for copy method
  medianFilterGenetics::parameters&
  medianFilterGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& medianFilterGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  medianFilterGenetics::parameters*
  medianFilterGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  medianFilterGenetics::parameters*
  medianFilterGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool medianFilterGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

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
  bool medianFilterGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && denoisingGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  //   lti::MedianFilterGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  medianFilterGenetics::medianFilterGenetics() 
    : denoisingGenetics() {
    
    // set the type of denoiser to be used
    medianFilter gd;
    setDenoiser(gd);
  }

  medianFilterGenetics::
  medianFilterGenetics(const medianFilterGenetics::parameters& p) 
    : denoisingGenetics() {
    
    // set the type of denoiser to be used
    medianFilter gd;
    setDenoiser(gd);

    setParameters(p);
  }

  medianFilterGenetics::
  medianFilterGenetics(const medianFilterGenetics& other) 
    : denoisingGenetics(other) {
  }

  /*
   * Destructor
   */
  medianFilterGenetics::~medianFilterGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& medianFilterGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  medianFilterGenetics* medianFilterGenetics::clone() const {
    return new medianFilterGenetics(*this);
  }

  /*
   * New instance method.
   */
  medianFilterGenetics* medianFilterGenetics::newInstance() const {
    return new medianFilterGenetics();
  }

  // return parameters
  const medianFilterGenetics::parameters& 
  medianFilterGenetics::getParameters() const {

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
  bool medianFilterGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    medianFilter::parameters* phen = 
      dynamic_cast<medianFilter::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    // variables to hold the decoded values
    int ires;
    uint32 ures;

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

    // the next BitsForSize bits encode the size, encoded as the index
    // (starting with zero) of the sequence {3,5,7,9,...}
    pos = binToUInt(genotype,pos,BitsForSize,ures);
    phen->kernelSize = 3+ures*2; // we want just 3,5,7,9 masks

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
  medianFilter::parameters* medianFilterGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    medianFilter::parameters* gdp = new medianFilter::parameters;
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
  bool medianFilterGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    const medianFilter::parameters* phen = 
      dynamic_cast<const medianFilter::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    // variables to hold the decoded values
    int ires;
    uint32 ures;
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
    pos = intToBin(ires,pos,BitsForBoundaryType,genotype);
    
    // the size of the kernel is encoded as the index (starting 
    // with zero) of the sequence {3,5,7,9,...}
    ures = static_cast<uint32>((phen->kernelSize - 3) / 2) & 
           static_cast<uint32>((0x1 << BitsForSize) - 1);
    pos = uintToBin(ures,pos,BitsForSize,genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int medianFilterGenetics::getChromosomeSize() const {
    return BitsForBoundaryType + BitsForSize;
  }


}
