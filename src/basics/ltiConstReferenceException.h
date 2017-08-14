/*
 * Copyright (C) 1999-2006
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
 * \file   ltiConstReferenceException.h
 *         Specialized exception for unallowed access to const object
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiConstReferenceException.h,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#ifndef _LTI_CONST_REFERENCE_EXCEPTION_H_
#define _LTI_CONST_REFERENCE_EXCEPTION_H_

#include "ltiException.h"

namespace lti {

  /**
   * constReferenceException class
   *
   * constReferenceException is thrown if you try to modify a constant
   * object.  A constant object is an object created with the constant
   * attribute.
   *
   * @see lti::genericVector::useExternData()
   *      lti::genericVector::genericVector(const int,T*,const bool)
   */
  class constReferenceException : public exception {
  public:
    constReferenceException();
    virtual const std::string& name() const;
    virtual constReferenceException* clone() const;
    virtual constReferenceException* newInstance() const;
  };

}  

#endif
