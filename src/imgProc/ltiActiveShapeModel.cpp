/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009
 * Pedro Urenna / Escuela de Ing. Electronica
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
 * \file   ltiPointDistributionModel.h
 *         Contains the data structure used with the ASM.
 * \author Benjamin Winkler
 * \author Holger Fillbrandt
 * \date   01.03.2002
 * \author Pedro Urenna
 * \date   01.07.2009
 * revisions ..: $Id: ltiActiveShapeModel.cpp,v 1.8 2010-02-09 15:55:32 alvarado Exp $
 */

#include "ltiActiveShapeModel.h"


#undef _LTI_DEBUG
//#define _LTI_DEBUG 4

#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#include "ltiImage.h"
#include "ltiDraw.h"
#include "ltiRound.h"
#include <cstdio>
#endif

namespace lti {
  // --------------------------------------------------
  // activeShapeModel::parameters
  // --------------------------------------------------

  // default constructor
  activeShapeModel::parameters::parameters()
    : functor::parameters() {
    varianceCoefficient = 3.0;
    model = pointDistributionModel();
    reliabilityAsWeightingFactor = false;
  }

  // copy constructor
  activeShapeModel::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  activeShapeModel::parameters::~parameters() {
  }

  // returns the name of this class
  const std::string& activeShapeModel::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  activeShapeModel::parameters&
  activeShapeModel::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    varianceCoefficient = other.varianceCoefficient;
    model = other.model;
    reliabilityAsWeightingFactor = other.reliabilityAsWeightingFactor;
    
