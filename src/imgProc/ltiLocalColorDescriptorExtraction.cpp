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
 * \file   ltiLocalColorDescriptorExtraction.cpp
 *         Contains the class localColorDescriptorExtraction.
 *         by Arnd Hannemann.
 * \author Arnd Hannemann
 * \date   10.12.2004
 *
 * revisions ..: $Id: ltiLocalColorDescriptorExtraction.cpp,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#include "ltiMeanAccumulator.h"
#include "ltiVarianceAccumulator.h"
#include "ltiLocalColorDescriptorExtraction.h"
#include "ltiLocalSampling_template.h"
// ^ this include is important, otherwise the compiler cannot compile the right
//   apply method and an undefined reference linker error occurs


namespace lti {
  // --------------------------------------------------
  // localColorDescriptorExtraction::parameters
  // --------------------------------------------------

  // default constructor
  localColorDescriptorExtraction::parameters::parameters()
    : localDescriptorExtraction::parameters() {    
    // default sampling
    setSampling("lti::sliceSampling");    
    accuType = Mean;
  }

  // copy constructor
  localColorDescriptorExtraction::parameters::parameters(const parameters& other)
    : localDescriptorExtraction::parameters() {
    copy(other);
  }

  // destructor
  localColorDescriptorExtraction::parameters::~parameters() {
  }

  // copy member
  localColorDescriptorExtraction::parameters&
  localColorDescriptorExtraction::parameters::copy(const parameters& other) {
    localDescriptorExtraction::parameters::copy(other);
    accuType = other.accuType;
    return *this;
  }

  // alias for copy method
  localColorDescriptorExtraction::parameters&
  localColorDescriptorExtraction::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& localColorDescriptorExtraction::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  localColorDescriptorExtraction::parameters* 
  localColorDescriptorExtraction::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  localColorDescriptorExtraction::parameters*
  localColorDescriptorExtraction::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool localColorDescriptorExtraction::parameters
  ::write(ioHandler& handler,
          const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      std::string tmp;
      switch (accuType) {
        case Mean: {
          tmp = "Mean";
          break;
        }
        case MeanAndVariance: {
          tmp = "MeanAndVariance";
          break;
        }
        default: {
          tmp = "Mean";
        }
      }
      lti::write(handler,"accuType",tmp);
    }

    b = b && localDescriptorExtraction::parameters::write(handler,false);

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
  bool localColorDescriptorExtraction::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {   
      b = handler.readBegin();
    }

    if (b) {
      std::string tmp;
      lti::read(handler,"accuType",tmp);
      if (tmp=="Mean") {
        accuType = Mean;
      } else if (tmp=="MeanAndVariance") {
        accuType = MeanAndVariance;
      } else {
        //default
        accuType = Mean;
      }
    }

    b = b && localDescriptorExtraction::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // localColorDescriptorExtraction
  // --------------------------------------------------

  // default constructor
  localColorDescriptorExtraction::localColorDescriptorExtraction()
    : localDescriptorExtraction() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  localColorDescriptorExtraction::localColorDescriptorExtraction(const parameters& par)
    : localDescriptorExtraction() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  localColorDescriptorExtraction::localColorDescriptorExtraction(const localColorDescriptorExtraction& other)
    : localDescriptorExtraction() {
    copy(other);
  }

  // destructor
  localColorDescriptorExtraction::~localColorDescriptorExtraction() {
  }

  // copy member
  localColorDescriptorExtraction& localColorDescriptorExtraction::
  copy(const localColorDescriptorExtraction& other) {
    localDescriptorExtraction::copy(other);
    return (*this);
  }

  // alias for copy member
  localColorDescriptorExtraction& localColorDescriptorExtraction::
  operator=(const localColorDescriptorExtraction& other) {
    return (copy(other));
  }

  // class name
  const std::string& localColorDescriptorExtraction::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  localColorDescriptorExtraction* localColorDescriptorExtraction::clone() const {
    return new localColorDescriptorExtraction(*this);
  }

