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
 * \file   ltiConstantReferenceType.h
 *         Contains the enum eConstantReferece which provides global
 *         constants to control the behaviour of memory allocation. The
 *         appropriate read and write functions are also defined.
 * \author Pablo Alvarado
 * \date   09.09.2006
 * $Id: ltiConstantReferenceType.h,v 1.3 2007-04-18 21:13:40 alvarado Exp $
f */

#ifndef _LTI_CONSTANT_REFERENCE_H_
#define _LTI_CONSTANT_REFERENCE_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * The constant reference type is used to indicate if the memory allocated
   * by vectors, matrices and tensors can be modified or not.
   *
   * It is used to avoid that the subtensors (or vectors) of tensors (or
   * matrices) modified the memory they use, because it is allocated outside.
   *
   * @ingroup gTypes
   */
  enum eConstantReference {
    VariableReference, /**< Referece is allowed to be modified */
    ConstantReference  /**< Reference cannot be modified */
  };
  
  /**
   * Read function for eResizeType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eConstantReference& data);

  /**
   * Write function for eResizeType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eConstantReference& data);

  
}
#endif
