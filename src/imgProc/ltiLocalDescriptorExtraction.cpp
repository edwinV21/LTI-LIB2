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
 * \file   ltiLocalDescriptorExtraction.cpp
 *         Implementation of lti::localDescriptorExtraction which is
 *         the base class of all functors that extract a local
 *         description (region or location) from an image.
 * \author Arnd Hannemann
 * \date   05.04.2004
 *
 * $Id: ltiLocalDescriptorExtraction.cpp,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#include "ltiLocalDescriptorExtraction.h"
#include "ltiSplitImageToRGB.h"
#include "ltiLocalSampling.h"
#include "ltiLocalSampling_template.h"
#include "ltiFactory.h"
 
#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // localDescriptorExtraction::parameters
  // --------------------------------------------------

  // default constructor
  localDescriptorExtraction::parameters::parameters()
    : descriptorExtraction::parameters() {
    borderWidth = 0;
    sampling_ = 0;
  }

  // copy constructor
  localDescriptorExtraction::parameters::parameters(const parameters& other)
    : descriptorExtraction::parameters() {

    sampling_ = 0;
    copy(other);
  }
  
  // destructor
  localDescriptorExtraction::parameters::~parameters() {
    delete sampling_;
  }

  // copy member
  localDescriptorExtraction::parameters&
    localDescriptorExtraction::parameters::copy(const parameters& other) {
    descriptorExtraction::parameters::copy(other);

    // borderwidth
    borderWidth = other.borderWidth;
    
    // delete old sampling
    delete sampling_;
    
    // clone other sampling (if not null)
    if (other.sampling_ != 0) {
      sampling_ = other.sampling_->clone();
    }
    return *this;
  }

  // alias for copy member
  localDescriptorExtraction::parameters&
    localDescriptorExtraction::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& localDescriptorExtraction::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  localDescriptorExtraction::parameters*
  localDescriptorExtraction::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  localDescriptorExtraction::parameters*
  localDescriptorExtraction::parameters::newInstance() const {
    return new parameters();
  }

  // setSampling
  bool localDescriptorExtraction::parameters::
  setSampling(const std::string& sampl) {
    std::string tmp;
    tmp = sampl.substr(0,5);
    //    _lti_debug("tmp:"<<tmp<<"\n");

    // automatically add lti:: if needed
    if (tmp=="lti::") {
      samplingName_ = sampl;
    } else {
      samplingName_ = "lti::"+sampl;
    }
    // delete old sampling
    if (sampling_ != 0) {
      delete sampling_;
    }

    // create new one
    factory<localSampling>& f = factory<localSampling>::getFactory();
    _lti_debug("Trying to create instance of '"<<samplingName_<<"' ...\n"); 
    sampling_ = f.newInstance(samplingName_); 
    if (sampling_==0) {
      _lti_debug(" failed.\n");
      return false;
    }
    _lti_debug(" succeeded.\n");
    return true;
  }
  
  // setSampling shortcut
  bool localDescriptorExtraction::parameters::
  setSampling(const std::string& sampl, 
              const localSampling::parameters& localPar) {
    bool rc = true;
    rc = rc && setSampling(sampl);
    rc = rc && setSamplingParameters(localPar);
    return rc;
  }

  // setSamplingParameters
  bool localDescriptorExtraction::parameters::
  setSamplingParameters(const localSampling::parameters& localPar) {
    bool rc = true;
    rc = (sampling_ != 0);
    rc = rc && sampling_->setParameters(localPar);
    return rc;
  } 

  // getSampling()
  localSampling* localDescriptorExtraction::parameters::getSampling() const
  {
    return sampling_;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool localDescriptorExtraction::parameters::write(ioHandler& handler,
                                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      if (sampling_ != 0) {
        lti::write(handler,"borderWidth",borderWidth);
        lti::write(handler,"sampling",samplingName_);
        lti::write(handler,"samplingParameters",sampling_->getParameters());
      }            
    }

    b = b && descriptorExtraction::parameters::write(handler,false);

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
   * @return true if read was successful
   */
  bool localDescriptorExtraction::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      delete sampling_;
      lti::read(handler,"borderWidth",borderWidth);
      lti::read(handler,"sampling",samplingName_);

      factory<localSampling>& f = factory<localSampling>::getFactory();
      sampling_ = f.newInstance(samplingName_);       
      if (sampling_ == 0) b = false;
      
      if (b) {
        // trick to get the right type
        localSampling::parameters *par = sampling_->getParameters().clone();
        lti::read(handler,"samplingParameters",*par);
        sampling_->setParameters(*par);
        delete par;
      }
      
    }

    b = b && descriptorExtraction::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // localDescriptorExtraction
  // --------------------------------------------------

  // default constructor
  localDescriptorExtraction::localDescriptorExtraction()
    : descriptorExtraction() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  localDescriptorExtraction::localDescriptorExtraction(const parameters& par)
    : descriptorExtraction() {
    // set the given parameters
    setParameters(par);
  }

  // copy constructor
  localDescriptorExtraction::
  localDescriptorExtraction(const localDescriptorExtraction& other)
    : descriptorExtraction() {
    copy(other);
  }

  // destructor
  localDescriptorExtraction::~localDescriptorExtraction() {

  }

  // copy member
  localDescriptorExtraction&
  localDescriptorExtraction::copy(const localDescriptorExtraction& other) {
    descriptorExtraction::copy(other);

    return (*this);
  }

  // alias for copy member
  localDescriptorExtraction&
  localDescriptorExtraction::operator=(const localDescriptorExtraction& other) {
    return (copy(other));
  }

  // class name
  const std::string& localDescriptorExtraction::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  localDescriptorExtraction* localDescriptorExtraction::clone() const {
    return new localDescriptorExtraction(*this);
  }

  // new instance
  localDescriptorExtraction* localDescriptorExtraction::newInstance() const {
    return new localDescriptorExtraction();
  }

  // return parameters
  const localDescriptorExtraction::parameters&
    localDescriptorExtraction::getParameters() const {
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


  bool localDescriptorExtraction::apply(const matrix<rgbaPixel>& src,
                                       const list<location>& locs,
                                       std::list<dvector>& dest) const {
    dest.clear();
    if (locs.empty()) {
      return true;
    }

    bool rc = true;
    channel red,green,blue;
    static splitImageToRGB splitter;
    splitter.apply(src,red,green,blue);
    list<location>::const_iterator it=locs.begin();
    const list<location>::const_iterator ite=locs.end();
    dvector tmp;
    dvector dest2;

    // use first location to determine descriptor size
    const location& firstLoc=*it;
    rc = apply(red, firstLoc, dest2);
    const int szone=dest2.size();
    const int szall=3*szone;
    const int toR=szone-1;
    const int fromG=szone;
    const int toG=2*szone-1;
    const int fromB=toG+1;
    const int toB=szall-1;
    dest2.resize(szall,0.,Copy);
    rc = apply(green, firstLoc, tmp) && rc;
    dest2.fill(tmp.data(),fromG,toG);
    rc = apply(blue, firstLoc, tmp) && rc;
    dest2.fill(tmp.data(),fromB,toB);
    ++it;

    //now do the rest. no more resizing here!
    for (; it != ite; ++it) {
      const location& currLoc=*it;
      rc = apply(red, currLoc, tmp) && rc;
      dest2.fill(tmp.data(),0,toR);
      rc = apply(green, currLoc, tmp) && rc;
      dest2.fill(tmp.data(),fromG,toG);
      rc = apply(blue, currLoc, tmp) && rc;
      dest2.fill(tmp.data(),fromB,toB);
      dest.push_back(dest2);
    }

    return rc;
  }


  bool localDescriptorExtraction::apply(const channel8& src,
                                       const list<location>& locs,
                                       std::list<dvector>& dest) const { 
    channel tmp;
    tmp.castFrom(src);
    return apply(tmp,locs,dest);
  }

  bool localDescriptorExtraction::apply(const fmatrix& src,
                                       const list<location>& locs,
                                       std::list<dvector>& dest) const {
    bool rc = true;
    list<location>::const_iterator it;
    dest.clear();
    dvector tmp;
    for (it = locs.begin(); ((it != locs.end()) && rc); it++) {
      rc = rc && apply(src,*it,tmp);
      dest.push_back(tmp);
    }
    return rc;
  }

  bool localDescriptorExtraction::apply(const fmatrix&,
                                       const location&,
                                       dvector&) const {
    setStatusString("Method not implemented. Do so!");
    return false;
  }

  // splitting image into channels, apply on each channel and
  // concatenate the resulting vectors
  bool localDescriptorExtraction::apply(const matrix<rgbaPixel>& src,
                                       const location& loc,
                                       dvector& dest) const {

    channel red,green,blue;
    splitImageToRGB splitter;
    splitter.apply(src,red,green,blue);
    bool rc = true;
    dvector tmp;
    rc = rc && apply(red,loc,dest);
    const int sz=dest.size();
    const int destSize=3*sz;
    dest.resize(destSize,0.,Copy);
    rc = rc && apply(green,loc,tmp);
    const int fromB=2*sz;
    dest.fill(tmp.data(),sz,fromB-1);
    rc = rc && apply(blue,loc,tmp);
    dest.fill(tmp.data(),fromB,destSize-1);
    return rc;
  }

  bool localDescriptorExtraction::apply(const fmatrix& src,
                                       dvector& dest) const {
    const int s = src.rows();

    // we need a square region
    assert(s==src.columns());

    const parameters& par = getParameters();
    const int border = par.borderWidth;
    const float s2 = static_cast<float>(s)/2.f;
    const float radius = s2-static_cast<float>(border);
    location loc(fpoint(s2,s2),0.0f, radius);
    return apply(src,loc,dest);
  }

}

#include "ltiUndebug.h"




