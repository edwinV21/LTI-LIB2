/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiMlp.cpp
 * \author Bastian Ibach
 * \author Pablo Alvarado
 * \date   13.8.2002
 *
 * $Id: ltiMlp.cpp,v 1.6 2012-01-03 02:22:25 alvarado Exp $
 */

#include "ltiMlp.h"
#include "ltiScramble.h"
#include "ltiFactory.h"
#include "ltiSTLIoInterface.h"

#include <cstdio>
#include <limits>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"


namespace lti {

  // ----------------------------------------------
  // activation functor
  // ----------------------------------------------
  
  // register in factory

  // WARNING: DO NOT REFORMAT THESE LINES, 
  //          the references-script needs them in one line each
  _LTI_REGISTER_IN_FACTORY_AS_VAR(linearActFunctor,mlp::activationFunctor,mlp::linearActFunctor,mlp_linearActFunctor)
  _LTI_REGISTER_IN_FACTORY_AS_VAR(signFunctor,mlp::activationFunctor,mlp::signFunctor,mlp_signFunctor)
  _LTI_REGISTER_IN_FACTORY_AS_VAR(sigmoidFunctor,mlp::activationFunctor,mlp::sigmoidFunctor,mlp_sigmoidFunctor)

  // ----------------------------------------------
  // activationFunctor
  // ----------------------------------------------
  bool mlp::activationFunctor::write(ioHandler& handler,
                                     const bool complete) const {
    bool b = true;
    if (complete) {
      b = b && handler.writeBegin();
      b = b && handler.writeEnd();
    }
    return b;
  }

  bool mlp::activationFunctor::read(ioHandler& handler,
                                    const bool complete) {
    bool b = true;
    if (complete) {
      b = b && handler.readBegin();
      b = b && handler.readEnd();
    }
    return b;
  }

  // ----------------------------------------------
  // linearActFunctor
  // ----------------------------------------------
    
  bool mlp::linearActFunctor::apply(dvector&) const {
    return true;
  }

  bool mlp::linearActFunctor::apply(const dvector& src,
                                          dvector& output) const {
    output.copy(src);
    return true;
  }

  bool mlp::linearActFunctor::deriv(dvector& output) const {
    output.fill(1.0);
    return true;
  }

  const double& mlp::linearActFunctor::onValue() const {
    static const double theOnValue = +1.0;
    return theOnValue;
  }

  const double& mlp::linearActFunctor::offValue() const {
    static const double theOffValue = -1.0;
    return theOffValue;
  }

  mlp::activationFunctor* mlp::linearActFunctor::clone() const {
    return new linearActFunctor(*this);
  }

  mlp::activationFunctor* mlp::linearActFunctor::newInstance() const {
    return new linearActFunctor;
  }

