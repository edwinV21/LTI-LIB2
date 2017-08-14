/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiIncompatibleDimensionsException.cpp
 *         Contains the class lti::ltiIncompatibleDimensionsException which is
 *         thrown by linear algrebra functors.
 * \author Thomas Rusert
 * \date   01.06.1999
 * 
 * $Id: ltiIncompatibleDimensionsException.cpp,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#include "ltiIncompatibleDimensionsException.h"

namespace lti {

  // Default constructor
  incompatibleDimensionsException::incompatibleDimensionsException()
    : exception("container dimensions do not match") {
  }

  // constructor
  incompatibleDimensionsException
  ::incompatibleDimensionsException(const char* msg)
    : exception(msg) {
  }

  // constructor
  incompatibleDimensionsException
  ::incompatibleDimensionsException(const std::string& msg)
    : exception(msg) {
  }

  // class name
  const std::string&
  incompatibleDimensionsException::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone MF
  incompatibleDimensionsException*
  incompatibleDimensionsException::clone() const {
    return new incompatibleDimensionsException(*this);
  }
  
  // newInstance MF
  incompatibleDimensionsException*
  incompatibleDimensionsException::newInstance() const {
    return new incompatibleDimensionsException();
  }


}
