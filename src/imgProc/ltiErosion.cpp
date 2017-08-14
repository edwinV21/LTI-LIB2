/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
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
 * \file   ltiErosion.cpp
 *         Contains the class lti::erosion,
 *         which implements the morphological operator known as erosion.
 * \author Pablo Alvarado
 * \date   19.07.2000
 *
 * revisions ..: $Id: ltiErosion.cpp,v 1.2 2010-11-19 13:37:54 alvarado Exp $
 */

#include "ltiErosion.h"
#include "ltiErosion_template.h"

#include "ltiConvolutionHelper.h"

namespace lti {

  // --------------------------------------------------
  // erosion
  // --------------------------------------------------

  // default constructor
  erosion::erosion() : morphology() {
    // the parent class sets the parameters
  }

  // default constructor
  erosion::erosion(const container& se) : morphology(se) {
  }

  // default constructor
  erosion::erosion(const parameters& par) : morphology(par) {
  }

  // copy constructor
  erosion::erosion(const erosion& other)
    : morphology()  {
    copy(other);
  }

  // destructor
  erosion::~erosion() {
  }

  // returns the name of this type
  const std::string& erosion::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  erosion& erosion::copy(const erosion& other) {
    morphology::copy(other);
    return (*this);
  }

  // clone member
  erosion* erosion::clone() const {
    return new erosion(*this);
  }

  erosion* erosion::newInstance() const {
    return new erosion;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type channel!
  bool erosion::apply(fmatrix& srcdest) const {
    fmatrix tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type matrix<ubyte>!
  bool erosion::apply(matrix<ubyte>& srcdest) const {
    matrix<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type fvector!
  bool erosion::apply(fvector& srcdest) const {

    fvector tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type vector<ubyte>!
  bool erosion::apply(vector<ubyte>& srcdest) const {

    vector<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On copy apply for type fmatrix!
  bool erosion::apply(const fmatrix& src,fmatrix& dest) const {

    const parameters& param = getParameters();

    switch(param.mode) {
    case Binary: {
      convHelper2D<float,accumulatorBin<float,float> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      if (conv.isKernelSeparable()) {
        conv.applySep(src,dest,param.boundaryType);
      } else {
        conv.apply(src,dest,param.boundaryType);
      }
    } break;
    case GrayNonFlat: {
      convHelper2D<float, accumulatorGrayNonFlat<float,float> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      if (conv.isKernelSeparable()) {
        conv.applySep(src,dest,param.boundaryType);
      } else {
        conv.apply(src,dest,param.boundaryType);
      }
    } break;
    case GrayFlat: {
      convHelper2D<float, accumulatorGrayFlat<float,float> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      if (conv.isKernelSeparable()) {
        conv.applySep(src,dest,param.boundaryType);
      } else {
        conv.apply(src,dest,param.boundaryType);
      }
    } break;
    default:
      setStatusString("Unknown morphology mode");
      return false;
    }
    
    return true;
  }

  // On copy apply for type matrix<ubyte>!
  bool erosion::apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const {

    // there are two possibilities: kernel2D or sepKernel
    const parameters& param = getParameters();

    switch(param.mode) {
    case Binary: {
      convHelper2D<ubyte,accumulatorBin<ubyte,int> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      if (conv.isKernelSeparable()) {
        conv.applySep(src,dest,param.boundaryType);
      } else {
        conv.apply(src,dest,param.boundaryType);
      }
    } break;
    case GrayNonFlat: {
      convHelper2D<ubyte, accumulatorGrayNonFlat<ubyte,int> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      if (conv.isKernelSeparable()) {
        conv.applySep(src,dest,param.boundaryType);
      } else {
        conv.apply(src,dest,param.boundaryType);
      }
    } break;
    case GrayFlat: {
      convHelper2D<ubyte, accumulatorGrayFlat<ubyte,int> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      if (conv.isKernelSeparable()) {
        conv.applySep(src,dest,param.boundaryType);
      } else {
        conv.apply(src,dest,param.boundaryType);
      }
    } break;
    default:
      setStatusString("Unknown morphology mode");
      return false;
    }

    return true;
  }

  // On copy apply for type fvector!
  bool erosion::apply(const fvector& src,fvector& dest) const {

    const parameters& param = getParameters();

    switch(param.mode) {
    case Binary: {
      convHelper1D<float,accumulatorBin<float,float> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      conv.apply(src,dest,param.boundaryType);
    } break;
    case GrayNonFlat: {
      convHelper1D<float, accumulatorGrayNonFlat<float,float> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      conv.apply(src,dest,param.boundaryType);
    } break;
    case GrayFlat: {
      convHelper1D<float, accumulatorGrayFlat<float,float> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      conv.apply(src,dest,param.boundaryType);
    }
    default:
      setStatusString("Unknown morphology mode");
      return false;
    }
    
    return true;
  }

  // On copy apply for type vector<ubyte>!
  bool erosion::apply(const vector<ubyte>& src,
                      vector<ubyte>& dest) const {


    const parameters& param = getParameters();

    switch(param.mode) {
    case Binary: {
      convHelper1D<ubyte,accumulatorBin<ubyte,int> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      conv.apply(src,dest,param.boundaryType);
    } break;
    case GrayNonFlat: {
      convHelper1D<ubyte, accumulatorGrayNonFlat<ubyte,int> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      conv.apply(src,dest,param.boundaryType);
    } break;
    case GrayFlat: {
      convHelper1D<ubyte, accumulatorGrayFlat<ubyte,int> > conv;
      if (!conv.setKernel(&param.getStructuringElement(),true)) {
        setStatusString("Wrong kernel type");
        return false;
      }
      
      conv.apply(src,dest,param.boundaryType);
    }
    default:
      setStatusString("Unknown morphology mode");
      return false;
    }
    
    return true;
  }

}
