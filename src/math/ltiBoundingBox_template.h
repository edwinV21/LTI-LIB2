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
 * \file   ltiBoundingBox_template.h
 * \author Jochen Wickel
 * \date   25.9.2000
 *
 * $Id: ltiBoundingBox_template.h,v 1.1 2005-07-22 17:52:25 gquiros Exp $
 */

#include "ltiSecondOrderStatistics.h"

namespace lti {
  
  // --------------------------------------------------
  // boundingBox::parameters
  // --------------------------------------------------
  
  // default constructor
  template <typename T>
  boundingBox<T>::parameters::parameters()
    : functor::parameters() {
    dataRepresentationType = Rows;
  };
  
  // copy constructor
  template <typename T>
  boundingBox<T>::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  };
  
  // destructor
  template <typename T>
  boundingBox<T>::parameters::~parameters() {};

  // returns the name of this type
  template <typename T>
  const std::string& boundingBox<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T>
  typename boundingBox<T>::parameters& 
  boundingBox<T>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    dataRepresentationType = other.dataRepresentationType;
    return *this;
  };
  
  // clone member
  template <typename T>
  typename boundingBox<T>::parameters* 
  boundingBox<T>::parameters::clone() const {
    return new parameters(*this);
  };

  // new instance member
  template <typename T>
  typename boundingBox<T>::parameters* 
  boundingBox<T>::parameters::newInstance() const {
    return new parameters;
  };

  // --------------------------------------------------
  // boundingBox
  // --------------------------------------------------

  // default constructor
  template <class T>
  boundingBox<T>::boundingBox()
    : functor() {
    parameters p;
    setParameters(p);
  }

  // copy constructor
  template <class T>
  boundingBox<T>::boundingBox(const boundingBox& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class T>
  boundingBox<T>::~boundingBox() {
  }

  // returns the name of this type
  template <class T> 
  const std::string& boundingBox<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <class T>
  boundingBox<T>& boundingBox<T>::copy(const boundingBox<T>& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  boundingBox<T>* boundingBox<T>::clone() const {
    return new boundingBox<T>(*this);
  }

  // new instance member
  template <class T>
  boundingBox<T>* boundingBox<T>::newInstance() const {
    return new boundingBox<T>;
  }
  
  // return parameters
  template <class T>
  const typename boundingBox<T>::parameters&
  boundingBox<T>::getParameters() const {
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

  // On copy apply for type matrix<T>!
  template <class T>
  void boundingBox<T>::apply(const matrix<T>& src,
                             vector<T>& min,
                             vector<T>& max) const {
    if (getParameters().dataRepresentationType == Rows) {
      return boundsOfRows(src,min,max);
    }
    else {
      return boundsOfColumns(src,min,max);
    }
  };

  template <class T>
  void boundingBox<T>::min(const matrix<T>& src, vector<T>& dest) const {
    if (getParameters().dataRepresentationType == Rows) {
      return minOfRows(src,dest);
    }
    else {
      return minOfColumns(src,dest);
    }
  }

  template <class T>
  void boundingBox<T>::max(const matrix<T>& src, vector<T>& dest) const {
    if (getParameters().dataRepresentationType == Rows) {
      return maxOfRows(src,dest);
    }
    else {
      return maxOfColumns(src,dest);
    }
  }
  
  template <class T>
  void boundingBox<T>::minOfRows(const matrix<T>& src,
                            vector<T>& dest) const {
    dest.resize(src.columns(),AllocateOnly);
    vector<T> tmp;
    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      dest[i]=tmp.findMinimum();
    }
  }

  template <class T>
  void boundingBox<T>::minOfColumns(const matrix<T>& src,
                               vector<T>& dest) const {
    dest.resize(src.rows(),AllocateOnly);
    for (int i=0; i<src.rows(); i++) {
      dest[i]=src.getRow(i).findMinimum();
    }
  }

  template <class T>
  void boundingBox<T>::maxOfRows(const matrix<T>& src,
                            vector<T>& dest) const {
    dest.resize(src.columns(),AllocateOnly);
    vector<T> tmp;
    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      dest[i]=tmp.findMaximum();
    }
  }

  template <class T>
  void boundingBox<T>::maxOfColumns(const matrix<T>& src,
                               vector<T>& dest) const {
    dest.resize(src.rows(),AllocateOnly);
    for (int i=0; i<src.rows(); i++) {
      dest[i]=src.getRow(i).findMaximum();
    }
  }


  template <class T>
  void boundingBox<T>::boundsOfRows(const matrix<T>& src,
                               vector<T>& min,
                               vector<T>& max) const {
    min.resize(src.columns(),AllocateOnly);
    max.resize(src.columns(),AllocateOnly);
    vector<T> tmp;
    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      max[i]=tmp.findMaximum();
      min[i]=tmp.findMinimum();
    }
  }

  template <class T>
  void boundingBox<T>::boundsOfColumns(const matrix<T>& src,
                                  vector<T>& min,
                                  vector<T>& max) const {
    min.resize(src.rows(),AllocateOnly);
    max.resize(src.rows(),AllocateOnly);
    for (int i=0; i<src.rows(); i++) {
      min[i]=src.getRow(i).findMinimum();
      max[i]=src.getRow(i).findMaximum();
    }
  }

  template <class T>
  void boundingBox<T>::min(const vector<T>& a, const vector<T>& b,
                      vector<T> &result) const {
    assert(a.size() == b.size());

    result.resize(a.size());

    typename vector<T>::const_iterator ia=a.begin();
    typename vector<T>::const_iterator ib=b.begin();
    typename vector<T>::iterator ir=result.begin();
    typename vector<T>::const_iterator aend=a.end();
    while (ia != aend) {
      *ir++=lti::min(*ia++,*ib++);
    }
  }

  template <class T>
  void boundingBox<T>::max(const vector<T>& a, const vector<T>& b,
                      vector<T> &result) const {
    assert(a.size() == b.size());

    result.resize(a.size());

    typename vector<T>::const_iterator ia=a.begin();
    typename vector<T>::const_iterator ib=b.begin();
    typename vector<T>::iterator ir=result.begin();
    typename vector<T>::const_iterator aend=a.end();
    while (ia != aend) {
      *ir++=lti::max(*ia++,*ib++);
    }
  }

  template <class T>
  void boundingBox<T>::clip(vector<T>& a,
                       const vector<T>& lower,
                       const vector<T>& upper) const {
    assert(lower.size() == upper.size());
    assert(a.size() == upper.size());
    typename vector<T>::const_iterator ilow=lower.begin();
    typename vector<T>::const_iterator iup=upper.begin();
    typename vector<T>::iterator ir=a.begin();
    typename vector<T>::iterator rend=a.end();
    while (ir != rend) {
      if (*ir < *ilow) {
        *ir=*ilow;
      } else if (*ir > *iup) {
        *ir=*iup;
      }
      ir++;
      ilow++;
      iup++;
    }
  }

  template <class T>
  void boundingBox<T>::clip(matrix<T>& a,
                       const matrix<T>& lower,
                       const matrix<T>& upper) const {
    assert(lower.size() == upper.size());
    assert(a.size() == upper.size());
    typename matrix<T>::const_iterator ilow=lower.begin();
    typename matrix<T>::const_iterator iup=upper.begin();
    typename matrix<T>::iterator ir=a.begin();
    typename matrix<T>::iterator rend=a.end();
    while (ir != rend) {
      if (*ir < *ilow) {
        *ir=*ilow;
      } else if (*ir > *(this->ihigh)) {
        *ir=*(this->ihigh);
      }
      ir++;
      ilow++;
      this->ihigh++;
    }
  }

}


