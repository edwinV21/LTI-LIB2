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
 * \file   ltiLocalSampling_inline.h
 *         Contains inlined function definitions for the 
 *         class lti::siftSampling
 * \author Arnd Hannemann
 * \date   18.04.2005
 *
 * revisions ..: $Id: ltiLocalSampling_inline.h,v 1.3 2009-08-30 04:30:29 alvarado Exp $
 */

#ifndef _LTI_LOCAL_SAMPLING_INLINE_H_
#define _LTI_LOCAL_SAMPLING_INLINE_H_

#include "ltiConstants.h"

namespace lti {

  // clipAngle helper function (does nothing at all see specialization)
  template <class T>
  inline void localSampling::clipAngle(T&, const float) const {
  }

  // specialization for float only then clipping makes sense
  template <>
  inline void localSampling::
  clipAngle(float& val, const float angle) const {
    val -= angle;
    while (val < 0) {
      val += constants<float>::twoPi();
    }
    while (val > constants<float>::twoPi()) {
      val -= constants<float>::twoPi();
    }
  }

} // namespace lti

#endif
