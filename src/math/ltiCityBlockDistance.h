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
 * \file   ltiCityBlockDistance.h
 *         This file contains the function T cityBlockDistance(const
 *         T&, const T&)
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiCityBlockDistance.h,v 1.1 2005-03-29 13:26:44 doerfler Exp $
 */

#ifndef _LTI_CITY_BLOCK_DISTANCE_H_
#define _LTI_CITY_BLOCK_DISTANCE_H_

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
   * @name City Block Distances Functions
   * global functions to compute the L1 distance between two n-dimensional
   * point representations
   *
   * @ingroup gLinearAlgebra
   */
  //@{

  /**
   * Default implementation of the L1 distance between two values of
   * type T. T must define the operator- and T must be castable to
   * distanceType<T>::distance_type, which in turn must be a possible
   * argument of lti::abs().
   *
   * Overloads for most LTI-Lib types are implemented.
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockDistance(const T& a, const T& b) {
    return abs(static_cast<typename distanceType<T>::distance_type>(a)
               -static_cast<typename distanceType<T>::distance_type>(b));
  };

  /**
   * cityBlockDistance computes the L1 distance between 
   * the vectors a and b.
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockDistance(const vector<T>& a,const vector<T>& b) {
    typename vector<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::distance_type sum(0);
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      sum+=
        lti::abs(static_cast<typename distanceType<T>::distance_type>(*ita)
                 -static_cast<typename distanceType<T>::distance_type>(*itb));
    }
    return sum;
  };

  /**
   * cityBlockDistance computes the L1 distance between 
   * the lti::matrix a and b.
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockDistance(const matrix<T>& a,const matrix<T>& b) {
    assert(a.size() == b.size());
    
    typename matrix<T>::const_iterator ait,bit,eit;
    typename distanceType<T>::distance_type dist(0);
    ait = a.begin();
    bit = b.begin();
    eit = a.end();
    
    while (ait != eit) {
      dist += 
        lti::abs(static_cast<typename distanceType<T>::distance_type>(*ait)-
                 static_cast<typename distanceType<T>::distance_type>(*bit));
      ++ait;
      ++bit;
    }
    return dist;
  };


  /**
   * cityBlockDistance computes the L1 distance between 
   * the std::vectors a and b.
   */
  template<class T>  
  typename distanceType<T>::distance_type
  cityBlockDistance(const std::vector<T>& a,const std::vector<T>& b) {
    typename std::vector<T>::const_iterator ita,itb,ite;
    assert(a.size()==b.size());
    typename distanceType<T>::distance_type sum(0);
    for (ita=a.begin(),itb=b.begin(),ite=a.end();
         ita!=ite;
         ++ita,++itb) {
      sum+=
        lti::abs(static_cast<typename distanceType<T>::distance_type>(*ita)
                 -static_cast<typename distanceType<T>::distance_type>(*itb));
    }
    return sum;
  };

  /**
   * cityBlockDistance computes the L1 distance between 
   * the points a and b.
   */
  template<class T >  
  typename distanceType<T>::distance_type
  cityBlockDistance(const point<T>& a,const point<T>& b) {
    return (lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.x)-b.x)+
            lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.y)-b.y));
  };

  /**
   * cityBlockDistance computes the L1 distance between 
   * the points a and b.
   */
  template<class T>
  typename distanceType<T>::distance_type
  cityBlockDistance(const point3D<T>& a, const point3D<T>& b) {
    return (lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.x)-b.x)+
            lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.y)-b.y)+
            lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.z)-b.z));
  };

  /**
   * cityBlockDistance computes the L1 distance between 
   * the RGB values a and b in the RGB color space.
   */
  template<class T>
  inline typename distanceType<T>::distance_type
  cityBlockDistance(const rgbPixel<T>& a, const rgbPixel<T>& b) {
    return (lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.red)-b.red)+
            lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.green)-b.green)+
            lti::abs(static_cast<typename distanceType<T>::distance_type>
                     (a.blue)-b.blue));
  };

  /**
   * cityBlockDistance computes the L1 distance between 
   * the RGB values a and b in the RGB color space.
   */
  inline distanceType<rgbaPixel>::distance_type
  cityBlockDistance(const rgbaPixel& a, const rgbaPixel& b) {
    return 
      (lti::abs(static_cast<distanceType<rgbaPixel>::distance_type>
                (a.red)-b.red)+
       lti::abs(static_cast<distanceType<rgbaPixel>::distance_type>
                (a.green)-b.green)+
       lti::abs(static_cast<distanceType<rgbaPixel>::distance_type>
                (a.blue)-b.blue));
  };

  //@}


}

#endif

