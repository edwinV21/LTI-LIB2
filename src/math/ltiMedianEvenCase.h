/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiMedianEvenCase.h
 *         Contains the enum eMedianCase which is used in
 *         lti::quickMedian and related classes. Read/write functions
 *         are available.
 * \author Peter Doerfler
 * \date   23.03.2005
 *
 * $Id: ltiMedianEvenCase.h,v 1.1 2005-03-29 13:26:46 doerfler Exp $
 */

#ifndef _LTI_MEDIAN_EVEN_CASE_H
#define _LTI_MEDIAN_EVEN_CASE_H

#include "ltiIoHandler.h"

namespace lti {

  /**
   * The median value of even length vectors is undefined. It can
   * either be the value before or after half the length. This enum is
   * used to specify which behaviour is desired: TakeLower or
   * TakeHigher.
   */
  enum eMedianEvenCase {
    TakeLower, /**< take the element with the lower index, i.e. N/2-1 with
                *   N the number of elements in the vector
                */
    TakeHigher /**< take the element with the higher index, i.e. N/2 with
                *   N the number of elements in the vector
                */
  };

  /**
   * read function for eMedianEvenCase.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eMedianEvenCase& data);

  /**
   * write function for eMedianEvenCase.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eMedianEvenCase& data);
  
}

#endif
