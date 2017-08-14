/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiSammonsMapping.h
 *         Contains the class lti::sammonMapping,
 *         which maps points in a high dimensional space into a lower space.
 * 
 * \author Peter Doerfler
 * \date   9.10.2002
 *
 * revisions ..: $Id: ltiSammonMapping.cpp,v 1.1 2008-05-26 00:47:16 alvarado Exp $
 */

#include "ltiSammonMapping.h"
#include "ltiDistanceFunctor.h"
#include "ltiEuclidianDistance.h"
#include "ltiEuclidianNorm.h"
#include "ltiUniformContinuousDistribution.h"
#include "ltiSecondOrderStatistics.h"
#include "ltiPCA.h"
#include "ltiGenericVector.h"

#include <cstdio>

namespace lti {
  // --------------------------------------------------
  // sammonsMapping::parameters
  // --------------------------------------------------

  // default constructor
  sammonMapping::parameters::parameters()
    : functor::parameters() {

    dimensions = 2;
    steps = 200;
    distanceThresh = 0.;
    errorThresh=0.;
    alpha=0.35;
    initType = PCA;
    initBox.resize(dimensions,1.);
    searchType = Steepest;
    mu = 0.1;
  }

  // copy constructor
  sammonMapping::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  sammonMapping::parameters::~parameters() {
  }

  // get type name
  const std::string& sammonMapping::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  sammonMapping::parameters&
  sammonMapping::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    dimensions = other.dimensions;
    steps = other.steps;
    distanceThresh = other.distanceThresh;
    errorThresh = other.errorThresh;
    alpha = other.alpha;
    initType = other.initType;
    initBox.copy(other.initBox);
    searchType = other.searchType;
    mu = other.mu;
    
