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
 * \file   ltiHistogram1D.cpp
 * \author Pablo Alvarado
 * \date   29.06.00
 * 
 * $Id: ltiHistogram1D.cpp,v 1.5 2006-09-14 15:14:06 doerfler Exp $
 */

#include "ltiHistogram1D.h"

namespace lti {

  // -----------------------------------------------------------------
  //             HISTOGRAM 1D
  // -----------------------------------------------------------------

  /*
   * default constructor creates an empty histogram1D;
   */
  histogram1D::histogram1D()
    : dhistogram(), firstCell_(0), lastCell_(0) {
  }

  /*
   * create a histogram1D of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the histogram1D
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the histogram1D.
   * @param cells the number of cells per dimension.
   */
  histogram1D::histogram1D(const int cells)
    : dhistogram(), firstCell_(0), lastCell_(0)  {
    resize(cells);
  }

  /*
   * create this histogram1D as a copy of another histogram1D
   * @param other the histogram1D to be copied.
   */
  histogram1D::histogram1D(const histogram1D& other): dhistogram() {
    copy(other);
  }

  /*
   * destructor
   */
  histogram1D::~histogram1D() {
  }

  /*
   * change dimensionality and cell number of the histogram1D.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram1D
   * @param cells      the number of cells per dimension
   *
   */
  void histogram1D::resize(const int cells) {
    // a temporary dimensionality vector
    ivector tmp(1,cells);
    firstCell_ = 0;
    lastCell_ = cells-1;
    // resize
    dhistogram::resize(1,tmp);
  }

  /*
   * initialize all cells of the histogram1D with 0 (or another specified
   * number).
   */
  void histogram1D::initialize(const dhistogram::value_type& value) {
    fill(value);
  }

  /*
   * fills the histogram1D elements with <code>iniValue</code> between
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
   *   lti::histogram1D hist(1,10); // 1D-histogram1D with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  void histogram1D::fill(const dhistogram::value_type& iniValue,
                         const int from,
                         const int to) {


    int f,t;
    int idx;
    double tmp;

    // check and correct the for the boundaries
    f = max( min(from,to) , firstCell_ );
    t = min( max(from,to) , lastCell_  );

    // can I fill everything?
    if ((f==firstCell_) && (t==lastCell_)) {
      theHistogram_.fill(iniValue);
      numberOfEntries_ = totalNumberOfCells_*iniValue;
      return;
    }

    idx = f; // the index! this will iterate on the histogram1D
             // hyperbox

    // to check if index at the end, we just need to check the last
    // dimension of the index
    for (idx=f;idx<=t;idx++) {
      // update of entry number due to this cell
      tmp = iniValue - theHistogram_.at(idx);
      numberOfEntries_ += tmp;

      // set the cell value
      theHistogram_.at(idx) = iniValue;
    }
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source histogram1D to be copied.
   * @return a reference to this object
   */
  histogram1D& histogram1D::copy(const histogram1D& other) {

    dhistogram::copy(other);
    firstCell_ =  other.firstCell_;
    lastCell_  =  other.lastCell_;

    return *this;
  }

  /*
   * create a clone of this histogram1D
   * @return a pointer to a copy of this histogram1D
   */
  histogram1D* histogram1D::clone() const {
    return new histogram1D(*this);
  }

  histogram1D* histogram1D::newInstance() const {
    return new histogram1D;
  }

  const std::string& histogram1D::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /**
   * write the object in the given ioHandler
   */
  bool histogram1D::write(ioHandler& handler,
                          const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
      dhistogram::write(handler,false);
    };

    if (complete) b = handler.writeEnd();
    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  bool histogram1D::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
      dhistogram::read(handler,false);
      if (dimensionality_==1) {
        firstCell_ = dhistogram::firstCell_.at(0);
        lastCell_  = dhistogram::lastCell_.at(0);
      } else {
        handler.setStatusString("Not a 1D histogram.");
        b=false;
      }
    }

    if (complete) b = handler.readEnd();
    return b;
  }

}
