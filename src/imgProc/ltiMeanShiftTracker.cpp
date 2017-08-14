/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Torsten Kaemper
 *
 * Copyright (C) 2009
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
 * \file   ltiMeanShiftTracker.cpp
 *         Contains the class lti::meanShiftTracker, used to track
 *         rectangular windows in image sequences.
 * 
 * \author Torsten Kaemper
 * \date   24.9.2001
 * \author Pablo Alvarado
 * \date   12.09.2009
 *
 * revisions ..: $Id: ltiMeanShiftTracker.cpp,v 1.2 2009-09-13 23:46:03 alvarado Exp $
 */

#include "ltiMeanShiftTracker.h"
#include "ltiRound.h"
#include "ltiMath.h"

namespace lti {

  // --------------------------------------------------
  // meanShiftTracker::parameters
  // --------------------------------------------------

  // default constructor
  meanShiftTracker::parameters::parameters()
    : functor::parameters() {
    
    kernelType = Normal;
    threshold = 0.8f;
    precision = 2.0f;
    sizeAdaptRatio = 0.1f;
  }

  // copy constructor
  meanShiftTracker::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  meanShiftTracker::parameters::~parameters() {
  }

  // copy member

  meanShiftTracker::parameters&
  meanShiftTracker::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    kernelType = other.kernelType;
    threshold = other.threshold;
    precision = other.precision;
    sizeAdaptRatio = other.sizeAdaptRatio;

