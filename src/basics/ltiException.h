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


/**
 * \file   ltiException.h
 *         Defines base class for all exceptions in the LTI-Lib.
 *         If you compile the library with the symbol _DEBUG_EXCEPTION defined
 *         the process of creating an exception will print an error message
 *         in std::cerr.  This should help you finding problems with systems
 *         that cannot cope with C++ exception (e.g. Java).
 * \author Pablo Alvarado
 * \date   07.04.99
 *
 * $Id: ltiException.h,v 1.5 2007-04-04 19:18:29 alvarado Exp $
 */

#ifndef _LTI_EXCEPTION_H
#define _LTI_EXCEPTION_H

#include "ltiObject.h"
#include <exception>
#include <string>

namespace lti {
  /**
   * Base class for all LTI-Lib exceptions
   */
  class exception : public object,public std::exception {
    public:
    /**
     * Constructor with an optional string.
     * @param excName name of the exception.  These string will be copied and
     *                can be accessed with the "what()" method
     */
    exception(const char* excName = "exception");

    /**
     * Constructor with an optional string.
     * @param excName name of the exception.  These string will be copied and
     *                can be accessed with the "what()" method
     */
    exception(const std::string& excName);

    /**
     * Copy constructor
     */
    exception(const exception& other);

    /**
     * Destructor
     */
    virtual ~exception() throw ();

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Exception string
     *
     * @return The internal string with the information set at construction
     *         time.
     */
    virtual const char* what() const throw ();

    /**
     * Copy member
     */
    exception& copy(const exception& other);

    /**
     * Clone this object
     */
    virtual exception* clone() const;

    /**
     * Create new instance of this object
     */
    virtual exception* newInstance() const;

    /**
     * Alias for copy operator
     */
    exception& operator=(const exception& other);

  protected:
    /**
     * The exception message.
     *
     * This string will be returned with the method what() and is set at
     * construction time.
     */
    std::string exceptionName_;
  };

}

#endif

