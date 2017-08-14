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
 * \file   ltiBorderPoints.cpp
 *         Declares borderPoints
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiBorderPoints.cpp,v 1.12 2013-09-12 09:03:50 alvarado Exp $
 */

#include "ltiBorderPoints.h"
#include "ltiChainCode.h"
#include "ltiBresenhamLine.h"

namespace lti {

  borderPoints::borderPoints() : ipointList() {
  }

  borderPoints::borderPoints(const borderPoints& other) : ipointList() {
    copy(other);
  }

  borderPoints::~borderPoints() {
  }

  const std::string& borderPoints::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  borderPoints& borderPoints::copy(const borderPoints& other) {
    pointList<int>::copy(other);
    return *this;
  }

  borderPoints* borderPoints::clone() const {
    return new borderPoints(*this);
  }
 
  borderPoints* borderPoints::newInstance() const {
    return new borderPoints();
  }

  borderPoints& borderPoints::castFrom(const ioPoints& theIOPoints) {
    //draws the ioPoints in a channel and generates borderPoints from
    //this channel this function removes the offset from the points so
    //you can cast negativ points
    ioPoints ioP; //i need non const ioPoints for calculate boundary
    ipointList::iterator it;
    ipointList::const_iterator itc;
    const ipointList::const_iterator itce=theIOPoints.end();

    //calculates the real size of the ioPoints without offset
    const irectangle maxRect = theIOPoints.computeBoundary();

    //creates a matrix<ubyte> as big as the ioPoints
    matrix<ubyte> mask(maxRect.getDimensions(),lti::ubyte(0));

    //remove the offset and copy the points to new list
    for (itc=theIOPoints.begin();itc!=itce;itc++){
      ioP.push_back((*itc) - maxRect.ul);
    }

    //draw the points in a chanel and get the borderPoints from the channel
    ioP.setBoundary(irectangle(ipoint(0,0),maxRect.br-maxRect.ul));
    ioP.generateMask(mask,false);
    extractFromMask(mask);

    //add the offset to the borderPoints
    const ipointList::iterator ite=end();
    for (it=begin();it!=ite;it++) {
      (*it) += maxRect.ul;
    }
    return *this;
  }

  borderPoints& borderPoints::castFrom(const areaPoints& theAreaPoints,
                                       matrix<ubyte>& mask) {

    //draws the areaPoints in a channel and generates borderPoints
    //from this channel this function removes the offset from the
    //points so you can cast negativ points
    areaPoints aP; //i need non const areaPoints for calculate boundary
    ipointList::iterator it;
    ipointList::const_iterator itc;
    const ipointList::const_iterator itce=theAreaPoints.end();

    //calculates the real size of the polygon without offset
    const irectangle maxRect = theAreaPoints.computeBoundary();

    //creates a matrix<ubyte> as big as the areaPoints
    mask.assign(maxRect.getDimensions(),static_cast<ubyte>(0));

    //remove the offset and copy the points to new list
    for (itc=theAreaPoints.begin();itc!=itce;itc++){
      aP.push_back((*itc) - maxRect.ul);
    }

    //draw the points in a channel and get the border points from the channel
    aP.setBoundary(irectangle(ipoint(0,0),maxRect.br-maxRect.ul));
    aP.generateMask(mask);
    extractFromMask(mask);

    //add the offset to the borderPoints
    const ipointList::iterator ite=end();
    for (it=begin();it!=ite;it++) {
      (*it) += maxRect.ul;
    }

    return *this;
  }

  borderPoints& borderPoints::castFrom(const areaPoints& theAreaPoints) {
    matrix<ubyte> mask;
    return castFrom(theAreaPoints,mask);
  }

  borderPoints& 
  borderPoints::castPositiveFrom(const areaPoints& theAreaPoints) {

    matrix<ubyte> mask;
    castFrom(theAreaPoints,mask);

    static const ipoint deltas[] = {ipoint( 1, 0), ipoint( 1,-1),
                                    ipoint( 0,-1), ipoint(-1,-1),
                                    ipoint(-1, 0), ipoint(-1,+1),
                                    ipoint( 0,+1), ipoint( 1,+1)};
    
    static const int codes[3][3] = {{3,2,1},
                                    {4,8,0},
                                    {5,6,7}};
    
    // verify if the resulting points are in a positive or negative direction
    // for this, outside the mask is considered zero
    
    // we need at least three points for this two work
    if (size()<3) {
      return *this;
    }
    
    iterator it=begin();
    const iterator eit=end();
    
    ipoint last(*it);
    ++it;
    int accLeft(0),accRight(0);
    
    while (it!=eit) {
      const ipoint current(*it);
      const ipoint d(last-current);
      const int code = codes[d.y+1][d.x+1];
      
      const int left = (code+2)%8;
      const int right= (code+6)%8;
      
      const ipoint onLeft(current+deltas[left]);
      if (inside(onLeft,mask)) {
        accLeft =mask.at(onLeft);
      }
      const ipoint onRight(current+deltas[right]);
      if (inside(onRight,mask)) {
        accRight=mask.at(current);
      }
    
      last=current;
      ++it;
    }
  
    if (accLeft>accRight) {
      // ipointList has no reverse() method, but it has swap!
      borderPoints tmp;
      
      for (it=begin();it!=eit;++it) {
        tmp.push_front(*it);
      }
      tmp.swap(*this);
    }

    return *this;
  }

