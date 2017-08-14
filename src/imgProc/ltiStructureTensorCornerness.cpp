/*
 * Copyright (C) 1998-2005
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
 * \file   ltiStructureTensorCornerness.cpp
 *         Contains the class structureTensorCornerness, , a
 *         cornernessFunctor which uses the structure tensor
 * \author Peter Doerfler
 * \date   13.1.2005
 *
 * revisions ..: $Id: ltiStructureTensorCornerness.cpp,v 1.7 2013-03-10 05:58:31 alvarado Exp $
 */

#include "ltiStructureTensorCornerness.h"
#include "ltiGaussKernels.h"

namespace lti {
  // --------------------------------------------------
  // structureTensorCornerness::parameters
  // --------------------------------------------------

  // default constructor
  structureTensorCornerness::parameters::parameters()
    : cornernessFunctor::parameters(), gradientParameters() {
    
    integrationKernelSize = 7;
    integrationVariance = -1.f;
    alpha = 0.04f;
    gradientParameters.kernelType = gradientFunctor::OGD;
    colorGradientParameters.kernelType = colorContrastGradient::OGD;
  }

  // copy constructor
  structureTensorCornerness::parameters::parameters(const parameters& other)
    : cornernessFunctor::parameters() {
    copy(other);
  }

  // destructor
  structureTensorCornerness::parameters::~parameters() {
  }

  // copy member

  structureTensorCornerness::parameters&
  structureTensorCornerness::parameters::copy(const parameters& other) {
    cornernessFunctor::parameters::copy(other);

    
      integrationKernelSize = other.integrationKernelSize;
      alpha = other.alpha;
      gradientParameters = other.gradientParameters;
      colorGradientParameters = other.colorGradientParameters;
      integrationVariance = other.integrationVariance;

    return *this;
  }

