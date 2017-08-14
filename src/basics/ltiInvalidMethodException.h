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
 * \file   ltiInvalidMethodException.h
 *         Contains the class lti::invalidMethodException, this is thrown by
 *         classes derived from lti::functor.
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   14.04.1999
 *
 * $Id: ltiInvalidMethodException.h,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#ifndef _LTI_INVALID_METHOD_EXCEPTION_H_
#define _LTI_INVALID_METHOD_EXCEPTION_H_

#include "ltiException.h"

namespace lti {

  /**
   * Exception thrown when a member function of a functor that is required by
   * the base class it is derived from is not implemented.
   *
   * \b Note: All occurences of this exception should be eliminated from the
   * library as they indicate either a missing implementation or a flawed
   * design.
   */
  class invalidMethodException : public exception {
  public:
    /**
     * Default constructor
     */
    invalidMethodException();

    /**
     * Constructor with alternative message
     */
    invalidMethodException(const std::string& str);

    /**
     * Constructor with alternative message
     */
    invalidMethodException(const char* str);

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the class.
     */
    virtual invalidMethodException* clone() const;

    /**
     * Returns a pointer to a clone of the class.
     */
    virtual invalidMethodException* newInstance() const;
  };

}

#endif
