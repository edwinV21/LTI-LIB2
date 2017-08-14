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
 * \file   ltiTriangularMatrixType.h
 *         Contains the enumeration eTriangularMatrixType which defines
 *         the global constants Lower and Upper. The corresponding 
 *         read/write functions are provided as well
 * \author Peter Doerfler
 * \date   07.07.2003
 * 
 * $Id: ltiTriangularMatrixType.h,v 1.4 2005-01-03 16:17:52 alvarado Exp $
 */

#include "ltiIoObject.h"

#ifndef _LTI_TRIANGULAR_MATRIX_TYPE_H_
#define _LTI_TRIANGULAR_MATRIX_TYPE_H_

namespace lti {
    
  /**
   * In the LTI-Lib triangular matrices are stored in regular
   * matrices with an eTriangularMatrixType indicating whether its upper
   * or lower triangular.
   */
  enum eTriangularMatrixType {
    Upper, /**< the data matrix is upper triangular*/ 
    Lower  /**< the data matrix is lower triangular*/
  };

  /**
   * write function for eTriangularMatrixType
   * @param handler the ioHandler to be used
   * @param ttype the eTriangularMatrixType to be written
   * @return true if write was successful
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const eTriangularMatrixType& ttype);

  /**
   * write function for eTriangularMatrixType
   * @param handler the ioHandler to be used
   * @param ttype the eTriangularMatrixType to be written
   * @return true if write was successful
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, eTriangularMatrixType& ttype);
  
}

#endif
