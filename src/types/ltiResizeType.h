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
 * \file   ltiResizeType.h
 *         Contains the enum eResizeType which provides global
 *         constants to control the behaviour of resize methods. The
 *         appropriate read and write functions are also defined.
 * \author Peter Doerfler
 * \date   29.11.2004
 * $Id: ltiResizeType.h,v 1.5 2007-04-18 21:13:40 alvarado Exp $
 */

#ifndef _LTI_RESIZETYPE_H_
#define _LTI_RESIZETYPE_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * The resize type defines the behaviour of resize member functions found in
   * many containers and other data structures. In most cases the default is
   * Copy, to be compliant with the STL.  Different functions: allocate() and
   * assign() are shortcuts for resize with OnlyAllocate or Init.
   *
   * For this type the methods read(ioHandler&, eResizeType&) and
   * write(ioHandler&, const eResizeType&) are implemented. Note
   * that template functions read(ioHandler&, const std::string&, T&)
   * and write(ioHandler&, const std::string&, const T&) exist. (see
   * Developer's Guide for further information).
   *
   * @ingroup gTypes
   */
  enum eResizeType {
    AllocateOnly, /**< Memory is allocated only, old values are not copied to 
                       the new container and new fields are not initialized */
    Copy,         /**< Old values are copied to the resized container but 
                       possibly existing new elements are not initialized */
    Init ,        /**< The resized container is completely filled with the
                       given init value.*/
    CopyAndInit   /**< The old data is copied to the resized container and new 
                       elements (if existent) are initialized with the 
                       init value*/
  };
  
  /**
   * Read function for eResizeType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eResizeType& data);

  /**
   * Write function for eResizeType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eResizeType& data);

  
}
#endif
