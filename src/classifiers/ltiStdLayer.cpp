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
 * \file   ltiStdLayer.h
 * \author Peter Doerfler
 * \date   11.08.2000
 *
 * $Id: ltiStdLayer.cpp,v 1.2 2008-04-04 20:38:31 alvarado Exp $
 */

#include "ltiObject.h"
#include "ltiStdLayer.h"
#include "ltiLispStreamHandler.h"
#include "ltiMath.h"

#include "ltiEuclidianDistance.h"
#include "ltiManhattanDistance.h"

#include <set>   // hash_set changed to set due to its absence in MSVC++...

namespace lti {

  // ----------------------------------------------
  // initialization functor
  // ----------------------------------------------
  /*
   * Default constructor
   */
  stdLayer::initFunctor::initFunctor() : object() {
  }
  
  /*
   * Destructor
   */
  stdLayer::initFunctor::~initFunctor() {
  }

  ivector stdLayer::initFunctor::uniqueIDs(const ivector& allIDs) const {

    // hash_set changed to set due to its absence in MSVC++...
    std::set<int> hset;
    int len=allIDs.size();
    int i;

    for (i=0; i<len; i++) {
      hset.insert(allIDs.at(i));
    }

    ivector unique(static_cast<int>(hset.size()));

    std::set<int>::iterator it=hset.begin();

    i=0;
    while (it!=hset.end()) {
      unique.at(i)=*it++;
    }

    return unique;
  }

  dmatrix stdLayer::initFunctor::classVectors(const dmatrix& allVectors,
                                              const ivector& allIDs,
                                              int nbViews,
                                              int id) const {


    dmatrix cv(nbViews, allVectors.columns());
    
    int j=0;
    for (int i=0; i<allIDs.size(); i++) {
      if (allIDs.at(i) == id) {
        cv.setRow(j++, allVectors.getRow(i));
      }
    }

    return cv;
  }

  // ----------------------------------------------
  // linearActFunctor
  // ----------------------------------------------

  bool stdLayer::linearActFunctor::operator()(dvector&) const {
    return true;
  }
  
  /*
   * Returns the name of this class
   */
  const std::string& stdLayer::linearActFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::activationFunctor* stdLayer::linearActFunctor::clone() const {
    return new linearActFunctor;
  }

  /*
   * newInstance function.
   */
  stdLayer::activationFunctor*
  stdLayer::linearActFunctor::newInstance() const {
    return new linearActFunctor;
  }

  // ----------------------------------------------
  // gaussActFunctor
  // ----------------------------------------------
  /*
   * Constructor for a gaussian activation function with mean value theMu and
   * std. deviation theSigma
   */
  stdLayer::gaussActFunctor::gaussActFunctor(const double& theMu,
                                             const double& theSigma)
    : mu_(theMu),sigma_(theSigma),sigmaVec_(0) {
  }
  
  /*
   * Constructor for a gaussian activation function with
   * mean value theMu and std. deviation vector theSigma
   */
  stdLayer::gaussActFunctor::gaussActFunctor(const double& theMu,
                                             const dvector& theSigmaVec)
    : mu_(theMu),sigma_(-1),sigmaVec_(&theSigmaVec) {
  }


  bool stdLayer::gaussActFunctor::operator()(dvector& output) const {
    static const double fac = sqrt(2*Pi);
    if (sigma_==-1.) {
      dvector::iterator it,e;
      dvector::const_iterator vit;
      for(it=output.begin(),vit=sigmaVec_->begin(),e=output.end();
          it!=e;
          it++,vit++) {
        double tmp = *it-mu_;
        *it = lti::exp(-(tmp*tmp)/(2*(*vit)*(*vit)))/(fac*(*vit));
      }
    } else {
      dvector::iterator it,e;
      for(it=output.begin(),e=output.end();
          it!=e;
          it++) {
        double tmp = *it - mu_;
        *it =  lti::exp(-(tmp*tmp)/(2*sigma_*sigma_))/(fac*sigma_);
      }
    }
    return true;
  }

