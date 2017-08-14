/*
 * Copyright (C) 1998-2006
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
 * \file   ltiColorQuantization.cpp
 *         Contains parent class for all color quantization algorithms.
 * \author Pablo Alvarado
 * \date   08.05.2001 (LTI-Lib 1)
 * \date   12.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiColorQuantization.cpp,v 1.3 2006-06-06 11:35:37 doerfler Exp $
 */

#include "ltiColorQuantization.h"
#include "ltiUsePalette.h"

namespace lti {
  // --------------------------------------------------
  // colorQuantization::parameters
  // --------------------------------------------------

  // default constructor
  colorQuantization::parameters::parameters()
    : functor::parameters() {

    numberOfColors = int(256);
  }

  // copy constructor
  colorQuantization::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  colorQuantization::parameters::~parameters() {
  }

  const std::string&  colorQuantization::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  colorQuantization::parameters&
  colorQuantization::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    numberOfColors = other.numberOfColors;

    return *this;
  }

  // alias for copy member
  colorQuantization::parameters&
  colorQuantization::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  colorQuantization::parameters* colorQuantization::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance member
  colorQuantization::parameters* 
  colorQuantization::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool colorQuantization::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"numberOfColors",numberOfColors);
    }

    b = b && functor::parameters::write(handler,false);

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
  bool colorQuantization::parameters::read(ioHandler& handler,
                                           const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"numberOfColors",numberOfColors);
    }

    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // colorQuantization
  // --------------------------------------------------

  // default constructor
  colorQuantization::colorQuantization()
    : functor(){
  }

  // copy constructor
  colorQuantization::colorQuantization(const colorQuantization& other)
    : functor()  {
    copy(other);
  }

  // destructor
  colorQuantization::~colorQuantization() {
  }

  // Name of the class
  const std::string& colorQuantization::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // Copy member
  colorQuantization&
  colorQuantization::copy(const colorQuantization& other) {
    functor::copy(other);
    return (*this);
  }
  
  // return parameters
  const colorQuantization::parameters&
  colorQuantization::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // On place apply for type image!
  bool colorQuantization::apply(image& srcdest) const {
    image tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type image!
  bool colorQuantization::apply(const image& src,
                                      image& dest) const {

    lti::palette pal;
    matrix<int32> mask;

    if (apply(src,mask,pal)) {
      usePalette usePal;
      return usePal.apply(mask,pal,dest);
    }

    return false;
  }

}




