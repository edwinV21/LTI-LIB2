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

/*
 * \file   ltiQuickPartialSort_template.h
 *         This file contains the functor quickMedian, which
 *         calculates the median quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickPartialSort_template.h,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */


namespace lti {

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // the real algorithm
  template<class V> 
  typename V::value_type quickPartialSort::findNth(V& vct,
                                                   const int begin,
                                                   const int end,
                                                   const int pos) const {
    int pivot;

    pivot=partition(vct,begin,end);
    if(pivot==pos) {
      return vct[pivot];
    } else if(pivot>pos) {
      return findNth(vct,begin,pivot-1,pos);
    } else if(pivot<pos) {
      return findNth(vct,pivot+1,end,pos);
    }

    return  typename V::value_type();
  }

  // partition algorithm (see quickSort)
  template<class V>
  int quickPartialSort::partition(V& vct,
                                  const int begin,
                                  const int end) const {
    
    int i,j;
    typename V::value_type v,t;

    v = vct[end];
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct[++i] < v) {
      }
      while ( (--j >= 0) && (v < vct[j]) ) {
      }
      if ( i >= j ) {
        break ;
      }
      t=vct[i];
      vct[i]=vct[j];
      vct[j]=t;
    }
    t=vct[i];
    vct[i]=vct[end];
    vct[end]=t;

    return i;
  }


  // On place apply for type matrix<T>!
  template <typename T>
  bool quickPartialSort::apply(const int row,const int col,
                               matrix<T>& srcdest, T& med) const {
    if (srcdest.getMode() == matrix<T>::Connected) {
      vector<T> tmpvct;
      tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                           &srcdest.at(0,0),
                           true);
      return apply(row*srcdest.columns()+col,tmpvct, med);
    } else {
      // create a connected matrix first
      matrix<T> tmp(srcdest); // this is always a connected matrix
      return apply(tmp,med);
    }
    return false;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool quickPartialSort::apply(const int row,const int col,
                               const matrix<T>& src, T& med) const {
    matrix<T> tmp(src);
    return apply(row,col,tmp, med);
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool quickPartialSort::apply(const int row,const int col,
                               const matrix<T>& src, matrix<T>& dest,
                               T& med) const {
    dest.copy(src);
    return apply(row,col,dest, med);
  }


  // On place apply for type vector<T>!
  template <class V>
  bool quickPartialSort::apply(const int pos,
                               V& srcdest, 
                               typename V::value_type& med) const {
    med = findNth(srcdest,0,srcdest.size()-1,pos);
    return true;
  }

  // On place apply for type vector<T>!
  template <class V>
  bool quickPartialSort::apply(const int pos, const V& src, 
                               typename V::value_type& med) const {
    V tmp(src);
    return apply(pos,tmp, med);
  }

  // On place apply for type vector<T>!
  template <class V>
  bool quickPartialSort::apply(const int pos, const V& src, V& dest,
                          typename V::value_type& med) const {
    dest.copy(src);
    return apply(pos,dest, med);
  }

  // shortcut function
  template <class V> typename V::value_type
  quickPartialSort::nth(const int pos,const V& src) const {
    typename V::value_type tmp;
    apply(pos,src,tmp);
    return tmp;
  }


}