  const std::string& stdLayer::gaussActFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::activationFunctor* stdLayer::gaussActFunctor::clone() const {
    return new gaussActFunctor(mu_,sigma_);
  }

  /*
   * newInstance function.
   */
  stdLayer::activationFunctor* stdLayer::gaussActFunctor::newInstance() const {
    return new gaussActFunctor(0.0,1.0);
  }

  // ----------------------------------------------
  // sigmoidActFunctor
  // ----------------------------------------------

  /*
   * Constructor with slope
   */
  stdLayer::sigmoidActFunctor::sigmoidActFunctor(const double& theSlope) :
    slope_(theSlope) {
  }
  
  /*
   * Default constructor with a slope 1.
   */
  stdLayer::sigmoidActFunctor::sigmoidActFunctor() : slope_(1.0) {
  }

  const std::string& stdLayer::sigmoidActFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::activationFunctor* stdLayer::sigmoidActFunctor::clone() const {
    return new sigmoidActFunctor(slope_);
  }

  /*
   * newInstance function.
   */
  stdLayer::activationFunctor* 
  stdLayer::sigmoidActFunctor::newInstance() const {
    return new sigmoidActFunctor;
  }

  bool stdLayer::sigmoidActFunctor::operator()(dvector& output) const {
    dvector::iterator it,e;
    if (slope_!=1) {
      for(it=output.begin(),e=output.end();
          it!=e;
          it++) {
        *it = 1.0/(1.0+lti::exp(-slope_*(*it)));
      }
    } else {
      for(it=output.begin(),e=output.end();
          it!=e;
          it++) {
        *it = 1.0/(1.0+lti::exp(-*it));
      }
    }
    return true;
  }

  // ----------------------------------------------
  // stdLayer::propagationFunctor
  // ----------------------------------------------
   /*
    * Defines a linear activation function
    */
  stdLayer::propagationFunctor::
  propagationFunctor(const activationFunctor& theAct) : act_(theAct) {
  }

  // ----------------------------------------------
  // stdLayer::distancePropFunctor
  // ----------------------------------------------
  /*
   * Default constructor
   *
   * For Propagation L1 and L2 distance can be used
   */
  stdLayer::distancePropFunctor::
  distancePropFunctor(const activationFunctor& theAct,eDistanceType theDistance)
    : propagationFunctor(theAct),distance_(theDistance) {
  }
  
  /*
   * l1 distance
   */
  bool stdLayer::distancePropFunctor::l1(const dmatrix& a,
                                         const dvector& b,
                                         dvector& dest) const {

    assert(a.columns()==b.size());
    
    int i=0;
    const int r=a.rows();

    dest.allocate(r);
    dvector::iterator it=dest.begin();

    for (i=0; i < r; i++, ++it) {
      *it = manhattanDistance(a.getRow(i),b);
    }
    
    return true;
  }

  /*
   * l2 distance
   */
  bool stdLayer::distancePropFunctor::l2(const dmatrix& a,
                                         const dvector& b,
                                         dvector& dest) const {

    assert(a.columns() == b.size());
    
    int i=0;
    const int r=a.rows();

    dest.allocate(r);
    dvector::iterator it=dest.begin();

    for (i=0; i < r; i++, ++it) {
      *it = euclidianDistance(a.getRow(i),b);
    }
    
    return true;
  }
  
  bool stdLayer::distancePropFunctor::operator()(const dvector& input,
                                                 const dmatrix& weights,
                                                 dvector& outLayer) const {

    switch(distance_) {
      case L1Distance: {
        l1(weights,input,outLayer);
      }
      break;
      case L2Distance: {
        l2(weights,input,outLayer);
      }
      break;
      default:
        // Distanceally impossibly to get here, but it's C++ after all ;-)
        return false; 
    }
    return act_(outLayer);
  }

