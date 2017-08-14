/*
 * Copyright (C) 1998-2005
 * Peter Doerfler
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
 * \file   ltiBresenhamCircle.h
 *         Contains the functor bresenhamCircle.
 * \author Peter Doerfler
 * \date   30.07.2005
 *
 * revisions ..: $Id: ltiBresenhamCircle.h,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_BRESENHAM_CIRCLE_H_
#define _LTI_BRESENHAM_CIRCLE_H_

#include "ltiFunctor.h"
#include "ltiGenericMatrix.h"
#include "ltiPointList.h"
#include "ltiArray.h"

namespace lti {

  /**
   * Functor for rendering circle segments with the Bresenham circle
   * drawing algorithm.
   * 
   * The apply methods take the first and last points of the circle
   * segment, and an object where to store the points. Currently two
   * types of point stores are supported:
   *
   * - genericMatrix<T>, in which points are drawn with a given value of T
   * - ipointList, in which points are stored in the list
   * 
   * Additional point stores may be added, see the addPoint() method.
   * 
   */
  class bresenhamCircle : public functor {
  public:
    /**
     * The parameters for the class bresenhamCircle
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * \param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * \param other the parameters object to be copied
       * \return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * \param other the parameters object to be copied
       * \return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * \param handler the ioHandler to be used
       * \param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * \return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * \param handler the ioHandler to be used
       * \param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * \return true if write was successful
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
     * Default constructor
     */
    bresenhamCircle();

    /**
     * Construct a functor using the given parameters
     */
    bresenhamCircle(const parameters& par);

    /**
     * Copy constructor
     * \param other the object to be copied
     */
    bresenhamCircle(const bresenhamCircle& other);

    /**
     * Destructor
     */
    virtual ~bresenhamCircle();

    /**
     * Render a circle with center \a (cx,cy) and radius \a radius.
     *
     * The circle will be drawn to the given genericMatrix<T> instance,
     * using the given color. 
     *
     * @param cx x-coord of center
     * @param cy y-coord of center
     * @param radius radius of the circle
     * @param m matrix the circle is drawn into
     * @param color color the circle is drawn in
     */
    template<typename T>
    inline bool apply(const int cx, const int cy,
                      const int radius,
                      genericMatrix<T>& m, T color) const;
    
    /**
     * Render a circle with center \a (cx,cy) and radius \a radius.
     *
     * The point will be added to the given pointlist \a l.
     *
     * @param cx x-coord of center
     * @param cy y-coord of center
     * @param radius radius of the circle
     * @param l pointlist the points are added to
     */
    inline bool apply(const int cx, const int cy,
                      const int radius,
                      ipointList& l) const;

    /**
     * Render a circle with center \a center and radius \a radius.
     *
     * The circle will be drawn to the given genericMatrix<T> instance,
     * using the given color. 
     *
     * @param center center of the circle
     * @param radius radius of the circle
     * @param m matrix the circle is drawn into
     * @param color color the circle is drawn in
     */
    template<typename T>
    inline bool apply(const ipoint& center, const int radius,
                      genericMatrix<T>& m, T color) const;

    /**
     * Render a circle with center \a center and radius \a radius.
     *
     * The point will be added to the given pointlist \a l.
     *
     * @param center center of the circle
     * @param radius radius of the circle
     * @param l pointlist the points are added to
     */
    inline bool apply(const ipoint& center, const int radius,
                      ipointList& l) const;
    
    /**
     * Render a circle with radius \a radius.
     *
     * The outmost x-coordinates of each y-coordinate will be added to
     * the given array \a ioPts. This array can then be used as io
     * points for sampling a circle area.
     *
     * @param radius radius of the circle
     * @param ioPts array containing io coordinates
     */
    inline bool apply(const int radius, array<int>& ioPts) const;

    /**
     * Copy data of "other" functor.
     * \param other the functor to be copied
     * \return a reference to this functor object
     */
    bresenhamCircle& copy(const bresenhamCircle& other);

    /**
     * Alias for copy member
     * \param other the functor to be copied
     * \return a reference to this functor object
     */
    bresenhamCircle& operator=(const bresenhamCircle& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bresenhamCircle* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bresenhamCircle* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    ensure there, that these attributes are properly initialized.

  private:
    
    /**
     * Implementation of the Bresenham circle rendering algorithm.
     * This method uses addPoint to store the produced points
     * in the given point store.
     */ 
    template<typename POINT_STORE, typename T>
    bool renderCircle(const int cx, const int cy, const int rad, 
                      POINT_STORE& store, T color) const;
    
    /**
     * Adds the given point plus symmetric points to the given point
     * list.
     */
    template<typename T>
    inline void addPoints(const int cx, const int cy,
                          const int x, const int y, 
                          ipointList& l, T unused) const;
    
    /**
     * Adds the given point plus symmetric points to the given iarray.
     */
    template<typename T>
    inline void addPoints(const int cx, const int cy,
                          const int x, const int y, 
                          array<int>& arr, T unused) const;
    
    /**
     * Draws the given point plus symmetric points to the given matrix
     * with the given color.
     */
    template<typename T>
    inline void addPoints(const int cx, const int cy,
                          const int x, const int y,
                          genericMatrix<T>& m, T color) const;
    
    /**
     * Sets the point \a (x,y) in \a m to \a color if 0<=x<mx 
     * and 0<=y<m<.
     */
    template<typename T>
    inline void setPoint(const int x, const int y,
                         const int mx, const int my,
                         genericMatrix<T>& m, T color) const;
    
  };
}

#include "ltiBresenhamCircle_inline.h"

/* The following include is necesary, so that the renderCircle method
   gets instantiated for every instantiation of genericMatrix<T> as a
   point store. Otherwise, we would need to manually instantiate each
   apply for every T in genericMatrix<T>.
*/
#include "ltiBresenhamCircle_template.h"  

#endif

