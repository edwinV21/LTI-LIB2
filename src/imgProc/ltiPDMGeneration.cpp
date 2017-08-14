/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiPDMGeneration.h
 *         Class to compute a shape model from a set of sample shapes.
 * \author Benjamin Winkler
 * \date   26.04.2001
 * \author Pablo Alvarado
 * \date   01.07.2009
 * revisions ..: $Id: ltiPDMGeneration.cpp,v 1.9 2013-03-08 01:51:12 alvarado Exp $
 */

#include "ltiPDMGeneration.h"
#include "ltiSymmetricEigenSystem.h"
#include "ltiBilinearInterpolation.h"
#include "ltiMatrixTransform.h"
#include "ltiPCA.h"
#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4

#include "ltiDebug.h"



namespace lti {
  // --------------------------------------------------
  // pdmGeneration::parameters
  // --------------------------------------------------

  // default constructor
  pdmGeneration::parameters::parameters()
    : functor::parameters() {

    maxNumberOfWeights = 0;
    maxRelativeWeights = 1.0f;
    maxAlignmentIterations = 100;
  }

  // copy constructor
  pdmGeneration::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  pdmGeneration::parameters::~parameters() {
  }

  // get type name
  const std::string& pdmGeneration::parameters::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member

  pdmGeneration::parameters&
  pdmGeneration::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    maxNumberOfWeights = other.maxNumberOfWeights;
    maxRelativeWeights = other.maxRelativeWeights;
    maxAlignmentIterations = other.maxAlignmentIterations;
    return *this;
  }

  // alias for copy member
  pdmGeneration::parameters&
  pdmGeneration::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  pdmGeneration::parameters* pdmGeneration::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  pdmGeneration::parameters* pdmGeneration::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool pdmGeneration::parameters::write(ioHandler& handler,
                                       const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"maxNumberOfWeights",maxNumberOfWeights);
      lti::write(handler,"maxRelativeWeights",maxRelativeWeights);
      lti::write(handler,"maxAlignmentIterations",maxAlignmentIterations);
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
  bool pdmGeneration::parameters::read(ioHandler& handler,
                                      const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"maxNumberOfWeights",maxNumberOfWeights);
      lti::read(handler,"maxRelativeWeights",maxRelativeWeights);
      lti::read(handler,"maxAlignmentIterations",maxAlignmentIterations);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // pdmGeneration
  // --------------------------------------------------

  // default constructor
  pdmGeneration::pdmGeneration() : functor() {
    parameters params;
    setParameters(params);
    clear();
  }

  // copy constructor
  pdmGeneration::pdmGeneration(const pdmGeneration& other) : functor()  {
    copy(other);
  }

  pdmGeneration::pdmGeneration(const pdmGeneration::parameters& par) :
    functor() {
    setParameters(par);
    clear();
  }

  // destructor
  pdmGeneration::~pdmGeneration() {
  }

  // returns the name of this type
  const std::string& pdmGeneration::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member
  pdmGeneration& pdmGeneration::copy(const pdmGeneration& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  pdmGeneration* pdmGeneration::clone() const {
    return new pdmGeneration(*this);
  }

  pdmGeneration* pdmGeneration::newInstance() const {
    return new pdmGeneration;
  }

  // return parameters
  const pdmGeneration::parameters&
  pdmGeneration::getParameters() const {
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

  /*
   * generate a pointDistributionModel given a set of shapes considered so far
   */
  void pdmGeneration::generate(pointDistributionModel &theModel) {

    // align the shapes 
    alignShapes();
    

    // Take all shapes and pack them into a dmatrix called data
    dmatrix data;
    calculateShapeMatrix(data);

    // Compute the eigenvalues and eigenvectors
    pca<double>::parameters pcaPar;
    pcaPar.resultDimension = data.columns(); // compute all components
    pcaPar.centerData = true; // ensure that the mean value is centered
    pcaPar.autoDimension = true;
    pca<double> p(pcaPar);
    p.train(data);

    // Get the eigenvectors
    theModel.eigenVectorMatrix.copy(p.getEigenVectors());

    // Get eigenvalues
    dvector eigenVals;
    p.getEigenValues(eigenVals);
    theModel.varianceVector.copy(eigenVals);

    // Get the mean value
    buildShape(p.getOffsetVector(),theModel.meanShape);
  }


  /*
   * generate pointDistributionModel
   */
  bool pdmGeneration::apply(pointDistributionModel& theModel) {

    if (shapes_.size() < 2) {
      setStatusString("more than one shape needed");
      return false;
    }

    generate(theModel);

    const parameters par = getParameters();

    const double absoluteIndex = theModel.varianceVector.computeSumOfElements();
    const float relativeMax = lti::within(par.maxRelativeWeights,0.0f,1.0f);
    int dimensionCount = par.maxNumberOfWeights;

    if ((dimensionCount <= 0) || 
        (dimensionCount > theModel.varianceVector.size())) {
      dimensionCount = theModel.varianceVector.size();
    }

    double relativeIndex = 0.0;
    const double relativeLimit = absoluteIndex * relativeMax;
    for (int index = 0; index < dimensionCount; ++index) {
      relativeIndex += theModel.varianceVector[index];

      if (relativeIndex > relativeLimit) {
        dimensionCount = index + 1;
        break;
      }
    }

    theModel.eigenVectorMatrix.resize(theModel.eigenVectorMatrix.rows(),
                                      dimensionCount);
    theModel.varianceVector.resize(dimensionCount);

    return true;
  }


  //
  // take shape into consideration
  //
  bool pdmGeneration::consider(const pointDistributionModel::shape &theShape) {
    if (shapeSize_ == 0) {
      shapeSize_ = theShape.size();
    }

    if (theShape.size() != shapeSize_) {
      setStatusString("shapes differ in size");
      return false;
    }
    
    shapes_.push_back(theShape);
    return true;
  }

  //
  // take list of shapes into consideration
  //
  bool pdmGeneration::
  consider(const std::list<pointDistributionModel::shape> &theShapes) {
    std::list<pointDistributionModel::shape>::const_iterator it;

    bool res=true;
    for (it = theShapes.begin(); !(it == theShapes.end()); ++it) {
      res = consider(*it) && res;
    }

    return res;
  }

  //
  // return number of elements considered so far
  //
  int pdmGeneration::getN() const {
    return static_cast<int>(shapes_.size());
  }

  //
  // clear all considered shapes
  //
  void pdmGeneration::clear() {
    shapes_.clear();
    shapeSize_ = 0;
  }

  //
  // private methods
  //

  /*
   * - align all shapes to cog=(0,0), meanDistance=1.0
   * - create mean shape
   * - align all shapes iteratively to the mean shape
   */
  void pdmGeneration::alignShapes() {

    pointDistributionModel::shape meanShape;

    if (shapes_.size() == 0) {
      return meanShape.clear();
    }

    const parameters& par = getParameters();
    std::list<pointDistributionModel::shape>::iterator it = shapes_.begin();

    // normalize first shape (position and scale)
    normalizeShape(*it);

    // dummy transformation parameters.  They won't be used here, but to call
    // the align function...
    matrixTransform<float>::parameters mtPar;

    _lti_debug("Aligning all shapes to the first one\n");
    _lti_if_debug(int _c = 0);

    // align all shapes to the first (1)
    for(++it;it != shapes_.end(); ++it) {
      theASM_.alignShape(shapes_.front(),*it, mtPar);
      _lti_debug("  Shape " << ++_c << std::endl);
    }

    // calculate (normalized) mean shape (2)
    pointDistributionModel::shape newMeanShape;
    createMeanShape(newMeanShape);
    normalizeShape(newMeanShape);

    float difference(0.0f);
    float newDifference(std::numeric_limits<float>::max());
    int run = 0;

    _lti_debug("Recomputing mean shape\n");

    // until meanShape converges:
    do {
      _lti_debug("  Iteration: " << run << "\n");

      // move newMeanShape to meanShape
      meanShape.swap(newMeanShape);

      _lti_if_debug(_c = 1);
      // align all shapes to mean shape (4)
      for (it = shapes_.begin();it!=shapes_.end();++it) {
        theASM_.alignShape(meanShape,*it,mtPar);
        _lti_debug("  Shape " << ++_c << " aligned to last mean" << std::endl);
      }

      // calculate (normalized) mean shape (5a)
      createMeanShape(newMeanShape);
      normalizeShape(newMeanShape);

      // calculate difference (5b, least squares approach)
      difference = newDifference;
      theASM_.calculateShapeDifference(meanShape, newMeanShape, newDifference);

      run++;

      _lti_debug("Diference: " << newDifference << std::endl);

    } while ((newDifference < difference) &&
             (newDifference != 0.0f) &&
             (run < par.maxAlignmentIterations)); 

  }


  /*
   * create mean shape
   */
  void pdmGeneration::
  createMeanShape(pointDistributionModel::shape& newShape) const {

    newShape.assign(shapeSize_,fpoint(0.0f,0.0f));

    std::list<pointDistributionModel::shape>::const_iterator shapesIt;

    int i(0);
    for (shapesIt=shapes_.begin(); shapesIt!=shapes_.end(); ++shapesIt,++i) {
      newShape.add(*shapesIt);
    }

    newShape/=fpoint(float(i),float(i));
  }

  void pdmGeneration::calculateShapeMatrix(dmatrix& data) const {
    if (shapes_.empty()) {
      data.clear();
      return;
    }
    
    const int cols = shapes_.front().size()*2;
    const int rows = static_cast<int>(shapes_.size()); // this takes its time, but we have to...
    data.allocate(rows,cols);
    std::list<pointDistributionModel::shape>::const_iterator it;
    int i;
    for (i=0,it=shapes_.begin();i<rows;++i,++it) {
      buildVector((*it),data.getRow(i));
    }
  }


  /*
   * create shape vector (x1, y1, x2, y2, ...)
   */
  void pdmGeneration::buildVector(const pointDistributionModel::shape &theShape,
                                  dvector& vct) const {

    const int size = theShape.size()*2;
    vct.allocate(size);

    pointDistributionModel::shape::const_iterator vIt = theShape.begin();
    const pointDistributionModel::shape::const_iterator eIt = theShape.end();

    int index = 0;

    while (vIt != eIt) {
      vct[index++] = (*vIt).x;
      vct[index++] = (*vIt).y;
      ++vIt;
    }
  }

  /*
   * create shape vector (x1, y1, x2, y2, ...)
   */
  void
  pdmGeneration::buildShape(const dvector& vct,
                            pointDistributionModel::shape &theShape) const {
  
    if ((vct.size()%2)!= 0) {
      return; // an odd size should never occur!
    }

    const int size = vct.size()/2;
    theShape.allocate(size);

    pointDistributionModel::shape::iterator sIt = theShape.begin();
    const pointDistributionModel::shape::iterator eIt = theShape.end();

    int index = 0;

    while (sIt != eIt) {
      (*sIt).x = static_cast<float>(vct[index++]);
      (*sIt).y = static_cast<float>(vct[index++]);
      ++sIt;
    }
  }


  //
  // normalize shape (i.e. move to (0,0), scale to 1.0)
  //
  void pdmGeneration::
  normalizeShape(pointDistributionModel::shape &theShape) const {

    typedef matrixTransform<float> geo_type;
    geo_type transformation;
    geo_type::parameters trafoParams;

    // shift shape to (0,0)
    fpoint shapeCOG; 
    theASM_.calculateShapeCenter(theShape,shapeCOG);

    translationMatrix(-shapeCOG).detach(trafoParams.transformation);
    
    float distance;
    theASM_.calculateShapeDistance(theShape,distance,shapeCOG);

    // scale shape to size 1.0
    trafoParams.transformation.leftMultiply(scalingMatrix(1.0f/distance));
    transformation.setParameters(trafoParams);
    transformation.use(ipoint(0,0));
    pointDistributionModel::shape tmpShape;
    transformation.forwards(theShape,tmpShape);
    tmpShape.swap(theShape);
  }

} // namespace lti
