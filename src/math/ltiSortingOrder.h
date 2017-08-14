/*
 * Copyright (C) 1998-2005
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
 * \file   ltiSortingOrder.h
 *         Contains the enum eSortingOrder for specifying sorting
 *         order. IO functions are provided as well.
 * \author Peter Doerfler
 * \date   06.04.2005
 *
 * $Id: ltiSortingOrder.h,v 1.1 2005-04-26 15:44:01 doerfler Exp $
 */

#ifndef _LTI_SORTING_ORDER
#define _LTI_SORTING_ORDER

#include "ltiIoHandler.h"

namespace lti {

  /**
   * Type used to specify the sorting order
   */
  enum eSortingOrder {
    Ascending, /*!< ascending order  */
    Descending /*!< descending order */
  };

  /**
   * read function for eSortingOrder.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eSortingOrder& data);

  /**
   * write function for eSortingOrder.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eSortingOrder& data);

}

#endif
