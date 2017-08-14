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
 * \file   ltiColorProbabilityMap.cpp
 *         Contains the class lti::colorProbabilityMapBase, an abstract class
 *         of all functors computing color probability maps.
 * \author Benjamin Winkler
 * \author Florian Bley
 * \author Pablo Alvarado
 *
 * \date   30.01.2001
 *
 * revisions ..: $Id: ltiColorProbabilityMapBase.cpp,v 1.2 2008-12-28 04:18:34 alvarado Exp $
 */

#include "ltiHistogram.h"
#include "ltiColorProbabilityMapBase.h"

namespace lti {
  // --------------------------------------------------
  // colorProbabilityMapBase::parameters
  // --------------------------------------------------

  // default constructor
  colorProbabilityMapBase::parameters::parameters()
    : functor::parameters() {

    objectProbability = 0.5;

    iterations = 1;
    gaussian = false;
    windowSize = 5;
    variance = -1;
  }

  // copy constructor
  colorProbabilityMapBase::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  colorProbabilityMapBase::parameters::~parameters() {
  }

  // get type name
  const std::string& colorProbabilityMapBase::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  colorProbabilityMapBase::parameters&
  colorProbabilityMapBase::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    // just copy the references
    objectProbability = other.objectProbability;

    iterations = other.iterations;
    gaussian   = other.gaussian;
    windowSize = other.windowSize;
    variance   = other.variance;

