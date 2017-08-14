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
 * \file   ltiHistogram2D.cpp
 * \author Pablo Alvarado
 * \date   29.06.00
 * 
 * $Id: ltiHistogram2D.cpp,v 1.6 2006-09-14 15:14:06 doerfler Exp $
 */

#include "ltiHistogram2D.h"

namespace lti {

  // -----------------------------------------------------------------
  //             HISTOGRAM 2D
  // -----------------------------------------------------------------

  /*
   * default constructor creates an empty histogram2D;
   */
  histogram2D::histogram2D()
    : firstCell_(0,0), lastCell_(0,0) {
  }

  /*
   * create a histogram2D of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the histogram2D
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the histogram2D.
   * @param cells the number of cells per dimension.
   */
  histogram2D::histogram2D(const int cells)
    : firstCell_(0,0), lastCell_(0,0)  {
    resize(cells,cells);
  }

  histogram2D::histogram2D(const int cellsY, const int cellsX)
    : firstCell_(0,0), lastCell_(0,0)  {
    resize(cellsY,cellsX);
  }

  histogram2D::histogram2D(const ipoint& cells)
    : firstCell_(0,0), lastCell_(0,0)  {
    resize(cells);
  }

  /*
   * create this histogram2D as a copy of another histogram2D
   * @param other the histogram2D to be copied.
   */
  histogram2D::histogram2D(const histogram2D& other) : dhistogram() {
    copy(other);
  }

  /*
   * destructor
   */
  histogram2D::~histogram2D() {
    theHistoMatrix_.clear();
  }

  inline histogram2D& histogram2D::operator=(const histogram2D& other) {
    return copy(other);
  }

  /*
   * change dimensionality and cell number of the histogram2D.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram2D
   * @param cells      the number of cells per dimension
   *
   */
  void histogram2D::resize(const int cellsY, const int cellsX) {
    // a temporary dimensionality vector
    ivector tmp(2,cellsY);
    tmp.at(0)=cellsX;
    firstCell_ = ipoint(0,0);
    lastCell_ =  ipoint(cellsX-1,cellsY-1);
    // resize
    dhistogram::resize(2,tmp);
    theHistoMatrix_.useExternData(cellsY,cellsX,theHistogram_.data());
  }

  void histogram2D::resize(const ipoint& cells) {
    resize(cells.y,cells.x);
  }

  /*
   * initialize all cells of the histogram2D with 0 (or another specified
   * number).
   */
  void histogram2D::initialize(const dhistogram::value_type& value) {
    fill(value);
  }

  /*
   * fills the histogram2D elements with <code>iniValue</code> between
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
   *   lti::histogram2D hist(1,10); // 1D-histogram2D with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  void histogram2D::fill(const dhistogram::value_type& iniValue,
                         const ipoint& from,
                         const ipoint& to) {


    ipoint f,t;
    ipoint idx;
    double tmp;

    // check and correct the for the boundaries
    f.x = max( min(from.x,to.x) , firstCell_.x );
    t.x = min( max(from.x,to.x) , lastCell_.x  );
    f.y = max( min(from.y,to.y) , firstCell_.y );
    t.y = min( max(from.y,to.y) , lastCell_.y  );

    // can I fill everything?
    if ((f==firstCell_) && (t==lastCell_)) {
      theHistogram_.fill(iniValue);
      numberOfEntries_ = totalNumberOfCells_*iniValue;
      return;
    }

    idx = f; // the index! this will iterate on the histogram2D
             // hyperbox

    // to check if index at the end, we just need to check the last
    // dimension of the index
    for (idx.y=f.y;idx.y<=t.y;idx.y++) {
      for (idx.x=f.x;idx.x<=t.x;idx.x++) {

        // update of entry number due to this cell
        tmp = iniValue - theHistoMatrix_.at(idx);
        numberOfEntries_ += tmp;

        // set the cell value
        theHistoMatrix_.at(idx) = iniValue;
      }
    }
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source histogram2D to be copied.
   * @return a reference to this object
   */
  histogram2D& histogram2D::copy(const histogram2D& other) {

    dhistogram::copy(other);
    firstCell_ =  other.firstCell_;
    lastCell_  =  other.lastCell_;
    theHistoMatrix_.useExternData(lastCell_.y+1,lastCell_.x+1,
                                 theHistogram_.data());
    return *this;
  }

  /*
   * create a clone of this histogram2D
   * @return a pointer to a copy of this histogram2D
   */
  histogram2D* histogram2D::clone() const {
    return new histogram2D(*this);
  }

  histogram2D* histogram2D::newInstance() const {
    return new histogram2D;
  }

  const std::string& histogram2D::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /**
   * write the object in the given ioHandler
   */
  bool histogram2D::write(ioHandler& handler,
                          const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
//       lti::write(handler,"firstCell_",firstCell_);
//       lti::write(handler,"lastCell_",lastCell_);
      dhistogram::write(handler,false);
    };

    if (complete) b = handler.writeEnd();

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  bool histogram2D::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
//       lti::read(handler,"firstCell_",firstCell_);
//       lti::read(handler,"lastCell_",lastCell_);
      dhistogram::read(handler,false);
      if (dimensionality_==2) {
        firstCell_.x = dhistogram::firstCell_.at(0);
        firstCell_.y = dhistogram::firstCell_.at(1);
        lastCell_.x  = dhistogram::lastCell_.at(0);
        lastCell_.y  = dhistogram::lastCell_.at(1);
      } else {
        handler.setStatusString("Not a 2D histogram");
        b=false;
      }
      theHistoMatrix_.useExternData(theCellsPerDimension_.at(1),
                                    theCellsPerDimension_.at(0),
                                    theHistogram_.data());
    }

    if (complete) b = handler.readEnd();
    return b;
  }

}
