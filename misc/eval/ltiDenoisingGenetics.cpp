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
 * $Id: ltiDenoisingGenetics.cpp,v 1.8 2008-10-14 22:47:27 alvarado Exp $
 */

#include "ltiDenoisingGenetics.h"
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
  denoisingGenetics::parameters::parameters()
    : genetics::parameters() {
    snr          = false;
    mse          = true;
    speed        = false;
    methodNoise  = true;

    images       = "images.txt";
    postfix = "";

  }

  // copy constructor
  denoisingGenetics::parameters::parameters(const parameters& other)
    : genetics::parameters() {
    copy(other);
  }

  // destructor
  denoisingGenetics::parameters::~parameters() {
  }

  // copy member

  denoisingGenetics::parameters&
  denoisingGenetics::parameters::copy(const parameters& other) {
    genetics::parameters::copy(other);

    snr          = other.snr;
    mse          = other.mse;
    speed        = other.speed;
    methodNoise  = other.methodNoise;

    images       = other.images;
    postfix      = other.postfix;
    noiseParams  = other.noiseParams;

    return *this;
  }

  // alias for copy method
  denoisingGenetics::parameters&
  denoisingGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& denoisingGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  denoisingGenetics::parameters*
  denoisingGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  denoisingGenetics::parameters*
  denoisingGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool denoisingGenetics::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"snr",snr);
      lti::write(handler,"mse",mse);
      lti::write(handler,"speed",speed);
      lti::write(handler,"methodNoise",methodNoise);
      lti::write(handler,"images",images);
      lti::write(handler,"postfix",postfix);
      lti::write(handler,"noiseParams",noiseParams);
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
  bool denoisingGenetics::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"snr",snr);
      lti::read(handler,"mse",mse);
      lti::read(handler,"speed",speed);
      lti::read(handler,"methodNoise",methodNoise);
      lti::read(handler,"images",images);
      lti::read(handler,"postfix",postfix);
      lti::read(handler,"noiseParams",noiseParams);
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
  denoisingGenetics::denoisingGenetics() 
    : genetics(false), denoiserInstance_(0) {
    parameters par;
    setParameters(par);
  }

  /*
   * Default constructor.
   */
  denoisingGenetics::denoisingGenetics(const denoisingGenetics& other) 
    : genetics(false), denoiserInstance_(0) {
    copy(other);
  }

  /*
   * Destructor
   */
  denoisingGenetics::~denoisingGenetics() {
    delete denoiserInstance_;
    denoiserInstance_ = 0;
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& denoisingGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  denoisingGenetics& denoisingGenetics::copy(const denoisingGenetics& other) {
    genetics::copy(other);
    setDenoiser(other.getDenoiser());
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  denoisingGenetics& 
  denoisingGenetics::operator=(const denoisingGenetics& other) {
    return copy(other);
  }
  

  bool denoisingGenetics::write(ioHandler& handler,const bool complete) const {
    return genetics::write(handler,complete);
  }

  bool denoisingGenetics::read(ioHandler& handler,const bool complete) {
    return genetics::read(handler,complete);
  }

    // return parameters
  const denoisingGenetics::parameters& 
  denoisingGenetics::getParameters() const {

    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool denoisingGenetics::updateParameters() {
    if (!genetics::updateParameters()) {
      return false;
    }

    const parameters& par = getParameters();

    noise noiser(par.noiseParams);
    ioImage iloader;

    loadImageList loader;
    if (!loader.useFileList(par.images)) {
      setStatusString(loader.getStatusString());
      return false;
    };
    const int numImgs = loader.computeSize();

    origs_.clear();
    origs_.reserve(numImgs);

    noisy_.clear();
    noisy_.reserve(numImgs);

    stdevNoise_.allocate(par.snr ? numImgs : 0);
    stdevImage_.allocate(par.snr ? numImgs : 0);
    
    std::string file;
    image img,nimg;
    channel chnl,nchnl,empty,diff;
    float mean;

    while (loader.hasNext()) {
      file = loader.getNextFilename();
      if (loader.load(img)) {
        if (img.empty()) {
          continue;
        }
        chnl.castFrom(img);
        if (par.postfix.empty() || 
            !iloader.load(getNoiseName(file,par.postfix),nimg)) {
          // if we have to generate or there was an error loading
          noiser.apply(chnl,nchnl);
        } else {
          nchnl.castFrom(nimg);
        }

        if (par.snr) {
          const int i = origs_.size();

          // pre-compute std.dev of the image
          stats_.apply(chnl,mean,stdevImage_[i]);
          
          // estimate the std.dev of the original noise
          diff.subtract(nchnl,chnl);
          stats_.apply(diff,mean,stdevNoise_[i]);
        }

        // cache the images
        origs_.push_back(empty);
        chnl.detach(origs_.back());
        noisy_.push_back(empty);
        nchnl.detach(noisy_.back());
      }
    }

    dimFitness_ = ((par.snr?1:0) +
                   (par.mse?1:0) +
                   (par.speed?1:0) +
                   (par.methodNoise?1:0)); 

    return true;
  }

  /*
   * Attach the instance of the denoiser to be used.
   *
   * This class will take care of the memory management
   */
  bool denoisingGenetics::setDenoiser(const denoising& instance) {
    delete denoiserInstance_;
    if (isNull(&instance)) {
      denoiserInstance_= 0;
    } else {
      denoiserInstance_ = instance.clone();
    }
    return notNull(denoiserInstance_);
  }

  /*
   * Return a read-only reference to the internal denoiser
   */
  const denoising& denoisingGenetics::getDenoiser() const {
    return *denoiserInstance_;
  }
   
  /*
   * Check if a valid denoising instance has already been set.
   */
  bool denoisingGenetics::canDenoise() const {
    return notNull(denoiserInstance_);
  }

  bool denoisingGenetics::evaluateChromosome(const int, // id, but here unused!
                                             const chromosome& individual,
                                                   dvector& fitness) const {
    return evaluateChromosome(individual,fitness);
  }

  bool denoisingGenetics::evaluateChromosome(const chromosome& individual,
                                                   dvector& fitness) const {

    if (origs_.empty() || !canDenoise()) {
      setStatusString("No images found to work with, " \
                      "or invalid denoising object.");
      return false;
    }

    const parameters& par = getParameters();
    
    denoising::parameters* dpar =
      dynamic_cast<denoising::parameters*>(chromosomeToPhenotype(individual));

    if (isNull(dpar)) {
      return false;
    }

    // since this method is const, (for multithreading VERY important so)
    // we need to create a denoising instance here, that can be altered as
    // we want!
    denoising* denoiser = denoiserInstance_->clone();
    if (!denoiser->attachParameters(*dpar)) {
      fitness.assign(dimFitness_,0.0);
      delete denoiser;
      return false;
    }

    channel res,diff,diffMethod;
    unsigned int i;
    int j;
    timer chronos;
    fitness.assign(dimFitness_,0.0);

    // for all images in the cache
    for (i=0;i<origs_.size();++i) {
      // apply the denoising algorithm
      chronos.start();
      if (denoiser->apply(noisy_[i],res)) {
        chronos.stop();

        // difference for MSE and SNR
        diff.subtract(origs_[i], res);

        // difference for method noise 
        diffMethod.subtract(noisy_[i],res);
        diffMethod.apply(scale);
  
        j=0;
        if (par.snr) {
          fitness.at(j)+=snr(i,diff);
          ++j;
        }
        if (par.mse) {
          fitness.at(j)+=mse(diff);
          ++j;
        }
        if (par.speed) {
          fitness.at(j)+=chronos.getTime();
          ++j;
        }
        if (par.methodNoise) {
          fitness.at(j)+=methodNoise(diffMethod);
          ++j;
        }
      }
    }

    fitness.divide(origs_.size());

    j=0;
    if (par.snr) {
      ++j;
    }
    if (par.mse) {
      ++j;
    }
    if (par.speed) {
      fitness.at(j)=1.0f/fitness.at(j);
      ++j;
    }

    delete denoiser;
    return true;
  }

  /*
   * This function computes the SNR 
   *
   * @param clean original image without noise
   * @param denoised the output of the denoising algorithm.
   */
  double denoisingGenetics::snr(const int i,
                                const fmatrix& diff) const {
    float mean,stdev,origsnr,newsnr;

    stats_.apply(diff,mean,stdev);

    origsnr = stdevImage_.at(i)/stdevNoise_.at(i);
    newsnr = stdevImage_.at(i)/stdev;
        
    // if now it is worse, then return 0 fitness
    return rectify(newsnr - origsnr);
  }
    
  /*
   * This function computes the 1/MSE 
   *
   * @param clean original image without noise
   * @param denoised the output of the denoising algorithm.
   */
  double denoisingGenetics::mse(const fmatrix& diff) const {
    fmatrix::const_iterator it,eit;
    double acc = 0.00001; // a small value just to avoid divisions by zero
    for (it=diff.begin(),eit=diff.end();it!=eit;++it) {
      acc += sqr(*it);
    }
    
    return (diff.rows()*diff.columns())/acc;
  }

  /*
   * This function computes the method noise scalar (1/ALV)
   *
   * @param image with method noise scaled
   * @param returns a scalar value representing the method noise
   */
  double denoisingGenetics::methodNoise(const fmatrix& diff) const {
    // compute the variance
    double ALV=0.0, average=0.0;
    double totalVariance = 0.0;
 
    for(int i =1; i<diff.rows()-1;++i) {
       for(int j =1; j<diff.columns()-1;++j){
         ALV = 0.0;
         average = 0.0;

         for(int m=i-1; m<=i+1; ++m) {
           for(int n=j-1; n<=j+1; ++n) {
             average += static_cast<double>(diff.at(m,n)); 
           }
         } 
         average /=9.0;

         for(int m=i-1; m<=i+1; ++m) {
           for(int n=j-1; n<=j+1; ++n) {
             ALV += static_cast<double>(sqr(diff.at(m,n)-average)); 
           }
         } 
         ALV /= 9.0;
         totalVariance += ALV;
        }
      }
      return  1.0/(totalVariance/((double)diff.rows()*diff.columns())+0.0001);
  }

 /*
  * Scale method noise image to the range 0-1 
  */
  float denoisingGenetics::scale (const float value) {
    return (value+1.0f)/2.0f;
  }

  /*
   * Construct the noisy image name from the original image name
   */
  std::string 
  denoisingGenetics::getNoiseName(const std::string& imgName,
                                  const std::string& postfix) const {
    
    std::string file(imgName);
    std::string::size_type pos = file.rfind('.');
    if (pos != std::string::npos) {
      file.insert(pos,postfix);
    } else {
      file+=postfix;
    }
    return file;
  }
}
