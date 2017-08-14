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


#include "ltiLocalExtremes.h"
#include "ltiLocalExtremes_template.h"

namespace lti {

  // --------------------------------------------------
  // localExtremes::parameters
  // --------------------------------------------------

  //default constructor
  localExtremes::parameters::parameters() : modifier::parameters() {
    extremesType=Max;
    kernelShape = Square;
    kernelSize = 3;
    relativeThreshold = float(0.0f);
    hystheresisThreshold = -1.0f;
    maxNumber = -1;
    strictHystheresis=true;
  }

  // copy constructor
  localExtremes::parameters::parameters(const parameters& other) 
    : modifier::parameters() {
    copy(other);
  }

  //destructor
  localExtremes::parameters::~parameters() {
  }

  //copy function
  localExtremes::parameters& 
  localExtremes::parameters::copy(const parameters& other) {
    modifier::parameters::copy(other);
    
    extremesType = other.extremesType;
    kernelShape = other.kernelShape;
    kernelSize = other.kernelSize;
    relativeThreshold = other.relativeThreshold;
    hystheresisThreshold = other.hystheresisThreshold;
    maxNumber = other.maxNumber;
    strictHystheresis = other.strictHystheresis;
    return *this;
  }

  // assignment operator
  localExtremes::parameters& 
  localExtremes::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& localExtremes::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  localExtremes::parameters*
  localExtremes::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  localExtremes::parameters*
  localExtremes::parameters::newInstance() const {
    return new parameters();
  }

  //write
  bool localExtremes::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      switch (extremesType) {
        case Min:
          lti::write(handler,"extremesType","Min");
          break;
        case Max:
          lti::write(handler,"extremesType","Max");
          break;
        case MinMax:
          lti::write(handler,"extremesType","MinMax");
          break;
        default:
          lti::write(handler,"extremesType","Unknown");
          handler.setStatusString("Unknown eExtremesType in localExtremes");
          b=false;
      }

      switch (kernelShape) {
        case Square:
          lti::write(handler,"kernelShape","Square");
          break;
        case Circle:
          lti::write(handler,"kernelShape","Circle");
          break;
        default:
          lti::write(handler,"kernelShape","Unknown");
          break;
      }

      lti::write(handler,"kernelSize",kernelSize);
      lti::write(handler,"relativeThreshold",relativeThreshold);
      lti::write(handler,"hystheresisThreshold",hystheresisThreshold);
      lti::write(handler,"maxNumber",maxNumber);
      lti::write(handler,"strictHystheresis",strictHystheresis);
    }
    
    b = b && modifier::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  //read
  bool localExtremes::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      
      std::string tmp;
      lti::read(handler,"extremesType",tmp);
      if (tmp=="Max" || tmp=="max") {
        extremesType = Max;
      } else if (tmp=="Min" || tmp=="min") {
        extremesType = Min;
      } else if (tmp=="MinMax" || tmp=="minmax") {
        extremesType = MinMax;
      } else {
        extremesType = Max; //default
        handler.setStatusString("Unknown eExtremesType in localExtremes");
        b=false;
      }

      lti::read(handler,"kernelShape",tmp);
      if (tmp=="Square" || tmp=="square") {
        kernelShape=Square;
      } else if (tmp=="Circle" || tmp=="circle") {
        kernelShape=Circle;
      } else {
        kernelShape=Square; //default
        handler.setStatusString("Unknown eKernelShape in localExtremes");
        b=false;
      }

      lti::read(handler,"kernelSize",kernelSize);
      lti::read(handler,"relativeThreshold",relativeThreshold);
      lti::read(handler,"hystheresisThreshold",hystheresisThreshold);
      lti::read(handler,"maxNumber",maxNumber);  
      lti::read(handler,"strictHystheresis",strictHystheresis);
    }
    
    b = b && modifier::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  

  // --------------------------------------------------
  // localExtremes
  // --------------------------------------------------

  // default constructor
  localExtremes::localExtremes() : modifier() {

    parameters param;
    setParameters(param);
  }

  // default constructor
  localExtremes::localExtremes(const parameters& par)
    : modifier() {
    setParameters(par);
  }


  // copy constructor
  localExtremes::localExtremes(const localExtremes& other)  : modifier() {
    copy(other);
  }

  // destructor
  localExtremes::~localExtremes() {
  }

  // copy member
  localExtremes&
    localExtremes::copy(const localExtremes& other) {
      modifier::copy(other);

    return (*this);
  }

  // alias for copy member
  localExtremes&
  localExtremes::operator=(const localExtremes& other) {
    return (copy(other));
  }

  // class name
  const std::string& localExtremes::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  localExtremes* localExtremes::clone() const {
    return new localExtremes(*this);
  }

  // newInstance member
  localExtremes* localExtremes::newInstance() const {
    return new localExtremes();
  }

  // return parameters
  const localExtremes::parameters&
    localExtremes::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // Go through the points and splice of any excess points
  // checking the size is just as expensive as walking through
  void localExtremes::clipPoints(ipointList& pts,
                                 const int maxNum) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    if (maxNum < 1) return;
    
    ipointList::iterator it=pts.begin();
    const ipointList::iterator ite=pts.end();

    int i=0;

    while (i<=maxNum && it!=ite) {
      i++;
      it++;
    }

    // splice off unwanted points
    if (it!=ite) {
      ipointList dummy;
      dummy.splice(dummy.begin(),pts,it,ite);
    }
      
