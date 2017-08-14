/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiScramble.cpp
 *         Contains the class scramble,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   09.10.2007
 *
 * revisions ..: $Id: ltiScramble_template.h,v 1.3 2010-04-22 04:58:26 alvarado Exp $
 */

namespace lti {

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type matrix<T>!
  template<typename T>
  bool scramble::apply(matrix<T>& srcdest) {
    vector<T> tmpvct;
    tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                         &srcdest.at(0,0),
                         true);
    return apply(tmpvct);
  }

  // On place apply for type vector<T>!
  template<typename T>
  bool scramble::apply(vector<T>& srcdest) {

    if (srcdest.empty()) {
      // nothing to do
      return true;
    }

    int i,j;
    int k;
    if (max_ != srcdest.lastIdx()) {
      max_ = srcdest.lastIdx();
      rnd_.setInterval(0,max_);
    }

    const int sz = (srcdest.size() + 1)/2;

    for (k=0; k<sz; k++) {

      // generate two random numbers.
      i=rnd_.rand();
      j=rnd_.rand();

      swap(srcdest.at(i),srcdest.at(j));
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template<typename T>
  bool scramble::apply(const matrix<T>& src, matrix<T>& dest) {
    dest.copy(src);
    return apply(dest);
  }

  // On copy apply for type vector<T>!
  template<typename T>
  bool scramble::apply(const vector<T>& src, vector<T>& dest) {
    dest.copy(src);
    return apply(dest);
  }

}

