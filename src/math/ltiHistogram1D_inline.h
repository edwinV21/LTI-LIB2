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
 * \file   ltiHistogram1D_inline.h
 * \author Pablo Alvarado
 * \date   29.06.00
 * 
 * $Id: ltiHistogram1D_inline.h,v 1.3 2006-09-14 15:14:06 doerfler Exp $
 */

namespace lti {

  // -----------------------------------------------------------------
  //             HISTOGRAM 1D
  // -----------------------------------------------------------------

  /*
   * returns a vector to the first element of the histogram1D
   * (usually every element of the vector is 0;
   */
  inline const int& histogram1D::getFirstCell() const {
    return firstCell_;
  }

  /*
   * returns a vector to the last element of the histogram1D
   */
  inline const int& histogram1D::getLastCell() const {
    return lastCell_;
  }

  /*
   * number of cells in the vector
   */
  inline int histogram1D::size() const {
    return lastCell_+1;
  }

  inline histogram1D& histogram1D::operator=(const histogram1D& other) {
    return copy(other);
  }

  /*
   * read-only access to the element x of the histogram1D
   * @param x index of the histogram1D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline const dhistogram::value_type& histogram1D::at(const int x) const {
    return theHistogram_.at(x);
  }

  /*
   * access element x of the histogram1D
   * @param x index of the histogram1D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline dhistogram::value_type& histogram1D::at(const int x) {
    return theHistogram_.at(x);
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram1D.
   * @param x         index of the histogram1D element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  inline const dhistogram::value_type&
  histogram1D::put(const int x,
                   const dhistogram::value_type& increment) {

    if (uint32(x)>uint32(theCellsPerDimension_.at(0))) {
      return outerBoundsCell();
    }

    theHistogram_.at(x)+=increment;
    numberOfEntries_ += increment;

    return theHistogram_.at(x);
  }

  /*
   * read-only access to the element x of the histogram1D as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram1D (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram1D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram1D
   *         data.
   */
  inline dhistogram::value_type histogram1D::getProbability(const int x) const{
    if (uint32(x)>uint32(theCellsPerDimension_.at(0))) {
      return outerBoundsCell();
    }
    return theHistogram_.at(x)/numberOfEntries_;
  }

  /**
   * get the index of the biggest element in the histogram
   */
  inline int histogram1D::findIndexOfMaximum() const {
    return findIndexOfMaximum();
  }
  
  /**
   * get the index of the smallest element in the histogram
   */
  inline int histogram1D::findIndexOfMinimum() const {
    return findIndexOfMinimum();
  }
  
}
