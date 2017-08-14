/*
 * Copyright (C) 2012
 * Carlos Gomez, Pablo Alvarado
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
 * \file   ltiOpticalFlowHornSchunck.cpp
 *         Contains the class lti::opticalFlowHornSchunck, 
 *         which implements the classic algorithm of Horn and Schunck for 
 *         global optical flow estimation.
 * 
 * \author Carlos Gomez
 * \author Pablo Alvarado
 * \date   28.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlowHornSchunck.cpp,v 1.6 2012-11-02 02:56:12 alvarado Exp $
 */

#include "ltiOpticalFlowHornSchunck.h"
//#include "ltiLaplacianKernel.h"
#include "ltiGaussKernels.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1

#ifdef _LTI_DEBUG
#  include "ltiDebug.h"
#  include "ltiViewer2D.h"
#  include "ltiOpticalFlowPainter.h"
#endif

#include "ltiDebug.h"


namespace lti {
  // --------------------------------------------------
  // opticalFlowHornSchunck::parameters
  // --------------------------------------------------

  // default constructor
  opticalFlowHornSchunck::parameters::parameters()
    : opticalFlow::parameters() {
    
    lambda = float(0.4);
    maxIterations = int(20);
    kernelSize = int(5);
    maxError = float(0.01);
    variance = float(1.3);
    gradient.format = lti::gradientFunctor::Cartesian;
    gradient.kernelType = lti::gradientFunctor::Ando;
  }

  // copy constructor
  opticalFlowHornSchunck::parameters::parameters(const parameters& other)
    : opticalFlow::parameters() {
    copy(other);
  }

  // destructor
  opticalFlowHornSchunck::parameters::~parameters() {
  }

  // copy member

  opticalFlowHornSchunck::parameters&
  opticalFlowHornSchunck::parameters::copy(const parameters& other) {
    opticalFlow::parameters::copy(other);

    lambda = other.lambda;
    maxIterations = other.maxIterations;
    kernelSize = other.kernelSize;
    maxError = other.maxError;
    variance = other.variance;
    gradient   = other.gradient;

    return *this;
  }

