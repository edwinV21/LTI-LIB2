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
 * \file   ltiHistogram2D_inline.h
 * \author Pablo Alvarado
 * \date   29.06.00
 * 
 * $Id: ltiHistogram2D_inline.h,v 1.4 2006-09-14 15:14:06 doerfler Exp $
 */

namespace lti {

  // -----------------------------------------------------------------
  //             HISTOGRAM 2D
  // -----------------------------------------------------------------

  /*
   * returns a vector to the first element of the histogram2D
   * (usually every element of the vector is 0;
   */
  inline const ipoint& histogram2D::getFirstCell() const {
    return firstCell_;
  }

  /*
   * returns a vector to the last element of the histogram2D
   */
  inline const ipoint& histogram2D::getLastCell() const {
    return lastCell_;
  }


  /*
   * read-only access to the element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline const dhistogram::value_type& histogram2D::at(const ipoint& p) const {
    return theHistoMatrix_.at(p);
  }

  /*
   * access element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline dhistogram::value_type& histogram2D::at(const ipoint& p) {
    return theHistoMatrix_.at(p);
  }

  /*
   * read-only access to the element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline const dhistogram::value_type& histogram2D::at(const int y,
                                                       const int x) const {
    return theHistoMatrix_.at(y,x);
  }

  /*
   * access element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline dhistogram::value_type& histogram2D::at(const int y, const int x) {
    return theHistoMatrix_.at(y,x);
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram2D.
   * @param x         index of the histogram2D element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  inline const dhistogram::value_type& 
  histogram2D::put(const int y,
                   const int x,
                   const dhistogram::value_type& increment) {
    if ((static_cast<uint32>(y) >= 
         static_cast<uint32>(theCellsPerDimension_.at(1))) ||
        (static_cast<uint32>(x) >= 
         static_cast<uint32>(theCellsPerDimension_.at(0)))) {
      return outerBoundsCell();
    }

    theHistoMatrix_.at(y,x)+=increment;
    numberOfEntries_ += increment;

    return theHistoMatrix_.at(y,x);
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram2D.
   * @param x         index of the histogram2D element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  inline const dhistogram::value_type&
  histogram2D::put(const ipoint& p,
                   const dhistogram::value_type& increment) {
    if ((static_cast<uint32>(p.y) >= 
         static_cast<uint32>(theCellsPerDimension_.at(1))) ||
        (static_cast<uint32>(p.x) >= 
         static_cast<uint32>(theCellsPerDimension_.at(0)))) {
      return outerBoundsCell();
    }

    theHistoMatrix_.at(p)+=increment;
    numberOfEntries_ += increment;

    return theHistoMatrix_.at(p);
  }

  /*
   * read-only access to the element x of the histogram2D as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram2D (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram2D
   *         data.
   */
  inline dhistogram::value_type
  histogram2D::getProbability(const int y, const int x) const {

    if ((static_cast<uint32>(y) >= 
         static_cast<uint32>(theCellsPerDimension_.at(1))) ||
        (static_cast<uint32>(x) >= 
         static_cast<uint32>(theCellsPerDimension_.at(0)))) {
      return outerBoundsCell();
    }

    return theHistoMatrix_.at(y,x)/numberOfEntries_;
  }

  /*
   * read-only access to the element x of the histogram2D as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram2D (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram2D
   *         data.
   */
  inline dhistogram::value_type
  histogram2D::getProbability(const ipoint& p) const {
    if ((static_cast<uint32>(p.y) >= 
         static_cast<uint32>(theCellsPerDimension_.at(1))) ||
        (static_cast<uint32>(p.x) >= 
         static_cast<uint32>(theCellsPerDimension_.at(0)))) {
      return outerBoundsCell();
    }

    return theHistoMatrix_.at(p)/numberOfEntries_;
  }

  /**
   * get the index of the biggest element in the histogram
   */
  inline ipoint histogram2D::getIndexOfMaximum() const {
    return theHistoMatrix_.findIndexOfMaximum();
  }

  /**
   * get the index of the smallest element in the histogram
   */
  inline ipoint histogram2D::getIndexOfMinimum() const {
    return theHistoMatrix_.findIndexOfMinimum();
  }
  
}
