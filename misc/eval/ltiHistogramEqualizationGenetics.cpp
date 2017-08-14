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
 * \file   ltiHistogramEqualizationGenetics.cpp
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Miguel Angel Aguilar Ulloa
 * \date   16.11.07
 *
 * $Id: ltiHistogramEqualizationGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiHistogramEqualizationGenetics.h"
#include "ltiHistogramEqualization.h"
#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,histogramEqualizationGenetics)

  // -------------------------------------------------------------------------
  //   lti::HistogramEqualizationGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  histogramEqualizationGenetics::parameters::parameters()
    : contrastEnhancementGenetics::parameters() {
    lowerInputLimitInterval.set(0.0,1.0);
    upperInputLimitInterval.set(0.0,1.0);
    lowerOutputLimitInterval.set(0.0,1.0);
    upperOutputLimitInterval.set(0.0,1.0);
  }

  // copy constructor
  histogramEqualizationGenetics::parameters::parameters(const parameters& other)
    : contrastEnhancementGenetics::parameters() {
    copy(other);
  }

  // destructor
  histogramEqualizationGenetics::parameters::~parameters() {
  }

  // copy member

  histogramEqualizationGenetics::parameters&
  histogramEqualizationGenetics::parameters::copy(const parameters& other) {
    contrastEnhancementGenetics::parameters::copy(other);

    lowerInputLimitInterval = other.lowerInputLimitInterval;
    upperInputLimitInterval = other.upperInputLimitInterval;
    lowerOutputLimitInterval = other.lowerOutputLimitInterval;
    upperOutputLimitInterval = other.upperOutputLimitInterval;

    return *this;
  }

  // alias for copy method
  histogramEqualizationGenetics::parameters&
  histogramEqualizationGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& histogramEqualizationGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  histogramEqualizationGenetics::parameters*
  histogramEqualizationGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  histogramEqualizationGenetics::parameters*
  histogramEqualizationGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool histogramEqualizationGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"lowerInputLimitInterval",lowerInputLimitInterval);
    b = b && lti::write(handler,"upperInputLimitInterval",upperInputLimitInterval);
    b = b && lti::write(handler,"lowerOutputLimitInterval",lowerOutputLimitInterval);
    b = b && lti::write(handler,"upperOutputLimitInterval",upperOutputLimitInterval);
    b = b && contrastEnhancementGenetics::parameters::write(handler,false);

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
  bool histogramEqualizationGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"lowerInputLimitInterval",lowerInputLimitInterval);
    b = b && lti::read(handler,"upperInputLimitInterval",upperInputLimitInterval);
    b = b && lti::read(handler,"lowerOutputLimitInterval",lowerOutputLimitInterval);
    b = b && lti::read(handler,"upperOutputLimitInterval",upperOutputLimitInterval);
    b = b && contrastEnhancementGenetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------------
  //   lti::HistogramEqualizationGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  histogramEqualizationGenetics::histogramEqualizationGenetics() 
    : contrastEnhancementGenetics() {
    
    // set the type of denoiser to be used
    histogramEqualization he;
    setContrastEnhancer(he);
  }

  histogramEqualizationGenetics::
  histogramEqualizationGenetics(const histogramEqualizationGenetics::parameters& p) 
    : contrastEnhancementGenetics() {
    
    // set the type of contrast enhancer to be used
    histogramEqualization he;
    setContrastEnhancer(he);

    setParameters(p);
  }

  histogramEqualizationGenetics::
  histogramEqualizationGenetics(const histogramEqualizationGenetics& other) 
    : contrastEnhancementGenetics(other) {
  }

  /*
   * Destructor
   */
  histogramEqualizationGenetics::~histogramEqualizationGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& histogramEqualizationGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  histogramEqualizationGenetics* histogramEqualizationGenetics::clone() const {
    return new histogramEqualizationGenetics(*this);
  }

  /*
   * New instance method.
   */
  histogramEqualizationGenetics* 
    histogramEqualizationGenetics::newInstance() const {
    return new histogramEqualizationGenetics();
  }

  // return parameters
  const histogramEqualizationGenetics::parameters& 
  histogramEqualizationGenetics::getParameters() const {

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
  bool histogramEqualizationGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    histogramEqualization::parameters* phen = 
      dynamic_cast<histogramEqualization::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }
    
    const parameters& par = getParameters();

    // variables to hold the decoded values
    int ires;
    double dres1,dres2,dres3,dres4;


    // initial bits for the number of cells
    pos = binToInt(genotype,pos,BitsForCells, ires);
    phen->cells = ires; 

    // next bits for lower input limit
    pos = binToDouble(genotype,pos,BitsForLowerInputLimit,
                      par.lowerInputLimitInterval.from,
                      par.lowerInputLimitInterval.to,
                      dres1);

    
    // next bits for upper input limit
    pos = binToDouble(genotype,pos,BitsForUpperInputLimit,
                      par.upperInputLimitInterval.from,
                      par.upperInputLimitInterval.to,
                      dres2);

    phen->lowerInputLimit = static_cast<float>(min(dres1,dres2));
    phen->upperInputLimit = static_cast<float>(max(dres1,dres2));

    // next bits for lower output limit
    pos = binToDouble(genotype,pos,BitsForLowerOutputLimit,
                      par.lowerOutputLimitInterval.from,
                      par.lowerOutputLimitInterval.to,
                      dres3);
    
    // next bits for upper input limit
    pos = binToDouble(genotype,pos,BitsForUpperOutputLimit,
                      par.upperOutputLimitInterval.from,
                      par.upperOutputLimitInterval.to,
                      dres4);

    phen->lowerOutputLimit = static_cast<float>(min(dres3,dres4));
    phen->upperOutputLimit = static_cast<float>(max(dres3,dres4));

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
  histogramEqualization::parameters* histogramEqualizationGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    histogramEqualization::parameters* hep = 
      new histogramEqualization::parameters;
    if (chromosomeToPhenotype(genotype,*hep)) {
      return hep;
    }
    delete hep;
    return 0;
  }

  /*
   * Convert a valid parameters object (phenotype) into binary-chain
   * representation of a chromosome.
   */
  bool histogramEqualizationGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    const histogramEqualization::parameters* phen = 
      dynamic_cast<const histogramEqualization::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    const parameters& par = getParameters();


    // variables to hold the decoded values
    int ires;
    double dres1,dres2,dres3,dres4;

    // number of cells
    ires = phen->cells;
    pos = intToBin(ires,pos,BitsForCells,genotype);
    
    // lower input limit
    dres1 = static_cast<double>(phen->lowerInputLimit);
    pos = doubleToBin(dres1,pos,BitsForLowerInputLimit,
                      par.lowerInputLimitInterval.from,
                      par.lowerInputLimitInterval.to,
                      genotype);

    // upper input limit
    dres2 = static_cast<double>(phen->upperInputLimit);
    pos = doubleToBin(dres2,pos,BitsForUpperInputLimit,
                      par.upperInputLimitInterval.from,
                      par.upperInputLimitInterval.to,
                      genotype);

    // lower output limit
    dres3 = static_cast<double>(phen->lowerOutputLimit);
    pos = doubleToBin(dres3,pos,BitsForLowerOutputLimit,
                      par.lowerOutputLimitInterval.from,
                      par.lowerOutputLimitInterval.to,
                      genotype);

    // upper output limit
    dres4 = static_cast<double>(phen->upperOutputLimit);
    pos = doubleToBin(dres4,pos,BitsForUpperOutputLimit,
                      par.upperOutputLimitInterval.from,
                      par.upperOutputLimitInterval.to,
                      genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int histogramEqualizationGenetics::getChromosomeSize() const {
    return BitsForLowerInputLimit + BitsForUpperInputLimit + 
           BitsForLowerOutputLimit + BitsForUpperOutputLimit+ BitsForCells;
  }


}
