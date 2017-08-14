/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiSegmentationGenetics.h
 *         Defines the abstract parent class of the genetic evaluation of
 *         segmentation algorithms.
 * \author Pablo Alvarado
 * \date   21.05.08
 *
 * $Id: ltiSegmentationGenetics.cpp,v 1.3 2008-06-04 02:20:12 alvarado Exp $
 */

#include "ltiSegmentationGenetics.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiIOImage.h"
#include "ltiLoadImageList.h"
#include "ltiTimer.h"
#include "ltiConstants.h"
#include "ltiList.h"
 
#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#define _LTI_SAVE_PARAM 1

#include "ltiViewer2D.h"
#include "ltiDraw.h"
#include "ltiLispStreamHandler.h"
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

#endif

namespace lti {

  // -------------------------------------------------------------------------
  //   lti::genetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  segmentationGenetics::parameters::parameters()
    : genetics::parameters() {

    numberOfRegions = false;
    numRegionsReciprocal = false;
    pixelWisePotentialAccuracy = true;
    objectWisePotentialAccuracy = false;
    regionWiseInformationContent = true;
    throughput = false;
    regionIntegrity = false;
    pixelWiseCertainty = false;

    images = std::string("images.txt");
    goldenPostfix = "_mask";
    prevStagePostfix = "_ibs";

    minValidRegionSize = 0.0005f;

  }

  // copy constructor
  segmentationGenetics::parameters::parameters(const parameters& other)
    : genetics::parameters() {
    copy(other);
  }

  // destructor
  segmentationGenetics::parameters::~parameters() {
  }

  // copy member

  segmentationGenetics::parameters&
  segmentationGenetics::parameters::copy(const parameters& other) {
    genetics::parameters::copy(other);

    numberOfRegions = other.numberOfRegions;
    numRegionsReciprocal = other.numRegionsReciprocal;
    pixelWisePotentialAccuracy = other.pixelWisePotentialAccuracy;
    objectWisePotentialAccuracy = other.objectWisePotentialAccuracy;
    regionWiseInformationContent = other.regionWiseInformationContent;
    throughput = other.throughput;
    regionIntegrity = other.regionIntegrity;
    pixelWiseCertainty = other.pixelWiseCertainty;

    images = other.images;
    goldenPostfix = other.goldenPostfix;
    prevStagePostfix = other.prevStagePostfix;

    minValidRegionSize = other.minValidRegionSize;

    return *this;
  }

  // alias for copy method
  segmentationGenetics::parameters&
  segmentationGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& segmentationGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  segmentationGenetics::parameters*
  segmentationGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  segmentationGenetics::parameters*
  segmentationGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool 
  segmentationGenetics::parameters::write(ioHandler& handler,
                                               const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"numberOfRegions",numberOfRegions);
      lti::write(handler,"numRegionsReciprocal",numRegionsReciprocal);
      lti::write(handler,"pixelWisePotentialAccuracy",
                 pixelWisePotentialAccuracy);
      lti::write(handler,"objectWisePotentialAccuracy",
                 objectWisePotentialAccuracy);
      lti::write(handler,"regionWiseInformationContent",
                 regionWiseInformationContent);
      lti::write(handler,"throughput",throughput);
      lti::write(handler,"regionIntegrity",regionIntegrity);
      lti::write(handler,"pixelWiseCertainty",pixelWiseCertainty);

      lti::write(handler,"images",images);
      lti::write(handler,"goldenPostfix",goldenPostfix);
      lti::write(handler,"prevStagePostfix",prevStagePostfix);

      lti::write(handler,"minValidRegionSize",minValidRegionSize);
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
  bool segmentationGenetics::parameters::read(ioHandler& handler,
                                                   const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"numberOfRegions",numberOfRegions);
      lti::read(handler,"numRegionsReciprocal",numRegionsReciprocal);
      lti::read(handler,"pixelWisePotentialAccuracy",
                 pixelWisePotentialAccuracy);
      lti::read(handler,"objectWisePotentialAccuracy",
                 objectWisePotentialAccuracy);
      lti::read(handler,"regionWiseInformationContent",
                 regionWiseInformationContent);
      lti::read(handler,"throughput",throughput);
      lti::read(handler,"regionIntegrity",regionIntegrity);
      lti::read(handler,"pixelWiseCertainty",pixelWiseCertainty);

      lti::read(handler,"images",images);
      lti::read(handler,"goldenPostfix",goldenPostfix);
      lti::read(handler,"prevStagePostfix",prevStagePostfix);

