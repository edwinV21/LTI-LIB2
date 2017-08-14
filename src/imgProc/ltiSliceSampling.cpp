/*
 * Copyright (C) 1998-2004
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
 * \file   ltiSliceSampling.cpp
 *         Contains the template class sliceSampling.
 * \author Arnd Hannemann
 * \date   27.08.2004
 *
 * revisions ..: $Id: ltiSliceSampling.cpp,v 1.9 2012-01-03 02:22:25 alvarado Exp $
 */
 

#include "ltiConstants.h"

#undef _LTI_DEBUG 
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

#include "ltiSliceSampling.h"

namespace lti {
  //register in localSampling factory
  _LTI_REGISTER_IN_FACTORY(localSampling,sliceSampling)


  // --------------------------------------------------
  // sliceSampling<T>::parameters
  // --------------------------------------------------

  // default constructor  
  sliceSampling::parameters::parameters()
    : localSampling::parameters() {
   
    maskRadius = 7;
    slices = 6;
    overlap = true;
    filter = false;
  }

  // copy constructor
  sliceSampling::parameters::parameters(const parameters& other)
    : localSampling::parameters() {
    copy(other);
  }

  // destructor  
  sliceSampling::parameters::~parameters() {
  }

  // copy member
  sliceSampling::parameters&
  sliceSampling::parameters::copy(const parameters& other) {
    localSampling::parameters::copy(other);

    
      maskRadius = other.maskRadius;
      slices = other.slices;
      overlap = other.overlap;
      filter = other.filter;
      filterKernel = other.filterKernel;

    return *this;
  }

  // alias for copy method  
  sliceSampling::parameters&
  sliceSampling::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string&
  sliceSampling::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method  
  sliceSampling::parameters*
  sliceSampling::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance  
   sliceSampling::parameters* 
  sliceSampling::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool sliceSampling::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"maskRadius",maskRadius);
      lti::write(handler,"slices",slices);
      lti::write(handler,"overlap",overlap);
      lti::write(handler,"filter",filter);
      lti::write(handler,"filterKernel",filterKernel);
    }

    b = b && localSampling::parameters::write(handler,false);

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
  bool sliceSampling::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"maskRadius",maskRadius);
      lti::read(handler,"slices",slices);
      lti::read(handler,"overlap",overlap); 
      lti::read(handler,"filter",filter);
      lti::read(handler,"filterKernel",filterKernel);
    }

    b = b && localSampling::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // sliceSampling helper functions
  // --------------------------------------------------  
  bool sliceSampling::generateMasks() {
    // shortcuts
    const parameters& par = getParameters();
    const int radius = par.maskRadius;
    const int slices = par.slices;

    // fill ioPts_
    circle_.apply(radius, ioPts_);
    normalMask_.resize(-radius,-radius,radius,radius,
                       0,Init);
    

    int i,j,idx;
    // float y2;
    float pa;

    static const float pi2 = constants<float>::twoPi();

    const float fac = slices/pi2;

    if (par.overlap) {
      overlapMask_.resize(-radius,-radius,radius,radius,
                          0,Init);
      int idx2;
      float pa2;
      const float angle = slices/constants<float>::pi();


      // go through the y-axis
      for (i=normalMask_.firstRow();i<=normalMask_.lastRow();++i) {
        // the in and out points correspond to the borders of
        // a circle:
        // y2 = static_cast<float>(i*(i+1)) + 0.25f; // == (i+0.5)^2
        const int o = ioPts_.at(i);
        //= 
        //               ((i <= 0) ? iround(sqrt(r2-y2)) : ioPts_.at(-i)));

        // for all points between the in and out points
        for (j=-o;j<=o;++j) {
          // compute the angle of this point
          pa = atan2_(i,j);
          /* the angle returned by arctanLUT is always 
           * between 0 and 2*Pi */

          // compute the angle of this point in overlap mask
          pa2 = atan2_(i,j) + angle;;

          // ensure the angle is between 0 and 2Pi
          while (pa2 > pi2) {
            pa2 -= pi2;
          }
          while (angle < 0) {
            pa2 += pi2;
          }
          
          // compute the index of the splice
          idx = iround(pa*fac) % slices;
          idx2 = iround(pa2*fac) % slices;
          
          // and indicate the computation in the mask.
          normalMask_.at(i,j)=idx;
          overlapMask_.at(i,j)=idx2;
        }
      }
    }
    else {
      // go through the y-axis
      for (i=normalMask_.firstRow();i<=normalMask_.lastRow();++i) {
        // the in and out points correspond to the borders of
        // a circle:
        // y2 = static_cast<float>(i*(i+1)) + 0.25f; // == (i+0.5)^2
        const int o = ioPts_.at(i);

        // for all points between the in and out points
        for (j=-o;j<=o;++j) {
          // compute the angle of this point
          pa = atan2_(i,j);
          
          /* the angle returned by arctanLUT is always 
           * between 0 and 2*Pi */
          
          // compute the index of the splice
          idx = iround(pa*fac) % slices;
          
          // and indicate the computation in the mask.
          normalMask_.at(i,j)=idx;
        }
      }
    }
    _lti_debug("ioPts:\n"<<ioPts_<<"\n");
    return true;
  }


  // --------------------------------------------------
  // sliceSampling
  // --------------------------------------------------
  // default constructor  
  sliceSampling::sliceSampling()
    : localSampling() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor  
  sliceSampling::sliceSampling(const parameters& par)
    : localSampling() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  sliceSampling::sliceSampling(const sliceSampling& other)
    : localSampling() {
    copy(other);
  }

  // destructor
  sliceSampling::~sliceSampling() {
  }

  // copy member  
  sliceSampling& 
  sliceSampling::copy(const sliceSampling& other) {
    localSampling::copy(other);
    
    return (*this);
  }

  // alias for copy member  
  sliceSampling&
  sliceSampling::operator=(const sliceSampling& other) {
    return (copy(other));
  }

  // class name
  const std::string&
  sliceSampling::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member  
  sliceSampling* sliceSampling::clone() const {
    return new sliceSampling(*this);
  }

  // clone member  
  sliceSampling* sliceSampling::newInstance() const {
    return new sliceSampling();
  }

  // return parameters 
  const  sliceSampling::parameters&
  sliceSampling::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }
  
  // update parameters
  bool sliceSampling::updateParameters() {
    bool ok=true;
    const parameters& par = getParameters();
    if (par.filter) {
      filter_.setKernel(par.filterKernel);
    }
  
    if ((par.slices < 1) || (par.slices > 120)) {
      setStatusString("Wrong number of slices (not in [1,120])");
      ok=false;
    }
    if ((par.maskRadius < 3) || (par.maskRadius > 255)) {
      setStatusString("Wrong radius for mask (not in [3,255])");
      ok=false;
    }

    ok = ok && generateMasks();

    // init accus
    //  int size = par.slices;
//    normalMask_.generate(par.maskRadius,0.0,par.slices)
    // if (par.overlap) {
      //    overlapMask_.generate(par.maskRadius,Pi/par.slices,par.slices);
    //    size *= 2;
    //  }
    // resize vector
    //    if (static_cast<int>(accuVec_.size())!=size) {
    //accuVec_.resize(size);
    //}

    return ok;
  }
 
  int sliceSampling::getNumberOfRegions() const {
    const parameters& par = getParameters();
    if (par.overlap) {
      return 2*par.slices;
    } else {
      return par.slices;
    }
  }

  localSampling::eResultOrder sliceSampling::getResultOrder() const {
    return Interleaved;
  }
  

}
#include "ltiUndebug.h"
