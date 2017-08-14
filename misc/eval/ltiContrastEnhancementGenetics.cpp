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
 * \file   ltiContrastEnhancementGenetics.h
 *         Defines the abstract parent class of the genetics for contrastEnhacement 
 *         algorithm.
 * \author Miguel Angel Aguilar
 * \date   16.11.07
 *
 * $Id: ltiContrastEnhancementGenetics.cpp,v 1.2 2010-11-04 10:55:18 schnurr Exp $
 */

#include "ltiContrastEnhancementGenetics.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiIOImage.h"
#include "ltiLoadImageList.h"
#include "ltiSecondOrderStatistics.h"
#include "ltiTimer.h"

namespace lti {

  // -------------------------------------------------------------------------
  //   lti::genetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  contrastEnhancementGenetics::parameters::parameters()
    : genetics::parameters() {
    ace          = true;
    entropy      = true;
    speed        = false;

    images       = "images.txt";

  }

  // copy constructor
  contrastEnhancementGenetics::parameters::parameters(const parameters& other)
    : genetics::parameters() {
    copy(other);
  }

  // destructor
  contrastEnhancementGenetics::parameters::~parameters() {
  }

  // copy member

  contrastEnhancementGenetics::parameters&
  contrastEnhancementGenetics::parameters::copy(const parameters& other) {
    genetics::parameters::copy(other);

    ace         = other.ace;
    entropy     = other.entropy;
    speed       = other.speed;

    images       = other.images;

    return *this;
  }

