/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiRandomDistribution.cpp
 *         Contains the class lti::randomDistribution, which is the parent
 *         class of all random distributions in the library.
 * 
 * \author Pablo Alvarado
 * \date   21.09.2007
 *
 * revisions ..: $Id: ltiRandomDistribution.cpp,v 1.6 2012-11-17 15:01:16 alvarado Exp $
 */

#include "ltiRandomDistribution.h"

#include <cstdlib>

namespace lti {
  // --------------------------------------------------
  // randomDistribution::parameters
  // --------------------------------------------------

  // default constructor
  randomDistribution::parameters::parameters()
    : functor::parameters() {
    generator = MT;
    seed      = 15485863;
    device    = "/dev/urandom";
  }

  // copy constructor
  randomDistribution::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  randomDistribution::parameters::~parameters() {
  }

  // copy member

  randomDistribution::parameters&
  randomDistribution::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    generator = other.generator;
    seed      = other.seed;
    device    = other.device;

    return *this;
  }

  // alias for copy method
  randomDistribution::parameters&
  randomDistribution::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& randomDistribution::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  randomDistribution::parameters*
  randomDistribution::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  randomDistribution::parameters*
  randomDistribution::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && lti::write(handler,"generator",generator);
    b = b && lti::write(handler,"seed",seed);
    b = b && lti::write(handler,"device",device);

    b = b && functor::parameters::write(handler,false);

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
  bool randomDistribution::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"generator",generator);
    b = b && lti::read(handler,"seed",seed);
    b = b && lti::read(handler,"device",device);

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // randomDistribution
  // --------------------------------------------------

  // default constructor
  randomDistribution::randomDistribution()
    : functor(),generator_(0) {

    // let the inherited classes do this job

    // create an instance of the parameters with the default values
    //parameters defaultParameters;
    // set the default parameters
    //setParameters(defaultParameters);
  }

  // default constructor
  randomDistribution::randomDistribution(const parameters&)
    : functor(),generator_(0) {

    // let the inherited classes do this job

    // set the given parameters
    // setParameters(par);
  }


  // copy constructor
  randomDistribution::randomDistribution(const randomDistribution& other)
    : functor(),generator_(0) {
    copy(other);
  }

  // destructor
  randomDistribution::~randomDistribution() {
    delete generator_;
    generator_ = 0;
  }

  // copy member
  randomDistribution& 
  randomDistribution::copy(const randomDistribution& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  randomDistribution&
  randomDistribution::operator=(const randomDistribution& other) {
    return (copy(other));
  }

  // class name
  const std::string& randomDistribution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  randomDistribution* randomDistribution::clone() const {
    return new randomDistribution(*this);
  }

  // create a new instance
  randomDistribution* randomDistribution::newInstance() const {
    return new randomDistribution();
  }

  // return parameters
  const randomDistribution::parameters&
  randomDistribution::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  randomDistribution::parameters& randomDistribution::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  bool randomDistribution::updateParameters() {
    delete generator_;
    generator_ = 0;

    const parameters& par = getParameters();

    switch (par.generator) {
      case SystemWide:
        generator_ = new rndSystem;
        break;
      case Stochastic: {
        rndStochastic* rndPtr  = new rndStochastic;
        if (!rndPtr->setDevice(par.device)) {
          delete rndPtr;
          rndPtr=0;
          std::string txt = "Random device ";
          txt += par.device + " could not be set.";
          setStatusString(txt);
          return false;
        }
        generator_ = rndPtr;
        } break;
      case MinStd:
        generator_ = new rndMinStd;
        break;
      case MinStd0:
        generator_ = new rndMinStd0;
        break;
      case MT:
        generator_ = new rndMersenneTwister;
        break;
      default:
        setStatusString("Unknown number generator");
    }
    
    if (notNull(generator_)) {
      max_ = generator_->max();
      if (par.seed != 0) {
        generator_->setSeed(par.seed);
      }
      return true;
    } 

    return false;
  }

  void randomDistribution::setSeed(const uint32 newSeed) {
    getRWParameters().seed = newSeed;

    if (notNull(generator_)) {
      if (newSeed != 0) {
        generator_->setSeed(newSeed);
      }
    } 

  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::write(ioHandler& handler,
                                 const bool complete) const {
    bool b=true;
    if (complete) {
      b = handler.writeBegin();
    }  

    if (functor::write(handler,false)) {
      if ( notNull(generator_) ) {
        return generator_->write(handler,false);
      } else {
        b = false;
      }
    }

    if (complete) {
      b = b && handler.writeEnd();
    }  

    return b;
  }
  
  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::read(ioHandler& handler,const bool complete) {
    bool b=true;
    if (complete) {
      b = handler.readBegin();
    }  

    if (functor::read(handler,false)) {
      if ( notNull(generator_) ) {
        return generator_->read(handler,false);
      }
    } else {
      b = false;
    }

    if (complete) {
      b = b && handler.readEnd();
    }  

    return b;
  }

  
  /**
   * Read the resize mode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,randomDistribution::eNumberGenerator& data) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("ystem") != std::string::npos) {
        data = randomDistribution::SystemWide;
      } else if (str == "Stochastic") {
        data = randomDistribution::Stochastic;
      } else if (str == "MinStd") {
        data = randomDistribution::MinStd;
      } else if (str == "MinStd0") {
        data = randomDistribution::MinStd0;
      } else if (str == "MT") {
        data = randomDistribution::MT;
      } else {
        str = "Unknown random generator type " + str;
        handler.setStatusString(str);
        data = randomDistribution::MT;
        return false;
      }
      return true;
    }
    return false;
  }

  /**
   * Write the resize mode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const randomDistribution::eNumberGenerator& data) {
    bool b=false;
    switch(data) {
      case randomDistribution::SystemWide:
        b=handler.write("SystemWide");
        break;
      case randomDistribution::Stochastic:
        b=handler.write("Stochastic");
        break;
      case randomDistribution::MinStd:
        b=handler.write("MinStd");
        break;
      case randomDistribution::MinStd0:
        b=handler.write("MinStd0");
        break;
      case randomDistribution::MT:
        b=handler.write("MT");
        break;
      default:
        handler.setStatusString("Unknown number generator type");
        handler.write("Unknown");
    }

    return b;
  }
  
  /*
   * Destructor
   */
  randomDistribution::rndBase::~rndBase() {
  }


  // ----------------------------------------------------------------------
  // Random Number Generator using the System rand()
  // ----------------------------------------------------------------------
  
  /*
   * Constructor
   */
  randomDistribution::rndSystem::rndSystem() {
  }

  /*
   * Destructor
   */
  randomDistribution::rndSystem::~rndSystem() {
  }

  /*
   * Get a single number
   */
  uint32 randomDistribution::rndSystem::draw() {
    return static_cast<uint32>(rand());
  }

  /*
   * Set the seed for the generator
   */
  void randomDistribution::rndSystem::setSeed(const uint32 seed) {
    srand(seed);
  }

  /*
   * Return the maximal obtainable number (inclusive)
   */
  uint32 randomDistribution::rndSystem::max() const {
    return RAND_MAX;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndSystem::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    uint32 state = 1;
    b = b && lti::write(handler,"state",state);

    if (complete) {
      b = b && handler.writeEnd();
    } 
    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndSystem::read(ioHandler& handler,
                                           const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    uint32 state;
    b = b && lti::read(handler,"state",state);
    srand(state);
    
    if (complete) {
      b = b && handler.readEnd();
    } 

    return b;
  }

  // ----------------------------------------------------------------------
  // Stochastic Random Number Generator
  // ----------------------------------------------------------------------
  
  /*
   * Constructor
   */
  randomDistribution::rndStochastic::rndStochastic() : handler_(0) {
  }

  /*
   * Destructor
   */
  randomDistribution::rndStochastic::~rndStochastic() {
    if (notNull(handler_)) {
      std::fclose(handler_);
    }
    handler_=0;
  }

  /*
   * Get a single number
   */
  uint32 randomDistribution::rndStochastic::draw() {
    uint32 ret;
    if (std::fread(reinterpret_cast<void*>(&ret), sizeof(uint32),1,handler_)
        == 1) { // did we read 1 uint32
      return ret;
    }

    return 0;
  }

  /*
   * Set the seed for the generator
   */
  void randomDistribution::rndStochastic::setSeed(const uint32) {
    // we need no seed here
  }
  
  /*
   * Return the maximal obtainable number (inclusive)
   */
  uint32 randomDistribution::rndStochastic::max() const {
    return 0xFFFFFFFF; // since all bytes are fully random
  }

  /*
   * Set the file device to be used
   */
  bool 
  randomDistribution::rndStochastic::setDevice(const std::string& device) {
    if (notNull(handler_)) {
      std::fclose(handler_);
      handler_=0;
    }

    handler_ = std::fopen(device.c_str(),"rb");
    if (isNull(handler_)) {
      return false;
    }
    
    return true;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndStochastic::write(ioHandler& handler,
                                                const bool complete) const {
    bool b = false;
    if (complete) {
      b = handler.writeBegin();
      b = b && handler.writeEnd();
    } else {
      return true;
    }
    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndStochastic::read(ioHandler& handler,
                                               const bool complete) {
    bool b = false;
    if (complete) {
      b = handler.readBegin();
      b = b && handler.readEnd();
    } else {
      return true;
    }
    return false;
  }

  // ----------------------------------------------------------------------
  // Stochastic Random Number Generator
  // ----------------------------------------------------------------------
  
  /*
   * Constructor
   */
  randomDistribution::rndMinStd::rndMinStd() : state_(1) {
    
  }

  /*
   * Destructor
   */
  randomDistribution::rndMinStd::~rndMinStd() {
  }

  /*
   * Get a single number
   */
  uint32 randomDistribution::rndMinStd::draw() {

    static const uint32 q = m_ / a_;
    static const uint32 r = m_ % a_;

    // we need some computations base on Schrage's algorithm to avoid
    // integer overflow.

    uint32 t1 = a_ * (state_ % q);
    uint32 t2 = r  * (state_ / q);
    if (t1 >= t2) {
      state_ = t1 - t2;
    } else {
      state_ = m_ - t2 + t1;
    }
  
    return state_;
  }

  /*
   * Set the seed for the generator
   */
  void randomDistribution::rndMinStd::setSeed(const uint32 seed) {
    state_ = seed;
  }

  /*
   * Return the maximal obtainable number (inclusive)
   */
  uint32 randomDistribution::rndMinStd::max() const {
    return m_-1;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndMinStd::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = false;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"state",state_);
    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndMinStd::read(ioHandler& handler,
                                           const bool complete) {
    bool b = false;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"state",state_);
    b = b && handler.readEnd();

    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  // ----------------------------------------------------------------------
  // Stochastic Random Number Generator
  // ----------------------------------------------------------------------
  
  /*
   * Constructor
   */
  randomDistribution::rndMinStd0::rndMinStd0() : state_(1) {

  }

  /*
   * Destructor
   */
  randomDistribution::rndMinStd0::~rndMinStd0() {
  }

  /*
   * Get a single number
   */
  uint32 randomDistribution::rndMinStd0::draw() {

    static const uint32 q = m_ / a_;
    static const uint32 r = m_ % a_;

    // we need some computations base on Schrage's algorithm to avoid
    // integer overflow.

    uint32 t1 = a_ * (state_ % q);
    uint32 t2 = r  * (state_ / q);
    if (t1 >= t2) {
      state_ = t1 - t2;
    } else {
      state_ = m_ - t2 + t1;
    }
  
    return state_;
  }

  /*
   * Set the seed for the generator
   */
  void randomDistribution::rndMinStd0::setSeed(const uint32 seed) {
    state_ = seed;
  }

  /*
   * Return the maximal obtainable number (inclusive)
   */
  uint32 randomDistribution::rndMinStd0::max() const {
    return m_-1;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndMinStd0::write(ioHandler& handler,
                                             const bool complete) const {
    bool b = false;
    if (complete) {
      b = handler.writeBegin();
    }

    uint32 state = 1;
    b = b && lti::write(handler,"state",state);

    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndMinStd0::read(ioHandler& handler,
                                           const bool complete) {
    bool b = false;
    if (complete) {
      b = handler.readBegin();
    }
    uint32 state=0;
    b = b && lti::read(handler,"state",state);
    srand(state);
    
    if (complete) {
      b = b && handler.readEnd();
    } 
    return b;
  }


  // ----------------------------------------------------------------------
  // Stochastic Random Number Generator
  // ----------------------------------------------------------------------
  
  /*
   * Constructor
   */
  randomDistribution::rndMersenneTwister::rndMersenneTwister() : p_(0) {
  }

  /*
   * Destructor
   */
  randomDistribution::rndMersenneTwister::~rndMersenneTwister() {
  }

  /*
   * Get a single number
   */
  uint32 randomDistribution::rndMersenneTwister::draw() {
    // Reload the vector - cost is O(n) amortized over n calls.
    if (p_ >= stateSize_) {
      const static uint32 upperMask = (~uint32()) << maskBits_;
      const static uint32 lowerMask = ~upperMask;
      
      for (int k = 0; k < (stateSize_ - shiftSize_); ++k) {
        uint32 y = ((x_[k] & upperMask) | (x_[k + 1] & lowerMask));
        x_[k] = (x_[k + shiftSize_] ^ (y >> 1) ^ ((y & 0x01) ? a_ : 0));
      }
      
      for (int k = (stateSize_ - shiftSize_); k < (stateSize_ - 1); ++k) {
        uint32 y = ((x_[k] & upperMask) | (x_[k + 1] & lowerMask));
        x_[k] = (x_[k + (shiftSize_ - stateSize_)] ^ (y >> 1)
                 ^ ((y & 0x01) ? a_ : 0));
      }
      
      uint32 y = ((x_[stateSize_ - 1] & upperMask) | (x_[0] & lowerMask));
      x_[stateSize_ - 1] = (x_[shiftSize_ - 1] ^ (y >> 1)
                            ^ ((y & 0x01) ? a_ : 0));
      p_ = 0;
    }

    // Calculate o(x(i)).
    uint32 z = x_[p_++];
    z ^= (z >> u_);
    z ^= (z << s_) & b_;
    z ^= (z << t_) & c_;
    z ^= (z >> l_);
    
    return z;
  }

  /*
   * Set the seed for the generator
   */
  void randomDistribution::rndMersenneTwister::setSeed(const uint32 seed) {
    x_[0] = seed;
    for (int i = 1; i < stateSize_; ++i) {
      uint32 x = x_[i - 1];
      x ^= x >> 30; // bitwise xor
      x *= static_cast<uint32>(1812433253ul);
      x += i;
      x_[i] = x;	  
    }
    p_ = stateSize_;
  }

  /*
   * Return the maximal obtainable number (inclusive)
   */
  uint32 randomDistribution::rndMersenneTwister::max() const {
    return 0xFFFFFFFFu;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool 
  randomDistribution::rndMersenneTwister::write(ioHandler& handler,
                                                const bool complete) const {
    bool b = false;
    if (complete) {
      b = handler.writeBegin();
    }

    // write the status string
    b = b && handler.writeBegin(); // begin status
    
    b = b && handler.writeSymbol(std::string("status"));
    b = b && handler.writeKeyValueSeparator();
    b = b && handler.writeBegin(); // begin data of status
    
    // begin from one to ensure that the last element of the container
    // will NOT be written yet!
    int i;
    for (i=0;i<stateSize_-1;++i) {
      b = b && write(handler,x_[i]);
      b = b && handler.writeDataSeparator();
    }
    b = b && write(handler,x_[i]);
    
    b = b && handler.writeEnd(); // end of data of status
    b = b && handler.writeEnd(); // end of status
    
    b = b && lti::write(handler,"pointer",p_);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool randomDistribution::rndMersenneTwister::read(ioHandler& handler,
                                           const bool complete) {
    bool b = false;
    if (complete) {
      b = handler.readBegin();
    }

    // read the status string
    b = b && handler.readBegin(); // begin status
    std::string str;
    b = b && handler.readSymbol(str);
    if (str == "status") {
      b = b && handler.readKeyValueSeparator();
      b = b && handler.readBegin(); // begin data of status
    
      // begin from one to ensure that the last element of the container
      // will NOT be written yet!
      int i;
      for (i=0;i<stateSize_-1;++i) {
        b = b && read(handler,x_[i]);
        b = b && handler.readDataSeparator();
      }
      b = b && read(handler,x_[i]);
    
      b = b && handler.readEnd(); // end of data of status
    }

    b = b && handler.readEnd(); // end of status
    
    b = b && lti::read(handler,"pointer",p_);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


}