    return *this;
  }

  // alias for copy member
  sammonMapping::parameters&
  sammonMapping::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  sammonMapping::parameters* sammonMapping::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance member
  sammonMapping::parameters* sammonMapping::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool sammonMapping::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      switch(initType) {
        case Random:
          lti::write(handler,"initType","Random");
          break;
        case PCA:
          lti::write(handler,"initType","PCA");
          break;
        default:
          lti::write(handler,"initType","PCA");
      }
      
      switch(searchType) {
        case Steepest:
          lti::write(handler,"searchType","Steepest");
          break;
        case Gradient:
          lti::write(handler,"searchType","Gradient");
          break;
        case Momentum:
          lti::write(handler,"searchType","Momentum");
          break;
        default:
          lti::write(handler,"searchType","Steepest");
      }

      lti::write(handler,"dimensions",dimensions);
      lti::write(handler,"steps",steps);
      lti::write(handler,"distanceThresh",distanceThresh);
      lti::write(handler,"errorThresh",errorThresh);
      lti::write(handler,"alpha",alpha);
      lti::write(handler,"mu",mu);
      lti::write(handler,"initBox",initBox);
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
  bool sammonMapping::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      std::string str;

      lti::read(handler,"initType",str);
      if (str=="Random") {
        initType = Random;
      } else if (str=="PCA") {
        initType = PCA;
      } else {
        initType = PCA;
      }

      lti::read(handler,"searchType",str);
      if (str=="Steepest") {
        searchType = Steepest;
      } else if (str=="Gradient") {
        searchType = Gradient;
      } else if (str=="Momentum") {
        searchType = Momentum;
      } else {
        searchType = Steepest;
      }

      lti::read(handler,"dimensions",dimensions);
      lti::read(handler,"steps",steps);
      lti::read(handler,"distanceThresh",distanceThresh);
      lti::read(handler,"errorThresh",errorThresh);
      lti::read(handler,"alpha",alpha);
      lti::read(handler,"mu",mu);
      lti::read(handler,"initBox",initBox);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // sammonMapping
  // --------------------------------------------------

  // default constructor
  sammonMapping::sammonMapping()
    : functor(),progressReporter() {


    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  sammonMapping::sammonMapping(const parameters& par)
    : functor(),progressReporter() {


    // set the default parameters
    setParameters(par);
  }

  // copy constructor
  sammonMapping::sammonMapping(const sammonMapping& other)
    : functor(),progressReporter()  {
    copy(other);
  }

  // destructor
  sammonMapping::~sammonMapping() {
  }

  // returns the name of this type
  const std::string& sammonMapping::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  sammonMapping&
  sammonMapping::copy(const sammonMapping& other) {
    functor::copy(other);
    progressReporter::copy(other);

    return (*this);
  }

  // alias for copy member
  sammonMapping&
  sammonMapping::operator=(const sammonMapping& other) {
    return (copy(other));
  }


  // clone member
  sammonMapping* sammonMapping::clone() const {
    return new sammonMapping(*this);
  }

  sammonMapping* sammonMapping::newInstance() const {
    return new sammonMapping();
  }

  // return parameters
  const sammonMapping::parameters&
    sammonMapping::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type dmatrix!
  bool sammonMapping::apply(const dmatrix& src,dmatrix& dest) const {
    double error;
    return apply(src,dest,error);
  }


  // On copy apply for type dmatrix!
  bool sammonMapping::apply(const dmatrix& src,dmatrix& dest,
                             double& error) const {

    bool b=true;
    const parameters& param=getParameters();
    const int dim=param.dimensions;
    const int maxSteps=param.steps; 
    const double maxError=param.errorThresh;
    int i,j,k;
    const int r=src.rows();

    if (haveValidProgressObject()) {
      progressObject_->reset();
      std::string str("Sammon's mapping using ");
      switch (param.searchType) {
          case Gradient:
            str += "gradient descent";
            break;
          case Momentum:
            str += "gradient descent with momentum";
            break;
          case Steepest:
            str += "steepest descent";
            break;
          default:
            str += "unknown method";
      }
      progressObject_->setTitle(str);
      progressObject_->setMaxSteps(maxSteps+3);
      progressObject_->step("calculating distance matrix");
    }

    // contains distances between all points in src space
    genericVector<bool> leaveMe(r,false);
    dmatrix distances(r,r,0.);
    //set size of dest
    dest.resize(r, dim, 0.);

    for (i=0; i<r; i++) {
      if (leaveMe.at(i)) continue;
      for (j=i+1; j<r; j++) {
        if (leaveMe.at(j)) continue;
        distances.at(i,j) = euclidianDistance(src.getRow(i),src.getRow(j));
        distances.at(j,i)=distances.at(i,j);
        if (distances[i][j]<=param.distanceThresh) {
          leaveMe.at(j)=true;
        }
      }
    }

    double errorC=distances.computeSumOfElements()/2.;


    if (haveValidProgressObject()) {
      progressObject_->step("initializing mapping");
    }

    //initialize dest
    switch (param.initType) {
        case Random:
          initRandom(dest);
          break;
        case PCA:
          initPca(src,dest);
          break;
        default:
          initPca(src,dest);
    }

    int step=0;
    double dImg;
    double fac,dist;
    dvector diffE(dim);
    char buffer[256];

    error=0.;
    for (i=0; i<r; i++) {
      if (leaveMe.at(i)) continue;
      for (j=i+1; j<r; j++) {
        if (leaveMe.at(j)) continue;
        dist = euclidianDistance(dest[i],dest[j]);;
        error+=sqr(distances[i][j]-dist)/distances[i][j];
      }
    }
    error/=errorC;

    switch (param.searchType) {

      case Gradient: {
        
        while (error>maxError && step < maxSteps) {
          if (haveValidProgressObject()) {
            sprintf(buffer,"error = %f",error);
            progressObject_->step(buffer);
          }
          for (i=0; i<r; i++) {
            if (leaveMe.at(i)) continue;
            dvector& curr=dest.getRow(i);
            for (j=0; j<r; j++) {
              if (leaveMe.at(j)) continue;
              if (j==i) continue;
              dImg = euclidianDistance(curr, dest.getRow(j));;
              double& dStar=distances[i][j];
              fac=(dStar-dImg)/(dStar*dImg);
              for (k=0; k<dim; k++) {
                diffE[k]+=fac*(dest[i][k]-dest[j][k]);
              }
            }
            curr.addScaled(2.*param.alpha/errorC,diffE);
            diffE.fill(0.);
          }
          error=0.;
          for (i=0; i<r; i++) {
            if (leaveMe.at(i)) continue;
            for (j=i+1; j<r; j++) {
              if (leaveMe.at(j)) continue;
              dist = euclidianDistance(dest[i],dest[j]);;
              error+=sqr(distances[i][j]-dist)/distances[i][j];
            }
          }
          error/=errorC;
          step++;
        }
      }
        
      case Momentum: {
        
        dmatrix delta(r,dim,0.);
        while (error>maxError && step < maxSteps) {
          if (haveValidProgressObject()) {
            sprintf(buffer,"error = %f",error);
            progressObject_->step(buffer);
          }
          for (i=0; i<r; i++) {
            if (leaveMe.at(i)) continue;
            dvector& curr=dest.getRow(i);
            for (j=0; j<r; j++) {
              if (leaveMe.at(j)) continue;
              if (j==i) continue;
              dImg = euclidianDistance(curr, dest.getRow(j));;
              const double& dStar=distances[i][j];
              fac=(dStar-dImg)/(dStar*dImg);
              for (k=0; k<dim; k++) {
                diffE[k]+=fac*(dest[i][k]-dest[j][k]);
              }
            }
            diffE.multiply(-2./errorC);
            delta[i].multiply(param.mu);
            delta[i].addScaled(param.alpha,diffE);
            curr.subtract(delta[i]);
            diffE.fill(0.);
          }
          error=0.;
          for (i=0; i<r; i++) {
            if (leaveMe.at(i)) continue;
            for (j=i+1; j<r; j++) {
              if (leaveMe.at(j)) continue;
              dist = euclidianDistance(dest[i],dest[j]);;
              error+=sqr(distances[i][j]-dist)/distances[i][j];
            }
          }
          error/=errorC;
          step++;
        }
      }
        
      case Steepest:
      default: {
        
        double diffDim,diffE2norm;
        double dSub,dProd;
        dvector diffE2(dim);
        while ((error>maxError) && (step < maxSteps)) {
          if (haveValidProgressObject()) {
            sprintf(buffer,"error = %f",error);
            progressObject_->step(buffer);
          }
          for (i=0; i<r; i++) {
            if (leaveMe.at(i)) continue;
            dvector& curr=dest.getRow(i);
            for (j=0; j<r; j++) {
              if (leaveMe.at(j)) continue;
              if (j==i) continue;
              dImg = euclidianDistance(curr, dest.getRow(j));;
              const double& dStar=distances[i][j];
              dSub=dStar-dImg;
              dProd=dStar*dImg;
              fac=dSub/dProd;
              for (k=0; k<dim; k++) {
                diffDim=dest[i][k]-dest[j][k];
                diffE[k]+=fac*diffDim;
                diffE2[k]+=(dSub-diffDim*diffDim/dImg*(1.+dSub/dImg))/dProd;
              }
            }
            diffE2norm=euclidianNorm(diffE2);
            if (diffE2norm==0) {
              // std::cerr << "second derivative is zero\n";
              diffE2norm=1.E-4;
            }
            curr.addScaled(param.alpha/diffE2norm, diffE);
            diffE.fill(0.);
            diffE2.fill(0.);
          }
          error=0.;
          for (i=0; i<r; i++) {
            if (leaveMe.at(i)) continue;
            for (j=i+1; j<r; j++) {
              if (leaveMe.at(j)) continue;
              dist = euclidianDistance(dest[i],dest[j]);;
              error+=sqr(distances[i][j]-dist)/distances[i][j];
            }
          }
          error/=errorC;
          step++;
        }
      }
    }

    // release lots of memory
    distances.clear();

    secondOrderStatistics<double> meanFunc;
    dvector destMean;
    meanFunc.mean(dest,destMean);
    for (i=0;i<r;i++) {
      dest[i].subtract(destMean);
    }

    if (haveValidProgressObject()) {
      sprintf(buffer,"done. error = %f",error);
      progressObject_->step(buffer);
    }

    return b;
  }


  void sammonMapping::initRandom(dmatrix& dest) const {
    int i,j;
    uniformContinuousDistribution rand;
    uniformContinuousDistribution::parameters randP;
    randP.min=0.;
    const dvector& box=getParameters().initBox;
    for (j=0; j<dest.columns(); j++) {
      randP.max=box[j];
      rand.setParameters(randP);
      for (i=0; i<dest.rows(); i++) {
        dest.at(i,j)=rand.draw();
      }
    }
  }

  void sammonMapping::initPca(const dmatrix& data, dmatrix& dest) const {

    pca<double> pcar;
    pcar.setDimension(dest.columns());

    pcar.apply(data,dest);

  }


}
