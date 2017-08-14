/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiComprehensiveColorNormalization.cpp
 *         Contains the functor comprehensiveColorNormalization.
 * \author Marius Wolf	
 * \date   19.03.2003
 *
 * $Id: ltiComprehensiveColorNormalization.cpp,v 1.4 2007-04-05 22:55:43 alvarado Exp $
 */


#include "ltiComprehensiveColorNormalization.h"
#include "ltiSplitImageToRGB.h"
#include "ltiMergeRGBToImage.h"


namespace lti {
  // --------------------------------------------------
  // comprehensiveColorNormalization::parameters
  // --------------------------------------------------

  // default constructor
  comprehensiveColorNormalization::parameters::parameters() 
    : colorNormalization::parameters() {      
    
    maxIterations=5;
    maxDistance=1.f;
  }

  // copy constructor
  comprehensiveColorNormalization::parameters
  ::parameters(const parameters& other)    
    : colorNormalization::parameters() {      
    copy(other);
  }
  
  // destructor
  comprehensiveColorNormalization::parameters::~parameters() {    
  }
  
  // copy member
  comprehensiveColorNormalization::parameters& 
  comprehensiveColorNormalization::parameters::copy(const parameters& other) {
    colorNormalization::parameters::copy(other);
    
    maxDistance = other.maxDistance;
    maxIterations = other.maxIterations;

    return *this;
  }

  // alias for copy member
  comprehensiveColorNormalization::parameters& 
  comprehensiveColorNormalization::parameters
  ::operator=(const parameters& other) {
    return copy(other);
  }

  const std::string& 
  comprehensiveColorNormalization::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  comprehensiveColorNormalization::parameters* 
  comprehensiveColorNormalization::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance
  comprehensiveColorNormalization::parameters* 
  comprehensiveColorNormalization::parameters::newInstance() const {
    return new parameters();
  }

  //write
  bool comprehensiveColorNormalization::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && colorNormalization::parameters::write(handler,false);

    if (b) {
      lti::write(handler,"maxDistance",maxDistance);
      lti::write(handler,"maxIterations",maxIterations);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  //read
  bool 
  comprehensiveColorNormalization::parameters::read(ioHandler& handler,
                                                    const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && colorNormalization::parameters::read(handler,false);

    if (b) {
      lti::read(handler,"maxDistance",maxDistance);
      lti::read(handler,"maxIterations",maxIterations);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // comprehensiveColorNormalization
  // --------------------------------------------------

  // default constructor
  comprehensiveColorNormalization::comprehensiveColorNormalization()
    : colorNormalization() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  comprehensiveColorNormalization
  ::comprehensiveColorNormalization(const parameters& par)
    : colorNormalization() {

    // set the given parameters
    setParameters(par);
  }

  // copy constructor
  comprehensiveColorNormalization::comprehensiveColorNormalization(const comprehensiveColorNormalization& other)
    : colorNormalization() {
    copy(other);
  }

  // destructor
  comprehensiveColorNormalization::~comprehensiveColorNormalization() {
  }

  // copy member
  comprehensiveColorNormalization&
  comprehensiveColorNormalization
  ::copy(const comprehensiveColorNormalization& other) {

    colorNormalization::copy(other);
    return (*this);
  }

  // alias for copy member
  comprehensiveColorNormalization&
  comprehensiveColorNormalization
  ::operator=(const comprehensiveColorNormalization& other) {
    return (copy(other));
  }

  const std::string& comprehensiveColorNormalization::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  comprehensiveColorNormalization* 
  comprehensiveColorNormalization::clone() const {
    return new comprehensiveColorNormalization(*this);
  }

  // newInstance
  comprehensiveColorNormalization* 
  comprehensiveColorNormalization::newInstance() const {
    return new comprehensiveColorNormalization();
  }

  // return parameters
  const comprehensiveColorNormalization::parameters&
  comprehensiveColorNormalization::getParameters() const {
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
  
  // On place apply for type image!
  bool comprehensiveColorNormalization::apply(image& srcdest) const {

    image tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On copy apply for type image!
  bool comprehensiveColorNormalization::apply(const image& src,
                                              image& dest) const {
    
    channel sr,sg,sb,dr,dg,db;
    splitImageToRGB splitter;
    mergeRGBToImage merger;

    splitter.apply(src,sr,sg,sb);

    const parameters& param = getParameters();          
    int steps=0;
    bool success = true;
    float dist;

    do {
      if (step(sr,sg,sb,dr,dg,db,dist)) {
        std::cerr << dist << "\n";
        if (dist<param.maxDistance) {
          // we're ready!  change was small enough!
          steps=param.maxIterations;  // flag to indicate end!
        } else {
          // prepare next iteration
          dr.detach(sr);
          dg.detach(sg);
          db.detach(sb);
        }
      } else {
        steps = param.maxIterations;
        success = false;
      }

      ++steps;
    } while (steps<param.maxIterations);

    if (success) {
      merger.apply(dr,dg,db,dest);
    }

    return success;
  }

  bool comprehensiveColorNormalization::step(const channel& srcred,
                                              const channel& srcgreen,
                                              const channel& srcblue,
                                              channel &destred,
                                              channel &destgreen,
                                              channel &destblue,
                                              float& dist) const {
    
    // resize the destination but do not initialize, this resize()
    // also ensures that dest is continous
    const int rows=srcred.rows();
    const int cols=srcred.columns();
    const int n = rows*cols;

    destred.allocate(rows,cols);
    destgreen.allocate(rows,cols);
    destblue.allocate(rows,cols);

    // --------------------------------------------------------------
    // first step, computation of the chromaticity normalization R(I)

    // access all channels with the vector access operator, assuming that
    // all channels are connected.
    int i;
    float sum,cr,cg,cb;
    // channel sums
    float sumr=0.f;
    float sumg=0.f;
    float sumb=0.f;

    for (i=0;i<n;++i) {
      sum = ((cr=srcred.elem(i))+
             (cg=srcgreen.elem(i))+
             (cb=srcblue.elem(i)));
      
      if (sum==0.0f) {
        sum=1.0f;
      }
      
      destred.elem(i)  = (cr/=sum);
      destgreen.elem(i)= (cg/=sum);
      destblue.elem(i) = (cb/=sum);
      
      sumr += cr;
      sumg += cg;
      sumb += cb;
    }
    
    // normalize the values from 0.0 to 1.0 instead of 0 to 255
    const float fac=3.f/n;
    sumr*=fac;
    sumg*=fac;
    sumb*=fac;

    // colSum is now 1/3 of the mean values at each channel. Invert it:
    if (sumr != 0.0f) {
      sumr=1.0f/sumr;
    }
    if (sumg != 0.0f) {
      sumg=1.0f/sumg;
    }
    if (sumb != 0.0f) {
      sumb=1.0f/sumb;
    }

    // --------------------------------------------------------------
    // now intensity normalization C(R(I))
    
    dist=0;
    for (i=0;i<n;++i) {
      cr=(destred.elem(i)  *= sumr) - srcred.elem(i);
      cg=(destgreen.elem(i)*= sumg) - srcgreen.elem(i);
      cb=(destblue.elem(i) *= sumb) - srcblue.elem(i);
      if ((sum=cr*cr+cg*cg+cb*cb)>dist) {
        dist=sum;
      }
    }

    return true;  
  }

}

