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
 * \file   ltiQuickMedian_template.h
 *         This file contains the functor quickMedian, which
 *         calculates the median quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickMedian2_template.h,v 1.4 2012-09-16 05:22:59 alvarado Exp $
 */


namespace lti {


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  template <typename V,typename W>
  typename V::value_type quickMedian2::findMedian(V& vct,
                                                  W& data,
                                                  const int begin,
                                                  const int end,
                                                  const int medianPos) const {
    int pivot;

    pivot=partition(vct,data,begin,end);
    if(pivot==medianPos) {
      return vct[pivot];
    } else if(pivot>medianPos) {
      return findMedian(vct,data,begin,pivot-1,medianPos);
    } else if(pivot<medianPos) {
      return findMedian(vct,data,pivot+1,end,medianPos);
    }

    return  typename V::value_type();
  }

  // partition algorithm (see quickSort)
  template <typename V,typename W>
  int quickMedian2::partition(V& vct,
                              W& data,
                              const int begin,
                              const int end) const {
    
    int i,j;
    typename V::value_type v,t;
    typename W::value_type d;

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

      d=data[i];
      data[i]=data[j];
      data[j]=d;
    }
    t=vct[i];
    vct[i]=vct[end];
    vct[end]=t;

    d=data[i];
    data[i]=data[end];
    data[end]=d;

    return i;
  }


  // On place apply for type vector<T>!
  template <class V,class W>
  bool quickMedian2::apply(V& keys,W& data,
                           typename V::value_type& median) const {
    assert(static_cast<int>(keys.size())==static_cast<int>(data.size()));

    int medianPos;
    const parameters& param = getParameters();
    
    if ((keys.size() % 2) != 0) {
      // odd
      medianPos = keys.size()/2;
    } else {
      // even
      if ( (param.medianEvenCase == TakeLower) && (!keys.empty()) ) {
        medianPos = keys.size()/2 - 1 ;
      } else {
        medianPos = keys.size()/2 ;
      }
    }
    
    median = findMedian(keys,data,0,keys.size()-1,medianPos);
    return true;
  }

  // On place apply for type vector<T>!
  template <class V,class W>
  bool quickMedian2::apply(V& keys,W& data) const {
    typename V::value_type tmp;
    return apply(keys, data, tmp);
  }

}
