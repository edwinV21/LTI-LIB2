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
 * \file   ltiGeneticsTester.cpp
 *         Example class for lti::genetics
 * \author Pablo Alvarado
 * \date   20.08.06
 *
 * $Id: ltiGeneticsTester.cpp,v 1.4 2015-01-21 06:20:26 alvarado Exp $
 */

#include "ltiGeneticsTester.h"
#include "ltiFactory.h"
#include "ltiMath.h"

#undef _LTI_DEBUG
#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // dummyFunctor::parameters
  // --------------------------------------------------

  // default constructor
  dummyFunctor::parameters::parameters()
    : functor::parameters() {
    linear = float(1);
    quadratic = float(1);
  }

  // copy constructor
  dummyFunctor::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  dummyFunctor::parameters::~parameters() {
  }

  // get type name
  const std::string& dummyFunctor::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  dummyFunctor::parameters& 
  dummyFunctor::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    linear = other.linear;
    quadratic = other.quadratic;
      
    return *this;
  }

  // alias for copy member
  dummyFunctor::parameters&
  dummyFunctor::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  dummyFunctor::parameters* dummyFunctor::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance member
  dummyFunctor::parameters* dummyFunctor::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
  bool dummyFunctor::parameters::write(ioHandler& handler,
                                         const bool& complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"linear",linear);
      lti::write(handler,"quadratic",quadratic);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was succeful
   */
  bool dummyFunctor::parameters::read(ioHandler& handler,
                                        const bool& complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"linear",linear);
      lti::read(handler,"quadratic",quadratic);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // dummyFunctor
  // --------------------------------------------------

  // default constructor
  dummyFunctor::dummyFunctor()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  dummyFunctor::dummyFunctor(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  dummyFunctor::dummyFunctor(const dummyFunctor& other)
    : functor() {
    copy(other);
  }

  // destructor
  dummyFunctor::~dummyFunctor() {
  }

  // returns the name of this type
  const std::string& dummyFunctor::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member
  dummyFunctor& dummyFunctor::copy(const dummyFunctor& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  dummyFunctor& dummyFunctor::operator=(const dummyFunctor& other) {
    return (copy(other));
  }


  // clone member
  dummyFunctor* dummyFunctor::clone() const {
    return new dummyFunctor(*this);
  }

  // clone member
  dummyFunctor* dummyFunctor::newInstance() const {
    return new dummyFunctor();
  }

  // return parameters
  const dummyFunctor::parameters& dummyFunctor::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On copy apply for type double!
  bool dummyFunctor::apply(const double& src,double& dest) const {

    // the "dummy" functor just evaluates a quadratic function:
    // rect (-x^2 + 4*x)

    const parameters& par = getParameters();


    double x = static_cast<double>(par.quadratic);
    double y = static_cast<double>(par.linear)+src;

    // force an optimum by (2,2), which should be somehow found by the
    // PESA algorithm

    double f = (-x*x - y*y + 4*(x+y) - 4);

    dest = 0.5*(f + sqrt(f*f + 0.01)) -  (1/(1.0+y));

    _lti_debug2("Q: " << par.quadratic <<
                " L: " << par.linear <<
                " -> " << dest << std::endl);

    return true;
  }

  // --------------------------------------------------
  //
  // P A R E T O   T E S T E R
  //
  // --------------------------------------------------

  // In factory<genetics> register this class
  _LTI_REGISTER_IN_FACTORY(genetics,geneticsTester)


  // --------------------------------------------------
  // geneticsTester
  // --------------------------------------------------

  // default constructor
  geneticsTester::geneticsTester()
    : genetics() {
    parameters defPar;
    setParameters(defPar);
  }

  // default constructor
  geneticsTester::geneticsTester(const parameters& par)
    : genetics() {
    setParameters(par);
  }

  // destructor
  geneticsTester::~geneticsTester() {
  }

  // returns the name of this type
  const std::string& geneticsTester::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  geneticsTester* geneticsTester::clone() const {
    return new geneticsTester(*this);
  }

  geneticsTester* geneticsTester::newInstance() const {
    return new geneticsTester();
  }

  /*
   * Convert a binary-chain representation of a chromosome to a valid
   * parameter object.
   */
  bool geneticsTester::chromosomeToPhenotype(const chromosome& genotype,
                                        functor::parameters& phenotype) const {
    
    dummyFunctor::parameters* par = 
      dynamic_cast<dummyFunctor::parameters*>(&phenotype);

    if (isNull(par)) {
      return false;
    }

    int pos=0;
    double ires;
    double dres;
    pos = binToDouble(genotype,pos,bitsForLinear,0.0,15.0,ires);
    par->linear = static_cast<float>(ires);

    pos = binToDouble(genotype,pos,bitsForQuadratic,0.0,10.0,dres);
    par->quadratic = static_cast<float>(dres);

    return true;

    
  }

  /*
   * Return a fresh allocated parameters for the evaluated functor, which is
   * equivalent to the given genotype.
   */
  functor::parameters* 
  geneticsTester::chromosomeToPhenotype(const chromosome& genotype) const {

    dummyFunctor::parameters* par = new dummyFunctor::parameters;
    chromosomeToPhenotype(genotype,*par);

    return par;
  }

  /*
   * Convert a valid parameters object (phenotype) into binary-chain
   * representation of a chromosome.
   */
  bool geneticsTester::phenotypeToChromosome(
                                          const functor::parameters& phenotype,
                                          chromosome& genotype) const {
    const dummyFunctor::parameters* par = 
      dynamic_cast<const dummyFunctor::parameters*>(&phenotype);

    if (isNull(par)) {
      return false;
    }

    genotype.resize(bitsForLinear + bitsForQuadratic);
    int pos;
    pos = doubleToBin(par->linear,0,bitsForLinear,0.0,15.0,genotype);
    pos = doubleToBin(par->quadratic,pos,bitsForQuadratic,
                      0.0,10.0,genotype);

    return true;
  }
   
  /*
   * Return the length in bits for a chromosome.
   *
   * This method needs to be reimplemented, in order to get some 
   * default implementations to work.
   */
  int geneticsTester::getChromosomeSize() const {

    return bitsForLinear+bitsForQuadratic;
  }

  /*
   * Evaluate Chromosome
   *
   * This method is one of the most important ones for the pareto evaluation.
   * Its task is to produce a multidimensional fitness measure for a given
   * chromosome.
   *
   * It returns true if the evaluation was successful, of false if the
   * phenotype represents some invalid parameterization.  It is highly 
   * recomended that the mutation and crossover methods are reimplemented to 
   * avoid invalid parameterizations.
   *
   * There are mainly two types of fitness measures that can be
   * analyzed with this functor of this kind: empirical goodness and
   * empirical discrepancy (Zhang).  The empirical goodness computes some
   * measure using exclusively the test data, without requiring any ground
   * truth.  The empirical discrepancy assumes the existency of ground truth
   * and provides as measure some distance between the result of an algorithm
   * and the ground truth.  Each class derived from geneticsTester should
   * specify clearly which kind of fitness measures it provides.
   * 
   */
  bool geneticsTester::evaluateChromosome(const chromosome& individual,
                                                dvector& fitness) const {
    
    // usually there will be a huge test set and some statistics for all
    // results will be used to build the multidimensional fitness measure.
    
    // Here we assume that dummyFunctor provides already a fitness measure
    // and that one of its parameters tell already how long it will take.
    dummyFunctor::parameters dfpar;
    chromosomeToPhenotype(individual,dfpar);
    dummyFunctor df(dfpar);

    fitness.allocate(2);

    fitness.at(0) = dfpar.linear;
    fitness.at(1) = sqrt(256.0-dfpar.linear*dfpar.linear)*dfpar.quadratic/10.0;

    _lti_debug3("Fitness: " << fitness.at(0) << "," << 
                fitness.at(1) << std::endl);

    return true;
  }


}

