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
 * \file   ltiSort_template.h
 *         Sort the elements in a matrix or vector
 * \author Pablo Alvarado
 * \date   17.08.2000
 *
 * $Id: ltiSort.cpp,v 1.1 2007-10-08 19:44:31 alvarado Exp $
 */

#include "ltiSort.h"

namespace lti {

  // --------------------------------------------------
  // sort::parameters
  // --------------------------------------------------

  /*
   * Default constructor
   */
  sort::parameters::parameters() : functor::parameters() {

    thresholdForBubble = int(_LTI_PERFORMANCE_SORT_STOP_QUICKSORT);
    sortingOrder = Ascending;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  sort::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }
  
  /*
   * Destructor
   */
  sort::parameters::~parameters() {
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  sort::parameters& sort::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    thresholdForBubble = other.thresholdForBubble;
    sortingOrder = other.sortingOrder;
    
    return *this;
  }

  /*
   * Returns the name of this class.
   */
  const std::string& sort::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Returns a pointer to a clone of the parameters
   */
  sort::parameters* sort::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * Returns a pointer to a new instance of the parameters
   */
  sort::parameters* sort::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool sort::parameters::read(ioHandler &handler,
                              const bool complete) {
    bool b=true;
    
    if (complete) {
      b=handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"thresholdForBubble",thresholdForBubble);
      lti::read(handler,"sortingOrder",sortingOrder);
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
  bool sort::parameters::write(ioHandler& handler,
                               const bool complete) const {
    bool b=true;
    
    if (complete) {
      b=handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"thresholdForBubble",thresholdForBubble);
      lti::write(handler,"sortingOrder",sortingOrder);
    }
    
    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b=b && handler.writeEnd();
    }
    
    return b;
  }
  
  // --------------------------------------------------
  // sort
  // --------------------------------------------------

  // default constructor
  sort::sort(const eSortingOrder sortingOrder) : functor() {
    parameters param;
    param.sortingOrder = sortingOrder;
    setParameters(param);
  }


  // default constructor
  sort::sort(const parameters& par) : functor() {
    setParameters(par);
  }


  // copy constructor
  sort::sort(const sort& other) : functor()  {
    copy(other);
  }

  // destructor
  sort::~sort() {
  }

  // copy member
  sort& sort::copy(const sort& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  const std::string& sort::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  sort* sort::clone() const {
    return new sort(*this);
  }

  // new instance member
  sort* sort::newInstance() const {
    return new sort;
  }

  // return parameters
  const sort::parameters& sort::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // set parameters
  bool sort::updateParameters() {
    const parameters& p = getParameters();
    thresholdForBubble_ = p.thresholdForBubble;
    order_ = p.sortingOrder;
    return true;
  }

}


