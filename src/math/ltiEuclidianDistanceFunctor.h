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
 * \file   ltiEuclidianDistanceFunctor.h
 *         This file contains the class euclidiandistanceFunctor, 
 *         a class for calculating the L2 distance between two vectors or
 *         matrices.
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiEuclidianDistanceFunctor.h,v 1.1 2005-03-29 13:26:46 doerfler Exp $
 */

#ifndef _LTI_EUCLIDIAN_DISTANCE_FUNCTOR_H_
#define _LTI_EUCLIDIAN_DISTANCE_FUNCTOR_H_

#include "ltiDistanceFunctor.h"
#include "ltiMath.h"

namespace lti {

  /**
   * This class is the base class for all functors which compute
   * distances between two vectors or matrices.
   *
   * @see similarityFunctor
   *
   * Be careful with the use of the parameters::rowWise.  It indicates
   * if the matrix should be considered as having row vectors (true) of
   * columns vectors (false).  Depending on that the computations will be
   * very different.
   */
  template <typename T>
  class euclidianDistanceFunctor : public distanceFunctor<T> {
  public:

    /**
     * The parameters of euclidianDistanceFunctor are equivalent to
     * those of distanceFunctor
     */
    typedef typename distanceFunctor<T>::parameters parameters;

    /**
     * default constructor
     */
    euclidianDistanceFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    euclidianDistanceFunctor(const euclidianDistanceFunctor<T>& other);

    /**
     * destructor
     */
    virtual ~euclidianDistanceFunctor();


    /**
     * calculate the distance between the vectors a and b. By default
     * uses the distance member function.
     *
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dist the distance between the vectors
     * @return false on error -> see status string
     */
    virtual bool apply(const vector<T>& a, const vector<T>& b,
                       T& dist) const;

    /**
     * calculate the distances between the rows or columns of the
     * matrices a and b, determined by the parameters rowWise.
     * By default uses the distance() member function.
     *
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dists the distances between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       vector<T>& dists) const;

    /**
     * Calculate the distance between each row or column of m
     * depending on the value of rowWise and the vector v.
     * By default uses the distance() member function.
     *
     * @param m the matrix<T>
     * @param v the vector to be compared with
     * @param dest the vector with the distances to the vector v
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, const vector<T>& v,
                       vector<T>& dest) const;

    /**
     * calculate something like the distance between the matrices a and b:
     * both matrices are seen as vectors.
     * By default uses the distance() member function.
     *
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @param dist the 'distance' between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       T& dist) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    euclidianDistanceFunctor<T>& 
    copy(const euclidianDistanceFunctor<T>& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual euclidianDistanceFunctor<T>* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual euclidianDistanceFunctor<T>* newInstance() const;

    /**
     * Return the name of the class 
     */
    virtual const std::string& name() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };

}

// #include "ltiEuclidianDistanceFunctor_template.h"

#endif


