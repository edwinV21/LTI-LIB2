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
 * \file   ltiSiftDescriptorExtraction.cpp
 *         Contains the class siftDescriptorExtraction.
 * \author Arnd Hannemann
 * \date   2.3.2005
 *
 * revisions ..: $Id: ltiSiftDescriptorExtraction.cpp,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */


#include "ltiSiftDescriptorExtraction.h"

// included accumulators
#include "ltiHistogramAccumulator.h"
#include "ltiInterpolatedHistogramAccumulator.h"
#include "ltiInterpolatedCyclicHistogramAccumulator.h"

#include "ltiFactory.h"

#include "ltiLocalSampling_template.h"
// ^ this include is important, otherwise the compiler cannot compile the right
//   apply method and an undefined reference linker error occurs

#undef   _LTI_DEBUG
#include "ltiDebug.h"
#include "ltiConstants.h"



namespace lti {
  // --------------------------------------------------
  // siftDescriptorExtraction::parameters
  // --------------------------------------------------

  // default constructor
  siftDescriptorExtraction::parameters::parameters()
    : localDescriptorExtraction::parameters() {
    
    setSampling("lti::siftSampling");
    accuType      = InterpolatedCyclicHistogram;
    histogramBins = 36;
    histLow       = 0;
    histHigh      = constants<float>::twoPi();
    normalize     = true;
    useClipping   = true;
    clippingValue = 0.2;
  }

  // copy constructor
  siftDescriptorExtraction::parameters::parameters(const parameters& other)
    : localDescriptorExtraction::parameters() {
    copy(other);
  }

  // destructor
  siftDescriptorExtraction::parameters::~parameters() {
  }

  // copy member

  siftDescriptorExtraction::parameters&
  siftDescriptorExtraction::parameters::copy(const parameters& other) {
    localDescriptorExtraction::parameters::copy(other);

    accuType      = other.accuType;
    histogramBins = other.histogramBins;
    histLow       = other.histLow;
    histHigh      = other.histHigh;
    normalize     = other.normalize;
    useClipping   = other.useClipping;
    clippingValue = other.clippingValue;

    return *this;
  }

  // alias for copy method
  siftDescriptorExtraction::parameters&
  siftDescriptorExtraction::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& siftDescriptorExtraction::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  siftDescriptorExtraction::parameters* 
  siftDescriptorExtraction::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  siftDescriptorExtraction::parameters* 
  siftDescriptorExtraction::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool siftDescriptorExtraction::parameters::write(ioHandler& handler,
                                                  const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      std::string tmp;
      switch (accuType) {
        case Histogram: {
          tmp = "Histogram";
          break; 
       }
        case InterpolatedHistogram: {
          tmp = "InterpolatedHistogram";
          break;
        }
        case InterpolatedCyclicHistogram: {
          tmp = "InterpolatedCyclicHistogram";
          break;
        }
        default: {
          tmp = "Histogram";
        }
      } 
      lti::write(handler,"accuType",tmp);
      lti::write(handler,"histogramBins",histogramBins);
      lti::write(handler,"histLow",histLow);
      lti::write(handler,"histHigh",histHigh);
      lti::write(handler,"normalize",normalize);
      lti::write(handler,"useClipping",useClipping);
      lti::write(handler,"clippingValue",clippingValue);
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
  bool siftDescriptorExtraction::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      std::string tmp;
      lti::read(handler,"accuType",tmp);
      if (tmp=="Histogram") {
        accuType = Histogram;
      } else if (tmp=="InterpolatedHistogram") {
        accuType = InterpolatedHistogram;
      } else if (tmp=="InterpolatedCyclicHistogram") {
        accuType = InterpolatedCyclicHistogram;
      } else { //default
        accuType = Histogram;
      }               
      lti::read(handler,"histogramBins",histogramBins);
      lti::read(handler,"histLow",histLow);
      lti::read(handler,"histHigh",histHigh);
      lti::read(handler,"normalize",normalize);
      lti::read(handler,"useClipping",useClipping);
      lti::read(handler,"clippingValue",clippingValue);
    }

    b = b && localDescriptorExtraction::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // siftDescriptorExtraction
  // --------------------------------------------------

