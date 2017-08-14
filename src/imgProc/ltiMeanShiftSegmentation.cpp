/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiMeanShiftSegmentation.cpp
 *         Contains the class lti::meanShiftSegmentation, 
 *         which implements the well known image segmentation algorithm.
 * \author Axel Berner
 * \author Jens Rietzschel
 * \author Pablo Alvarado
 * \date   22.6.2001
 *
 * revisions ..: $Id: ltiMeanShiftSegmentation.cpp,v 1.3 2012-01-03 02:22:25 alvarado Exp $
 */

#include "ltiMeanShiftSegmentation.h"
#include "ltiSTLIoInterface.h"
#include "ltiRound.h"
#include <ctime> 
#include <cstring> // for memcpy and similar functions

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include <iomanip>
#endif

namespace lti {

  // --------------------------------------------------
  // meanShiftSegmentation::parameters
  // --------------------------------------------------

  // default constructor
  meanShiftSegmentation::parameters::parameters()
    : segmentation::parameters() {

    maxTrial = 10;
    multivariateNormalKernel=false;
    speedup = MediumSpeedup;
    sigmaS = 5;
    sigmaR = 5;
    maxNeighbourColorDistance = 3;
    minRegionSize = 15;
    thresholdConverged = 0.1;
  }

  // copy constructor
  meanShiftSegmentation::parameters::parameters(const parameters& other)
    : segmentation::parameters()  {
    copy(other);
  }

  // destructor
  meanShiftSegmentation::parameters::~parameters() {
  }

  // get type name
  const std::string& meanShiftSegmentation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  meanShiftSegmentation::parameters&
  meanShiftSegmentation::parameters::copy(const parameters& other) {
    segmentation::parameters::copy(other);

    maxTrial = other.maxTrial;
    multivariateNormalKernel=other.multivariateNormalKernel;
    speedup = other.speedup;
    sigmaS=other.sigmaS;
    sigmaR=other.sigmaR;
    maxNeighbourColorDistance = other.maxNeighbourColorDistance;
    minRegionSize = other.minRegionSize;
    thresholdConverged=other.thresholdConverged;

    return *this;
  }

  // alias for copy member
  meanShiftSegmentation::parameters&
  meanShiftSegmentation::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  meanShiftSegmentation::parameters*
  meanShiftSegmentation::parameters::clone() const {
    return new parameters(*this);
  }

  meanShiftSegmentation::parameters*
  meanShiftSegmentation::parameters::newInstance() const {
    return new parameters;
  }

