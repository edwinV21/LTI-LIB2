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
 * \file   ltiEuclidianDistantor.h
 *         This file contains the policy classes euclidianDistantor
 *         and euclidianSqrDistantor, which calculate the L2 distance
 *         and it square, respecively
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiEuclidianDistantor.h,v 1.1 2005-03-29 13:26:46 doerfler Exp $
 */

#ifndef _LTI_EUCLIDIAN_DISTANTOR_H_
#define _LTI_EUCLIDIAN_DISTANTOR_H_

#include "ltiDistanceType.h"
#include "ltiEuclidianDistance.h"


namespace lti {


  /**
   * Policy class used by several classifiers/trees to measure the square
   * of the euclidian distance between two points of type T.
   *   
   * The type T MUST define the \a value_type type, which exist for example
   * for vectors, (t)points and (t)rgbPixels.
   *
   * If you \e really want, you can use this class directly through the
   * operator().
   *
   * \code
   * euclidianSqrDistantor< vector<double> > myDist;
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = myDist(a,b);
   * \endcode
   * 
   * But remember that there is also a global template function distanceSqr()
   * which is easier to use:
   *
   * \code
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = distanceSqr(a,b);
   * \endcode
   *
   * You can of course also use the lti::l2Distance functor, as usual with its
   * apply() methods.
   */
  template <class T, class D=typename distanceType<T>::square_distance_type>
  class euclidianSqrDistantor {
  public:
    /**
     * type returned by the distantor
     */
    typedef D distance_type;

    /**
     * compute the distance between a and b
     */
    inline distance_type operator()(const T& a,const T& b) const {
      return 
        static_cast<distance_type>(euclidianDistanceSqr(a,b));
    };

    /**
     * @name Special methods for Minkowski distances
     */
    //@{

    /**
     * This member accumulates in the given accumulator, the
     * given element.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the euclidianSqrDistantor this is just acc+=(elem*elem).
     *
     * @param element component of the difference between two points.
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const distance_type element,
                           distance_type& accumulator) const {
      accumulator+=(element*element);
    }

    /**
     * This member accumulates in the given accumulator, the difference 
     * of the given elements.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the euclidianSqrDistantor this is just acc+=((elem1-elem2)^2).
     *
     * @param element1 component of the first point
     * @param element2 component of the second point
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const typename T::value_type element1,
                           const typename T::value_type element2,
                           distance_type& accumulator) const {
      const distance_type tmp = static_cast<distance_type>(element2-element1);
      accumulator+=(tmp*tmp);
    }

    /**
     * Compute from the given accumulator the desired distance
     */
    inline distance_type 
    computeDistance(const distance_type& accumulator) const {
      return accumulator;
    }

    /**
     * return the distance between two components, which is in some way
     * a component of the total distance (that is the reason for the name).
     *
     * For this distantor it return (element2-element1)^2
     */
    inline distance_type 
    component(const typename T::value_type element1,
              const typename T::value_type element2) const {
      const distance_type tmp = static_cast<distance_type>(element2-element1);
      return tmp*tmp;
    }

    /**
     * Return true if the given partial computed from accumulator is less than
     * the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is less than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc < dist
     */
    inline bool accLessThan(const distance_type acc,
                            const distance_type dist) const {
      return (acc < dist);
    }
    
    /**
     * Return true if the given partial computed from accumulator is greater
     * than the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is greater than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc > dist
     */
    inline bool accGreaterThan(const distance_type acc,
                               const distance_type dist) const {
      return (acc > dist);
    }
    //@}
  };

  /**
   * Policy class used by several classifiers/trees to measure the 
   * euclidian distance between two points of type T.
   *   
   * The type T MUST define the \a value_type type, which exist for example
   * for vectors, (t)points and (t)rgbPixels.
   *
   * If you \e really want, you can use this class directly through the
   * operator().
   *
   * \code
   * euclidianDistantor< vector<double> > myDist;
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = myDist(a,b);
   * \endcode
   * 
   * But remember that there is also a global template function distanceSqr()
   * which can be more easily used:
   *
   * \code
   * dvector a,b;
   * ... // fill vectors with data
   * double dist = sqrt(distanceSqr(a,b));
   * \endcode
   *
   * You can of course also use the lti::l2Distance functor, as usual with its
   * apply() methods.
   */
  template <class T, class D=typename distanceType<T>::fp_distance_type>
  class euclidianDistantor {
  public:
    /**
     * type returned by the distantor
     */
    typedef D distance_type;

    /**
     * compute the distance between a and b
     */
    inline distance_type operator()(const T& a,const T& b) const {
      return static_cast<distance_type>(euclidianDistance(a,b));
    }

    /**
     * @name Special methods for Minkowski distances
     */
    //@{

    /**
     * This member accumulates in the given accumulator, the
     * given element.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the euclidianDistantor this is just acc+=(elem*elem).
     *
     * @param element component of the difference between two points.
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const distance_type element,
                           distance_type& accumulator) const {
      accumulator+=(element*element);
    }

    /**
     * This member accumulates in the given accumulator, the difference 
     * of the given elements.
     *
     * It can be used when the distance need to be computed manually, but
     * using a distantor to still allow the flexibility of changing distances.
     *
     * For the euclidianDistantor this is just acc+=((elem2-elem1)^2).
     *
     * @param element1 component of the first point
     * @param element2 component of the second point
     * @param accumulator variable where the elements will be accumulated.
     */
    inline void accumulate(const typename T::value_type element1,
                           const typename T::value_type element2,
                           distance_type& accumulator) const {
      const distance_type tmp = static_cast<distance_type>(element2-element1);
      accumulator+=(tmp*tmp);
    }

    /**
     * Compute from the given accumulator the desired distance
     */
    inline distance_type 
    computeDistance(const distance_type& accumulator) const {
      return sqrt(accumulator);
    }
 
    /**
     * return the distance between two components, which is in some way
     * a component of the total distance (that is the reason for the name).
     *
     * For this distantor it return abs(element2-element1)
     */
    inline distance_type 
    component(const typename T::value_type element1,
              const typename T::value_type element2) const {
      return static_cast<distance_type>(abs(element2-element1));
    }

    /**
     * Return true if the given partial computed from accumulator is less than
     * the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is less than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc < (dist*dist)
     */
    inline bool accLessThan(const distance_type acc,
                            const distance_type dist) const {
      return (acc < (dist*dist));
    }
    
    /**
     * Return true if the given partial computed from accumulator is greater
     * than the given distance.
     *
     * Assume you have accumulated \a acc until now, and you want to check if
     * the partial distance derived from this accumulator is less than
     * the given distance.  So you check accLessThan(accumulator,distance)
     * 
     * For this norm it computes acc > (dist*dist)
     */
    inline bool accGreaterThan(const distance_type acc,
                               const distance_type dist) const {
      return (acc > (dist*dist));
    }
    //@}
  };

}

#endif