#ifdef TIMING    
    timo.stop();
    std::cerr << "clipPoints(): " << timo.getTime() << "\n";
#endif
  }

  // Go through the points and splice of any excess points
  // checking the size is just as expensive as walking through
  void localExtremes::clipPoints(lti::list<int>& pts,
                                 const int maxNum) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    if (maxNum < 1) return;
    
    lti::list<int>::iterator it=pts.begin();
    const lti::list<int>::iterator ite=pts.end();

    int i=0;

    while ((i<=maxNum) && (it!=ite)) {
      i++;
      it++;
    }

    // splice off unwanted points
    if (it!=ite) {
      lti::list<int> dummy;
      dummy.splice(dummy.begin(),pts,it,ite);
    }
      
#ifdef TIMING    
    timo.stop();
    std::cerr << "clipPoints(): " << timo.getTime() << "\n";
#endif
  }

  //*****************************************************************
  // explicit instatiations of apply() member functions for matrices
  //*****************************************************************


  template bool localExtremes::apply<float>(const matrix<float>&, 
                                            ipointList&) const;

  template bool localExtremes::apply<float>(const matrix<float>&, 
                                            ipointList&,
                                            ipointList&) const;

  template bool localExtremes::apply<double>(const matrix<double>&, 
                                             ipointList&) const;

  template bool localExtremes::apply<double>(const matrix<double>&, 
                                             ipointList&,
                                             ipointList&) const;

  template bool localExtremes::apply<ubyte>(const matrix<ubyte>&, 
                                            ipointList&) const;

  template bool localExtremes::apply<ubyte>(const matrix<ubyte>&, 
                                            ipointList&,
                                            ipointList&) const;

  template bool localExtremes::apply<int>(const matrix<int>&, 
                                          ipointList&) const;

  template bool localExtremes::apply<int>(const matrix<int>&, 
                                          ipointList&,
                                          ipointList&) const;

  //****************************************************************
  // explicit instatiations of apply() member functions for vectors 
  //****************************************************************


  template bool localExtremes::apply<float>(const vector<float>&, 
                                            lti::list<int>&) const;

  template bool localExtremes::apply<float>(const vector<float>&, 
                                            lti::list<int>&,
                                            lti::list<int>&) const;

  template bool localExtremes::apply<double>(const vector<double>&, 
                                             lti::list<int>&) const;

  template bool localExtremes::apply<double>(const vector<double>&, 
                                             lti::list<int>&,
                                             lti::list<int>&) const;

  template bool localExtremes::apply<ubyte>(const vector<ubyte>&, 
                                            lti::list<int>&) const;

  template bool localExtremes::apply<ubyte>(const vector<ubyte>&, 
                                            lti::list<int>&,
                                            lti::list<int>&) const;

  template bool localExtremes::apply<int>(const vector<int>&, 
                                          lti::list<int>&) const;

  template bool localExtremes::apply<int>(const vector<int>&, 
                                          lti::list<int>&,
                                          lti::list<int>&) const;


}