  bool meanShiftSegmentation::parameters::write(ioHandler& handler,
                                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {      
      lti::write(handler,"maxTrial",maxTrial);
      lti::write(handler,"multivariateNormalKernel",multivariateNormalKernel);
      lti::write(handler,"speedup",speedup);
      lti::write(handler,"sigmaS",sigmaS);
      lti::write(handler,"sigmaR",sigmaR);
      lti::write(handler,"maxNeighbourColorDistance",
                 maxNeighbourColorDistance);
      lti::write(handler,"minRegionSize",minRegionSize);
      lti::write(handler,"thresholdConverged",thresholdConverged);
    }

    b = b && segmentation::parameters::write(handler,false);

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
   * @return true if write was successful
   */
  bool meanShiftSegmentation::parameters::read(ioHandler& handler,
                                               const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      std::string str;

      b = lti::read(handler,"maxTrial",maxTrial) && b;
      b = lti::read(handler,"multivariateNormalKernel",
                multivariateNormalKernel) && b;
      b = lti::read(handler,"speedup",speedup) && b;
      b = lti::read(handler,"sigmaS",sigmaS) && b;
      b = lti::read(handler,"sigmaR",sigmaR) && b;
      b = lti::read(handler,"maxNeighbourColorDistance",
                    maxNeighbourColorDistance) && b;     
      b = lti::read(handler,"minRegionSize",minRegionSize) && b;
      b = lti::read(handler,"thresholdConverged",thresholdConverged) && b;
    }

    b = b && segmentation::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // meanShiftSegmentation
  // --------------------------------------------------

  // default constructor
  meanShiftSegmentation::meanShiftSegmentation()
    : segmentation() {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  meanShiftSegmentation::meanShiftSegmentation(const parameters& par)
    : segmentation() {

    // set the default parameters
    setParameters(par);
  }

  // copy constructor
  meanShiftSegmentation::meanShiftSegmentation(const 
                                               meanShiftSegmentation& other)
    : segmentation() {
    copy(other);
  }

  // destructor
  meanShiftSegmentation::~meanShiftSegmentation() {
  }

  // returns the name of this type
  const std::string& meanShiftSegmentation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  meanShiftSegmentation&
  meanShiftSegmentation::copy(const meanShiftSegmentation& other) {
    segmentation::copy(other);
    return (*this);
  }

  // clone member
  meanShiftSegmentation* meanShiftSegmentation::clone() const {
    return new meanShiftSegmentation(*this);
  }

  // clone member
  meanShiftSegmentation* meanShiftSegmentation::newInstance() const {
    return new meanShiftSegmentation(*this);
  }

  // return parameters
  const meanShiftSegmentation::parameters&
  meanShiftSegmentation::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  void range(const int a, int& x, const int b) {
    if(x < a)
      x = a;
    else if(x > b)
      x = b;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool meanShiftSegmentation::apply(const image& src, image& dest) const {

    image imgFiltered;
    palette colorMap;
    imatrix labels;
    return apply(src,imgFiltered,dest,labels,colorMap);
    
  }

  bool meanShiftSegmentation::apply(const image& src,imatrix& dest) const {
    image imgFiltered,imgSegmented;
    palette colorMap;
    return apply(src,imgFiltered,imgSegmented,dest,colorMap);
  }


  bool meanShiftSegmentation::apply(const image& src,
                                    imatrix& labels,
                                    palette& colorMap) const {

    image imgFiltered,imgSegmented;
    return apply(src,imgFiltered,imgSegmented,labels,colorMap);    
  }
 
  bool meanShiftSegmentation::apply(const channel8& chnl1,
                                    const channel8& chnl2,
                                    const channel8& chnl3,
                                    imatrix& dest) const {
    int i;

    if ((chnl1.empty()) ||
        ((chnl1.size() != chnl2.size()) ||
         (chnl2.size() != chnl3.size()) ||
         (chnl3.size() != chnl1.size()))) {
      setStatusString("Incompatible channel sizes");
      dest.clear();
      return false;
    }
    
    // get parameters
    const parameters& param = getParameters();
    internals data;
    
    // determine dimensions
    int dimensionRange = 3;
    int dimensionSpace = 2;

    dest.assign(chnl1.size(),0);
    
    // initialize members and allocate memory
    data.initialize(param,chnl1.size(),dimensionRange,dimensionSpace);

    // copy the three channels into one array
    for(i = 0; i < data.imageSize; i++){
      data.imageLuvOrgF[i*data.dimensionRange]   = float(chnl1.elem(i));
      data.imageLuvOrgF[i*data.dimensionRange+1] = float(chnl2.elem(i));
      data.imageLuvOrgF[i*data.dimensionRange+2] = float(chnl3.elem(i));
    }

    // Filter image 
    if (param.speedup == NoSpeedup) {
      nonOptimizedFilter(data);
    }
    else {
      optimizedFilter(data);
    }
    
    // connect neighbour pixel with the same color to a region
    connect(data);

    // fuse regions that have similar colors (difference less than sigmaS)
    fuseRegions(data);

    // prune small regions that have less than minRegionSize pixels
    pruneRegions(data);

    // store result
    for(i = 0; i < data.imageSize ; i++){
      dest.elem(i) = data.labels[i];
    }
   
    return true;
  }

    
  /*
   * Newer Mean-Shift implementation
   */
  bool meanShiftSegmentation::apply(const image& src, 
                                    image& destFiltered, 
                                    image& destSegmented,
                                    imatrix& destLabels,
                                    palette& destColorMap) const {

    // get parameters
    const parameters& param = getParameters();
    int i;
    
    // determine dimensions
    const int dimensionRange = 3;
    const int dimensionSpace = 2;
 
    internals data;

    // initialize members and allocate memory
    data.initialize(param,src.size(),dimensionRange,dimensionSpace);

    destFiltered.assign(src.size(), rgbaPixel(0,0,0));
    destSegmented.assign(src.size(), rgbaPixel(0,0,0));
    destLabels.assign(src.size(), 0);

    // convert image from rgb to luv (imageLuvOrgF)
    rgbToLuv(src,data);

    // Filter image 
    if (param.speedup == NoSpeedup){
      nonOptimizedFilter(data);
    } else {
      optimizedFilter(data);
    }

    // connect neighbour pixel with the same color to a region
    connect(data);

    // fuse regions that have similar colors (difference less than sigmaS)
    fuseRegions(data);

    // prune small regions that have less than minRegionSize pixels
    pruneRegions(data);

    // convert filtered image from luv to rgb
    luvToRgb(data.imageLuvFilteredF, destFiltered,data);
    
    // convert luv palette (modes) into rgb palette (colorMap)
    destColorMap.resize(data.regionCount);
    luvPixel luvColor; 
    rgbaPixel rgbColor;
    int base = 0;
    for (i = 0; i < data.regionCount; i++){
      luvColor.l = data.modes[base];
      luvColor.u = data.modes[base+1];
      luvColor.v = data.modes[base+2];
      base+=data.dimensionRange;
      luvToRgb(luvColor, rgbColor);
      destColorMap.at(i) = rgbColor;
    }

    // copy labels into destLabels and store segmented image
    for(i = 0; i < data.imageSize ; i++){
      destLabels.elem(i) = data.labels[i];
      destSegmented.elem(i) = destColorMap.at(data.labels[i]);
    }
    
    return true;
  }

  //--------------------------------------------------------------------------
  // implementation of new algorithm similar to EDISON
  //--------------------------------------------------------------------------
  bool meanShiftSegmentation::filter(const image& src, image& dest) const {
    
    //get parameters
    const parameters& param = getParameters();

    //determine dimensions
    const int dimensionRange = 3;
    const int dimensionSpace = 2;

    internals data;
    dest.assign(src.size(),rgbaPixel(0,0,0));
    
    //initialize members and allocate memory
    data.initialize(param,src.size(),dimensionRange,dimensionSpace);

    //convert image from rgb to luv (imageLuvOrgF)
    rgbToLuv(src,data);

    //Filter image 
    if (param.speedup == NoSpeedup){
      nonOptimizedFilter(data);
    } else {
      optimizedFilter(data);
    }
    
    //convert image from luv to rgb 
    luvToRgb(data.imageLuvFilteredF,dest,data);

    return true;
  }

  bool meanShiftSegmentation::filter(image& srcdest) const {
    
    image img;
    if (filter(srcdest,img)) {
      img.detach(srcdest);
      return true;
    }
    return false;
  }

  meanShiftSegmentation::internals::internals() {
    width=height=imageSize=regionCount=0;
    dimensionRange=dimensionSpace=dimensionFeatureSpace=0;
    imageLuvOrgF=imageLuvFilteredF=weightMap=0;
    imageLuvFilteredI=labels=0;
  }

  meanShiftSegmentation::internals::~internals() {
    freeMemory();
  }

  void  meanShiftSegmentation::internals::initialize(const parameters& pars,
                                                     const ipoint& size,
                                                     const int dr,
                                                     const int ds) {
    freeMemory();

    int i;

    height = size.y;
    width  = size.x;

    dimensionRange = dr;
    dimensionSpace = ds;
    dimensionFeatureSpace = dimensionRange + dimensionSpace;

    imageSize = height * width;
    modePointCounts.assign(imageSize,0);
    imageLuvFilteredI = new int[imageSize*dimensionRange];
    imageLuvFilteredF = new float[imageSize*dimensionRange];
    imageLuvOrgF = new float[imageSize*dimensionRange];
    labels = new int[imageSize];
    modes.assign((imageSize+1)*dimensionRange,0.0f);
    weightMap = new float[imageSize];
    for(i = 0; i < imageSize; i++){
      weightMap[i] = 1.0;
    }
    vecTrialsToConverge.resize(pars.maxTrial,0);
  }


  void  meanShiftSegmentation::internals::freeMemory(){  
    delete[] imageLuvFilteredI;
    imageLuvFilteredI=0;
    delete[] imageLuvFilteredF;
    imageLuvFilteredF=0;
    delete[] imageLuvOrgF;
    imageLuvOrgF=0;
    delete[] labels;
    labels=0;
    modes.clear();
    delete[] weightMap;
    weightMap=0;
   }


  void  meanShiftSegmentation::nonOptimizedFilter(internals& data) const {

    const parameters& param = getParameters();

    float center[5], newCenter[5]; // , meanshiftVector[5];
    int lowerBoundX,lowerBoundY,upperBoundX,upperBoundY;
    int pos, pos1, pos2;
    float msAbs = 0; //magnitude of meanshift vector
    float weightSum, sqrRadius = 0;
    int i,j;

    //initialize vectors
    for(i = 0; i < data.dimensionFeatureSpace; i++){
      center[i] = 0.0f;
      newCenter[i] = 0.0f;
      // meanshiftVector[i] = 0.0f;
    } 

    //traverse image
    for(i = 0; i < data.imageSize; i++) {

      // assign window center 
      center[0] = static_cast<float>(i%data.width); //column x
      center[1] = static_cast<float>(i/data.width); //row y

      const int drxi = data.dimensionRange*i;

      for(j = 0; j < data.dimensionRange; j++){
        center[j+2] = data.imageLuvOrgF[drxi+j];//luv colors
      }
  
      int iterationCount = 0;
      
      //determine by this loop the new center by following the meanshift vector
      do{
        //determine lower and upper bounds on grid
        lowerBoundX = int(center[0] - param.sigmaS);
        if (lowerBoundX < 0) {
          lowerBoundX = 0;
        }

        lowerBoundY = int(center[1] - param.sigmaS);
        if (lowerBoundY < 0) {
          lowerBoundY = 0; 
        }

        upperBoundX = int(center[0] + param.sigmaS);
        if (upperBoundX >= data.width) {
          upperBoundX = data.width - 1;
        }

        upperBoundY = int(center[1] + param.sigmaS);
        if (upperBoundY >= data.height) {
          upperBoundY = data.height - 1;
        }

        for (j = 0; j < data.dimensionFeatureSpace; j++) {
          newCenter[j] = 0;
        }
        weightSum = 0;

        //iterate once through a window of size sigmaS
        for(int y = lowerBoundY; y <= upperBoundY; y++) {
          const int yw = y*data.width;
          for(int x = lowerBoundX; x <= upperBoundX; x++) {

            pos1 = (yw+x);
            pos2 = pos1*data.dimensionRange;

            //test if feature point is within sphere of radius sigmaS / sigmaR
            sqrRadius += static_cast<float>(sqr((x - center[0])/param.sigmaS));
            sqrRadius += static_cast<float>(sqr((y - center[1])/param.sigmaS));
            sqrRadius += static_cast<float>(sqr((data.imageLuvOrgF[pos2]
                                                 - center[2]) / param.sigmaR));
            sqrRadius += static_cast<float>(sqr((data.imageLuvOrgF[pos2+1]
                                                 - center[3]) / param.sigmaR));
            sqrRadius += static_cast<float>(sqr((data.imageLuvOrgF[pos2+2]
                                                 - center[4]) / param.sigmaR));
    
            // considered point is within sphere => accumulate to mean
            // for newCenter
            if (sqrRadius < 1.0) {
              newCenter[0] += data.weightMap[pos1] * x;
              newCenter[1] += data.weightMap[pos1] * y;
              newCenter[2] += data.weightMap[pos1] * data.imageLuvOrgF[pos2];
              newCenter[3] += data.weightMap[pos1] * data.imageLuvOrgF[pos2+1];
              newCenter[4] += data.weightMap[pos1] * data.imageLuvOrgF[pos2+2];
              weightSum    += data.weightMap[pos1];
            }

            sqrRadius = 0;
          }
        }
  
        msAbs = 0;
  
        //determine the new center and the magnitude of the meanshift vector
        //meanshiftVector = newCenter - center;
        for(j = 0; j < data.dimensionFeatureSpace; j++){
          newCenter[j] /= weightSum;
          msAbs += sqr(newCenter[j] - center[j]);
          center[j] = newCenter[j];
        }

        iterationCount++;
      } while( msAbs > param.thresholdConverged && 
               iterationCount < param.maxTrial );
      //shifting window

      //for debug only
      data.vecTrialsToConverge[iterationCount-1]++;

      //store result
      pos = i*data.dimensionRange;
      data.imageLuvFilteredF[pos] = center[2];
      data.imageLuvFilteredF[pos+1] = center[3];
      data.imageLuvFilteredF[pos+2] = center[4];

      //store rounded result (faster than round())
      if (center[2] < 0) {
        data.imageLuvFilteredI[pos] = int(center[2] - 0.5f); 
      } else {
        data.imageLuvFilteredI[pos] = int(center[2] + 0.5f);
      }

      if (center[3] < 0) {
        data.imageLuvFilteredI[pos+1] = int(center[3] - 0.5f); 
      } else {
        data.imageLuvFilteredI[pos+1] = int(center[3] + 0.5f);
      }

      if (center[4] < 0) {
        data.imageLuvFilteredI[pos+2] = int(center[4] - 0.5f); 
      } else {
        data.imageLuvFilteredI[pos+2] = int(center[4] + 0.5f);
      }
    }
  }
  
  void  meanShiftSegmentation::optimizedFilter(internals& data) const {

    const parameters& param = getParameters();

    float center[5], newCenter[5]; //, meanshiftVector[5];
    float diff = 0, msAbs = 0, weightSum = 0, sqrRadius = 0;
    int lowerBoundX, lowerBoundY, upperBoundX, upperBoundY;
    int pos, pos1, pos2, pointCount;
    int modeCandidateX, modeCandidateY, modeCandidate_i;
    int i,j;
    
    // one entry for each pixel: 
    // 0 means no mode yet assigned
    // 1 means mode already assigned  
    // 2 means the mode another pixel is now converging to should be assigned
    //        to this one too once the mode is determined
    int * modeTable = new int[data.imageSize];

    // stores the mode candidates that get the same mode assigned
    int * pointList = new int[data.imageSize]; 

    // the luv color of a mode candidate
    float* modeCandidatePoint = new float[data.dimensionRange];

    // initialize with 0
    memset(modeTable,0,data.imageSize*sizeof(int));
    memset(pointList,0,data.imageSize*sizeof(int));
    
    // initialize vectors
    for(j = 0; j < data.dimensionFeatureSpace; j++){
      center[j] = 0.0;
      newCenter[j] = 0.0;
      // meanshiftVector[j] = 0.0;
    } 


    // traverse image
    for(i = 0; i < data.imageSize; i++) {

      // if a mode is already assigned to this data point then skip this 
      // point, otherwise proceed to find its mode by applying mean shift
      if (modeTable[i] == 1){
        continue;
      }
      pointCount = 0;
   
      // assign window center 
      center[0] = static_cast<float>(i%data.width);
      center[1] = static_cast<float>(i/data.width);
      for(j = 0; j < data.dimensionRange; j++){
        center[j+2] = data.imageLuvOrgF[data.dimensionRange*i+j];
      }
  
      int iterationCount = 0;
  
      // determine by this loop the new center by following the meanshift
      // vector
      do{
        // calculate the location of center on the lattice
        modeCandidateX  = int(center[0] + 0.5f);
        modeCandidateY  = int(center[1] + 0.5f);
        modeCandidate_i = modeCandidateY * data.width + modeCandidateX;

        if((modeTable[modeCandidate_i] != 2) && (modeCandidate_i != i)) {
          // obtain the data point at basin_i to see if it is within h* 0.5 of
          // of center
          for (j = 0; j < data.dimensionRange; j++){
            modeCandidatePoint[j] = 
              data.imageLuvOrgF[data.dimensionRange * modeCandidate_i + j];
          }
    
          diff = 0.0f;
          diff += static_cast<float>(sqr((modeCandidatePoint[0] - center[2]) /
                                         param.sigmaR));
          diff += static_cast<float>(sqr((modeCandidatePoint[1] - center[3]) /
                                         param.sigmaR));
          diff += static_cast<float>(sqr((modeCandidatePoint[2] - center[4]) /
                                         param.sigmaR));
    
          // if the color is within radius of sigmaR than the same
          // mode is assigned
          if(diff < 0.5f){
            // if the data point at basin_i has not been associated to a mode 
            // then associate it with the mode that this one will converge to
            if (modeTable[modeCandidate_i] == 0){
              pointList[pointCount++] = modeCandidate_i;
              modeTable[modeCandidate_i] = 2;
            } 
            else{
              // the mode has already been associated with another
              // mode, therefore associate this one mode and the modes
              // in the point list with the mode associated with
              // data[basin_i]...

              // store the mode info into center using imageLuvFilteredF
              for (j = 0; j < data.dimensionRange; j++){
                center[j+2] = 
                  data.imageLuvFilteredF[modeCandidate_i*data.dimensionRange +
                                         j];
              }
        
              // update mode table for this data point indicating that a mode
              // has been associated with it
              modeTable[i] = 1;

              // stop mean shift calculation...
              break;
            }
          }
        }

        // determine lower and upper bounds on grid
        lowerBoundX = int(center[0] - param.sigmaS);
        if (lowerBoundX < 0) {
          lowerBoundX = 0;
        }
        lowerBoundY = int(center[1] - param.sigmaS);
        if (lowerBoundY < 0) {
          lowerBoundY = 0;
        }
        upperBoundX = int(center[0] + param.sigmaS);
        if (upperBoundX >= data.width) {
          upperBoundX = data.width - 1;
        }
        upperBoundY = int(center[1] + param.sigmaS);
        if (upperBoundY >= data.height) {
          upperBoundY = data.height - 1;
        }

        for(int l = 0; l < data.dimensionFeatureSpace; l++){
          newCenter[l] = 0;
        }
        weightSum = 0;

        // iterate once through window of size sigmaS and determine the
        // newCenter
        for(int x = lowerBoundX; x <= upperBoundX; x++){
          for(int y = lowerBoundY; y <= upperBoundY; y++){

            pos1 = (y*data.width+x);
            pos2 = (y*data.width+x)*data.dimensionRange;

            // test if feature point is within sphere of radius sigmaS / sigmaR
            sqrRadius += static_cast<float>(sqr((x - center[0]) / 
                                                param.sigmaS));
            sqrRadius += static_cast<float>(sqr((y - center[1]) /
                                                param.sigmaS));
            sqrRadius += static_cast<float>(sqr((data.imageLuvOrgF[pos2] -
                                                 center[2]) / param.sigmaR));
            sqrRadius += static_cast<float>(sqr((data.imageLuvOrgF[pos2+1] -
                                                 center[3]) / param.sigmaR));
            sqrRadius += static_cast<float>(sqr((data.imageLuvOrgF[pos2+2] -
                                                 center[4]) / param.sigmaR));
    
            // considered point is within sphere => accumulate to mean
            // for newCenter
            if(sqrRadius < 1.0f){
        
              newCenter[0] += data.weightMap[pos1] * x;
              newCenter[1] += data.weightMap[pos1] * y;
              newCenter[2] += data.weightMap[pos1] * data.imageLuvOrgF[pos2];
              newCenter[3] += data.weightMap[pos1] * data.imageLuvOrgF[pos2+1];
              newCenter[4] += data.weightMap[pos1] * data.imageLuvOrgF[pos2+2];
              weightSum += data.weightMap[pos1];

              // highSpeedup: point within sphere => the same mode is
              // assigned to it even though it might have converged to
              // another mode (inaccuracy) the only difference to
              // mediumSpeedup
              if(param.speedup == HighSpeedup && 
                 modeTable[pos1] == 0) {
                pointList[pointCount++]  = pos1;
                modeTable[pos1]  = 2;
              }
            }

            sqrRadius = 0;
          }
        }
  
        msAbs = 0;
  
        // determine the new center and the magnitude of the meanshift vector
        // meanshiftvector = newCenter - center
        for(j = 0; j < data.dimensionFeatureSpace; j++){
          newCenter[j] /= weightSum;
          msAbs += sqr(newCenter[j] - center[j]);
          center[j] = newCenter[j];
        }

        iterationCount++;
        _lti_debug3("itCount: "<<iterationCount<<"   meanshift magnitude: "<<
                    msAbs<<std::endl);

        // shifting window
      } while ((msAbs > param.thresholdConverged) &&
               (iterationCount < param.maxTrial));

      // for debug only
      data.vecTrialsToConverge[iterationCount-1]++;

      // associate the data point indexed by the point list with the mode
      // stored by center
      for (j = 0; j < pointCount; j++){
  
        // obtain the point location from the point list
        modeCandidate_i = pointList[j];

        // store result into imageLuvFilteredF and imageLuvFilteredI
        for(int k = 0; k < data.dimensionRange; k++){
          pos =  modeCandidate_i * data.dimensionRange + k;
          data.imageLuvFilteredF[pos] = center[k+2];
          if(center[k+2] < 0) {
            data.imageLuvFilteredI[pos] = int(center[k+2] - 0.5f); 
          } else {
            data.imageLuvFilteredI[pos] = int(center[k+2] + 0.5f);
          }
        }

        // update the mode table for this point
        modeTable[modeCandidate_i] = 1;
      }
      
      // store result for point i
      pos = i*data.dimensionRange;
      data.imageLuvFilteredF[pos] = center[2];
      data.imageLuvFilteredF[pos+1] = center[3];
      data.imageLuvFilteredF[pos+2] = center[4];

      if(center[2] < 0) {
        data.imageLuvFilteredI[pos] = int(center[2] - 0.5f); 
      } else {
        data.imageLuvFilteredI[pos] = int(center[2] + 0.5f);
      }

      if(center[3] < 0) {
        data.imageLuvFilteredI[pos+1] = int(center[3] - 0.5f); 
      } else {
        data.imageLuvFilteredI[pos+1] = int(center[3] + 0.5f);
      }

      if(center[4] < 0) {
        data.imageLuvFilteredI[pos+2] = int(center[4] - 0.5f); 
      } else {
        data.imageLuvFilteredI[pos+2] = int(center[4] + 0.5f);
      }

      // update mode table for this data point indicating that a mode has been 
      // associated with it
      modeTable[i] = 1;
     
    }

    delete[] modeTable;
    delete[] pointList; 
    delete[] modeCandidatePoint;
  }


  
  void  meanShiftSegmentation::connect(internals& data) const {
    
    data.regionCount = 0;
    int   i, k, neighLoc, neighborsFound, label = -1, regionLoc = 0;

    // stores all neighbour points of the considered seed that have
    // the same color and thus become themselves region seeds
    int* indexTable = new int[data.imageSize];
    
    // define eight connected neighbors
    int neigh[8];
    neigh[0]  = 1;
    neigh[1]  = 1-data.width;
    neigh[2]  = -data.width;
    neigh[3]  = -(1+data.width);
    neigh[4]  = -1;
    neigh[5]  = data.width-1;
    neigh[6]  = data.width;
    neigh[7]  = data.width+1;
  
    // initialize labels with -1
    for(i = 0; i < data.imageSize; i++){
      data.labels[i] = -1;
    }

    // traverse the image labeling each new region encountered
    for(i = 0; i < data.imageSize; i++){

      regionLoc = i;

      if(data.labels[i] < 0){  // if this region has not yet been labeled 
                               // - label it
        data.labels[i] = ++label;// assign new label to this region

        // copy region color into modes
        for(k = 0; k < data.dimensionRange; k++){
          data.modes[(label * data.dimensionRange) + k]  =
            float(data.imageLuvFilteredI[(data.dimensionRange * i) + k]);
        }
  
        // initialzie indexTable
        int  index    = 0;
        indexTable[0]  = regionLoc;

        // increment mode point counts for this region to
        // indicate that one pixel belongs to this region
        data.modePointCounts[label]++;

        // populate labels with label for this specified region
        // calculating modePointCounts[label]
        while(true){

          //assume no neighbors will be found
          neighborsFound  = 0;

          // check the eight connected neighbors at regionLoc -
          // if a pixel has the same color as that located at 
          // regionLoc then declare it as part of this region
          for(int n = 0; n < 8; n++){

            // check bounds and if neighbor has been already labeled
            neighLoc = regionLoc + neigh[n];
            if((neighLoc >= 0) && 
               (neighLoc < data.imageSize) && (data.labels[neighLoc] < 0)) {
              for(k = 0; k < data.dimensionRange; ++k){
                if(data.imageLuvFilteredI[(regionLoc*data.dimensionRange)+k]
                   != 
                   data.imageLuvFilteredI[(neighLoc*data.dimensionRange)+k])
                  break;
              }
              // neighbor i belongs to this region so label it and place 
              // it onto the index table buffer for further processing
              if(k == data.dimensionRange){

                //assign label to neighbor i
                data.labels[neighLoc] = label;
    
                //increment region point count
                data.modePointCounts[label]++;
    
                //place index of neighbor i onto the index tabel buffer
                indexTable[++index] = neighLoc;
    
                //indicate that a neighboring region pixel was
                //identified
                neighborsFound  = 1;
              }
            }
          }

          // check the indexTable to see if there are any more
          // entries to be explored - if so explore them, otherwise
          // exit the loop - we are finished
          if(neighborsFound)
            regionLoc  = indexTable[index];
          else if (index > 1)
            regionLoc  = indexTable[--index];
          else
            break; // fill complete
        }
      }
    }
    // calculate region count using label
    data.regionCount  = label+1;
    delete[] indexTable;
  }
  
  
  
  void 
  meanShiftSegmentation::buildRegionAdjacencyMatrix(internals& data) const {

    const int memoryFactor = 10; // for size of allocated memory block
    int usedMemory = 0; //for debug only

    // allocate memory for region adjacency matrix 
    data.raMatrix = new regionAdjacencyList [data.regionCount];
    data.raPool = new regionAdjacencyList [memoryFactor * data.regionCount];

    // initialize the region adjacency array of lists
    int i;
    for(i = 0; i < data.regionCount; i++){
      data.raMatrix[i].label = i;
      data.raMatrix[i].next = NULL;
    }

    // initialize free list
    regionAdjacencyList* freeRaList = data.raPool;
    for(i = 0; i < memoryFactor*data.regionCount -1; i++){
      data.raPool[i].next = &data.raPool[i+1];
    }
    data.raPool[memoryFactor*data.regionCount-1].next = NULL;
    
    // traverse the labeled image building the RAM by looking 
    // to the right of and below the current pixel location thus
    // determining if a given region is adjacent to another
    int  j, curLabel, rightLabel, bottomLabel;
    bool inserted;

    regionAdjacencyList *raNode1, *raNode2, *oldRAFreeList;
    for(i = 0; i < data.height - 1; i++){
      // check the right and below neighbors
      // for pixel locations whose x < width - 1
      for(j = 0; j < data.width - 1; j++){
        //calculate pixel labels
        curLabel    = data.labels [i*data.width+j];  //current pixel
        rightLabel  = data.labels[i*data.width+j+1];  //right   pixel
        bottomLabel = data.labels[(i+1)*data.width+j]; //bottom  pixel

        // check to the right, if the label of the right pixel is 
        // not the same as that of the current one then 
        // region[j] and region[j+1] are adjacent to one another
        // update the raMatrix
        if(curLabel != rightLabel){
          // obtain RAList object from region adjacency free list
          raNode1 = freeRaList;
          raNode2 = freeRaList->next;
          usedMemory += 2;

          // keep a pointer to the old region adj. free list just 
          // in case nodes already exist in respective region lists
          oldRAFreeList  = freeRaList;

          // update region adjacency free list
          freeRaList = freeRaList->next->next;

          // populate RAList nodes
          raNode1->label = curLabel;
          raNode2->label = rightLabel;

          // insert nodes into the RAM
          inserted = true;
          data.raMatrix[curLabel].insert(raNode2);
          inserted = data.raMatrix[rightLabel].insert(raNode1);
          // if the node already exists then place nodes back 
          // onto the region adjacency free list
          if(!inserted) { freeRaList = oldRAFreeList; usedMemory -= 2; }
        }

        // check below, if the label of
        // the bottom pixel is not the same as that
        // of the current one then region[j] and region[j+width]
        // are adjacent to one another - update the raMatrix
        if(curLabel != bottomLabel){
          // obtain RAList object from region adjacency free list
          raNode1 = freeRaList;
          raNode2 = freeRaList->next;
          usedMemory += 2;
          // keep a pointer to the old region adj. free list just in 
          // case nodes already exist in respective region lists
          oldRAFreeList  = freeRaList;

          // update region adjacency free list
          freeRaList = freeRaList->next->next;

          // populate RAList nodes
          raNode1->label = curLabel;
          raNode2->label = bottomLabel;

          // insert nodes into the RAM
          inserted = 0;
          data.raMatrix[curLabel].insert(raNode2);
          inserted = data.raMatrix[bottomLabel].insert(raNode1);

          // if the node already exists then place nodes back onto 
          // the region adjacency free list
          if(!inserted) { freeRaList = oldRAFreeList; usedMemory -= 2; }
        }

      }// (for(j = 0; j < width - 1;...

      // check only to the bottom neighbors of the right boundary pixels...

      // calculate pixel locations (j = width-1)
      curLabel  = data.labels[i*data.width+j]; //current pixel
      bottomLabel = data.labels[(i+1)*data.width+j]; //bottom  pixel
      
      // check below, if the label of the bottom pixel is not the same 
      // as that of the current one then region[j] and region[j+width]
      // are adjacent to one another - update the raMatrix
      if(curLabel != bottomLabel){
        //obtain RAList object from region adjacency free list
        raNode1  = freeRaList;
        raNode2  = freeRaList->next;
        usedMemory += 2;

        //keep a pointer to the old region adj. free list just in case 
        //nodes already exist in respective region lists
        oldRAFreeList = freeRaList;
  
        //update region adjacency free list
        freeRaList = freeRaList->next->next;
      
        //populate RAList nodes
        raNode1->label = curLabel;
        raNode2->label = bottomLabel;
      
        //insert nodes into the RAM
        inserted = true;
        data.raMatrix[curLabel].insert(raNode2);
        inserted = data.raMatrix[bottomLabel].insert(raNode1);
      
        //if the node already exists then place nodes back onto the 
        //region adjacency free list
        if(!inserted) { freeRaList = oldRAFreeList; usedMemory -= 2;}
      }
    }

    //check only to the right neighbors of the bottom boundary pixels...

    //check the right for pixel locations whose x < width - 1
    for(j = 0; j < data.width - 1; j++){
      //calculate pixel labels (i = height-1)
      curLabel  = data.labels[i*data.width+j];  //current pixel
      rightLabel  = data.labels[i*data.width+j+1];  //right   pixel
    
      //check to the right, if the label of the right pixel is not the same as
      //that of the current one then region[j] and region[j+1] are adjacent to
      //one another update the raMatrix
      if(curLabel != rightLabel){
        //obtain RAList object from region adjacency free list
        raNode1  = freeRaList;
        raNode2  = freeRaList->next;
        usedMemory += 2;

        //keep a pointer to the old region adj. free list just in case nodes
        //already exist in respective region lists
        oldRAFreeList = freeRaList;
  
        //update region adjacency free list
        freeRaList = freeRaList->next->next;
  
        //populate RAList nodes
        raNode1->label = curLabel;
        raNode2->label = rightLabel;
      
        //insert nodes into the RAM
        inserted = true;
        data.raMatrix[curLabel].insert(raNode2);
        inserted = data.raMatrix[rightLabel].insert(raNode1);
  
        //if the node already exists then place nodes back onto the region 
        //adjacency free list
        if (!inserted) {
          freeRaList = oldRAFreeList; usedMemory -= 2; 
        }
      }
    }

    _lti_debug2("\nallocated memory pool for region adjacency list: \n");
    _lti_debug2(memoryFactor * regionCount <<"   used memory : "<<
                usedMemory<<std::endl);
  }

  
  meanShiftSegmentation::regionAdjacencyList:: regionAdjacencyList(){
    
    label = -1;
    next = NULL;
  }


  bool meanShiftSegmentation::regionAdjacencyList::insert(
                                                  regionAdjacencyList *entry) {
    
    //if the list contains only one element then insert this element into next
    if(!next){
      next = entry;
      entry->next = NULL;
      return true;
    }

    //check first entry 
    if(next->label > entry->label){
  
      //insert entry into the list at this location
      entry->next  = next;
      next    = entry;
      return true;
    }

    //traverse the list until either:
    // (a) entry's label already exists - do not insert and return false
    // (b) the list ends or the current label is greater than entry's 
    // label, thus insert the entry at this location
 
    cur_ = next;
    while(cur_){
      if(entry->label == cur_->label){  //node already exists
        return false;
      }
      else if((!(cur_->next))||(cur_->next->label > entry->label)){
        //insert entry into the list at this location
        entry->next  = cur_->next;
        cur_->next  = entry;
        return true;
      }

      //traverse the region adjacency list
      cur_ = cur_->next;
    }
    return false;
  }

  void meanShiftSegmentation::fuseRegions(internals& data) const {

    const parameters& param = getParameters();
    int oldRegionCount = data.regionCount;
    int counter = 0;
    std::vector<int> vecRegionCount;//for debug only
    bool inWindow;
    double diff;
    int mode1, mode2;
    double maxDistance = param.maxNeighbourColorDistance;

    do {
      oldRegionCount = data.regionCount;
      counter++;

      // find for each region its neighbours
      buildRegionAdjacencyMatrix(data);
  
      //traverse the raMatrix attempting to join raMatrix[i] with its
      //neighbour regions whose mode is a normalized distance < 0.5 from that
      //of region i.
      int i, iCanEl, neighCanEl;
      regionAdjacencyList* neighbor;
      
      //Step (1):
      //Treat each region Ri as a disjoint set:
      // - attempt to join Ri and Rj for all i != j that are neighbors and
      //   whose associated modes are a normalized distance of < 0.5 from one
      //   another

      // - the label of each region in the raMatrix is treated as a pointer
      //   to the canonical element of that region (e.g. raMatrix[i],
      //   initially has raMatrix[i].label = i, namely each region is
      //   initialized to have itself as its canonical element).
      for(i = 0; i < data.regionCount; i++){
        // aquire first neighbor in region adjacency list pointed to by
        // raMatrix[i]
        neighbor = data.raMatrix[i].next;
        while(neighbor){
    
          inWindow = false;
          diff = 0;
          mode1 = i; 
          mode2 = neighbor->label;
    
          // test if feature point is within a normalized distance of 0.5 from
          // mode
          diff += sqr((data.modes[mode1*data.dimensionRange] -
                       data.modes[mode2*data.dimensionRange]) / maxDistance);
          diff += sqr((data.modes[mode1*data.dimensionRange+1] -
                       data.modes[mode2*data.dimensionRange+1]) / maxDistance);
          diff += sqr((data.modes[mode1*data.dimensionRange+2] -
                       data.modes[mode2*data.dimensionRange+2]) / maxDistance);
          inWindow = (diff < 0.25); // range part of difference about 0.5 and
                                    // sqrt(0.5) = 0.25
    
          //attempt to join region and neighbor...
          if (inWindow) {
            //region i and neighbor belong together so join them by:
            // (1) find the canonical element of region i
            iCanEl = i;
            while(data.raMatrix[iCanEl].label != iCanEl)
              iCanEl = data.raMatrix[iCanEl].label;
      
            // (2) find the canonical element of neighboring region
            neighCanEl = neighbor->label;
            while(data.raMatrix[neighCanEl].label != neighCanEl)
              neighCanEl = data.raMatrix[neighCanEl].label;

            // if the canonical elements of are not the same then assign
            // the canonical element having the smaller label to be the parent
            // of the other region...
            if(iCanEl < neighCanEl)
              data.raMatrix[neighCanEl].label = iCanEl;
            else{
              //must replace the canonical element of previous
              //parent as well
              data.raMatrix[data.raMatrix[iCanEl].label].label = neighCanEl;
        
              //re-assign canonical element
              data.raMatrix[iCanEl].label = neighCanEl;
            }
          }
          //check the next neighbor...
          neighbor = neighbor->next;
        }
      }

      // Step (2):
      // Level binary trees formed by canonical elements
      for(i = 0; i < data.regionCount; i++){
        iCanEl  = i;
        while(data.raMatrix[iCanEl].label != iCanEl){
          iCanEl = data.raMatrix[iCanEl].label;
        }
        data.raMatrix[i].label = iCanEl;
      }

      // Step (3):
      //Traverse joint sets, relabeling image.
      // (a)
      // Accumulate modes and re-compute point counts using canonical
      // elements generated by step 2.
      // allocate memory for mode and point count temporary buffers...
      float * modesBuffer = new float[data.dimensionRange*data.regionCount];
      int * MPCBuffer = new int[data.regionCount];
      
      //initialize buffers to zero
      for(i = 0; i < data.regionCount; i++)
        MPCBuffer[i]  = 0;
      for(i = 0; i < data.dimensionRange*data.regionCount; i++)
        modesBuffer[i]  = 0;
      
      //traverse raMatrix accumulating modes and point counts
      //using canoncial element information...
      int k, iMPC;
      for(i = 0; i < data.regionCount; i++){

        //obtain canonical element of region i
        iCanEl = data.raMatrix[i].label;
  
        //obtain mode point count of region i
        iMPC = data.modePointCounts[i];

        //color just mixed?
        //accumulate modesBuffer[iCanEl]
        for(k = 0; k < data.dimensionRange; k++)
          modesBuffer[(data.dimensionRange*iCanEl)+k] += 
            iMPC*data.modes[(data.dimensionRange*i)+k];
  
        //accumulate MPCBuffer[iCanEl]
        MPCBuffer[iCanEl] += iMPC;
      }

      // (b)
      // Re-label new regions of the image using the canonical
      // element information generated by step (1)
      // Also use this information to compute the modes of the newly
      // defined regions, and to assign new region point counts in
      // a consecute manner to the modePointCounts array
      
      int *labelBuffer  = new int [data.regionCount];
      
      //initialize label buffer to -1
      for(i = 0; i < data.regionCount; i++){
        labelBuffer[i]  = -1;
      }
      //traverse raMatrix re-labeling the regions
      int label = -1;
      for(i = 0; i < data.regionCount; i++){
        //obtain canonical element of region i
        iCanEl  = data.raMatrix[i].label;
        if(labelBuffer[iCanEl] < 0){
          //assign a label to the new region indicated by canonical
          //element of i
          labelBuffer[iCanEl]  = ++label;
    
          //recompute mode storing the result in modes[label]...
          iMPC  = MPCBuffer[iCanEl];
          for(k = 0; k < data.dimensionRange; k++)
            data.modes[(data.dimensionRange*label)+k] = 
              (modesBuffer[(data.dimensionRange*iCanEl)+k])/(iMPC);
          
          //assign a corresponding mode point count for this region into
          //the mode point counts array using the MPC buffer...
          data.modePointCounts[label] = MPCBuffer[iCanEl];
        }
      }
      
      //re-assign region count using label counter
      data.regionCount = label+1;

      // (c)
      // Use the label buffer to reconstruct the label map, which specified
      // the new image given its new regions calculated above
      for(i = 0; i < data.imageSize; i++){
        data.labels[i] = labelBuffer[data.raMatrix[data.labels[i]].label];
      }
      //de-allocate memory
      delete [] modesBuffer;
      delete [] MPCBuffer;
      delete [] labelBuffer;

#ifdef _LTI_DEBUG
      vecRegionCount.push_back(data.regionCount);
#endif

      // remove mem alloc in buildRegionAdjacencyMatrix() | alvarado 040217
      
      delete[] data.raPool;
      data.raPool = 0;
      
      delete[] data.raMatrix;
      data.raMatrix = 0;

    }
    while((oldRegionCount-data.regionCount > 0 )&&(counter<10));


#ifdef _LTI_DEBUG
    _lti_debug("\n\nnumber of regions: "<<std::endl);
    std::vector<int>::const_iterator it;
    for(it = vecRegionCount.begin(); it != vecRegionCount.end();it++){
      _lti_debug(*it<<"   ");
    }
    _lti_debug("\n");
#endif
    
  }


  void  meanShiftSegmentation::pruneRegions(internals& data) const {
    
    const parameters& param = getParameters();

    float* modesBuffer = new float[data.dimensionRange*data.regionCount];
    int* MPCBuffer     = new int[data.regionCount];
    int* labelBuffer   = new int[data.regionCount];
  
    int  i, k, candidate, iCanEl, neighCanEl, iMPC;
    int label, minRegionCount; // oldRegionCount, 
    double minSqDistance, neighborDistance;
    regionAdjacencyList *neighbor;

    // Apply pruning algorithm to classification structure, removing all
    // regions whose area is under the threshold area minRegionSize (pixels)
    do {
      //assume that no region has area under threshold area  of 
      minRegionCount  = 0;    
      
      //Step (1):
      // Build raMatrix using classifiction structure originally
      buildRegionAdjacencyMatrix(data);
      
      // Step (2):
      
      // Traverse the raMatrix joining regions whose area is less than
      // minRegion (pixels) with its respective candidate region.  A
      // candidate region is a region that displays the following
      // properties:
      //  - it is adjacent to the region being pruned
      //  - the distance of its mode is a minimum to that of the region
      //    being pruned such that or it is the only adjacent region having
      //    an area greater than minRegion
      
      for(i = 0; i < data.regionCount; i++){
        
        // ******************************************************************
        // Note: Adjust this if statement if a more sophisticated
        //       pruning criterion is desired. Basically in this step a
        //       region whose area is less than minRegion is pruned by
        //       joining it with its "closest" neighbor (in color).
        //       Therefore, by placing a different criterion for fusing a
        //       region the pruning method may be altered to implement a
        //       more sophisticated algorithm. 
        //******************************************************************
        if(data.modePointCounts[i] < param.minRegionSize){
            //update minRegionCount to indicate that a region
            //having area less than minRegion was found
            minRegionCount++;

            //obtain a pointer to the first region in the
            //region adjacency list of the ith region...
            neighbor = data.raMatrix[i].next;
        
            //calculate the distance between the mode of the ith
            //region and that of the neighboring region...
            candidate = neighbor->label;
            minSqDistance = 0.0;
            minSqDistance += sqr((data.modes[i*data.dimensionRange] - 
                                  data.modes[candidate*data.dimensionRange]));
            minSqDistance += sqr((data.modes[i*data.dimensionRange+1] -
                                  data.modes[candidate*
                                             data.dimensionRange+1]));
            minSqDistance += sqr((data.modes[i*data.dimensionRange+2] -
                                  data.modes[candidate*
                                             data.dimensionRange+2]));
                
            //traverse region adjacency list of region i and select
            //a candidate region
            neighbor  = neighbor->next;
            while(neighbor)  {
        
              //calculate the square distance between region i
              //and current neighbor...
              neighborDistance = 0.0;
              neighborDistance+=sqr((data.modes[i*data.dimensionRange]-
                                     data.modes[neighbor->label *
                                                data.dimensionRange]));
              neighborDistance+=sqr((data.modes[i*data.dimensionRange+1]-
                                     data.modes[neighbor->label * 
                                                data.dimensionRange+1]));
              neighborDistance+=sqr((data.modes[i*data.dimensionRange+2]-
                                     data.modes[neighbor->label *
                                                data.dimensionRange+2]));

              //if this neighbors square distance to region i is less
              //than minSqDistance, then select this neighbor as the
              //candidate region for region i
              if(neighborDistance < minSqDistance){
                minSqDistance  = neighborDistance;
                candidate  = neighbor->label;
              }
              //traverse region list of region i
              neighbor  = neighbor->next;
            }

            //join region i with its candidate region:
            // (1) find the canonical element of region i
            iCanEl    = i;
            while(data.raMatrix[iCanEl].label != iCanEl)
              iCanEl    = data.raMatrix[iCanEl].label;
      
            // (2) find the canonical element of neighboring region
            neighCanEl  = candidate;
            while(data.raMatrix[neighCanEl].label != neighCanEl)
              neighCanEl  = data.raMatrix[neighCanEl].label;

            // if the canonical elements of are not the same then assign
            // the canonical element having the smaller label to be the parent
            // of the other region...
            if(iCanEl < neighCanEl)
              data.raMatrix[neighCanEl].label = iCanEl;
            else
              {
                //must replace the canonical element of previous
                //parent as well
                data.raMatrix[data.raMatrix[iCanEl].label].label  = neighCanEl;

                //re-assign canonical element
                data.raMatrix[iCanEl].label  = neighCanEl;
              }
          }

        }

        // Step (3):
    
        // Level binary trees formed by canonical elements
        for(i = 0; i < data.regionCount; i++)
          {
            iCanEl  = i;
            while(data.raMatrix[iCanEl].label != iCanEl)
              iCanEl  = data.raMatrix[iCanEl].label;
            data.raMatrix[i].label  = iCanEl;
          }
    
        // Step (4):
        //Traverse joint sets, relabeling image.
        // Accumulate modes and re-compute point counts using canonical
        // elements generated by step 2.
        //initialize buffers to zero
        for(i = 0; i < data.regionCount; i++)
          MPCBuffer[i]  = 0;
        for(i = 0; i < data.dimensionRange*data.regionCount; i++)
          modesBuffer[i]  = 0;

        //traverse raMatrix accumulating modes and point counts
        //using canoncial element information...
        for(i = 0; i < data.regionCount; i++){
      
          //obtain canonical element of region i
          iCanEl  = data.raMatrix[i].label;
      
          //obtain mode point count of region i
          iMPC  = data.modePointCounts[i];
      
          //accumulate modesBuffer[iCanEl]
          for(k = 0; k < data.dimensionRange; k++)
            modesBuffer[(data.dimensionRange*iCanEl)+k] +=
              iMPC*data.modes[(data.dimensionRange*i)+k];
      
          //accumulate MPCBuffer[iCanEl]
          MPCBuffer[iCanEl] += iMPC;
      
        }
  
        // (b)
        // Re-label new regions of the image using the canonical
        // element information generated by step (2)
        // Also use this information to compute the modes of the newly
        // defined regions, and to assign new region point counts in
        // a consecute manner to the modePointCounts array
    
        //initialize label buffer to -1
        for(i = 0; i < data.regionCount; i++)
          labelBuffer[i]  = -1;
    
        //traverse raMatrix re-labeling the regions
        label = -1;
        for(i = 0; i < data.regionCount; i++){
          //obtain canonical element of region i
          iCanEl  = data.raMatrix[i].label;
          if(labelBuffer[iCanEl] < 0){
            //assign a label to the new region indicated by canonical
            //element of i
            labelBuffer[iCanEl]  = ++label;
        
            //recompute mode storing the result in modes[label]...
            iMPC  = MPCBuffer[iCanEl];
            for(k = 0; k < data.dimensionRange; k++)
              data.modes[(data.dimensionRange*label)+k]  = 
                (modesBuffer[(data.dimensionRange*iCanEl)+k])/(iMPC);
        
            //assign a corresponding mode point count for this region into
            //the mode point counts array using the MPC buffer...
            data.modePointCounts[label]  = MPCBuffer[iCanEl];
          }
        }
    
        //re-assign region count using label counter
        // oldRegionCount  = data.regionCount;
        data.regionCount = label+1;
    
        // (c)
        // Use the label buffer to reconstruct the label map, which specified
        // the new image given its new regions calculated above
  
        for(i = 0; i < data.height*data.width; i++)
          data.labels[i]  = labelBuffer[data.raMatrix[data.labels[i]].label];

        // remove mem alloc in buildRegionAdjacencyMatrix() | alvarado 040217

        delete[] data.raPool;
        data.raPool = 0;

        delete[] data.raMatrix;
        data.raMatrix = 0;

      }
    while(minRegionCount > 0);

    //de-allocate memory
    delete [] modesBuffer;
    delete [] MPCBuffer;
    delete [] labelBuffer;
  }



  void  meanShiftSegmentation::rgbToLuv(const image& src,
                                        internals& data) const {

    int i,j;

    static const double Yn      = 1.00000;
    static const double Un_prime  = 0.19784977571475;
    static const double Vn_prime  = 0.46834507665248;
    static const double Lt      = 0.008856;
    
    //RGB to LUV conversion
    static const double XYZ[3][3] = {  {  0.4125,  0.3576,  0.1804 },
                                       {  0.2125,  0.7154,  0.0721 },
                                       {  0.0193,  0.1192,  0.9502 }  };

    int rgbVal[3];
    int pos;

    for(i = 0; i < data.height; i++){
      const int iw = i*data.width;
      for(j = 0; j < data.width; j++){
      
        pos = (iw+j)*data.dimensionRange;

        rgbVal[0] = (src.at(i,j)).getRed(); 
        rgbVal[1] = (src.at(i,j)).getGreen(); 
        rgbVal[2] = (src.at(i,j)).getBlue();
      
        //declare variables
        double  x, y, z, L0, u_prime, v_prime, constant;
  
        //convert RGB to XYZ...
        x    = XYZ[0][0]*rgbVal[0] + XYZ[0][1]*rgbVal[1] + XYZ[0][2]*rgbVal[2];
        y    = XYZ[1][0]*rgbVal[0] + XYZ[1][1]*rgbVal[1] + XYZ[1][2]*rgbVal[2];
        z    = XYZ[2][0]*rgbVal[0] + XYZ[2][1]*rgbVal[1] + XYZ[2][2]*rgbVal[2];

        //convert XYZ to LUV...
  
        //compute L*
        L0    = y / (255.0 * Yn);
        if(L0 > Lt){
          data.imageLuvOrgF[pos] = float(116.0 * (pow(L0, 1.0/3.0)) - 16.0);
        }
        else{
          data.imageLuvOrgF[pos] = float(903.3 * L0);
        }
        //compute u_prime and v_prime
        constant  = x + 15 * y + 3 * z;
        if(constant != 0)  {
          u_prime  = (4 * x) / constant;
          v_prime = (9 * y) / constant;
        }
        else{
          u_prime  = 4.0;
          v_prime  = 9.0/15.0;
        }

        //compute u* and v*
        data.imageLuvOrgF[pos +1] = float(13 * data.imageLuvOrgF[pos] * 
                                          (u_prime - Un_prime));
        data.imageLuvOrgF[pos +2] = float(13 * data.imageLuvOrgF[pos] *
                                          (v_prime - Vn_prime));
      }
    }

  }
  

  void  meanShiftSegmentation::luvToRgb(float* src, image& dest,
                                        internals& data) const{
 
    int i,j;
    
    static const double Yn        = 1.00000;
    static const double Un_prime  = 0.19784977571475;
    static const double Vn_prime  = 0.46834507665248;
   
    
    static const double RGB[3][3] = {  {  3.2405, -1.5371, -0.4985 },
                                       { -0.9693,  1.8760,  0.0416 },
                                       {  0.0556, -0.2040,  1.0573 }  };
    
    int    r, g, b;
    double  x, y, z, u_prime, v_prime, L, U, V;
  
    int pos;

    for(i = 0; i < data.height; i++){
      for(j = 0; j < data.width; j++){
     
        pos = (i*data.width+j)*data.dimensionRange;
        L = double(src[pos]); 
        U = double(src[pos+1]);
        V = double(src[pos+2]);

        if(L < 0.1) {
          r = g = b = 0;
        } else {
          //convert luv to xyz...
          if(L < 8.0)
            y  = Yn * L / 903.3;
          else{
            y  = (L + 16.0) / 116.0;
            y  *= Yn * y * y;
          }
    
          u_prime  = U / (13 * L) + Un_prime;
          v_prime  = V / (13 * L) + Vn_prime;

          x    = 9 * u_prime * y / (4 * v_prime);
          z    = (12 - 3 * u_prime - 20 * v_prime) * y / (4 * v_prime);

          //convert xyz to rgb...
          //[r, g, b] = RGB*[x, y, z]*255.0
          r    = iround((RGB[0][0]*x + RGB[0][1]*y + RGB[0][2]*z)*255.0);
          g    = iround((RGB[1][0]*x + RGB[1][1]*y + RGB[1][2]*z)*255.0);
          b    = iround((RGB[2][0]*x + RGB[2][1]*y + RGB[2][2]*z)*255.0);

          //check bounds...
          if(r < 0)  r = 0; if(r > 255)  r = 255;
          if(g < 0)  g = 0; if(g > 255)  g = 255;
          if(b < 0)  b = 0; if(b > 255)  b = 255;

        }

        //assign rgb values to dest
        dest.at(i,j).set(r,g,b,0);
      }
    }
  }
  
  void  meanShiftSegmentation::luvToRgb(luvPixel src, rgbaPixel& dest) const {
    
    const double Yn      = 1.00000;
    const double Un_prime  = 0.19784977571475;
    const double Vn_prime  = 0.46834507665248;    
    
    const double RGB[3][3] = {  {  3.2405, -1.5371, -0.4985 },
                                { -0.9693,  1.8760,  0.0416 },
                                {  0.0556, -0.2040,  1.0573 }  };
    
    int    r, g, b;
    double  x, y, z, u_prime, v_prime, L, U, V;
  

    L = double(src.l);
    U = double(src.u);
    V = double(src.v);

    if(L < 0.1)
      r = g = b = 0;
    else{
      //convert luv to xyz...
      if(L < 8.0)
        y  = Yn * L / 903.3;
      else{
        y  = (L + 16.0) / 116.0;
        y  *= Yn * y * y;
      }
      
      u_prime  = U / (13 * L) + Un_prime;
      v_prime  = V / (13 * L) + Vn_prime;
      
      x    = 9 * u_prime * y / (4 * v_prime);
      z    = (12 - 3 * u_prime - 20 * v_prime) * y / (4 * v_prime);
      
      //convert xyz to rgb...
      //[r, g, b] = RGB*[x, y, z]*255.0
      r = iround((RGB[0][0]*x + RGB[0][1]*y + RGB[0][2]*z)*255.0);
      g = iround((RGB[1][0]*x + RGB[1][1]*y + RGB[1][2]*z)*255.0);
      b = iround((RGB[2][0]*x + RGB[2][1]*y + RGB[2][2]*z)*255.0);
      
      //check bounds...
      if(r < 0)  r = 0; if(r > 255)  r = 255;
      if(g < 0)  g = 0; if(g > 255)  g = 255;
      if(b < 0)  b = 0; if(b > 255)  b = 255;
      
    }
    
    //assign rgb values to dest
    dest.set(r,g,b,0);
  }

  /**
   * Read a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            meanShiftSegmentation::eSpeedUpType& data) {
    
    std::string str;
    
    if (handler.read(str)) {
      if (str.find("No") != std::string::npos) {
        data=meanShiftSegmentation::NoSpeedup;
      } else if (str.find("Medium") != std::string::npos) {
        data=meanShiftSegmentation::MediumSpeedup;
      } else if (str.find("High") != std::string::npos) {
        data=meanShiftSegmentation::HighSpeedup;
      } else {
        data=meanShiftSegmentation::NoSpeedup;
        return false;
      }
      return true;
    }

    return false;

  }

  /**
   * Write a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const meanShiftSegmentation::eSpeedUpType& data) {

    switch(data) {
      case meanShiftSegmentation::NoSpeedup:
        return lti::write(handler,"NoSpeedup");
        break;
      case meanShiftSegmentation::MediumSpeedup:
        return lti::write(handler,"MediumSpeedup");
        break;
      case meanShiftSegmentation::HighSpeedup:
        return lti::write(handler,"HighSpeedup");
        break;
      default:
        lti::write(handler,"Unknown");
    }

    return false;

  }


}
  
