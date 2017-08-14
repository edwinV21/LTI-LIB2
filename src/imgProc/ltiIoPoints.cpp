/*
 * Copyright (C) 1998-2004
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


/*
 * \file   ltiIoPoints.cpp
 *         Implements ioPoints
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiIoPoints.cpp,v 1.8 2007-09-24 04:01:11 alvarado Exp $
 */

#include "ltiIoPoints.h"

namespace lti {

  ioPoints::ioPoints() : ipointList() {
  }

  ioPoints::ioPoints(const ioPoints& other)
    : ipointList() {
    copy(other);
  }

  ioPoints::~ioPoints() {
  }

  const std::string& ioPoints::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioPoints& ioPoints::copy(const ioPoints& other) {
    pointList<int>::copy(other);
    return *this;
  }

  ioPoints* ioPoints::clone() const {
    return new ioPoints(*this);
  }
 
  ioPoints* ioPoints::newInstance() const {
    return new ioPoints();
  }

  ioPoints& ioPoints::castFrom(const areaPoints& theAreaPoints) {

    if (theAreaPoints.size() == 1) {
      // special case with just one pixel can be done much faster..
      clear();
      push_back(*theAreaPoints.begin()); // in
      push_back(*theAreaPoints.begin()); // out
      return *this;
    }
    //draws the borderPoints in a channel and generates ioPoints from
    //this channel this function removes the offset from the points so
    //you can cast negativ points
    areaPoints ap; //the points without offset

    ipointList::const_iterator itc;
    ipointList::const_iterator itce=theAreaPoints.end();
    ipointList::iterator it;

    //calculates the real size of the area without offset
    const irectangle maxRect = theAreaPoints.computeBoundary();

    //creates a matrix<ubyte> as big as the ioPoints
    matrix<ubyte> mask(maxRect.getDimensions(),static_cast<ubyte>(0));
    //remove the offset and copy the points to new list
    for (itc=theAreaPoints.begin();itc!=itce;itc++){
      ap.push_back((*itc) - maxRect.ul);
    }

    //draw the points in a channel and get the io points from the channel
    ap.setBoundary(irectangle(ipoint(0,0),maxRect.br-maxRect.ul));
    ap.generateMask(mask,false);
    extractFromMask(mask);

    //add the offset to the ioPoints
    const iterator ite=end();
    for (it=begin();it!=ite;it++) {
      (*it) += maxRect.ul;
    }

    return *this;
  }

  ioPoints& ioPoints::castFrom(const borderPoints& theBorderPoints) {
    //draws the borderPoints in a channel and generates ioPoints from
    //this channel this function removes the offset from the points so
    //you can cast negativ points
    borderPoints bp; //i need non const points for calculate boundary
    ipointList::const_iterator itc;
    ipointList::const_iterator itce=theBorderPoints.end();
    ipointList::iterator it;

    //calculates the real size of the border without offset
    const irectangle maxRect = theBorderPoints.computeBoundary();

    //creates a matrix<ubyte> as big as the borderPoints
    matrix<ubyte> mask(maxRect.getDimensions(),static_cast<ubyte>(0));

    //remove the offset and copy the points to new list
    for (itc=theBorderPoints.begin();itc!=itce;itc++){
      bp.push_back((*itc) - maxRect.ul);
    }

    //draw the points in a chanel and get the ioPoints from the channel
    bp.setBoundary(irectangle(ipoint(0,0),maxRect.br-maxRect.ul));
    bp.generateMask(mask,false);
    extractFromMask(mask);

    //add the offset to the ioPoints
    const iterator ite=end();
    for (it=begin();it!=ite;it++) {
      (*it) += maxRect.ul;
    }

    return *this;
  }

  bool ioPoints::extractFromMask(const matrix<ubyte>& mask) {
    ipoint p,q;

    // calculate io points
    clear();

    bool out=true;
    static const ubyte off = 0;
    const int mrows=mask.rows();
    const int mcols=mask.columns();

    q = ipoint(0,0);

    for (p.y=0;p.y<mrows;p.y++) {
      for (p.x=0;p.x<mcols;q=p,p.x++) {
        if (out && mask.at(p)!=off) {
          // IN point
          push_back(p);
          out = false;
        } else if (!out && mask.at(p) == off) {
          // OUT point
          push_back(q);
          out = true;
        }
      }

      if (!out) { // end of line! => force OUT point
        push_back(q);
        out = true;
      }
    }

    return true;
  }

  bool ioPoints::generateMask(matrix<ubyte>& mask,
                              const bool compBoundary,
                              const bool exactBoundaryDimensions,
                              const bool keepMaskData) const {
    const_iterator it;
    const const_iterator ite=end();
    ipoint in,out;
    int x;

    static const ubyte off = 0;
    static const ubyte on  = 255;

    if (empty()) {
      return true;
    }

    const irectangle tmpBoundary = 
      compBoundary ? this->computeBoundary() : getBoundary();

    if (keepMaskData) {
      mask.resize(max(tmpBoundary.br.y+1,mask.rows()),
                  max(tmpBoundary.br.x+1,mask.columns()),
                  off,CopyAndInit);
    } else {
      mask.assign(max(tmpBoundary.br.y+1,mask.rows()),
                  max(tmpBoundary.br.x+1,mask.columns()),
                  off);
    }

    const int mrows=mask.rows();
    const int mlastCol=mask.lastColumn();
    it = begin();

    while (it!=ite) {
      // get in and out
      in = *it;
      it++;
      if (it==ite) {
        // no output point? -> there must be an error!
        return false;
      }

      out = *it;
      it++;

      if (out.y!=in.y) {
        // in and out must lie in the same line! -> error
        return false;
      }

      if (static_cast<unsigned int>(in.y) <
          static_cast<unsigned int>(mrows)) {
        vector<ubyte>& vct = mask.getRow(in.y);
        const int maxx=min(out.x,mlastCol);
        for (x=max(0,in.x);x<=maxx;x++) {
          vct.at(x)=on;
        }
      }
    }

    // This is a quick-and-dirty hack. The fastest implementation
    // would be to completely copy the above code for both cases
    // (exactBoundaryDimensions == false / true) and adapt the matrix
    // resize and access operations accordingly.
    if (exactBoundaryDimensions) {
      lti::matrix<ubyte> mask2(mask);

      mask.assign(tmpBoundary.br.y-tmpBoundary.ul.y+1,
                  tmpBoundary.br.x-tmpBoundary.ul.x+1,
                  off);

      mask.fill(mask2,0,0,
                matrix<ubyte>::MaxIndex,matrix<ubyte>::MaxIndex,
                tmpBoundary.ul.y,tmpBoundary.ul.x);
    }

    return true;
  }


}