  // alias for copy method
  opticalFlowHornSchunck::parameters&
  opticalFlowHornSchunck::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& opticalFlowHornSchunck::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  opticalFlowHornSchunck::parameters*
  opticalFlowHornSchunck::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  opticalFlowHornSchunck::parameters*
  opticalFlowHornSchunck::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool opticalFlowHornSchunck::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"lambda",lambda);
      lti::write(handler,"maxIterations",maxIterations);
      lti::write(handler,"kernelSize",kernelSize);
      lti::write(handler,"maxError",maxError);
      lti::write(handler,"variance",variance);
      lti::write(handler,"gradient",gradient);
    }

    b = b && opticalFlow::parameters::write(handler,false);

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
  bool opticalFlowHornSchunck::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"lambda",lambda);
      lti::read(handler,"maxIterations",maxIterations);
      lti::read(handler,"kernelSize",kernelSize);
      lti::read(handler,"maxError",maxError);
      lti::read(handler,"variance",variance);
      lti::read(handler,"gradient",gradient);
    }

    b = b && opticalFlow::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // opticalFlowHornSchunck
  // --------------------------------------------------

  // default constructor
  opticalFlowHornSchunck::opticalFlowHornSchunck()
    : opticalFlow() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  opticalFlowHornSchunck::opticalFlowHornSchunck(const parameters& par)
    : opticalFlow() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  opticalFlowHornSchunck::
  opticalFlowHornSchunck(const opticalFlowHornSchunck& other)
    : opticalFlow() {
    copy(other);
  }

  // destructor
  opticalFlowHornSchunck::~opticalFlowHornSchunck() {
  }

  // copy member
  opticalFlowHornSchunck& 
  opticalFlowHornSchunck::copy(const opticalFlowHornSchunck& other) {
    opticalFlow::copy(other);

    return (*this);
  }

  // alias for copy member
  opticalFlowHornSchunck&
  opticalFlowHornSchunck::operator=(const opticalFlowHornSchunck& other) {
    return (copy(other));
  }

  // class name
  const std::string& opticalFlowHornSchunck::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  opticalFlowHornSchunck* opticalFlowHornSchunck::clone() const {
    return new opticalFlowHornSchunck(*this);
  }

  // create a new instance
  opticalFlowHornSchunck* opticalFlowHornSchunck::newInstance() const {
    return new opticalFlowHornSchunck();
  }

  // return parameters
  const opticalFlowHornSchunck::parameters&
  opticalFlowHornSchunck::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&opticalFlow::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }
  bool opticalFlowHornSchunck::updateParameters() {
    // get the set of parameters just indicated by the user
     const parameters& par = getParameters();
     
     // do whatever you need to initialize with par
     bool success=true;
    
     // ensure the use of cartesian coordinates
     gradientFunctor::parameters gradPar(par.gradient);
     gradPar.format = gradientFunctor::Cartesian;

     grad_.setParameters(gradPar);
     
     // return true only if everything was ok.
     return success;
  }
  // -------------------------------------------------------------------
  // The apply() member functions are all inherited from opticalFlow
  // -------------------------------------------------------------------
  bool opticalFlowHornSchunck::apply(channel& chnl1OrU, channel& chnl2OrV) {
    return opticalFlow::apply(chnl1OrU,chnl2OrV);
  }

  bool opticalFlowHornSchunck::apply(const channel& chnl1, 
                                     const channel& chnl2,
                                     channel& u,
                                     channel& v) {
    return opticalFlow::apply(chnl1,chnl2,u,v);
  }


  bool opticalFlowHornSchunck::apply(const channel& next,
                                           channel& u,channel& v) {
    if (next.size() != last_.size()) {
      u.clear();
      v.clear();
      last_.copy(next);
      setStatusString("Input images have different sizes");
      return false;
    }

    const parameters& par = getParameters();
    const int rows=next.rows();
    const int cols=next.columns();
    const float lambda=par.lambda;
    
    u.allocate(rows,cols);
    v.allocate(rows,cols);
    
    // computation of space gradient and time derivative
    channel ix,iy,it;
    grad_.apply(next,ix,iy);
    it.subtract(last_,next);
    
    for(int i=0;i<rows;++i){
      // get each row to avoid unnecessary repeated row accesses
      const vector<float>& ixrow = ix.getRow(i);
      const vector<float>& iyrow = iy.getRow(i);
      const vector<float>& itrow = it.getRow(i);
      const vector<float>& u_row = u_.getRow(i);
      const vector<float>& v_row = v_.getRow(i);
			
      vector<float>& urow = u.getRow(i);
      vector<float>& vrow = v.getRow(i);
      for (int j=0;j<cols;++j){
        // avoid expensive double accesses 
        const float fix = ixrow.at(j);
        const float fiy = iyrow.at(j);
        const float fit = itrow.at(j);
        const float fu_ = u_row.at(j);
        const float fv_ = v_row.at(j);
        const float a =(fix*fu_+fiy*fv_+fit)/(lambda+fix*fix+fiy*fiy);
        urow.at(j)=fu_+fix*a;
        vrow.at(j)=fv_+fiy*a;
      }
    }
    //compute the velocity averages
    gaussKernel2D<channel::value_type> gaussKernel(par.kernelSize,par.variance);
    convolution filter(gaussKernel);
    
    filter.apply(u,u_);
    filter.apply(v,v_);
    last_.copy(next);
    
    return true;
  }

  bool opticalFlowHornSchunck::apply(const channel& last,
                                     const channel& next,
                                     const list<location>& locs1,
                                     channel& u,
                                     channel& v,
                                     std::vector<fpoint>& pts1f,
                                     std::vector<fpoint>& pts2f) {
    return opticalFlow::apply(last,next,locs1,u,v,pts1f,pts2f);
  }

  bool opticalFlowHornSchunck::apply(const channel& last,
                                     const channel& next,
                                     const list<location>& locs1,
                                     channel& u,
                                     channel& v) {
    return computeFlow(last,next,locs1,u,v);
  }


  bool opticalFlowHornSchunck::computeFlow(const channel& last,
                                           const channel& next,
                                           channel& u,
                                           channel& v) {
    const parameters& par = getParameters();
    const int rows=next.rows();
    const int cols=next.columns();
    const int maxIt=par.maxIterations;
    const float maxE=par.maxError;
    const float lambda=par.lambda;
    float error=maxE;
    float eu,ev;
    int k=0;

#if defined(_LTI_DEBUG)	
    static matrix<float> canvas;
    static matrix<float> canvas2;
    static viewer2D view("IntegralImage::debug");
    static viewer2D view2("IntegralImage2::debug");
#endif

    // computation of space gradient and time derivative
    channel ix,iy,it;
    grad_.apply(next,ix,iy);
    it.subtract(last,next);
    
    // creation of filter to compute the velocity average 
    gaussKernel2D<channel::value_type> gaussKernel(par.kernelSize,par.variance);
    convolution filter(gaussKernel);
    
    u.assign(rows,cols,0);
    v.assign(rows,cols,0);
    
    while (error>=maxE && k<maxIt) {
      error=0;
      ++k;
      filter.apply(u,u_);
      filter.apply(v,v_);

      _lti_if_debug(canvas.copy(u_));
      _lti_if_debug(view.show(canvas));

      for(int i=0;i<rows;++i){
        // get each row to avoid unnecessary repeated row accesses
        const vector<float>& ixrow = ix.getRow(i);
        const vector<float>& iyrow = iy.getRow(i);
        const vector<float>& itrow = it.getRow(i);
        const vector<float>& u_row = u_.getRow(i);
        const vector<float>& v_row = v_.getRow(i);
        
        vector<float>& urow = u.getRow(i);
        vector<float>& vrow = v.getRow(i);
        
        for (int j=0;j<cols;++j){
          // avoid expensive double accesses 
          const float fix = ixrow.at(j);
          const float fiy = iyrow.at(j);
          const float fit = itrow.at(j);
          const float fu_ = u_row.at(j);
          const float fv_ = v_row.at(j);
          const float fuant = urow.at(j);
          const float fvant = vrow.at(j);
          const float a =(fix*fu_+fiy*fv_+fit)/(lambda+fix*fix+fiy*fiy);
          urow.at(j)=fu_+fix*a;
          vrow.at(j)=fv_+fiy*a;
          const float fu = urow.at(j);
          const float fv = vrow.at(j);
          if ( (abs(fu)>std::numeric_limits<float>::epsilon()) && 
               (abs(fv)>std::numeric_limits<float>::epsilon()) ) {
            eu=(fu-fuant)/fu;
            ev=(fv-fvant)/fv;
            error=max(eu,ev,error);
          }

          _lti_showVar(error);
          _lti_showVar(eu);
          _lti_showVar(ev);
          _lti_showVar(fu);
          _lti_showVar(fv);
          _lti_showVar(fuant);
          _lti_showVar(fvant);
          _lti_showVar(fu_);
          _lti_showVar(fv_);
        }
      } 
    }

    return true;
  }

  bool opticalFlowHornSchunck::computeFlow(const channel& last,
                                           const channel& next,
                                           const list<location>& ,
                                           channel& u,
                                           channel& v) {
    // there is no real optimization of this method, so just call the one
    // without the locations
    return computeFlow(last,next,u,v);
  }

  bool opticalFlowHornSchunck::computeFlow(const channel& last,
                                           const channel& next,
                                           const list<location>& locs1,
                                           channel& u,
                                           channel& v,
                                           std::vector<fpoint>& pts1f,
                                           std::vector<fpoint>& pts2f) {

    if ( computeFlow(last,next,locs1,u,v) ) {

      list<location>::const_iterator itl;
      const list<location>::const_iterator eit=locs1.end();

      // ensure the proper size for the destination points
      const unsigned int numPts = locs1.size();
      pts1f.resize(numPts);
      pts2f.resize(numPts);
      
      int l;
      for (l=0,itl=locs1.begin();itl!=eit;++itl,l++) {
        const location& loc = (*itl);
        
        const int j=iround(loc.position.x);
        const int i=iround(loc.position.y);
        pts1f[l].set(static_cast<float>(i),static_cast<float>(j));
        pts2f[l].set(static_cast<float>(i+iround(u.at(i,j))),static_cast<float>(j+iround(v.at(i,j))));
      }

      return true;
    }

    pts1f.clear();
    pts2f.clear();

    return false;
    
  }

}

