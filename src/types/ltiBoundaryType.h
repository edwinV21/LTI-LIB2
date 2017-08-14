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
 * \file   ltiBoundaryType.h
 *         Contains the enum eBoundaryType which provides global
 *         constants for differnt boudary conditions. The appropriate
 *         read and write functions are also defined.
 * \author Peter Doerfler
 * \date   02.04.2003
 * $Id: ltiBoundaryType.h,v 1.2 2007-04-18 21:13:40 alvarado Exp $
 */

#ifndef _LTI_BOUNDARYTYPE_H_
#define _LTI_BOUNDARYTYPE_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * boundary condition type.  This type indicates how to
   * consider/interpret the boundaries of an image or matrix.
   *
   * For this type the methods read(ioHandler&, eBoundaryType&) and
   * write(ioHandler&, const eBoundaryType&) are implemented. Note
   * that template functions read(ioHandler&, const std::string&, T&)
   * and write(ioHandler&, const std::string&, const T&) exist. (see
   * Developer's Guide for further information).
   *
   * @ingroup gTypes
   */
  enum eBoundaryType {
    Zero,      /**< The boundary does not exist (only 0-value) */
    Mirror,    /**< The signal/image/vector is mirrored */
    Periodic , /**< A periodic continuation of the signal is assumed */
    Constant,  /**< The last value of the border will be assumed */
    NoBoundary /**< The boundary will be ignored. The effect is
                  an unitialized border in those parts of the vector,
                  matrix or image, where a complete filter kernel cannot
                  be applied due to its size.
                  Please note that the border will be left UNinitialized.
                  It is up to you to set some value at this border,
                  which will have a maximum height or width of the size
                  of the filter kernel minus one.  The reason for this
                  option to exist is that some algorithms will ignore
                  this border anyway and all other border types will
                  spend some time calculating it */
  };
  
  /**
   * read function for eBoundaryType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eBoundaryType& data);

  /**
   * write function for eBoundaryType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eBoundaryType& data);

  
}
#endif