      lti::read(handler,"minValidRegionSize",minValidRegionSize);
    }

    b = b && genetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  //   lti::segmentationGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  segmentationGenetics::segmentationGenetics() 
    : genetics(false), segmenter_(0), dimFitness_(0) {
  }

  /*
   * Default constructor.
   */
  segmentationGenetics::
  segmentationGenetics(const segmentationGenetics& other) 
    : genetics(false), segmenter_(0), dimFitness_(0) {
    copy(other);
  }

  /*
   * Destructor
   */
  segmentationGenetics::~segmentationGenetics() {
    delete segmenter_;
    segmenter_=0;
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& segmentationGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  segmentationGenetics& 
  segmentationGenetics::copy(const segmentationGenetics& other) {
    genetics::copy(other);

    if (other.canSegment()) {
      setSegmenter(other.getSegmenter());
    }

    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  segmentationGenetics& 
  segmentationGenetics::operator=(const segmentationGenetics& other){
    return copy(other);
  }
  

  bool 
  segmentationGenetics::write(ioHandler& handler,const bool c) const {
    return genetics::write(handler,c);
  }

  bool segmentationGenetics::read(ioHandler& handler,const bool c) {
    return genetics::read(handler,c);
  }

    // return parameters
  const segmentationGenetics::parameters& 
  segmentationGenetics::getParameters() const {
    
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool segmentationGenetics::updateParameters() {
    if (!genetics::updateParameters()) {
      return false;
    }

    const parameters& param = getParameters();

    ioImage iloader;
    loadImageList loader;
    loader.useFileList(param.images);
    const int numImgs = loader.computeSize();

    image img;
    channel8 mask;
    imatrix pmask;
    int i;
    bool error = false;
    std::string file;

    channel8 emptyChannel;
    image emptyImage;
    imatrix emptyMatrix;

    imageData_.clear();
    imageData_.reserve(numImgs);

    maskData_.clear();
    maskData_.reserve(numImgs);

    prevMaskData_.clear();
    prevMaskData_.reserve(numImgs);

    goldenNames_.clear();
    goldenNames_.reserve(loader.computeSize());

    std::string errMsg;

    i=0;
    while (loader.hasNext()) {
      file = loader.getNextFilename();
      if (loader.load(img) && 
          iloader.load(getMaskName(file,param.goldenPostfix),mask)) {
        
        goldenNames_.push_back(file); // cache the filename

        // avoid copying too many times the loaded image: first reserve the
        // container, then transfer the available contents to them.
        maskData_.push_back(emptyChannel);
        imageData_.push_back(emptyImage);

        mask.detach(maskData_[i]);
        img.detach(imageData_[i]);

        // try to load previous stage mask
        if (iloader.load(getPrevName(file,param.prevStagePostfix),pmask)) {
          if (i == static_cast<int>(prevMaskData_.size())) {
            prevMaskData_.push_back(emptyMatrix);
          } else {
            prevMaskData_.resize(i+1,emptyMatrix);
          }
          pmask.detach(prevMaskData_[i]);
        } else {
          if (!prevMaskData_.empty()) {
            if (!error) {
              errMsg = "Some masks for the previous level not found:";
              error = true;
            }
            errMsg += getPrevName(file,param.prevStagePostfix) + "\n  ";
          }
        }

        i++;
      } else {
        if (!error) {
          errMsg = "Following images could not be read:\n";
          error = true;
        }

        errMsg += file + "\n  ";
      }
    }
    
    if ( !error && (i==0) ) {
      setStatusString("No images could be found.  Maybe wrong path?");
      return false;
    } 

    if (!errMsg.empty()) {
      setStatusString(errMsg);
    } else {
      dimFitness_ = ((param.numberOfRegions ?1:0) +
                     (param.numRegionsReciprocal ?1:0) +
                     (param.pixelWisePotentialAccuracy ?1:0) +
                     (param.objectWisePotentialAccuracy ?1:0) +
                     (param.regionWiseInformationContent ?1:0) +
                     (param.throughput ?1:0) +
                     (param.regionIntegrity ?1:0) +
                     (param.pixelWiseCertainty ?1:0));
    }
    
    return !error;
  }

  bool segmentationGenetics::evaluateChromosome(const int, // id, here unused!
                                                const chromosome& individual,
                                                      dvector& fitness) const {
    return evaluateChromosome(individual,fitness);
  }

  bool segmentationGenetics::evaluateChromosome(const chromosome& individual,
                                                      dvector& fitness) const {

    const segmentation::parameters* segPar =
      chromosomeToPhenotype(individual);

    dvector mfitness;

#if defined(_LTI_SAVE_PARAM)
    // for debug purposes, if the system crashes the given file will have
    // the parameters currently under evaluation, so that it can be possible
    // to isolate the case:
    char buffer[128];
    sprintf(buffer,"lti_iseg_eval_dbg_%i.txt",getpid());
    std::ofstream _dbg_out(buffer);
    lispStreamHandler lsh(_dbg_out);
    lsPar->write(lsh);
    _dbg_out << std::endl;
    _dbg_out.close();
#endif

    if (evaluate(*segPar,mfitness)) {
      const parameters& par = getParameters();
      if (fitness.size() != dimFitness_) {
        fitness.allocate(dimFitness_);
      }

      int j;
      j = 0;

      if ((par.numberOfRegions) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxNumRegions);
        j++;
      } 

      if ((par.numRegionsReciprocal) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxNumRegionsRecip);
        j++;
      } 

      if ((par.pixelWisePotentialAccuracy) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxPixelAccuracy);
        j++;
      }

      if ((par.objectWisePotentialAccuracy) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxObjectAccuracy);
        j++;
      }

      if ((par.regionWiseInformationContent) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxRegionWiseInfo);
        j++;
      }

      if ((par.throughput) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxThroughput);
        j++;
      }

      if ((par.regionIntegrity) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxRegionIntegrity);
        j++;
      }

      if ((par.pixelWiseCertainty) &&
          (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(IdxPixelCertainty);
        j++;
      }
    } else {
      delete segPar;
      segPar = 0;
      return false;
    }

    delete segPar;
    segPar = 0;

    return true;
  }

  /*
   * Attach the instance of segmentation to be used.
   *
   * This class will take care of the memory management
   */
  bool segmentationGenetics::setSegmenter(const segmentation& instance) {
    delete segmenter_;
    if (isNull(&instance)) {
      segmenter_= 0;
    } else {
      segmenter_ = instance.clone();
    }
    return notNull(segmenter_);
  }

  /*
   * Return a read-only reference to the internal denoiser
   */
  const segmentation& segmentationGenetics::getSegmenter() const {
    return *segmenter_;
  }
   
  /*
   * Check if a valid segmentation instance has already been set.
   */
  bool segmentationGenetics::canSegment() const {
    return notNull(segmenter_);
  }

  /*
   * Construct the noisy image name from the original image name
   */
  std::string 
    segmentationGenetics::getMaskName(const std::string& imgName,
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

  std::string 
    segmentationGenetics::getPrevName(const std::string& imgName,
                                      const std::string& prevPostfix) const {
    std::string file;
    std::string::size_type pos = imgName.rfind('.');
    if (pos != std::string::npos) {
      file = imgName.substr(0,pos) + prevPostfix + ".lti";
    } else {
      file = imgName + prevPostfix + ".lti";
    }
    return file;
  }


  bool segmentationGenetics::evaluate(const segmentation::parameters& param,
                                            dvector& fitness) const {

    // This method will be called in parallel, so that it requires
    // its own instance of the detector_ with its own parameters. 

    image img;
    imatrix segMask;

    // set the location parameters only once
    if (isNull(segmenter_)) {
      setStatusString("No segmentation instance set yet.");
      fitness.clear();
      return false;
    }

    const int size = static_cast<int>(imageData_.size());
    if (size == 0) {
      // the statusString was initialized while setting the parameters!
      return false;
    }

    // we need an instance of the detector for this thread only!
    segmentation* segmenter = segmenter_->clone();
    if (!segmenter->setParameters(param)) {
      setStatusString(segmenter->getStatusString());
      return false;
    }

    // assume all images are ok
    fitness.assign(IdxTotalFitnessDim,-1.0);

     // assume all images are ok
    dvector accFitness(IdxTotalFitnessDim,0.0);
    dvector accNorm(IdxTotalFitnessDim,0.0);
    dvector norm(IdxTotalFitnessDim,0.0);

    int i=0; // i counts the number of successful read images
    int j=0;
    
    // are there any mask in the previous masks cache?
    
    if (prevMaskData_.empty()) {
      static const imatrix emptyIMatrix;

      // for all channels read in setParameters()
      while (i<size) {
        // evaluate the image with the given ground data
        if (evaluate(*segmenter,
                     imageData_[i],
                     emptyIMatrix,
                     maskData_[i],
                     fitness,
                     norm)) {
          accFitness.add(fitness);
          accNorm.add(norm);
          j++;
        } 
        ++i;
      }

    } else {
      // for all channels read in setParameters()
      while (i<size) {
        // evaluate the image with the given ground data
        if (evaluate(*segmenter,
                     imageData_[i],
                     prevMaskData_[i],
                     maskData_[i],
                     fitness,
                     norm)) {
          accFitness.add(fitness);
          accNorm.add(norm);
          j++;
        }
        ++i;
      }
    }

    fitness.edivide(accFitness,accNorm);

    // remove the thread-scoped detector instance
    delete segmenter;

    return (j==size);
  }


  bool segmentationGenetics::evaluate(segmentation& segmenter,
                                      const image& img,
                                      const imatrix& prevStage,
                                      const channel8& mask,
                                      const segmentation::parameters& param,
                                            dvector& fitness,
                                            dvector& norm) const {

    if (segmenter.setParameters(param)) {
      return evaluate(segmenter,img,prevStage,mask,fitness,norm);
    }

    return false;
  }

  bool segmentationGenetics::evaluate(segmentation& segmenter,
                                      const image& img,
                                      const channel8& mask,
                                      const segmentation::parameters& param,
                                      dvector& fitness,
                                      dvector& norm) const {

    if (segmenter.setParameters(param)) {
      static const imatrix emptyMatrix;
      return evaluate(segmenter,img,emptyMatrix,mask,fitness,norm);
    } 
    return false;
  }

  bool segmentationGenetics::evaluate(segmentation& segmenter,
                                      const image& img,
                                      const imatrix& prevStage,
                                      const channel8& refMask,
                                            dvector& fitness,
                                            dvector& norm) const {


    timer chronos;

    imatrix mask;
    channel certainty;

    // call the segmentation method of the inherited class and measure the
    // time it takes.
    chronos.start();
    if (!segment(segmenter,img,prevStage,mask,certainty)) {
      chronos.stop();
      return false;
    }
    chronos.stop();

    const parameters& par = getParameters();

    if (evaluate(mask,refMask,fitness,norm,par.minValidRegionSize)) {
      // evaluate did't care about certainty and throughput, but we do
      fitness.at(IdxThroughput) = 1000000.0;
      norm.at(IdxThroughput) = chronos.getTime(); // segms per second

      if (certainty.empty()) {
        fitness.at(IdxPixelCertainty) = 1.0;
        norm.at(IdxPixelCertainty) = 1.0;
      } else {
        fitness.at(IdxPixelCertainty) = certainty.computeSumOfElements();
        norm.at(IdxPixelCertainty) = certainty.rows()*certainty.columns();
      }      
    }

    return true;
  }

  bool segmentationGenetics::evaluate(const imatrix& mask,
                                      const channel8& refMask,
                                            dvector& fitness,
                                            dvector& norm,
                                      const float minValidRegionSize) const {

    fitness.assign(IdxTotalFitnessDim,0.0);
    norm.assign(IdxTotalFitnessDim,0.0);

    imatrix rmask,rrefMask;

    // mask   : the result of the current parameterization/algorithm
    // refMask: ground truth

    // analyze the mask
    int potentialMax,potentialMin;
    mask.findExtremes(potentialMin,potentialMax);
    imatrix::const_iterator imit,eimit;
    int i,j,x,y;
    ivector tmpVct;

    // compute region sizes
    tmpVct.assign(1+potentialMax-potentialMin,0);
    for (imit=mask.begin(),eimit=mask.end();imit!=eimit;++imit) {
      tmpVct.at((*imit)-potentialMin)++;
    }
   
    // count number of regions
    int numRegs = 0;
    ivector equivSeg(tmpVct.size(),-1);
    for (i=0,j=0;i<tmpVct.size();++i) {
      if (tmpVct.at(i) != 0) {
        equivSeg.at(i)=j;
        numRegs++;
        j++;
      }
    }
    
    // compact all region sizes into a vector indexed from 0 to numRegs-1
    ivector regSizes(numRegs);
    for (i=0,j=0;i<tmpVct.size();++i) {
      if (tmpVct.at(i) != 0) {
        regSizes.at(j)=tmpVct.at(i);
        j++;
      }
    }
    
    // analyze the reference mask

    // compute region sizes
    channel8::const_iterator cit,ecit;

    // count number of pixels per region (since channel8 -> only 256 regs pos.)
    tmpVct.assign(256,0);
    for (cit=refMask.begin(),ecit=refMask.end();cit!=ecit;++cit) {
      tmpVct.at(*cit)++;
    }
   
    // count number of regions
    int rNumRegs = 0;
    ivector rEquivSeg(tmpVct.size(),0);
    for (i=0,j=0;i<tmpVct.size();++i) {
      if (tmpVct.at(i) != 0) {
        rEquivSeg.at(i)=j;
        rNumRegs++;
        j++;
      }
    }

    // compact the region sizes into a vector with rNumRegs elements
    ivector rRegSizes(rNumRegs);
    for (i=0,j=0;i<tmpVct.size();++i) {
      if (tmpVct.at(i) != 0) {
        rRegSizes.at(j)=tmpVct.at(i);
        j++;
      }
    }
    
    // now construct the matrix with all necessary data: It contains in each
    // row the information for a region of the evaluated segmentation many of
    // its pixels belong to each of the ground-truth regions
    //
    // info Matrix: Ref 0 | Ref 1 | ... | Ref n | 
    //              -----------------------------
    //     region 0 |  5  |  16   | ... |   2   |
    //              -----------------------------
    //     region 1 |  0  | 231   | ... |   0   |
    //              -----------------------------
    //       ...
    //              -----------------------------
    //     region m | 10  |  1    | ... |   0   |
    //              -----------------------------
    imatrix info(numRegs,rNumRegs,0);
    
    for (y=0;y<mask.rows();++y) {
      for (x=0;x<mask.columns();++x) {
        info.at(equivSeg.at(mask.at(y,x)-potentialMin),
                rEquivSeg.at(refMask.at(y,x)))++;
      }
    }

    // compute the probability of each ground-truth region
    dvector plambda;
    plambda.castFrom(rRegSizes);
    plambda.divide(refMask.rows()*refMask.columns());
    const int sizeThresh =
      iround(mask.rows()*mask.columns()*minValidRegionSize);
        
    // From info, all fitness measures can be computed:

    int osize;
    double wri;
    double tmp;
    double sumri=0;
    double sumwri=0;
    dvector sumo(rNumRegs,0.0);
    dvector sumi(rNumRegs,0.0);

    // for each region i
    for (i=0;i<info.rows();++i) {
      j = info.getRow(i).findIndexOfMaximum(); // j best object class
      tmp = info.at(i,j);  // number of pixels of i in ref.-region j
      sumo.at(j)+=tmp; // acc best object areas
      sumi.at(j)+=1.0; // count how many regions per reference region are used

      // region-wise information content
      osize = rRegSizes.at(j);
      wri = (osize > sizeThresh) ? 1.0 : 0.0;
      sumri += tmp*wri/osize;
      sumwri += wri;
    }

    double pa = 0;
    double sum = 0;

    double oa = 0.0;
    double woa = 0.0;
    double sumwoa = 0.0;

    for (i=0;i<sumo.size();++i) {
      osize = rRegSizes.at(i);
      // compute numerator and denominator of the pixel accuracy
      pa += sumo.at(i);
      sum += osize;

      // compute numerator and denominator of the object accuracy
      woa = (osize > sizeThresh) ? (1.0/plambda.at(i)) : 0;
      oa += sumo.at(i)*woa/osize;
      sumwoa +=woa;
    }

    // insert the first statistics in the results vector
    fitness.at(IdxNumRegions) = numRegs;
    norm.at(IdxNumRegions) = 1;

    fitness.at(IdxNumRegionsRecip) = 1.0/numRegs;
    norm.at(IdxNumRegionsRecip) = 1;

    fitness.at(IdxPixelAccuracy) = pa;
    norm.at(IdxPixelAccuracy) = sum;

    fitness.at(IdxObjectAccuracy) = oa;
    norm.at(IdxObjectAccuracy) = sumwoa;

    fitness.at(IdxRegionWiseInfo) = sumri;
    norm.at(IdxRegionWiseInfo) = sumwri;    

    double dx = sumi.computeSumOfElements()/sumi.size();
    static const double enorm = exp(-1.0);

    fitness.at(IdxRegionIntegrity) = dx*exp(-dx)/enorm;
    norm.at(IdxRegionIntegrity) = 1.0;

    return true;
  }

  bool segmentationGenetics::segment(segmentation& segmenter,
                                     const image& img,
                                     const imatrix& ,
                                     imatrix& mask,
                                     channel& certainty) const {
    if (!segmenter.apply(img,mask)) {
      _lti_debug("Error in segmenter: " << segmenter.getStatusString() <<
                 std::endl);
      setStatusString(segmenter.getStatusString());
      return false;
    }
    certainty.clear(); // assume no certainty computation 
    return true;
  }
}
