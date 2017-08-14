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
 * \file   ltiPolygonPoints_template.h
 * \author Ruediger Weiler
 * \author Pablo Alvarado
 * \date   15.11.2002
 * 
 * $Id: ltiPolygonPoints_template.h,v 1.5 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiObject.h"
#include "ltiDraw.h"
#include "ltiImage.h"
#include "ltiSort.h"
#include "ltiGeometry.h"
#include "ltiRectangle.h"
#include "ltiConvexHull.h"

#include "ltiDebug.h"

namespace lti {

  // -----------------------------------------------------------------
  // Polygon Points
  // -----------------------------------------------------------------
  template<class T>
  polygonPoints<T>::polygonPoints() : pointList<T>() {
  }

  template<class T>
  polygonPoints<T>::polygonPoints(const polygonPoints<T>& other)
    : pointList<T>() {
    this->copy(other);
  }

  template<class T>
  polygonPoints<T>::~polygonPoints() {
  }

  template<class T>
  const std::string& polygonPoints<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  template<class T>
  polygonPoints<T>&
  polygonPoints<T>::castFrom(const borderPoints& theBorderPoints,
                             const int minLength,
                             const double maxDistance,
                             const bool closed,
                             const bool searchMaxDist) {
    
    return approximate(theBorderPoints,minLength,
                       maxDistance,closed,searchMaxDist);
  }

  template<class T>
  polygonPoints<T>& polygonPoints<T>::castFrom(const ioPoints& thePointList) {
    // filter the io points that represent single pixels
    pointList<T> pts;
    ioPoints::const_iterator it1,it2;

    _lti_debug("Called castFrom(ioPoints)\n");

    it1 = it2 = thePointList.begin();

    if (it1 != thePointList.end()) {
      it2++;
      while (it2 != thePointList.end()) {
        if (*it1 != *it2) {
          // this does not make too much sense to me...
          //pts.push_back(point<T>((*it1).x,(*it2).y));
          pts.push_back(point<T>(static_cast<T>((*it1).x),
                                 static_cast<T>((*it1).y)));
        }
        ++it1;
        ++it2;
      }
      // at this point is it2==thePointList.end(), so this does not make
      // sense
      //pts.push_back(point<T>((*it1).x,(*it2).y));
      // I guess it is supposed to be:
      pts.push_back(point<T>(static_cast<T>((*it1).x),
                             static_cast<T>((*it1).y)));
    } else {
      return *this;
    }
    
    convexHull ch;
    return ch.computeConvexHull(pts,*this);
  }

  template<class T>
  polygonPoints<T>&
  polygonPoints<T>::castFrom(const pointList<T>& thePointList) {
    convexHull ch;
    return ch.computeConvexHull(thePointList,*this);
  }

  template<class T>
  void polygonPoints<T>::invert(){
    polygonPoints<T> tmp(*this);
    typename polygonPoints<T>::const_iterator it;
    this->clear();
    for (it=tmp.begin();it!=tmp.end();++it){
      this->push_front(*it);
    }
  }

