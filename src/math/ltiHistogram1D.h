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
 * \file   ltiHistogram1D.h
 * \author Pablo Alvarado
 * \date   29.06.00
 *
 * $Id: ltiHistogram1D.h,v 1.5 2006-09-14 15:14:06 doerfler Exp $
 */

#ifndef _LTI_HISTOGRAM_1D_H_
#define _LTI_HISTOGRAM_1D_H_

#include "ltiHistogram.h"

namespace lti {
  
  /**
   * one dimensional histogram of type double
   *
   * The implementation of the 1D histogram allows an efficient way
   * to create 1D histogram... much faster than using n-dimensional
   * histograms with dimension 1.
   *
   * @ingroup gAggregate
   */
  class histogram1D : public dhistogram {
  public:

    /**
     * default constructor creates an empty histogram;
     */
    histogram1D();

    /**
     * create a one dimensional histogram of the given dimensionality.
     *
     * @param cells the number of cells.
     */
    histogram1D(const int cells);

    /**
     * create this histogram as a copy of another histogram
     * @param other the histogram to be copied.
     */
    histogram1D(const histogram1D& other);

    /**
     * destructor
     */
    virtual ~histogram1D();

    /**
     * returns the name of this class: "histogram"
     */
    const std::string& name() const;

    /**
     * Returns the index of the first histogram element, which is always zero
     */
    inline const int& getFirstCell() const;

    /**
     * Returns a vector to the last element of the histogram
     */
    inline const int& getLastCell() const;

    /**
     * Returns the total number of cells in this histogram
     *
     * This method is slower than getLastCell, since it need to make some
     * arithmetical operations.  You should use getLastCell instead.
     */
    inline int size() const;

    /**
     * change cell number of the histogram.  All data will
     * be lost! (it will be initialized with 0)
     *
     * @param cells      the number of cells per dimension
     *
     */
    void resize(const int cells);

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
              const int from=0,
              const int to=MaxIndex);

    /**
     * read-only access to the element x of the histogram
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    inline const value_type& at(const int x) const;

    /**
     * access element x of the histogram
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the number of entries in the given cell
     */
    inline value_type& at(const int x);

    /**
     * increment the cell at \e x by the given number of elements (or 1.0 if
     * nothing is explicitly indicated!) and update the number of entries in
     * the histogram.
     * @param x         index of the histogram element to be incremented
     * @param increment amount of the incrementation (default: 1)
     * @return the new number of entries of the incremented cell.
     */
    inline const value_type& put(const int x,
                                 const value_type& increment=1.0f);

    /**
     * read-only access to the element x of the histogram as a discrete
     * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
     * where \e n is the number of entries in the histogram (see
     * getNumberOfEntries()).
     *
     * @param x index of the histogram element to be accessed.  It should
     *          be between getFirstCell() and getLastCell()
     * @return the probabilty of the element x, respect to the histogram
     *         data.
     */
    inline value_type getProbability(const int x) const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source histogram to be copied.
     * @return a reference to this object
     */
    virtual histogram1D& copy(const histogram1D& other);

    /**
     * create a clone of this histogram
     * @return a pointer to a copy of this histogram
     */
    histogram1D* clone() const;

    /**
     * create a new instance of this histogram
     * @return a pointer to a new instance of this histogram
     */
    histogram1D* newInstance() const;

    /**
     * assigment operator (alias for copy(other)).
     * @param other the histogram to be copied
     * @return a reference to the actual histogram
     */
    histogram1D& operator=(const histogram1D& other);

    /**
     * get the index of the biggest element in the histogram
     */
    inline int findIndexOfMaximum() const;

    /**
     * get the index of the smallest element in the histogram
     */
    inline int findIndexOfMinimum() const;

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
     * first index ( 0 )
     */
    int firstCell_;

    /**
     * last index (cellsInDimension(0) - 1)
     */
    int lastCell_;

  };

}

#include "ltiHistogram1D_inline.h"

#endif