  // alias for copy method
  structureTensorCornerness::parameters&
  structureTensorCornerness::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& structureTensorCornerness::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  structureTensorCornerness::parameters* 
  structureTensorCornerness::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  structureTensorCornerness::parameters* 
  structureTensorCornerness::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool structureTensorCornerness::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"integrationKernelSize",integrationKernelSize);
      lti::write(handler,"integrationVariance",integrationVariance);
      lti::write(handler,"alpha",alpha);
      lti::write(handler,"gradientParameters",gradientParameters);
      lti::write(handler,"colorGradientParameters",colorGradientParameters);
    }

    b = b && cornernessFunctor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool structureTensorCornerness::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"integrationKernelSize",integrationKernelSize);
      lti::read(handler,"integrationVariance",integrationVariance);
      lti::read(handler,"alpha",alpha);
      lti::read(handler,"gradientParameters",gradientParameters);
      lti::read(handler,"colorGradientParameters",colorGradientParameters);
    }

    b = b && cornernessFunctor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // structureTensorCornerness
  // --------------------------------------------------

  // default constructor
  structureTensorCornerness::structureTensorCornerness()
    : cornernessFunctor(), gradient_(), colorGradient_(), gaussFilter_() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

    this->cornernessType_=Max;
  }

  // default constructor
  structureTensorCornerness::structureTensorCornerness(const parameters& par)
    : cornernessFunctor(), gradient_(), colorGradient_(), gaussFilter_() {

    // set the given parameters
    setParameters(par);

    this->cornernessType_=Max;
  }

  // copy constructor
  structureTensorCornerness::structureTensorCornerness(const structureTensorCornerness& other)
    : cornernessFunctor(), gradient_(), colorGradient_(), gaussFilter_() {
    copy(other);

    this->cornernessType_=Max;
  }

  // destructor
  structureTensorCornerness::~structureTensorCornerness() {
  }

  // copy member
  structureTensorCornerness& 
  structureTensorCornerness::copy(const structureTensorCornerness& other) {
    cornernessFunctor::copy(other);

    gradient_.copy(other.gradient_);
    colorGradient_.copy(other.colorGradient_);
    gaussFilter_.copy(other.gaussFilter_);

    return (*this);
  }

  // alias for copy member
  structureTensorCornerness&
  structureTensorCornerness::operator=(const structureTensorCornerness& other){
    return (copy(other));
  }

  // class name
  const std::string& structureTensorCornerness::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  structureTensorCornerness* structureTensorCornerness::clone() const {
    return new structureTensorCornerness(*this);
  }

  // create a new instance
  structureTensorCornerness* structureTensorCornerness::newInstance() const {
    return new structureTensorCornerness();
  }

  // return parameters
  const structureTensorCornerness::parameters&
  structureTensorCornerness::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool structureTensorCornerness::updateParameters() {

    bool b = true;

    const parameters& par=getParameters();

    gradientFunctor::parameters gradP = par.gradientParameters;
    gradP.format = gradientFunctor::Cartesian;
    b = b && gradient_.setParameters(gradP);

    colorContrastGradient::parameters colorP = par.colorGradientParameters;
    colorP.format = colorContrastGradient::Cartesian;
    b = b && colorGradient_.setParameters(colorP);

    gaussKernel2D<float> gk(par.integrationKernelSize,par.integrationVariance);
    convolution::parameters filterPar;
    filterPar.boundaryType = lti::Constant;
    filterPar.setKernel(gk);
    b = b && gaussFilter_.setParameters(filterPar);

    return b;
  }

  // -------------------------------------------------------------------
  // The apply helper functions!
  // -------------------------------------------------------------------

  /*
   * compute the structure tensor and integrate with Gauss kernel.
   */
  bool structureTensorCornerness::computeStructureTensor(const channel& src,
                                                         channel& gx2,
                                                         channel& gy2,
                                                         channel& gxgy) const {

    bool b=gradient_.apply(src,gx2,gy2);

    gxgy.allocate(src.size());

    channel::iterator igx=gx2.begin();
    channel::iterator igxend=gx2.end();
    channel::iterator igy=gy2.begin();
    channel::iterator igxy=gxgy.begin();

    while (igx!=igxend) {
      const float tx=(*igx);
      const float ty=(*igy);
      (*igx)=tx*tx;
      (*igy)=ty*ty;
      (*igxy)=tx*ty;
      ++igx; ++igy; ++igxy;
    }

    return (b && 
            gaussFilter_.apply(gx2) && 
            gaussFilter_.apply(gy2) && 
            gaussFilter_.apply(gxgy));
  }

  /*
   * compute the structure tensor and integrate with Gauss kernel.
   */
  bool structureTensorCornerness::computeStructureTensor(const image& src,
                                                         channel& gx2,
                                                         channel& gy2,
                                                         channel& gxgy) const {

    bool b=colorGradient_.apply(src,gx2,gy2);

    gxgy.allocate(src.size());

    channel::iterator igx=gx2.begin();
    channel::iterator igxend=gx2.end();
    channel::iterator igy=gy2.begin();
    channel::iterator igxy=gxgy.begin();

    while (igx!=igxend) {
      const float tx=(*igx);
      const float ty=(*igy);
      (*igx)=tx*tx;
      (*igy)=ty*ty;
      (*igxy)=tx*ty;
      ++igx; ++igy; ++igxy;
    }

    return (b && 
            gaussFilter_.apply(gx2) && 
            gaussFilter_.apply(gy2) && 
            gaussFilter_.apply(gxgy));
  }

  bool structureTensorCornerness::computeCornerness(const channel& gx2,
                                                    const channel& gy2,
                                                    const channel& gxgy,
                                                    channel& cornerness,
                                                    float& minCornerness,
                                                    float& maxCornerness)const{

    maxCornerness = 0.0f;
    minCornerness = std::numeric_limits<float>::max();

    // we can assume that all channels are connected, but try it out if not
    assert (gx2.getMode() == channel::Connected);
    assert (gy2.getMode() == channel::Connected);
    assert (gxgy.getMode() == channel::Connected);
    
    if (gx2.empty() || gy2.empty() || gxgy.empty()) {
      setStatusString("Empty gradient in structureTensorCornerness::computeCornerness\n");
      cornerness.clear();
      return false;
    }
    
    int i;
    const int end = gx2.rows()*gx2.columns();
    const float *const pgx2 = &gx2.at(0,0);
    const float *const pgxgy = &gxgy.at(0,0);
    const float *const pgy2 = &gy2.at(0,0);

    cornerness.allocate(gx2.size());
    float* pcor = &cornerness.at(0,0);

    const float alpha = getParameters().alpha;

    for (i=0;i<end;++i) {
      const float txx=pgx2[i];
      const float txy=pgxgy[i];
      const float tyy=pgy2[i];
      const float det=txx*tyy - txy*txy;
      const float trace=txx+tyy;
      const float c = det-alpha*trace*trace;
      pcor[i]=c;
      if (c>maxCornerness) {
        maxCornerness=c;
      } else if (c<minCornerness) {
        minCornerness=c;
      }
    }

    return true;

  }

  // -------------------------------------------------------------------
  // The workers
  // -------------------------------------------------------------------

  bool structureTensorCornerness::worker(const channel& src, channel& dest,
                                         float& minCornerness, 
                                         float& maxCornerness) const {
    channel gx2, gy2, gxgy;
    return (computeStructureTensor(src,gx2,gy2,gxgy) 
            && computeCornerness(gx2,gy2,gxgy,dest,
                                 minCornerness,maxCornerness));
  }
  
  bool structureTensorCornerness::worker(const image& src, channel& dest,
                                         float& minCornerness, 
                                         float& maxCornerness) const {
    channel gx2, gy2, gxgy;
    return (computeStructureTensor(src,gx2,gy2,gxgy) 
            && computeCornerness(gx2,gy2,gxgy,dest,
                                 minCornerness,maxCornerness));
  }

}

