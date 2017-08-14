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
 * \file   ltiFunctor.cpp
 *         Contains the class lti::functor, which is the parent class for all
 *         functionality classes that work on images, matrices, etc.
 * \author Thomas Rusert
 * \date   14.04.1999
 *
 * $Id: ltiFunctor.cpp,v 1.13 2006-08-29 12:27:42 doerfler Exp $
 */

#include "ltiFunctor.h"
#include <sstream>

namespace lti {

  // ----------------------------
  // parameters
  // ----------------------------

  functor::parameters::parameters() 
    : parametersManager::parameters() {
  }

  functor::parameters::parameters(const parameters& other) 
    : parametersManager::parameters(other) {
  }
  
  functor::parameters::~parameters() {
  }

  // ----------------------------
  // functor
  // ----------------------------

  // constructor
  functor::functor() : ioObject(), status(), parametersManager() {
  }

  // copy constructor
  functor::functor(const functor& other) : 
    ioObject(), status(), parametersManager() {
    copy(other);
  }

  // destructor
  functor::~functor() {
  }

  // copy data of "other" functor.
  functor& functor::copy(const functor& other) {
    parametersManager::copy(other);
    return (*this);
  }

  // just invoke parametersManager::write
  bool functor::write(ioHandler& handler,
                      const bool complete) const {
    return this->parametersManager::write(handler,complete);
  }

  // just invoke parametersManager::read
  bool functor::read(ioHandler& handler,
                     const bool complete) {
    return this->parametersManager::read(handler,complete);
  }
  
  

} // namespace lti

