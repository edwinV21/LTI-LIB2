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
 * \file   ltiGALContrastEnhancementGenetics.cpp
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Miguel Angel
 * \date   19.08.06
 *
 * $Id: ltiGALContrastEnhancementGenetics.cpp,v 1.2 2008-10-14 01:43:11 alvarado Exp $
 */

#include "ltiGALContrastEnhancementGenetics.h"
#include "ltiGALContrastEnhancement.h"
#include "ltiFactory.h"

namespace lti {

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,galContrastEnhancementGenetics)


  // -------------------------------------------------------------------------
  //   lti::galContrastEnhancementGenetics::parameters
  // -------------------------------------------------------------------------

  
  // default constructor
  galContrastEnhancementGenetics::parameters::parameters()
    : contrastEnhancementGenetics::parameters() {
    deltaInterval.set(0.0,15.0);
  }

  // copy constructor
  galContrastEnhancementGenetics::parameters::parameters(const parameters& other)
    : contrastEnhancementGenetics::parameters() {
    copy(other);
  }

  // destructor
  galContrastEnhancementGenetics::parameters::~parameters() {
  }

  // copy member

  galContrastEnhancementGenetics::parameters&
  galContrastEnhancementGenetics::parameters::copy(const parameters& other) {
    contrastEnhancementGenetics::parameters::copy(other);

    deltaInterval = other.deltaInterval;

    return *this;
  }

  // alias for copy method
  galContrastEnhancementGenetics::parameters&
  galContrastEnhancementGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& galContrastEnhancementGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  galContrastEnhancementGenetics::parameters*
  galContrastEnhancementGenetics::parameters::clone() const {
    return new parameters(*this);
  }
  // new instance
  galContrastEnhancementGenetics::parameters*
  galContrastEnhancementGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool galContrastEnhancementGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"deltaInterval",deltaInterval);
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
   * @return true if read w
   */
  bool galContrastEnhancementGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"deltaInterval",deltaInterval);
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
  galContrastEnhancementGenetics::galContrastEnhancementGenetics() 
    : contrastEnhancementGenetics() {
    
    // set the type of denoiser to be used
    galContrastEnhancement he;
    setContrastEnhancer(he);
  }

  galContrastEnhancementGenetics::
  galContrastEnhancementGenetics(const galContrastEnhancementGenetics::parameters& p) 
    : contrastEnhancementGenetics() {
    
    // set the type of contrast enhancer to be used
    galContrastEnhancement gal;
    setContrastEnhancer(gal);

    setParameters(p);
  }

  galContrastEnhancementGenetics::
  galContrastEnhancementGenetics(const galContrastEnhancementGenetics& other) 
    : contrastEnhancementGenetics(other) {
  }

  /*
   * Destructor
   */
  galContrastEnhancementGenetics::~galContrastEnhancementGenetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& galContrastEnhancementGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone method.
   */
  galContrastEnhancementGenetics* galContrastEnhancementGenetics::clone() const {
    return new galContrastEnhancementGenetics(*this);
  }

  /*
   * New instance method.
   */
  galContrastEnhancementGenetics* galContrastEnhancementGenetics::newInstance() const {
    return new galContrastEnhancementGenetics();
  }

  // return parameters
  const galContrastEnhancementGenetics::parameters& 
  galContrastEnhancementGenetics::getParameters() const {

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
  bool galContrastEnhancementGenetics::
  chromosomeToPhenotype(const chromosome& genotype,
                              functor::parameters& phenotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    galContrastEnhancement::parameters* phen = 
      dynamic_cast<galContrastEnhancement::parameters*>(&phenotype);
    
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }
    
    const parameters& par = getParameters();

    // variables to hold the decoded values
    double dres;

    // initial bits for the number of cells
    pos = binToDouble(genotype,pos,BitsForDelta,
                      par.deltaInterval.from,
                      par.deltaInterval.to,
                     dres);
    phen->delta = static_cast<float>(dres);

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
  galContrastEnhancement::parameters* galContrastEnhancementGenetics::
  chromosomeToPhenotype(const chromosome& genotype) const {
    galContrastEnhancement::parameters* galp = new galContrastEnhancement::parameters;
    if (chromosomeToPhenotype(genotype,*galp)) {
      return galp;
    }
    delete galp;
    return 0;
  }

  /*
   * Convert a valid parameters object (phenotype) into binary-chain
   * representation of a chromosome.
   */
  bool galContrastEnhancementGenetics::
  phenotypeToChromosome(const functor::parameters& phenotype,
                        chromosome& genotype) const {

    int pos=0; // bit index within the chromosome (a chain of bits/genes)

    const galContrastEnhancement::parameters* phen = 
      dynamic_cast<const galContrastEnhancement::parameters*>(&phenotype);
  
    if (isNull(phen)) {
      // this should never happen!  we got the wrong parameters type!
      return false;
    }

    const parameters& par = getParameters();

    // variables to hold the decoded values
    double dres;

    // number of cells
    dres = static_cast<double>(phen->delta);
    pos = doubleToBin(dres,pos,BitsForDelta,
                      par.deltaInterval.from,
                      par.deltaInterval.to,
                      genotype);

    return true;
  }

  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int galContrastEnhancementGenetics::getChromosomeSize() const {
    return BitsForDelta;
  }


}
