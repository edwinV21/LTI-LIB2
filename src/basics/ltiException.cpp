/*
 * Copyright (C) 1998-2004
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


/*
 * \file   ltiException.cpp
 *         Defines base class for all exceptions in the LTI-Lib
 *         If you compile the library with the symbol _DEBUG_EXCEPTION defined
 *         the process of creating an exception will print an error message
 *         in std::cerr.  This should help you finding problems with systems
 *         that cannot cope with C++ exception (e.g. Java).
 * \author Pablo Alvarado
 * \date   07.04.1999
 *
 * $Id: ltiException.cpp,v 1.6 2007-04-04 19:18:29 alvarado Exp $
 */

#include "ltiException.h"

#ifdef _DEBUG_EXCEPTION
#include <iostream>
#endif

namespace lti {

  exception::exception(const char* excName)
    : object(),std::exception(),exceptionName_(excName) {
#   ifdef _DEBUG_EXCEPTION
    std::cerr << "lti::exception created with message: " << std::endl << "  ";
    std::cerr << excName << std::endl;
#   endif

  }

  exception::exception(const std::string& excName)
    : object(), std::exception(), exceptionName_(excName) {
#   ifdef _DEBUG_EXCEPTION
    std::cerr << "lti::exception created with message: " << std::endl << "  ";
    std::cerr << excName << std::endl;
#   endif
  }

  exception::~exception() throw () {
  }

  exception& exception::copy(const exception& other) {
    (*this)=other;
    return (*this);
  }

  exception& exception::operator=(const exception& other) {
    // call the parent class
    std::exception::operator=(other);
    exceptionName_ = other.exceptionName_;
    return *this;
  }

  /*
   * copy constructor
   */
  exception::exception(const exception& other)
    : object(), std::exception(), exceptionName_() {
    copy(other);
  }

  const std::string& exception::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  exception* exception::clone() const {
    return (new exception(*this));
  }

  exception* exception::newInstance() const {
    return (new exception());
  }

  const char* exception::what() const throw () {
    return exceptionName_.c_str();
  }

}

