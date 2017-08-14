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
 * \file   ltiInvalidMethodException.cpp
 *         Contains the class lti::invalidMethodException, this is thrown by
 *         classes derived from lti::functor.
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   14.04.1999
 *
 * $Id: ltiInvalidMethodException.cpp,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#include "ltiInvalidMethodException.h"

namespace lti {

  invalidMethodException::
  invalidMethodException()
    : exception("Method not implemented for given parameters") {
  }
  
  invalidMethodException::
  invalidMethodException(const std::string& str)
    : exception(str) {
  }

  invalidMethodException::invalidMethodException(const char* str)
    : exception(str) {
  }

  const std::string& 
  invalidMethodException::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  invalidMethodException*
  invalidMethodException::clone() const {
    return new invalidMethodException(*this);
  }
  
  invalidMethodException*
  invalidMethodException::newInstance() const {
    return new invalidMethodException();
  }

}
