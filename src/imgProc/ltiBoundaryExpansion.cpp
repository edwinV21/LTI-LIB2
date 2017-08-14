/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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
 * \file   ltiBoundaryExpansion.cpp
 *         Contains the class lti::boundaryExpansion, 
 *         which is used to expand an image with a border defined through the
 *         boundaryType parameter
 * 
 * \author Pablo Alvarado
 * \date   03.10.2007
 *
 * revisions ..: $Id: ltiBoundaryExpansion.cpp,v 1.1 2007-10-05 00:46:15 alvarado Exp $
 */

#include "ltiBoundaryExpansion.h"

namespace lti {
  // --------------------------------------------------
  // boundaryExpansion::parameters
  // --------------------------------------------------

  // default constructor
  boundaryExpansion::parameters::parameters()
    : functor::parameters() {
    bottomBorder = int(5);
    leftBorder = int(5);
    rightBorder = int(5);
    topBorder = int(5);
    boundaryType = eBoundaryType(Zero);
    normalize = false;
  }

  // copy constructor
  boundaryExpansion::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  boundaryExpansion::parameters::~parameters() {
  }

  // copy member

  boundaryExpansion::parameters&
  boundaryExpansion::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    bottomBorder = other.bottomBorder;
    leftBorder = other.leftBorder;
    rightBorder = other.rightBorder;
    topBorder = other.topBorder;
    boundaryType = other.boundaryType;
    normalize = other.normalize;

    return *this;
  }

  // alias for copy method
  boundaryExpansion::parameters&
  boundaryExpansion::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& boundaryExpansion::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  boundaryExpansion::parameters*
  boundaryExpansion::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  boundaryExpansion::parameters*
  boundaryExpansion::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool boundaryExpansion::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"bottomBorder",bottomBorder);
      lti::write(handler,"leftBorder",leftBorder);
      lti::write(handler,"rightBorder",rightBorder);
      lti::write(handler,"topBorder",topBorder);
      lti::write(handler,"boundaryType",boundaryType);
      lti::write(handler,"normalize",normalize);
    }

    b = b && functor::parameters::write(handler,false);

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
  bool boundaryExpansion::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"bottomBorder",bottomBorder);
      lti::read(handler,"leftBorder",leftBorder);
      lti::read(handler,"rightBorder",rightBorder);
      lti::read(handler,"topBorder",topBorder);
      lti::read(handler,"boundaryType",boundaryType);
      lti::read(handler,"normalize",normalize);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // boundaryExpansion
  // --------------------------------------------------

  // default constructor
  boundaryExpansion::boundaryExpansion()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  boundaryExpansion::boundaryExpansion(const parameters& par)
    : functor() {


    // set the given parameters
    setParameters(par);
  }
  
  boundaryExpansion::boundaryExpansion(const int width,
                                       const eBoundaryType type,
                                       const bool normalize)
    : functor() {
    
    parameters par;
    par.leftBorder = par.rightBorder  = width;
    par.topBorder  = par.bottomBorder = width;
    par.boundaryType = type;
    par.normalize = normalize;

    setParameters(par);
  }

  // copy constructor
  boundaryExpansion::boundaryExpansion(const boundaryExpansion& other)
    : functor() {
    copy(other);
  }

  // destructor
  boundaryExpansion::~boundaryExpansion() {
  }

  // copy member
  boundaryExpansion& 
  boundaryExpansion::copy(const boundaryExpansion& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  boundaryExpansion&
  boundaryExpansion::operator=(const boundaryExpansion& other) {
    return (copy(other));
  }

  // class name
  const std::string& boundaryExpansion::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  boundaryExpansion* boundaryExpansion::clone() const {
    return new boundaryExpansion(*this);
  }

  // create a new instance
  boundaryExpansion* boundaryExpansion::newInstance() const {
    return new boundaryExpansion();
  }

  // return parameters
  const boundaryExpansion::parameters&
  boundaryExpansion::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

}

