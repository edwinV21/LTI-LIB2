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
 * \file   ltiKdTree_inline.h
 *         This file contains the aggregate type kdTree, a data
 *         structure for fast nearest neighbor search
 * \author Frederik Lange
 * \author Pablo Alvarado
 * \author Jens Rietzschel
 * \date   09.01.2003
 *
 * $Id: ltiKdTree_inline.h,v 1.3 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiDebug.h"

namespace lti {
  
  //------------------------------------------------------------------
  // kdTree::element
  //------------------------------------------------------------------

  template <typename T, typename D, class U>
  inline typename kdTree<T,D,U>::value_type& 
  kdTree<T,D,U>::element::operator[](const int a) {
    return point[a];
  }

  template <typename T, typename D, class U>
  inline const typename kdTree<T,D,U>::value_type& 
  kdTree<T,D,U>::element::operator[](const int a) const {
    return point[a];
  }

  template <typename T, typename D, class U>
  inline int kdTree<T,D,U>::element::size() const {
    return point.size();
  }

  template <typename T, typename D, class U>
  inline typename kdTree<T,D,U>::element& 
  kdTree<T,D,U>::element::copy(const element& other) {
    point=other.point;
    data=other.data;
    return *this;
  }

  template <typename T, typename D, class U>
  inline typename kdTree<T,D,U>::element&
  kdTree<T,D,U>::element::operator=(const element& other) {
    return copy(other);
  }

  //------------------------------------------------------------------
  // kdTree::node
  //------------------------------------------------------------------

  template <typename T, typename D, class U>
  inline void kdTree<T,D,U>::node::add(element* f) {
    points.push_back(f);
  }

  template <typename T, typename D, class U>
  inline bool kdTree<T,D,U>::node::isLeaf() const {
    return (!points.empty()); 
  }

  template <typename T, typename D, class U>
  inline typename kdTree<T,D,U>::node& 
  kdTree<T,D,U>::node::operator=(const node& other) {
    return copy(other);
  }

  /*
   * deep clear points
   */
  template<class T,class D,class U>
  inline void kdTree<T,D,U>::node::clearPoints() {
    // delete the elements
    typename points_type::iterator it,eit;
    for (it=points.begin(),eit=points.end();it!=eit;++it) {
      delete (*it);
      (*it)=0;
    }

    points.clear();
  }

  template<class T,class D,class U>
  inline bool kdTree<T,D,U>::node::getPoint(const T& key,element& elem) const {
    if (!points.empty()) {
      typename points_type::const_iterator it,eit;
      for (it=points.begin(),eit=points.end();it!=eit;++it) {
        if ((*it)->point == key) {
          elem=*(*it);
          return true;
        }
      }
    }
    return false;
  }

  template<class T,class D,class U>
  inline bool kdTree<T,D,U>::node::getPoint(const T& key,
                                            std::list<element>& elems) const {
    bool found=false;
    if (!points.empty()) {
      typename points_type::const_iterator it,eit;
      for (it=points.begin(),eit=points.end();it!=eit;++it) {
        if ((*it)->point == key) {
          elems.push_back(*(*it));
          found=true;
        }
      }
    }
    return found;
  }
  
  template<class T,class D,class U>
  inline void kdTree<T,D,U>::node::add(std::list<element*>& pts) {
    typename std::list<element*>::iterator it=pts.begin();
    while (it!=pts.end()) {
      points.push_back(*it);
      ++it;
    }
  }

  //------------------------------------------------------------------
  // kdTree
  //------------------------------------------------------------------
    
  /*
   * check if the given key point and the given bounding box
   */ 
  template<class T,class D,class U>
  inline bool
  kdTree<T,D,U>::checkHypersphereWithinBounds(const T& key,
                                              const matrix<value_type>& bounds,
                                              const distance_type& dist) const {
    
    _lti_debug4("  Check sphere within bounds. " << std::endl <<
                "    key   : " << key << std::endl <<
                "    bounds: " << bounds << std::endl <<
                "    dist  : " << dist << std::endl);
    
    value_type k;
    int i(key.size()-1);
    const vector<value_type>& boundsMin = bounds.getRow(0);
    const vector<value_type>& boundsMax = bounds.getRow(1);
    while (i>=0) {
      if ( (distantor_.component(k=key[i],boundsMin.at(i)) <= dist) ||
           (distantor_.component(boundsMax.at(i),k) <= dist) ) {
        _lti_debug4("false." << std::endl << std::endl);
        return false;
      }
      --i;
    }
    _lti_debug4("true." << std::endl << std::endl);
    return true;
  }

