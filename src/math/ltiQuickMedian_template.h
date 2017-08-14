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
 * $Id: ltiQuickMedian_template.h,v 1.10 2012-09-16 05:22:59 alvarado Exp $
 */


namespace lti {

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // the real algorithm
  template<class V>
  typename V::value_type quickMedian::findMedian(V& vct,
                                                 const int begin,
                                                 const int end,
                                                 const int medianPos) const {
    int pivot;

    pivot=partition(vct,begin,end);
    if(pivot==medianPos) {
      return vct[pivot];
    } else if(pivot>medianPos) {
      return findMedian(vct,begin,pivot-1,medianPos);
    } else if(pivot<medianPos) {
      return findMedian(vct,pivot+1,end,medianPos);
    }

    return  typename V::value_type();
  }

  // partition algorithm (see quickSort)
  template<class V>
  int quickMedian::partition(V& vct,
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
  bool quickMedian::apply(genericMatrix<T>& srcdest, T& med) const {
    if (srcdest.getMode() == genericMatrix<T>::Connected) {
      genericVector<T> tmpvct;
      tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                           &srcdest.at(0,0),
                           ConstantReference);
      return apply(tmpvct, med);
    } else {
      // create a connected matrix first
      genericMatrix<T> tmp(srcdest); // this is a connected matrix
      return apply(tmp,med);
    }
    return false;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool quickMedian::apply(const genericMatrix<T>& src, T& med) const {
    genericMatrix<T> tmp(src);
    return apply(tmp, med);
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool quickMedian::apply(const genericMatrix<T>& src,
                          genericMatrix<T>& dest,
                             T& med) const {
    dest.copy(src);
    return apply(dest, med);
  }


  // On place apply for type vector<T>!
  template <class V>
  bool quickMedian::applyVector(V& srcdest, typename V::value_type& med) const {
    int medianPos;
    const parameters& param = getParameters();
    
    if (srcdest.empty()) {
      return false; // the empty set has no median!
    }

    if ((srcdest.size() & 0x1) != 0) {
      // odd
      medianPos = srcdest.size()/2;
    } else {
      // even
      if (param.medianEvenCase == TakeLower) {
        medianPos = srcdest.size()/2 - 1 ;
      } else {
        medianPos = srcdest.size()/2 ;
      }
    }
    
    med = findMedian(srcdest,0,srcdest.size()-1,medianPos);

    return true;
  }

  // On place apply for type vector<T>!
  template <class V>
  bool quickMedian::applyVector(const V& src, 
                                typename V::value_type& med) const {
    V tmp(src);
    return apply(tmp, med);
  }

  // On place apply for type vector<T>!
  template <class V>
  bool quickMedian::applyVector(const V& src,V& dest,
                                typename V::value_type& med) const {
    dest.copy(src);
    return apply(dest, med);
  }

  // shortcut function
  template <class V>
  typename V::value_type quickMedian::medianVector(const V& src) const {
    typename V::value_type tmp = typename V::value_type();
    apply(src,tmp);
    return tmp;
  }

  /*
   * Find the median of a vector of type T, which can be an lti::genericVector
   * or its derived classes.
   *
   * The resulting vector contains the elements less or equal than the median
   * for the indexes <code>x</code> such that <code>x < size()/2</code>,
   * and higher or equal otherwise.
   *
   * @param srcdest vector<T> with the source data.  The result
   *                 will be left here too.
   * @param medianVal  the median value
   *
   * @return true on success false otherwise
   */
  template <class T>
  bool quickMedian::apply(genericVector<T>& srcdest, T& medianVal) const {
    return applyVector(srcdest,medianVal);
  }

  /*
   * Operates on the given parameter.
   * @param src vector with the source data.
   * @param median the median value
   * @return true on success false otherwise
   */
  template <class T>
  bool quickMedian::apply(const genericVector<T>& src, T& medianVal) const {
    return applyVector(src,medianVal);
  }
  
  /*
   * Operates on the given parameter.
   * @param src vector with the source data.
   * @param dest the partially sorted vector.  The elements at the
   *             first half of the vector are less or equal than the median
   *             and on the other half greater or equal.
   * @param median  the median value
   * @return true on success false otherwise
   */
  template <class T>
  bool quickMedian::apply(const genericVector<T>& src, genericVector<T>& dest,
                          T& medianVal) const {
    return applyVector(src,dest,medianVal);
  }

  /*
   * A shortcut function for apply(const vector<T>&, T&) const. Note
   * that internally another vector and T are used.
   *
   * @param src vector whose median is sought
   * @returns the median of \a src
   */
  template <class T>
  T quickMedian::median(const genericVector<T>& src) const {
    return medianVector(src);
  }

  /*
   * With STL vectors
   */

  template <class T>
  bool quickMedian::apply(std::vector<T>& srcdest, T& medianVal) const {
    return applyVector(srcdest,medianVal);
  }

  template <class T>
  bool quickMedian::apply(const std::vector<T>& src, T& medianVal) const {
    return applyVector(src,medianVal);
  }
  
  template <class T>
  bool quickMedian::apply(const std::vector<T>& src, std::vector<T>& dest,
                          T& medianVal) const {
    return applyVector(src,dest,medianVal);
  }

  template <class T>
  T quickMedian::median(const std::vector<T>& src) const {
    return medianVector(src);
  }



}