    return *this;
  }

  // alias for copy member
  activeShapeModel::parameters&
    activeShapeModel::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  activeShapeModel::parameters* activeShapeModel::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member function
  activeShapeModel::parameters* 
  activeShapeModel::parameters::newInstance() const {
    return new parameters();
  }
  
  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool activeShapeModel::parameters::write(ioHandler& handler,
                                           const bool complete) const
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      
      lti::write(handler,"varianceCoefficient",varianceCoefficient);
      lti::write(handler,"model",model);
      lti::write(handler,"reliabilityAsWeightingFactor",
                 reliabilityAsWeightingFactor);
    }

    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
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
  bool activeShapeModel::parameters::read(ioHandler& handler,
                                        const bool complete)
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"varianceCoefficient",varianceCoefficient);
      lti::read(handler,"model",model);
      lti::read(handler,"reliabilityAsWeightingFactor",
                reliabilityAsWeightingFactor);
    }
    
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // activeShapeModel
  // --------------------------------------------------

  // default constructor
  activeShapeModel::activeShapeModel()
    : functor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  activeShapeModel::activeShapeModel(const parameters& par)
    : functor(){

    // set the default parameters
    setParameters(par);
  }

  // copy constructor
  activeShapeModel::activeShapeModel(const activeShapeModel& other)
    : functor()  {
    copy(other);
  }

  // destructor
  activeShapeModel::~activeShapeModel() {
  }

  // returns the name of this type
  // returns the name of this class
  const std::string& activeShapeModel::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  activeShapeModel& activeShapeModel::copy(const activeShapeModel& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  activeShapeModel&
  activeShapeModel::operator=(const activeShapeModel& other) {
    return (copy(other));
  }


  // clone member
  activeShapeModel* activeShapeModel::clone() const {
    return new activeShapeModel(*this);
  }

  // newInstance member function
  activeShapeModel* activeShapeModel::newInstance() const {
    return new activeShapeModel();
  }
  
  // return parameters
  const activeShapeModel::parameters&
    activeShapeModel::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // convert a shape [(x1,y1), (x2,y2), ...] to a vector [x1, y1, x2, y2, ...]
  void activeShapeModel::buildVector(const pointDistributionModel::shape &src,
                                     dvector& destVector) const {
    
    destVector.allocate(src.size() * 2);
    pointDistributionModel::shape::const_iterator it;
    const pointDistributionModel::shape::const_iterator eit=src.end();
    int i;
    for (i=0,it=src.begin();it!=eit;++it) {
      destVector[i++] = (*it).x;
      destVector[i++] = (*it).y;
    }
  }

  // convert a vector [x1, y1, x2, y2, ...] into a shape [(x1,y1), (x2,y2), ...]
  void activeShapeModel::
  buildShape(const dvector &src,
             pointDistributionModel::shape& destShape) const {
    
    destShape.allocate(src.size()/2);
    int i,j;
    for (i=0,j=0; i < destShape.size(); ++i,j+=2) {
      destShape[i].set( static_cast<float>( src[j] ),
                        static_cast<float>( src[j+1] ) );
    }
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // trim shape
  bool activeShapeModel::
  trimShape(pointDistributionModel::shape& srcdest) const {
    return trimShape(srcdest, fvector(srcdest.size(), 1.0f));
  }




  // trim shape
  bool activeShapeModel::trimShape(pointDistributionModel::shape& srcdest,
				   const fvector &pointReliability) const {

    if (pointReliability.size() != static_cast<int>(srcdest.size())) {
      setStatusString("invalid reliability vector");
      return false;
    }
    

    // align shape with meanShape
    matrixTransform<float>::parameters trafoParameters;
    
    if (!alignShape(getParameters().model.meanShape,
                    pointReliability,
                    srcdest,
                    trafoParameters)) {
      return false;
    }

    // truncate weights of the transformed shape
    dvector temp;

    if (getParameters().reliabilityAsWeightingFactor) {
      // move unreliable points towards mean shape
      convertShapeToWeightsVector(srcdest, pointReliability, temp);
    } else {
      convertShapeToWeightsVector(srcdest, temp);
    }

    trimWeightsVector(temp);

    pointDistributionModel::shape tmpShape; // Declaración de un shape temporal

    convertWeightsVectorToShape(temp, tmpShape);

    // retransform trimmed shape
    matrixTransform<float> transformation;
    transformation.setParameters(trafoParameters);
    transformation.use(ipoint(0,0));
    transformation.backwards(tmpShape,srcdest);

    return true;
  }


  // trim weights according to given variance coefficient
  void activeShapeModel::trimWeightsVector(dvector& srcdest) const {

    const dvector& variances = getParameters().model.varianceVector;
    const double coeff = getParameters().varianceCoefficient;

    // cut srcdest if too big; fill with 0.0 if too small:
    srcdest.resize(variances.size(), 0.0f);

    // valid values range from -c*var to c*var
    for (int i=0; i < srcdest.size(); ++i) {
      const double c = coeff*sqrt(variances[i]);
      srcdest[i]=within(srcdest[i],-c,c);
    }
  }


  // convert a shape to a weights vector, i.e. project a shape into the PDM
  // space
  bool activeShapeModel::
  convertShapeToWeightsVector(const pointDistributionModel::shape &src,
                              dvector &dest) const {

    const parameters& par = getParameters();
    dvector vctA,vctB;
    
    buildVector(src,vctA);
    buildVector(par.model.meanShape,vctB);
    vctA.subtract(vctB);

    par.model.eigenVectorMatrix.leftMultiply(vctA,dest);
    return true;
  }




  // convert a shape to a weights vector and move unreliable points towards
  // mean shape
  bool activeShapeModel::
  convertShapeToWeightsVector(const pointDistributionModel::shape &src, 
                              const fvector &pointReliability,
                              dvector &dest) const {

    if (pointReliability.size() != static_cast<int>(src.size())) {
      setStatusString("Invalid reliability vector");
      return false;
    }

    const parameters& par = getParameters();

    dvector vctA,vctB;
    buildVector(src,vctA);
    buildVector(par.model.meanShape,vctB);
    vctA.subtract(vctB);

    int i,j;
    // move features towards mean shape
    for (i=0,j=0; i<pointReliability.size(); ++i) {
      vctA[j++] *= pointReliability[i];
      vctA[j++] *= pointReliability[i];
    }

    par.model.eigenVectorMatrix.leftMultiply(vctA,dest);

    return true;
  }




  // convert a parameter vector into a shape
  void activeShapeModel::
  convertWeightsVectorToShape(const dvector &src,
                              pointDistributionModel::shape &dest) const {

    const parameters& par = getParameters();
    dvector vct; 
    
    par.model.eigenVectorMatrix.multiply(src,vct);
    buildShape(vct,dest);
    dest.add(par.model.meanShape);
  }

  // calculate the center of gravity of a shape
  bool activeShapeModel::
  calculateShapeCenter(const pointDistributionModel::shape &shape,
                       fpoint& center) const {
    return calculateShapeCenter(shape, fvector(shape.size(), 1.0f),center);
  }

  // calculate the center of gravity of a weighted shape
  bool activeShapeModel::
  calculateShapeCenter(const pointDistributionModel::shape &shape,
                       const fvector &pointReliability,
                       fpoint& center) const {
    
    if (pointReliability.size() != static_cast<int>(shape.size())) {
      setStatusString("invalid reliability vector");
      return false;
    }
    
    pointDistributionModel::shape::const_iterator it = shape.begin();
    float totalReliability = 0.0f;

    center.set(0.0f, 0.0f);
    for (int i=0; i<static_cast<int>(shape.size()); ++i, ++it) {
      const float r=pointReliability[i];
      center += (*it) * r;
      totalReliability += r;
    }
    
    center /= totalReliability;

    return true;
  }
  



  // calculate the medium distance of a given point to the points of a shape
  bool activeShapeModel::
  calculateShapeDistance(const pointDistributionModel::shape &shape,
                         float& dist, 
                         const fpoint &thePoint) const {

    return calculateShapeDistance(shape,
                                  fvector(shape.size(), 1.0f),
                                  dist,
                                  thePoint);
  }




  // calculate the medium distance of a given point to the points of a weighted
  // shape
  bool activeShapeModel::
  calculateShapeDistance(const pointDistributionModel::shape &shape,
                         const fvector &pointReliability,
                         float& distance,
                         const fpoint &thePoint) const {

    if (pointReliability.size() != static_cast<int>(shape.size())) {
      setStatusString("invalid reliability vector");
      return false;
    }
    
    distance = 0.0;
    float sumOfElements(0.0f);

    pointDistributionModel::shape::const_iterator it = shape.begin();
    for (int i=0; i<static_cast<int>(shape.size()); ++i, ++it) {
      const float r = pointReliability[i];
      distance += sqrt(thePoint.distanceSqr(*it)) * r;
      sumOfElements += r;
    }

    if (sumOfElements == 0.0f) {
      setStatusString("reliability vector ignores all points");
      distance=1.0f;
      return false;
    }

    distance /= sumOfElements;
    return true;
  }




  // calculate the relative orientation of two shapes and a rotation center
  bool activeShapeModel::
  calculateShapeOrientation(const pointDistributionModel::shape &shape,
                            const pointDistributionModel::shape &referenceShape,
                            double& angle,
                            const fpoint &theCenter) const {

    return calculateShapeOrientation(shape,
                                     referenceShape, 
                                     fvector(shape.size(), 1.0f), 
                                     angle,
                                     theCenter);
  }


  // calculate angle between a point and the x-axis
  inline double activeShapeModel::calculateAngle(const fpoint &thePoint) const {
    return std::atan2(thePoint.y,thePoint.x);
  }


  // calculate the relative orientation of two shapes and a rotation center
  bool activeShapeModel::
  calculateShapeOrientation(const pointDistributionModel::shape &shape,
                            const pointDistributionModel::shape &refShape,
                            const fvector &pointReliability,
                            double& theRotation,
                            const fpoint &theCenter) const {

    theRotation = 0.0;

    if (shape.size() != refShape.size()) {
      setStatusString("Shapes have different sizes");
      return false;
    }

    if (pointReliability.size() != static_cast<int>(shape.size())) {
      setStatusString("Invalid reliability vector");
      return false;
    }

    // calculate rotation
    
    double relativeAngle;
    float sumR(0.0f);

    // accumulate relative angles of corresponding points
    for (int i=0;i<shape.size();++i) {
      const float r = pointReliability[i];
      relativeAngle = calculateAngle(refShape.at(i) - theCenter) - 
                      calculateAngle(shape.at(i) - theCenter);

      if (relativeAngle > Pi) {
        relativeAngle -= 2*Pi;
      } else if (relativeAngle < -Pi) {
        relativeAngle += 2*Pi;
      }

      // weigh angle
      theRotation += relativeAngle * r;
      sumR += r;
    }

    // average rotation of a point
    theRotation /= sumR;
    
    if (theRotation > Pi) {
      theRotation -= 2*Pi;
    } else if (theRotation < -Pi) {
      theRotation += 2*Pi;
    }

    return true;
  }

  //
  // calculate weighted scaling factor
  //
  bool activeShapeModel::
  calculateRelativeShapeScale(const pointDistributionModel::shape& shape,
                              const fpoint& shapeCenter,
                              const pointDistributionModel::shape& refShape,
                              const fpoint& refShapeCenter,
                              const fvector& pointReliability,
                              float& scale) const {
    
    scale = 0.0f;

    if (shape.size() != refShape.size()) {
      setStatusString("Shapes have different sizes");
      return false;
    }

    if (pointReliability.size() != shape.size()) {
      setStatusString("Invalid size for reliability vector");
      return false;
    }

    float shapeDist(0.0f), refShapeDist(0.0f),r;

    for (int i=0; i<shape.size(); ++i) {
      r=pointReliability[i];
      shapeDist    += sqrt(shapeCenter.distanceSqr(shape.at(i)))*r;
      refShapeDist += sqrt(refShapeCenter.distanceSqr(refShape.at(i)))*r;
    }
           
    scale = shapeDist / refShapeDist;

    return true;
  }



  // align a given shape to another
  bool activeShapeModel::
  alignShape(const pointDistributionModel::shape &refShape,
             pointDistributionModel::shape &shape,
             matrixTransform<float>::parameters& p
             ) const {
    return alignShape(refShape, fvector(shape.size(), 1.0f),shape, p);
  }


  // align a given weighted shape to another
  bool activeShapeModel::
  alignShape(const pointDistributionModel::shape& refShape,
             const fvector& pointReliability,
             pointDistributionModel::shape& shape,
             matrixTransform<float>::parameters&
             trafoParameters
             ) const {

    if (shape.size() != refShape.size()) {
      setStatusString("Shapes have different sizes");
      return false;
    }
    
    if (pointReliability.size() != shape.size()) {
      setStatusString("Invalid reliability vector");
      return false;
    }

    // We follow to the word the results at the appendix A of the
    // Cootes paper of 1995.
    
    // Here x2 is shape and x1 is the refShape
    // 
    dmatrix a(4,4,0.0);
    dvector v(4);
    double x1(0.0),y1(0.0),x2(0.0),y2(0.0),z(0.0),w(0.0),c1(0.0),c2(0.0);
    for (int k=0;k<shape.size();++k) {
      const float wk = pointReliability.at(k);
      const float x1k = refShape.at(k).x;
      const float y1k = refShape.at(k).y;
      const float x2k = shape.at(k).x;
      const float y2k = shape.at(k).y;

      x1+=wk*x1k;
      x2+=wk*x2k;
      y1+=wk*y1k;
      y2+=wk*y2k;
      z+=wk*(x2k*x2k+y2k*y2k);
      w+=wk;
      c1+=wk*(x1k*x2k+y1k*y2k);
      c2+=wk*(y1k*x2k-x1k*y2k);
    }

    a.at(0,0)=x2;
    a.at(0,1)=-y2;
    a.at(0,2)=w;
    a.at(1,0)=y2;
    a.at(1,1)=x2;
    a.at(1,3)=w;
    a.at(2,0)=z;
    a.at(2,2)=x2;
    a.at(2,3)=y2;
    a.at(3,1)=z;
    a.at(3,2)=-y2;
    a.at(3,3)=x2;

    v.at(0)=x1;
    v.at(1)=y1;
    v.at(2)=c1;
    v.at(3)=c2;
    
    dvector sol;
    lls_.apply(a,v,sol);
    
    fmatrix mt(3,3,0.0f);
    mt.at(0,0)=static_cast<float>(sol.at(0));
    mt.at(0,1)=static_cast<float>(-sol.at(1));
    mt.at(0,2)=static_cast<float>(sol.at(2));
    mt.at(1,0)=static_cast<float>(sol.at(1));
    mt.at(1,1)=static_cast<float>(sol.at(0));
    mt.at(1,2)=static_cast<float>(sol.at(3));
    mt.at(2,2)=1.0f;

    trafoParameters.transformation.copy(mt);
    matrixTransform<float> transformation(trafoParameters);
    
    pointDistributionModel::shape tmpShape; //temporal shape declaration
    transformation.forwards(shape,tmpShape);

    tmpShape.swap(shape);

#ifdef _LTI_DEBUG
    // recompute scale factor
    float s = sqrt(sqr(sol.at(0))+sqr(sol.at(1)));
    _lti_debug("  Scale s=" << s << "\n");
    
    // rotation angle
    float theta = atan2(sol.at(1),sol.at(0));
    _lti_debug("  Rotation theta=" << radToDeg(theta) << " degrees \n");

    // translation
    _lti_debug("  Translation by (" << sol.at(2) << "," << sol.at(3) << ")\n");

    show(refShape,shape,64.0f,ipoint(256,256));
    //show(shape,64.0f,ipoint(256,256));
    getchar();
#endif

    return true;
  }

  // calculate difference of two shapes, i.e. the sum of squared distances of
  // all corresponding points divided by the number of points
  bool activeShapeModel::
  calculateShapeDifference(const pointDistributionModel::shape &firstShape,
                           const pointDistributionModel::shape &secondShape,
                           float& diff) const {

    return calculateShapeDifference(firstShape, 
                                    secondShape, 
                                    fvector(firstShape.size(), 1.0f),
                                    diff);
  }

  // calculate difference of two weighted shapes
  bool activeShapeModel::
  calculateShapeDifference(const pointDistributionModel::shape &firstShape,
                           const pointDistributionModel::shape &secondShape,
                           const fvector &pointReliability,
                           float& theDifference) const {

    theDifference = 0.0f;

    if (firstShape.size() != secondShape.size()) {
      setStatusString("shapes have different sizes");
      return false;
    }

    if (pointReliability.size() != static_cast<int>(firstShape.size())) {
      setStatusString("invalid reliability vector");
      return false;
    }

    float sumR(0.0f);
    // calculate sum of square differences of all points
    for (int i=0;i<firstShape.size();++i) {
      const float r = pointReliability[i];
      theDifference += sqrt(firstShape.at(i).distanceSqr(secondShape.at(i)))*r; 
      sumR+=r;
    }

    theDifference /= sumR;
    return true;
  }

  //
  // calculate normal (actually bisecting line) of a point between two line
  // segments:
  // n = normalized( normalized(v1) + normalized(v2) ) * (-90 rotation);
  inline fpoint activeShapeModel::calculateNormal(const fpoint &a,
                                           const fpoint &b) const {
    fpoint s = getNormalizedVector(a-b); // segment from b to a
    return fpoint(-s.y,s.x);
  }

  //
  // return normalized vector
  //
  fpoint activeShapeModel::getNormalizedVector(const fpoint &vec) const {
    float squareLength = vec.absSqr();
    if (squareLength!=0.0) {
      return vec / sqrt(squareLength);
    } else {
      assert(squareLength != 0.0f);
      return vec;
    }
    
  }

  //
  // create normals for each point of the shape
  //
  void activeShapeModel::
  createVectorOfNormals(const pointDistributionModel::shape &theShape,
                        vector<fpoint>& normals) const {

    // several special cases
    if (theShape.size() < 2) {
      normals.assign(theShape.size(),fpoint(0.0f,0.0f));
      return;
    }
    
    if (theShape.size() == 2) {
      normals.allocate(theShape.size());
      
      normals.at(0) = (theShape.at(1)-theShape.at(0));
      normals.at(1) = (theShape.at(0)-theShape.at(1)); 

      return; 
    }

    // for the first point
    normals.at(0) = calculateNormal(normals.at(theShape.lastIdx()),
                                    normals.at(2));

    // for all but the extremes points
    for (int i=1;i<theShape.lastIdx();++i) {
      normals.at(i) = calculateNormal(normals.at(i-1),normals.at(i+1));
    }

    // for the last point
    normals.at(theShape.lastIdx()) 
      = calculateNormal(normals.at(theShape.lastIdx()-1),normals.at(0));
  }

#ifndef _LTI_DEBUG
  void activeShapeModel::show(const pointDistributionModel::shape&,
                              const float,
                              const ipoint) const {
  }

  void activeShapeModel::show(const pointDistributionModel::shape&,
                              const pointDistributionModel::shape&,
                              const float,
                              const ipoint) const {
  }

#else
  void activeShapeModel::show(const pointDistributionModel::shape &theShape,
                              const float scale,
                              const ipoint offset) const {
    image canvas(512,512,rgbaPixel(255,255,255));
    draw<rgbaPixel> painter;

    if (theShape.size() < 1) {
      return;
    }

    painter.use(canvas);
    ipoint p(iround(scale*theShape.at(0).x+offset.x),
             iround(scale*theShape.at(0).y+offset.y));
    ipoint q;

    painter.set(p);
    painter.setColor(rgbaPixel(255,64,64));
    painter.number(0,p);
    
    for (int i=1;i<theShape.size();++i) {
      q.set(iround(scale*theShape.at(i).x+offset.x),
            iround(scale*theShape.at(i).y+offset.y));
      
      painter.setColor(rgbaPixel(255,64,64));
      painter.line(p,q);
      painter.setColor(rgbaPixel(64,255,64));
      painter.marker(p.x,p.y,draw<rgbaPixel>::Xmark);
      painter.number(i,q);
      p=q;
    }
    painter.marker(q.x,q.y,draw<rgbaPixel>::Xmark);

    static viewer2D view("Shape");
    view.show(canvas);

  }

  void activeShapeModel::show(const pointDistributionModel::shape &refShape,
                              const pointDistributionModel::shape &theShape,
                              const float scale,
                              const ipoint offset) const {
    image canvas(512,512,rgbaPixel(255,255,255));
    draw<rgbaPixel> painter;

    if (theShape.size() < 1) {
      return;
    }

    painter.use(canvas);
    ipoint pr(iround(scale*refShape.at(0).x+offset.x),
              iround(scale*refShape.at(0).y+offset.y));
    ipoint qr;

    painter.set(pr);
    
    for (int i=1;i<theShape.size();++i) {
      qr.set(iround(scale*refShape.at(i).x+offset.x),
             iround(scale*refShape.at(i).y+offset.y));
      
      painter.setColor(rgbaPixel(128,128,192));
      painter.line(pr,qr);
      painter.setColor(rgbaPixel(128,128,32));
      painter.marker(pr.x,pr.y,draw<rgbaPixel>::Xmark);
      pr=qr;
    }
    painter.marker(qr.x,qr.y,draw<rgbaPixel>::Xmark);


    ipoint p(iround(scale*theShape.at(0).x+offset.x),
             iround(scale*theShape.at(0).y+offset.y));
    ipoint q;

    painter.set(p);

    for (int i=1;i<theShape.size();++i) {
      q.set(iround(scale*theShape.at(i).x+offset.x),
            iround(scale*theShape.at(i).y+offset.y));
      
      painter.setColor(rgbaPixel(255,64,64));
      painter.line(p,q);
      painter.setColor(rgbaPixel(64,255,64));
      painter.marker(p.x,p.y,draw<rgbaPixel>::Xmark);
      p=q;
    }

    painter.marker(q.x,q.y,draw<rgbaPixel>::Xmark);

    static viewer2D view("Shape");
    view.show(canvas);
  }

#endif

}
