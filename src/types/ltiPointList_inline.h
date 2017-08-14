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
 * \file   ltiPointList_inline.h
 *         Defines a class for lists of points
 * \author Suat Akyol
 * \date   28.06.2000
 *
 * $Id: ltiPointList_inline.h,v 1.6 2012-11-17 14:49:33 alvarado Exp $
 */

namespace lti {
  
  template<typename T>
  template<typename U>
  inline pointList<T>& pointList<T>::castFrom(const pointList<U>& other) {
    this->clear();
    typename pointList<U>::const_iterator it,eit;
    for (it=other.begin(),eit=other.end();it!=eit;++it) {
      this->push_back(point<T>(static_cast<T>((*it).x),
			       static_cast<T>((*it).y)));
    }
    return (*this);
  }

  template<typename T>
  template<typename U>
  inline pointList<T>& pointList<T>::castFrom(const vector< point<U> >& other) {
    this->clear();
    typename vector< point<U> >::const_iterator it,eit;
    for (it=other.begin(),eit=other.end();it!=eit;++it) {
      this->push_back(*it);
    }
    return *this;
  }

  template<typename T>
  template<typename U>
  inline pointList<T>& pointList<T>::castFrom(const vector< point<U> >& other) const {
    this->clear();
    typename vector< point<U> >::const_iterator it,eit;
    for (it=other.begin(),eit=other.end();it!=eit;++it) {
      this->push_back(*it);
    }
    return *this;
  }
  
  template<typename T>
  template<typename U>
  inline void pointList<T>::castTo(vector< point<U> >& other) const {
    other.resize(this->size(),point<U>(),AllocateOnly);
    typename pointList<T>::const_iterator it;
    int i;
    for (i=0,it=this->begin();i<other.size();++i,++it) {
      other.at(i).copy(*it);
    }
  }
  
  template<typename T>
  inline const rectangle<T>& pointList<T>::getBoundary() const { 
    return boundary_; 
  }
  
  template<typename T>
  inline void pointList<T>::setBoundary(const rectangle<T>& r) {
    boundary_.copy(r);
  }
  
}
