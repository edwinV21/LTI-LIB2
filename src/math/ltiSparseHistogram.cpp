/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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
 * \file   ltiSparseHistogram.cpp
 * \author Jochen Wickel
 * \date   30.10.2001
 *
 * $Id: ltiSparseHistogram.cpp,v 1.4 2006-09-13 12:04:59 doerfler Exp $
 */

#include "ltiObject.h"
#include <limits>
#include <cstdio>
#include "ltiSparseHistogram.h"
#include "ltiMath.h"

// This stupid so-called C++ compiler from MS sucks!
#ifdef _LTI_MSC_6
#undef min
#undef max
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // sparseHistogram
  // --------------------------------------------------

  sparseHistogram::sparseHistogram() {
    resize(0,1);
  }
  

  sparseHistogram::sparseHistogram(const int dim, const int n) { 
    resize(dim,n);
  }

  sparseHistogram::sparseHistogram(const int n,
                                   const dvector& min,
                                   const dvector& max) {
    resize(n,min,max);
  }
  

  sparseHistogram::sparseHistogram(const ivector& bn) {
    resize(bn);
  }

  sparseHistogram::sparseHistogram(const ivector& bn,
                                   const dvector& min,
                                   const dvector& max) {
    resize(bn,min,max);
  }
  
  sparseHistogram::~sparseHistogram() {
    clear();
  }

  sparseHistogram::sparseHistogram(const sparseHistogram& other) 
    : container() {
    copy(other);
  }

  const std::string& sparseHistogram::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Clear the previous content of the histogram and resize it
   * to the given dimensions and bins per dimensions.
   *
   * The maximum number of bins per dimension is limited to 64.
   *
   * @param dim number of dimensions
   * @param numberOfBins number of bins per dimension
   */
  void sparseHistogram::resize(const int dim, const int numberOfBins) {
    bins.resize(dim,numberOfBins,Init);
    dvector min(dim,0.0);
    dvector max(dim,double(numberOfBins));
    resize(bins,min,max);
  }

  /*
   * Clear and resize the histogram to the number of dimensions equal to
   * the size of the vector bins, and having at each dimension the number
   * of bins given at each component of bins vector.
   *
   * The maximum number of bins per dimension is limited to 64.
   */
  void sparseHistogram::resize(const ivector& bn) {
    dvector min(bn.size(),0.0);
    dvector max;
    max.castFrom(bn);
    resize(bins,min,max);
  }

  /*
   * Clear and resize the histogram.
   *
   * The new histogram will have the given numbers of bins per dimension, for
   * a number of dimensions equal min.size() or max.size() (which must have
   * the same number of elements).
   * Furthermore, you must give the lower and upper bounds
   * of the hyperbox which is supposed to be occupied by the
   * histogram.
   * This will be used to access the histogram by vectors with
   * double elements.
   *
   * The maximum number of bins per dimension is limited to 64.
   */
  void sparseHistogram::resize(const int n,
                               const dvector& min,
                               const dvector& max) {
    bins.resize(min.size(),n,Init);
    resize(bins,min,max);
  }

  /*
   * Clear and resize the histogram
   *
   * The first vector gives the number of bins for each dimension.
   * The second and third vector give the lower and upper bounds of
   * the hyperbox which is supposed to be occupied by the histogram.
   * This will be used to access the histogram by vectors with
   * double elements.
   *
   * The maximum number of bins per dimension is limited to 64.
   */
  void sparseHistogram::resize(const ivector& bn,
                               const dvector& min,
                               const dvector& max) {
    //assert(bn.findMaximum() < maxN);
    assert(min.size() == max.size());
    assert(bn.size() == max.size());

    clear();

    bins=bn;
    // compute transform to get an index vector from a value vector
    offset=min;
    dvector tmp=max;
    tmp.subtract(min);
    scale.castFrom(bins);
    dvector::iterator i;
    for (i=tmp.begin(); i != tmp.end(); i++) {
      // avoid division by zero error
      if (lti::abs(*i) < std::numeric_limits<double>::min()) {
        (*i)=1.0;
      }
    }
    scale.edivide(tmp);
    minIndex.resize(min.size(),0,Init);
    maxIndex=bins;
    maxIndex.add(-1);

    _lti_debug("MinIndex = " << minIndex << "\n");
    _lti_debug("MaxIndex = " << maxIndex << "\n");
    _lti_debug("Offset = " << offset << "\n");
    _lti_debug("Scale = " << scale << "\n");
  }

  sparseHistogram& sparseHistogram::copy(const sparseHistogram& other) {

    clear();

    bins=other.bins;
    scale=other.scale;
    offset=other.offset;
    // copy the core
    for (mapType::const_iterator i=other.core.begin();
         i != other.core.end();
         ++i) {
      core[i->first] = i->second;
    }

    return *this;
  }

  void sparseHistogram::clear() {
    core.clear();
  }

  const float& sparseHistogram::at(const ivector& a) const {
    static float zero = 0;
    mapType::const_iterator i=core.find(a);
    if (i != core.end()) {
      return i->second;
    } else {
      return zero;
    }
  }

  float& sparseHistogram::at(const ivector& a) {
    mapType::iterator i=core.find(a);
    if (i != core.end()) {
      return (*i).second;
    } else {
      core[a]=0;
      return core[a];
    }
  }


  float sparseHistogram::get(const ivector& a) const {
    mapType::const_iterator i=core.find(a);
    if (i != core.end()) {
      return i->second;
    } else {
      return 0;
    }
  }

  void sparseHistogram::put(const ivector& a, float v) {
    mapType::iterator i=core.find(a);
    if (i == core.end()) {
      core[a]=v;
    } else {
      i->second=v;
    }
  }

  void sparseHistogram::clear(const ivector& a) {
    mapType::iterator i=core.find(a);
    if (i != core.end()) {
      core.erase(i);
    }
  }

  void sparseHistogram::add(const ivector& a, float v) {
    mapType::iterator i=core.find(a);
    if (i == core.end()) {
      core[a]=v;
    } else {
      i->second+=v;
    }
  }

  void sparseHistogram::multiply(const ivector& a, const float& v) {
    mapType::iterator i=core.find(a);
    if (i != core.end()) {
      i->second*=v;
    }
  }

  void sparseHistogram::divide(const ivector& a, const float& v) {
    mapType::iterator i=core.find(a);
    if (i != core.end()) {
      i->second/=v;
    }
  }

  void sparseHistogram::divide(const float& sum) {
    for (mapType::iterator i=core.begin(); i != core.end(); i++) {
      i->second/=sum;
    }
  }

  sparseHistogram* sparseHistogram::clone() const {
    return new sparseHistogram(*this);
  }

  sparseHistogram* sparseHistogram::newInstance() const {
    return new sparseHistogram;
  }

  bool sparseHistogram::write(ioHandler& handler,
                              const bool complete) const {
    bool b=true;
    if (complete) {
      b=handler.writeBegin();
    }
    if (b) {
      b=b && lti::write(handler, "bins", bins);
      b=b && lti::write(handler, "offset", offset);
      b=b && lti::write(handler, "scale", scale);
      b=b && lti::write(handler, "core", static_cast<int>(core.size()));
      b=b && handler.writeBegin();
      for (mapType::const_iterator i=core.begin(); i != core.end(); i++) {
        b=b && handler.writeBegin();
        b=b && i->first.write(handler);
        b=b && handler.writeKeyValueSeparator();
        b=b && handler.write(i->second);
        b=b && handler.writeEnd();
      }
      b=b && handler.writeEnd();
    }
    if (complete) {
      b=b && handler.writeEnd();
    }
    return b;
  }

  bool sparseHistogram::read(ioHandler& handler,
                             const bool complete) {

    clear();
    bool b=true;
    if (complete) {
      b=handler.readBegin();
    }
    if (b) {
      b=b && lti::read(handler, "bins", bins);
      b=b && lti::read(handler, "offset", offset);
      b=b && lti::read(handler, "scale", scale);
      int n;
      b=b && lti::read(handler, "core", n);
      clear();
      b=b && handler.readBegin();
      for (int i=0; i<n; i++) {
        b=b && handler.readBegin();
        ivector key;
        b=b && key.read(handler);
        float value;
        b=b && handler.read(value);
        core[key]=value;
        b=b && handler.readEnd();
      }
      minIndex.resize(bins.size());
      minIndex.fill(0);
      maxIndex=bins;
      maxIndex.add(-1);

      b=b && handler.readEnd();
    }
    if (complete) {
      b=handler.readEnd();
    }
    return b;
  }

  const ivector& sparseHistogram::convertIndex(const dvector& d) const {
    static ivector tmpIndex;
    const int size = d.size();
    
    for(int i = 0; i < size; ++i){
      int index  = static_cast<int>((d.at(i) - offset.at(i)) * scale.at(i));
      index = max(index,minIndex.at(i));
      index = min(index,maxIndex.at(i));
      tmpIndex.at(i) = index;
    }
    return tmpIndex;
  }

}
