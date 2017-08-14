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
 * \file   ltiSort_template.h
 *         Sort the elements in a matrix or vector
 * \author Pablo Alvarado
 * \date   17.08.2000
 *
 * $Id: ltiSort_template.h,v 1.12 2008-08-06 03:40:58 alvarado Exp $
 */

namespace lti {

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<T>!
  template <class T>
  bool sort::apply(matrix<T>& srcdest) const {
    vector<T> tmpvct;
    tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                         &srcdest.at(0,0),
                         ConstantReference);

    return apply(tmpvct);
  }

  // On place apply for type vector<T>!
  template <class T>
  bool sort::apply(vector<T>& srcdest) const {

    quicksort(srcdest,0,srcdest.lastIdx());

    return true;
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool sort::apply(const matrix<T>& src,matrix<T>& dest) const {

    dest.copy(src);
    return apply(dest);
  }

  // On copy apply for type vector<T>!
  template <class T>
  bool sort::apply(const vector<T>& src,vector<T>& dest) const {

    dest.copy(src);
    return apply(dest);
  }

  template<class T>
  void sort::insertionsortAsc(vector<T>& vct,
                              const int begin,
                              const int end) const {
    T a;
    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(a < vct.at(i))) {
          break;
        }
        vct.at(i+1)=vct.at(i);
      }
      vct.at(i+1)=a;
    }
  }

  template<class T>
  void sort::insertionsortDesc(vector<T>& vct,
                               const int begin,
                               const int end) const {
    T a;
    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(vct.at(i) < a)) {
          break;
        }
        vct.at(i+1)=vct.at(i);
      }
      vct.at(i+1)=a;
    }
  }

  template<class T>
  void sort::quicksort(vector<T>& vct,
                       const int begin,
                       const int end) const {

    int i;

    if (order_ == Ascending) {

      if ((end-begin) > thresholdForBubble_) {
        i = partitionAsc(vct,begin,end);
        quicksort(vct,begin,i-1);
        quicksort(vct,i+1,end);
      } else {
        insertionsortAsc(vct,begin,end);
      }
    } else { // Descending order

      if (end-begin > thresholdForBubble_) {
        i = partitionDesc(vct,begin,end);
        quicksort(vct,begin,i-1);
        quicksort(vct,i+1,end);
      } else {
        insertionsortDesc(vct,begin,end);
      }
    }
  }

  template<class T>
  int sort::partitionDesc(vector<T>& vct,
                             const int begin,
                             const int end) const {

    int i,j;
    T v,t;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (v < vct.at(++i)) {
      }
      while ( (--j >= 0) && (vct.at(j) < v) ) {
      }
      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;
    }
    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    return i;
  }

  template<class T>
  int sort::partitionAsc(vector<T>& vct,
                            const int begin,
                            const int end) const {

    int i,j;
    T v,t;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct.at(++i) < v) {
      }

      while ( (--j >= 0) && (v < vct.at(j)) ) {
      }

      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;
    }
    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    return i;
  }

}


