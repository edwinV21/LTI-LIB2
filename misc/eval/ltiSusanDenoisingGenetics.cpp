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
 * \file   ltiSusanDenoisingGenetics.cpp
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Miguel Angel Aguilar
 * \date   22.10.07
 *
 * $Id: ltiSusanDenoisingGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiSusanDenoisingGenetics.h"
#include "ltiSusanDenoising.h"
#include "ltiFactory.h"

namespace lti {
  // -------------------------------------------------------------------------
  //   lti::SusanDenoisingGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  susanDenoisingGenetics::parameters::parameters()
    : denoisingGenetics::parameters() {
 //   thresholdInterval.set(0,10);
   // formFactorInterval.set(0,4); // even (interval*2) 0,2,4,6,8,10

  }

  // copy constructor
  susanDenoisingGenetics::parameters::parameters(const parameters& other)
    : denoisingGenetics::parameters() {
    copy(other);
  }

  // destructor
  susanDenoisingGenetics::parameters::~parameters() {
  }

  // copy member

  susanDenoisingGenetics::parameters&
  susanDenoisingGenetics::parameters::copy(const parameters& other) {
    denoisingGenetics::parameters::copy(other);

  //  thresholdInterval = other.thresholdInterval;
  //  formFactorInterval = other.formFactorInterval;

    return *this;
  }

  // alias for copy method
  susanDenoisingGenetics::parameters&
  susanDenoisingGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& susanDenoisingGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  susanDenoisingGenetics::parameters*
  susanDenoisingGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  susanDenoisingGenetics::parameters*
  susanDenoisingGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool susanDenoisingGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

//    b = b && lti::write(handler,"thresholdInterval",thresholdInterval);
  //  b = b && lti::write(handler,"formFactorInterval",formFactorInterval);
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
  bool susanDenoisingGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

 //   b = b && lti::read(handler,"thresholdInterval",thresholdInterval);
//    b = b && lti::read(handler,"formFactorInterval",formFactorInterval);
    b = b && denoisingGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------------
  //   lti::SusanDenoisingGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  susanDenoisingGenetics::susanDenoisingGenetics() 
    : denoisingGenetics() {
    
    // set the type of denoiser to be used
    susanDenoising gd;
    setDenoiser(gd);
  }

  susanDenoisingGenetics::
  susanDenoisingGenetics(const susanDenoisingGenetics::parameters& p) 
    : denoisingGenetics() {
    
    // set the type of denoiser to be used
    susanDenoising gd;
    setDenoiser(gd);

    setParameters(p);
  }

  susanDenoisingGenetics::
  susanDenoisingGenetics(const susanDenoisingGenetics& other) 
    : denoisingGenetics(other) {
  }

  /*
   * Destructor
   */
  susanDenoisingGenetics::~susanDenoisingGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& susanDenoisingGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  susanDenoisingGenetics* susanDenoisingGenetics::clone() const {
    return new susanDenoisingGenetics(*this);
  }

  /*
   * New instance method.
   */
  susanDenoisingGenetics* susanDenoisingGenetics::newInstance() const {
    return new susanDenoisingGenetics();
  }

  // return parameters
  const susanDenoisingGenetics::parameters& 
  susanDenoisingGenetics::getParameters() const {

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
  bool susanDenoisingGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    susanDenoising::parameters* phen = 
      dynamic_cast<susanDenoising::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }
    
  //  const parameters& par = getParameters();

    // variables to hold the decoded values
    int ires1, ires2;
    uint32 ures;

    // mask size on the first BitsForMaskSize bits beginning at pos
    pos = binToInt(genotype,pos,BitsForMaskSize,ires1);
    switch(ires1) {
      case 0:
        phen->kernelSize = susanDenoising::Mask9;
        break;
//       case 0:
//         phen->kernelSize = susanDenoising::Mask37;
        break;
      default:
        phen->kernelSize = susanDenoising::Mask9;
        break;
    }

    // the next BitsForThreshold bits encode the the threshold
    pos = binToUInt(genotype,pos,BitsForThreshold,ures);
    phen->threshold = static_cast<ubyte>(ures); 

    // the next BitsForFormFactor bits encode the the form factor 
    // of exponential
    pos = binToInt(genotype,pos,BitsForFormFactor, ires2);
    phen->formFactor = ires2 * 2;  // for even values

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
  susanDenoising::parameters* susanDenoisingGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    susanDenoising::parameters* gdp = new susanDenoising::parameters;
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
  bool susanDenoisingGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    const susanDenoising::parameters* phen = 
      dynamic_cast<const susanDenoising::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

  //  const parameters& par = getParameters();


    // variables to hold the decoded values
    int ires1, ires2;
    uint32 ures;
    int pos = 0;

    // mask size on the first BitsForMaskSize bits beginning at pos
    switch(phen->kernelSize) {
      case susanDenoising::Mask9:
        ires1 = 0;
        break;
//       case susanDenoising::Mask37:
//         ires1 = 0;
//         break;
      default:
        ires1 = 0;
        break;
    }
    pos = intToBin(ires1,pos,BitsForMaskSize,genotype);
    
    // noise suppresion threshold value.
    ures = static_cast<uint32>(phen->threshold);  
    pos = uintToBin(ures,pos,BitsForThreshold,genotype);

    // form factor of the exponential
    ires2 = (phen->formFactor/2);    
    pos = intToBin(ires2,pos,BitsForFormFactor,genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int susanDenoisingGenetics::getChromosomeSize() const {
    return BitsForMaskSize + BitsForThreshold + BitsForFormFactor;
  }


}
