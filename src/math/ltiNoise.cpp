/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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
 * \file   ltiNoise.cpp
 *         Contains the class noise, to add noise to containers.
 * 
 * \author Pablo Alvarado
 * \date   27.09.2007
 *
 * revisions ..: $Id: ltiNoise.cpp,v 1.3 2009-04-19 04:06:10 alvarado Exp $
 */

#include "ltiNoise.h"
#include "ltiRound.h"
#include "ltiFactory.h"
#include "ltiTypeInfo.h"
#include "ltiUniformContinuousDistribution.h"
#include "ltiNormalDistribution.h"

namespace lti {
  // --------------------------------------------------
  // noise::parameters
  // --------------------------------------------------

  // default constructor
  noise::parameters::parameters()
    : functor::parameters(),generator_(0) {

    coverage = 100.0f;
    crop = true;
    generator_ = new normalDistribution(0.0,0.25);
  }

  // copy constructor
  noise::parameters::parameters(const parameters& other)
    : functor::parameters(),generator_(0) {
    copy(other);
  }

  // destructor
  noise::parameters::~parameters() {
    delete generator_;
    generator_=0;
  }

  // copy member

  noise::parameters&
  noise::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    coverage = other.coverage;
    crop = other.crop;
    selectorParameters.copy(selectorParameters);

    delete generator_;
    generator_ = other.generator_->clone();

