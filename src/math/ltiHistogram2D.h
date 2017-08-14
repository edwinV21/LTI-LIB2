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
 * \file   ltiHistogram2D.h
 * \author Pablo Alvarado
 * \date   29.06.00
 *
 * $Id: ltiHistogram2D.h,v 1.4 2006-09-14 15:14:06 doerfler Exp $
 */

#ifndef _LTI_HISTOGRAM_2D_H_
#define _LTI_HISTOGRAM_2D_H_

#include "ltiHistogram.h"

namespace lti {

  /**
   * two dimensional histogram of type double
   *
   * The implementation of the 2D histogram allows an efficient way
   * to create 2D histograms... much faster than using n-dimensional
   * histograms with dimension 2.
   *
   * @ingroup gAggregate
   */
  class histogram2D : public dhistogram {
  public:
    /**
     * default constructor creates an empty histogram;
     */
    histogram2D();

    /**
     * create a two dimensional histogram of \e cells x  \e cells
     *
     * @param cells the number of cells per dimension.
     */
    histogram2D(const int cells);

    /**
     * create a two dimensional histogram of \e cellsY x \e cellsX
     *
     * Please note the use of matrix notation (y,x) and NOT (x,y)
     * @param cellsY the number of cells in the first dimension.
     * @param cellsX the number of cells in the second dimension.
     */
    histogram2D(const int cellsY, const int cellsX);

    /**
     * create a two dimensional histogram of \e cells.x x \e cells.y
     *
     * @param cells the number of cells per dimension.
     *        (cells.x is the number of columns of the histogram
     *         cells.y is the number of rows of the histogram)
     */
    histogram2D(const ipoint& cells);

    /**
     * create this histogram as a copy of another histogram
     * @param other the histogram to be copied.
     */
    histogram2D(const histogram2D& other);

    /**
     * destructor
     */
    virtual ~histogram2D();

    /**
     * returns the name of this class: "histogram"
     */
    const std::string& name() const;

    /**
     * returns a vector to the first element of the histogram
     * (usually every element of the vector is 0;
     */
    inline const ipoint& getFirstCell() const;

    /**
     * returns a vector to the last element of the histogram
     */
    inline const ipoint& getLastCell() const;

    /**
     * change cell number of the histogram2D.  All data will
     * be lost!
     *
     * @param cells the number of cells
     *
     */
    void resize(const ipoint& cells);

    /**
     * change cell number of the histogram.  All data will
     * be lost!
     *
     * @param cellsY the number of rows
     * @param cellsX the number of columns
     *
     */
    void resize(const int cellsY, const int cellsX);

    /**
     * initialize all cells of the histogram with 0 (or another specified
     * number).
     */
    void initialize(const value_type& value = value_type(0));

    /**
     * fills the histogram elements with <code>iniValue</code> between
     * the n-dimensional points <code>from</code> and <code>to</code>.
     * @param iniValue the elements will be initialized with this
     *                 value.
     * @param from     first element index
     * @param to       last element index
     *
     * If <code>from</code> or <code>to</code> are out of bounds,
     * they will be (internaly) adjusted to a correct value.
     *
     * Example:
     * \code
     *   lti::histogram1D hist(10); // 1D-histogram with 10 elements
     *   hist.clear();
     *   hist.fill(9,1,3);          // hist=[0,9,9,9,0,0,0,0,0,0]
     * \endcode
     */
    void fill(const value_type& iniValue,
              const ipoint& from=ipoint(0,0),
              const ipoint& to=ipoint(MaxIndex,MaxIndex));

    /**
     * read-only access to the element (x,y) of the histogram
     *
     * Note the use of the matrix notation: first row (y) and then column (x)
     * @param y row of the histogram element to be accessed.  It should
     *          be between getFirstCell().y and getLastCell().y
     * @param x column of the histogram element to be accessed.  It should
     *          be between getFirstCell().x and getLastCell().x
     * @return the number of entries in the given cell
     */
    inline const value_type& at(const int y, const int x) const;

    /**
     * access element x of the histogram
     * @param y row of the histogram element to be accessed.  It should
     *          be between getFirstCell().y and getLastCell().y
     * @param x column of the histogram element to be accessed.  It should
     *          be between getFirstCell().x and getLastCell().x
     * @return the number of entries in the given cell
     */
    inline value_type& at(const int y, const int x);

    /**
     * read-only access to the element p of the histogram
     * @param p index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    inline const value_type& at(const ipoint& p) const;

    /**
     * access element p of the histogram
     * @param p index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    inline value_type& at(const ipoint& p);

    /**
     * increment the cell at row \e y and column \e x by the given number of
     * entries (or 1.0 if nothing is explicitly indicated!) and update the
     * total number of entries in the histogram.
     * @param y row of the histogram element to be incremented.  It should
     *          be between getFirstCell().y and getLastCell().y
     * @param x column of the histogram element to be incremented.  It should
     *          be between getFirstCell().x and getLastCell().x
     * @param increment amount of the incrementation (default: 1)
     * @return the new number of entries of the incremented cell.
     */
    inline const value_type& put(const int y, const int x,
                                 const value_type& increment=value_type(1));

    /**
     * increment the cell at p by the given number of
     * entries (or 1.0 if nothing is explicitly indicated!) and update the
     * total number of entries in the histogram.
     * @param p index of the histogram element to be incremented.  It should
     *          be between getFirstCell() and getLastCell()
     * @param increment amount of the incrementation (default: 1)
     * @return the new number of entries of the incremented cell.
     */
    inline const value_type& put(const ipoint& p,
                                 const value_type& increment=value_type(1));


    /**
     * read-only access to the element p of the histogram as a discrete
     * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
     * where \e n is the number of entries in the histogram (see
     * getNumberOfEntries()).
     *
     * @param p index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the probabilty of the element x, respect to the histogram
     *         data.
     */
    inline value_type getProbability(const ipoint& p) const;


    /**
     * read-only access to the element of the row y and the column x of the
     * histogram as a discrete probability distribution term.  This is
     * equivalent to \f$at(x)/n\f$, where \e n is the number of entries in the
     * histogram (see getNumberOfEntries()).
     *
     * @param y row of the histogram element to be accessed.  It should
     *          be between getFirstCell().y and getLastCell().y
     * @param x column of the histogram element to be accessed.  It should
     *          be between getFirstCell().x and getLastCell().x
     * @return the probabilty of the element (y,x), respect to the histogram
     *         data.
     */
    inline value_type getProbability(const int y, const int x) const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source histogram to be copied.
     * @return a reference to this object
     */
    virtual histogram2D& copy(const histogram2D& other);

    /**
     * create a clone of this histogram
     * @return a pointer to a copy of this histogram
     */
    histogram2D* clone() const;
    histogram2D* newInstance() const;

    /**
     * assigment operator (alias for copy(other)).
     * @param other the histogram to be copied
     * @return a reference to the actual histogram
     */
    histogram2D& operator=(const histogram2D& other);

    /**
     * get the index of the biggest element in the histogram
     */
    inline ipoint getIndexOfMaximum() const;

    /**
     * get the index of the smallest element in the histogram
     */
    inline ipoint getIndexOfMinimum() const;

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);


  protected:

    /**
     * first index ( 0,0 )
     */
    ipoint firstCell_;

    /**
     * last index (cellsInDimension(0) - 1,cellsInDimension(1) - 1 )
     */
    ipoint lastCell_;

    /**
     * wrapper matrix for histogram::theHistogram
     */
    matrix<value_type> theHistoMatrix_;
  };

}

#include "ltiHistogram2D_inline.h"

#endif
