/*
 * Copyright (C) 1998-2004
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
 * \file   ltiDistanceType.h
 *         This file contains the class distanceType<T> which specifies three
 *         types that are recommended for the result of a distance calculation
 *         between two instances of T. The three types are:
 *         - \c distance_type is used when a simple distance is calculated
 *           between two instances of T which does not involve accumulation of
 *           squares or typical floating point operations.
 *         - \c square_distance_type is used for the result of distances
 *           that involve the square of the type T.
 *         - \c fp_distance_type is used for distances that involve floating
 *           point operations such as the sqrt or log functions.
 * \author Peter Doerfler
 * \date   11.05.2003
 * $Id: ltiDistanceType.h,v 1.3 2007-04-18 21:13:40 alvarado Exp $
 */

#ifndef _LTI_DISTANCE_TYPE_H_
#define _LTI_DISTANCE_TYPE_H_

#include "ltiObject.h"
#include "ltiTypeInfo.h"

namespace lti {

  /**
   * This class defines the appropriate return types when calculating
   * the distance between two values of type T. These can be simple
   * types like \c int or \c double or \c lti types like \c vector<T>, 
   * rgbaPixel, etc. There are three different distance types:
   * - \c distance_type is used when a simple distance is calculated
   *   between two instances of T which does not involve accumulation of
   *   squares or typical floating point operations.
   * - \c square_distance_type is used for the result of distances
   *   that involve the square of the type T.
   * - \c fp_distance_type is used for distances that involve floating
   *   point operations such as the sqrt or log functions.
   *
   * @ingroup gTypes
   */
  template<class T>
  class distanceType {
  public:
    /**
     * Suggested distance type for distances that use simple
     * accumulation of type elements (for example l1Distantor).
     */
    typedef typename distanceType<typename T::value_type>::distance_type
    distance_type;

    /**
     * Suggested distance type for distances that use
     * accumulation of squares of type elements (for example l2SqrDistantor).
     */
    typedef typename distanceType<typename T::value_type>::square_distance_type
    square_distance_type;

    /**
     * Suggested distance type for distances that use accumulation of
     * type elements or their squares and use a typical floating point (fp)
     * operation like sqrt or log on that. (for example l2Distantor).
     */
    typedef typename distanceType<typename T::value_type>::fp_distance_type
    fp_distance_type;

  private:
    /**
     * Disable constructor of instances of this class
     */ 
    distanceType() {};
  };

  // ------------------------
  // template specializations
  // ------------------------

  template<>
  class distanceType<ubyte> {
  public:
    typedef typeInfo<ubyte>::accumulation_type distance_type;
    typedef typeInfo<ubyte>::square_accumulation_type square_distance_type;
    typedef float fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<byte> {
  public:
    typedef typeInfo<byte>::accumulation_type distance_type;
    typedef typeInfo<byte>::square_accumulation_type square_distance_type;
    typedef float fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<short int> {
  public:
    typedef typeInfo<short int>::accumulation_type distance_type;
    typedef typeInfo<short int>::square_accumulation_type square_distance_type;
    typedef float fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<unsigned short int> {
  public:
    typedef typeInfo<unsigned short int>::accumulation_type distance_type;
    typedef typeInfo<unsigned short int>::square_accumulation_type 
    square_distance_type;
    typedef float fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<int> {
  public:
    typedef typeInfo<int>::accumulation_type distance_type;
    typedef typeInfo<int>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<unsigned int> {
  public:
    typedef typeInfo<unsigned int>::accumulation_type distance_type;
    typedef typeInfo<unsigned int>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<float> {
  public:
    typedef typeInfo<float>::accumulation_type distance_type;
    typedef typeInfo<float>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  private:
    distanceType() {};
  };

  template<>
  class distanceType<double> {
  public:
    typedef typeInfo<double>::accumulation_type distance_type;
    typedef typeInfo<double>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  private:
    distanceType() {};
  };

}


#endif

