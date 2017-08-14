/*
 * Copyright (C) 2002 2003 2004 2005 2006
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
 * \file   ltiCannyEdges.cpp
 *         Contains the class lti::cannyEdges, 
 *         which implements the classical Canny Edge Detector.
 * 
 * \author Christian Harte
 * \author Pablo Alvarado
 * \date   19.07.2002
 *
 * revisions ..: $Id: ltiCannyEdges.cpp,v 1.3 2012-07-25 23:33:30 alvarado Exp $
 */

#include "ltiCannyEdges.h"
#include "ltiGaussKernels.h"
#include "ltiGradientKernels.h"
#include "ltiSplitImageToRGB.h"
#include "ltiMaximumFilter.h"
#include "ltiConstants.h"
#include "ltiConvolution.h"

#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#endif

namespace lti {
  // --------------------------------------------------
  // cannyEdges::parameters
  // --------------------------------------------------

  // default constructor
  cannyEdges::parameters::parameters()
    : edgeDetection::parameters() {

    variance = float(1.0f);
    kernelSize = 7;
    thresholdMin = float(0.5f);
    thresholdMax = float(0.04f);
    gradientParameters.kernelType = gradientFunctor::Difference;
    gradientParameters.format     = gradientFunctor::Polar;
  }

  // copy constructor
  cannyEdges::parameters::parameters(const parameters& other)
    : edgeDetection::parameters()  {
    copy(other);
  }

  // destructor
  cannyEdges::parameters::~parameters() {
  }

  // get type name
  const std::string& cannyEdges::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  cannyEdges::parameters&
  cannyEdges::parameters::copy(const parameters& other) {
    edgeDetection::parameters::copy(other);

    variance = other.variance;
    kernelSize = other.kernelSize;
    thresholdMin = other.thresholdMin;
    thresholdMax = other.thresholdMax;
    gradientParameters = other.gradientParameters;

    return *this;
  }

  // alias for copy member
  cannyEdges::parameters&
    cannyEdges::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  cannyEdges::parameters* cannyEdges::parameters::clone() const {
    return new parameters(*this);
  }

