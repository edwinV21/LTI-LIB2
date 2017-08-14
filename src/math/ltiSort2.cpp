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
 * \file   ltiSort2_template.h
 *         Sort the elements in a matrix or vector
 * \author Pablo Alvarado
 * \date   17.08.2000
 *
 * $Id: ltiSort2.cpp,v 1.1 2007-10-08 20:18:51 alvarado Exp $
 */

#include "ltiSort2.h"

namespace lti {
  
  // -----------------------------------------------------------------
  // sort2::parameters
  // -----------------------------------------------------------------

  /*
   * Default constructor
   */
  sort2::parameters::parameters() : functor::parameters() {
        
    thresholdForBubble = int(_LTI_PERFORMANCE_SORT_STOP_QUICKSORT);
    sortingOrder = Ascending;
    whichVectors = Rows;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
   sort2::parameters::parameters(const parameters& other) 
     : functor::parameters() {
     copy(other);
   }
  
  /*
   * Destructor
   */
  sort2::parameters::~parameters() {
  }
  
  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  sort2::parameters& sort2::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    whichVectors = other.whichVectors;
    thresholdForBubble = other.thresholdForBubble;
    sortingOrder = other.sortingOrder;
    
    return *this;
  }
  
  /*
   * Returns the name of this class.
   */
  const std::string& sort2::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Returns a pointer to a clone of the parameters
   */
  sort2::parameters* sort2::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * Returns a pointer to a new instance of the parameters
   */
  sort2::parameters* sort2::parameters::newInstance() const{
    return new parameters;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool sort2::parameters::read(ioHandler &handler,
                               const bool complete) {
    bool b=true;
    
    if (complete) {
      b=handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"thresholdForBubble",thresholdForBubble);
      
      std::string str;
      lti::read(handler,"sortingOrder",sortingOrder);
      lti::read(handler,"whichVectors",str);
      if (str == "Columns") {
        whichVectors = Columns;
      } else {
        whichVectors = Rows;
      }
    }
    
    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b=b && handler.readEnd();
    }
    
    return b;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be
   *        written.
   * @return true if write was successful
   */
  bool sort2::parameters::write(ioHandler& handler,
                                const bool complete) const {
    bool b=true;
    
    if (complete) {
      b=handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"thresholdForBubble",thresholdForBubble);
      lti::write(handler,"sortingOrder",sortingOrder);
      if (whichVectors == Columns) {
        lti::write(handler,"whichVectors","Columns");
      } else {
        lti::write(handler,"whichVectors","Rows");
      }
    }
    
    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b=b && handler.writeEnd();
    }
    
    return b;
  }
  

  // -----------------------------------------------------------------
  // sort2
  // -----------------------------------------------------------------
  
  // default constructor
  sort2::sort2(const eSortingOrder sortingOrder,
               const eWhichVectors whichVectors) {
    parameters par;

    par.sortingOrder = sortingOrder;
    par.whichVectors = whichVectors;

    setParameters(par);
  }

  // default constructor
  sort2::sort2(const parameters& par) : functor() {
    setParameters(par);
  }


  // copy constructor
  sort2::sort2(const sort2& other) : functor() {
    copy(other);
  }

  // destructor
  sort2::~sort2() {
  }

  // copy member
  sort2& sort2::copy(const sort2& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  const std::string& sort2::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  sort2* sort2::clone() const {
    return new sort2(*this);
  }

  // newInstance member
  sort2* sort2::newInstance() const {
    return new sort2;
  }

  // return parameters
  const sort2::parameters& sort2::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}
