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
 * \file   ltiDistanceFunctor.h
 *         This file contains the class distanceFunctor, the base
 *         class for calculation of distances between two vectors or
 *         matrices.
 * \author Jochen Wickel
 * \date   28.06.2000
 *
 * $Id: ltiDistanceFunctor.h,v 1.2 2005-06-22 15:26:41 doerfler Exp $
 */

#ifndef _LTI_DISTANCE_FUNCTOR_H_
#define _LTI_DISTANCE_FUNCTOR_H_

#include "ltiFunctor.h"
#include "ltiMath.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

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
  class distanceFunctor: public functor {
  public:
    /**
     * the parameters for the class distanceFunctor
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Return the name of the class 
       */
      virtual const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete) const;

      /**
       * read the parametersfrom the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,
                const bool complete);

      /**
       * determine whether distances between the row vectors in a
       * matrix (true) or the column vectors in it (false) should be
       * calculated.
       *
       * Default value: true
       */
      bool rowWise;

    };

    /**
     * default constructor
     */
    distanceFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    distanceFunctor(const distanceFunctor<T>& other);

    /**
     * destructor
     */
    virtual ~distanceFunctor();

    /**
     * returns the distance between the vectors a and b. This is a
     * shortcut for apply(const vector<T>&, const vector<T>&, T&).
     *
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the distance between the vectors
     */
    virtual T distance(const vector<T>& a, const vector<T>& b) const;

    /**
     * returns something like the distance between the matrices a and
     * b: both matrices are seen as vectors. This is a shortcut for
     * apply(const matrix<T>&, const matrix<T>&, T&).
     *
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @return the 'distance' between the matrices
     */
    virtual T distance(const matrix<T>& a, const matrix<T>& b) const;

    /**
     * calculate the distance between the vectors a and b.
     *
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dist the distance between the vectors
     * @return false on error -> see status string
     */
    virtual bool apply(const vector<T>& a, const vector<T>& b,
                       T& dist) const=0;

    /**
     * calculate the distances between the rows or columns of the
     * matrices a and b, determined by the parameter rowWise.
     *
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dists the distances between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       vector<T>& dists) const=0;

    /**
     * Calculate the distance between each row or column of m
     * depending on the value of rowWise and the vector v.
     *
     * @param m the matrix<T>
     * @param v the vector to be compared with
     * @param dest the vector with the distances to the vector v
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, const vector<T>& v,
                       vector<T>& dest) const=0;

    /**
     * calculate something like the distance between the matrices a and b:
     * both matrices are seen as vectors.
     *
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @param dist the 'distance' between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       T& dist) const=0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    distanceFunctor<T>& copy(const distanceFunctor<T>& other);

    /**
     * Return the name of the class 
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual distanceFunctor<T>* clone() const=0;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual distanceFunctor<T>* newInstance() const=0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };

}

#include "ltiDistanceFunctor_template.h"

#endif