  const std::string& stdLayer::distancePropFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::propagationFunctor*
  stdLayer::distancePropFunctor::clone() const {
    return new distancePropFunctor(*this);
  }

  /*
   * newInstance function.
   */
  stdLayer::propagationFunctor*
  stdLayer::distancePropFunctor::newInstance() const {
    return new distancePropFunctor(act_,distance_);
  }


  // ----------------------------------------------
  // stdLayer::dotPropFunctor
  // ----------------------------------------------
  /*
   * Default constructor
   */
  stdLayer::dotPropFunctor::dotPropFunctor(const activationFunctor& theAct)
    : propagationFunctor(theAct) {
  }


  bool stdLayer::dotPropFunctor::operator()(const dvector& input,
                                            const dmatrix& weights,
                                            dvector& outLayer) const {

    weights.multiply(input,outLayer);
    return act_(outLayer);
  }

  const std::string& stdLayer::dotPropFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clone function.
   */
  stdLayer::propagationFunctor* stdLayer::dotPropFunctor::clone() const {
    return new dotPropFunctor(*this);
  }

  /*
   * newInstance function.
   */
  stdLayer::propagationFunctor* stdLayer::dotPropFunctor::newInstance() const {
    return new dotPropFunctor(act_);
  }


  // ----------------------------------------------
  // stdLayer::trainFunctor
  // ----------------------------------------------
  /*
   * Default constructor
   * Both neccessary since there are nets which are trained
   * with an overall net error, not a layer error
   */
  stdLayer::trainFunctor::trainFunctor() : object() {
  }

  /*
   * Destructor
   */
  stdLayer::trainFunctor::~trainFunctor() {
  }

  /*
   * Set the learn rate factor
   */
  void stdLayer::trainFunctor::setLearnRate(const double & lr) {
    learnRate_ = lr;
  }

  /*
   * The functor operator for supervised training requires
   * following parameters:
   * @param input the input vector
   * @param weights the weights matrix to be changed
   * @param outLayer the output layer vector
   * @param outID the class identification for each output neuron
   * @param trainID the class ID for the presented vector
   * @param netError the error of the layer
   * @param modified indicates whether the weights were modified
   */
  bool stdLayer::trainFunctor::operator()(const dvector& ,
                                          dmatrix& ,
                                          dvector& ,
                                          const ivector& ,
                                          const int& ,
                                          dvector& ,
                                          bool& ) {
    return false; // throw exception("training functor not implemented");
  }

  /*
   * The functor operator for unsupervised training requires
   * following parameters:
   * @param input the input vector
   * @param weights the weights matrix to be changed
   * @param outLayer the output layer vector
   * @param netError the error of the layer
   * @param modified indicates whether the weights were modified
   */
  bool stdLayer::trainFunctor::operator()(const dvector& ,
                                          dmatrix& ,
                                          dvector& ,
                                          dvector& ,
                                          bool& ) {
    return false; // throw exception("training functor not implemented");
  }

  /*
   * The functor operator for supervised training requires
   * following parameters:
   * @param input the input vector
   * @param weights the weights matrix to be changed
   * @param outLayer the output layer vector
   * @param outID the class identification for each output neuron
   * @param trainID the class ID for the presented vector
   * @param modified indicates whether the weights were modified
   * @return true if successful, false otherwise
   */
  bool stdLayer::trainFunctor::operator()(const dvector& ,
                                          dmatrix& ,
                                          dvector& ,
                                          const ivector& ,
                                          const int& ,
                                          bool& ) {
    return false; // throw exception("training functor not implemented");
  }

  /*
   * The functor operator for unsupervised training requires
   * following parameters:
   * @param input the input vector
   * @param weights the weights matrix to be changed
   * @param outLayer the output layer vector
   * @param modified indicates whether the weights were modified
   * @return true if successful, false otherwise
   */
  bool stdLayer::trainFunctor::operator ()(const dvector& ,
                                           dmatrix& ,
                                           dvector& ,
                                           bool& ) {
    return false; // throw exception("training functor not implemented");
  }
  
