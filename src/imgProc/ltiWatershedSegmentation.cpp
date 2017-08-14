/* Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
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

/*
 * \file   ltiWatershedSegmentation.cpp
 *         Watershed segmentation.
 * 
 * \author Benjamin Winkler
 * \author Axel Berner
 * \date   11.1.2001
 *
 * revisions ..: $Id: ltiWatershedSegmentation.cpp,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiWatershedSegmentation.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 2
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include "ltiViewer.h"
#include "ltiTimer.h"
#endif

namespace lti {
  // --------------------------------------------------
  // watershedSegmentation::parameters
  // --------------------------------------------------

  // default constructor
  watershedSegmentation::parameters::parameters()
    : functor::parameters() {

    neighborhood8 = false;

    watershedValue = 255;
    basinValue = 0;
    rainfall = true;
    threshold = 0;
  }

  // copy constructor
  watershedSegmentation::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  watershedSegmentation::parameters::~parameters() {
  }

  // get type name
  const std::string& watershedSegmentation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  watershedSegmentation::parameters&
  watershedSegmentation::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    neighborhood8 = other.neighborhood8;
    watershedValue = other.watershedValue;
    basinValue = other.basinValue;
    rainfall = other.rainfall;
    threshold = other.threshold;

    return *this;
  }

  // alias for copy member
  watershedSegmentation::parameters&
  watershedSegmentation::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  watershedSegmentation::parameters*
  watershedSegmentation::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  watershedSegmentation::parameters*
  watershedSegmentation::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * Write the parameters in the given ioHandler.
   *
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool watershedSegmentation::parameters::write(ioHandler& handler,
                                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"neighborhood8",neighborhood8);
      lti::write(handler,"watershedValue",watershedValue);
      lti::write(handler,"basinValue",basinValue);
      lti::write(handler,"rainfall",rainfall);
      lti::write(handler,"threshold",threshold);
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
   * @return true if write was successful
   */
  bool watershedSegmentation::parameters::read(ioHandler& handler,
                                               const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"neighborhood8",neighborhood8);
      lti::read(handler,"watershedValue",watershedValue);
      lti::read(handler,"basinValue",basinValue);
      lti::read(handler,"rainfall",rainfall);
      lti::read(handler,"threshold",threshold);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // watershedSegmentation
  // --------------------------------------------------

  // default constructor
  watershedSegmentation::watershedSegmentation()
    : functor(){
    parameters param;
    setParameters(param);
  }

  // default constructor
  watershedSegmentation::watershedSegmentation(const parameters& par)
    : functor() {
    setParameters(par);
  }

  // copy constructor
  watershedSegmentation::
  watershedSegmentation(const watershedSegmentation& other)
    : functor()  {
    copy(other);
  }

  // destructor
  watershedSegmentation::~watershedSegmentation() {
  }

  // returns the name of this type
  const std::string& watershedSegmentation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  watershedSegmentation&
  watershedSegmentation::copy(const watershedSegmentation& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  watershedSegmentation* watershedSegmentation::clone() const {
    return new watershedSegmentation(*this);
  }

  // clone member
  watershedSegmentation* watershedSegmentation::newInstance() const {
    return new watershedSegmentation;
  }

  // return parameters
  const watershedSegmentation::parameters&
  watershedSegmentation::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The private methods!
  // -------------------------------------------------------------------

  /**
   * fill neighborhood pointlist
   */
  void watershedSegmentation::createNeighborhood(const int colms, 
                                                 const bool neigh8) {
    if (!neigh8) {
      neigh_.allocate(4);
    } else {
      neigh_.allocate(8);
    }

    neigh_[0]= +1;
    neigh_[1]= -colms;
    neigh_[2]= -1;
    neigh_[3]= +colms;

    if( neigh8 ) {
      neigh_[4]= -colms+1;
      neigh_[5]= -colms-1;
      neigh_[6]= +colms-1;
      neigh_[7]= +colms+1;
    }
  }

  /**
   * Initialize a border LUT to save time detecting if a pixel is in
   * the border.
   */
  void watershedSegmentation::initBorderLUT(const ipoint& size,
                                            channel8& borderLUT) const {
    borderLUT.assign(size,0);
    borderLUT.getRow(0).fill(255);
    borderLUT.getRow(borderLUT.lastRow()).fill(255);

    const int lastRow = size.y-1;
    for (int y=0;y<lastRow;++y) {
      borderLUT.at(y,0)=borderLUT.at(y,borderLUT.lastColumn())=255;
    }
  }

  /**
   * create "histogram" with pixelPositions
   */
  void 
  watershedSegmentation::sortPixels(const channel8& src,
                                  std::vector<list_type>& sortedPoints) const {
    // sortedPoints is assumed empty.
    sortedPoints.resize(256);
    int i, threshold = getParameters().threshold;
    if( threshold != 0 ) { // raise water to threshold (to eleminate noise)
      for(i=0;i<imgSize_;i++) {
	if (src.elem(i) < threshold) {
	  sortedPoints[threshold].push_back(i);
        }
	else {
	  sortedPoints[src.elem(i)].push_back(i);
        }
      }
    }
    else {
      for (i=0; i<imgSize_; i++) {
	sortedPoints[src.elem(i)].push_back(i);
      }
    }
  }
  
  // scan all points of specific gray value
  // "result" is filled with -1
  void watershedSegmentation::maskCurrLevelPoints(
                                        const list_type& currentPointList,
                                        vector<int>& distance,
                                        std::queue<int>& fifoQueue,
                                        matrix<int>& result) const {
    static const int WSHED =  0;
    static const int MASK  = -2;

    int currentNeighbor;
    list_type::const_iterator pointIterator = currentPointList.begin();
    while (pointIterator != currentPointList.end()) {

      const int currentPoint = *pointIterator;
      pointIterator++;

      result.elem(currentPoint) = MASK;
      int n;
      for(n=0;n<neigh_.size();n++) { // scan neighborhood
	// calculate absolute neighbor point
	currentNeighbor = currentPoint + neigh_[n];
	// skip invalid neighbors (pixel outside of image)
	if (invalidNeighbor(currentPoint,currentNeighbor)) continue;
	// already processed and not MASKed?
	if (result.elem(currentNeighbor) >= WSHED) {
	  distance.at(currentPoint) = 1;
	  fifoQueue.push(currentPoint);
	  // point is supposed to be pushed only once, so leave
	  // neighborhood process
	  break;
	}
      }
    }
  }
    
  void
  watershedSegmentation::assignCurrLevelPoints(vector<int>& distance,
                                               std::queue<int>& fifoQueue,
                                               matrix<int>& result) const {
    static const int WSHED =  0;
    static const int MASK  = -2;
    static const int fictitiousPixel = -1; // used as a queue marker
    int currentDistance = 1;
    int currentNeighbor,currentPoint;

    fifoQueue.push(fictitiousPixel);

    // process fifo queue
    while (true) {
      currentPoint = fifoQueue.front(); // get first pixel
      fifoQueue.pop(); // remove it

      if (currentPoint == fictitiousPixel) { // marker found

	// all points have been processed and MASKed or declared as
	// watersheds
	if (fifoQueue.empty()) // leave while-loop
	  break;

	// else ...

	fifoQueue.push(fictitiousPixel);
	currentDistance++;
	currentPoint = fifoQueue.front();// get next pixel
	fifoQueue.pop(); //remove it
      }

      int n;
      for(n=0;n<neigh_.size();n++) {
	currentNeighbor = currentPoint + neigh_[n];

	// skip invalid neighbors (pixel outside of image)
	if (invalidNeighbor(currentPoint,currentNeighbor)) continue;

	// neighbor belongs to an already labeled basin or to the watersheds
	if ((distance.at(currentNeighbor) < currentDistance) &&
	    (result.elem(currentNeighbor) >= WSHED)) {

	  if (result.elem(currentNeighbor) > 0) { // neighbor already labeled
	    if ((result.elem(currentPoint) == MASK) ||
		(result.elem(currentPoint) == WSHED)) {
	      result.elem(currentPoint) = result.elem(currentNeighbor);
	    }
	    else if (result.elem(currentPoint)!=result.elem(currentNeighbor)) {
              // different basin in the neighborhood => watershed found
              result.elem(currentPoint) = WSHED;
	    }
	  }
	  else if (result.elem(currentPoint) == MASK)
	    result.elem(currentPoint) = WSHED;
	}
	else if ((result.elem(currentNeighbor) == MASK) &&
		 (distance.at(currentNeighbor) == 0)) {
	  distance.at(currentNeighbor) = currentDistance + 1;
	  fifoQueue.push(currentNeighbor);
	}
      }//for
    }//while
  }

  // check if new minima have been discovered
  // process all pixels of the current gray value again
  // to avoid alloc of a new queue: fifoQueue is taken from parent-methode
  void 
  watershedSegmentation::checkForMins(const list_type& currentPointList,
                                      vector<int>& distance,
                                      std::queue<int>& fifoQueue,
                                      matrix<int>& result,
                                      int& currentLabel) const {
    static const int MASK = -2;
    list_type::const_iterator pointIterator = currentPointList.begin();
    while (pointIterator != currentPointList.end()) {
      const int currentPoint = *pointIterator;
      pointIterator++;

      // distance is reset to 0
      distance.at(currentPoint) = 0;

      // new basin found
      if (result.elem(currentPoint) == MASK) {
	currentLabel++;
	fifoQueue.push(currentPoint);
	result.elem(currentPoint) = currentLabel;

	while (!fifoQueue.empty()) {
	  // get next pixel
	  const int nextPoint = fifoQueue.front();
	  fifoQueue.pop();

	  // scan neighborhood
          int n;
	  for(n=0;n<neigh_.size();n++) {
	    // calculate absolute neighbor point
	    const int currentNeighbor = nextPoint + neigh_[n];
	    // skip invalid neighbors (pixel outside of image)
	    if (invalidNeighbor(nextPoint,currentNeighbor)) continue;
	    // expand basin
	    if (result.elem(currentNeighbor) == MASK) {
	      fifoQueue.push(currentNeighbor);
	      result.elem(currentNeighbor) = currentLabel;
	    }
	  }
	}
      }
    }
  }

  void watershedSegmentation::raiseWaterLevel(
                                    const std::vector<list_type>& sortedPoints,
                                    matrix<int>& result) const {
    std::queue<int> fifoQueue;
    vector<int> distance(imgSize_, 0);

    int currentLabel = 0;
    int currentLevel = 0;

    // check for minimum value used
    currentLabel = 0;
    while (currentLabel<256 && (sortedPoints[currentLabel].empty())) {
      ++currentLabel;
    }
    const int hmin = currentLabel;

    // check for maximum value used
    currentLabel = 255;
    while (currentLabel>hmin && (sortedPoints[currentLabel].empty())) {
      --currentLabel;
    }
    const int hmax = currentLabel;

    // raise water level
    for (currentLevel = hmin; currentLevel <= hmax; currentLevel++) {

      maskCurrLevelPoints  (sortedPoints[currentLevel], distance,
                            fifoQueue, result);
      assignCurrLevelPoints(                            distance, 
                                                        fifoQueue, result);
      checkForMins         (sortedPoints[currentLevel], distance,
                            fifoQueue, result, currentLabel);

#    if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 2)
      static viewer view("Flooding",2,0,1,false);
      view.show(result);
#     if (_LTI_DEBUG >= 3)
      view.waitKey();
#     endif
#    endif
    }
  }

  /** convert the result matrix to appropriate channel8  */
  void watershedSegmentation::copyMatrixToChannel8(const matrix<int>& src,
                                                   channel8& dest) {
    int WSHED =  0; // watersheds have value 0

    // get colors for resulting channel8
    const int watershedColor = getParameters().watershedValue;
    const int basinColor = getParameters().basinValue;

    // 4-neighborhood is sufficient here, so the watersheds won't get too thick
    // only for visualisation (calculation by user-parameter)
    createNeighborhood(src.columns(),false);

    dest.allocate(src.size());

    int currentPoint;
    for (currentPoint = 0; currentPoint < imgSize_; currentPoint++) {
      int currentValue = src.elem(currentPoint);

      if (currentValue == WSHED) // watershed
	dest.elem(currentPoint) = watershedColor;
      else {                     // point is labeled
	// assume point is in basin
	dest.elem(currentPoint) = basinColor;
	// check for adjacent basins
        int n;
	for(n=0;n<neigh_.size();n++) {
	  int currentNeighbor = currentPoint + neigh_[n];
	  // skip invalid neighbors (pixel outside of image)
	  if (invalidNeighbor(currentPoint,currentNeighbor)) continue;
	  if (currentValue != src.elem(currentNeighbor)) {
	    // different basin bordering => declare point as watershed
	    dest.elem(currentPoint) = watershedColor;
	    break; // next currentPoint
	  }
	}
      }
    }
  }

  //----------------------------------------------------------------
  //                       RAINFALLING-WATERSHED
  //----------------------------------------------------------------

  // create regions (numbers by "counter") which are lokal minima(s)
  // rainfalling-method
  void watershedSegmentation::markMinimas(const matrix<int>& downPos,
                                          const channel8& src,
                                          matrix<int>& result) {
    // smelt points of same level and give it a number(counter)
    int* tempRegion = new int[imgSize_];
    const int MASK = -2;
    int countF, countB ;
    int counter = 1; // number of region
    int tempi,tempiNeigh;
    int n,i;
    for(i=0;i<imgSize_;i++) {
      if (result.elem(i)==-1 && downPos.elem(i)<0) { //unused & minima
        // i is point of a new found region
        countF = countB = 0;
        tempRegion[countB++] = i; // put point in queue
        int tempLevel = src.elem(i);
        // find all points which have the same tempLevel like point i
        // and mark them with counter
        while(countF<countB) {
          tempi = tempRegion[countF++]; //get point from queue
          result.elem(tempi) = counter; // label point
          for(n=0; n<neigh_.size(); n++) {
            tempiNeigh = tempi + neigh_[n];
            //tempiNeigh a valid image point
            if( validNeighbor(tempi,tempiNeigh) && 
                result.elem(tempiNeigh)==-1 && // unused
                src.elem(tempiNeigh)==tempLevel ) { //same level
              //each point only once in queue
              tempRegion[countB++] = tempiNeigh; // put point in queue
              result.elem(tempiNeigh) = MASK; // prevent double in queue
            }
          }
        }
        counter++;
      }
    }
    
    delete[] tempRegion;

    _lti_debug("Found lakes/lokalMins: "<< counter << std::endl);

  }
  
  // rainfalling-method
  void  watershedSegmentation::letsRain(const matrix<int>& downPos,
                                        matrix<int>& result) {
    int i,c,tempi;
    vector<int> tempRegion(imgSize_);  // uninitialized vector 

    int regionC;
    for(i=0;i<imgSize_;i++) {
      regionC = 0;
      tempi = i;
      while(result.elem(tempi) == -1) { // unassigned pixel
        tempRegion[regionC++] = tempi;
        tempi = downPos.elem(tempi);
      }
      // a way found down to a lokalMin(lake/point)
      // set all points belong to the way down := tempi,
      // which is the counterNumber of the lokalMin
      int numOfLokalMin = result.elem(tempi);
      for(c=0; c<regionC; c++) {
        result.elem(tempRegion[c]) = numOfLokalMin;
      }
    }
  }

  // rainfalling-method
  void watershedSegmentation::findLowerNeigh(const channel8& src,
                                             matrix<int>& downPos,
                                             channel8& tSrc) {
    static const int lokalMin = -1;
    static const int saddle = -2;
    downPos.assign(src.size(),lokalMin);
    int i,n,max,pos,diff;
    for(i=0;i<imgSize_;i++) {
      max = -1;
      for(n=0; n<neigh_.size(); n++) {
        pos = i + neigh_[n];
        if (invalidNeighbor(i,pos)) continue;
        diff = src.elem(i)-src.elem(pos);
        if(diff > max) { // neigh with lower level
          max = diff;
          downPos.elem(i) = pos;
        }
      }
      if( max == 0 ) { // all neighours are on the same level = saddlePoint
        downPos.elem(i) = saddle;
      }
    }
    // try if a saddlePoint have lower "neigh"
    bool change = true;
    while (change) {

      _lti_debug("saddle" << std::endl);

      change = false;
      for(i=0;i<imgSize_;i++) {
        if(downPos.elem(i) == saddle) {
          for(n=0; n<neigh_.size(); n++) {
            pos = i + neigh_[n];
            if (invalidNeighbor(i,pos)) continue;
            if(src.elem(i)==src.elem(pos) && downPos.elem(pos) >= 0) { 
              // no more saddle, no lokalMin
              downPos.elem(i) = downPos.elem(pos);
              change = true;
              break; // next i
            }
          }
        }
      }
    }

    // remaining saddle points must be lokalMins
    // and all points<threshold are lokalMins
    int threshold = getParameters().threshold;
    tSrc.copy(src);
    for(i=0;i<imgSize_;i++) {
      if (tSrc.elem(i) < threshold) {
        downPos.elem(i) = lokalMin;
        tSrc.elem(i) = threshold;
      }
      else if(downPos.elem(i) == saddle)
        downPos.elem(i) = lokalMin;
    }
  }


  //-------------------------------------------------------------------
  //                  The apply-methods!
  //-------------------------------------------------------------------

  // On place apply for type channel8!
  bool watershedSegmentation::apply(channel8& srcdest) {
    return apply(srcdest, srcdest);
  }

  bool watershedSegmentation::apply(const channel8& src, channel8& dest) {
    matrix<int> result;
    if (apply(src, result)) {
      copyMatrixToChannel8(result, dest);//prepare result (show border)
      return true;
    }
    return false;    
  }

  bool watershedSegmentation::apply(const channel8& src, matrix<int> &result) {

    // the algorithm assumes a connected input image!
    if (src.getMode() != channel8::Connected) {
      // so, not a connected input channel => create a connected one
      const channel8 tmp(src);  
      return apply(src,result);
    }

    // compute offsets (4 or 8 dimensional vector)
    createNeighborhood(src.columns(), getParameters().neighborhood8);
    initBorderLUT(src.size(),borderLUT_);
    
    // number of pixels of the image
    imgSize_ = src.columns() * src.rows();
    
    result.assign(src.size(),-1); // -1 = unused

    if( getParameters().rainfall ) { // rainfalling
      /*
       * according to idea of the rainfallingWatersheds from
       * P. De Smet and Rui Luis V.P.M.Pires
       * http://telin.rug.ac.be/ipi/watershed
       */
      matrix<int> downPos;
      channel8 tSrc;
      findLowerNeigh(src, downPos,tSrc);
      markMinimas(downPos, tSrc, result);
      letsRain(downPos, result);
    }
    else { // standard
      /*
       * according to pseudo code provided in "vincent and soille -
       * watersheds in digital spaces" IEEE Vol.13, No.6, p. 583f
       */
      std::vector<list_type> sortedPoints;
      sortPixels(src, sortedPoints);
      raiseWaterLevel(sortedPoints, result);
      // clear
      for (int waterLevel = 0; waterLevel < 256; waterLevel++) {
	sortedPoints[waterLevel].clear();
      }
      sortedPoints.clear();
    }

    return true;
  }

}
