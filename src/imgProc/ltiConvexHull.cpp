/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiConvexHull.cpp
 * \author Pablo Alvarado
 * \date   16.11.2002
 * 
 * $Id: ltiConvexHull.cpp,v 1.7 2007-10-08 20:19:47 alvarado Exp $
 */

#include "ltiConvexHull.h"
#include "ltiSort.h"
#include "ltiGeometry.h"

namespace lti {
  // --------------------------------------------------
  // convexHull::parameters
  // --------------------------------------------------

  // default constructor
  convexHull::parameters::parameters()
    : functor::parameters() {
  }

  // copy constructor
  convexHull::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  convexHull::parameters::~parameters() {
  }

  // get type name
  const std::string& convexHull::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  convexHull::parameters&
    convexHull::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    return *this;
  }

  // alias for copy member
  convexHull::parameters&
    convexHull::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  convexHull::parameters* convexHull::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member
  convexHull::parameters* convexHull::parameters::newInstance() const {
    return new parameters;
  }
  
  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool convexHull::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool convexHull::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // convexHull
  // --------------------------------------------------

  // default constructor
  convexHull::convexHull()
    : functor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  convexHull::convexHull(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  convexHull::convexHull(const convexHull& other)
    : functor()  {
    copy(other);
  }

  // destructor
  convexHull::~convexHull() {
  }

  // returns the name of this type
  const std::string& convexHull::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  convexHull& convexHull::copy(const convexHull& other) {
    functor::copy(other);
    
    return (*this);
  }

  // alias for copy member
  convexHull&
    convexHull::operator=(const convexHull& other) {
    return (copy(other));
  }


  // clone member
  convexHull* convexHull::clone() const {
    return new convexHull(*this);
  }
  
  // clone member
  convexHull* convexHull::newInstance() const {
    return new convexHull;
  }

  // return parameters
  const convexHull::parameters&
    convexHull::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  /*
   * operates on a copy of the given %parameters.
   * @param src pointList with the source data.
   * @param dest pointList where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool convexHull::apply(const ipointList& src,ipolygonPoints& dest) const {
    computeConvexHull(src,dest);
    return true;
  }

  /*
   * operates on a copy of the given %parameters.
   * @param src pointList with the source data.
   * @param dest pointList where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool convexHull::apply(const ioPoints& src,ipolygonPoints& dest) const {
    dest.castFrom(src);
    return true;
  }

  /*
   * operates on a copy of the given %parameters.
   * @param src pointList with the source data.
   * @param dest pointList where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool convexHull::apply(const pointList<float>& src,
                         polygonPoints<float>& dest) const {
    computeConvexHull(src,dest);
    return true;
  }

  /*
   * operates on a copy of the given %parameters.
   * @param src pointList with the source data.
   * @param dest pointList where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool convexHull::apply(const pointList<double>& src,
                         polygonPoints<double>& dest) const {
    computeConvexHull(src,dest);
    return true;
  }

  template<typename T>
  polygonPoints<T>& convexHull::computeConvexHull(const pointList<T>& thePointList,
                                                  polygonPoints<T>& thePolygonPoints) const {

    
    // _lti_debug("Called castFrom(pointList)\n");
    thePolygonPoints.clear();

    if (thePointList.empty()) {
      return thePolygonPoints;  // thePointList is empty!  We are ready!
    }
 
    vector< point<T> > vPts;
    thePointList.castTo(vPts);
    sort sorter(Ascending); // sort in ascending order
    sorter.apply(vPts);                 // sort the points

    if (vPts.size() < 3) {
      thePolygonPoints.pointList<T>::castFrom(vPts);
      return thePolygonPoints;
    }

    // now use the following ivector to store the indices of the points
    // in the vector
    ivector lidx;
    lidx.resize(vPts.size(),int(),AllocateOnly);
    lidx.at(0)=0;
    lidx.at(1)=1;
    int sptr = 2;
    int i;

    // first try the right side
    for (i=2;i<vPts.size();++i) {
      lidx.at(sptr)=i;
      sptr++;
      while ((sptr>2) && (clockwiseTurn(vPts.at(lidx.at(sptr-3)),
                                        vPts.at(lidx.at(sptr-2)),
                                        vPts.at(lidx.at(sptr-1))) <= 0))  {
        // on not a right turn, delete the middle point
        lidx.at(sptr-2) = lidx.at(sptr-1);
        sptr--;
      }
    }

    for (i=0;i<sptr;++i) {
      thePolygonPoints.push_back(vPts.at(lidx.at(i)));
    }

    // and now the left side
    lidx.at(0)=vPts.lastIdx();
    lidx.at(1)=vPts.lastIdx()-1;
    sptr = 2;

    for (i=vPts.lastIdx()-2;i>=0;--i) {
      lidx.at(sptr)=i;
      sptr++;
      while ((sptr>2) && (clockwiseTurn(vPts.at(lidx.at(sptr-3)),
                                        vPts.at(lidx.at(sptr-2)),
                                        vPts.at(lidx.at(sptr-1))) <= 0))  {
        // on not a right turn, delete the middle point
        lidx.at(sptr-2) = lidx.at(sptr-1);
        sptr--;
      }
    }

    sptr--;
    for (i=1;i<sptr;++i) {
      thePolygonPoints.push_back(vPts.at(lidx.at(i)));
    }

    return thePolygonPoints;
  }


}