  /*
   * check if the bounds overlap the hypersphere.  It was called in
   * the original paper "bounds overlap ball".
   */
  template<class T,class D,class U>
  inline bool 
  kdTree<T,D,U>::checkBoundsOverlapHypersphere(const T& key,
                                              const matrix<value_type>& bounds,
                                              const distance_type& dist) const {

    _lti_debug4("  Check bounds overlap sphere. " << std::endl <<
                "    key   : " << key << std::endl <<
                "    bounds: " << bounds << std::endl <<
                "    dist  : " << dist << std::endl);

    distance_type sum(0);
    int i(key.size()-1);
    const vector<value_type>& boundsMin = bounds.getRow(0);
    const vector<value_type>& boundsMax = bounds.getRow(1);
    value_type k;
    while (i>=0) {
      k = key[i];
      if (k < boundsMin.at(i)) {
        // lower than low boundary
        distantor_.accumulate(boundsMin.at(i),k,sum);
        if (distantor_.accGreaterThan(sum,dist)) {
          _lti_debug4("false." << std::endl << std::endl);
          return false;
        }
      } else if (k > boundsMax.at(i)) {
        // higher than high boundary
        distantor_.accumulate(k,boundsMax.at(i),sum);
        if (distantor_.accGreaterThan(sum,dist)) {
          _lti_debug4("false." << std::endl << std::endl);
          return false;
        }
      }
      --i;
    }

    _lti_debug4("true." << std::endl << std::endl);
    return true;
  }
  
  /*
   * get the square of the Euclidean distance of the given point to
   * the node's hyperbox.
   *
   * If the point lies withing the hyperbox, the distance will be zero.
   *
   * @param indexPoint the point to be compared with the hyperbox.
   * @param na hyperbox representing the node's covered subspace.
   *           The first row represents the minimum and the second the
   *           maximum values.  It must be na.columns()==indexPoint.size()
   */
  template<class T,class D,class U>
  inline typename kdTree<T,D,U>::distance_type
  kdTree<T,D,U>::minDistancePointToBox(const T& indexPoint,
                                       const matrix<value_type>& na) const {
    
    distance_type dist(0);
    value_type tmp;

    const vector<value_type>& boxMin = na.getRow(0);
    const vector<value_type>& boxMax = na.getRow(1);
    
    for (int i = indexPoint.size()-1; i >= 0; --i) {
      if ((tmp=indexPoint[i]) > boxMax.at(i)) {
        distantor_.accumulate(tmp,boxMax.at(i),dist);
      } else if (tmp < boxMin.at(i)) {
        distantor_.accumulate(tmp,boxMin.at(i),dist);
      }
    }
    
    return distantor_.computeDistance(dist);
  }
  
  template<class T,class D,class U>
  inline bool kdTree<T,D,U>::withinBox(const T& boxMin,
                                       const T& boxMax,
                                       const T& key) const {
    const int sz = key.size();
    int i;
    for (i=0; i<sz; ++i) {
      const typename T::value_type k=key[i];
      if ( (k<boxMin[i]) || (k>boxMax[i]) ) {
        return false;
      }
    }
    return true;
  }

  template<class T,class D,class U>
  inline bool kdTree<T,D,U>::withinBox(const matrix<value_type>& bbox,
                                       const T& boxMin,
                                       const T& boxMax) const {
    const int sz = bbox.columns();
    int i;
    const vector<value_type>& low  = bbox.getRow(0);
    const vector<value_type>& high = bbox.getRow(1);
    for (i=0; i<sz; ++i) {
      if ( (boxMin[i]<low.at(i)) || (boxMax[i]>high.at(i)) ) {
        return false;
      }
    }
    return true;
  }

}