  /* TODO
  template<class T>
  polygonPoints<T>&
  polygonPoints<T>::clipPoly(const polygonPoints<T> &polygon1,
                              const polygonPoints<T> &polygon2,
                              const int minLength,
                              const double& maxDistance){

    //this function draws both polygons in an channel8
    //and extracts the new polygon consisting of pP1 and pP2
    //if the poly1 and poly2 are not overlapping return poly1

    ipointList pPs1;
    pPs1.castFrom(polygon1); //non const polygons are need
    ipointList pPs2;
    pPs2.castFrom(polygon2);

    typename ipointList::iterator itpp;
    typename ipointList::const_iterator it_pp;

    irectangle rec1 = pPs1.computeBoundary();
    irectangle rec2 = pPs2.computeBoundary();
    //calculate the ofset of the two polygons
    int xMin = min(rec1.ul.x,rec2.ul.x);
    int xMax = max(rec1.br.x,rec2.br.x);
    int yMin = min(rec1.ul.y,rec2.ul.y);
    int yMax = max(rec1.br.y,rec2.br.y);
    //calculates the real size of the polygons without ofset
    int xSize = (xMax-xMin);
    int ySize = (yMax-yMin);
    //remove the ofset
    for (itpp=pPs1.begin();itpp!=pPs1.end();itpp++){
      (*itpp).x = (*itpp).x - xMin;
      (*itpp).y = (*itpp).y - yMin;
    }
    //remove the ofset
    for (itpp=pPs2.begin();itpp!=pPs2.end();itpp++){
      (*itpp).x = (*itpp).x - xMin;
      (*itpp).y = (*itpp).y - yMin;
    }

    std::list<borderPoints> objectList;
    std::list<borderPoints>::iterator it;

    objectsFromMask extractor;
    objectsFromMask::parameters extractorParams;
    extractorParams.level = -1;
    extractor.setParameters(extractorParams);

    draw <lti::channel8::value_type> drawer;
    channel8 channel;
    channel.resize(ySize+1,xSize+1,0,false,true);

    drawer.use(channel);
    drawer.setColor(255);
    //draw both polygons in one image
    drawer.set(*pPs1.begin());
    for (it_pp=pPs1.begin();it_pp!=pPs1.end();it_pp++){
      drawer.lineTo((*it_pp));
    }//close the polygon
    drawer.lineTo(*pPs1.begin());
    //draw the 2end polygon
    drawer.set(*pPs2.begin());
    for (it_pp=pPs2.begin();it_pp!=pPs2.end();it_pp++){
      drawer.lineTo((*it_pp));
    }//close the polygon
    drawer.lineTo(*pPs2.begin());

    extractor.apply(channel,objectList);
    if(objectList.size() == 1){
      it = objectList.begin();
      castFrom(*it,minLength,maxDistance);
      //add the offset to the borderPoints
      typename polygonPoints<T>::iterator it;
      for (it=this->begin();it!=this->end();it++) {
        (*it).x += xMin;
        (*it).y += yMin;
      }
    }
    else{
     copy(polygon1);
    }
    return *this;
  }
  */

  // Segmentation of Arcs into Simple Segments
  // Ramer or Duda and Hart method
  template<class T>
  polygonPoints<T>&
  polygonPoints<T>::approximate(const borderPoints& theBorderPoints,
                                const int minLength,
                                const double maxDistance,
                                const bool closed,
                                const bool searchMaxDist) {

    this->clear();

    if (theBorderPoints.empty()) {
      return *this;
    }

    vector< ipoint > pts;
    theBorderPoints.castTo(pts);

    int i,j;
    if (pts.size() < 4) {
      // three or less points are already the searched polygon
      for (i=0;i<pts.size();++i) {
        pointList<T>::push_back(pts.at(i));
      }
      return *this;
    }

    if (maxDistance>=0.0) {

      // 1. find the two points with the maximal distance to each other.
      //    They belong to the polygon representation
      const int last = pts.lastIdx();
      const int maxDistLast = searchMaxDist ? last : 1;
      int maxDist2 = -1;
      int tmp;

      int maxIdx1 = 0;
      int maxIdx2 = 0;
      for (i=0;i<maxDistLast;++i) {
        const ipoint& p = pts.at(i);
        for (j=i+1;j<=last;++j) {
          tmp = p.distanceSqr(pts.at(j));
          if ( tmp > maxDist2) {
            maxDist2 = tmp;
            maxIdx1 = i;
            maxIdx2 = j;
          }
        }
      }

      _lti_debug("Maximum length found between " << pts.at(maxIdx1) <<
                 " and " << pts.at(maxIdx2) << endl);
      
      // 2. reorder the list making a cyclic shift the original
      //    list in order to get two "adjacent" lists of points.
      vector < ipoint > twolst;
      twolst.resize(pts.size()+1,ipoint(),AllocateOnly);

      twolst.fill(pts,0,last,maxIdx1);
      twolst.fill(pts,pts.size()-maxIdx1);
      twolst.at(pts.size()) = twolst.at(0);

      // adapt the two indices to the new list
      maxIdx2 -= maxIdx1;
      maxIdx1 =  0;

      // 3. Begin with the "recursions"
      vector<ubyte> flags(twolst.size(),ubyte(0));
      // the first and second points of the maximal axis belong to the polygon
      flags.at(maxIdx1)=1;
      flags.at(maxIdx2)=1;
      if (closed) {
        // closed means, we want the last point of the polygon too
        flags.at(flags.size()-2)=1;
      }

      fitAndSplit(twolst,maxIdx1,maxIdx2,max(0.0,maxDistance),flags);
      fitAndSplit(twolst,maxIdx2,twolst.lastIdx(),max(0.0,maxDistance),flags);

      // now create the actual point list respecting the "original" sequence
      const int theMinLength = (minLength > 0) ? minLength : flags.size();
      i=0;
      j=0;
      while(i<flags.lastIdx()) { // the last elements was a dummy
        if ((flags.at(i) != 0) || (j>theMinLength)) {
          j=0;
          pointList<T>::push_back(twolst.at(i));
        } else {
          ++j;
        }
        ++i;
      }
    } else {
      const int theMinLength = (minLength > 0) ? minLength : 1;
      i=0;
      while(i<pts.size()) {
        pointList<T>::push_back(pts.at(i));
        i+=theMinLength;
      }
    }
    return *this;
  }