  cannyEdges::parameters* cannyEdges::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool cannyEdges::parameters::write(ioHandler& handler,
                                     const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"variance",variance);
      lti::write(handler,"kernelSize",kernelSize);
      lti::write(handler,"thresholdMin",thresholdMin);
      lti::write(handler,"thresholdMax",thresholdMax);
      lti::write(handler,"gradientParameters",gradientParameters);
    }

    b = b && edgeDetection::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool cannyEdges::parameters::read(ioHandler& handler,
                                    const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"variance",variance);
      lti::read(handler,"kernelSize",kernelSize);
      lti::read(handler,"thresholdMin",thresholdMin);
      lti::read(handler,"thresholdMax",thresholdMax);
      lti::read(handler,"gradientParameters",gradientParameters);
      
    }

    b = b && edgeDetection::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // cannyEdges
  // --------------------------------------------------

  /*
   * convert a pair of values in cartesic system into a pair in
   * polar system
   */
  inline void cannyEdges::cartToPolar(const int dx,const int dy,
                                      float& mag,float& angle) const {
    static const float factor = 1.0f/255.0f;
    mag = factor*sqrt(float(dx*dx+dy*dy));
    angle = static_cast<float>(atan2(dy,dx));
  }

  // default constructor
  cannyEdges::cannyEdges()
    : edgeDetection(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

  }

  // default constructor
  cannyEdges::cannyEdges(const parameters& thePars)
    : edgeDetection() {

    // set the default parameters
    setParameters(thePars);
  }


  // copy constructor
  cannyEdges::cannyEdges(const cannyEdges& other)
    : edgeDetection() {

    copy(other);
  }

  // destructor
  cannyEdges::~cannyEdges() {
  }

  // returns the name of this type
  const std::string& cannyEdges::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  cannyEdges& cannyEdges::copy(const cannyEdges& other) {
    edgeDetection::copy(other);

    return (*this);
  }

  // alias for copy member
  cannyEdges&
    cannyEdges::operator=(const cannyEdges& other) {
    return (copy(other));
  }


  // clone member
  cannyEdges* cannyEdges::clone() const {
    return new cannyEdges(*this);
  }

  cannyEdges* cannyEdges::newInstance() const {
    return new cannyEdges;
  }

  // return parameters
  const cannyEdges::parameters& cannyEdges::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // set parameters
  bool cannyEdges::updateParameters() {
    const parameters& par = getParameters();
    nonMaximaSuppression::parameters nmsPar;
    nmsPar.thresholdMin = par.thresholdMin;
    nmsPar.thresholdMax = (par.thresholdMax<=1.0) ? par.thresholdMax :
      par.thresholdMax/255.0f;
    nmsPar.edgeValue    = par.edgeValue;
    nmsPar.background   = par.noEdgeValue;
    nmsPar.checkAngles  = false;
    
    colorContrastGradient::parameters gradPar(par.gradientParameters);
    gradPar.format = colorContrastGradient::Polar;
    
    return (grad_.setParameters(gradPar) && suppressor_.setParameters(nmsPar));
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type channel8!
  bool cannyEdges::apply(channel8& srcdest) const {
    return edgeDetection::apply(srcdest);
  }

  // On place apply for type channel!
  bool cannyEdges::apply(channel& srcdest) const {
    return edgeDetection::apply(srcdest);
  }

  // On copy apply for type channel8!
  bool cannyEdges::apply(const channel8& src,channel8& dest) const {
    channel orient;
    return apply(src,dest,orient);
  }

  // On copy apply for type channel8!
  bool cannyEdges::apply(const channel8& src,
                               channel8& dest,
                               channel& orientation) const {
    channel gradAbs;
    return apply(src,dest,orientation,gradAbs);
  }

  // On copy apply for type channel8!
  bool cannyEdges::apply(const channel8& src,
                               channel8& dest,
                               channel& orientation,
                               channel& gradAbs) const {

    const parameters& param = getParameters();
    if (src.empty()) {
      setStatusString("input image empty");
      return(false);
    }

    dest.allocate(src.size());

    // temporary images
    channel8 chanSmoothed; // low-pass filtered image (noise reduction)
    // channel gradAbs;       // magnitude of gradient

    gradAbs.allocate(src.size());
    orientation.allocate(src.size());

    _lti_debug("Gauss Filter");
    const channel8* smoothPtr = 0;

    // apply Gauss filter to smooth image, only if desired
    if (param.kernelSize > 0) {
      gaussKernel2D<ubyte> kernel(param.kernelSize,param.variance);
      
      convolution::parameters filterParam;       // parameters
      filterParam.setKernel(kernel);             // use the gauss kernel
      filterParam.boundaryType = lti::Constant;
      convolution filter(filterParam);           // convolution operator
      
      // filter the channel 
      filter.apply(src,chanSmoothed);
      smoothPtr = &chanSmoothed;
    } else {
      smoothPtr = &src;
    }

    _lti_debug("Differentiation and result in polar coordinates");

    // compute the gradient
    if (grad_.apply(*smoothPtr,gradAbs,orientation)) {

#     ifdef _LTI_DEBUG
      static viewer2D viewAbs("Abs"),viewArg("Arg");
      viewAbs.show(gradAbs);
      viewArg.show(orientation);
#     endif      

      // if gradient ok, then non-maxima suppression and hystheresis
      return suppressor_.apply(gradAbs,orientation,dest);
    }
   
    return false;
  }

  // On copy apply for type channel8!
  bool cannyEdges::apply(const channel& src,channel8& dest) const {
    channel8 tmp;
    tmp.castFrom(src);
    return apply(tmp,dest);
  }

  // On copy apply for type channel8!
  bool cannyEdges::apply(const channel& src,channel8& dest,channel& o) const {
    channel8 tmp;
    tmp.castFrom(src);
    return apply(tmp,dest,o);
  }

  // On copy apply for type channel8!
  bool cannyEdges::apply(const channel& src,
                         channel8& dest,
                         channel& o,
                         channel& strength) const {
    channel8 tmp;
    tmp.castFrom(src);
    return apply(tmp,dest,o,strength);
  }


  // On copy apply for type channel!
  bool cannyEdges::apply(const channel& src,channel& dest) const {
    channel8 tmp;
    if (apply(src,tmp)) {
      dest.castFrom(tmp);
      return true;
    }
    return false;
  }

  // On copy apply for type channel!
  bool cannyEdges::apply(const image& src,channel8& dest) const {
    splitImageToRGB splitter;
    channel r,g,b,o;
    splitter.apply(src,r,g,b);
    
    return apply(r,g,b,dest,o);
  }

  // On copy apply for type channel!
  bool cannyEdges::apply(const image& src,channel8& dest,channel& ori) const {
    splitImageToRGB splitter;
    channel r,g,b;
    splitter.apply(src,r,g,b);
    
    return apply(r,g,b,dest,ori);
  }

  // On copy apply for type channel!
  bool cannyEdges::apply(const image& src,
                         channel8& dest,
                         channel& ori,
                         channel& strength) const {
    splitImageToRGB splitter;
    channel r,g,b;
    splitter.apply(src,r,g,b);
    
    return apply(r,g,b,dest,ori,strength);
  }

  bool cannyEdges::apply(const channel& c1,
                         const channel& c2,
                         const channel& c3,
                         channel8& dest,
                         channel& orientation) const {
    channel strength;
    return apply(c1,c2,c3,dest,orientation,strength);
  }

  bool cannyEdges::apply(const channel& c1,
                         const channel& c2,
                         const channel& c3,
                         channel8& dest,
                         channel& orientation,
                         channel& gradAbs) const {

    const parameters& param = getParameters();

    if ((c1.size() != c2.size()) ||
        (c1.size() != c3.size())) {
      setStatusString("Input channels have different sizes");
      return false;
    }

    if (c1.empty()) {
      setStatusString("input image empty");
      return(false);
    }    

    dest.allocate(c1.size());

    // temporary images
    channel c1s,c2s,c3s; // low-pass filtered image (noise reduction)
    // channel gradAbs;       // magnitude of gradient
    const channel *c1p,*c2p,*c3p;
    c1p=c2p=c3p=0;

    gradAbs.allocate(c1.size());
    orientation.allocate(c1.size());

    _lti_debug("Gauss Filter");

    // apply gauss filter to smooth image, only if desired
    if (param.kernelSize > 0) {
      
      gaussKernel2D<float> kernel(param.kernelSize,param.variance);
      
      convolution::parameters filterParam;       // parameters
      filterParam.setKernel(kernel);             // use the gauss kernel
      filterParam.boundaryType = lti::Constant;
      convolution filter(filterParam);           // convolution operator
      
      // filter the channel 
      filter.apply(c1,c1s);
      filter.apply(c2,c2s);
      filter.apply(c3,c3s);
      c1p=&c1s;
      c2p=&c2s;
      c3p=&c3s;
    } else {
      c1p=&c1;
      c2p=&c2;
      c3p=&c3;
    }

    _lti_debug("Differentiation and result in polar coordinates");

    float maxVal;
    
    // compute color contrast gradient
    if (grad_.apply(*c1p,*c2p,*c3p,gradAbs,orientation,maxVal)) {

#     ifdef _LTI_DEBUG
      static viewer2D viewAbs("Abs Color"),viewArg("Arg");
      viewAbs.show(gradAbs);
      viewArg.show(orientation);
#     endif      

      // non-maxima suppression
      return suppressor_.apply(gradAbs,orientation,dest,maxVal);
    }   
    
    return false;
  }

}
