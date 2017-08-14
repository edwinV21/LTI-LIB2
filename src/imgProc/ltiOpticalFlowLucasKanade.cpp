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
 * \file   ltiOpticalFlowLucasKanade.cpp
 *         Contains the class opticalFlowLucasKanade,
 *         which has still to be documented by Carlos Gomez or Pablo Alvarado.
 * 
 * \author Carlos Gomez
 * \author Pablo Alvarado
 * \date   28.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlowLucasKanade.cpp,v 1.6 2012-11-02 02:32:48 alvarado Exp $
 */

#include "ltiOpticalFlowLucasKanade.h"
#include "ltiGaussKernels.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // opticalFlowLucasKanade::parameters
  // --------------------------------------------------

  // default constructor
  opticalFlowLucasKanade::parameters::parameters()
    : opticalFlow::parameters() {
    variance = 1.0e10f;
    kernelSize = int(5);
    gradient.format = lti::gradientFunctor::Cartesian;
    gradient.kernelType = lti::gradientFunctor::Ando;
  }

  // copy constructor
  opticalFlowLucasKanade::parameters::parameters(const parameters& other)
    : opticalFlow::parameters() {
    copy(other);
  }

  // destructor
  opticalFlowLucasKanade::parameters::~parameters() {
  }

  // copy member

  opticalFlowLucasKanade::parameters&
  opticalFlowLucasKanade::parameters::copy(const parameters& other) {
    opticalFlow::parameters::copy(other);

    
    variance   = other.variance;
    kernelSize = other.kernelSize;
    gradient   = other.gradient;

    return *this;
  }

  // alias for copy method
  opticalFlowLucasKanade::parameters&
  opticalFlowLucasKanade::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& opticalFlowLucasKanade::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  opticalFlowLucasKanade::parameters*
  opticalFlowLucasKanade::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  opticalFlowLucasKanade::parameters*
  opticalFlowLucasKanade::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool opticalFlowLucasKanade::parameters::write(ioHandler& handler,
                                                 const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"variance",variance);
      lti::write(handler,"kernelSize",kernelSize);
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
  bool opticalFlowLucasKanade::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"variance",variance);
      lti::read(handler,"kernelSize",kernelSize);
      lti::read(handler,"gradient",gradient);
    }

    b = b && opticalFlow::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // opticalFlowLucasKanade
  // --------------------------------------------------

  // default constructor
  opticalFlowLucasKanade::opticalFlowLucasKanade()
    : opticalFlow() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  opticalFlowLucasKanade::opticalFlowLucasKanade(const parameters& par)
    : opticalFlow() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  opticalFlowLucasKanade::opticalFlowLucasKanade(const opticalFlowLucasKanade& other)
    : opticalFlow() {
    copy(other);
  }

  // destructor
  opticalFlowLucasKanade::~opticalFlowLucasKanade() {
  }

  // copy member
  opticalFlowLucasKanade& 
  opticalFlowLucasKanade::copy(const opticalFlowLucasKanade& other) {
    opticalFlow::copy(other);

    return (*this);
  }

  // alias for copy member
  opticalFlowLucasKanade&
  opticalFlowLucasKanade::operator=(const opticalFlowLucasKanade& other) {
    return (copy(other));
  }

  // class name
  const std::string& opticalFlowLucasKanade::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  opticalFlowLucasKanade* opticalFlowLucasKanade::clone() const {
    return new opticalFlowLucasKanade(*this);
  }

  // create a new instance
  opticalFlowLucasKanade* opticalFlowLucasKanade::newInstance() const {
    return new opticalFlowLucasKanade();
  }

  // return parameters
  const opticalFlowLucasKanade::parameters&
  opticalFlowLucasKanade::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&opticalFlow::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool opticalFlowLucasKanade::updateParameters() {
    // get the set of parameters just indicated by the user
     const parameters& par = getParameters();
     
     // do whatever you need to initialize with par
     bool success=true;
    
     // ensure the use of cartesian coordinates
     gradientFunctor::parameters gradPar(par.gradient);
     gradPar.format = gradientFunctor::Cartesian;

     grad_.setParameters(gradPar);

     // create the desired filter
     gaussKernel2D<float> average(par.kernelSize,par.variance);
     float norm = float(par.kernelSize)/
       average.getRowFilter(0).computeSumOfElements();
     average.getRowFilter(0).multiply(norm);
     norm = float(par.kernelSize)/
       average.getColFilter(0).computeSumOfElements();
     average.getColFilter(0).multiply(norm);

     convolution::parameters convPar;
     convPar.setKernel(average);
     convPar.boundaryType = lti::Constant;
     success = filter_.setParameters(convPar);

     // return true only if everything was ok.
     return success;
  }

  // -------------------------------------------------------------------
  // The apply() member functions are all inherited
  // -------------------------------------------------------------------

  bool opticalFlowLucasKanade::computeFlow(const channel& last,
                                           const channel& next,
                                           const list<location>& locs1,
                                           channel& u,
                                           channel& v)  {
    
    std::vector<fpoint> pts1f;
    std::vector<fpoint> pts2f;

    return computeFlow(last,next,locs1,u,v,pts1f,pts2f);
  }  

  bool opticalFlowLucasKanade::computeFlow(const channel& last,
                                           const channel& next,
                                           const list<location>& locs1,
                                           channel& u,
                                           channel& v,
                                           std::vector<fpoint>& pts1f,
                                           std::vector<fpoint>& pts2f)  {
    
    const int rows=next.rows();
    const int cols=next.columns();
    
    // computation of space gradient and time derivative
    channel ix,iy,it;
    grad_.apply(next,ix,iy);
    it.subtract(last,next);
    
    // prepare products for least mean squares
    Iyy_.allocate(rows,cols);
    Ixx_.allocate(rows,cols);
    Ixy_.allocate(rows,cols);
    Ixt_.allocate(rows,cols);
    Iyt_.allocate(rows,cols);
    
    for(int i=0;i<rows;++i) {
      
      // get each row to avoid unnecessary repeated row accesses
      const vector<float>& ixrow = ix.getRow(i);
      const vector<float>& iyrow = iy.getRow(i);
      const vector<float>& itrow = it.getRow(i);
      
      vector<float>& ixxrow = Ixx_.getRow(i);
      vector<float>& ixyrow = Ixy_.getRow(i);
      vector<float>& iyyrow = Iyy_.getRow(i);
      vector<float>& ixtrow = Ixt_.getRow(i);
      vector<float>& iytrow = Iyt_.getRow(i);
      
      for (int j=0;j<cols;++j){
        
        // avoid expensive double accesses 
        const float fix = ixrow.at(j);
        const float fiy = iyrow.at(j);
        const float fit = itrow.at(j);
        
        ixxrow.at(j)=fix*fix;
        ixyrow.at(j)=fix*fiy;
        iyyrow.at(j)=fiy*fiy;
        ixtrow.at(j)=fix*fit;
        iytrow.at(j)=fiy*fit;
      }
    }
    
    filter_.apply(Ixx_);
    filter_.apply(Ixy_);
    filter_.apply(Iyy_);
    filter_.apply(Ixt_);
    filter_.apply(Iyt_);
    
    u.allocate(rows,cols);
    v.allocate(rows,cols);
    
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

      const float xx = Ixx_.at(i,j);
      const float yy = Iyy_.at(i,j);
      const float xy = Ixy_.at(i,j);
      const float det= xx*yy-xy*xy;
      
      // avoid invalid computations
      if (abs(det)<std::numeric_limits<float>::epsilon()) {
        u.at(i,j)=v.at(i,j)=0.0f;
      } else {
        const float xt = Ixt_.at(i,j);
        const float yt = Iyt_.at(i,j);
        
        u.at(i,j)=(yy*xt-xy*yt)/det;
        v.at(i,j)=(xx*yt-xy*xt)/det;
      }
      pts2f[l].set(static_cast<float>(i+iround(u.at(i,j))),static_cast<float>(j+iround(v.at(i,j))));
    }
    
    return true;
  }
  
  bool opticalFlowLucasKanade::computeFlow(const channel& last,
                                           const channel& next,
                                           channel& u,
                                           channel& v)  {

    const int rows=next.rows();
    const int cols=next.columns();

    // computation of space gradient and time derivative
    channel ix,iy,it;
    grad_.apply(next,ix,iy);
    it.subtract(last,next);

    // prepare products for least mean squares
    Iyy_.allocate(rows,cols);
    Ixx_.allocate(rows,cols);
    Ixy_.allocate(rows,cols);
    Ixt_.allocate(rows,cols);
    Iyt_.allocate(rows,cols);

    for(int i=0;i<rows;++i){

      // get each row to avoid unnecessary repeated row accesses
      const vector<float>& ixrow = ix.getRow(i);
      const vector<float>& iyrow = iy.getRow(i);
      const vector<float>& itrow = it.getRow(i);

      vector<float>& ixxrow = Ixx_.getRow(i);
      vector<float>& ixyrow = Ixy_.getRow(i);
      vector<float>& iyyrow = Iyy_.getRow(i);
      vector<float>& ixtrow = Ixt_.getRow(i);
      vector<float>& iytrow = Iyt_.getRow(i);

      for (int j=0;j<cols;++j){

        // avoid expensive double accesses 
        const float fix = ixrow.at(j);
        const float fiy = iyrow.at(j);
        const float fit = itrow.at(j);

        ixxrow.at(j)=fix*fix;
        ixyrow.at(j)=fix*fiy;
        iyyrow.at(j)=fiy*fiy;
        ixtrow.at(j)=fix*fit;
        iytrow.at(j)=fiy*fit;
      }
    }

    filter_.apply(Ixx_);
    filter_.apply(Ixy_);
    filter_.apply(Iyy_);
    filter_.apply(Ixt_);
    filter_.apply(Iyt_);

    u.allocate(rows,cols);
    v.allocate(rows,cols);
    
    for(int i=0;i<rows;i++) {
      // avoid unnecessary repeated row accesses
      const vector<float>& xxrow = Ixx_.getRow(i);
      const vector<float>& xyrow = Ixy_.getRow(i);
      const vector<float>& yyrow = Iyy_.getRow(i);
      const vector<float>& xtrow = Ixt_.getRow(i);
      const vector<float>& ytrow = Iyt_.getRow(i);

      vector<float>& urow = u.getRow(i);
      vector<float>& vrow = v.getRow(i);

      for (int j=0;j<cols;j++){
        // avoid double access
        const float xx = xxrow.at(j);
        const float yy = yyrow.at(j);
        const float xy = xyrow.at(j);

        const float det=xx*yy-xy*xy;

        // avoid invalid computations
        if (abs(det)<std::numeric_limits<float>::epsilon()) {
          urow.at(j)=vrow.at(j)=0.0f;
        } else {
          const float xt = xtrow.at(j);
          const float yt = ytrow.at(j);

          urow.at(j)=(yy*xt-xy*yt)/det;
          vrow.at(j)=(xx*yt-xy*xt)/det;
        }
      }
    }

    return true;
  }

}