  // Segmentation of Arcs into Simple Segments
  // modified Ramer or Duda and Hart method
  template<class T>
  polygonPoints<T>&
  polygonPoints<T>::approximate(const borderPoints& theBorderPoints,
                                const ipointList& forcedVertices,
                                const int minLength,
                                const double maxDistance,
                                const bool closed,
                                const bool searchMaxDist) {

    this->clear();

    if (theBorderPoints.empty()) {
      return *this;
    }

    // are there any forced vertices?
    if (forcedVertices.empty()) {
      // no forced vertices -> normal approximate:
      return approximate(theBorderPoints,
                         minLength,maxDistance,closed,searchMaxDist);
    }

    // border points as vector to accelerate access.
    vector< ipoint > pts;
    theBorderPoints.castTo(pts);

    int i,j;
    if (pts.size() < 4) {
      // three or less points are already the searched polygon,
      // and if there are forced vertices, they MUST be in the three point
      // set.
      for (i=0;i<pts.size();++i) {
        pointList<T>::push_back(pts.at(i));
      }
      return *this;
    }

    // 1. find the two points with the maximal distance to each other.
    //    They belong to the polygon representation
    const int last = pts.lastIdx();
    vector< ubyte > tmpflags(pts.size()+1,ubyte(0));
    ipointList::const_iterator fvit,efvit=forcedVertices.end();
    
    // localize which points must be vertices.
    for (i=0,fvit=forcedVertices.begin();i<=last;++i) {
      const ipoint& p = pts.at(i);
      if ((fvit != efvit) && (p == (*fvit))) {
        tmpflags.at(i) = 1;
        ++fvit;
      }
    }
    
    while (fvit != efvit) {        
      // hmm, the user ignored the warning, and the points in forcedVertices
      // seem to be in a different order than in theBorderPoints. 
      // We need to search for the points in another sorted order and the
      // user has to wait!  (but he was warned!)
      bool notBorderPointFlag = true;
      for (i=0;i<last;++i) {
        const ipoint& p = pts.at(i);
        if ((fvit != efvit) && (p == (*fvit))) {
          tmpflags.at(i) = 1;
          ++fvit;
          notBorderPointFlag = false;
        }
      }
      if (notBorderPointFlag) {
        // hmm, even worst!  One of the forced vertices is not even a point
        // in the boundary, so, let's just ignore this point.
        ++fvit;
      }
    }
    
    // try to approximate the polygon or should only the minStep be used?
    if (maxDistance>=0.0) {

      // search only if necessary
      const int maxDistLast = searchMaxDist ? last : 1;

      int maxDist2 = -1;
      int tmp;

      int maxIdx1 = 0;
      int maxIdx2 = 0;

      
      for (i=0;i<maxDistLast;++i) {
        const ipoint& p = pts.at(i);

        for (j=i+1;j<=last;++j) {
          tmp = p.distanceSqr(pts.at(j));
          if ( tmp > maxDist2 ) {
            maxDist2 = tmp;
            maxIdx1 = i;
            maxIdx2 = j;
          }
        }
      }

      _lti_debug("Maximum length found between " << pts.at(maxIdx1) <<
                 " and " << pts.at(maxIdx2) << endl);
        
      // 2. reorder the list making a cyclic shift the original
      //    list in order to get two "adjacent" lists of points.
      vector< ubyte > flags(pts.size()+1,ubyte(0));
      vector< ipoint > twolst;
      twolst.resize(pts.size()+1,ipoint(),AllocateOnly);

      twolst.fill(pts,0,last,maxIdx1);
      twolst.fill(pts,pts.size()-maxIdx1);
      twolst.at(pts.size()) = twolst.at(0);

      flags.fill(tmpflags,0,last,maxIdx1);
      flags.fill(tmpflags,pts.size()-maxIdx1);
      flags.at(pts.size()) = flags.at(0);

      // adapt the two indices to the new list
      maxIdx2 -= maxIdx1;
      maxIdx1 =  0;

      // 3. Begin with the "recursions"
      // the first and second points of the maximal axis belong to the polygon
      flags.at(maxIdx1)=1;
      flags.at(maxIdx2)=1;
      if (closed) {
        // closed means, we want the last point of the polygon too
        flags.at(flags.size()-2)=1;
      }

      // the first half 
      int k1,k2;
      k1=maxIdx1;
      k2=k1+1;
      // check if any of the forced vertices need to be split
      while(k2<=maxIdx2) {
        while((k2<maxIdx2) && (flags.at(k2) == 0)) {
          ++k2;
        };
        fitAndSplit(twolst,k1,k2,max(0.0,maxDistance),flags);
        k1=k2;
        k2++;
      }
      
      // the second half
      k1=maxIdx2;
      k2=k1+1;
      while(k2<=twolst.lastIdx()) {
        while((k2<twolst.lastIdx()) && (flags.at(k2) == 0)) {
          ++k2;
        };
        fitAndSplit(twolst,k1,k2,max(0.0,maxDistance),flags);
        k1=k2;
        k2++;
      }

      // now create the actual point list respecting the "original" sequence
      const int theMinLength = (minLength > 0) ? minLength : flags.size();
      i=0;
      j=0;
      while(i<flags.lastIdx()) { // the last elements was a dummy
        if ((flags.at(i) != 0) || (j>theMinLength)) {
          j=0;
          pointList<T>::push_back(twolst.at(i));
        } else {
          ++j;
        }
        ++i;
      }
    } else {
      // minimum step and forced vertices:

      const int theMinLength = (minLength > 0) ? minLength : tmpflags.size();
      i=0;
      j=0;
      while(i<tmpflags.lastIdx()) { // the last element was a dummy
        if ((tmpflags.at(i) != 0) || (j>theMinLength)) {
          j=0;
          pointList<T>::push_back(pts.at(i));
        } else {
          ++j;
        }
        ++i;
      }
    }

    return *this;
  }