    return *this;
  }

  // alias for copy member
  colorProbabilityMapBase::parameters&
  colorProbabilityMapBase::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  colorProbabilityMapBase::parameters*
  colorProbabilityMapBase::parameters::clone() const {
    return new parameters(*this);
  }

  colorProbabilityMapBase::parameters*
  colorProbabilityMapBase::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool colorProbabilityMapBase::parameters::write(ioHandler& handler,
                                                  const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"objectProbability",objectProbability);

      lti::write(handler,"iterations",iterations);
      lti::write(handler,"gaussian",gaussian);
      lti::write(handler,"windowSize",windowSize);
      lti::write(handler,"variance",variance);
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
  bool colorProbabilityMapBase::parameters::read(ioHandler& handler,
                                                 const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"objectProbability",objectProbability);

      lti::read(handler,"iterations",iterations);
      lti::read(handler,"gaussian",gaussian);
      lti::read(handler,"windowSize",windowSize);
      lti::read(handler,"variance",variance);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // colorProbabilityMapBase
  // --------------------------------------------------

  // default constructor
  colorProbabilityMapBase::colorProbabilityMapBase()
    : functor(),
      ownObjModel_(true),ownNonObjModel_(true),
      nonObjectColorModel_(0),objectColorModel_(0) {

    probabilityHistogram_.clear();
  }

  // copy constructor
  colorProbabilityMapBase::
  colorProbabilityMapBase(const colorProbabilityMapBase& other)
    : functor(),
      ownObjModel_(true),ownNonObjModel_(true),
      nonObjectColorModel_(0),objectColorModel_(0) {

    probabilityHistogram_.clear();

    copy(other);
  }

  // destructor
  colorProbabilityMapBase::~colorProbabilityMapBase() {
    if (ownObjModel_) {
      delete objectColorModel_;
      objectColorModel_ = 0;
    }

    if (ownNonObjModel_) {
      delete nonObjectColorModel_;
      nonObjectColorModel_ = 0;
    }

  }

  // returns the name of this type
  const std::string& colorProbabilityMapBase::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  colorProbabilityMapBase&
  colorProbabilityMapBase::copy(const colorProbabilityMapBase& other) {
    functor::copy(other);

    // we have to be careful deciding if we should remove a model
    if (other.objectColorModel_ != objectColorModel_) {
      // the models differ: we can get on normally
      if (ownObjModel_) {
        delete objectColorModel_;
        objectColorModel_ = 0;
      }
      ownObjModel_ = other.ownObjModel_;
      if (ownObjModel_) {
        // now we own it, so we need our own copy
        if (notNull(other.objectColorModel_)) {
         objectColorModel_ = other.objectColorModel_->clone();
        } else {
          objectColorModel_ = 0;
        }
      } else {
        // we don't own it, so just get the reference 
        objectColorModel_ = other.objectColorModel_;
      }
    } else {
      // both objects share exactly the same model!
      if (other.ownObjModel_) {
        // the other owns the model, so we do now too...
        ownObjModel_ = other.ownObjModel_;
        if (notNull(other.objectColorModel_)) {
          objectColorModel_ = other.objectColorModel_->clone();
        } else {
          objectColorModel_ = 0;
        }
      } 

      // else (the other does NOT own the object)
      //   the other has the same model but just a reference:
      //   do we own it?
      //     if not, so leave it just as it is: we simply do not own it
      //     if yes, we cannot remove it, since it would invalidate
      //             the other one and we would get invalid too...
    }

    if (other.nonObjectColorModel_ != nonObjectColorModel_) {
      // the models differ: we can get on normally
      if (ownNonObjModel_) {
        delete nonObjectColorModel_;
        nonObjectColorModel_ = 0;
      }
      ownNonObjModel_ = other.ownNonObjModel_;
      if (ownNonObjModel_) {
        // now we own it, so we need our own copy
        if (notNull(other.nonObjectColorModel_)) {
          nonObjectColorModel_ = other.nonObjectColorModel_->clone();
        } else {
          nonObjectColorModel_ = 0;
        }
      } else {
        // we don't own it, so just get the reference 
        nonObjectColorModel_ = other.nonObjectColorModel_;
      }
    } else {
      // both objects share exactly the same model!
      if (other.ownNonObjModel_) {
        // the other owns the model, so we do now too...
        ownNonObjModel_ = other.ownNonObjModel_;
        if (notNull(other.nonObjectColorModel_)) {
          nonObjectColorModel_ = other.nonObjectColorModel_->clone();
        } else {
          nonObjectColorModel_ = 0;
        }
      } 

      // else (the other does NOT own the object)
      //   the other has the same model but just a reference:
      //   do we own it?
      //     if not, so leave it just as it is: we simply do not own it
      //     if yes, we cannot remove it, since it would invalidate
      //             the other one and we would get invalid too...
    }

    // after copying successfuly the parameters, the state
    // of the functors must be updated
    generate();

    return (*this);
  }

  // copy member
  colorProbabilityMapBase&
    colorProbabilityMapBase::operator=(const colorProbabilityMapBase& other) {
    return copy(other);
  }


  // return parameters
  const colorProbabilityMapBase::parameters&
    colorProbabilityMapBase::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  /*
   * setParameters needs to be overloaded to avoid reloading the
   * histograms every time apply is called.
   */
  bool colorProbabilityMapBase::updateParameters() {
    generate();  // generate can return false if setParameters() is called
                 // before any model is set, so we just try, but if not, then
                 // is all the same...  It will be called when the models are
                 // set.

    return true;
  }


  /**
   * generate lookup tables for faster element access
   */
  void 
  colorProbabilityMapBase::generateLookupTable(const ivector &dimensions) {

    int numberOfDim ( dimensions.size() );
    lookupTable_.allocate(numberOfDim, 256); // matrix of numberOfDim rows and
                                            // 256 columns

    for (int dimCounter = 0; dimCounter < numberOfDim; dimCounter++) {
      vector<ubyte> &row = lookupTable_.getRow(dimCounter);
      for (int elemCounter = 0; elemCounter < 256; elemCounter++) {
        // map to 0 .. (dimensions-1)
        row[elemCounter] = elemCounter * dimensions[dimCounter] / 256;
      }
    }
  }


  /*
   * generate map from 2 histograms
   */
  bool colorProbabilityMapBase::generate(const dhistogram &objectModel,
                                         const dhistogram &nonObjectModel) {

    double objectProbability = getParameters().objectProbability;
    const ivector& histogramSize = objectModel.getCellsPerDimension();

    // check for same size
    if (!(nonObjectModel.getCellsPerDimension() == histogramSize)) {
      setStatusString("Histograms must have same size and dimensions");
      return false;
    }

    // initialize probability histogram
    probabilityHistogram_.resize(histogramSize.size(), histogramSize);

    // scan all 3 histograms
    dhistogram::const_iterator objectIt    = objectModel.begin();
    dhistogram::const_iterator nonObjectIt = nonObjectModel.begin();
    dhistogram::iterator probabilityIt = probabilityHistogram_.begin();

    double objNumEntries = objectModel.getNumberOfEntries();
    double nonObjNumEntries = nonObjectModel.getNumberOfEntries();

    // avoid division by zero
    if (objNumEntries == 0.0) {
      objNumEntries = 1.0;  // no difference since all entries are 0.0
    }

    // avoid division by zero
    if (nonObjNumEntries == 0.0) {
      nonObjNumEntries = 1.0;  // no difference since all entries are 0.0
    }

    double relObjProb;
    double relNonObjProb;
    const double nonObjectProbability = (1.0-objectProbability);

    while (objectIt != objectModel.end()) {

      relObjProb = (*objectIt) * objectProbability / objNumEntries;
      relNonObjProb = (*nonObjectIt) * nonObjectProbability / nonObjNumEntries;

      // assume non-object if no entries are given
      if ((relObjProb == 0) && (relNonObjProb == 0)) {
        (*probabilityIt) = 0.0;
      } else {
        // bayes
        (*probabilityIt) = relObjProb / (relObjProb + relNonObjProb);
      }

      objectIt++;
      nonObjectIt++;
      probabilityIt++;
    }

    return true;
  }

  /*
   * generate map of 1 histogram and an equipartition representing the
   * non object model
   */
  bool colorProbabilityMapBase::generate(const dhistogram &objectModel) {

    double objectProbability = getParameters().objectProbability;
    const ivector& histogramSize = objectModel.getCellsPerDimension();

    // calculate constant non object value = 1.0 / numberOfCells
    // (equal distribution assumed)
    double constNonObjectValue = 1.0;
    ivector::const_iterator vecEntry = histogramSize.begin();
    while (!(vecEntry == histogramSize.end())) {
      constNonObjectValue /= (*(vecEntry++));
    }

    // scan both histograms
    dhistogram::const_iterator objectIt = objectModel.begin();
    dhistogram::iterator probabilityIt  = probabilityHistogram_.begin();

    double objNumEntries = objectModel.getNumberOfEntries();

    // avoid division by zero
    if (objNumEntries == 0.0) {
      objNumEntries = 1.0;  // no difference since all entries are 0.0
    }

    double relNonObjProb = constNonObjectValue * (1.0 - objectProbability);
    double relObjProb;
    while (objectIt != objectModel.end()) {

      relObjProb = (*objectIt) * objectProbability / objNumEntries;

      // bayes
      (*probabilityIt) = relObjProb / (relObjProb + relNonObjProb);

      objectIt++;
      probabilityIt++;
    }

    return true;
  }

  /**
   * generate probability map
   */
  bool colorProbabilityMapBase::generate() {

    if (!isObjectColorModelValid()) {
      setStatusString("No object color model specified yet");
      return false;
    }

    probabilityHistogram_.clear();

    // initialize probability histogram
    const ivector& histogramSize = objectColorModel_->getCellsPerDimension();
    probabilityHistogram_.resize(histogramSize.size(), histogramSize);

    generateLookupTable(histogramSize);

    // generate map
    if (!isNonObjectColorModelValid()) {  //ToDo: probably cases matrix
                                                //overflow constant non-object
                                                //model

      return generate(*objectColorModel_);
    } else {
      return generate(*objectColorModel_, *nonObjectColorModel_);
    }

    return false;
  }

  bool colorProbabilityMapBase::write(ioHandler& handler,
                                      const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (!functor::write(handler,false)) {
      return false;
    }

    dhistogram dummy;
    
    if (notNull(nonObjectColorModel_)) {
      b = lti::write(handler,"nonObjectColorModel", *nonObjectColorModel_) &&b;
    } else {
      b = lti::write(handler,"nonObjectColorModel", dummy) && b;
    }
    
    if (notNull(objectColorModel_)) {
      b = lti::write(handler,"objectColorModel", *objectColorModel_) && b;
    } else {
      b = lti::write(handler,"objectColorModel", dummy) && b;
    }

    if (complete) {
      b = handler.writeEnd() && b;
    }

    return b;
  }

  bool colorProbabilityMapBase::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (!functor::read(handler,false)) {
      return false;
    }

    if (isNull(nonObjectColorModel_)) {
      nonObjectColorModel_ = new dhistogram;
      ownNonObjModel_ = true;
    }
    
    // since the attributes are const, we need to read them in a copy, and
    // then replace them... (slow but there is no other (safe) way
    dhistogram* model = nonObjectColorModel_->clone();
    b = lti::read(handler,"nonObjectColorModel", *model) && b;

    if (ownNonObjModel_) {
      delete nonObjectColorModel_;
    }
    nonObjectColorModel_ = model;
    ownNonObjModel_ = true;
    
    if (isNull(objectColorModel_)) {
      objectColorModel_ = new dhistogram;
      ownObjModel_ = true;
    }

    model = objectColorModel_->clone();
    b = lti::read(handler,"objectColorModel", *model) && b;
    if (ownObjModel_) {
      delete objectColorModel_;
    }
    objectColorModel_ = model;
    ownObjModel_ = true;

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  /*
   * set object color model
   */
  bool
  colorProbabilityMapBase::setObjectColorModel(const dhistogram& objModel) {
    if (ownObjModel_) {
      delete objectColorModel_;
    } else {
      ownObjModel_ = true;
    }

    objectColorModel_ = objModel.clone();

    return generate();
  }

  bool
  colorProbabilityMapBase::useObjectColorModel(const dhistogram& objModel) {
    if (&objModel == objectColorModel_) {
      return true; // same object, do nothing!
    }

    if (ownObjModel_) {
      delete objectColorModel_;
      ownObjModel_ = false;
    } 

    objectColorModel_ = &objModel;

    return generate();
  }

  /*
   * get object color model
   */
  const dhistogram& colorProbabilityMapBase::getObjectColorModel() const {
    return *objectColorModel_;
  }

  /*
   * set non-object color model
   */
  bool
  colorProbabilityMapBase::setNonObjectColorModel(const dhistogram&
                                                  nonObjModel) {
    if (ownNonObjModel_) {
      delete nonObjectColorModel_;
    } else {
      ownNonObjModel_ = true;
    }

    nonObjectColorModel_ = nonObjModel.clone();

    return generate();
  }

  bool
  colorProbabilityMapBase::useNonObjectColorModel(const dhistogram&
                                                  nonObjModel) {
    if (&nonObjModel == nonObjectColorModel_) {
      return true; // ready, same object!
    }

    if (ownNonObjModel_) {
      delete nonObjectColorModel_;
      ownNonObjModel_ = false;
    } 

    nonObjectColorModel_ = &nonObjModel;

    return generate();
  }

  /*
   * get non-object color model
   */
  const dhistogram&
  colorProbabilityMapBase::getNonObjectColorModel() const {
    return *nonObjectColorModel_;
  }

  bool
  colorProbabilityMapBase::setColorModels(const dhistogram& objModel,
                                          const dhistogram& nonObjModel) {
    if (ownObjModel_) {
      delete objectColorModel_;
    } else {
      ownObjModel_ = true;
    }

    objectColorModel_ = objModel.clone();

    if (ownNonObjModel_) {
      delete nonObjectColorModel_;
    } else {
      ownNonObjModel_ = true;
    }

    nonObjectColorModel_ = nonObjModel.clone();

    return generate();
  }

  bool
  colorProbabilityMapBase::useColorModels(const dhistogram& objModel,
                                          const dhistogram& nonObjModel,
                                          const bool forceRegeneration) {

    bool changed = false;
    if (&objModel != objectColorModel_) {
      
      if (ownObjModel_) {
        delete objectColorModel_;
        ownObjModel_ = false;
      } 
      
      objectColorModel_ = &objModel;

      changed = true;
    }

    if (&nonObjModel != nonObjectColorModel_) {

      if (ownNonObjModel_) {
        delete nonObjectColorModel_;
        ownNonObjModel_ = false;
      } 
      
      nonObjectColorModel_ = &nonObjModel;

      changed = true;
    }

    if (changed || forceRegeneration) {
      return generate();
    }

    // nothing changed!
    return true;
  }

}