  // clone member
  localColorDescriptorExtraction* 
  localColorDescriptorExtraction::newInstance() const {
    return new localColorDescriptorExtraction();
  }
  
  // return parameters
  const localColorDescriptorExtraction::parameters&
  localColorDescriptorExtraction::getParameters() const {
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

  // apply on single rgbaPixel
  bool localColorDescriptorExtraction::apply(const matrix<rgbaPixel>& src,
                                            const location& loc,
                                            dvector& dest) const {

    const parameters& par = getParameters();
    const localSampling* ls = par.getSampling();
    bool rc = true;
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      rc = false;
    }
    // apply desired template function
    switch (par.accuType) {
      case Mean: {
        meanAccumulator<rgbaPixel> accu;
        rc = rc && ls->apply(accu,src,loc,dest);
        break;
      }
      case MeanAndVariance: {
        varianceAccumulator<rgbaPixel> accu;
        rc = rc && ls->apply(accu,src,loc,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }   
    }
    
    return rc;
  }

  // apply on one matrix without location
  bool localColorDescriptorExtraction::apply(const matrix<rgbaPixel>& src,
                                            dvector& dest) const {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    bool rc = true;
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      rc = false;
    }
    // apply desired template function
    switch (par.accuType) {
      case Mean: {
        meanAccumulator<rgbaPixel> accu;
        rc = rc && ls->apply(accu,src,dest);
        break;
      }
      case MeanAndVariance: {
        varianceAccumulator<rgbaPixel> accu;
        rc = rc && ls->apply(accu,src,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }   
    }
    
    return rc;
  }

  bool localColorDescriptorExtraction::apply(const channel8& src,
                                            const list<location>& locs,
                                            std::list<dvector>& dest) const {
    return localDescriptorExtraction::apply(src,locs,dest);
  }

  // apply on single float
  bool localColorDescriptorExtraction::apply(const matrix<float>& src,
                                            const location& loc,
                                            dvector& dest) const {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    bool rc = true;
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      rc = false;
    }
    // apply desired template function
    switch (par.accuType) {
      case Mean: {
        meanAccumulator<float> accu;
        rc = rc && ls->apply(accu,src,loc,dest);
        break;
      }
      case MeanAndVariance: {
        varianceAccumulator<float> accu;
        rc = rc && ls->apply(accu,src,loc,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }   
    }

    return rc;
  }

  // apply on single float without location
  bool localColorDescriptorExtraction::apply(const matrix<float>& src,
                                            dvector& dest) const {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    bool rc = true;
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      rc = false;
    }
    // apply desired template function
    switch (par.accuType) {
      case Mean: {
        meanAccumulator<float> accu;
        rc = rc && ls->apply(accu,src,dest);
        break;
      }
      case MeanAndVariance: {
        varianceAccumulator<float> accu;
        rc = rc && ls->apply(accu,src,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }   
    }

    return rc;
  }

  // apply on channel and lists of locations
  bool localColorDescriptorExtraction::apply(const matrix<float>& src,
                                            const list<location>& locs,
                                            std::list<dvector>& dest) const {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    bool rc = true;
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      rc = false;
    }
    // apply desired template function
    switch (par.accuType) {
      case Mean: {
        meanAccumulator<float> accu;
        rc = rc && ls->apply(accu,src,locs,dest);
        break;
      }
      case MeanAndVariance: {
        varianceAccumulator<float> accu;
        rc = rc && ls->apply(accu,src,locs,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }   
    }

    return rc;
  }


  // apply on image and lists of locations
  bool localColorDescriptorExtraction::apply(const matrix<rgbaPixel>& src,
                                            const list<location>& locs,
                                            std::list<dvector>& dest) const {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    bool rc = true;
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      rc = false;
    }
    // apply desired template function
    switch (par.accuType) {
      case Mean: {
        meanAccumulator<rgbaPixel> accu;
        rc = rc && ls->apply(accu,src,locs,dest);
        break;
      }
      case MeanAndVariance: {
        varianceAccumulator<rgbaPixel> accu;
        rc = rc && ls->apply(accu,src,locs,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }   
    }

    return rc;
  }

}

