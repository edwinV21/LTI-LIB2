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
 *         Contains the class opticalFlow,
 *         which has still to be documented by Carlos Gomez or Pablo Alvarado.
 * 
 * \author Carlos Gomez
 * \author Pablo Alvarado
 * \date   28.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlow.cpp,v 1.2 2012-11-02 02:32:48 alvarado Exp $
 */

#include "ltiOpticalFlowLucasKanade.h"
#include "ltiGaussKernels.h"
#include <limits>

namespace lti {

  // --------------------------------------------------
  // opticalFlow
  // --------------------------------------------------

  // default constructor
  opticalFlow::opticalFlow()
    : functor() {
  }

  // destructor
  opticalFlow::~opticalFlow() {
  }

  // copy member
  opticalFlow& 
  opticalFlow::copy(const opticalFlow& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  opticalFlow&
  opticalFlow::operator=(const opticalFlow& other) {
    return (copy(other));
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // On place apply for type channel!
  bool opticalFlow::apply(channel& chnl1OrU, channel& chnl2OrV) {
    channel u;
    channel v;
    if(apply(chnl1OrU, chnl2OrV, u, v)){
        u.detach(chnl1OrU);
        v.detach(chnl2OrV);
        return true;
    }
    return false;
  }

  // On copy apply for type channel!
  bool opticalFlow::apply(const channel& chnl1, 
                          const channel& chnl2,
                          channel& u,
                          channel& v) {

    if (chnl1.size() != chnl2.size()) {
      u.clear();
      v.clear();
      last_.clear();
      setStatusString("Input images have different sizes");
      return false;
    }
    
    bool ok = computeFlow(chnl1,chnl2,u,v);
    last_.copy(chnl2);
    return ok;
  }

  bool opticalFlow::apply(const channel& next,
                          channel& u,channel& v) {



    if (next.size() != last_.size()) {
      u.clear();
      v.clear();
      last_.copy(next);
      setStatusString("Input images have different sizes");
      return false;
    }
    
    bool ok = computeFlow(last_,next,u,v);
    last_.copy(next);
    return ok;
  }

  bool opticalFlow::apply(const channel& last,
                          const channel& next,
                          const list<location>& locs1,
                          channel& u,
                          channel& v)  {
    
    if (next.size() != last.size()) {
      u.clear();
      v.clear();
      last_.copy(next);
      setStatusString("Input images have different sizes");
      return false;
    }

    bool ok = computeFlow(last,next,locs1,u,v);
    last_.copy(next);
    return ok;

  }

  bool opticalFlow::apply(const channel& last,
                          const channel& next,
                          const list<location>& locs1,
                          channel& u,
                          channel& v,
                          std::vector<fpoint>& pts1f,
                          std::vector<fpoint>& pts2f) {
    
    if (next.size() != last.size()) {
      u.clear();
      v.clear();
      last_.copy(next);
      setStatusString("Input images have different sizes");
      return false;
    }

    bool ok = computeFlow(last,next,locs1,u,v,pts1f,pts2f);
    last_.copy(next);
    return ok;
  }
  

}