    return *this;
  }

  // alias for copy method
  noise::parameters&
  noise::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& noise::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  noise::parameters*
  noise::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  noise::parameters*
  noise::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool noise::parameters::write(ioHandler& handler,
                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"coverage",coverage);
    b = b && lti::write(handler,"crop",crop);
    b = b && lti::write(handler,"selectorParameters",selectorParameters);
    b = b && lti::write(handler,"generatorType",generator_->name());

    b = b && handler.writeBegin();
    b = b && handler.writeSymbol(std::string("generatorParameters"));
    b = b && handler.writeKeyValueSeparator();
    b = b && generator_->getParameters().write(handler);
    b = b && handler.writeEnd();
    
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
  bool noise::parameters::read(ioHandler& handler,
                               const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    std::string str;
    
    b = b && lti::read(handler,"coverage",coverage);
    b = b && lti::read(handler,"crop",crop);
    b = b && lti::read(handler,"selectorParameters",selectorParameters);
    b = b && lti::read(handler,"generatorType",str);

    // use the continuous distribution factory to get the proper type
    univariateContinuousDistribution* ptr =
      factory<univariateContinuousDistribution>::getFactory().newInstance(str);

    if (isNull(ptr)) {
      str = "Unknown distribution type: " +  str;
      handler.setStatusString(str);
      return false;
    } else {
      delete generator_;
      generator_=ptr;
    }

    if (handler.tryBegin()) {
      if (handler.trySymbol("generatorParameters")) {
        b = b && handler.readKeyValueSeparator();
        // we need a frish cloned parameter set of an unknown class
        univariateContinuousDistribution::parameters* uPar =
          generator_->getParameters().clone();
        // read the parameters as they think 
        b = b && uPar->read(handler);
        // now give control of the instance to the generator_
        generator_->attachParameters(*uPar);
      }
      b = b && handler.readEnd();
    }
    
    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  /*
   * Set the random number generator to be used.
   * 
   * A copy of the given generator will be done.
   */
  bool 
  noise::parameters::setNoiseGenerator(univariateContinuousDistribution& g) {
    delete generator_;
    generator_ = g.clone();
    return notNull(generator_);
  }

  /*
   * Get the random number generator to be used.
   *
   * The default value is a normalDistribution with zero mean and a
   * standard deviation of 0.25.
   */
  univariateContinuousDistribution* noise::parameters::getNoiseGenerator() {
    return generator_;
  }

  // --------------------------------------------------
  // noise
  // --------------------------------------------------

  // default constructor
  noise::noise() : functor(), 
                   matrixProcessingInterface<float>(),
                   matrixProcessingInterface<double>(),
                   matrixProcessingInterface<int>(),
                   matrixProcessingInterface<ubyte>(),
                   generator_(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  noise::noise(const parameters& par) : functor(),
                                        matrixProcessingInterface<float>(),
                                        matrixProcessingInterface<double>(),
                                        matrixProcessingInterface<int>(),
                                        matrixProcessingInterface<ubyte>(),
                                        generator_(0) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  noise::noise(const noise& other) : functor(),
                                     matrixProcessingInterface<float>(),
                                     matrixProcessingInterface<double>(),
                                     matrixProcessingInterface<int>(),
                                     matrixProcessingInterface<ubyte>(),
                                     generator_(0) {
    copy(other);
  }

  // destructor
  noise::~noise() {
  }

  // copy member
  noise& noise::copy(const noise& other) {
    functor::copy(other);

    // the generator_ is automatically set when the functor::copy copies and
    // updates the parameters

    return (*this);
  }

  // alias for copy member
  noise&
  noise::operator=(const noise& other) {
    return (copy(other));
  }

  // class name
  const std::string& noise::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  noise* noise::clone() const {
    return new noise(*this);
  }

  // create a new instance
  noise* noise::newInstance() const {
    return new noise();
  }

  // return parameters
  const noise::parameters&
  noise::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  noise::parameters& noise::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool noise::updateParameters() {

    if (functor::updateParameters()) {
      parameters& par = getRWParameters();
      univariateContinuousDistribution* ptr = par.getNoiseGenerator();
      if (isNull(ptr)) {
        setStatusString("No valid univariate distribution has been set yet.");
        return false;
      }
      generator_=par.getNoiseGenerator()->clone();
      uniform_.setParameters(par.selectorParameters);
      uniform_.setInterval(0,100000); // use percentage with three decimals
      return true;
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  template<typename T,typename U>
  inline T noise::crop(const U val) const {
    if (val < static_cast<U>(0)) {
      return static_cast<T>(0);
    } else if (val > static_cast<U>(typeInfo<T>::suggestedNorm())) {
      return typeInfo<T>::suggestedNorm();
    }
    return static_cast<T>(val);
  }

  template<typename T>
  inline bool noise::addNoise(vector<T>& srcdest) const {
    const parameters& par = getParameters();
    if (par.coverage <= 0.0f) {
      return true;
    }

    assert(notNull(generator_));

    typename vector<T>::iterator it,eit;
    
    // if all pixels have to be modified
    if (par.coverage >= 100.0f) {

      if (par.crop) {
        // alter all pixels with crop
        for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
          // use the vector accessor of the matrix
          (*it) = 
            crop<T,double>((*it) +
                           generator_->draw()*typeInfo<T>::suggestedNorm());
        }
      } else {
        // alter all pixels just casting
        for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
          // use the vector accessor of the matrix
          (*it) += 
            static_cast<T>(generator_->draw()*typeInfo<T>::suggestedNorm());
        }
      }


      return true;
    } 
        
    // there are two ways for doing this.  One "exact" in the sense
    // that exactly the given percentage of pixels are altered,
    // generating random indices and altering that pixel.  This version has
    // the disadvantage that many cache fails will be produced.

    // the version here implemented runs sequentially over the image
    // and compares if a uniformly distributed value lies below the
    // given coverage value.  If it is below, then the pixel is
    // altered, otherwise it is left untouched.  The uniformly
    // randomness of the numbers determines how precise the coverage
    // will be.

    // alter just a percentage of the pixels
      
    const int thresh = iround(1000*par.coverage);

    if (par.crop) {
      // crop values
      for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
        if (uniform_.rand()<=thresh) { // random selection
          (*it) = 
            crop<T,double>((*it) +
                           generator_->draw()*typeInfo<T>::suggestedNorm());
        }
      }
    } else {
      // cast only
      for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
        if (uniform_.rand()<=thresh) {
          (*it) += 
            static_cast<T>(generator_->draw()*typeInfo<T>::suggestedNorm());
        }
      }
    }
    return true;

  }

  template<typename T>
  inline bool noise::addNoise(matrix<T>& srcdest) const {
    const parameters& par = getParameters();
    if (par.coverage <= 0.0f) {
      return true;
    }

    assert(notNull(generator_));

    if (srcdest.getMode() == matrix<T>::Connected) {
      typename matrix<T>::iterator it,eit;
      if (par.coverage >= 100.0f) {

        if (par.crop) {
          // alter all pixels with crop
          for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
            // use the vector accessor of the matrix
            (*it) = 
              crop<T,double>((*it) +
                             generator_->draw()*typeInfo<T>::suggestedNorm());
          }
        } else {
          // alter all pixels just casting
          for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
            // use the vector accessor of the matrix
            (*it) += 
              static_cast<T>(generator_->draw()*typeInfo<T>::suggestedNorm());
          }
        }
        return true;
      } 
        
      // alter just a percentage of the pixels
      
      const int thresh = iround(1000*par.coverage);

      if (par.crop) {
        for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
          if (uniform_.rand()<=thresh) {
            // use the vector accessor of the matrix
            (*it) =
              crop<T,double>((*it) +
                             generator_->draw()*typeInfo<T>::suggestedNorm());
          }
        }
      } else {
        for (it=srcdest.begin(),eit=srcdest.end();it!=eit;++it) {
          if (uniform_.rand()<=thresh) {
            // use the vector accessor of the matrix
            (*it) +=
              static_cast<T>(generator_->draw()*typeInfo<T>::suggestedNorm());
          }
        }
      }
      return true;
    } else {
      // not connected matrix
      
      bool b=true;
      for (int y=0;y<srcdest.rows();++y) {
        // add noise to each line
        b = b && addNoise(srcdest.getRow(y));
      }
      return b;
    }

    return false;
  }

  
  // On place apply for type fmatrix!
  bool noise::apply(fmatrix& srcdest) const {
    return addNoise(srcdest);
  }

  // On place apply for type dmatrix!
  bool noise::apply(dmatrix& srcdest) const {
    return addNoise(srcdest);
  }

  // On place apply for type imatrix!
  bool noise::apply(imatrix& srcdest) const {
    return addNoise(srcdest);
  }

  // On place apply for type matrix<ubyte>!
  bool noise::apply(matrix<ubyte>& srcdest) const {
    return addNoise(srcdest);
  }

  // On place apply for type fvector!
  bool noise::apply(fvector& srcdest) const {
    return addNoise(srcdest);
  }

  // On place apply for type dvector!
  bool noise::apply(dvector& srcdest) const {
    return addNoise(srcdest);
  }

  // On place apply for type ivector!
  bool noise::apply(ivector& srcdest) const {
    return addNoise(srcdest);
  }

  // On copy apply for type fmatrix!
  bool noise::apply(const fmatrix& src, fmatrix& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  // On copy apply for type dmatrix!
  bool noise::apply(const dmatrix& src, dmatrix& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  // On copy apply for type imatrix!
  bool noise::apply(const imatrix& src, imatrix& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  // On copy apply for type matrix<ubyte>!
  bool noise::apply(const matrix<ubyte>& src, matrix<ubyte>& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  // On copy apply for type fvector!
  bool noise::apply(const fvector& src, fvector& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  // On copy apply for type dvector!
  bool noise::apply(const dvector& src, dvector& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  // On copy apply for type ivector!
  bool noise::apply(const ivector& src, ivector& dest) const {
    dest.copy(src);
    return addNoise(dest);
  }

  bool 
  noise::setNoiseGenerator(univariateContinuousDistribution& g) {
    parameters& par = getRWParameters();
    par.setNoiseGenerator(g);
    return updateParameters();
  }

  /*
   * Get the random number generator to be used.
   *
   * The default value is a normalDistribution with zero mean and a
   * standard deviation of 0.25.
   */
  univariateContinuousDistribution* noise::getNoiseGenerator() {
    parameters& par = getRWParameters();
    return par.getNoiseGenerator();
  }


}

