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
 * \file   ltiGeneralizedLocalMoments.cpp
 *         This file contains the declaration of the class
 *         generalizedLocalMoments.
 * \author Arnd Hannemann
 * \date   27.05.2005
 *
 * $Id: ltiGeneralizedLocalMoments.cpp,v 1.9 2009-08-30 04:30:29 alvarado Exp $
 */

#include "ltiLocalSampling.h"
#include "ltiLocalSampling_template.h"

#include "ltiGeneralizedLocalMoments.h"
#include "ltiGeneralizedLocalMoments_template.h"

#undef _LTI_DEBUG
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // generalizedLocalMoments::parameters
  // --------------------------------------------------

  // default constructor
  generalizedLocalMoments::parameters::parameters()
    : localDescriptorExtraction::parameters() {
    setSampling("lti::sliceSampling");
    invariantType = parameters::GPD;
    independent = true;
  }

  // copy constructor
  generalizedLocalMoments::parameters::parameters(const parameters& other)
    : localDescriptorExtraction::parameters() {
    copy(other);
  }

  // destructor
  generalizedLocalMoments::parameters::~parameters() {
    
  }
  
  // class name
  const std::string& generalizedLocalMoments::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  // clone method
  generalizedLocalMoments::parameters* 
  generalizedLocalMoments::parameters::clone() const {
    return new parameters(*this);
  }
  
  // new instance
  generalizedLocalMoments::parameters* 
  generalizedLocalMoments::parameters::newInstance() const {
    return new parameters();
  }

  // copy member

  generalizedLocalMoments::parameters&
    generalizedLocalMoments::parameters::copy(const parameters& other) {

    localDescriptorExtraction::parameters::copy(other);
    
    invariantType = other.invariantType;
    independent = other.independent;

    return *this;
  }

  // alias for copy member
  generalizedLocalMoments::parameters&
    generalizedLocalMoments::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool generalizedLocalMoments::parameters::write(ioHandler& handler,
                                         const bool complete) const
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && localDescriptorExtraction::parameters::write(handler,false);
    b = b && lti::write(handler,"independent",independent);
    if (b) {
      std::string tmp;
      switch (invariantType) {
        case NoGP: tmp = "NoGP"; break;
        case Rot:  tmp = "Rot";  break;
        case GPD:  tmp = "GPD";  break;
        case GPSO: tmp = "GPSO"; break;
      default: tmp = "GPD"; 
      }
      b = b && lti::write(handler,"invariantType",tmp);
      
    }


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
  bool generalizedLocalMoments::parameters::read(ioHandler& handler,
                                        const bool complete)

  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && localDescriptorExtraction::parameters::read(handler,false);
    b = b && lti::read(handler,"independent",independent);
    if (b) {
      std::string tmp;
      b = b && lti::read(handler,"invariantType",tmp);
      if (tmp == "NoGP") {
        invariantType = NoGP;
      } else if (tmp == "Rot") {
        invariantType = Rot;
      } else if (tmp == "GPD") {
        invariantType = GPD;
      } else if (tmp == "GPSO") {
        invariantType = GPSO;
      } else {
        // default
        invariantType = GPD;
      }
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // generalizedLocalMoments
  // --------------------------------------------------

  // default constructor
  generalizedLocalMoments::generalizedLocalMoments()
    : localDescriptorExtraction(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  generalizedLocalMoments::generalizedLocalMoments(const parameters& par)
    : localDescriptorExtraction() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  generalizedLocalMoments::generalizedLocalMoments(const generalizedLocalMoments& other)
    : localDescriptorExtraction() {
    copy(other);
  }

  // destructor
  generalizedLocalMoments::~generalizedLocalMoments() {
  }

 // class name
  const std::string& generalizedLocalMoments::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  generalizedLocalMoments* generalizedLocalMoments::clone() const {
    return new generalizedLocalMoments(*this);
  }

  // newInstance member
  generalizedLocalMoments* generalizedLocalMoments::newInstance() const {
    return new generalizedLocalMoments();
  }

  // copy member
  generalizedLocalMoments&
    generalizedLocalMoments::copy(const generalizedLocalMoments& other) {
      localDescriptorExtraction::copy(other);

    return (*this);
  }

  // alias for copy member
  generalizedLocalMoments&
    generalizedLocalMoments::operator=(const generalizedLocalMoments& other) {
    return (copy(other));
  }


  // return parameters
  const generalizedLocalMoments::parameters&
    generalizedLocalMoments::getParameters() const {
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
  bool generalizedLocalMoments::apply(const matrix<image::value_type>& img,
                                      const location& loc,
                                      dvector& result) const {


    // shortcut
    typedef image::value_type T;

    bool rc = true;

    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();

    // how many accu results are expected in the dest dvector
    int accuNo = ls->getNumberOfRegions();
    
    int i;
    
    dvector dest;

    // calculating invariants
    switch (par.invariantType) {
      case parameters::NoGP: {
        noGPaccumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = noGPaccumulator<T>::getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments 
        // sequential order needed for calculating moments!
        rc = rc && ls->apply(accu,img,loc,dest,localSampling::Sequential);
    
        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }

          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there are 18 noGP invariants
          const int invNo = 18;
          result.resize(accuNo*invNo,AllocateOnly);
         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateNoGPInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
        break;
      }
      case parameters::GPD: {
        accumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = accu.getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments
        rc = rc && ls->apply(accu,img,loc,dest,localSampling::Sequential);
        
        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there 21 independent GPD invariants (24 at all);
          int invNo;
          if (par.independent) {
            invNo = 21;
          } else{
            invNo = 24;
          }
          result.resize(accuNo*invNo,AllocateOnly);
         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateGPDInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
        break;
      } // case GPD
      case parameters::GPSO: {
        accumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = accu.getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments
        rc = rc && ls->apply(accu,img,loc,dest,localSampling::Sequential);

        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there 18 independent GPSO invariants (21 at all);
          int invNo;
          if (par.independent) {
            invNo = 18;
          } else{
            invNo = 21;
          }
          result.resize(accuNo*invNo,AllocateOnly);

         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateGPSOInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
        break;
      } // case GPSO
        /************************************** GPSO ***************/
      case parameters::Rot: {
        noGPaccumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = accu.getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments
        rc = rc && ls->apply(accu,img,loc,dest,localSampling::Sequential);

        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there are 9 ROT invariants
          const int invNo = 9;
          result.resize(accuNo*invNo,AllocateOnly);

         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateRotInvariants(destPtr+destOffset,par,d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
      break;
      }
    } // switch
    return rc;
  }

  /**
   * Apply for a region
   */
  bool generalizedLocalMoments::apply(const matrix<image::value_type>& img,
                                      dvector& result) const {
    
    // shortcut
    typedef image::value_type T;

    bool rc = true;

    const parameters& par = getParameters();
    localSampling* ls = par.getSampling();

    // how many accu results are expected in the dest dvector
    int accuNo = ls->getNumberOfRegions();
    
    int i;
    
    
    dvector dest;

    //     lti::fixedRaySampling<rgbaPixel, accumulator<rgbaPixel,false> > frs;

     // calculating invariants
    switch (par.invariantType) {
      case parameters::NoGP: {
        noGPaccumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = noGPaccumulator<T>::getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments 
        // sequential order needed for calculating moments!
        rc = rc && ls->apply(accu,img,dest,localSampling::Sequential);
    
        if (rc) {
          if (dest.size() != expected) {
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there are 18 noGP invariants
          const int invNo = 18;
          result.resize(accuNo*invNo,AllocateOnly);

         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateNoGPInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
        break;
      }
      case parameters::GPD: {
        accumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = accu.getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments
        rc = rc && ls->apply(accu,img,dest,localSampling::Sequential);
        
        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there 21 independent GPD invariants (24 at all);
          int invNo;
          if (par.independent) {
            invNo = 21;
          } else{
            invNo = 24;
          }
          result.resize(accuNo*invNo,AllocateOnly);

         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateGPDInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
        break;
      } // case GPD
      case parameters::GPSO: {
        accumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = accu.getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments
        rc = rc && ls->apply(accu,img,dest,localSampling::Sequential);

        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there 18 independent GPSO invariants (21 at all);
          int invNo;
          if (par.independent) {
            invNo = 18;
          } else{
            invNo = 21;
          }
          result.resize(accuNo*invNo,AllocateOnly);

         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateGPSOInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
        break;
      } // case GPSO
        /************************************** GPSO ***************/
      case parameters::Rot: {
        noGPaccumulator<T> accu;
        accu.reset();

        // size of one accu result
        const int size = accu.getArraySize();
        // expected length of result
        const int expected = accuNo * size;
        // sample image and calculate moments
        rc = rc && ls->apply(accu,img,dest,localSampling::Sequential);

        if (rc) {
          if (dest.size() != expected){
            setStatusString("Confused: unexpected vector size, maybe a bug?");
            return false;
          }
          
          if (dest.size() == 0) {
            return true;
          }
          // pointer to the sampled data
          const double* destPtr = &dest[0];
          
          // there are 9 ROT invariants
          const int invNo = 9;
          result.resize(accuNo*invNo,AllocateOnly);

         
          // vector for temporary result of one calculation
          dvector d(invNo);
          
          for (i=0;i<accuNo;i++) {
            const int destOffset = i*size;
            const int resultOffset = i*invNo;
            // ugly pointers but fast...
            rc = rc && calculateRotInvariants(destPtr+destOffset,par,
                                               d);
            // copy results
            result.fill(d,resultOffset);
          }
        } // if
      break;
      }
    } // switch
    return rc;

  } 

  bool 
  generalizedLocalMoments::calculateRotInvariants(const double m[],
                                                  const parameters&,
                                                  dvector& dest) const {
    typedef noGPaccumulator<rgbaPixel> a_;
    dest.resize(9);
    dvector::iterator it = dest.begin();
    double n = m[a_::M_M];
    double num,denom;

    if(m[a_::R_M] == 0) const_cast<double*>(m)[a_::R_M] = 1.0;
    if(m[a_::G_M] == 0) const_cast<double*>(m)[a_::G_M] = 1.0;
    if(m[a_::B_M] == 0) const_cast<double*>(m)[a_::B_M] = 1.0;

    // D_{02}
    // Color combination RG
    *(it++) = m[a_::RG_M]/n;
    // Color combination RB
    *(it++) = m[a_::RB_M]/n;
    // Color combination GB
    *(it++) = m[a_::GB_M]/n;

    // RED
    // S_{11}
    num = m[a_::R_X]*m[a_::R_X]+m[a_::R_Y]*m[a_::R_Y];
    num = sqrt(num);
    *(it++) = num / m[a_::R_M];

    // S_{21}
    num = m[a_::R_X2]+m[a_::R_Y2];
    //num = sqrt(num);
    denom = n*m[a_::R_M];
    *(it++) = sqrt(num / denom);

    // GREEN
    // S_{11}
    num = m[a_::G_X]*m[a_::G_X]+m[a_::G_Y]*m[a_::G_Y];
    num = sqrt(num);
    *(it++) = num / m[a_::G_M];

    // S_{21}
    num = m[a_::G_X2]+m[a_::G_Y2];
    //num = sqrt(num);
    denom = n*m[a_::G_M];
    *(it++) = sqrt(num / denom);

    // BLUE
    // S_{11}
    num = m[a_::B_X]*m[a_::B_X]+m[a_::B_Y]*m[a_::B_Y];
    num = sqrt(num);
    *(it++) = num / m[a_::B_M];

    // S_{21}
    num = m[a_::B_X2]+m[a_::B_Y2];
    //num = sqrt(num);
    denom = n*m[a_::B_M];
    *(it++) = sqrt(num / denom);


    return true;
  }


  /**
   * Internal helper function to calculate Simple invariants
   */
  bool 
  generalizedLocalMoments::calculateNoGPInvariants(const double m[],
                                                   const parameters&,
                                                   dvector& dest) const {
    typedef noGPaccumulator<rgbaPixel> a_;
    // we have 18 results
    dest.resize(18,double(),AllocateOnly);
    dvector::iterator it = dest.begin();
    const float n = static_cast<float>(m[a_::M_M]);
    
    //if the mean value of any color band == 0 then the weighted values 
    //are also 0 and you can set this value to 1 to avoid division by zero 
    //on an equation like 0/0
    if(m[a_::R_M] == 0) const_cast<double*>(m)[a_::R_M] = 1.0;
    if(m[a_::G_M] == 0) const_cast<double*>(m)[a_::G_M] = 1.0;
    if(m[a_::B_M] == 0) const_cast<double*>(m)[a_::B_M] = 1.0; 

    // RG
    *(it++) = m[a_::RG_M]/n;
    // GB
    *(it++) = m[a_::GB_M]/n;
    // RB
    *(it++) = m[a_::RB_M]/n;

    // 1-band with X
    // R
    *(it++) = m[a_::R_X]/m[a_::R_M];
    // G
    *(it++) = m[a_::G_X]/m[a_::G_M];
    // B
    *(it++) = m[a_::B_X]/m[a_::B_M];

    // 1-band with Y
    // R
    *(it++) = m[a_::R_Y]/m[a_::R_M];
    // G
    *(it++) = m[a_::G_Y]/m[a_::G_M];
    // B
    *(it++) = m[a_::B_Y]/m[a_::B_M];

    // 1-band with X and Y
    // R
    *(it++) = m[a_::R_XY]/m[a_::R_M];
    // G
    *(it++) = m[a_::G_XY]/m[a_::G_M];
    // B
    *(it++) = m[a_::B_XY]/m[a_::B_M];


    // 1-band with squared X
    // R
    *(it++) = m[a_::R_X2]/m[a_::R_M];
    // G
    *(it++) = m[a_::G_X2]/m[a_::G_M];
    // B
    *(it++) = m[a_::B_X2]/m[a_::B_M];

    // 1-band with squeared Y
    // R
    *(it++) = m[a_::R_Y2]/m[a_::R_M];
    // G
    *(it++) = m[a_::G_Y2]/m[a_::G_M];
    // B
    *(it++) = m[a_::B_Y2]/m[a_::B_M];

    return true;
  }

  /**
   * Internal helper function to calculate PSO invariants
   */
  bool 
  generalizedLocalMoments::calculatePSOInvariants(const double m[], 
                                                  const parameters&,
                                                  dvector& dest) const {
    
    typedef accumulator<rgbaPixel> a_;

    // 24 independent
    dest.resize(24);

    double n = m[a_::M_M];

    dvector::iterator it = dest.begin();
    double num,denom;


    // 1 - band
    // RED
    // S_11
//     *(it++) = m[a_::R_X]/m[a_::R_Y];
    num = n*m[a_::R_X]-m[a_::M_X]*m[a_::R_M];
    denom = n*m[a_::R_Y]-m[a_::M_Y]*m[a_::R_M];
    *(it++) = num/denom;
    // S_12^1 x
    num = m[a_::M_X]*m[a_::R2_X]-m[a_::R_X]*m[a_::R_X];
    denom = n*m[a_::R_X]-m[a_::M_X]*m[a_::R_M];
    denom *= denom;
    *(it++) = num/denom;
//    _lti_debug("s12^1: num="<<num<<"\ndenom="<<denom<<"\n");
    // S_12^1 y 
    num = m[a_::M_Y]*m[a_::R2_Y]-m[a_::R_Y]*m[a_::R_Y];
    denom = n*m[a_::R_Y]-m[a_::M_Y]*m[a_::R_M];
    denom *= denom;
    *(it++) = num/denom;
    // S_12^2
    num = n*m[a_::R2_M]-m[a_::R_M]*m[a_::R_M];
    denom = (n*m[a_::R_X]-m[a_::M_X]*m[a_::R_M]);
    denom *= (n*m[a_::R_Y]-m[a_::M_Y]*m[a_::R_M]);
    //_lti_debug("s12^2: num="<<num<<"\ndenom="<<denom<<"\n");
    
    *(it++) = num/denom;

    // GREEN 
    num = n*m[a_::G_X]-m[a_::M_X]*m[a_::G_M];
    denom = n*m[a_::G_Y]-m[a_::M_Y]*m[a_::G_M];
    *(it++) = num/denom;
    // S_12^1 x
    num = m[a_::M_X]*m[a_::G2_X]-m[a_::G_X]*m[a_::G_X];
    denom = n*m[a_::G_X]-m[a_::M_X]*m[a_::G_M];
    denom *= denom;
    *(it++) = num/denom;
    // S_12^1 y 
    num = m[a_::M_Y]*m[a_::G2_Y]-m[a_::G_Y]*m[a_::G_Y];
    denom = n*m[a_::G_Y]-m[a_::M_Y]*m[a_::G_M];
    denom *= denom;
    *(it++) = num/denom;
    // S_12^2
    num = n*m[a_::G2_M]-m[a_::G_M]*m[a_::G_M];
    denom = (n*m[a_::G_X]-m[a_::M_X]*m[a_::G_M]);
    denom *= (n*m[a_::G_Y]-m[a_::M_Y]*m[a_::G_M]);
    *(it++) = num/denom;


    // D_12^1
    num = n*m[a_::RG_M]-m[a_::R_M]*m[a_::G_M];
    denom = n*m[a_::R_X]-m[a_::M_X]*m[a_::M_X];
    denom *= n*m[a_::G_Y]-m[a_::M_Y]*m[a_::M_Y];
    *(it++) = num/denom;
      
    return true;
  }

  /**
   * Internal helper function to calculate GPSO invariants
   */
  bool generalizedLocalMoments::
  calculateGPSOInvariants(const double m[], const parameters& par,
                          dvector& dest) const {
    // just a shortcut
    bool indep = par.independent;

    typedef accumulator<rgbaPixel> a_;

    if (indep) {
      // there are 18 independent GPSO invariants
      dest.resize(18,double(),AllocateOnly);
    } else {
      // but 21 at all
      dest.resize(21,double(),AllocateOnly);
    }
    
    //fast bugfix, zero division is not checked
    if(   m[a_::R_M] == 0
       || m[a_::G_M] == 0
       || m[a_::B_M] == 0){
      dest = lti::dvector(indep ? 18 : 21, 0.0);
      return true;
    }

    //size is M_{00}^{000}
    const double n = m[a_::M_M];

    dvector::iterator it = dest.begin();

    // 1-band <----------
    // -----
    //  Red
    // -----
    double denom = m[a_::R2_M]-m[a_::R_M]*m[a_::R_M]/n;
    denom *= denom * denom;
    double num = m[a_::R2_X]*m[a_::R_Y]-m[a_::R2_Y]*m[a_::R_X];
    num *= num;

    // S_{12}
    *(it++) = num/denom;

    // -----
    //  Green
    // -----
    denom = m[a_::G2_M]-m[a_::G_M]*m[a_::G_M]/n;
    denom *= denom * denom;
    num = m[a_::G2_X]*m[a_::G_Y]-m[a_::G2_Y]*m[a_::G_X];
    num *= num;

    // S_{12}
    *(it++) = num/denom;

    // -----
    //  Blue
    // -----
    denom = m[a_::B2_M]-m[a_::B_M]*m[a_::B_M]/n;
    denom *= denom * denom;
    num = m[a_::B2_X]*m[a_::B_Y]-m[a_::B2_Y]*m[a_::B_X];
    num *= num;

    // S_{12}
    *(it++) = num/denom;

    // 2-band <----------
    
    // --------------------
    // Color combination RG
    // --------------------
    const double n2 = n*n;
    const double pr = m[a_::R2_M]-m[a_::R_M]*m[a_::R_M]/n;
    const double pg = m[a_::G2_M]-m[a_::G_M]*m[a_::G_M]/n;
    const double pb = m[a_::B2_M]-m[a_::B_M]*m[a_::B_M]/n;
    const double pr2 = pr*pr*n2;
    const double pg2 = pg*pg*n2;
    const double pb2 = pb*pb*n2;
    
    // D_{02}
    num = m[a_::RG_M]-m[a_::R_M]*m[a_::G_M]/n;
    num *= num;
    denom = pr*pg;
    *(it++) = num/denom;
    // D^{1}_{12}
    num = m[a_::R_X]*m[a_::G_Y]-m[a_::R_Y]*m[a_::G_X];
    num *= num;
    denom *= n2;
    *(it++) = num/denom;
    // D^{2}_{12}
    if (!(indep)) {
      num = n*(m[a_::R2_X]*m[a_::G_Y]-m[a_::R2_Y]*m[a_::G_X])
        +2*m[a_::R_M]*(m[a_::R_Y]*m[a_::G_X]-m[a_::R_X]*m[a_::G_Y]);
      num *= num;
      denom = pr2*pg;
      *(it++) = num/denom;
    }
    // D^{3}_{12}
    num = n*(m[a_::G2_X]*m[a_::R_Y]-m[a_::G2_Y]*m[a_::R_X])
      +2*m[a_::G_M]*(m[a_::R_X]*m[a_::G_Y]-m[a_::G_X]*m[a_::R_Y]);
    num *= num;
    denom = pr*pg2;
    *(it++) = num/denom;
    // D^{4}_{12}
    num = n*(m[a_::RG_X]*m[a_::R_Y]-m[a_::RG_Y]*m[a_::R_X])
      +m[a_::R_M]*(m[a_::R_X]*m[a_::G_Y]-m[a_::R_Y]*m[a_::G_X]);
    num *= num;
    denom = pr2*pg;
    *(it++) = num/denom;
    // D^{5}_{12}
    num = n*(m[a_::RG_X]*m[a_::G_Y]-m[a_::RG_Y]*m[a_::G_X])
      +m[a_::G_M]*(-m[a_::R_X]*m[a_::G_Y]+m[a_::R_Y]*m[a_::G_X]);
    num *= num;
    denom = pr*pg2;
    *(it++) = num/denom;

    // --------------------
    // Color combination RB
    // --------------------

    // D_{02}
    num = m[a_::RB_M]-m[a_::R_M]*m[a_::B_M]/n;
    num *= num;
    denom = pr*pb;
    *(it++) = num/denom;
    // D^{1}_{12}
    num = m[a_::R_X]*m[a_::B_Y]-m[a_::R_Y]*m[a_::B_X];
    num *= num;
    denom *= n2;
    *(it++) = num/denom;
    // D^{2}_{12}
    num = n*(m[a_::R2_X]*m[a_::B_Y]-m[a_::R2_Y]*m[a_::B_X])
      +2*m[a_::R_M]*(m[a_::R_Y]*m[a_::B_X]-m[a_::R_X]*m[a_::B_Y]);
    num *= num;
    denom = pr2*pb;
    *(it++) = num/denom;
    // D^{3}_{12}
    if (!(indep)) {
      num = n*(m[a_::B2_X]*m[a_::R_Y]-m[a_::B2_Y]*m[a_::R_X])
        +2*m[a_::B_M]*(m[a_::R_X]*m[a_::B_Y]-m[a_::B_X]*m[a_::R_Y]);
      num *= num;
      denom = pr*pb2;
      *(it++) = num/denom;
    }
    // D^{4}_{12}
    num = n*(m[a_::RB_X]*m[a_::R_Y]-m[a_::RB_Y]*m[a_::R_X])
      +m[a_::R_M]*(m[a_::R_X]*m[a_::B_Y]-m[a_::R_Y]*m[a_::B_X]);
    num *= num;
    denom = pr2*pb;
    *(it++) = num/denom;
    // D^{5}_{12}
    num = n*(m[a_::RB_X]*m[a_::B_Y]-m[a_::RB_Y]*m[a_::B_X])
      +m[a_::B_M]*(-m[a_::R_X]*m[a_::B_Y]+m[a_::R_Y]*m[a_::B_X]);
    num *= num;
    denom = pr*pb2;
    *(it++) = num/denom;

    // --------------------
    // Color combination GB
    // --------------------

    // D_{02}
    num = m[a_::GB_M]-m[a_::G_M]*m[a_::B_M]/n;
    num *= num;
    denom = pg*pb;
    *(it++) = num/denom;
    // D^{1}_{12}
    num = m[a_::G_X]*m[a_::B_Y]-m[a_::G_Y]*m[a_::B_X];
    num *= num;
    denom *= n2;
    *(it++) = num/denom;
    // D^{2}_{12}
    if (!(indep)) {
      num = n*(m[a_::G2_X]*m[a_::B_Y]-m[a_::G2_Y]*m[a_::B_X])
        +2*m[a_::G_M]*(m[a_::G_Y]*m[a_::B_X]-m[a_::G_X]*m[a_::B_Y]);
      num *= num;
      denom = pg2*pb;
      *(it++) = num/denom;
    }
    // D^{3}_{12}
    num = n*(m[a_::B2_X]*m[a_::G_Y]-m[a_::B2_Y]*m[a_::G_X])
      +2*m[a_::B_M]*(m[a_::G_X]*m[a_::B_Y]-m[a_::B_X]*m[a_::G_Y]);
    num *= num;
    denom = pg*pb2;
    *(it++) = num/denom;
    // D^{4}_{12}
    num = n*(m[a_::GB_X]*m[a_::G_Y]-m[a_::GB_Y]*m[a_::G_X])
      +m[a_::G_M]*(m[a_::G_X]*m[a_::B_Y]-m[a_::G_Y]*m[a_::B_X]);
    num *= num;
    denom = pg2*pb;
    *(it++) = num/denom;
    // D^{5}_{12}
    num = n*(m[a_::GB_X]*m[a_::B_Y]-m[a_::GB_Y]*m[a_::B_X])
      +m[a_::B_M]*(-m[a_::G_X]*m[a_::B_Y]+m[a_::G_Y]*m[a_::B_X]);
    num *= num;
    denom = pg*pb2;
    *(it++) = num/denom;

    return true;
  }


  /**
   * Internal helper function to calculate GPD invariants
   */
  bool generalizedLocalMoments::
  calculateGPDInvariants(const double m[], const parameters& par, 
                         dvector& dest) const {

    typedef accumulator<rgbaPixel> a_;

    // just a shortcut
    bool indep = par.independent;

    if (indep) {
      // there are 21 independent GPD Inavriants
      dest.resize(21);
    } else {
      // but 24 at all
      dest.resize(24);
    }
    
    //fast bugfix, zero division is not checked
    if(   m[a_::R_M] == 0
       || m[a_::G_M] == 0
       || m[a_::B_M] == 0){
      dest = lti::dvector(indep ? 21 : 24, 0.0);
      return true;
    }
    
    //size is M_{00}^{000}
    double n = m[a_::M_M];

    dvector::iterator it = dest.begin();


    // TODO: GPD(15) is claimed to have a bug
    // 1-band
    // -----
    //  Red
    // -----
    *(it++) = n*m[a_::R2_M]/(m[a_::R_M]*m[a_::R_M]);
    // S_{12}
    *(it++) = (m[a_::R_X]*m[a_::R2_Y]-m[a_::R2_X]*m[a_::R_Y])/(m[a_::R2_M]*m[a_::R_M]);
    // -----
    //  Green
    // -----
    *(it++) = n*m[a_::G2_M]/(m[a_::G_M]*m[a_::G_M]);
    // S_{12}
    *(it++) = (m[a_::G_X]*m[a_::G2_Y]-m[a_::G2_X]*m[a_::G_Y])/(m[a_::G2_M]*m[a_::G_M]);   
    // -----
    //  Blue
    // -----
    *(it++) = n*m[a_::B2_M]/(m[a_::B_M]*m[a_::B_M]);
    // S_{12}
    *(it++) = (m[a_::B_X]*m[a_::B2_Y]-m[a_::B2_X]*m[a_::B_Y])/(m[a_::B2_M]*m[a_::B_M]);



    // 2-band
    // --------------------
    // Color combination RG
    // --------------------
    
    // D_{02}
    *(it++) = (m[a_::RG_M]*n)/(m[a_::R_M]*m[a_::G_M]);
    // D_{11}
    *(it++) = (m[a_::R_X]*m[a_::G_Y]-m[a_::R_Y]*m[a_::G_X])/(m[a_::R_M]*m[a_::G_M]);
    // D^{1}_{12}
    *(it++) = (m[a_::R_X]*m[a_::RG_Y]-m[a_::RG_X]*m[a_::R_Y])/(m[a_::RG_M]*m[a_::R_M]);
    // D^{2}_{12}
    *(it++) = (m[a_::G_X]*m[a_::RG_Y]-m[a_::RG_X]*m[a_::G_Y])/(m[a_::RG_M]*m[a_::G_M]);
    // D^{3}_{12}
    *(it++) = (m[a_::R_X]*m[a_::G2_Y]-m[a_::G2_X]*m[a_::R_Y])/(m[a_::G2_M]*m[a_::R_M]);
    // D^{4}_{12}
    if (!(indep)) {
      *(it++) = (m[a_::R2_X]*m[a_::G_Y]-m[a_::G_X]*m[a_::R2_Y])/(m[a_::R2_M]*m[a_::G_M]);
    }
    
    // --------------------
    // Color combination RB
    // --------------------

    // D_{02}
    *(it++) = (m[a_::RB_M]*n)/(m[a_::R_M]*m[a_::B_M]);
    // D_{11}
    *(it++) = (m[a_::R_X]*m[a_::B_Y]-m[a_::R_Y]*m[a_::B_X])/(m[a_::R_M]*m[a_::B_M]);
    // D^{1}_{12}
    *(it++) = (m[a_::R_X]*m[a_::RB_Y]-m[a_::RB_X]*m[a_::R_Y])/(m[a_::RB_M]*m[a_::R_M]);
    // D^{2}_{12}
    *(it++) = (m[a_::B_X]*m[a_::RB_Y]-m[a_::RB_X]*m[a_::B_Y])/(m[a_::RB_M]*m[a_::B_M]);
    // D^{3}_{12}
    if (!(indep)) {
      *(it++) = (m[a_::R_X]*m[a_::B2_Y]-m[a_::B2_X]*m[a_::R_Y])/(m[a_::B2_M]*m[a_::R_M]);
    }
    // D^{4}_{12}
    *(it++) = (m[a_::R2_X]*m[a_::B_Y]-m[a_::B_X]*m[a_::R2_Y])/(m[a_::R2_M]*m[a_::B_M]);
    
    // --------------------
    // Color combination GB
    // --------------------
    // D_{02}
    *(it++) = (m[a_::GB_M]*n)/(m[a_::G_M]*m[a_::B_M]);
    // D_{11}
    *(it++) = (m[a_::G_X]*m[a_::B_Y]-m[a_::G_Y]*m[a_::B_X])/(m[a_::G_M]*m[a_::B_M]);
    // D^{1}_{12}
    *(it++) = (m[a_::G_X]*m[a_::GB_Y]-m[a_::GB_X]*m[a_::G_Y])/(m[a_::GB_M]*m[a_::G_M]);
    // D^{2}_{12}
    *(it++) = (m[a_::B_X]*m[a_::GB_Y]-m[a_::GB_X]*m[a_::B_Y])/(m[a_::GB_M]*m[a_::B_M]);
    // D^{3}_{12}
    *(it++) = (m[a_::G_X]*m[a_::B2_Y]-m[a_::B2_X]*m[a_::G_Y])/(m[a_::B2_M]*m[a_::G_M]);
    // D^{4}_{12}
    if (!(indep)) {
      *(it++) = (m[a_::G2_X]*m[a_::B_Y]-m[a_::B_X]*m[a_::G2_Y])/(m[a_::G2_M]*m[a_::B_M]);
    }

    return true;
  }


}
#include "ltiUndebug.h"


