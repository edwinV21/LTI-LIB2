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
 * \file   ltiIncompatibleDimensionsException.h
 *         Contains the class lti::ltiIncompatibleDimensionsException which is
 *         thrown by linear algrebra functors.
 * \author Thomas Rusert
 * \date   01.06.1999
 * 
 * $Id: ltiIncompatibleDimensionsException.h,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#ifndef _LTI_INCOMPATIBLE_DIMENSIONS_EXCEPTION_H_
#define _LTI_INCOMPATIBLE_DIMENSIONS_EXCEPTION_H_

#include "ltiException.h"

namespace lti {

  /**
   * Incompatible dimensions exception
   */
  class incompatibleDimensionsException : public exception {
  public:

    /**
     * Default constructor
     */
    incompatibleDimensionsException();

    /**
     * Constructor with alternative message \p str
     */
    incompatibleDimensionsException(const char* str);

    /**
     * Constructor with alternative message \p str
     */
    incompatibleDimensionsException(const std::string& str);

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the exception
     */
    virtual incompatibleDimensionsException* clone() const;

    /**
     * Returns a pointer to a new instance of the exception
     */
    virtual incompatibleDimensionsException* newInstance() const;
  };

}

#endif