  // alias for copy method
  contrastEnhancementGenetics::parameters&
  contrastEnhancementGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& contrastEnhancementGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  contrastEnhancementGenetics::parameters*
  contrastEnhancementGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  contrastEnhancementGenetics::parameters*
  contrastEnhancementGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool contrastEnhancementGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"ace",ace);
      lti::write(handler,"entropy",entropy);
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
  bool contrastEnhancementGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"ace",ace);
      lti::read(handler,"entropy",entropy);
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
  //   lti::DenoisingGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  contrastEnhancementGenetics::contrastEnhancementGenetics() 
    : genetics(false), contrastEnhancerInstance_(0) {
    parameters par;
    setParameters(par);
  }

  /*
   * Default constructor.
   */
  contrastEnhancementGenetics::contrastEnhancementGenetics(const contrastEnhancementGenetics& other) 
    : genetics(false), contrastEnhancerInstance_(0) {
    copy(other);
  }

  /*
   * Destructor
   */
  contrastEnhancementGenetics::~contrastEnhancementGenetics() {
    delete contrastEnhancerInstance_;
    contrastEnhancerInstance_ = 0;
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& contrastEnhancementGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  contrastEnhancementGenetics& contrastEnhancementGenetics::copy(const contrastEnhancementGenetics& other) {
    genetics::copy(other);
    setContrastEnhancer(other.getContrastEnhancer());
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  contrastEnhancementGenetics& 
  contrastEnhancementGenetics::operator=(const contrastEnhancementGenetics& other) {
    return copy(other);
  }
  

  bool contrastEnhancementGenetics::write(ioHandler& handler,const bool complete) const {
    return genetics::write(handler,complete);
  }

  bool contrastEnhancementGenetics::read(ioHandler& handler,const bool complete) {
    return genetics::read(handler,complete);
  }

    // return parameters
  const contrastEnhancementGenetics::parameters& 
  contrastEnhancementGenetics::getParameters() const {

    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool contrastEnhancementGenetics::updateParameters() {
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

    dimFitness_ = ((par.ace?1:0) +
                   (par.entropy?1:0) +
                   (par.speed?1:0));

    return true;
  }

  /*
   * Attach the instance of the contrast enhancer to be used.
   *
   * This class will take care of the memory management
   */
  bool contrastEnhancementGenetics::setContrastEnhancer(const contrastEnhancement& instance) {
    delete contrastEnhancerInstance_;
    if (isNull(&instance)) {
      contrastEnhancerInstance_= 0;
    } else {
      contrastEnhancerInstance_ = instance.clone();
    }
    return notNull(contrastEnhancerInstance_);
  }

  /*
   * Return a read-only reference to the internal denoiser
   */
  const contrastEnhancement& contrastEnhancementGenetics::getContrastEnhancer() const {
    return *contrastEnhancerInstance_;
  }
   
  /*
   * Check if a valid contrastEnhancement instance has already been set.
   */
  bool contrastEnhancementGenetics::canEnhance() const {
    return notNull(contrastEnhancerInstance_);
  }

  bool contrastEnhancementGenetics::evaluateChromosome(const int, // id, but here unused!
                                             const chromosome& individual,
                                                   dvector& fitness) const {
    return evaluateChromosome(individual,fitness);
  }

  bool contrastEnhancementGenetics::evaluateChromosome(const chromosome& individual,
                                                   dvector& fitness) const {

    if (origs_.empty() || !canEnhance()) {
      setStatusString("No images found to work with, " \
                      "or invalid contrastEnhancement object.");
      return false;
    }

    const parameters& par = getParameters();
    
    contrastEnhancement::parameters* dpar =
      dynamic_cast<contrastEnhancement::parameters*>(chromosomeToPhenotype(individual));

    if (isNull(dpar)) {
      return false;
    }

    // since this method is const, (for multithreading VERY important so)
    // we need to create a contrastEnhancement instance here, that can be altered as
    // we want!
    contrastEnhancement* contrastEnhancer = contrastEnhancerInstance_->clone();
    if (!contrastEnhancer->attachParameters(*dpar)) {
      fitness.assign(dimFitness_,0.0);
      delete contrastEnhancer;
      return false;
    }

    channel res;
    unsigned int i;
    int j;
    timer chronos;
    fitness.assign(dimFitness_,0.0);

    // for all images in the cache
    for (i=0;i<origs_.size();++i) {
      // apply the contrastEnhancement algorithm
      chronos.start();
      if (contrastEnhancer->apply(origs_[i], res)) {
        chronos.stop();

        j=0;
        if (par.ace) {
          fitness.at(j)+=ace(origs_[i], res);
          ++j;
        }
        if (par.entropy) {
          fitness.at(j)+=entropy(res);
          ++j;
        }
        if (par.speed) {
          fitness.at(j)+=chronos.getTime();
          ++j;
        }
      }
    }

    fitness.divide(origs_.size());

    j=0;
    if (par.ace) {
      ++j;
    }
    if (par.entropy) {
      ++j;
    }
    if (par.speed) {
      fitness.at(j)=1.0f/fitness.at(j);
      ++j;
    }

    delete contrastEnhancer;
    return true;
  }

  /*
   * This function computes the ACE (average Contrast Enhancement) 
   *
   * @param lowContrast  image that supuse have a low contrast
   * @param highContrast enhanced version of original image
   */
  double contrastEnhancementGenetics::ace(const channel& lowContrast,
                                          const channel& highContrast) const {
     // Average Contrast Enhance
    double localAverage = 0;
    double average=0;
   
    for(int row = 1; row<lowContrast.rows()-1; ++row) {
      for(int col = 1; col<lowContrast.columns()-1; ++col) {
        localAverage = 0 ;
        for(int m = row -1; m<=row+1; m+=2) { 
            if(lowContrast.at(row,col)!=lowContrast.at(m,col)) {
              localAverage += abs((highContrast.at(row,col)-highContrast.at(m,col))/
                                (lowContrast.at(row,col)-lowContrast.at(m,col)));
            }
        }
        for(int n = col -1; n<=col+1; n+=2){
            if(lowContrast.at(row,col)!=lowContrast.at(row,n)) {
              localAverage += abs((highContrast.at(row,col)-highContrast.at(row,n))/
                                (lowContrast.at(row,col)-lowContrast.at(row,n)));
            }
        }
        average += localAverage; 
      }
    }
    average /= (4*lowContrast.rows()*lowContrast.columns());

    return average;
  }
    
  /*
   * This function computes the Entropy 
   *
   * @param  src image that will compute its entropy
   */
  double contrastEnhancementGenetics::entropy(const channel& src) const {
   
    double E=0;
    vector<double> H(256,0.0);
    channel8 tmp;
    tmp.castFrom(src);

    for(int row = 0; row<tmp.rows(); ++row) {
     for(int col = 0; col<tmp.columns(); ++col) {
       H.at(tmp.at(row,col))+=1.0;
     }
    }
   
    for(int s = 0; s<256; ++s) {
      if((H.at(s)/(tmp.rows()*tmp.columns()))!=0.0){
       E+=-(H.at(s)/(tmp.rows()*tmp.columns()))*
         (lti::log10(H.at(s)/(tmp.rows()*tmp.columns()))/lti::log10(2.));
      }
    }

    return E;
  }
 
}
