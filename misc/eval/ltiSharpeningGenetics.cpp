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
 * \file   ltiSharpeningGenetics.h
 *         Defines the abstract parent class of the genetics for sharpening 
 *         algorithm.
 * \author Miguel Angel Aguilar Ulloa
 * \date   19.11.07
 *
 * $Id: ltiSharpeningGenetics.cpp,v 1.1 2008-06-02 02:45:50 alvarado Exp $
 */

#include "ltiSharpeningGenetics.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiIOImage.h"
#include "ltiLoadImageList.h"
#include "ltiSecondOrderStatistics.h"
#include "ltiTimer.h"
#include "ltiGradientFunctor.h"
#include "ltiMatrix.h"

namespace lti {

  // -------------------------------------------------------------------------
  //   lti::sharpeningGenetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  sharpeningGenetics::parameters::parameters()
    : genetics::parameters() {
    smoothing    = true;
    sharpening   = true;
    speed        = false;

    images       = "images.txt";

  }

  // copy constructor
  sharpeningGenetics::parameters::parameters(const parameters& other)
    : genetics::parameters() {
    copy(other);
  }

  // destructor
  sharpeningGenetics::parameters::~parameters() {
  }

  // copy member

  sharpeningGenetics::parameters&
  sharpeningGenetics::parameters::copy(const parameters& other) {
    genetics::parameters::copy(other);

    smoothing   = other.smoothing;
    sharpening  = other.sharpening;
    speed       = other.speed;

    images       = other.images;

    return *this;
  }

