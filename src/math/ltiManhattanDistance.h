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
 * \file   ltiManhattanDistance.h
 *         This file contains the functions T manhattanDistance(const
 *         T&, const T&) which calculate the Manhattan distance.
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiManhattanDistance.h,v 1.1 2008-03-25 17:02:18 alvarado Exp $
 */

#ifndef _LTI_MANHATTAN_DISTANCE_H_
#define _LTI_MANHATTAN_DISTANCE_H_

#include <vector>
#include "ltiPoint.h"
#include "ltiPoint3D.h"
#include "ltiRGBPixel.h"
#include "ltiRGBAPixel.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiDistanceType.h"

namespace lti {
   
  /**
   * @name Manhattan Distances Functions
   *
   * Global functions to compute the L1 distance between two n-dimensional
   * point representations
   *
   * @ingroup gLinearAlgebra
   */
  //@{

  /**
   * Default implementation of the L1 distance between two values of
   * type T. It is assumed that T is scalar and an efficient
   * implementation is used. T must define the operator- and T must be
   * castable to distanceType<T>::square_distance_type, which in turn must be
   * a possible argument of lti::abs().
   *
   * Overloads for most LTI-Lib types are implemented.
   */
  template<class T>  
  typename distanceType<T>::square_distance_type
  manhattanDistance(const T& a, const T& b) {
    return abs(static_cast<typename distanceType<T>::square_distance_type>(a)
              -static_cast<typename distanceType<T>::square_distance_type>(b));
  }

  /**
   * The manhattanDistance computes the L1 distance between 
   * the vectors a and b.
   */
  template<class T>  
  typename distanceType<T>::square_distance_type
  manhattanDistance(const vector<T>& a,const vector<T>& b) {
    typename vector<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::square_distance_type sum(0);
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      sum+=
        abs(static_cast<typename distanceType<T>::square_distance_type>(*ita)
           -static_cast<typename distanceType<T>::square_distance_type>(*itb));
    }
    return sum;
  }

  /**
   * The manhattanDistance computes the L1 distance between 
   * the lti::matrix a and b.
   */
  template<class T>  
  typename distanceType<T>::square_distance_type
  manhattanDistance(const matrix<T>& a,const matrix<T>& b) {
    assert(a.size() == b.size());
    
    typename matrix<T>::const_iterator ait,bit,eit;
    typename distanceType<T>::square_distance_type dist(0);
    ait = a.begin();
    bit = b.begin();
    eit = a.end();
    
    while (ait != eit) {
      dist+=
        abs(static_cast<typename distanceType<T>::square_distance_type>(*ait)
           -static_cast<typename distanceType<T>::square_distance_type>(*bit));

      ++ait;
      ++bit;
    }
    return dist;
  }


  /**
   * The manhattanDistance computes the L1 distance between 
   * the std::vectors a and b.
   */
  template<class T>  
  typename distanceType<T>::square_distance_type
  manhattanDistance(const std::vector<T>& a,const std::vector<T>& b) {
    typename std::vector<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::square_distance_type sum(0);
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      sum+=
        abs(static_cast<typename distanceType<T>::square_distance_type>(*ita)
           -static_cast<typename distanceType<T>::square_distance_type>(*itb));
    }
    return sum;
  }

  /**
   * The manhattanDistance computes the L1 distance between 
   * the points a and b.
   */
  template<class T >  
  typename distanceType<T>::square_distance_type
  manhattanDistance(const point<T>& a,const point<T>& b) {
    return abs(a.x-b.x)+abs(a.y-b.y);
  }

  /**
   * The manhattanDistance computes the L1 distance between 
   * the points a and b.
   */
  template<class T>
  typename distanceType<T>::square_distance_type
  manhattanDistance(const point3D<T>& a, const point3D<T>& b) {
    return abs(a.x-b.x)+abs(a.y-b.y)+abs(a.z-b.z);
  }

  /**
   * The manhattanDistance computes the L1 distance between 
   * the RGB values a and b in the RGB color space.
   */
  template<class T>
  inline typename distanceType<T>::square_distance_type
  manhattanDistance(const rgbPixel<T>& a, const rgbPixel<T>& b) {
    typedef typename distanceType<T>::square_distance_type typ;
    return 
      abs(static_cast<typ>(a.red)-static_cast<typ>(b.red)) +
      abs(static_cast<typ>(a.green)-static_cast<typ>(b.green)) +
      abs(static_cast<typ>(a.blue)-static_cast<typ>(b.blue));
  }

  /**
   * The manhattanDistance computes the L1 distance between 
   * the RGB values a and b in the RGB color space.
   */
  inline distanceType<rgbaPixel>::square_distance_type
  manhattanDistance(const rgbaPixel& a, const rgbaPixel& b) {
    typedef distanceType<rgbaPixel>::square_distance_type typ;
    return 
      abs(static_cast<typ>(a.red)  -static_cast<typ>(b.red)) +
      abs(static_cast<typ>(a.green)-static_cast<typ>(b.green)) +
      abs(static_cast<typ>(a.blue) -static_cast<typ>(b.blue));
  }

  //@}

}

#endif
