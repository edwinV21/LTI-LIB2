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
 * \file   ltiAreaPoints.h
 *         Declares areaPoints
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiAreaPoints.cpp,v 1.8 2007-09-24 04:01:11 alvarado Exp $
 */

#include "ltiAreaPoints.h"

namespace lti {


  areaPoints::areaPoints() : ipointList() {
  }

  areaPoints::areaPoints(const areaPoints& other) : ipointList() {
    copy(other);
  }

  areaPoints::~areaPoints() {
  }

  const std::string& areaPoints::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  areaPoints& areaPoints::copy(const areaPoints& other) {
    pointList<int>::copy(other);
    return *this;
  }

  areaPoints* areaPoints::clone() const {
    return new areaPoints(*this);
  }
 
  areaPoints* areaPoints::newInstance() const {
    return new areaPoints();
  }

  areaPoints& areaPoints::castFrom(const ioPoints& theIOPoints) {

    ioPoints::const_iterator itc=theIOPoints.begin();
    const ioPoints::const_iterator itce=theIOPoints.end();

    clear();
    ipoint in,out;
    int x;
    while (itc!=itce) {
      // get in and out
      in = *itc;
      itc++;
      if (itc==itce) {
        // no output point? -> there must be an error!
        return *this;
      }
      out = *itc;
      itc++;
      if (out.y!=in.y) {
        // in and out must lie in the same line! -> error
        return *this;
      }
      for (x=in.x;x<=out.x;x++) {
        push_back(ipoint(x,in.y));
      }
    }

    return *this;
  }

  areaPoints& areaPoints::castFrom(const borderPoints& theBorderPoints) {
    // draws the borderPoints in a channel and generates areaPoints
    // from this channel

    // this function removes the offset from the points so you can cast
    // negativ points
    borderPoints bp; //non const borderPoints for calculate boundary

    ipointList::iterator it;
    ipointList::const_iterator itc;
    const ipointList::const_iterator itce=theBorderPoints.end();

    //calculates the real size of the border without offset
    const irectangle maxRect = theBorderPoints.computeBoundary();

    //creates a matrix<ubyte> as big as the borderPoints
    matrix<ubyte> mask;

    //remove the offset and copy the points to new list
    for (itc=theBorderPoints.begin(); itc!=itce; itc++) {
      bp.push_back((*itc)-maxRect.ul);
    }

    //draw the points in a channel and get the areaPoints from the channel
    bp.setBoundary(irectangle(ipoint(0,0),maxRect.br-maxRect.ul));
    bp.generateMask(mask,false);

    extractFromMask(mask);
    //add the offset to the areaPoints
    const iterator ite=end();
    for (it=begin();it!=ite;it++) {
      (*it)+=maxRect.ul;
    }

    return *this;
  }

//   areaPoints& areaPoints::castFrom(const polygonPoints& thePolygonPoints){
//     // this function draws the polygon into a matrix<ubyte> and extracts
//     // the areaPoints with the extractFromMask function of the
//     // areaPoints
//     areaPoints bp;
//     draw<ubyte> drawer;
//     polygonPoints::iterator it;
//     ipointList::const_iterator itc,ite;
//     rectangle maxRect;

//     // a non const polygon is need
//     polygonPoints pPs;

//     // calculates the real size of the polygon without offset
//     maxRect = thePolygonPoints.computeBoundary();

//     // creates a matrix<ubyte> as big as the polygon
//     matrix<ubyte> mask(maxRect.getDimensions(),lti::ubyte(0));

//     //remove the offset
//     for (itc=thePolygonPoints.begin();itc!=thePolygonPoints.end();itc++) {
//       pPs.push_back((*itc) - maxRect.ul);
//     }

//     drawer.use(mask);
//     drawer.setColor(255);
//     drawer.set(*pPs.begin());
//     //connect the polygonPoints with lines
//     for (it=pPs.begin();it!=pPs.end();it++) {
//       drawer.lineTo((*it));
//     }//close the polygon
//     drawer.lineTo(*pPs.begin());

//     //extract the object as areaPointList
//     std::list<areaPoints> objectList;
//     objectsFromMask extractor;
//     objectsFromMask::parameters extractorParams;
//     extractorParams.level = 0;
//     extractorParams.meltHoles = true;
//     extractor.setParameters(extractorParams);
//     extractor.apply(mask,objectList);
//     // if more than one object in the channel -> abort
//     if (objectList.size() == 1) {
//       itc = (*objectList.begin()).begin();
//       ite = (*objectList.begin()).end();
//       clear();
//       // add offset to the areaPoints, while copying
//       for (;itc!=ite;itc++) {
//         push_back((*itc) + maxRect.ul);
//       }
//     }
//     else {
//       clear();
//     }

//     return *this;
//   }


  bool areaPoints::extractFromMask(const matrix<ubyte>& mask) {
    ipoint p;

    clear();

    static const ubyte off = 0;

    for (p.y=0;p.y<mask.rows();p.y++) {
      for (p.x=0;p.x<mask.columns();p.x++) {
        if (mask.at(p)!=off) {
          push_back(p);
        }
      }
    }
    return true;
  }

  bool areaPoints::generateMask(matrix<ubyte>& mask,
                                const bool compBoundary,
                                const bool exactBoundaryDimensions,
                                const bool keepMaskData) const {

    static const ubyte off = 0;
    static const ubyte on  = 255;

    if (empty()) {
      return true;
    }

    const irectangle maxRect = 
      compBoundary ? this->computeBoundary() : getBoundary();

    if (exactBoundaryDimensions) {

      mask.assign(maxRect.br.y-maxRect.ul.y+1,
                  maxRect.br.x-maxRect.ul.x+1,
                  off);

      const_iterator it;
      const const_iterator ite=end();
      if ((maxRect.ul.x < 0) || (maxRect.ul.y < 0)) {
        for (it=begin();it!=ite;++it) {
          if (((*it).x >= 0) && ((*it).y >= 0)) {
            mask.at(*it-maxRect.ul) = on;
          }
        }
      } else {
        for (it=begin();it!=ite;++it) {
          mask.at(*it-maxRect.ul) = on;
        }
      }

    } else {

      if (keepMaskData) {
        mask.resize(max(maxRect.br.y+1,mask.rows()),
                    max(maxRect.br.x+1,mask.columns()),
                    off,CopyAndInit);
      } else {
        mask.assign(max(maxRect.br.y+1,mask.rows()),
                    max(maxRect.br.x+1,mask.columns()),
                    off);
      }

      const_iterator it;
      const const_iterator ite=end();
      if ((maxRect.ul.x < 0) || (maxRect.ul.y < 0)) {
        for (it=begin();it!=ite;++it) {
          if (((*it).x >= 0) && ((*it).y >= 0)) {
            mask.at(*it) = on;
          }
        }
      } else {
        for (it=begin();it!=ite;++it) {
          mask.at(*it) = on;
        }
      }

    }

    return true;
  }

  int areaPoints::intersectionArea(const irectangle& rect) const {
    int count = 0;
    const_iterator it;
    const const_iterator ite=end();
    for (it = begin(); it != ite; it++)
      if (rect.contains(*it))
        count++;
    return count;
  }


}
