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
 * \file   ltiContainer.h
 *         Contains the parent class to all container classes like vectors and
 *         matrices.
 * \author Pablo Alvarado
 * \date   01.04.1999
 *
 * $Id: ltiContainer.h,v 1.4 2005-07-12 16:37:30 gquiros Exp $
 */

#ifndef _LTI_CONTAINER_H_
#define _LTI_CONTAINER_H_

#include "ltiIoObject.h"

namespace lti {

  /**
   * Abstract base class to all container classes
   *
   * Derived classes must provide:
   * - The name() method (from lti::object)
   * - The clone() and newInstance() methods (from lti::object)
   * - The read(ioHandler,const bool complete) method
   * - The write(ioHandler,const bool complete) const method
   *
   * It is highly recommended that the derived classes define the type
   * value_type.
   */
  class container : public ioObject {
  public:

    enum {
    
      /**
       * Maximum allowed index
       */
      MaxIndex = 0x7FFFFFFF,
      
      /**
       * Minimum allowed index
       */
      MinIndex = -0x7FFFFFFF
    
    };
    
    /**
     * Clone method
     */
    container* clone() const = 0;

    /**
     * New instance
     */
    container* newInstance() const = 0;
  };
}

#endif

