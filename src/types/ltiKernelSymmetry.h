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
 * \file   ltiKernelSymmetry.h
 *         Contains the enum kernelsymmetry.
 * \author Bodo Hoffmann
 * \date   11.07.2004
 *
 * revisions ..: $Id: ltiKernelSymmetry.h,v 1.1 2010-04-24 23:49:42 alvarado Exp $
 */
#ifndef _LTI_KERNEL_SYMMETRY_H_
#define _LTI_KERNEL_SYMMETRY_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * Kernel symmetry.
   *
   * This enumeration type specifies what kind of symmetry a kernel has.
   *
   * For this type the methods read(ioHandler&, eKernelSymmetry&) and
   * write(ioHandler&, const eKernelSymmetry&) are implemented. Note
   * that template functions read(ioHandler&, const std::string&, T&)
   * and write(ioHandler&, const std::string&, const T&) exist. (see
   * Developer's Guide for further information).
   */
  enum eKernelSymmetry {
    Default,   /**< There are no known symmetries found.*/
    Asymmetric,/**< Left part of kernel has negativ value of right part*/
    Symmetric  /**< Left part of kernel is equal to right part*/
  };
  
  /**
   * Read function for eKernelSymmetry.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eKernelSymmetry& data);

  /**
   * Write function for eKernelSymmetry.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eKernelSymmetry& data);

  
}
#endif
