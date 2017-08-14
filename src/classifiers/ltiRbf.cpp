/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiRbf.cpp
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   11.08.2000
 *
 * $Id: ltiRbf.cpp,v 1.4 2008-08-06 03:40:57 alvarado Exp $
 */

#include "ltiRbf.h"
#include "ltiSort.h"
#include "ltiScramble.h"
#include "ltiManhattanDistance.h"
#include "ltiEuclidianDistance.h"

#include <cstdio>
#include <cstdlib> // for all srand rand etc.
#include <iostream>
#include <fstream>
#include <limits>
#include <ctime>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

namespace lti {


  // --------------------------------------------------
  // rbf::parameters
  // --------------------------------------------------

  // default constructor
  rbf::parameters::parameters()
    : classifier::parameters() {

    // --------------
    // the parameters
    // --------------

    nbPresentations1 = 0;
    nbPresentations2 = 20;
    nbHiddenNeurons = 5;

    learnRate1 = 0.3;
    learnRate2 = 0.1;
    learnFactor = 0.3;
    windowSize = 0.2;
    sigmaFactor = 0.7;
    lambda = 0.0;

    high = 0.99;

    distance = L2Distance;
    lvqInitType = LvqMaxDist;

    lvqType1 = OLVQ1;
    lvqType2 = OLVQ3;

    llsParameters.algorithm=linearLeastSquares::SVD;
    
  }

  // copy constructor
  rbf::parameters::parameters(const parameters& other)
    : classifier::parameters()  {
    copy(other);
  }

  // destructor
  rbf::parameters::~parameters() {
  }

  // get type name
  const std::string& rbf::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  rbf::parameters&
  rbf::parameters::copy(const parameters& other) {
    supervisedInstanceClassifier::parameters::copy(other);

    nbPresentations1 = other.nbPresentations1;
    nbPresentations2 = other.nbPresentations2;
    nbHiddenNeurons  = other.nbHiddenNeurons;

    learnRate1  = other.learnRate1;
    learnRate2  = other.learnRate2;
    learnFactor = other.learnFactor;
    windowSize  = other.windowSize;
    sigmaFactor = other.sigmaFactor;
    lambda      = other.lambda;


    high = other.high;

    distance = other.distance;
    lvqInitType = other.lvqInitType;

    lvqType1 = other.lvqType1;
    lvqType2 = other.lvqType2;

    llsParameters.copy(other.llsParameters);

    return *this;
  }

  // clone member
  rbf::parameters* rbf::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  rbf::parameters* rbf::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool rbf::parameters::write(ioHandler& handler,
                              const bool complete) const {

    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"nbPresentations1",nbPresentations1);
      lti::write(handler,"nbPresentations2",nbPresentations2);
      lti::write(handler,"nbHiddenNeurons",nbHiddenNeurons);

      lti::write(handler,"learnRate1",learnRate1);
      lti::write(handler,"learnRate2",learnRate2);
      lti::write(handler,"learnFactor",learnFactor);
      lti::write(handler,"windowSize",windowSize);
      lti::write(handler,"sigmaFactor",sigmaFactor);
      lti::write(handler,"lambda",lambda);

      lti::write(handler,"high",high);

      lti::write(handler,"distance",distance);
      lti::write(handler,"lvqInitType",lvqInitType);
      
      lti::write(handler,"lvqType1",lvqType1);
      lti::write(handler,"lvqType2",lvqType2);
      
      lti::write(handler,"llsParameters",llsParameters);
      