  // ----------------------------------------------
  // stdLayer
  // ----------------------------------------------

  stdLayer::stdLayer(const std::string& theName)
    : ioObject(),status() {
    layerName_ =theName;
    sizeIn_    = 0;
    sizeOut_   = 0;
    learnRate_ = 0.0;
  }

  stdLayer::stdLayer(const stdLayer& other) : ioObject(),status() {
    copy(other);
  }

  stdLayer::~stdLayer() {
  }

  stdLayer& stdLayer::copy(const stdLayer& other) {
    weights_.copy(other.weights_);
    outID_.copy(outID_);
    sizeIn_ = other.sizeIn_;
    sizeOut_ = other.sizeOut_;
    layerName_ = other.layerName_;
    learnRate_ = other.learnRate_;
    return *this;
  }

  void stdLayer::initWeights(initFunctor& initFunc) {
    initFunc(weights_, outID_);
  }

  bool stdLayer::propagate(const dvector& input,
                           propagationFunctor& prop,
                           dvector& output) const {
    return prop(input,weights_,output);
  }

  bool stdLayer::write(ioHandler& handler,const bool complete) const {
    bool b(true);
    if (complete) {
      b = handler.writeBegin();
    }

    lti::write(handler,"layerName",layerName_);
    lti::write(handler,"sizeIn",sizeIn_);
    lti::write(handler,"sizeOut",sizeOut_);
    lti::write(handler,"learnRate",learnRate_);
    lti::write(handler,"weights",weights_);
    lti::write(handler,"outID",outID_);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool stdLayer::read(ioHandler& handler,const bool complete) {
    bool b(true);
    if (complete) {
      b = handler.readBegin();
    }

    lti::read(handler,"layerName",layerName_);
    lti::read(handler,"sizeIn",sizeIn_);
    lti::read(handler,"sizeOut",sizeOut_);
    lti::read(handler,"learnRate",learnRate_);
    lti::read(handler,"weights",weights_);
    lti::read(handler,"outID",outID_);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  void stdLayer::setWeights(const dmatrix& w) {
    weights_=w;
  }

  void stdLayer::setIDs(const ivector& v) {
    outID_=v;
  }

  void stdLayer::setSize(const int &in, const int& out) {
    sizeIn_=in;
    sizeOut_=out;

    weights_.resize(sizeOut_,sizeIn_,0.);
    outID_.resize(sizeOut_,0);
  }


  /**
   * train the network (supervised) with the given input vector
   */
  bool stdLayer::train(const dvector& input,
                       const int& trainID,
                       trainFunctor& trainFunc) {
    bool modified;
    dvector outLayer;
    trainFunc(input, weights_, outLayer, outID_, trainID, modified);

    return true;
  }

  /**
   * train the network (unsupervised) with the given input vector
   */
  bool stdLayer::train(const dvector& input,trainFunctor& trainFunc) {
    bool modified;
    dvector outLayer;
    trainFunc(input, weights_, outLayer, modified);

    return true;
  }

  /**
   * Read a eDistanceType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eDistanceType& data) {
    std::string str;
    
    if (handler.read(str)) {  
      if (str == "L1Distance") {
        data = L1Distance;
      } else if (str == "L2Distance") {
        data = L2Distance;
      } else  {
        data = L1Distance;
      }

      return true;
    }

    return false;
  }

  /**
   * Write a eDistanceType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eDistanceType& data) {
    bool b=false;
    
    switch(data) {
    case L1Distance:
      handler.write("L1Distance");
      break;
    case L2Distance:
      handler.write("L2Distance");
      break;
    default:
      handler.write("Unknown");
      b = false;
      handler.setStatusString("undefined eDistanceType");
      break;
    }
    return b;
  }

}
