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
 * \file   ltiGenetics.cpp
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Pablo Alvarado
 * \date   19.08.06
 *
 * $Id: ltiGenetics.cpp,v 1.12 2010-11-04 10:55:18 schnurr Exp $
 */

#include "ltiGenetics.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include <cstdlib>

namespace lti {


  // -------------------------------------------------------------------------
  //                              lti::genetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  genetics::parameters::parameters()
    : parametersManager::parameters() {
    
  }

  // copy constructor
  genetics::parameters::parameters(const parameters& other)
    : parametersManager::parameters() {
    copy(other);
  }

  // destructor
  genetics::parameters::~parameters() {
  }

  // copy member

  genetics::parameters&
  genetics::parameters::copy(const parameters& other) {
    parametersManager::parameters::copy(other);

    randomParams.copy(other.randomParams);

    return *this;
  }

  // alias for copy method
  genetics::parameters&
  genetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& genetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  genetics::parameters*
  genetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  genetics::parameters*
  genetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool genetics::parameters::write(ioHandler& handler,
                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"randomParams",randomParams);
    }

    b = b && parametersManager::parameters::write(handler,false);

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
  bool genetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"randomParams",randomParams);
    }

    b = b && parametersManager::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // -------------------------------------------------------------------------
  //                              lti::genetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  genetics::genetics(const bool init) 
    : ioObject(),parametersManager(),status() { 
    if (init) {
      parameters param;
      setParameters(param);
    } 
  }

  /*
   * Default constructor.
   */
  genetics::genetics(const genetics& other) 
    : ioObject(),parametersManager(),status() { 
    copy(other);
  }

  /*
   * Destructor
   */
  genetics::~genetics() {
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& genetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  bool genetics::write(ioHandler& handler, const bool complete) const {
    return parametersManager::write(handler,complete);
  }

  bool genetics::read(ioHandler& handler,const bool complete) {
    return parametersManager::read(handler,complete);
  }

  // return parameters
  const genetics::parameters& genetics::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool genetics::updateParameters() {
    if (!parametersManager::updateParameters()) {
      return false;
    }

    const parameters& par = getParameters();
    rnd_.setParameters(par.randomParams);
    return true;
  }

  genetics& genetics::copy(const genetics& other) {
    parametersManager::copy(other);
    return *this;
  }

  genetics& genetics::operator=(const genetics& other) {
    return copy(other);
  }

  // ----------------------------------------------------------------------

  bool genetics::randomIndividual(chromosome& genotype) const {
    genotype.resize(getChromosomeSize());
    unsigned int j;
    for (j=0;j<genotype.size();++j) {
      genotype[j] = (rnd_.rand() >= 0.5);
    }
    return true;
  }

  bool genetics::initIndividual(const int index,chromosome& genotype) const {
    if (index>=0) {
      return randomIndividual(genotype);
    }
    return false;
  }


  // mutate
  bool genetics::mutate(const chromosome& parent,
                              chromosome& mutant,
                        const double& mutationRate) const {
    
    if (mutant.size() != parent.size()) {
      mutant.resize(parent.size());
    }

    unsigned int i;
    for (i=0;i<parent.size();++i) {
      mutant[i] = (rnd_.rand() < mutationRate) ? !parent[i] : parent[i];
    }
    
    return true;
  }

  // crossover
  bool genetics::crossover(const chromosome& parent1,
                           const chromosome& parent2,
                                 chromosome& child,
                           const double& mutationRate) const {

    assert(parent1.size() == parent2.size());

    if (child.size() != parent1.size()) {
      child.resize(parent1.size());
    }

    // uniform crossover implementation with mutation
    unsigned int i;
    bool gen;
    for (i=0;i<child.size();++i) {
      gen = (rnd_.rand() < 0.5) ? parent1[i] : parent2[i];  // crossover
      child[i] = (rnd_.rand() < mutationRate) ? !gen : gen; // mutation
    }

    return true;
  }

  /*
   * Return the gray code of the given number
   */
  inline uint32 genetics::grayCode(const uint32 i) {
    return i ^ (i >> 1);  // easy, isn't it?
  }
  
  /*
   * Return the integer value corresponding to the given gray code
   */
  inline uint32 genetics::iGrayCode(const uint32 g) {
    /*
    uint32 a(g),idiv;
    int ish=1; // shift
    while(true) {
      a ^= (idiv = a >> ish);
      if ((idiv <= 1) || (ish == 32)) return a;
      ish <<= 1; // shift x2
    }
    */
    uint32 i=16;
    uint32 tmp(g);
    do {
      tmp^=(tmp>>i);
      i>>=1;
    } while (i>0);
    return tmp;
  }
  
  int genetics::binToInt(const chromosome& chain,
                         const int startBit,
                         const int bitLength,
                               int32& result) {

    uint32 r = 0;
    int next = binToUInt(chain,startBit,bitLength,r);

    if (next > 0) {
      if ((bitLength < 32) && 
          (r >= static_cast<uint32>(1<<(bitLength-1)))) {
        result = static_cast<int>(1<<bitLength) - static_cast<int>(r);
      } else {
        result = static_cast<int>(r);
      }
    }

    return next;
  }


  int genetics::binToInt(const chromosome& chain,
                         const int startBit,
                         const int bitLength,
                         const int lowLimit,
                         const int highLimit,
                               int32& result) {

    uint32 r = 0;
    // value stored with intToBin(...) shifted the value range from lowLimit
    // to 0, so that r is now from zero to (highLimit-lowLimit)
    int next = binToUInt(chain,startBit,bitLength,r);

    int l,h;
    minmax(lowLimit,highLimit,l,h);

    if (next > 0) {
      const int mod = h-l+1;
      result = static_cast<int>(r%mod) + l;
    }

    return next;
  }

  int genetics::binToUInt(const chromosome& chain,
                          const int startBit,
                          const int bitLength,
                                uint32& result) {

    // it will be assumed, that the input bit string encodes the number in
    // Gray code.

    if (static_cast<unsigned int>(startBit+bitLength) > chain.size()) {
      return -1;
    }
    
    assert(bitLength <= 32);

    int i;
    result = 0;
    for (i=0;i<bitLength;++i) {
      result = result << 1;
      result = result | ((chain[i+startBit] ? 1 : 0));
    }

    result = iGrayCode(result); // decode the result

    return bitLength+startBit;
  }

  int genetics::binToUInt(const chromosome& chain,
                          const int startBit,
                          const int bitLength,
                          const uint32 lowLimit,
                          const uint32 highLimit,
                                uint32& result) {

    uint32 r = 0;
    // value stored with intToBin(...) shifted the value range from lowLimit
    // to 0, so that r is now from zero to (highLimit-lowLimit)
    int next = binToUInt(chain,startBit,bitLength,r);

    uint32 l,h;
    minmax(lowLimit,highLimit,l,h);

    if (next > 0) {
      const uint32 mod = h-l+1;
      result = static_cast<uint32>(r%mod) + l;
    }

    return next;
  }


  int genetics::binToDouble(const chromosome& chain,
                            const int startBit,
                            const int bitLength,
                            const double& lowLimit,
                            const double& highLimit,
                                  double& result) {

    double l,h;
    minmax(lowLimit,highLimit,l,h);
    uint32 val;
    int r;
    r=binToUInt(chain,startBit,bitLength,val);
    if (r>0) {
      const uint32 maxis = 0xFFFFFFFF >> (32-bitLength);
      result = val*(h-l)/maxis + l;
      return r;
    }
    return r;
  }

  int genetics::uintToBin(const uint32 value,
                          const int startBit,
                          const int bitLength,
                                chromosome& chain) {
    if (static_cast<unsigned int>(startBit+bitLength) > chain.size()) {
      return -2; // report size exceeded
    }
    
    uint32 v = grayCode(value); // encode with Gray codes
    int i = startBit+bitLength-1;
    while (i>=startBit) {
      chain[i]=((v & 0x01) != 0);
      v = v >> 1;
      --i;
    }

    return startBit+bitLength;
  }

  int genetics::intToBin(const int value,
                         const int startBit,
                         const int bitLength,
                               chromosome& chain) {

    uint32 v = static_cast<uint32>(value);
    return uintToBin(v,startBit,bitLength,chain);
  }

  int genetics::intToBin(const int value,
                         const int startBit,
                         const int bitLength,
                         const int lowLimit,
                         const int highLimit,
                               chromosome& chain) {

    int l,h;
    minmax(lowLimit,highLimit,l,h);
    // v is the input value shifted to map lowLimit to zero.  The value
    // is also cut to be between lowLimit and highLimit
    uint32 v = static_cast<uint32>(max(l,min(h,value))-l);
    return uintToBin(v,startBit,bitLength,chain);
  }

  int genetics::uintToBin(const uint32 value,
                          const int startBit,
                          const int bitLength,
                          const uint32 lowLimit,
                          const uint32 highLimit,
                                chromosome& chain) {

    uint32 l,h;
    minmax(lowLimit,highLimit,l,h);
    // v is the input value shifted to map lowLimit to zero.  The value
    // is also cut to be between lowLimit and highLimit
    uint32 v = static_cast<uint32>(max(l,min(h,value))-l);
    return uintToBin(v,startBit,bitLength,chain);
  }

  int genetics::doubleToBin(const double& value,
                            const int startBit,
                            const int bitLength,
                            const double& lowLimit,
                            const double& highLimit,
                                  chromosome& chain) {

    double l,h;
    minmax(lowLimit,highLimit,l,h);

    if ((value < l) || (value > h)) {
      return -1;
    }

    double normed = (value-l)/(h-l);
    const uint32 maxis = 0xFFFFFFFF >> (32-bitLength);    
    
    uint32 ival = iround(maxis*normed);
    return uintToBin(ival,startBit,bitLength,chain);
  }


  bool genetics::evaluateChromosome(const int, //< id, but here unused!
                                    const chromosome& individual,
                                          dvector& fitness) const {
    return evaluateChromosome(individual,fitness);
  }

  bool genetics::evaluateChromosome(const chromosome& individual,
                                    dvector& fitness,
                                    chromosome& ) const {
    return evaluateChromosome(individual,fitness);
  }

  bool genetics::evaluateChromosome(const int id, //< id, but here unused!
                                    const chromosome& individual,
                                    dvector& fitness,
                                    chromosome&) const {
    return evaluateChromosome(id,individual,fitness);
  }

}

