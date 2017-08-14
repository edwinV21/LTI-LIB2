/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiConvexHull.h
 *         Contains the functor convexHull
 * \author Pablo Alvarado
 * \date   16.11.2002
 *
 * $Id: ltiConvexHull.h,v 1.5 2006-01-18 12:05:43 doerfler Exp $
 */

#ifndef _LTI_CONVEX_HULL_H_
#define _LTI_CONVEX_HULL_H_

#include "ltiFunctor.h"
#include "ltiPolygonPoints.h"

namespace lti {
  /**
   * Computes the convex hull of a set of points.
   *
   * The algorithm used is similar to the one explained in:
   *
   * M. de Berg, et. al. Computational Geometry. Algorithms and
   * Applications, 2nd. edition. Springer, 2000
   */
  class convexHull : public functor {
  public:
    /**
     * the parameters for the class convexHull
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
      ~parameters();

      /**
       * returns name of this type
       */
      const std::string& name() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


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
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


    };

    /**
     * default constructor
     */
    convexHull();

    /**
     * Construct a functor using the given parameters
     */
    convexHull(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    convexHull(const convexHull& other);

    /**
     * destructor
     */
    virtual ~convexHull();

    /**
     * returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const ipointList& src,ipolygonPoints& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const ioPoints& src,ipolygonPoints& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const pointList<float>& src,polygonPoints<float>& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const pointList<double>& src,
                     polygonPoints<double>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    convexHull& copy(const convexHull& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    convexHull& operator=(const convexHull& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual convexHull* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual convexHull* newInstance() const;
    
    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    /**
     * Creates the smallest convex polygon that contains all points
     * in the given point list.
     *
     * The list of points \b must be a set, i.e. the same point is not
     * allowed to be twice in the list.  (This usually is not the case
     * in lti::ioPoints lists, see castFrom(const ioPoints&)).
     *
     * For more information on the algorithm used here see:
     *
     * M. de Berg, et.al. Computational Geometry, Algorithms and Applications.
     * 2nd. edition, Springer, 2000, pp. 6ff
     *
     * @param thePointList a set of points (the same point is not allowed to
     *                     be twice in the list).
     * @param thePolygonPoints the resulting polygon 
     * @return a reference to the result
     */
    template<typename T>
    polygonPoints<T>& computeConvexHull(const pointList<T>& thePointList,
                                        polygonPoints<T>& thePolygonPoints) const;
    
  };
}

#endif

