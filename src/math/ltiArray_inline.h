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


/* 
 * \file   ltiArray_inline.h
 *         Contains the template class array.
 *
 * \author Pablo Alvarado
 * \date   09.04.99
 *
 * $Id: ltiArray_inline.h,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */

namespace lti {

  template<typename T>
  inline void array<T>::resize(const int from, const int to) {
    resize(from,to,T(),Copy);
  }

  template<typename T>
  inline void array<T>::allocate(const int from, const int to) {
    resize(from,to,T(),AllocateOnly);
  }

  template<typename T>
  inline void array<T>::assign(const int from,
                               const int to,
                               const T& initValue) {
    resize(from,to,initValue,Init);
  }


}
