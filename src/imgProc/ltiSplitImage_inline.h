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
 * \file   ltiSplitImage_inline.h
 *         Implementation of special min() and max() functions 
 * \author Peter Doerfler
 * \date   02.08.2004
 *
 * $Id: ltiSplitImage_inline.h,v 1.3 2005-04-28 15:35:06 doerfler Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_INLINE_H_
#define _LTI_SPLIT_IMAGE_INLINE_H_


namespace lti {

  // returns the minimum of three integers
  template <typename T>
  inline T splitImage::minimum(const T a,const T b,const T c) const {
    return ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c));
  }

  // returns the maximum of three integers
  template <typename T>
  inline T splitImage::maximum(const T a,const T b, const T c) const {
    return ((a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c));
  }

}

#endif
