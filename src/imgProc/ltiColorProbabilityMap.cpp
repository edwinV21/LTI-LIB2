/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
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
 * \file   ltiColorProbabilityMap.h
 *         Contains the class lti::colorProbabilityMap, the main derived class
 *         from colorProbabilityMapBase, that uses 3D color histograms.
 * \author Benjamin Winkler
 * \author Florian Bley
 * \author Pablo Alvarado
 *
 * \date   30.01.2001
 *
 * revisions ..: $Id: ltiColorProbabilityMap.cpp,v 1.1 2008-12-27 20:14:56 alvarado Exp $
 */

#include "ltiColorProbabilityMap.h"
#include "ltiSquareConvolution.h"
#include "ltiConvolution.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiGaussKernels.h"

namespace lti {

  // --------------------------------------------------
  // colorProbabilityMap::parameters
  // --------------------------------------------------

  // inherited from base class

  // --------------------------------------------------
  // colorProbabilityMap
  // --------------------------------------------------

  // default constructor
  colorProbabilityMap::colorProbabilityMap() : colorProbabilityMapBase() {
  }

  colorProbabilityMap::colorProbabilityMap(const parameters& theParams)
    : colorProbabilityMapBase() {
    setParameters(theParams);
  }

  // copy constructor
  colorProbabilityMap::colorProbabilityMap(const colorProbabilityMap& other)
    : colorProbabilityMapBase()  {
    copy(other);
  }

  // destructor
  colorProbabilityMap::~colorProbabilityMap() {
  }

  // returns the name of this type
  const std::string& colorProbabilityMap::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  colorProbabilityMap&
  colorProbabilityMap::copy(const colorProbabilityMap& other) {
    colorProbabilityMapBase::copy(other);

    return (*this);
  }

  // copy member
  colorProbabilityMap&
  colorProbabilityMap::operator=(const colorProbabilityMap& other) {
    return copy(other);
  }

  // clone member
  colorProbabilityMap* colorProbabilityMap::clone() const {
    return new colorProbabilityMap(*this);
  }

  // clone member
  colorProbabilityMap* colorProbabilityMap::newInstance() const {
    return new colorProbabilityMap;
  }

  // return parameters
  const colorProbabilityMap::parameters&
  colorProbabilityMap::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  /*
   * read the functor parameters
   */
  bool colorProbabilityMap::read(ioHandler& handler, const bool complete) {
    // we must ensure that the proper histogram type is indicated
    dhistogram dummy;
    setColorModels(dummy,dummy);
    return colorProbabilityMapBase::read(handler,complete);
  }

  bool colorProbabilityMap::isObjectColorModelValid() const {
    return (notNull(objectColorModel_) &&
            (objectColorModel_->getDimensions() == 3));
  }

