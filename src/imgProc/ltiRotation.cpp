/*
 * Copyright (C) 2003 Vlad Popovici, EPFL STI-ITS, Switzerland
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
 * \file   ltiRotation.cpp
 *         This file contains the functor rotation with rotates matrices
 * \author Vlad Popovici
 * \author Pablo Alvarado
 * \date   18.06.2003
 *
 * $Id: ltiRotation.cpp,v 1.3 2006-05-16 11:59:36 doerfler Exp $
 */

#include "ltiRotation.h"
#include "ltiRotation_template.h"
#include "ltiRGBAPixel.h"


namespace lti {

  // --------------------------------------------------
  // rotation::parameters
  // --------------------------------------------------

  // default constructor
  rotation::parameters::parameters()
    : modifier::parameters() {
    angle = 0.0;
  }

  // copy constructor
  rotation::parameters::parameters(const parameters& other)
    : modifier::parameters() {
    copy(other);
  }

  // destructor
  rotation::parameters::~parameters() {
  }

  // copy member
  rotation::parameters&
    rotation::parameters::copy(const parameters& other) {
    modifier::parameters::copy(other);
    
    angle = other.angle;

    return *this;
  }

  // alias for copy member
  rotation::parameters&
  rotation::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // return class name
  const std::string& rotation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  rotation::parameters* rotation::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance member
  rotation::parameters* rotation::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool rotation::parameters::write(ioHandler& handler,
                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"angle",angle);
    }

    b = b && modifier::parameters::write(handler,false);

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
   * @return true if write was successful
   */
  bool rotation::parameters::read(ioHandler& handler,
                                  const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"angle",angle);
    }

    b = b && modifier::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // rotation
  // --------------------------------------------------

  // default constructor
  rotation::rotation()
    : modifier(){
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  rotation::rotation(const parameters& par)
    : modifier() {

    // set the given parameters
    setParameters(par);
  }

  // copy constructor
  rotation::rotation(const rotation& other)
    : modifier() {
    copy(other);
  }

  // destructor
  rotation::~rotation() {
  }

  // copy member
  rotation& rotation::copy(const rotation& other) {
    modifier::copy(other);

    return (*this);
  }

  // alias for copy member
  rotation& rotation::operator=(const rotation& other) {
    return (copy(other));
  }

  // return class name
  const std::string& rotation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  rotation* rotation::clone() const {
    return new rotation(*this);
  }

  // new instance member
  rotation* rotation::newInstance() const {
    return new rotation();
  }

  // return parameters
  const rotation::parameters& rotation::getParameters() const {
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

  // all applies are template member functions see ltiRotation_template.h


  // -------------------------------------------------------------------
  // explicit function instantiations
  // -------------------------------------------------------------------

  // matrix<float>
  template bool rotation::rotate90(const matrix<float>&, 
                                   matrix<float>&) const;
  template bool rotation::rotate180(const matrix<float>&, 
                                    matrix<float>&) const;
  template bool rotation::rotate270(const matrix<float>&, 
                                    matrix<float>&) const;
  template bool rotation::rotateShear(const matrix<float>&, 
                                      matrix<float>&,
                                      const double&) const;
  template bool rotation::rotate(const matrix<float>&, 
                                 matrix<float>&,
                                 const double&) const;
  template bool rotation::rotate(matrix<float>&,
                                 const double&) const;
  template bool rotation::apply(const matrix<float>&, 
                                 matrix<float>&) const;
  template bool rotation::apply(matrix<float>&) const;

  // matrix<ubyte>
  template bool rotation::rotate90(const matrix<ubyte>&, 
                                   matrix<ubyte>&) const;
  template bool rotation::rotate180(const matrix<ubyte>&, 
                                    matrix<ubyte>&) const;
  template bool rotation::rotate270(const matrix<ubyte>&, 
                                    matrix<ubyte>&) const;
  template bool rotation::rotateShear(const matrix<ubyte>&, 
                                      matrix<ubyte>&,
                                      const double&) const;
  template bool rotation::rotate(const matrix<ubyte>&, 
                                 matrix<ubyte>&,
                                 const double&) const;
  template bool rotation::rotate(matrix<ubyte>&,
                                 const double&) const;
  template bool rotation::apply(const matrix<ubyte>&, 
                                 matrix<ubyte>&) const;
  template bool rotation::apply(matrix<ubyte>&) const;

  // matrix<rgbaPixel>
  template bool rotation::rotate90(const matrix<rgbaPixel>&, 
                                   matrix<rgbaPixel>&) const;
  template bool rotation::rotate180(const matrix<rgbaPixel>&, 
                                    matrix<rgbaPixel>&) const;
  template bool rotation::rotate270(const matrix<rgbaPixel>&, 
                                    matrix<rgbaPixel>&) const;
  template bool rotation::rotateShear(const matrix<rgbaPixel>&, 
                                      matrix<rgbaPixel>&,
                                      const double&) const;
  template bool rotation::rotate(const matrix<rgbaPixel>&, 
                                 matrix<rgbaPixel>&,
                                 const double&) const;
  template bool rotation::rotate(matrix<rgbaPixel>&,
                                 const double&) const;
  template bool rotation::apply(const matrix<rgbaPixel>&, 
                                 matrix<rgbaPixel>&) const;
  template bool rotation::apply(matrix<rgbaPixel>&) const;
  

}


