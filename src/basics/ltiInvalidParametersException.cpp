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
 * \file   ltiInvalidParametersException.cpp
 *         Contains the class lti::invalidParametersException, an exception
 *         thrown by classes derived from lti::parametersManager::parameters
 * \author Pablo Alvarado
 * \date   06.01.2005
 *
 * $Id: ltiInvalidParametersException.cpp,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#include "ltiInvalidParametersException.h"

namespace lti {

  invalidParametersException::invalidParametersException()
    : exception("Wrong parameter type or parameters not set yet") {
  }
  
  invalidParametersException::
  invalidParametersException(const std::string& str)
    : exception(std::string("wrong parameter type or parameters not set " \
                            "yet at ")+str) {
  }
  
  invalidParametersException::
  invalidParametersException(const char* str)
    : exception(std::string("Wrong parameter type or parameters not set " \
                            "yet at ")+str) {
  }

  const std::string& 
  invalidParametersException::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  invalidParametersException*
  invalidParametersException::clone() const {
    return new invalidParametersException(*this);
  }
  
  invalidParametersException*
  invalidParametersException::newInstance() const {
    return new invalidParametersException();
  }

}