  const std::string& mlp::linearActFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }


  // ----------------------------------------------
  // signFunctor
  // ----------------------------------------------

  /**
   * The functor operator
   */
  bool mlp::signFunctor::apply(dvector& output) const {
    dvector::iterator it,eit;
    for (it=output.begin(),eit=output.end();it!=eit;++it) {
      (*it) = (*it) >= 0 ? 1.0 : -1.0;
    }
    return true;
  }
  
  /**
   * The functor operator.  Operates on copy
   */
  bool mlp::signFunctor::apply(const dvector& src, dvector& output) const {
    dvector::const_iterator it,eit;
    dvector::iterator oit;
    output.allocate(src.size());
    for (it=src.begin(),eit=src.end(),oit=output.begin();
         it!=eit;
         ++it,++oit) {
      (*oit) = (*it) >= 0 ? 1.0 : -1.0;
    }
    return true;
  }

  /**
   * The derivative of the functor (will return 1.0 to allow learning)
   */
  bool mlp::signFunctor::deriv(dvector& output) const {
    output.fill(1.0);
    return true;
  }

  /**
   * Return value used to represent "true" or "on"
   */
  const double& mlp::signFunctor::onValue() const {
    static const double theOnValue = +1.0;
    return theOnValue;
  }

  /**
   * Return value used to represent "false" or "off"
   */
  const double& mlp::signFunctor::offValue() const {
    static const double theOffValue = -1.0;
    return theOffValue;
  }

  /**
   * Return a copy of this functor
   */
  mlp::activationFunctor* mlp::signFunctor::clone() const {
    return new signFunctor(*this);
  }

  /**
   * Return a copy of this functor
   */
  mlp::activationFunctor* mlp::signFunctor::newInstance() const {
    return new signFunctor;
  }

  const std::string& mlp::signFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }


  // ----------------------------------------------
  // sigmoidFunctor
  // ----------------------------------------------

  /**
   * Constructor using explicit sigmoid slope
   */
  mlp::sigmoidFunctor::sigmoidFunctor(const double& theSlope):
    slope_(theSlope) {
  }
  
  /**
   * Constructor of a sigmoid with slope 1.0
   */
  mlp::sigmoidFunctor::sigmoidFunctor() : slope_(1.0) {
  }

  /**
   * The functor operator
   */
  bool mlp::sigmoidFunctor::apply(dvector& output) const {
    dvector::iterator it,eit;
    for (it=output.begin(),eit=output.end();it!=eit;++it) {
      (*it) = 1.0/(1.0+exp(-(*it)*slope_));
    }
    return true;
  }
  
  /**
   * The functor operator.  Operates on copy
   */
  bool mlp::sigmoidFunctor::apply(const dvector& src, dvector& output) const {
    dvector::const_iterator it,eit;
    dvector::iterator oit;
    output.allocate(src.size());
    for (it=src.begin(),eit=src.end(),oit=output.begin();
         it!=eit;
         ++it,++oit) {
      (*oit) = 1.0/(1.0+exp(-(*it)*slope_));
      
    }
    return true;
  }

  /**
   * The derivative of the functor
   */
  bool mlp::sigmoidFunctor::deriv(dvector& output) const {
    dvector::iterator it,eit;
    for (it=output.begin(),eit=output.end();it!=eit;++it) {
      (*it) = 1.0/(1.0+exp(-(*it)*slope_));
      (*it) = (*it)*(1.0-(*it))*slope_;
    }
    return true;
  }
  
  /**
   * Return a copy of this functor
   */
  mlp::activationFunctor* mlp::sigmoidFunctor::clone() const {
    return new sigmoidFunctor(slope_);
  }

  /**
   * Return a copy of this functor
   */
  mlp::activationFunctor* mlp::sigmoidFunctor::newInstance() const {
    return new sigmoidFunctor;
  }

  const std::string& mlp::sigmoidFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  
  /**
   * Return value used to represent "true" or "on"
   */
  const double& mlp::sigmoidFunctor::onValue() const {
    static const double theOnValue = +1.0;
    return theOnValue;
  }

  /**
   * Return value used to represent "false" or "off"
   */
  const double& mlp::sigmoidFunctor::offValue() const {
    static const double theOffValue = 0.0;
    return theOffValue;
  }

  /**
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool mlp::sigmoidFunctor::write(ioHandler& handler,
                                  const bool complete) const {
    bool b = true;
    if (complete) {
      b = b && handler.writeBegin();
    }
    b = b && lti::write(handler,"slope",slope_);
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
  bool mlp::sigmoidFunctor::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) {
      b = b && handler.readBegin();
    }
    b = b && lti::read(handler,"slope",slope_);
    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  // --------------------------------------------------
  // mlp::parameters
  // --------------------------------------------------

  // default constructor
  mlp::parameters::parameters()
    : supervisedInstanceClassifier::parameters() {

    trainingMode = ConjugateGradients;
    batchMode = bool(true);
    momentum = double(0.0);
    hiddenUnits = ivector(1,4);
    learnrate = float(0.1);
    maxNumberOfEpochs = int(500);
    stopError = double(0.005);

    activationFunctions.resize(hiddenUnits.size()+1,"sigmoidFunctor");
    
  }

  // copy constructor
  mlp::parameters::parameters(const parameters& other)
    : supervisedInstanceClassifier::parameters()  {
    copy(other);
  }

  // destructor
  mlp::parameters::~parameters() {
    activationFunctions.clear();
  }

  bool mlp::parameters::setLayers(const int hidden,
                                  const activationFunctor& activ) {

    unsigned int i;

    static const int layers = 2;
    const std::string fnctName = activ.name();

    hiddenUnits.assign(1,hidden);
    activationFunctions.assign(layers,fnctName);

    for (i=0;i<activationFunctions.size();++i) {
      activationFunctions[i] = fnctName;
    }

    return true;
  }

  bool mlp::parameters::setLayers(const int hidden,
                                  const std::string& activ) {

    unsigned int i;

    static const int layers = 2;

    hiddenUnits.assign(1,hidden);
    activationFunctions.assign(layers,activ);

    for (i=0;i<activationFunctions.size();++i) {
      activationFunctions[i] = activ;
    }

    return true;
  }

  bool mlp::parameters::setLayers(const activationFunctor& activ) {
    static const int layers = 1;
    const std::string fnctName = activ.name();

    hiddenUnits.clear();
    activationFunctions.assign(layers,fnctName);

    return true;
  }

  bool mlp::parameters::setLayers(const std::string& activ) {
    static const int layers = 1;

    hiddenUnits.clear();
    activationFunctions.assign(layers,activ);

    return true;
  }


  bool mlp::parameters::setLayerActivation(const int layer,
                                           const activationFunctor& aFct){

    const unsigned int layers = hiddenUnits.size()+1;
    const std::string fctName = aFct.name();

    activationFunctions.resize(layers,fctName);

    if ((layer>=0) && (static_cast<unsigned int>(layer)<layers)) {
      activationFunctions[layer] = fctName;
      return true;
    }

    return false;
  }

  // get type name
  const std::string& mlp::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  mlp::parameters&
    mlp::parameters::copy(const parameters& other) {
    supervisedInstanceClassifier::parameters::copy(other);

    trainingMode = other.trainingMode;
    batchMode = other.batchMode;
    momentum = other.momentum;
    hiddenUnits = other.hiddenUnits;
    learnrate = other.learnrate;
    maxNumberOfEpochs = other.maxNumberOfEpochs;
    stopError = other.stopError;

    activationFunctions = other.activationFunctions;

    return *this;
  }

  // alias for copy member
  mlp::parameters& mlp::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  mlp::parameters* mlp::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  mlp::parameters* mlp::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool mlp::parameters::write(ioHandler& handler,
                              const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"trainingMode",trainingMode);
      lti::write(handler,"batchMode",batchMode);
      lti::write(handler,"momentum",momentum);
      lti::write(handler,"hiddenUnits",hiddenUnits);
      lti::write(handler,"learnrate",learnrate);
      lti::write(handler,"maxNumberOfEpochs",maxNumberOfEpochs);
      lti::write(handler,"stopError",stopError);
      lti::write(handler,"activationFunctions",activationFunctions);
      lti::write(handler,"rndConfig",rndConfig);
    }

    b = b && supervisedInstanceClassifier::parameters::write(handler,false);

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
  bool mlp::parameters::read(ioHandler& handler,
                             const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"trainingMode",trainingMode);
      lti::read(handler,"batchMode",batchMode);
      lti::read(handler,"momentum",momentum);
      lti::read(handler,"hiddenUnits",hiddenUnits);
      lti::read(handler,"learnrate",learnrate);
      lti::read(handler,"maxNumberOfEpochs",maxNumberOfEpochs);
      lti::read(handler,"stopError",stopError);
      lti::read(handler,"activationFunctions",activationFunctions);
      lti::read(handler,"rndConfig",rndConfig);
    }

    b = b && supervisedInstanceClassifier::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // mlp
  // --------------------------------------------------


  // default constructor
  mlp::mlp()
    : supervisedInstanceClassifier(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

  }

  // default constructor
  mlp::mlp(const parameters& params)
    : supervisedInstanceClassifier(){

    // set the default parameters
    setParameters(params);
  }

  // copy constructor
  mlp::mlp(const mlp& other)
    : supervisedInstanceClassifier()  {
    copy(other);
  }

  // destructor
  mlp::~mlp() {
    cleanActivationFunctions();
  }

  const std::string& mlp::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  mlp& mlp::copy(const mlp& other) {
    supervisedInstanceClassifier::copy(other);

    return (*this);
  }

  // alias for copy member
  mlp& mlp::operator=(const mlp& other) {
    return (copy(other));
  }


  // clone member
  classifier* mlp::clone() const {
    return new mlp(*this);
  }

  // clone member
  classifier* mlp::newInstance() const {
    return new mlp;
  }

  // return parameters
  const mlp::parameters& mlp::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&classifier::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool mlp::updateParameters() {
    if (classifier::updateParameters()) {
      const parameters& par = getParameters();
      cleanActivationFunctions();
      activationFunctions_.assign(par.activationFunctions.size(),0);
      unsigned int i;
      for (i=0;i<activationFunctions_.size();++i) {
        activationFunctions_[i] =
          factory<activationFunctor>::
          getFactory().newInstance(par.activationFunctions[i]);
        if (isNull(activationFunctions_[i])) {
          setStatusString("Activation function could not be created");
          return false;
        }
      }

      // initialize random number generator
      uniformContinuousDistribution::parameters rnd;
      static_cast<randomDistribution::parameters>(rnd).copy(par.rndConfig);
      rnd.min=-1;
      rnd.max=1;
      unifRnd_.setParameters(rnd);

      return true;
    }
    return false;
  }

  // write method
  bool mlp::write(ioHandler& handler,const bool complete) const {
    bool b(true);

    if (complete) {
      b = handler.writeBegin();
    }

    // write the standard data (output and parameters)
    supervisedInstanceClassifier::write(handler,false);

    if (b) {
      lti::write(handler,"inputs",inputs_);
      lti::write(handler,"outputs",outputs_);
      lti::write(handler,"weights",weights_);

    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // read method
  bool mlp::read(ioHandler& handler,const bool complete) {
    bool b(true);

    if (complete) {
      b = handler.readBegin();
    }

    // read the standard data (output and parameters)
    supervisedInstanceClassifier::read(handler,false);

    if (b) {
      if (lti::read(handler,"inputs",inputs_) && 
          lti::read(handler,"outputs",outputs_)) {
        if (lti::read(handler,"weights",weights_)) {
          b = initWeights(true); // initialize internal state keeping
          // the weights...
        } else {
          initWeights(false);
          b = false;
        }
      } else {
        b = false;
      }
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  const dvector& mlp::getWeights() const {
    return weights_;
  }

  // -------------------------------------------------------------------
  // The train-methods!
  // -------------------------------------------------------------------
  // Normal training method
  bool mlp::train(const dmatrix& data,
                  const ivector& ids) {
    checkHowManyOutputs(ids);
    inputs_=data.columns();
    initWeights(false);
    return train(weights_,data,ids);
  }

  void mlp::checkHowManyOutputs(const ivector& ids) {
    // count how many different ids are present in the training set
    std::map<int,int> extToInt;
    std::map<int,int>::iterator it;

    int i,k;
    for (i=0,k=0;i<ids.size();++i) {
      it = extToInt.find(ids.at(i));
      if (it == extToInt.end()) {
        extToInt[ids.at(i)] = k;
        ++k;
      }
    }

    outputs_ = static_cast<int>(extToInt.size());
  }

  // general train method
  bool mlp::train(const dvector& theWeights,
                  const dmatrix& data,
                  const ivector& ids) {

    if (data.empty()) {
      setStatusString("Train data empty");
      return false;
    }


    if (ids.size()!=data.rows()) {
      std::string str;
      str = "Dimensionality of IDs vector and the number of rows " \
            "of the input matrix must have the same size.";
      setStatusString(str.c_str());
      return false;
    }


    // tracks the status of the training process.
    // if an error occurs set to false and use setStatusString()
    // however, training should continue, fixing the error as well as possible
    bool b=true;

    // vector with internal ids
    ivector newIds,idsLUT;
    newIds.allocate(ids.size());

    // map to get the internal Id to an external Id;
    std::map<int,int> extToInt;
    std::map<int,int>::iterator it;

    int i,k;
    for (i=0,k=0;i<ids.size();++i) {
      it = extToInt.find(ids.at(i));
      if (it != extToInt.end()) {
        newIds.at(i) = (*it).second;
      } else {
        extToInt[ids.at(i)] = k;
        newIds.at(i) = k;
        ++k;
      }
    }

    idsLUT.resize(static_cast<int>(extToInt.size()));
    for (it=extToInt.begin();it!=extToInt.end();++it) {
      idsLUT.at((*it).second) = (*it).first;
    }

    // initialize the inputs and output units from the given data
    outputs_= idsLUT.size();
    inputs_ = data.columns();

    const parameters& param = getParameters();

    // display which kind of algorithm is to be used
    if (haveValidProgressObject()) {
      getProgressObject().reset();
      std::string str("mlp: Training using ");
      switch(param.trainingMode) {
        case ConjugateGradients:
          str += "conjugate gradients";
          break;
        case SteepestDescent:
          str += "steepest descent";
          break;
        default:
          str += "unnamed method";
      }
      getProgressObject().setTitle(str);
      getProgressObject().setMaxSteps(param.maxNumberOfEpochs+1);
    }

    dvector grad;
    if (&theWeights != &weights_) {
      weights_.copy(theWeights);
    }

    if (!initWeights(true)) { // keep the weights
      setStatusString("Wrong weights!");
      return false;
    };

    computeErrorNorm(newIds);

    if (param.trainingMode == ConjugateGradients) {
      b = trainConjugateGradients(data,newIds);
    } else {
      if (param.batchMode) { // batch training mode:
        b = trainSteepestBatch(data,newIds);
      } else { // sequential training mode:
        b = trainSteepestSequential(data,newIds);
      }
    }

    if (haveValidProgressObject()) {
      getProgressObject().step("Training ready.");
    }

    outputTemplate tmpOutTemp(idsLUT);
    setOutputTemplate(tmpOutTemp);

    // create the appropriate outputTemplate
    makeOutputTemplate(outputs_,data,ids);

    return b;
  }

  bool mlp::trainSteepestBatch(const dmatrix& data,
                               const ivector& internalIds) {

    const parameters& param = getParameters();
    char buffer[256];
    bool abort = false;
    int i;

    if (param.momentum > 0) {
      // with momentum
      dvector grad,delta(weights_.size(),0.0);

      // train with all epochs
      for (i=0;!abort && (i<param.maxNumberOfEpochs);++i) {

        calcGradient(data,internalIds,grad);
        delta.addScaled(param.learnrate,grad,param.momentum,delta);
        weights_.add(delta);

        // update progress info object
        if (haveValidProgressObject()) {
          sprintf(buffer,"Error=%f",totalError_/errorNorm_);
          getProgressObject().step(buffer);
          abort = abort || (totalError_/errorNorm_ <= param.stopError);
          abort = abort || getProgressObject().breakRequested();
        }
      }
    } else {
      // without momentum
      dvector grad;

      // train with all epochs
      for (i=0;!abort && (i<param.maxNumberOfEpochs);++i) {

        calcGradient(data,internalIds,grad);
        weights_.addScaled(param.learnrate,grad);

        // update progress info object
        if (haveValidProgressObject()) {
          sprintf(buffer,"Error=%f",totalError_/errorNorm_);
          getProgressObject().step(buffer);
          abort = abort || (totalError_/errorNorm_ <= param.stopError);
          abort = abort || getProgressObject().breakRequested();
        }
      }
    }

    return true;
  }

  bool mlp::trainSteepestSequential(const dmatrix& data,
                                    const ivector& internalIds) {

    const parameters& param = getParameters();
    char buffer[256];
    bool abort = false;
    scramble::parameters rnd;
    static_cast<randomDistribution::parameters>(rnd.randomParams).
      copy(param.rndConfig);
    scramble scrambler(rnd);
    int i,j,k;
    double tmpError;
    ivector idx;
    idx.allocate(data.rows());
    for (i=0;i<idx.size();++i) {
      idx.at(i)=i;
    }

    if (param.momentum > 0) {
      // with momentum
      dvector grad,delta(weights_.size(),0.0);

      for (i=0; !abort && (i<param.maxNumberOfEpochs); ++i) {
        scrambler.apply(idx); // present the pattern in a random sequence
        totalError_ = 0;
        for (j=0;j<idx.size();++j) {
          k=idx.at(j);
          calcGradient(data.getRow(k),internalIds.at(k),grad);
          computeActualError(internalIds.at(k),tmpError);
          totalError_+=tmpError;
          delta.addScaled(param.learnrate,grad,param.momentum,delta);
          weights_.add(delta);
        }

        // update progress info object
        if (haveValidProgressObject()) {
          sprintf(buffer,"Error=%f",totalError_/errorNorm_);
          getProgressObject().step(buffer);
          abort = abort || (totalError_/errorNorm_ <= param.stopError);
          abort = abort || getProgressObject().breakRequested();
        }
      }
    } else {
      // without momentum
      idx.allocate(data.rows());
      dvector grad;

      for (i=0;i<idx.size();++i) {
        idx.at(i)=i;
      }
      for (i=0; !abort && (i<param.maxNumberOfEpochs); ++i) {
        scrambler.apply(idx); // present the pattern in a random sequence
        totalError_ = 0;
        for (j=0;j<idx.size();++j) {
          k=idx.at(j);
          calcGradient(data.getRow(k),internalIds.at(k),grad);
          computeActualError(internalIds.at(k),tmpError);
          totalError_+=tmpError;
          weights_.addScaled(param.learnrate,grad);
        }

        // update progress info object
        if (haveValidProgressObject()) {
          sprintf(buffer,"Error=%f",totalError_/errorNorm_);
          getProgressObject().step(buffer);
          abort = abort || (totalError_/errorNorm_ <= param.stopError);
          abort = abort || getProgressObject().breakRequested();
        }
      }
    }
    return true;
  }

  bool mlp::trainConjugateGradients(const dmatrix& data,
                                    const ivector& internalIds) {

    const parameters& param = getParameters();
    char buffer[256];
    bool abort = false;
    int i;

    // without momentum
    dvector gradTau,gradTauP1,r;
    double beta(0.0),eta,absg0,tmp;
    dvector newWeights;

    double bestError;
    dvector bestWeights(weights_);

    // compute the negative gradient
    calcGradient(data,internalIds,gradTau);
    r.copy(gradTau);
    bestError = totalError_;

#if  defined(_LTI_DEBUG) && (_LTI_DEBUG > 4)
    std::cout << "grad 0: \t" << gradTau << endl;
#endif

    absg0 = param.stopError*param.stopError*gradTau.dot(gradTau);

    // train with all epochs
    for (i=0;!abort && (i<param.maxNumberOfEpochs);++i) {

      lineSearch(data,internalIds,r,eta,newWeights);

      // exchanging the weights content and updating the weight matrices is
      // faster than copying the whole newWeights vector!
      weights_.swap(newWeights);
      updateWeightIndices(weights_,matWeights_); // matWeights must
                                                 // share the memory
                                                 // used by weights, and
                                                 // with exchange we
                                                 // changed that!

      calcGradient(data,internalIds,gradTauP1);

#if  defined(_LTI_DEBUG) && (_LTI_DEBUG > 4)
      std::cout << "grad " << i+1 << ": \t" << gradTauP1 << endl;
#endif

      if (gradTauP1.dot(gradTauP1) < absg0) {
        // gradient about zero -> local minimum reached.
        // check if it is good enough

        if (totalError_/errorNorm_ > param.stopError) {
          // gradient is small enough, but the error is still too big!
          // we are in a local minimum! -> retry with other weights

          if (totalError_ < bestError) {
            // but keep first actual weights as the best so far
            bestError = totalError_;
            bestWeights.fill(weights_);
          }

          initWeights(false);
          // compute the negative gradient
          calcGradient(data,internalIds,gradTau);
          r.copy(gradTau);
          absg0 = param.stopError*param.stopError*gradTau.dot(gradTau);
          beta=0.0;
        } else {
          // minimum is good enough!
          abort = true;
        }
      } else {
        tmp = gradTau.dot(gradTau);
        gradTau.subtract(gradTauP1);
        beta = max(0.0,-gradTauP1.dot(gradTau)/tmp);
        r.addScaled(gradTauP1,beta,r);
        gradTau.swap(gradTauP1);
      }

      // update progress info object
      if (haveValidProgressObject()) {
        if (beta != 0.0) {
          sprintf(buffer,"Error=%f",totalError_/errorNorm_);
        } else {
          sprintf(buffer,"Error=%f (*)",totalError_/errorNorm_);
        }
        getProgressObject().step(buffer);
        abort = abort || getProgressObject().breakRequested();
      }
    }

    if (!abort) {
      // maximal iterations reached!
      if (totalError_ > bestError) {
        // get the best weights found
        weights_.copy(bestWeights);

        // update progress info object
        if (haveValidProgressObject()) {
          sprintf(buffer,"Max Iteration reached, best error=%f",
                  bestError/errorNorm_);
          getProgressObject().step(buffer);
        }
      }
    }

    return true;
  }

  /*
   * line search computes the eta scalar factor at which the error
   * is minimized.  It begins at the actual weight and follows the given
   * direction.
   */
  bool mlp::lineSearch(const dmatrix& inputs,
                       const ivector& ids,
                       const dvector& direction,
                       double& xmin,
                       dvector& newWeights) const {

    // following algorithms are based on Press, W.H. et.al. Numerical
    // Recipies in C. Chapter 10, Minimization or Maximization of
    // Functions. pp. 397ff

    static const double gold = 1.618034; // golden mean
    static const double glimit = 100.0;  // maximum magnification
                                         // allowed for parabolic-fit
                                         // step
    static const double tiny = 1.0e-20;
    static const int itmax = 100; // maximum allowed number of
                                  // iterations to find minimum
    static const double cgold = 0.3819660; // golden ration
    static const double zeps = 1.0e-10; // small number that protects
                                        // against trying to achieve
                                        // fractional accuracy for a
                                        // minimum that happens to be
                                        // exaclty 0


    const parameters& param = getParameters();
    const int layers = param.hiddenUnits.size()+1;
    std::vector<dmatrix> mWeights(layers);
    newWeights.copy(weights_);
    updateWeightIndices(newWeights,mWeights);

    // -----------------------------------------------------------------------
    //  Initial Bracket
    // -----------------------------------------------------------------------

    double ax(0.0),bx(1.0),cx;
    double fa,fb,fc;
    double ulim,u,r,q,fu,dum;

    // evaluate error at eta=0 and eta=1
    computeTotalError(mWeights,inputs,ids,fa);
    newWeights.add(direction);
    computeTotalError(mWeights,inputs,ids,fb);

    if (fb > fa) { // switch roles of a and b so that we can go
                   // downhill in the direction from a to b
      ax=1.0;
      bx=0.0;
      dum=fa; fa=fb; fb=dum;
    }

    // first guess for c:
    cx = bx + gold*(bx-ax);
    newWeights.addScaled(weights_,cx,direction);
    computeTotalError(mWeights,inputs,ids,fc);

    while (fb>fc) { // keep returning here until we bracket
      r=(bx-ax)*(fb-fc);  // Compute u by parabolic extrapolation from a,b,c.
      q=(bx-cx)*(fb-fa);
      if (q>r) {          // tiny is used to prevent any posible division by 0
        u=bx-((bx-cx)*q-(bx-ax)*r)/(2.0*(max(q-r,tiny)));
      } else {
        u=bx-((bx-cx)*q-(bx-ax)*r)/(-2.0*(max(r-q,tiny)));
      }

      ulim=bx+glimit*(cx-bx);
      // We won't go farther than this.  Test various possibilities:
      if ((bx-u)*(u-cx) > 0.0) {
        newWeights.addScaled(weights_,u,direction);
        computeTotalError(mWeights,inputs,ids,fu);
        if (fu < fc) { // Got a minimum between b and c
          ax=bx;
          bx=u;
          fa=fb;
          fb=fu;
          break;
        } else if (fu > fb) { // Got a minimum between a and u
          cx=u;
          fc=fu;
          break;
        }
        u=cx+gold*(cx-bx); // Parabolic fit was no use. Use default
                           // magnification
        newWeights.addScaled(weights_,u,direction);
        computeTotalError(mWeights,inputs,ids,fu);
      } else if ((cx-u)*(u-ulim) > 0.0) {
        // Parabolic fit is between c and its allowed limit.
        newWeights.addScaled(weights_,u,direction);
        computeTotalError(mWeights,inputs,ids,fu);
        if (fu < fc) {
          bx=cx;
          cx=u;
          u=cx+gold*(cx-bx);
          fb=fc;
          fc=fu;
          newWeights.addScaled(weights_,u,direction);
          computeTotalError(mWeights,inputs,ids,fu);
        }
      } else if ((u-ulim)*(ulim-cx) >= 0.0) { // Limit parabolic u to
                                             // maximum allowed value
        u=ulim;
        newWeights.addScaled(weights_,u,direction);
        computeTotalError(mWeights,inputs,ids,fu);
      } else { // Reject parabolic u, use default magnification
        u=cx+gold*(cx-bx);
        newWeights.addScaled(weights_,u,direction);
        computeTotalError(mWeights,inputs,ids,fu);
      }

      // Eliminate oldest point and continue
      ax=bx;
      bx=cx;
      cx=u;
      fa=fb;
      fb=fc;
      fc=fu;
    }

    // -----------------------------------------------------------------------
    //  Line search: Brent's method
    // -----------------------------------------------------------------------

    // fractional precision of found minimum
    static const double tol =
      2.0*sqrt(std::numeric_limits<double>::epsilon());
    double fv,fw,fx,etemp;
    double p,tol1,tol2,v,w,x,xm,a,b,d=0.0;
    int iter;

    q=p=etemp=0.0;

    // This will be the distance moved on the step before last
    double e=0.0;
    // a and b must be in ascending order, but input abscissas need not be.
    if (ax < cx) {
      a = ax;
      b = cx;
    } else {
      a = cx;
      b = ax;
    }
    // Initializations...
    x=w=v=bx;
    fw=fv=fx=fb;

    for (iter=1;iter<=itmax;++iter) { // main loop
      xm=0.5*(a+b);
      tol2=2.0*(tol1=tol*abs(x)+zeps);

      _lti_debug4(iter << ". Analysing [" << a << ";" << b << "]:\n"); 
      _lti_debug4("  Current minimum at f(" << x << ") = " << fx << "\n");

      // test for done here
      if (abs(x-xm) <= (tol2-0.5*(b-a))) {
        xmin = x;
        newWeights.addScaled(weights_,xmin,direction);
        return true;
      }

      if (abs(e) > tol1) {
        r=(x-w)*(fx-fv);
        q=(x-v)*(fx-fw);
        p=(x-v)*q - (x-w)*r;
        q=2.0*(q-r);
        if (q > 0.0) {
          p = -p;
        } else {
          q = -q;
        }
        etemp=e;
        e=d;
      }

      if ((abs(p) < abs(0.5*q*etemp)) ||
          (p < q*(x-a)) ||
          (p < q*(b-x))) {

        // The above conditions determine the acceptability of the
        // parabolic fit.  Here we take the golden section step into
        // the larger of the two segments

        d=p/q;
        u=x+d;

        _lti_debug4("  Parabolic fit ok. Next step d=" << d << "\n");

        if (u-a < tol2 || b-u < tol2) {
          d=(x<xm)?tol1:-tol1;
        }        
      } else {
        _lti_debug4("  Parabolic fit failed. Next step d=" << d << "\n");
        e = (x<xm) ? b-x : a-x;
        d = cgold*e;
      }

      u=(abs(d) >= tol1) ? x+d : x + ((d>0.0) ? tol1 : -tol1);

      // This is the one function evaluation per iteration
      newWeights.addScaled(weights_,u,direction);
      computeTotalError(mWeights,inputs,ids,fu);

      // Now decide what to do with our function evaluation
      if (fu <= fx) {
        if (u>=x) {
          a=x;
        } else {
          b=x;
        }
        // Housekeeping follows:
        v=w;
        w=x;
        x=u;
        fv=fw;
        fw=fx;
        fx=fu;
      } else {
        if (u<x) {
          a=u;
        } else {          
          b=u;
        }

        if (fu <= fw || w == x) {
          v=w;
          w=u;
          fv=fw;
          fw=fu;
        } else if (fu <= fv || v == x || v == w) {
          v=u;
          fv=fu;
        }
      }  // done with housekeeping.  Back for another iteration
    } // end of main loop.

    setStatusString("Too many iterations in brent line search");
    xmin=x;
    newWeights.addScaled(weights_,xmin,direction);
    return false;
  }



  // -------------------------------------------------------------------
  // The classify-method!
  // -------------------------------------------------------------------

  //TODO Usually this method does not need to be implemented use inherited
  // method from direct subclass of classifier (ie centroidClassifier) instead
  // In this case delete. Otherwise comment!

  bool mlp::classify(const dvector& feature,
                     result& res) const {

    // tResult is a vector that results from your classification process.
    // rename if desired.
    propagate(feature);

    if (outTemplate_.apply(unitsOut_[unitsOut_.size()-1], res)) {
      res.setWinnerAtMax();
      return true;
    };

    return false;
  }

  /*
   * initialize weights with random values
   *
   */
  bool mlp::initWeights(const bool keepWeightVals) {

    const parameters& param = getParameters();
    const int layers = param.hiddenUnits.size()+1;
    int i;

    // this is a std::vector<matrix>, so no allocate available...
    matWeights_.clear();
    matWeights_.resize(layers);
    
    layerIndex_.allocate(layers);

    ivector units(layers+1);

    // fill the units vector beginning with the second element with the
    // number of hidden units in each layer.
    units.fill(param.hiddenUnits,1);
    units.at(0)=inputs_;
    units.at(layers)=outputs_;

    // count the number of weights required
    int weightsSize(0);

    for (i=0;i<layers;++i) {
      layerIndex_.at(i)=weightsSize;
      weightsSize+=((units.at(i+1))*(units.at(i)+1));
    }

    // resize the weights vector
    if (keepWeightVals) {
      if (weightsSize != weights_.size()) {
        return false;
      }
    } else {
      weights_.allocate(weightsSize);
    }

    // create the weight matrices, using the same memory regions in
    // the weights vector
    for (i=0;i<layers;++i) {
      matWeights_[i].useExternData(units.at(i+1),units.at(i)+1,
                                   &weights_[layerIndex_[i]]);
    }

    if (!keepWeightVals) {
      // initialize the matrix with random values
      for (i=0;i<weights_.size();++i) {
        weights_.at(i)=unifRnd_.draw();
      }
    }

    unitsOut_.resize(layers);
    unitsNet_.resize(layers);

    off_ = activationFunctions_[layers-1]->offValue();
    on_  = activationFunctions_[layers-1]->onValue();

    return true;
  }

   /*
    * Given the weights vector, update the vector of matrices so that
    * each matrix uses its respective memory block in the vector.
    */
  void mlp::updateWeightIndices(dvector& theWeights,
                                std::vector<dmatrix>& theWMats) const {

    // create the weight matrices, using the same memory regions in
    // the weights vector
    for (unsigned int i=0;i<theWMats.size();++i) {
      const ipoint& size = matWeights_[i].size();
      theWMats[i].useExternData(size.y,size.x,
                                &theWeights[layerIndex_[i]]);
    }
  }

  /*
   * calculate output of units
   * @param weights vector
   * @param activFunctor, the activation function to use
   */
  bool mlp::propagate(const dvector& input) const {
    unsigned int i;
    i=0;

    biasMultiply(matWeights_[i],input,unitsNet_[i]);
    activationFunctions_[i]->apply(unitsNet_[i],unitsOut_[i]);

#if  defined(_LTI_DEBUG) && (_LTI_DEBUG > 9)
    std::cout << "Output of Layer " << i << ":" << endl;
    std::cout << unitsOut_[i] << endl;
#endif

    for (i=1;i<unitsOut_.size();++i) {
      biasMultiply(matWeights_[i],unitsOut_[i-1],unitsNet_[i]);
      activationFunctions_[i]->apply(unitsNet_[i],unitsOut_[i]);

#if  defined(_LTI_DEBUG) && (_LTI_DEBUG > 9)
      std::cout << "Output of Layer " << i << ":" << endl;
      std::cout << unitsOut_[i] << endl;
#endif
    }

    return true;
  }

  /*
   * calculate output of Units
   * @param weights vector
   * @param activFunctor, the activation function to use
   */
  bool mlp::propagate(const dvector& input,
                      const std::vector<dmatrix>& mWeights,
                      std::vector<dvector>& uNet,
                      std::vector<dvector>& uOut) const {
    unsigned int i;
    const parameters& param = getParameters();
    const int layers = param.hiddenUnits.size()+1;
    if (static_cast<int>(uOut.size()) != layers) {
      uNet.resize(layers);
      uOut.resize(layers);
    }

    i=0;
    biasMultiply(mWeights[i],input,uNet[i]);
    activationFunctions_[i]->apply(uNet[i],uOut[i]);

    for (i=1;i<uOut.size();++i) {
      biasMultiply(mWeights[i],uOut[i-1],uNet[i]);
      activationFunctions_[i]->apply(uNet[i],uOut[i]);
    }

    return true;
  }

  /*
   * compute mat*vct' where vct' is a vector with one additional element
   * (1.0) at the beginning of vct.
   */
  bool mlp::biasMultiply(const dmatrix& mat,
                         const dvector& vct,
                         dvector& res) const {
    int j;
    dmatrix::const_iterator it; //,eit;
    dvector::iterator rit;
    dvector::const_iterator vit,evit;

    res.allocate(mat.rows());
    it = mat.begin();
    // eit = mat.end();
    rit = res.begin();
    evit = vct.end();

    for (j=0;j<mat.rows();++j,++rit) {
      *rit = *it;
      ++it;
      for (vit=vct.begin();vit!=evit;++it,++vit) {
        *rit += (*vit)*(*it);
      }
    }

    return true;
  }

  bool mlp::calcGradient(const dmatrix& inputs,
                         const ivector& ids,
                         dvector& grad) {

    if (inputs.rows() != ids.size()) {
      setStatusString("Number of vectors not consistent with number of ids");
      return false;
    }

    dvector tmp;
    int i;
    double tmpError;

    totalError_ = 0;
    calcGradient(inputs.getRow(0),ids.at(0),grad);
    computeActualError(ids.at(0),totalError_);

    for (i=1;i<inputs.rows();++i) {
      calcGradient(inputs.getRow(i),ids.at(i),tmp);
      computeActualError(ids.at(i),tmpError);
      grad.add(tmp);
      totalError_+=tmpError;
    }

    return true;
  }

   /*
    * compute the error using the last propagated input and the given
    * pattern
    */
  bool mlp::computeActualError(const int id,double& error) const {
    return computePatternError(id,unitsOut_.back(),error);
  }

   /*
    * compute the error using the last propagated input and the given
    * pattern
    */
  bool mlp::computePatternError(const int id,
                                const dvector& outUnits,
                                double& error) const {

    const int lastIdx = outUnits.size();
    int j;

    double tmp;
    error = 0.0;
    for (j=0;j<lastIdx;++j) {
      tmp = (outUnits.at(j)-((j==id)?on_:off_));
      error += tmp*tmp;
    }

    error *= 0.5;

    return true;
  }

  /*
   * compute the error of the given weights for the whole training set.
   */
  bool mlp::computeTotalError(const std::vector<dmatrix>& mWeights,
                              const dmatrix& inputs,
                              const ivector& ids,
                              double& totalError) const {

    if (ids.size() != inputs.rows()) {
      return false;
    }

    const parameters& param = getParameters();
    const int layers = param.hiddenUnits.size()+1;
    std::vector<dvector> uNet(layers),uOut(layers);
    int i;
    double tmp;
    totalError=0.0;
    for (i=0;i<ids.size();++i) {
      propagate(inputs.getRow(i),mWeights,uNet,uOut);
      computePatternError(ids.at(i),uOut.back(),tmp);
      totalError+=tmp;
    }

    return true;
  }


  /*
   * calculate gradient of error surface using back-propagation algorithm
   *
   * @param input input vector
   * @param outputId desired output.  This value must be between 0 and
   *                 the number of output elements-1.
   * @param grad computed gradient of the error surface
   * @return true if successful, or false otherwise.
   */
  bool mlp::calcGradient(const dvector& input,
                         const int outputId,
                         dvector& grad) {

    const parameters& param = getParameters();
    const int layers = param.hiddenUnits.size()+1;

    propagate(input);
    grad.resize(weights_.size());

    int i,j,jj,k,idx,lastIdx;
    int layer = layerIndex_.size()-1;

    double delta;

    // compute f'(net) at unitsNet
    for (i=0;i<layers;++i) {
      activationFunctions_[i]->deriv(unitsNet_[i]);
    }

    // ---------------------------------------------
    // gradient for the elements of the output layer
    // ---------------------------------------------

    const dmatrix& outMatr = matWeights_[layer];

    const dvector* theInput = 0;
    if (layer>0) {
      theInput = &unitsOut_[layer-1];
    } else {
      theInput = &input;
    }

    idx = layerIndex_.at(layer);

    dvector lastDeltas,newDeltas;

    lastDeltas.allocate(outMatr.rows());

    for (j=0;j<outMatr.rows();++j) {
      delta = ((((j==outputId)?on_:off_) - unitsOut_[layer].at(j)) *
               unitsNet_[layer].at(j));

      lastDeltas.at(j)=delta;

      grad.at(idx)=delta; // bias = 1.0
      ++idx;
      for (i=0;i<theInput->size();++i,++idx) {
        // idx means layerIndex.at(layer)+i+j*ROWS
        grad.at(idx) = delta*theInput->at(i);
      }
    }

    // ----------------------------------------------
    // gradient for the elements of the hidden layers
    // ----------------------------------------------
    --layer;
    while (layer>=0) {

      const dmatrix& outMat = matWeights_[layer];
      const dmatrix& lastMat = matWeights_[layer+1];
      theInput = 0;

      if (layer>0) {
        theInput = &unitsOut_[layer-1];
      } else {
        theInput = &input;
      }

      idx = layerIndex_.at(layer);
      lastIdx = theInput->size();

      newDeltas.allocate(outMat.rows());
      for (j=0,jj=1;j<outMat.rows();++j,++jj) {
        delta = 0;
        for (k=0;k<lastMat.rows();++k) {
          delta+=(lastDeltas.at(k)*lastMat.at(k,jj));
        }
        delta*=unitsNet_[layer].at(j);
        newDeltas.at(j)=delta;

        grad.at(idx)=delta; // bias = 1.0
        ++idx;
        for (i=0;i<lastIdx;++i,++idx) {
          // idx means layerIndex.at(layer)+i+j*ROWS
          grad.at(idx) = delta*theInput->at(i);
        }
      }

      newDeltas.detach(lastDeltas);

      // continue with next layer
      --layer;
    };

    return true;
  }

  bool mlp::computeErrorNorm(const ivector& ids) {

    dvector vct(outputs_,off_*ids.size());

    const double fix=on_-off_;
    int i;
    double tmp,tmp2,v;

    // compute first the average outputs for the training set

    for (i=0;i<ids.size();++i) {
      vct.at(ids.at(i))+=fix;
    }

    vct.divide(ids.size());
    double offError(0.0);

    // now compute the error
    for (i=0;i<vct.size();++i) {
      tmp = off_ - vct.at(i);
      offError += (tmp*tmp);
    }

    errorNorm_ = 0.0;
    for (i=0;i<ids.size();++i) {
      v = vct.at(ids.at(i));
      tmp  = off_ - v;
      tmp2 = on_  - v;
      errorNorm_ += (offError - tmp*tmp + tmp2*tmp2);
    }
    errorNorm_ *= 0.5;

    return true;
  }

  /*
   * Undocumented function for debug purposes only.  It display the
   * internal weight matrices
   */
  bool mlp::prettyPrint() {
#ifdef _LTI_DEBUG
    const parameters& param = getParameters();
    const int layers = param.hiddenUnits.size()+1;

    unsigned int i;
    std::cout << "---------------------------------------------------" 
              << std::endl;
    std::cout << "Neural Network: " << layers << " layers" << std::endl;
    std::cout << "Inputs : " << inputs_ << std::endl;
    std::cout << "Outputs: " << outputs_ << std::endl;
    std::cout << "Units in hidden layers: " << param.hiddenUnits << std::endl;
    std::cout << "Weight matrices: " << std::endl;
    for (i=0;i<matWeights_.size();++i) {
      std::cout << "Layer " << i << ":" << std::endl;
      std::cout << matWeights_[i] << std::endl;
    }
    std::cout << "---------------------------------------------------"
              << std::endl;
    std::cout << std::endl;
#endif
    return true;
  }

  void mlp::cleanActivationFunctions() {
    std::vector<activationFunctor*>::iterator it;
    for (it=activationFunctions_.begin();it!=activationFunctions_.end();++it){
      delete (*it);
      (*it)=0;
    }
    activationFunctions_.clear();
  }


  /**
   * Read a mlp::eTrainingType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,mlp::eTrainingType& data) {
    std::string str;
    
    if (handler.read(str)) {
      if (str == "ConjugateGradients") {
        data = mlp::ConjugateGradients;
      } else if (str == "SteepestDescent") {
        data = mlp::SteepestDescent;
      } else {
        data = mlp::SteepestDescent;
      }
    }
    return true;
  }

  /**
   * Write a mlp::eTrainingType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const mlp::eTrainingType& data) {
    bool b = false;
    switch(data) {
      case mlp::SteepestDescent:
        lti::write(handler,"SteepestDescent");
        break;
      case mlp::ConjugateGradients:
        lti::write(handler,"ConjugateGradients");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined mlp::eTrainingType");
        break;
    }
    return b;
  }


}