      b = b && supervisedInstanceClassifier::parameters::write(handler,false);
    }

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
  bool rbf::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"nbPresentations1",nbPresentations1);
      lti::read(handler,"nbPresentations2",nbPresentations2);
      lti::read(handler,"nbHiddenNeurons",nbHiddenNeurons);

      lti::read(handler,"learnRate1",learnRate1);
      lti::read(handler,"learnRate2",learnRate2);
      lti::read(handler,"learnFactor",learnFactor);
      lti::read(handler,"windowSize",windowSize);
      lti::read(handler,"sigmaFactor",sigmaFactor);
      lti::read(handler,"lambda",lambda);

      lti::read(handler,"high",high);

      lti::read(handler,"distance",distance);
      lti::read(handler,"lvqInitType",lvqInitType);
      lti::read(handler,"lvqType1",lvqType1);
      lti::read(handler,"lvqType2",lvqType2);

      lti::read(handler,"llsParameters",llsParameters);

    }

    b = b && supervisedInstanceClassifier::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // lvq classes
  // --------------------------------------------------

  // --------------------------------------------------
  // lvq::lvqTrainFunctor
  // --------------------------------------------------
  /**
   * Constructor
   *
   * @param theProp the distance propagation functor
   */
  rbf::lvqTrainFunctor::
  lvqTrainFunctor(const stdLayer::distancePropFunctor& theProp)
    : prop_(theProp), firstUse_(true) {
  }


  /**
   * Set the learn rate factor
   */
  void rbf::lvqTrainFunctor::setLearnFactor(const double& lrFac){
    learnFactor_ = lrFac;
  }

  /**
   * Set the window size
   */
  void rbf::lvqTrainFunctor::setWindowSize(const double& winSize){
    windowSize_ = (1.0-winSize)/(1.0+winSize);
  }

  void rbf::lvqTrainFunctor::twoMinIndex(const dvector& vct,
                                         int& min1,
                                         int& min2) {
    double themin1,themin2;

    dvector::const_iterator it,e;
    int i;

    if (vct.size()>0) {
      it = vct.begin();
      e=vct.end();
      themin1 = themin2 = *it;
      min1 = min2 = 0;
      it++;
      if (it!=e) {
        if (*it < themin1) {
          themin1=*it;
          min1=1;
        } else {
          themin2=*it;
          min2=1;
        }
      }
      it++;
      for (i=2;it!=e;it++,i++) {
        if (*it < themin2) {
          if (*it < themin1) {
            themin2 = themin1;
            min2 = min1;
            themin1 = *it;
            min1 = i;
          } else {
            themin2 = *it;
            min2 = i;
          }
        }
      }
    }
  }

  // --------------------------------------------------
  // rbf::lvq1TrainFunctor
  // --------------------------------------------------

  rbf::lvq1TrainFunctor::
  lvq1TrainFunctor(stdLayer::distancePropFunctor& theProp)
    : lvqTrainFunctor(theProp) {
  }
  
  bool rbf::lvq1TrainFunctor::operator()(const dvector& input,
                                         dmatrix& weights,
                                         dvector& outLayer,
                                         const ivector& outID,
                                         const int& trainID,
                                         bool& modified) {

    modified = true;

    if (firstUse_) {
      delta_.assign(input.size(),0.);
      firstUse_=false;
    }

    // Propagate
    if (prop_(input,weights,outLayer)) {

      // Lowest exitation = winner
      int winner=outLayer.findIndexOfMinimum();

      delta_.subtract(input,weights[winner]);
      delta_.multiply(learnRate_);

      if (outID[winner]==trainID) {
        delta_.add(weights[winner]);
        weights[winner]=delta_;
      } else {
        delta_.subtract(weights[winner],delta_);
        weights[winner]=delta_;
      }

      return true;
    }

    return false;

  }

  /**
   * Clone
   */
  stdLayer::trainFunctor* rbf::lvq1TrainFunctor::clone() const {
    return new lvq1TrainFunctor(*this);
  }
  
  /**
   * New instance
   */
  stdLayer::trainFunctor* rbf::lvq1TrainFunctor::newInstance() const {
    return new lvq1TrainFunctor(*this);
  }
  
  /**
   * Returns the name of this class
   */
  const std::string& rbf::lvq1TrainFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  

  // --------------------------------------------------
  // rbf::olvq1TrainFunctor
  // --------------------------------------------------

  /**
   * Constructor
   */
  rbf::olvq1TrainFunctor::
  olvq1TrainFunctor(stdLayer::distancePropFunctor& theProp)
    : lvqTrainFunctor(theProp) {
  }

  /**
   * set the learn rate
   */
  void rbf::olvq1TrainFunctor::setLearnRate(const double &theLr) {
    learnRate_ = theLr;
    lr_.fill(theLr);
  }

  /**
   * apply operator
   */
  bool rbf::olvq1TrainFunctor::operator()(const dvector& input,
                                          dmatrix& weights,
                                          dvector& outLayer,
                                          const ivector& outID,
                                          const int& trainID,
                                          bool& modified) {

    modified = true;

    if (firstUse_) {
      firstUse_=false;
      lr_.resize(outLayer.size(),learnRate_);
      delta_.resize(input.size());
    }

    // Propagate
    if (prop_(input,weights,outLayer)) {

      // Lowest exitation = winner
      int winner=outLayer.findIndexOfMinimum();

      delta_.subtract(input,weights[winner]);
      delta_.multiply(lr_[winner]);

      if (outID[winner]==trainID) {
        delta_.add(weights[winner]);
        weights[winner]=delta_;
        lr_[winner]=lr_[winner]/(1+lr_[winner]);
      } else {
        delta_.subtract(weights[winner],delta_);
        weights[winner]=delta_;
        lr_[winner]=lr_[winner]/(1-lr_[winner]);
        if (lr_[winner]>learnRate_) {
          lr_[winner]=learnRate_;
        }
      }

      return true;
    }

    return false;
  }

  /**
   * Clone
   */
  stdLayer::trainFunctor* rbf::olvq1TrainFunctor::clone() const {
    return new olvq1TrainFunctor(*this);
  }
  
  /**
   * New instance
   */
  stdLayer::trainFunctor* rbf::olvq1TrainFunctor::newInstance() const {
    return new olvq1TrainFunctor(*this);
  }
  
  /**
   * Returns the name of this class
   */
  const std::string& rbf::olvq1TrainFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // --------------------------------------------------
  // rbf::lvq3TrainFunctor
  // --------------------------------------------------
  /**
   * Constructor
   */
  rbf::lvq3TrainFunctor::
  lvq3TrainFunctor(stdLayer::distancePropFunctor& theProp)
    : lvqTrainFunctor(theProp) {
  }
  

  bool rbf::lvq3TrainFunctor::operator()(const dvector& input,
                                         dmatrix& weights,
                                         dvector& outLayer,
                                         const ivector& outID,
                                         const int& trainID,
                                         bool& modified) {
    if(firstUse_) {
      delta_.resize(input.size(),0.);
      delta2_.resize(input.size(),0.);
      firstUse_=false;
    }

    modified = false;

    // Propagate
    if (prop_(input,weights,outLayer)) {

      // Lowest exitation = winner
      int winner,second;
      twoMinIndex(outLayer,winner,second);

      delta_.subtract(input,weights[winner]);
      delta_.multiply(learnRate_);
      delta2_.subtract(input,weights[second]);
      delta2_.multiply(learnRate_);

      if ((outID[winner]==trainID || outID[second]==trainID)
          && ((outLayer[winner] < outLayer[second] ?
               outLayer[winner]/outLayer[second] :
               outLayer[second]/outLayer[winner]) > windowSize_))  {
        if(outID[winner]!=outID[second]) {
          if(outID[winner]==trainID) {
            delta_.add(weights[winner]);
            weights[winner]=delta_;
            delta2_.subtract(weights[second],delta2_);
            weights[second]=delta2_;
          } else {
            delta2_.add(weights[second]);
            weights[second]=delta2_;
            delta_.subtract(weights[winner],delta_);
            weights[winner]=delta_;
          }
        } else if(outID[winner]==trainID) {
          delta_.multiply(learnFactor_);
          delta_.add(weights[winner]);
          weights[winner]=delta_;
          delta2_.multiply(learnFactor_);
          delta2_.add(weights[second]);
          weights[second]=delta2_;
        }
        modified = true;
      }
      return true;
    }

    return false;
  }

      
  /**
   * Clone
   */
  stdLayer::trainFunctor* rbf::lvq3TrainFunctor::clone() const {
    return new lvq3TrainFunctor(*this);
  }
  
  /**
   * New instance
   */
  stdLayer::trainFunctor* rbf::lvq3TrainFunctor::newInstance() const {
    return new lvq3TrainFunctor(*this);
  }
  
  /**
   * Returns the name of this class
   */
  const std::string& rbf::lvq3TrainFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // --------------------------------------------------
  // rbf::olvq3TrainFunctor
  // --------------------------------------------------
  /*
   * Constructor
   */
  rbf::olvq3TrainFunctor::
  olvq3TrainFunctor(stdLayer::distancePropFunctor& theProp)
    : lvqTrainFunctor(theProp) {
  }

  void rbf::olvq3TrainFunctor::setLearnRate(const double &theLr) {
    learnRate_=theLr;
    lr_.fill(theLr);
  }

  bool rbf::olvq3TrainFunctor::operator()(const dvector& input,
                                          dmatrix& weights,
                                          dvector& outLayer,
                                          const ivector& outID,
                                          const int& trainID,
                                          bool& modified) {

    modified = false;

    if(firstUse_) {
      delta_.assign(input.size(),0.);
      delta2_.assign(input.size(),0.);
      firstUse_=false;
      lr_.assign(outLayer.size(),learnRate_);
    }

    // Propagate
    if (prop_(input,weights,outLayer)) {

      // Lowest exitation = winner
      int winner,second;
      twoMinIndex(outLayer,winner,second);

      delta_.subtract(input,weights[winner]);
      delta_.multiply(lr_[winner]);
      delta2_.subtract(input,weights[second]);
      delta2_.multiply(lr_[second]);

      if ((outID[winner]==trainID || outID[second]==trainID)
          && ((outLayer[winner] < outLayer[second] ?
               outLayer[winner]/outLayer[second] :
               outLayer[second]/outLayer[winner]) > windowSize_)) {
        if(outID[winner]!=outID[second]) {
          if(outID[winner]==trainID) {
            delta_.add(weights[winner]);
            weights[winner]=delta_;
            delta2_.subtract(weights[second],delta2_);
            weights[second]=delta2_;
            lr_[winner]/=(1+lr_[winner]);
            lr_[second]/=(1-lr_[second]);
            if (lr_[second]>learnRate_) {
              lr_[second]=learnRate_;
            }
          } else {
            delta2_.add(weights[second]);
            weights[second]=delta2_;
            delta_.subtract(weights[winner],delta_);
            weights[winner]=delta_;
            lr_[second]/=(1+lr_[second]);
            lr_[winner]/=(1-lr_[winner]);
            if (lr_[winner]>learnRate_) {
              lr_[winner]=learnRate_;
            }
          }
        } else if (outID[winner]==trainID) {
          delta_.multiply(learnFactor_);
          delta_.add(weights[winner]);
          weights[winner]=delta_;
          delta2_.multiply(learnFactor_);
          delta2_.add(weights[second]);
          weights[second]=delta2_;
          lr_[winner]/=(1+lr_[winner]);
          lr_[second]/=(1+lr_[second]);
        }
        modified=true;
      }
      return true;
    }

    return false;
  }

  /**
   * Clone
   */
  stdLayer::trainFunctor* rbf::olvq3TrainFunctor::clone() const {
    return new olvq3TrainFunctor(*this);
  }
  
  /**
   * New instance
   */
  stdLayer::trainFunctor* rbf::olvq3TrainFunctor::newInstance() const {
    return new olvq3TrainFunctor(*this);
  }
  
  /**
   * Returns the name of this class
   */
  const std::string& rbf::olvq3TrainFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // --------------------------------------------------
  // rbf::lvq4TrainFunctor
  // --------------------------------------------------
  /*
   * Constructor
   */
  rbf::lvq4TrainFunctor::
  lvq4TrainFunctor(stdLayer::distancePropFunctor& theProp)
    : lvqTrainFunctor(theProp) {
  }
      
  bool rbf::lvq4TrainFunctor::operator()(const dvector& input,
                                         dmatrix& weights,
                                         dvector& outLayer,
                                         const ivector& outID,
                                         const int& trainID,
                                         bool& modified) {

    modified = false;

    if(firstUse_) {
      delta_.assign(input.size(),0.);
      delta2_.assign(input.size(),0.);
      firstUse_=false;
    }

    // Propagate
    if (prop_(input,weights,outLayer)) {

      // Lowest exitation = winner
      int winner,second;
      twoMinIndex(outLayer,winner,second);

      delta_.subtract(input,weights[winner]);
      delta_.multiply(learnRate_);
      delta2_.subtract(input,weights[second]);
      delta2_.multiply(learnRate_);

      if ((outID[winner]==trainID || outID[second]==trainID)
          && ((outLayer[winner] < outLayer[second] ?
               outLayer[winner]/outLayer[second] :
               outLayer[second]/outLayer[winner]) > windowSize_))  {
        if(outID[winner]!=outID[second]) {
          if(outID[winner]==trainID) {
            delta_.add(weights[winner]);
            weights[winner]=delta_;
            delta2_.subtract(weights[second],delta2_);
            weights[second]=delta2_;
          } else {
            delta2_.add(weights[second]);
            weights[second]=delta2_;
            delta_.subtract(weights[winner],delta_);
            weights[winner]=delta_;
          }
        } else if(outID[winner]==trainID) {
          delta_.multiply(learnFactor_);
          delta_.add(weights[winner]);
          weights[winner]=delta_;
        }
        modified = true;
      } else if (outID[winner]!=trainID) {
        delta_.multiply(learnFactor_);
        delta_.subtract(weights[winner],delta_);
        weights[winner]=delta_;
        modified = true;
      }
      return true;
    }

    return false;
  }


  /**
   * Clone
   */
  stdLayer::trainFunctor* rbf::lvq4TrainFunctor::clone() const {
    return new lvq4TrainFunctor(*this);
  }
  
  /**
   * New instance
   */
  stdLayer::trainFunctor* rbf::lvq4TrainFunctor::newInstance() const {
    return new lvq4TrainFunctor(*this);
  }
  
  /**
   * Returns the name of this class
   */
  const std::string& rbf::lvq4TrainFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // --------------------------------------------------
  // rbf::randInitFunctor
  // --------------------------------------------------
  /*
   * LVQ ANNs are best initialized with the training data
   * therefore the somewhat bulky constructor
   */
  rbf::randInitFunctor::randInitFunctor(const dmatrix& theFeatures,
                                        const ivector& theTrainIDs,
                                        const ivector& theNbViewsObj,
                                        const int& theNbObj,
                                        const int& theNbNeurObj)
    : stdLayer::initFunctor(),features_(theFeatures),trainIDs_(theTrainIDs),
      nbViewsObj_(theNbViewsObj), nbObj_(theNbObj),nbNeurObj_(theNbNeurObj) {
  }

  bool rbf::randInitFunctor::operator()(dmatrix& weights, ivector& outID) {

    sort::parameters sortParam;
    sort sorter; // default ascending order!

    imatrix randInd(nbObj_,nbNeurObj_,0);

    srand((unsigned)time(0));

    // Find nbNeurObj random training-patterns for each object to
    // initialize the weights

    // For this, first find indices of these patterns and sort them
    // in increasing order

    int k;
    int i;
    ivector temp;

    for(i=0; i<nbObj_; i++) { // for each available class
      const int numPatternFori = nbViewsObj_.at(i); // available number of
                                                    // patterns for class i
      temp.allocate(numPatternFori);
      int j;
      for(j=0;j<numPatternFori;j++) {
        temp.at(j)=j;
      }
      const int toj = min(nbNeurObj_,numPatternFori);
      for(j=0; j<toj ;j++) { //
        k=int((double(rand())/RAND_MAX)*(numPatternFori - j));

        randInd[i][j]=temp[k];
        temp[k]=temp[numPatternFori-j-1];

      }
      sorter.apply(randInd.getRow(i));
    }

    ivector p(nbObj_,0);
    ivector s(nbObj_,0);

    // Now go through the training patterns once and for the chosen indices
    // copy the pattern into weights and assign a classID to the output neuron

    int j=0;
    for (i=0; i<trainIDs_.size(); i++) {
      if (s[trainIDs_[i]]==randInd[trainIDs_[i]][p[trainIDs_[i]]]) {
        weights[j]=features_[i];
        outID[j++]=trainIDs_[i];
        p[trainIDs_[i]]++;
      }
      s[trainIDs_[i]]++;
      if (p[trainIDs_[i]]==nbNeurObj_) {
        i += nbViewsObj_[trainIDs_[i]] - 
             randInd[trainIDs_[i]][p[trainIDs_[i]]-1]-1;
      }
    }

    return true;
  }
  
  /*
   * Returns the name of this class
   */
  const std::string& rbf::randInitFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::initFunctor* rbf::randInitFunctor::clone() const {
    return new randInitFunctor(*this);
  }

  /*
   * newInstance function.
   */
  stdLayer::initFunctor* rbf::randInitFunctor::newInstance() const {
    return new randInitFunctor(*this);
  }

  // --------------------------------------------------
  // rbf::maxDistInitFunctor
  // --------------------------------------------------

  rbf::maxDistInitFunctor::maxDistInitFunctor(const dmatrix& theFeatures,
                                              const ivector& theTrainIDs,
                                              const ivector& theNbViewsObj,
                                              const int& theNbObj,
                                              const int& theNbNeurObj,
                                              const eDistanceType& theDistance)
  : stdLayer::initFunctor(),features_(theFeatures), trainIDs_(theTrainIDs),
    nbViewsObj_(theNbViewsObj), nbObj_(theNbObj), nbNeurObj_(theNbNeurObj),
    distance_(theDistance) {
  }


  bool rbf::maxDistInitFunctor::operator()(dmatrix& weights, ivector& outID){

    int farthest=0; //feature vector farthest form others of the same
                    // class already selected

    int views=nbViewsObj_.findMaximum();

    dmatrix distance;
    dvector absvalue;
    dvector cumDist;

    dmatrix clsVectors;
    dvector tmp;

    for(int obj=0; obj<nbObj_; obj++) {

      views = nbViewsObj_[obj];

      // find the vectors in features_ that belong to the class obj
      clsVectors=classVectors(features_, trainIDs_, views, obj);

      distance.assign(views,views,0.);
      absvalue.assign(views,0.);
      cumDist.resize(views,0.);

      //Find feature vector with lowest absolute value
      //(gotta start somewhere :-)
      int i;
      for(i=0; i<views; i++) {
        tmp=clsVectors.getRow(i);
        absvalue[i]=tmp.dot(tmp);
      }

      farthest=absvalue.findIndexOfMinimum();
      weights.setRow(obj*nbNeurObj_, clsVectors.getRow(farthest));
      outID[obj*nbNeurObj_]=obj;

      //Find the distances between prototypes the same class
      if (distance_==L1Distance) {
        for(i=0; i<views; i++) {
          for(int j=i+1; j<views; j++) {
            distance[i][j]=manhattanDistance(clsVectors[i],clsVectors[j]);
            distance[j][i]=distance[i][j];
          }
        }
      } else {

        for(i=0; i<views; i++) {
          for(int j=i+1; j<views; j++) {
            distance[i][j]=euclidianDistance(clsVectors[i],clsVectors[j]);
            distance[j][i]=distance[i][j];
          }
        }
      }

      //Always find the feature vector with the greatest mean (geometric)
      //distance to the already selected prototypes of each class

      cumDist=distance[farthest];
      for(i=1; i<nbNeurObj_; i++) {
        farthest=cumDist.findIndexOfMaximum();
        weights.setRow(obj*nbNeurObj_+i, clsVectors.getRow(farthest));
        outID[obj*nbNeurObj_+i]=obj;
        cumDist.emultiply(distance[farthest]);
      }

    }


    return true;
  }

  /*
   * Returns the name of this class
   */
  const std::string& rbf::maxDistInitFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::initFunctor* rbf::maxDistInitFunctor::clone() const {
    return new maxDistInitFunctor(*this);
  }

  /*
   * newInstance function.
   */
  stdLayer::initFunctor* rbf::maxDistInitFunctor::newInstance() const {
    return new maxDistInitFunctor(*this);
  }

  // --------------------------------------------------
  // rbf::rbInitFunctor1
  // --------------------------------------------------

  rbf::initFunctor1::initFunctor1(const dmatrix& theFeatures,
                                  const ivector& theTrainIDs,
                                  const ivector& theNbViewsObj,
                                  const int& theNbObj,
                                  const int& theNbNeurObj,
                                  const eLvqInit& theLvqInit,
                                  const eDistanceType& theDistance)
    : features_(theFeatures),trainIDs_(theTrainIDs),nbViewsObj_(theNbViewsObj),
      nbObj_(theNbObj),nbNeurObj_(theNbNeurObj),lvqInit_(theLvqInit),
      distance_(theDistance), linear_(stdLayer::linearActFunctor()),
      distProp_(linear_,theDistance), trainer_(0), changed_(true),
      index_(theFeatures.rows()),dummy_(theNbObj*theNbNeurObj), step_(0),
      modified_(true) {
  }

  rbf::initFunctor1::~initFunctor1() {
   
    delete trainer_;
    trainer_=0;
  }

  bool rbf::initFunctor1::operator()(dmatrix& weights, ivector& outID) {
    // The initialization of the first layer of a RBF net is analog to the
    // training of a LVQ net. Thus, this function is analog to the train() of
    // the LVQ net

    // In the first step the weights of this layer must be initialized.
    if (step_==0) { // step_ is set to zero at construction time

      if (lvqInit_ == LvqRand) {
        rbf::randInitFunctor initFunc(features_, trainIDs_, nbViewsObj_,
                                      nbObj_, nbNeurObj_);
        initFunc(weights, outID);
      } else {
        rbf::maxDistInitFunctor initFunc(features_, trainIDs_, nbViewsObj_,
                                         nbObj_, nbNeurObj_,distance_);
        initFunc(weights, outID);
      }

      //Initializations
      step_++;
      for(int i=0;i<features_.rows();i++) {
        index_[i]=i;
      }

      modified_ = true;

    } else if (modified_) { //Perform a training step

      // if the type of lvqTrainFunctor was changed since last use, set
      // trainer to the appropriate functor
      if (changed_) {
        delete trainer_;
        trainer_ = 0;

        switch (lvqType_) {
          case LVQ1: {
            trainer_ = new lvq1TrainFunctor(distProp_);
            trainer_->setLearnRate(learnRate_);
          }
          break;

          case OLVQ1: {
            trainer_ = new olvq1TrainFunctor(distProp_);
            trainer_->setLearnRate(learnRate_);
          }
          break;

          case LVQ3: {
            trainer_ = new lvq3TrainFunctor(distProp_);
            trainer_->setLearnFactor(lrFac_);
            trainer_->setWindowSize(winSize_);
            trainer_->setLearnRate(learnRate_);
          }
          break;

          case OLVQ3: {
            trainer_ = new olvq3TrainFunctor(distProp_);
            trainer_->setLearnFactor(lrFac_);
            trainer_->setWindowSize(winSize_);
            trainer_->setLearnRate(learnRate_);
          }
          break;

          case LVQ4: {
            trainer_ = new lvq4TrainFunctor(distProp_);
            trainer_->setLearnFactor(lrFac_);
            trainer_->setWindowSize(winSize_);
            trainer_->setLearnRate(learnRate_);
          }
          break;

          default: {
            return false; // "unknown eLvqTrainType in rbfInitFunctor1!");
          }
        }
      }

      //present training data in different order on each rum
      scramble scrambler;
      scrambler.apply(index_);

      modified_ = false;
      bool modStep;

      //present each training vector
      for (int i=0;i<index_.size();i++) {
        (*trainer_)(features_[index_[i]], weights, dummy_, outID,
                    trainIDs_[index_[i]], modStep);
        modified_ = modified_ || modStep;
      }

      step_++;
      changed_=false;
    }

    return true;
  }

  void rbf::initFunctor1::setTrainParams(const eLvqTrainType& theTrainType,
                                         const double& theLearnRate,
                                         const double& theLrFac,
                                         const double& theWinSize) {

    lvqType_=theTrainType;
    learnRate_=theLearnRate;
    lrFac_=theLrFac;
    winSize_=theWinSize;
    if (winSize_==0. && (lvqType_==LVQ3 || lvqType_==LVQ4) ) {
      std::string txt;
      txt="initFunctor1::setTrainParams needs 4 parameters for LVQ3/4!";
      throw exception(txt);
    }

    if ((lrFac_!=0. || winSize_!=0.) && (lvqType_==LVQ1 ||
                                         lvqType_==OLVQ1)) {
      std::string txt;
      txt="initFunctor1::setTrainParams needs 2 parameters for (O)LVQ1! only";
      throw exception(txt);
    }

    changed_=true;
  }

  /*
   * Returns the name of this class
   */
  const std::string& rbf::initFunctor1::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * clone function.
   */
  stdLayer::initFunctor* rbf::initFunctor1::clone() const {
    return new initFunctor1(*this);
  }
  
  /*
   * newInstance function.
   */
  stdLayer::initFunctor* rbf::initFunctor1::newInstance() const {
    return new initFunctor1(*this);
  }

  // --------------------------------------------------
  // rbf::rbfInitFunctor2
  // --------------------------------------------------


  rbf::initFunctor2::initFunctor2(dmatrix& theH,
                                  dmatrix& theHtilde,
                                  const double & theLambda,
                                  const ivector& theTrainIDs,
                                  const linearLeastSquares::parameters& lls)
    : H_(theH),Htilde_(theHtilde),trainIDs_(theTrainIDs),
      lambda_(theLambda),lls_(lls) {
  }


  bool rbf::initFunctor2::operator()(dmatrix& weights, ivector& outID) {

    //Initialize the IDs
    int i,j;
    for(i=0;i<outID.size();i++) {
      outID[i]=i;
    }

    // Computation of the matrix H^T Y
    //    
    const double starter=1;
    dmatrix Y(H_.rows(),outID.size(), -starter);
    for (i=0;i<trainIDs_.size();i++) {
      Y.at(i,trainIDs_[i]) = starter;
    }

    dmatrix Ht;
    Ht.transpose(H_);

    dmatrix HtY;
    HtY.multiply(Ht,Y);

    // compute the matrix H^T H + lambda Htilde by hand to save memory and time
    dmatrix dummy(Htilde_.size()); // reserve the space
    const int l = Htilde_.rows();

    // This is somehow tricky.
    //
    // Assuming Htilde is symmetrical (which is always the case with
    // L2 and L1 distances), we can also assume that the resulting matrix
    // is symmetrical
    for (i=0;i<l;++i) {
      for (j=0;j<=i;++j) {
        dummy.at(i,j) = dummy.at(j,i) =
          Ht.getRow(i).dot(Ht.getRow(j))-lambda_*Htilde_.at(i,j);
      }
    }

    linearLeastSquares lls(lls_);
    lls.apply(dummy,HtY,weights);
    weights.transpose();

    return true;
  }

  /*
   * Returns the name of this class
   */
  const std::string& rbf::initFunctor2::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /*
   * clone function.
   */
  stdLayer::initFunctor* rbf::initFunctor2::clone() const {
    return new initFunctor2(*this);
  }
  
  /*
   * newInstance function.
   */
  stdLayer::initFunctor* rbf::initFunctor2::newInstance() const {
    return new initFunctor2(*this);
  }

  // --------------------------------------------------
  // rbf::layer1
  // --------------------------------------------------
  rbf::layer1::layer1(std::string theName) 
    : stdLayer(theName) {
  }

  rbf::layer1::~layer1() {
  }

  bool rbf::layer1::train(const dvector& ,
                          const int& ,
                          trainFunctor& ) {

    std::string text = "Training for RBF nets is not implemented!\n";
    text += "Initialization is sufficient for use";

    setStatusString(text.c_str());
    return false;
  }

  void rbf::layer1::getH(const dmatrix& features,
                         stdLayer::propagationFunctor& prop,
                         dmatrix& H) {

    for(int i=0;i<H.rows();i++) {
      prop(features[i],weights_,H[i]);
    }
  }

  void rbf::layer1::getHtilde(stdLayer::propagationFunctor& prop,
                              dmatrix& Htilde) {
    for(int i=0;i<sizeOut_;i++) {
      prop(weights_[i],weights_,Htilde[i]);
    }
  }


  void rbf::layer1::findSigmas(const double& sigmaFac,
                               dvector& sigmas) {
    // Get number of neurons per class from class information
    // works since there is an equal number of neurons for each
    // class (yet :-)
    int nbNeurObj=1;
    int firstID=outID_[0];
    while(outID_[nbNeurObj]==firstID) {
      nbNeurObj++;
    }

    dmatrix distance(sizeOut_,sizeOut_,std::numeric_limits<double>::max());

    //Find the distances between prototypes of different classes
    //Distance between same classes is set to big number 1E+99
    int i;

    for(i=0; i<sizeOut_; i++) {
      for(int j=i+(nbNeurObj-i%nbNeurObj); j<sizeOut_; j++) {
        distance.at(j,i) =
          distance.at(i,j) = euclidianDistance(weights_.getRow(i),
                                               weights_.getRow(j));
      }
    }

    //Set sigmas proportional to smallest distance to other class
    //prototype
    for(i=0;i<sizeOut_;i++) {
      sigmas[i]=sigmaFac*(distance[i].findMinimum());
    }
  }

  void rbf::layer1::setTrainParams(rbf::initFunctor1& initFtor,
                                   const eLvqTrainType& theTrainType,
                                   const double& theLearnRate,
                                   const double& theLrFac,
                                   const double& theWinSize) {

    initFtor.setTrainParams(theTrainType,theLearnRate,theLrFac,theWinSize);
  }

  /*
   * Returns the name of this class
   */
  const std::string& rbf::layer1::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * clone function.
   */
  stdLayer* rbf::layer1::clone() const {
    return new layer1(*this);
  }
  
  /*
   * newInstance function.
   */
  stdLayer* rbf::layer1::newInstance() const {
    return new layer1(layerName_);
  }

  // --------------------------------------------------
  // rbf::layer2
  // --------------------------------------------------

  rbf::layer2::layer2(std::string theName) 
    : stdLayer(theName) {
  }

  rbf::layer2::~layer2() {
  }


  bool rbf::layer2::train(const dvector&,
                          const int&,
                          trainFunctor&) {

    std::string text = "Training for RBF nets is not implemented!\n";
    text += "Initialization is sufficient for use";

    setStatusString(text.c_str());
    return false;
  }


  /*
   * Returns the name of this class
   */
  const std::string& rbf::layer2::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * clone function.
   */
  stdLayer* rbf::layer2::clone() const {
    return new layer2(*this);
  }
  
  /*
   * newInstance function.
   */
  stdLayer* rbf::layer2::newInstance() const {
    return new layer2(layerName_);
  }

  // --------------------------------------------------
  // rbf
  // --------------------------------------------------

  rbf::rbf()
    : supervisedInstanceClassifier(),
      rbf1_("Layer 1"),rbf2_("Layer 2"),distProp_(0),
      dotProp_(0),gaussAct_(0),sigmoidAct_(0) {

    // default parameters
    parameters par;
    _lti_debug("Setting params\n");
    setParameters(par);
    _lti_debug("Done Setting params, creating template\n");

  }

  rbf::rbf(const rbf& other)
    : supervisedInstanceClassifier(),
      rbf1_("Layer 1"),rbf2_("Layer 2"),distProp_(0),
      dotProp_(0),gaussAct_(0),sigmoidAct_(0) {
    copy(other);
  }

  rbf::rbf(const parameters& param)
    : supervisedInstanceClassifier(),
      rbf1_("Layer 1"),rbf2_("Layer 2"),distProp_(0),dotProp_(0),
      gaussAct_(0),sigmoidAct_(0) {

    setParameters(param);
  }

  rbf::~rbf() {
    delete distProp_;
    delete dotProp_;
    delete gaussAct_;
    delete sigmoidAct_;
  }

  // get type name
  const std::string& rbf::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  bool rbf::train(const dmatrix& input,
                  const ivector& ids) {

    _lti_debug("ltiRbf::train\n");

    if (input.empty()) {
      setStatusString("Train data empty");
      return false;
    }


    if (ids.size()!=input.rows()) {
      std::string str;
      str = "dimensionality of IDs vector and the number of rows ";
      str+= "of the input matrix must have the same size.";
      setStatusString(str.c_str());
      return false;
    }

    const parameters& param = getParameters();

    if (haveValidProgressObject()) {

      getProgressObject().reset();
      getProgressObject().setTitle("RBF: Training");
      getProgressObject().setMaxSteps(param.nbPresentations1+
                                      param.nbPresentations2+3);

    }

    // extract usefull information from the input data

    // how many objects are in the ids vector?
    realToIntern_.clear();

    std::map<int,int>::iterator mit;
    std::vector<int> nbViewsObjTmp;
    std::vector<int> internToRealTmp;

    // initialize the tables realToIntern, internToReal and nbViewsObj
    int counter = 0;

    _lti_debug("constructing maps\n");

    int i;
    for (i=0;i<ids.size();i++) {
      mit = realToIntern_.find(ids.at(i));
      if (mit==realToIntern_.end()) {
        realToIntern_[ids.at(i)]=counter;
        internToRealTmp.push_back(ids.at(i));
        nbViewsObjTmp.push_back(1);
        counter++;
      }
      else {
        // increment the number of patterns per object
        nbViewsObjTmp[(*mit).second]++;
      }
    }

    internToReal_.castFrom(internToRealTmp);

    // number of patterns available to train each class
    // (internal ids are the indices)
    ivector nbViewsObj(nbViewsObjTmp);

    // the number of classes present in the training set 
    // (number of output neurons):
    sizeOut_=counter;

    // number of input neurons is equal to the number of dimensions of the
    // input patterns.
    sizeIn_=input.columns();

    _lti_debug("init members\n");

    // initialize the layers

    // first layer with sizeIn_ inputs and nbHiddenNeurons for each
    // output neuron.
    rbf1_.setSize(sizeIn_,param.nbHiddenNeurons*sizeOut_);

    // second layer has the nbHiddenNeurons for each output unit (there are 
    // as many output units as number of classes).
    rbf2_.setSize(param.nbHiddenNeurons*sizeOut_, sizeOut_);

    // each hidden unit has its own std. deviation sigma.
    sigmas_.resize(param.nbHiddenNeurons*sizeOut_, 0.);

    // ensure that at least one Id for each output neuron exists.
    internToReal_.resize(sizeOut_,0);

    // shadow all training patterns
    features_.copy(input);

    // initialize the member trainID, which contains the internal
    // class id for the respective input vector
    trainID_.allocate(ids.size());

    for (i=0;i<ids.size();i++) {
      trainID_.at(i) = realToIntern_[ids.at(i)];
    }

    initFunctor1 layerOneInit(features_,
                              trainID_,
                              nbViewsObj,
                              sizeOut_,
                              param.nbHiddenNeurons,
                              param.lvqInitType,
                              param.distance);


    if (haveValidProgressObject()) {
      getProgressObject().step("Initializing layer 1");
    }

    _lti_debug("initialize first layer\n");

    // Initialization of LVQ-training
    rbf1_.initWeights(layerOneInit);

    if (param.lvqType1!=NO_LVQ) {
      _lti_debug("doing first training\n");
      rbf1_.setTrainParams(layerOneInit, param.lvqType1, param.learnRate1);
      std::string display;
      char buf[50];

      if (haveValidProgressObject()) {
        display="layer 1 ";
        display+= (param.lvqType1==LVQ1 ?
                   "LVQ1 step " :
                   "OLVQ1 step ");
      }

      for (i=0; i<param.nbPresentations1; i++) {
        // unsupported by g++: itoa(i,buf,10);
        // workaround:

        _lti_debug(".");
        if (haveValidProgressObject()) {
          sprintf(buf,"%i",i);

          getProgressObject().step(display+buf);
        }

        rbf1_.initWeights(layerOneInit);
      }
    }

    if (param.lvqType2 != NO_LVQ) {
      _lti_debug("doing second training\n");
      std::string display;
      rbf1_.setTrainParams(layerOneInit,
                           param.lvqType2,
                           param.learnRate2,
                           param.learnFactor,
                           param.windowSize);

      if (haveValidProgressObject()) {
        display="layer 1 ";
        switch (param.lvqType2) {
          case LVQ3: {
            display+="LVQ3 step ";
          }
          break;

          case LVQ4: {
            display+="LVQ4 step ";
          }
          break;

          case OLVQ3: {
            display+="OLVQ3 step ";
          }
          break;

          case NO_LVQ:
            break;

          case LVQ1: {
            display+="LVQ1 step ";
          }
          break;

          case OLVQ1:  {
            display+="OLVQ1 step ";
          }
          break;
        }
      }

      for (i=0; i<param.nbPresentations2; i++) {
        _lti_debug(".");

        if (haveValidProgressObject()) {
          char buf[50];
          sprintf(buf,"%i",i);
          getProgressObject().step(display+buf);
        }

        rbf1_.initWeights(layerOneInit);
      }
    }

    _lti_debug("calculating sigmas\n");
    rbf1_.findSigmas(param.sigmaFactor, sigmas_);

    if (haveValidProgressObject()) {
      getProgressObject().step("Initializing layer 2");
    }

    delete gaussAct_;
    gaussAct_=new stdLayer::gaussActFunctor(0., sigmas_);

    delete sigmoidAct_;
    sigmoidAct_ = new stdLayer::sigmoidActFunctor(-log(1/param.high-1));

    delete distProp_;
    distProp_ = new stdLayer::distancePropFunctor(*gaussAct_, param.distance);

    delete dotProp_;
    dotProp_ = new stdLayer::dotPropFunctor(*sigmoidAct_);

    _lti_debug("training second layer\n");

    // Computation of the Moore-Penrose Pseudo Inverse
    int outDim = sizeOut_*param.nbHiddenNeurons;
    dmatrix H(features_.rows(), outDim, 0.);
    dmatrix Htilde(outDim,outDim, 0.);

    rbf1_.getH(features_, *distProp_, H);
    rbf1_.getHtilde(*distProp_, Htilde);
    initFunctor2 rbfFunctor2(H, Htilde, param.lambda, trainID_,
                             param.llsParameters);
    rbf2_.initWeights(rbfFunctor2);

    // Calculate object probabilities
    if(!calcObjProbs(trainID_, features_)) {
      setStatusString("Very bad training! One class never activated!");
      return false;
    }
    
    if (haveValidProgressObject()) {
      getProgressObject().step("Training ready.");
    }


    return true;

  }

  bool rbf::classify(const dvector& feature, result& res) const {


    dvector rbf1Output, rbf2Output;

    bool b=true;

    if (notNull(distProp_)) {
      b = b && rbf1_.propagate(feature,*distProp_,rbf1Output);
    }

    if (notNull(dotProp_)) {
      b = b && rbf2_.propagate(rbf1Output,*dotProp_,rbf2Output);
    }

    // abs. value = 1 for prob. distribution
    double numerator = rbf2Output.computeSumOfElements();
    if (numerator != 0.0) {
      numerator = 1.0/numerator;
    }

    dvector outStat;
    outStat.multiply(rbf2Output,numerator);

    b = b && (outStat.size()==outTemplate_.size());

    if (b) {
      bool ov=outTemplate_.apply(outStat, res);
      res.setWinnerAtMax();
      return ov;
    } else {
      outStat=dvector(outTemplate_.size(), 0.);
      outTemplate_.apply(outStat, res);
      res.setReject(true);
      return false;
    }
  }


  bool rbf::write(ioHandler& handler,const bool complete) const {
    bool b(true);

    if (complete) {
      b = handler.writeBegin();
    }

    // write the standard data (output and parameters)
    supervisedInstanceClassifier::write(handler,false);

    if (b) {
      lti::write(handler,"rbf1",rbf1_);
      lti::write(handler,"rbf2",rbf2_);
      lti::write(handler,"sizeIn",sizeIn_);
      lti::write(handler,"sizeOut",sizeOut_);
      lti::write(handler,"sigmas",sigmas_);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool rbf::read(ioHandler& handler,const bool complete) {
    bool b(true);

    if (complete) {
      b = handler.readBegin();
    }

    // read the standard data (output and parameters)
    supervisedInstanceClassifier::read(handler,false);

    const parameters& param = getParameters();

    if (b) {
      lti::read(handler,"rbf1",rbf1_);
      lti::read(handler,"rbf2",rbf2_);
      lti::read(handler,"sizeIn",sizeIn_);
      lti::read(handler,"sizeOut",sizeOut_);
      lti::read(handler,"sigmas",sigmas_);

      if (sigmas_.size() != sizeOut_*param.nbHiddenNeurons) {
        sigmas_.resize(sizeOut_*param.nbHiddenNeurons,0.7,CopyAndInit);
      }

      delete gaussAct_;
      gaussAct_ = new stdLayer::gaussActFunctor(0.,sigmas_);

      delete sigmoidAct_;
      sigmoidAct_ = new stdLayer::sigmoidActFunctor(-log(1/param.high-1));

      delete distProp_;
      distProp_ = new stdLayer::distancePropFunctor(*gaussAct_,param.distance);

      delete dotProp_;
      dotProp_ = new stdLayer::dotPropFunctor(*sigmoidAct_);

    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;

  }

  bool rbf::calcObjProbs(ivector& internIds,
                         dmatrix &feats) {

    bool allOk=true;
    lti::matrix<int> clCount(sizeOut_,sizeOut_,0);

    int i,j,k;
    result outV;
    ivector ideez(sizeOut_);
    for (i=0; i<sizeOut_; i++) {
      ideez.at(i)=i;
    }
    outTemplate_=outputTemplate(ideez);

    for(i=0;i<feats.rows();i++) {
      allOk = classify(feats[i], outV) && allOk; // this calls setWinnerAtMax()
      clCount.at(outV.getWinner(),internIds[i])++; // just recall the winner
    }

    double rowsum;
    int rowsize;
    outTemplate_=outputTemplate(sizeOut_);
    for(i=0;i<sizeOut_;i++) {
      rowsum=clCount[i].computeSumOfElements();
      result rowV;
      if (rowsum!=0) {
        rowsize=0;
        for (j=0;j<sizeOut_;j++) {
          if (clCount[i][j]!=0) {
            rowsize++;
          }
        }
        rowV=result(rowsize);
        for(j=0,k=0;j<sizeOut_;j++) {
          if (clCount[i][j]!=0) {
            rowV.setPair(k++, internToReal_[j], clCount[i][j]/rowsum);
          }
        }
      } else {
        allOk=false;
      }
      outTemplate_.setProbs(i, rowV);
    }
    outTemplate_.setIds(internToReal_);

    return allOk;
  }

  void rbf::setTestSet(const dmatrix& input,
                       const ivector& ids) {

    testIDs_.copy(ids);
    testFeatures_.copy(input);
  }


  // return parameters
  const rbf::parameters& rbf::getParameters() const {
    const parameters* par =
      dynamic_cast<const rbf::parameters*>(&classifier::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  rbf& rbf::copy(const rbf& other) {
    supervisedInstanceClassifier::copy(other);

    rbf1_ = other.rbf1_;
    rbf2_ = other.rbf2_;

    sizeIn_  = other.sizeIn_;
    sizeOut_ = other.sizeOut_;

    trainID_.copy(other.trainID_);
    internToReal_.copy(other.internToReal_);
    realToIntern_ = other.realToIntern_;
    features_.copy(other.features_);

    sigmas_ = other.sigmas_;
    testFeatures_ = other.testFeatures_;
    testIDs_ = other.testIDs_;
    trainStatIDs_ = other.trainStatIDs_;

    targetVec_ = other.targetVec_;

    delete distProp_;
    if (notNull(other.distProp_)) {
      distProp_ = new stdLayer::distancePropFunctor(*other.distProp_);
    } else {
      distProp_ = 0;
    }

    delete dotProp_;
    if (notNull(other.dotProp_)) {
      dotProp_ = new stdLayer::dotPropFunctor(*other.dotProp_);
    } else {
      dotProp_ = 0;
    }

    delete gaussAct_;
    if (notNull(other.gaussAct_)) {
      gaussAct_ = new stdLayer::gaussActFunctor(*other.gaussAct_);
    } else {
      gaussAct_ = 0;
    }

    delete sigmoidAct_;
    if (notNull(other.sigmoidAct_)) {
      sigmoidAct_ = new stdLayer::sigmoidActFunctor(*other.sigmoidAct_);
    } else {
      sigmoidAct_ = 0;
    }

    return *this;
  }

  classifier* rbf::clone() const {
    return new rbf(*this);
  }

  classifier* rbf::newInstance() const {
    return new rbf;
  }

  const dmatrix& rbf::getWeights1() {
    return rbf1_.getWeights();
  }

  const dmatrix& rbf::getWeights2() {
    return rbf2_.getWeights();
  }

  const dvector& rbf::getSigmas1() {
    return sigmas_;
  }

  const ivector& rbf::getIds() {
    return rbf2_.getOutID();
  }

  void rbf::setSigmas(const dvector& s) {
    sigmas_.copy(s);
  }

  void rbf::setLayer1Data(const dmatrix& weights) {
    rbf1_.setSize(weights.columns(), weights.rows());
    rbf1_.setWeights(weights);
    ivector v(weights.rows(),int(0));
    rbf1_.setIDs(v);
  }

  void rbf::setLayer2Data(const dmatrix& weights, const ivector& ids) {
    rbf2_.setSize(weights.columns(), weights.rows());
    rbf2_.setWeights(weights);
    rbf2_.setIDs(ids);
  }

  void rbf::initNet(const int& inSize, const int& outSize) {

    const parameters& param = getParameters();

    sizeIn_=inSize;
    sizeOut_=outSize;

    delete gaussAct_;
    gaussAct_ = new stdLayer::gaussActFunctor(0.,sigmas_);

    delete sigmoidAct_;
    sigmoidAct_ = new stdLayer::sigmoidActFunctor(-log(1/param.high-1));

    delete distProp_;
    distProp_ = new stdLayer::distancePropFunctor(*gaussAct_,param.distance);

    delete dotProp_;
    dotProp_ = new stdLayer::dotPropFunctor(*sigmoidAct_);

  }


  /**
   * Read a rbf::eLvqInit
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,rbf::eLvqInit& data) {
    std::string str;
    
    if (handler.read(str)) {  
      if (str == "LvqRand") {
        data = rbf::LvqRand;
      } else if (str == "LvqMaxDist") {
        data = rbf::LvqMaxDist;
      } else {
        data = rbf::LvqMaxDist;
      }
      return true;
    }
    return false;
  }

  /**
   * Write a rbf::eLvqInit
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const rbf::eLvqInit& lvqInitType) {
    bool b=false;
    switch(lvqInitType) {
      case rbf::LvqRand:
        b = handler.write("LvqRand");
        break;
      case rbf::LvqMaxDist:
        b = handler.write("LvqMaxDist");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined rbf::eLvqInit");
        break;
    }
    return b;
  }

  /**
   * Read a rbf::eLvqTrainType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,rbf::eLvqTrainType& data) {
    std::string str;
    
    if (handler.read(str)) {  
      if (str == "NO_LVQ") {
        data = rbf::NO_LVQ;
      } else if (str == "LVQ1") {
        data = rbf::LVQ1;
      } else if (str == "OLVQ1") {
        data = rbf::OLVQ1;
      } else if (str == "LVQ3") {
        data = rbf::LVQ3;
      } else if (str == "OLVQ3") {
        data = rbf::OLVQ3;
      } else if (str == "LVQ4") {
        data = rbf::LVQ4;
      } else {
        data = rbf::OLVQ1;
      }
      return true;
    }
    return false;
  }

  /**
   * Write a rbf::eLvqTrainType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const rbf::eLvqTrainType& data) {
    bool b=false;

    switch(data) {
      case rbf::NO_LVQ:
        handler.write("NO_LVQ");
        break;
      case rbf::LVQ1:
        handler.write("LVQ1");
        break;
      case rbf::OLVQ1:
        handler.write("OLVQ1");
        break;
      case rbf::LVQ3:
        handler.write("LVQ3");
        break;
      case rbf::OLVQ3:
        handler.write("OLVQ3");
        break;
      case rbf::LVQ4:
        handler.write("LVQ4");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined rbf::eLvqTrainType");
        break;
    }
    return b;
  }


}