    return *this;
  }

  // alias for copy method
  meanShiftTracker::parameters&
  meanShiftTracker::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& meanShiftTracker::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  meanShiftTracker::parameters*
  meanShiftTracker::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  meanShiftTracker::parameters*
  meanShiftTracker::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool meanShiftTracker::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"kernelType",kernelType);
      lti::write(handler,"threshold",threshold);
      lti::write(handler,"precision",precision);
      lti::write(handler,"sizeAdaptRatio",sizeAdaptRatio);
    }

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
  bool meanShiftTracker::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"kernelType",kernelType);
      lti::read(handler,"threshold",threshold);
      lti::read(handler,"precision",precision);
      lti::read(handler,"sizeAdaptRatio",sizeAdaptRatio);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // meanShiftTracker::trackerState
  // --------------------------------------------------


  meanShiftTracker::trackerState::trackerState() {
    targetProb.resize(Dim,CellsPerDim);
    candProb.resize(Dim,CellsPerDim);

    clear();
  }

  meanShiftTracker::trackerState::trackerState(const trackerState& other)
    : ioObject() {
    copy(other);
  }

  meanShiftTracker::trackerState::~trackerState() {
  }

  void meanShiftTracker::trackerState::clear() {
    y1.set(0,0);
    bhat1=0.0f;
    distance = 0;
    hwRatio = 1;
    
    targetProb.initialize();
    candProb.initialize();
  }

  meanShiftTracker::trackerState& meanShiftTracker::trackerState::
  copy(const meanShiftTracker::trackerState& other) {
    y1=other.y1;
    bhat1=other.bhat1;
    distance = other.distance;
    hwRatio = other.hwRatio;

    targetProb.copy(other.targetProb);
    candProb.copy(other.candProb);

    return *this;
  }

  meanShiftTracker::trackerState& meanShiftTracker::trackerState::
  operator=(const meanShiftTracker::trackerState& other) {
    return copy(other);
  }

  const std::string& meanShiftTracker::trackerState::name() const {
    _LTI_RETURN_CLASS_NAME;
  }
  
  ioObject* meanShiftTracker::trackerState::clone() const {
    return new trackerState(*this);
  }
      
  ioObject* meanShiftTracker::trackerState::newInstance() const {
    return new trackerState;
  }

  /**
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool meanShiftTracker::trackerState::write(ioHandler& handler,
                                             const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"y1",y1);
      lti::write(handler,"bhat1",bhat1);
      lti::write(handler,"targetProb",targetProb);
      lti::write(handler,"candProb",candProb);
      lti::write(handler,"distance",distance);
      lti::write(handler,"hwRatio",hwRatio);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }
  
  /**
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool meanShiftTracker::trackerState::read(ioHandler& handler,
                                            const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"y1",y1);
      lti::read(handler,"bhat1",bhat1);
      lti::read(handler,"targetProb",targetProb);
      lti::read(handler,"candProb",candProb);
      lti::read(handler,"distance",distance);
      lti::read(handler,"hwRatio",hwRatio);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // meanShiftTracker
  // --------------------------------------------------

  // default constructor
  meanShiftTracker::meanShiftTracker()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  meanShiftTracker::meanShiftTracker(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  meanShiftTracker::meanShiftTracker(const meanShiftTracker& other)
    : functor() {
    copy(other);
  }

  // destructor
  meanShiftTracker::~meanShiftTracker() {
  }

  // copy member
  meanShiftTracker& 
  meanShiftTracker::copy(const meanShiftTracker& other) {
    functor::copy(other);

    td_.copy(other.td_);
    initialized_ = other.initialized_;
    valid_ = other.valid_;

    return (*this);
  }

  // alias for copy member
  meanShiftTracker&
  meanShiftTracker::operator=(const meanShiftTracker& other) {
    return (copy(other));
  }

  // class name
  const std::string& meanShiftTracker::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  meanShiftTracker* meanShiftTracker::clone() const {
    return new meanShiftTracker(*this);
  }

  // create a new instance
  meanShiftTracker* meanShiftTracker::newInstance() const {
    return new meanShiftTracker();
  }

  // return parameters
  const meanShiftTracker::parameters&
  meanShiftTracker::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool meanShiftTracker::updateParameters() {
    const parameters& param = getParameters();

    kernelType_ = param.kernelType;
    
    return true;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool meanShiftTracker::write(ioHandler& handler,
                               const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"state",td_);
      lti::write(handler,"initialized",initialized_);
      lti::write(handler,"valid_",valid_);
    }

    b = b && functor::write(handler,false);

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
  bool meanShiftTracker::read(ioHandler& handler,
                              const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"state",td_);
      lti::read(handler,"initialized",initialized_);
      lti::read(handler,"valid_",valid_);
    }

    b = b && functor::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------
  // The apply() and related member functions
  // -------------------------------------------------------------------
 
  void meanShiftTracker::calcProb(fhistogram& prob,
                                  const irectangle& canvas,
                                  const image& src,
                                  const channel8& mask) const {
    // fills the histogram "prob" with the colors inside the rectangle "canvas"
    // in the image. Each color is weighted with a kernel function AND with the
    // given mask. 
    // 
    // Note: the histogramm is not normalized to 1, thus use
    // prob.getProbability() to obtain probabilities.
    prob.initialize();

    vector<int> idx(3,0);
    const ipoint dim=canvas.getDimensions();
    const ipoint radius(dim.x/2,dim.y/2);
    const ipoint canvasCenter(canvas.getCenter());
    const ipoint limit(min(src.lastColumn(),canvas.br.x),
                       min(src.lastRow(),canvas.br.y));

    for (int row=max(0,canvas.ul.y);row<=limit.y; ++row) {   //row (y)
      for (int col=max(0,canvas.ul.x);col<limit.x; ++col) {  //column (x)

        const ipoint position(col,row);
        const ubyte weight = mask.at(row,col);
        if (weight) {

          const rgbaPixel& pixel=src.at(row,col);

          idx.at(0)=pixel.getRed()*CellsPerDim/256;
          idx.at(1)=pixel.getGreen()*CellsPerDim/256;
          idx.at(2)=pixel.getBlue()*CellsPerDim/256;

          const float kern = kernel(canvasCenter,position,radius)*weight;
          prob.put(idx,kern);
        }
      }
    }
  }

  void meanShiftTracker::calcProb(fhistogram& prob,
                                  const irectangle& canvas,
                                  const image& src,
                                  const channel& mask) const {
    // fills the histogram "prob" with the colors inside the rectangle "canvas"
    // in the image. Each color is weighted with a kernel function AND with the
    // given mask.
    //
    // Note: the histogramm is not normalized to 1, thus use 
    // prob.getProbability() to obtain probabilities.
    prob.initialize();

    vector<int> idx(3,0);
    const ipoint dim=canvas.getDimensions();
    const ipoint radius(dim.x/2,dim.y/2);
    const ipoint canvasCenter(canvas.getCenter());
    const ipoint limit(min(src.lastColumn(),canvas.br.x),
                       min(src.lastRow(),canvas.br.y));

    for (int row=max(0,canvas.ul.y);row<limit.y; ++row) {   //row (y)
      for (int col=max(0,canvas.ul.x);col<limit.x; ++col) {  //column (x)

        const ipoint position(col,row);
        const float weight = mask.at(row,col);
        if (weight!=0.0f) {

          const rgbaPixel& pixel = src.at(row,col);

          idx.at(0)=pixel.getRed()*CellsPerDim/256;
          idx.at(1)=pixel.getGreen()*CellsPerDim/256;
          idx.at(2)=pixel.getBlue()*CellsPerDim/256;

          const float kern = kernel(canvasCenter,position,radius)*weight;
          prob.put(idx,kern);
        }
      }
    }
  }


  void meanShiftTracker::calcProb(fhistogram& prob,
                                  const irectangle& canvas,
                                  const image& src) const {
    // fills the histogram "prob" with the colors inside the
    // rectangle "canvas" in the image. Each color is weighted
    // with a kernel function. Note: the histogramm is not
    // normalized to 1, thus use prob.getProbability() to obtain
    // probabilities.
    prob.initialize();

    vector<int> idx(3,0);
    const ipoint dim=canvas.getDimensions();
    const ipoint radius(dim.x/2,dim.y/2);
    const ipoint canvasCenter(canvas.getCenter());
    const ipoint limit(min(src.lastColumn(),canvas.br.x),
                       min(src.lastRow(),canvas.br.y));

    for (int row=max(0,canvas.ul.y);row<limit.y; ++row) {   //row (y)
      for (int col=max(0,canvas.ul.x);col<limit.x; ++col) {  //column (x)

        const ipoint position(col,row);
        const rgbaPixel& pixel=src.at(row,col);

        idx.at(0)=pixel.getRed()*CellsPerDim/256;
        idx.at(1)=pixel.getGreen()*CellsPerDim/256;
        idx.at(2)=pixel.getBlue()*CellsPerDim/256;

        const float kern = kernel(canvasCenter,position,radius);
        prob.put(idx,kern);
      }
    }
  }

  float meanShiftTracker::kernel(const ipoint& cen,
                                 const ipoint& pos,
                                 const ipoint& h) const {

    const float temp1 = sqr(static_cast<float>(cen.x-pos.x)/h.x);
    const float temp2 = sqr(static_cast<float>(cen.y-pos.y)/h.y);
    const float ksi = temp1 + temp2;

    // Normal Kernel
    switch(kernelType_) {
    case Normal:
      return exp(-0.5f*ksi);
      break;
    case Epanechnikov:
      return (ksi <= 1.0f) ? (1.0f-ksi) : 0.0f;
      break;
    default:
      return 0.0f;
    }

    return 0.0f;
  }

  float meanShiftTracker::derivateKernel(const ipoint& cen,
                                         const ipoint& pos,
                                         const ipoint& h) const {

    const float temp1 = sqr(static_cast<float>(cen.x-pos.x)/h.x);
    const float temp2 = sqr(static_cast<float>(cen.y-pos.y)/h.y);
    const float ksi = temp1 + temp2;

    switch(kernelType_) {
    case Normal:
      // Normal Kernel
      return 0.5f*exp(-0.5f*ksi);
      break;
    case Epanechnikov:
      // Epanechnikov Kernel
      return (ksi <= 1.0f) ? 1.0f : 0.0f;
      break;
    default:
      return 0.0f;
    }

    return 0.0f;
  }

  void meanShiftTracker::initialize(const image& src,
                                    const channel8& mask,
                                    irectangle& window) {

    td_.y1=fpoint(static_cast<float>(window.getDimensions().x)/2.0f,
                  static_cast<float>(window.getDimensions().y)/2.0f);
    td_.bhat1=0.0;
    td_.distance=0;
    calcProb(td_.targetProb,window,src,mask);
    td_.candProb.initialize();
    td_.hwRatio = static_cast<float>(window.getDimensions().y)/
                                     window.getDimensions().x;

    initialized_=true;
    valid_=false;
  }

  void meanShiftTracker::initialize(const image& src,
                                    const channel& mask,
                                    irectangle& window) {
    td_.y1=fpoint(static_cast<float>(window.getDimensions().x)/2.0f,
                  static_cast<float>(window.getDimensions().y)/2.0f);
    td_.bhat1=0.0f;
    td_.distance=0;
    calcProb(td_.targetProb,window,src,mask);
    td_.candProb.initialize();
    td_.hwRatio = static_cast<float>(window.getDimensions().y)/
                                     window.getDimensions().x;

    initialized_=true;
    valid_=false;
  }

  void meanShiftTracker::initialize(const image& src,irectangle& window) {
    td_.y1=fpoint(static_cast<float>(window.getDimensions().x)/2.0f,
                  static_cast<float>(window.getDimensions().y)/2.0f);
    td_.bhat1=0.0f;
    td_.distance=0;
    calcProb(td_.targetProb,window,src);
    td_.candProb.initialize();
    td_.hwRatio = static_cast<float>(window.getDimensions().y)/
                                     window.getDimensions().x;

    initialized_=true;
    valid_=false;
  }

  float meanShiftTracker::calcBhatCoef(const fhistogram& targetProb,
                                       const fhistogram& candProb) const {
    // calculates the bhattacharyya metric for measuring
    // similarity of two color distributions
    const float entries = targetProb.getNumberOfEntries()*
                          candProb.getNumberOfEntries();

    if (entries <= 0.0f) {
      return 0.0f;
    }

    float bhat=0.0f;
    fhistogram::const_iterator it1,it2;
    const fhistogram::const_iterator eit2=candProb.end();
    for (it1=targetProb.begin(),it2=candProb.begin();
         it2!=eit2;
         ++it1,++it2) {
      bhat += sqrt( (*it1)*(*it2) );
    }
    return bhat/sqrt(entries);
  }

  inline float meanShiftTracker::calcDistance(const fhistogram& targetProb,
                                       const fhistogram& candProb) const {
    // calculate the color distribution distance between two histogramms
    return (1.0f-calcBhatCoef(targetProb,candProb));
  }

  void meanShiftTracker::correctRect(irectangle& rect,
                                     const irectangle& canvas) const {
    int diff = rect.ul.x - canvas.ul.x;
    // relocate box inside canvas
    if (diff < 0) {
      rect.ul.x -= diff;
      rect.br.x -= diff;
    }
    diff = rect.ul.y - canvas.ul.y;
    if (diff < 0) {
      rect.ul.y -= diff;
      rect.br.y -= diff;
    }
    diff = canvas.br.x - rect.br.x;
    if (diff < 0) {
      rect.ul.x += diff;
      rect.br.x += diff;
    }
    diff = canvas.br.y - rect.br.y;
    if (diff < 0) {
      rect.ul.y += diff;
      rect.br.y += diff;
    }
    // if still outside, clip length
    if (rect.ul.x < canvas.ul.x) {
      rect.ul.x = canvas.ul.x;
    }
    if (rect.ul.y < canvas.ul.y) {
      rect.ul.y = canvas.ul.y;
    }
    if (rect.br.x > canvas.br.x) {
      rect.br.x = canvas.br.x;
    }
    if (rect.br.y > canvas.br.y) {
      rect.br.y = canvas.br.y;
    }
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On copy apply for type image!
  bool meanShiftTracker::apply(const image& src, irectangle& window){

    // get parameters
    const parameters& param = getParameters();

    ipoint newCenter;
    irectangle origWindow = window;

    // the image canvas (needed for correctRect())
    const irectangle canvas(0,0,src.lastColumn(),src.lastRow());

    // make sure, that the window is inside the image
    correctRect(window,canvas);

    // initialize the tracker and return
    if (!initialized_) {
      initialize(src,window);
      return true;
    }

    fhistogram tempHist(Dim,CellsPerDim);

    //local distance between y0 and y1
    fpoint y0;
    y0.x=static_cast<float>(window.br.x + window.ul.x)/2;
    y0.y=static_cast<float>(window.br.y + window.ul.y)/2;
    float dist = 0;
    float lastDist;
    td_.y1 = y0;

    ivector idx(3,0);
    float bhat0;
    // begin iterations of mean-shift algorithm
    do {
      // step 1: initialize the location of the target in the current
      // frame with y0, compute the distribution and evaluate the
      // bhattacharrya metric
      y0=td_.y1;

      calcProb(tempHist,window,src);
      bhat0=calcBhatCoef(td_.targetProb,tempHist);

      // step 2: derive the weights {wi}
      // is done inside the loop!

      // step 3: based on the mean-shift vector, derive the new location
      // of the target
      // denominator for y1 calculation
      float denominator=0.0;
      // numerator for y1 calculation
      fpoint numerator(0,0);

      // radius of distribution
      ipoint radius, position;
      radius.x=window.getDimensions().x/2;
      radius.y=window.getDimensions().y/2;

      //weight
      float wi;

      for (int row=window.ul.y;row<window.br.y; row++) {    //zeile(y),
        for (int col=window.ul.x;col<window.br.x; col++) {  //spalte(x)
          position.x=col;
          position.y=row;
          const rgbaPixel& pixel=src.at(row,col);
          idx.at(0)=pixel.getRed()*CellsPerDim/256;
          idx.at(1)=pixel.getGreen()*CellsPerDim/256;
          idx.at(2)=pixel.getBlue()*CellsPerDim/256;
          //reads the weights in histogram for the color at position (row,col)
          const float temp1 =
            static_cast<float>(td_.targetProb.getProbability(idx));
          const float temp2 = static_cast<float>(tempHist.getProbability(idx));
          if (temp2>0.0f) {
            wi=sqrt(temp1/temp2);
            const float kern =
              derivateKernel(window.getCenter(),position,radius);
            // numerator
            numerator.x += wi*kern*position.x;
            numerator.y += wi*kern*position.y;
            // denominator
            denominator += wi*kern;
          }
        }
      }
      if(denominator!=0.0){
        td_.y1.x=numerator.x/denominator;
        td_.y1.y=numerator.y/denominator;
      }
      else{
        return false;
      }

      // update and evaluate at new position y1
      newCenter.x=iround(td_.y1.x);
      newCenter.y=iround(td_.y1.y);
      window.setCenter(newCenter);
      correctRect(window, canvas);
      //calculates the histogram and the bhattacharrya coeff.
      // for new window position y1
      calcProb(td_.candProb,window,src);
      td_.bhat1=calcBhatCoef(td_.targetProb,td_.candProb);

      // step 4:
      float lastBhat = 0;
      while ((td_.bhat1<bhat0)&&(fabs(lastBhat-td_.bhat1)>0.01)) {
        td_.y1.x=0.5f*(y0.x+td_.y1.x);
        td_.y1.y=0.5f*(y0.y+td_.y1.y);

        newCenter.x=iround(td_.y1.x);
        newCenter.y=iround(td_.y1.y);
        window.setCenter(newCenter);
        correctRect(window, canvas);
        calcProb(td_.candProb,window,src);
        lastBhat = td_.bhat1;
        td_.bhat1=calcBhatCoef(td_.targetProb,td_.candProb);
      }

      lastDist=dist;
      dist=sqrt(td_.y1.distanceSqr(y0));

      //back to Step1
    } while((dist>=param.precision) && (abs(lastDist-dist)>0.1f));// FIXME 0.1?
    

    // store distance
    td_.distance=sqrt(1.0f-td_.bhat1);
    
    // scale adaptation
    if (param.sizeAdaptRatio>0.0) {
      fhistogram tempHist2(Dim,CellsPerDim);
      // calculates the color distribution distances for original-size,
      // +10% and -10%
      float sDistance,lDistance;
      irectangle sWindow,lWindow;
      ipoint origDim=origWindow.getDimensions();

      //+ param.sizeAdaptRatio*100%
      ipoint largerDim;
      largerDim.x=iround(origDim.x*(1+param.sizeAdaptRatio));
      largerDim.y=iround(largerDim.x*td_.hwRatio);
      //calculates the distance for larger window-size
      lWindow.resize(largerDim);
      lWindow.setCenter(newCenter);
      correctRect(lWindow,canvas);
      calcProb(tempHist,lWindow,src);
      lDistance=calcDistance(td_.targetProb,tempHist);

      //- param.sizeAdaptRatio*100%
      ipoint smallerDim;
      smallerDim.x=iround(origDim.x*(1-param.sizeAdaptRatio));
      smallerDim.y=iround(smallerDim.x*td_.hwRatio);
      //calculates the distance for smaller window-size
      sWindow.resize(smallerDim);
      sWindow.setCenter(newCenter);
      correctRect(sWindow,canvas);
      calcProb(tempHist2,sWindow,src);
      sDistance=calcDistance(td_.targetProb,tempHist2);

      // see if larger or smaller window fits better
      if ( (sDistance<td_.distance) || (lDistance<td_.distance) ) {
        if ( lDistance<=sDistance ) {
          window = lWindow;
          td_.distance=lDistance;
          tempHist.detach(td_.candProb);
        }
        else if ( sDistance<lDistance ) {
          window = sWindow;
          td_.distance=sDistance;
          tempHist2.detach(td_.candProb);
        }
      }
    }


    // see if result is valid
    if(td_.distance>param.threshold){
      window = origWindow;
      valid_ = false;
    }
    else {
      valid_ = true;
    }

    return true;
  }

  bool meanShiftTracker::isInitialized() const {
    return initialized_;
  }

  void meanShiftTracker::reset() {
    td_.clear();
    initialized_=false;
    valid_=false;
  }

  bool meanShiftTracker::isValid() const {
    return valid_;
  }

  float meanShiftTracker::getDistance() const {
    return td_.distance;
  }

  fpoint meanShiftTracker::getCenter() const {
    return td_.y1;
  }

  const fhistogram& meanShiftTracker::getTargetHistogram() const {
    return td_.targetProb;
  }

  const fhistogram& meanShiftTracker::getCandidateHistogram() const {
    return td_.candProb;
  }



  // read function for eCoordinateSystem.
  bool read(ioHandler& handler,meanShiftTracker::eKernelType& data) {

    std::string str;
    if (handler.read(str)) {
   
      if (str.find("ormal") != std::string::npos) {
        data = meanShiftTracker::Normal;
      } else if (str.find("pane") != std::string::npos)  {
        data = meanShiftTracker::Epanechnikov;
      } else {
        data = meanShiftTracker::Normal;
        handler.setStatusString("undefined meanShiftTracker::eKernelType");
        return false;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eCoordinateSystem.
  bool write(ioHandler& handler,const meanShiftTracker::eKernelType& data) {
    bool b=false;
    switch(data) {
    case meanShiftTracker::Normal:
      b=handler.write("Normal");
      break;
    case meanShiftTracker::Epanechnikov:
      b=handler.write("Epanechnikov");
      break;
    default:
      b=false;
      handler.setStatusString("undefined meanShiftTracker::eKernelType");
      handler.write("Unknown");
    }

    return b;
  }


}