  borderPoints& borderPoints::castFrom(const ipolygonPoints& thePolygonPoints){

    static bresenhamLine bl;
    ipolygonPoints::const_iterator 
      start = thePolygonPoints.begin(),
      stop  = thePolygonPoints.end(),
      it    = start;
    
    clear();
    
    ipoint from = *(it++), to;
    while(it != stop){
      to = *(it++);
      bl.apply(from,to,*this);
      //remove last point to prevent double entries
      pop_back();
      from = to;
    }
    bl.apply(from,*start,*this);
    //remove last point to prevent double entries
    pop_back();

    return *this;

  }

  bool borderPoints::extractFromMask(const matrix<ubyte>& mask) {

    ipoint p,q,anchor;

    static const ubyte off = 0;
    const int cols=mask.columns();

    // calculate border points
    clear();

    //  look for anchor
    anchor.x = -1;

    p.y = 0;
    while ((anchor.x<0) && (p.y<mask.rows())) {
      p.x=0;
      while (p.x<cols) {
        if (mask.at(p) != off) {
          anchor.copy(p);
          p.x = cols; // flag for exit
        }
        p.x++;
      }
      p.y++;
    }

    if (anchor.x<0) {
      // no object in the mask!
      return true;
    }

    // current direction
    chainCode current(chainCode::E);
    p=anchor;

    push_back(p);

    // search border points
    int dirCounter;
    do {
      dirCounter = 0;
      q = current.getNext(p); // next point
      // repeat until masked point found or all directions have been checked
      while ((!inside(q, mask) || (mask.at(q) == off)) && (dirCounter < 8)) {
        // rotate clockwise
        current.subtract(chainCode(chainCode::NE));
        q = current.getNext(p);
        dirCounter++;
      }
      // no border point found
      if (dirCounter == 8) {
        return true;
      }

      // new point is a border point
      p = q;
      push_back(p);

      // turn 90 degrees counterclockwise and continue searching clockwise
      current.add(chainCode(chainCode::N));
    } while (p!=anchor);

    // the last element is again the anchor element,
    // which was inserted at the beginning:
    pop_back(); // delete duplicated anchor!

    return true;
  }

  bool borderPoints::generateMask(matrix<ubyte>& mask,
                                  const bool compBoundary,
                                  const bool exactBoundaryDimensions,
                                  const bool keepMaskData) const {

    static const ubyte off     = chainCode::Nothing;
    static const ubyte on      = 255;

    if (empty()) {
      return true;
    }

    // boundary is not updated automatically -> compute it on user
    // request, else use stored boundary (possibly inaccurate)
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

    const_iterator it;
    const const_iterator ite=end();

    if (size() < 4) {
      // three points are at least required for the next algorithms, so
      // if there are too few points, just draw them in the mask at that
      // was it!
      for (it = begin(); it != ite; ++it) {
        mask.at(*it) = on;
      }
      return true;
    }

    ipoint p,tst;

    // for each three points in the contour create the canzler-codes map
    it = begin();
    p = *it;
    ++it;
    tst = *it;
    ++it;
    chainCode oldC(p,tst);

    for (;it!=ite;++it) {
      p = *it;
      chainCode c(tst,p);
      mask.at(tst) |= oldC.getCanzlerCode(c);
      oldC = c;
      tst = p;
    }

    // the last things (end of the list and the begining
    it = begin();
    p = *it;
    chainCode c(tst,p);
    mask.at(tst) |= oldC.getCanzlerCode(c); // the last element in the list
    oldC = c;
    tst = p;
    ++it;
    p = *it;
    c = chainCode(tst,p);
    mask.at(tst) |= oldC.getCanzlerCode(c); // the first element in the list

    // now we have the canzler map, which contains the io-points, we can
    // just search for them and fill the contents.

    int y;
    bool drawFromBegin;
    bool drawing;
    ubyte val;
    const int mrows=mask.rows();

    vector<ubyte>::iterator mit,eit,xit;
    for (y=0;y<mrows;y++) {
      drawFromBegin = true;
      drawing = false;

      vector<ubyte>& vct = mask.getRow(y);
      for (mit=vct.begin(),eit=vct.end();mit!=eit;++mit) {
        val = (*mit);
        if ((val & chainCode::Left) == chainCode::Left) {
          drawFromBegin = false;
          drawing = true;
        }

        if (drawing) {
          (*mit) = on;
        }

        if ((val & chainCode::Right) == chainCode::Right) {
          drawing = false;
          if (drawFromBegin) { // ups! an "out" pixel found first..
            for (xit=vct.begin();xit!=mit;++xit) {
              (*xit) = on;
            }
            drawFromBegin = false;
          }
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

  void borderPoints::invert() {
    borderPoints tmp(*this);
    const_iterator it = tmp.begin();
    const const_iterator ite = tmp.end();
    clear();
    for (it=tmp.begin();it!=ite;++it) {
      push_front(*it);
    }
  }

  bool borderPoints::isConsistent() const {
    if (size() == 0 || size() == 1)
      return true;
    const_iterator it = begin();
    const const_iterator ite = end();
    ipoint p;
    p = *it - back();
    for (;;) {
      if (p.x < -1 || p.x > 1 || p.y < -1 || p.y > 1 || (p.x == 0 && p.y == 0))
        return false;
      p = *it;
      if (++it == ite)
        return true; // no inconsistency detected
      p = *it - p;
    }
  }


}