  bool colorProbabilityMap::isNonObjectColorModelValid() const {
    return (notNull(nonObjectColorModel_) &&
            (nonObjectColorModel_->getDimensions() == 3));
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // return probability channel
  bool colorProbabilityMap::apply(const image& src,channel& dest) const{

    const parameters& param = getParameters();

    // the color model MUST have 3 dimensions!
    if ( (probabilityHistogram_.getDimensions() != 3) ||
         (!isObjectColorModelValid()) ) {
      setStatusString("No valid models set yet");
      return false;
    }

    // resize probability channel
    dest.allocate(src.size());

    ivector theBin(3);

    // compute first iteration
    int y;
    vector<rgbaPixel>::const_iterator srcIterator;
    vector<channel::value_type>::iterator destIterator;
    for (y=0;y<src.rows();++y) {
      srcIterator = src.getRow(y).begin();
      const vector<rgbaPixel>::const_iterator eit = src.getRow(y).end();
      destIterator = dest.getRow(y).begin();
      while (srcIterator != eit) {

        theBin[0] = lookupTable_[0][(*srcIterator).getRed()];
        theBin[1] = lookupTable_[1][(*srcIterator).getGreen()];
        theBin[2] = lookupTable_[2][(*srcIterator).getBlue()];
        (*destIterator)=static_cast<float>(probabilityHistogram_.at(theBin));

        srcIterator++;
        destIterator++;
      }
    }

    // compute all other iterations
    if (param.iterations > 1) {
      int i;

      if (param.gaussian) {
        gaussKernel2D<float> gk(param.windowSize,param.variance);
        convolution convolver;
        convolution::parameters convParam;

        convParam.boundaryType = lti::Mirror;
        convParam.setKernel(gk);
        convolver.setParameters(convParam);

        for (i=1;i<param.iterations;++i) {
          convolver.apply(dest);
          computeMap(src,dest);
        }
      } else {
        squareConvolution<float> convolver;
        squareConvolution<float>::parameters convParam;

        convParam.boundaryType = lti::Mirror;
        convParam.initSquare(param.windowSize);

        convolver.setParameters(convParam);

        for (i=1;i<param.iterations;++i) {
          convolver.apply(dest);
          computeMap(src,dest);
        }
      }
    }

    return true;
  }



  bool colorProbabilityMap::apply(const image& src,
                                        channel& dest, 
                                  const channel &apriori) const{
    channel probChannel(apriori);
    const parameters& param = getParameters();
    bool returnValue=true;

    // the color model MUST have 3 dimensions!
    if ( (probabilityHistogram_.getDimensions() != 3) ||
         (!isObjectColorModelValid()) ) {
      setStatusString("No valid models set yet");
      return false;
    }

    // resize probability channel
    dest.allocate(src.size());    

    if (probChannel.size()!=src.size()) {
      probChannel.resize(src.size(),param.objectProbability);
      returnValue=false;
      setStatusString("colorProbabilityMap: apriori channel has wrong size");
    }
        
    // compute iterations
    if (param.iterations > 0) {
      int i;
      
      if (param.gaussian) {
        gaussKernel2D<float> gk(param.windowSize,param.variance);
        convolution convolver;
        convolution::parameters convParam;
        
        convParam.boundaryType = lti::Mirror;
        convParam.setKernel(gk);
        convolver.setParameters(convParam);
        
        for (i=0;i<param.iterations;++i) {
          convolver.apply(probChannel);
          computeMap(src,probChannel);
        }
      } else {
        squareConvolution<float> convolver;
        squareConvolution<float>::parameters convParam;
        
        convParam.boundaryType = lti::Mirror;
        convParam.initSquare(param.windowSize);
        
        convolver.setParameters(convParam);
        
        for (i=0;i<param.iterations;++i) {
          convolver.apply(probChannel);
          computeMap(src,probChannel);
        }
      }
    }      
    dest=probChannel;
    return returnValue;
  }

  /*
   * compute the second and up iterations of a probability map
   * using the given apriori probabilites per pixel.
   */
  void colorProbabilityMap::computeMap(const image& img,
                                       channel& aPrioriDest) const{

    int y;
    vector<rgbaPixel>::const_iterator srcIterator,eit;
    vector<channel::value_type>::iterator destIterator;

    const dhistogram& objModel    = getObjectColorModel();
    const dhistogram& nonObjModel = getNonObjectColorModel();

    float relObjProb;
    float relNonObjProb;

    ivector theBin(3);

    for (y=0;y<img.rows();++y) {
      srcIterator = img.getRow(y).begin();
      eit = img.getRow(y).end();
      destIterator = aPrioriDest.getRow(y).begin();

      while (srcIterator != eit) {
        theBin[0] = lookupTable_[0][(*srcIterator).getRed()];
        theBin[1] = lookupTable_[1][(*srcIterator).getGreen()];
        theBin[2] = lookupTable_[2][(*srcIterator).getBlue()];

        relObjProb = static_cast<float>(objModel.getProbability(theBin) *
                                        (*destIterator));
        relNonObjProb = static_cast<float>(nonObjModel.getProbability(theBin)*
                                           (1.0f-(*destIterator)));

        // assume non-object if no entries are given
        if ((relObjProb == 0.0f) && (relNonObjProb == 0.0f)) {
          (*destIterator) = 0.0f;
        } else {
          // bayes
          (*destIterator) = relObjProb / (relObjProb + relNonObjProb);
        }

        srcIterator++;
        destIterator++;
      }
    }
  }

  // return probability value of an rgb pixel
  float colorProbabilityMap::apply(const rgbaPixel &src,
                                   ivector& theBin) const {

    assert((probabilityHistogram_.getDimensions() == 3) &&
           (theBin.size() == 3));

    theBin[0] = lookupTable_[0][src.getRed()];
    theBin[1] = lookupTable_[1][src.getGreen()];
    theBin[2] = lookupTable_[2][src.getBlue()];

    return static_cast<float>(probabilityHistogram_.at(theBin));
  }

  // return probability value of an rgb pixel
  float colorProbabilityMap::apply(const rgbaPixel &src) const{

    assert (probabilityHistogram_.getDimensions() == 3);

    ivector theBin(3);

    theBin[0] = lookupTable_[0][src.getRed()];
    theBin[1] = lookupTable_[1][src.getGreen()];
    theBin[2] = lookupTable_[2][src.getBlue()];

    return static_cast<float>(probabilityHistogram_.at(theBin));
  }
 
} // of namespace