  template<class T>
  void polygonPoints<T>::fitAndSplit(const vector< ipoint >& vct,
                                      const int index1,
                                      const int index2,
                                      const double maxDistance,
                                      vector<ubyte>& flags) {

    // stack to avoid real recursion.  The x coordinate of each
    // point means "from" and the y coordinate "to"
    ipointList stack;
    ipoint interv(index1,index2);
    const int& idx1 = interv.x;  // reference to the x component of interv
    const int& idx2 = interv.y;  // reference to the y component of interv
    int i;
    int maxIdx;
    double maxDist;
    double dist;

    stack.push_back(interv);

    do {
      interv=stack.back();
      stack.pop_back();       // indirectly get the idx1 and idx2
                              // values from the stack

      _lti_debug("from " << idx1 << " to " << idx2 << endl)

      // precompute some values to accelerate calculations of distance
      // between the line segment defined by idx1 and idx2 and another point
      const point<double> a(vct.at(idx1));
      const point<double> b(vct.at(idx2));
      const point<double> d = b-a;
      const double norm = sqrt(d.absSqr());
      const double w0 = (b.x*a.y-b.y*a.x)/norm;
      const point<double> w(d.y/norm,-d.x/norm);

      // search for maximal distance to the line segment
      maxIdx=-1;
      maxDist=-1;
      for (i=idx1+1;i<idx2;++i) {
        dist = abs(w0 + w.x*vct.at(i).x + w.y*vct.at(i).y);
        if (dist>maxDist) {
          maxDist = dist;
          maxIdx = i;
        }
      }

      _lti_debug(" max distance " << maxDist << " at " << vct.at(maxIdx))

      if (maxDist > maxDistance) {
        _lti_debug(" -> recurse!")

        // two much distortion... recurse
        flags.at(maxIdx)++;
        if (idx2 != maxIdx) {
          stack.push_back(ipoint(maxIdx,idx2));
        }
        if (idx1 != maxIdx) {
          stack.push_back(ipoint(idx1,maxIdx));
        }
      }

      _lti_debug(endl)

    } while (!stack.empty());
  }

  template<class T>
  T polygonPoints<T>::areaX2() const {
    T acc = 0;
    if (this->size()<3) {
      return acc;
    }
    
    typename polygonPoints<T>::const_iterator it1,it2;
    for (it1=it2=this->begin(),++it2;it2!=this->end();++it2,++it1) {
      acc+=(((*it2).x-(*it1).x)*((*it2).y+(*it1).y));
    }
    it2=this->begin();
    acc+=(((*it2).x-(*it1).x)*((*it2).y+(*it1).y));

    return acc;
  }
  
  template<class T>
  bool polygonPoints<T>::clockwise() const {
    return(areaX2()>0);
  }

} // end of namespace lti::
