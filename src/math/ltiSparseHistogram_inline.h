/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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
 * \file   ltiSparseHistogram_inline.h
 * \author Jochen Wickel
 * \date   30.10.2001
 *
 * $Id: ltiSparseHistogram_inline.h,v 1.2 2006-06-06 09:42:39 doerfler Exp $
 */

namespace lti {

  inline const float& sparseHistogram::at(const dvector& x) const {
    return at(convertIndex(x));
  }

  inline float& sparseHistogram::at(const dvector& x) {
    return at(convertIndex(x));
  }
  
  inline int sparseHistogram::dimensions() const {
    return bins.size();
  }
  
  inline sparseHistogram::const_iterator sparseHistogram::begin() const {
    return const_iterator(core.begin());
  }
  
  inline sparseHistogram::iterator sparseHistogram::begin() {
    return iterator(core.begin());
  }
  
  inline sparseHistogram::const_iterator sparseHistogram::end() const {
    return const_iterator(core.end());
  }

  inline sparseHistogram::iterator sparseHistogram::end() {
    return iterator(core.end());
  }

  inline float sparseHistogram::get(const dvector& index) const {
    return get(convertIndex(index));
  }

  inline void sparseHistogram::put(const dvector& index, float value) {
    put(convertIndex(index),value);
  }

  inline void sparseHistogram::add(const dvector& index, float value) {
    add(convertIndex(index),value);
  }

  inline void sparseHistogram::multiply(const dvector& index, const float& value) {
    multiply(convertIndex(index),value);
  }
  
}
