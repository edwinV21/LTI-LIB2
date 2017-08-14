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
 * \file   ltiDataRepresentationType.h
 *         Contains the enum eDataRepresentationType.
 * \author Jochen Wickel
 * \author Peter Doerfler
 * \date   25.09.2000
 *
 * $Id: ltiDataRepresentationType.h,v 1.1 2005-07-19 18:23:25 gquiros Exp $
 */

#ifndef _LTI_DATA_REPRESENTATION_TYPE_H_
#define _LTI_DATA_REPRESENTATION_TYPE_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * Determines how a data sample is represented when a matrix is
   * given. In the LTI-Lib a row in a matrix usually corresponds to
   * a data sample or point. Choose Rows for this
   * behaviour. However, you can also choose Columns for the
   * opposite.
   */
  enum eDataRepresentationType {
    Rows,   /**< Data samples are in each row of a matrix */
    Columns /**< Data samples are in each column of a matrix */
  };

  /**
   * read function for eDataRepresentationType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eDataRepresentationType& data);

  /**
   * write function for eDataRepresentationType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eDataRepresentationType& data);

}

#endif
