/*
 * Copyright (C) 2006
 * Pablo Alvarado, Instituto Tecnológico de Costa Rica
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
 * \file   ltiAllocException.h
 *         Defines the memory allocation exception
 * \author Pablo Alvarado
 * \date   15.09.2006
 *
 * $Id: ltiAllocException.h,v 1.1 2007-04-04 19:18:15 alvarado Exp $
 */

#ifndef _LTI_ALLOC_EXCEPTION_H
#define _LTI_ALLOC_EXCEPTION_H

#include "ltiException.h"

namespace lti {

  /**
   * allocException is thrown if a memory allocation problem occurs
   */
  class allocException : public exception  {
  public:
    /**
     * Construct allocation exception with the default string name
     * "memory allocation error"
     */
    allocException();

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Clone this object
     */
    virtual allocException* clone() const;

    /**
     * Create new instance of this object
     */
    virtual allocException* newInstance() const;
  };
}

#endif