  // default constructor
  siftDescriptorExtraction::siftDescriptorExtraction()
    : localDescriptorExtraction() {


    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  siftDescriptorExtraction::siftDescriptorExtraction(const parameters& par)
    : localDescriptorExtraction() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  siftDescriptorExtraction::siftDescriptorExtraction(const siftDescriptorExtraction& other)
    : localDescriptorExtraction() {
    copy(other);
  }

  // destructor
  siftDescriptorExtraction::~siftDescriptorExtraction() {
  }

  // copy member
  siftDescriptorExtraction& 
  siftDescriptorExtraction::copy(const siftDescriptorExtraction& other) {
    localDescriptorExtraction::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  siftDescriptorExtraction&
  siftDescriptorExtraction::operator=(const siftDescriptorExtraction& other) {
    return (copy(other));
  }

  // class name
  const std::string& siftDescriptorExtraction::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  siftDescriptorExtraction* siftDescriptorExtraction::clone() const {
    return new siftDescriptorExtraction(*this);
  }

  // create a new instance
  siftDescriptorExtraction* siftDescriptorExtraction::newInstance() const {
    return new siftDescriptorExtraction();
  }

  // return parameters
  const siftDescriptorExtraction::parameters&
  siftDescriptorExtraction::getParameters() const {
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
 
  // apply on two matrices
  bool siftDescriptorExtraction::apply(const matrix<float>& keys,
                                      const matrix<float>& values,
                                      const location& loc,
                                      dvector& dest) {

    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      return false;
    }
    
    bool rc = true;
    // apply desired template function
    switch (par.accuType) {
      case Histogram: {
        histogramAccumulator<float> accu(par.histogramBins,
                                         par.histLow,
                                         par.histHigh);
        rc = rc && ls -> apply(accu,keys,values,loc,dest);
        break;
      }
      case InterpolatedHistogram: {
        interpolatedHistogramAccumulator<float> accu(par.histogramBins,
                                                     par.histLow,
                                                     par.histHigh);
        rc = rc && ls -> apply(accu,keys,values,loc,dest);
        break;
      }
      case InterpolatedCyclicHistogram: {
        interpolatedCyclicHistogramAccumulator<float> accu(par.histogramBins,
                                                           par.histLow,
                                                           par.histHigh);
        rc = rc && ls -> apply(accu,keys,values,loc,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }
    } // switch

    // perform normalization and clipping if activated in params
    normalizeAndClip(dest);

    return rc;

  }

  // apply on two float matrices without location
  bool siftDescriptorExtraction::apply(const matrix<float>& keys,
                                      const matrix<float>& values,
                                      dvector& dest) {

    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      return false;
    }
    
    bool rc = true;
    // apply desired template function
    switch (par.accuType) {
      case Histogram: {
        histogramAccumulator<float> accu(par.histogramBins,
                                         par.histLow,
                                         par.histHigh);
        rc = rc && ls -> apply(accu,keys,values,dest);
        break;
      }
      case InterpolatedHistogram: {
        interpolatedHistogramAccumulator<float> accu(par.histogramBins,
                                                     par.histLow,
                                                     par.histHigh);
        rc = rc && ls -> apply(accu,keys,values,dest);
        break;
      }
      case InterpolatedCyclicHistogram: {
        interpolatedCyclicHistogramAccumulator<float> accu(par.histogramBins,
                                                           par.histLow,
                                                           par.histHigh);
        rc = rc && ls -> apply(accu,keys,values,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }
    } // switch

    // perform normalization and clipping if activated in params
    normalizeAndClip(dest);

    return rc;

  }

  // apply on single matrix<float>
  bool siftDescriptorExtraction::apply(const matrix<float>& src,
                                      const location& loc,
                                      dvector& dest) {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      return false;
    }
    
    bool rc = true;
    // apply desired template function
    switch (par.accuType) {
      case Histogram: {
        histogramAccumulator<float> accu(par.histogramBins,
                                         par.histLow,
                                         par.histHigh);
        rc = rc && ls -> apply(accu,src,loc,dest);
        break;
      }
      case InterpolatedHistogram: {
        interpolatedHistogramAccumulator<float> accu(par.histogramBins,
                                                     par.histLow,
                                                     par.histHigh);
        rc = rc && ls -> apply(accu,src,loc,dest);
        break;
      }
      case InterpolatedCyclicHistogram: {
        interpolatedCyclicHistogramAccumulator<float> accu(par.histogramBins,
                                                           par.histLow,
                                                           par.histHigh);
        rc = rc && ls -> apply(accu,src,loc,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }
    } // switch

    // perform normalization and clipping if activated in params
    normalizeAndClip(dest);

    return rc;
  }// apply

  // apply on single matrix<float> without location
  bool siftDescriptorExtraction::apply(const matrix<float>& src,
                                      dvector& dest) {
    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();
    
    // check sampling
    if (ls == 0) {
      setStatusString("Invalid sampling (null pointer)\n");
      return false;
    }
    
    bool rc = true;
    // apply desired template function
    switch (par.accuType) {
      case Histogram: {
        histogramAccumulator<float> accu(par.histogramBins,
                                         par.histLow,
                                         par.histHigh);
        rc = rc && ls -> apply(accu,src,dest);
        break;
      }
      case InterpolatedHistogram: {
        interpolatedHistogramAccumulator<float> accu(par.histogramBins,
                                                     par.histLow,
                                                     par.histHigh);
        rc = rc && ls -> apply(accu,src,dest);
        break;
      }
      case InterpolatedCyclicHistogram: {
        interpolatedCyclicHistogramAccumulator<float> accu(par.histogramBins,
                                                           par.histLow,
                                                           par.histHigh);
        rc = rc && ls -> apply(accu,src,dest);
        break;
      }
      default: {
        // default
        setStatusString("Invalid accuType (not known)\n");
        rc = false;
      }
    } // switch

    // perform normalization and clipping if activated in params
    normalizeAndClip(dest);

    return rc;
  }// apply

  void siftDescriptorExtraction::normalizeAndClip(dvector& descr) const {
    
    const parameters& param=getParameters();

    // nothing to do
    if (!param.normalize) return;

    const double fac=1./descr.computeSumOfElements();
    descr.multiply(fac);

    if (param.useClipping) {
      // clip
      const double cval=param.clippingValue;
      bool clipped = false;
      dvector::iterator it, ite;
      for (it=descr.begin(), ite=descr.end(); it!=ite; ++it) {
        if ((*it)>cval) {
          (*it) = cval;
          clipped = true;
        }
      }
      // normalize again
      if (clipped) {
        const double fac2=1./descr.computeSumOfElements();
        descr.multiply(fac2);
      }
    }

  }
}

#include "ltiUndebug.h"
