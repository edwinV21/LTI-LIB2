/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiPolygonPoints_inline.h
 * \author Ruediger Weiler
 * \date   06.12.2000
 * 
 * $Id: ltiPolygonPoints_inline.h,v 1.4 2012-11-17 14:49:04 alvarado Exp $
 */

#ifndef _LTI_POLYGON_POINTS_INLINE_H
#define _LTI_POLYGON_POINTS_INLINE_H

namespace lti {
  
  /**
   * assigment operator (alias for copy(other)).
   * @param other the pointList to be copied
   * @return a reference to the actual pointList
   */
  template<typename T>
  inline polygonPoints<T>& polygonPoints<T>::operator=(const polygonPoints<T>& other) {
    this->copy(other);
    return *this;
  }
  
}

#endif