  // alias for copy method
  sharpeningGenetics::parameters&
  sharpeningGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& sharpeningGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  sharpeningGenetics::parameters*
  sharpeningGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  sharpeningGenetics::parameters*
  sharpeningGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool sharpeningGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"smoothing",smoothing);
      lti::write(handler,"sharpening",sharpening);
      lti::write(handler,"speed",speed);
      lti::write(handler,"images",images);
    }

    b = b && genetics::parameters::write(handler,false);

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
  bool sharpeningGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"smoothing",smoothing);
      lti::read(handler,"sharpening",sharpening);
      lti::read(handler,"speed",speed);
      lti::read(handler,"images",images);
    }

    b = b && genetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  //   lti::SharpeningGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  sharpeningGenetics::sharpeningGenetics() 
    :genetics(false), sharpenerInstance_(0) {
    parameters par;
    setParameters(par);
  }

  /*
   * Default constructor.
   */
  sharpeningGenetics::sharpeningGenetics(const sharpeningGenetics& other) 
    :genetics(false), sharpenerInstance_(0) {
    copy(other);
  }

  /*
   * Destructor
   */
  sharpeningGenetics::~sharpeningGenetics() {
    delete sharpenerInstance_;
    sharpenerInstance_ = 0;
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& sharpeningGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  sharpeningGenetics& sharpeningGenetics::copy(const sharpeningGenetics& other) {
    genetics::copy(other);
    setSharpener(other.getSharpener());
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  sharpeningGenetics& 
  sharpeningGenetics::operator=(const sharpeningGenetics& other) {
    return copy(other);
  }
  

  bool sharpeningGenetics::write(ioHandler& handler,const bool complete) const {
    return genetics::write(handler,complete);
  }

  bool sharpeningGenetics::read(ioHandler& handler,const bool complete) {
    return genetics::read(handler,complete);
  }

    // return parameters
  const sharpeningGenetics::parameters& 
  sharpeningGenetics::getParameters() const {

    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool sharpeningGenetics::updateParameters() {

    if (!genetics::updateParameters()) {
      return false;
    }

    const parameters& par = getParameters();

    ioImage iloader;

    loadImageList loader;
    loader.useFileList(par.images);
    const int numImgs = loader.computeSize();

    origs_.clear();
    origs_.reserve(numImgs);

    std::string file;
    image img;
    channel chnl, empty;

    while (loader.hasNext()) {
      file = loader.getNextFilename();
      if (loader.load(img)) {
        if (img.empty()) {
          continue;
        }
        chnl.castFrom(img);

        // cache the images
        origs_.push_back(empty);
        chnl.detach(origs_.back());
      }
    }

    dimFitness_ = ((par.smoothing?1:0) +
                   (par.sharpening?1:0) +
                   (par.speed?1:0));
    return true;
  }

  /*
   * Attach the instance of the sharpener to be used.
   *
   * This class will take care of the memory management
   */
  bool sharpeningGenetics::setSharpener(const sharpening& instance) {
    delete sharpenerInstance_;
    if (isNull(&instance)) {
      sharpenerInstance_= 0;
    } else {
      sharpenerInstance_ = instance.clone();
    }
    return notNull(sharpenerInstance_);
  }

  /*
   * Return a read-only reference to the internal denoiser
   */
  const sharpening& sharpeningGenetics::getSharpener() const {
    return *sharpenerInstance_;
  }
   
  /*
   * Check if a valid sharpening instance has already been set.
   */
  bool sharpeningGenetics::canSharp() const {
    return notNull(sharpenerInstance_);
  }

  bool sharpeningGenetics::evaluateChromosome(const int, // id, but here unused!
                                             const chromosome& individual,
                                                   dvector& fitness) const {
    return evaluateChromosome(individual,fitness);
  }

  bool sharpeningGenetics::evaluateChromosome(const chromosome& individual,
                                                   dvector& fitness) const {

    if (origs_.empty() || !canSharp()) {
      setStatusString("No images found to work with, " \
                      "or invalid sharpening object.");
      return false;
    }

    const parameters& par = getParameters();
    
    sharpening::parameters* dpar =
      dynamic_cast<sharpening::parameters*>(chromosomeToPhenotype(individual));

    if (isNull(dpar)) {
      return false;
    }

    // since this method is const, (for multithreading VERY important so)
    // we need to create a sharpening instance here, that can be altered as
    // we want!
    sharpening* sharpener = sharpenerInstance_->clone();
    if (!sharpener->attachParameters(*dpar)) {
      fitness.assign(dimFitness_,0.0);
      delete sharpener;
      return false;
    }

    channel res;
    unsigned int i;
    int j;
    timer chronos;
    fitness.assign(dimFitness_,0.0);

    double sm, sh;
    // for all images in the cache
    for (i=0;i<origs_.size();++i) {
      // apply the sharpening algorithm
      chronos.start();
      if (sharpener->apply(origs_[i], res)) {
        chronos.stop();
       
        sm=0.0;
        sh=0.0;
        smsh(origs_[i],res,sm,sh);

        j=0;

        if (par.smoothing) {
          fitness.at(j)+=sm;
          ++j;
        }
        if (par.sharpening) {
          fitness.at(j)+=sh;
          ++j;
        }
        if (par.speed) {
          fitness.at(j)+=1.0/(chronos.getTime()/1000000.0);
          ++j;
        }
      }
    }

    fitness.divide(origs_.size());

    j=0;
    if (par.smoothing) {
      ++j;
    }
    if (par.sharpening) {
      ++j;
    }
    if (par.speed) {
      fitness.at(j)=fitness.at(j);
      ++j;
    }

    delete sharpener;
    return true;
  }


  /*
   * This function computes the amount of sharpening
   * induced by the filter 
   *
   * @param  aB slope of sharpening line
   */
  bool sharpeningGenetics::smsh(const channel& smoothImage,
    const channel& sharpImage, double& sm, double& sh) const {
   
    // Channels for gradient magnitude
    channel smoothGradient, sharpGradient;
    // Gradient Functor
    gradientFunctor gradient;
    // Counters of elements in set A and B
    int aCount =0, bCount=0;
    // slopes
    double aA=0,aB=0;
    sh=sm=0.0;
    // Matrix for sets A and B
    matrix<float> A(smoothImage.rows()*smoothImage.columns(),2,-1.0f);
    matrix<float> B(smoothImage.rows()*smoothImage.columns(),2,-1.0f);

    // Compute magnitude of gradients in each pixel
    gradient.apply(smoothImage, smoothGradient);
    gradient.apply(sharpImage, sharpGradient);

    // Classify gradients into sets A and B
    for(int i=0; i<smoothGradient.rows();++i) {
      for(int j=0; j<smoothGradient.columns();j++) {

        if (smoothGradient.at(i,j)>=sharpGradient.at(i,j)) {
          A.at(aCount,0) = smoothGradient.at(i,j);
          A.at(aCount,1) = sharpGradient.at(i,j);
          ++aCount;      
        }
        else {
          B.at(bCount,0) = smoothGradient.at(i,j);
          B.at(bCount,1) = sharpGradient.at(i,j);
          ++bCount;    
        }
      }
    }

    // Resize the matrix's of each set
    A.copy(A,0,0,aCount-1,1);
    B.copy(B,0,0,bCount-1,1);

    // Sums
    float ExyA=0.0f, ExyB=0.0f, ExxA=0.0f, ExxB=0.0f;

    // Compute sums for A set
    for(int i=0; i<aCount;++i) {
      ExyA +=A.at(i,0)*A.at(i,1); 
      ExxA +=sqr(A.at(i,0));
    }
    // Compute sums for B set
    for(int i=0; i<bCount;++i) {
      ExyB +=B.at(i,0)*B.at(i,1); 
      ExxB +=sqr(B.at(i,0));
    }
    // Compute slopes
    aA = (double)ExyA/((double)(ExxA+0.001f))+0.09; //0.001f is a small value to 
                                                    //prevent divide by zero
                                                    //0.09 is to scale 1/aA in the
                                                    //same range that aB
    aB = (double)ExyB/((double)(ExxB+0.001f));

    sm = (1.0/aA-1.0)*((double)aCount)/((double)(aCount+bCount));
    sh = (aB-1.0)*((double)(bCount))/((double)(aCount+bCount));
    return true;
  }
}
