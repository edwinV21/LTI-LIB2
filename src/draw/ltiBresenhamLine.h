/*
 * Copyright (C) 1998-2005
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
 * \file   ltiBresenhamLine.h
 *         Contains the functor bresenhamLine.
 * \author Gustavo Quiros
 * \date   18.3.2005
 *
 * revisions ..: $Id: ltiBresenhamLine.h,v 1.5 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_BRESENHAM_LINE_H_
#define _LTI_BRESENHAM_LINE_H_

#include "ltiFunctor.h"
#include "ltiGenericMatrix.h"
#include "ltiPointList.h"

namespace lti {

  /**
   * Functor for rendering line segments with the Bresenham line
   * drawing algorithm.
   * 
   * The apply methods take the first and last points of the line
   * segment, and an object where to store the points. Currently two
   * types of point stores are supported:
   *
   * - genericMatrix<T>, in which points are drawn with a given value of T
   * - ipointList, in which points are stored in the list
   * 
   * Additional point stores may be added, see the addPoint() method.
   * 
   */
  class bresenhamLine : public functor {
  public:
    /**
     * The parameters for the class bresenhamLine
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
    bresenhamLine();

    /**
     * Construct a functor using the given parameters
     */
    bresenhamLine(const parameters& par);

    /**
     * Copy constructor
     * \param other the object to be copied
     */
    bresenhamLine(const bresenhamLine& other);

    /**
     * Destructor
     */
    virtual ~bresenhamLine();

    /**
     * Render a line from the point (fx,fy) to point (tx,ty).
     * The line will be drawn to the given genericMatrix<T> instance,
     * using the given color. 
     */
    template<typename T>
    inline bool apply(const int fx, const int fy,
                      const int tx, const int ty,
                      genericMatrix<T>& m, T color) const;
    
    /**
     * Render a line from the point (fx,fy) to point (tx,ty).
     * The points in the line will be added to the given list.
     */
    inline bool apply(const int fx, const int fy,
                      const int tx, const int ty,
                      ipointList& l) const;
    
    /**
     * Render a line from the point f to point t.
     * The line will be drawn to the given genericMatrix<T> instance,
     * using the given color.
     */
    template<typename T>
    inline bool apply(const ipoint& from, const ipoint& to,
                      genericMatrix<T>& m, T color) const;

    /**
     * Render a line from the point f to point t.
     * The points in the line will be added to the given list.
     */
    inline bool apply(const ipoint& from, const ipoint& to, ipointList& l) const;
    
    /**
     * Copy data of "other" functor.
     * \param other the functor to be copied
     * \return a reference to this functor object
     */
    bresenhamLine& copy(const bresenhamLine& other);

    /**
     * Alias for copy member
     * \param other the functor to be copied
     * \return a reference to this functor object
     */
    bresenhamLine& operator=(const bresenhamLine& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bresenhamLine* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bresenhamLine* newInstance() const;

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
     * Implementation of the Bresenham line rendering algorithm.
     * This method uses addPoint to store the produced points
     * in the given point store.
     */ 
    template<typename POINT_STORE, typename T>
    bool renderLine(const int fx, const int fy,
                    const int tx, const int ty, 
                    POINT_STORE& store, T color) const;
    
    /**
     * Adds the given point to the given point list.
     */
    template<typename T>
    inline void addPoint(const int x, const int y, ipointList& l, T unused) const;
    
    /**
     * Draws the given point to the given matrix with the given color.
     */
    template<typename T>
    inline void addPoint(const int x, const int y, genericMatrix<T>& m, T color) const;
    
  };
}

#include "ltiBresenhamLine_inline.h"

/* The following include is necesary, so that the renderLine method gets
   instantiated for every instantiation of genericMatrix<T> as a point store. Otherwise,
   we would need to manually instantiate each apply for every T in genericMatrix<T>.
*/
#include "ltiBresenhamLine_template.h"  

#endif

