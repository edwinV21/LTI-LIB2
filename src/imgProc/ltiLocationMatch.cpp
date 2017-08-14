/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiLocationMatch.cpp
 *         Contains the class locationMatch,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   08.01.2008
 *
 * revisions ..: $Id: ltiLocationMatch.cpp,v 1.1 2008-01-15 01:02:22 alvarado Exp $
 */

#include "ltiLocationMatch.h"
#include "ltiEuclidianDistantor.h"
#include "ltiKdTree.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // locationMatch::parameters
  // --------------------------------------------------

  // default constructor
  locationMatch::parameters::parameters()
    : functor::parameters() {
   
    bidirectional = true;
    threshold = 1.0;
    matchType = LinearSearch;
    bucketSize = 32;
  }

  // copy constructor
  locationMatch::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  locationMatch::parameters::~parameters() {
  }

  // copy member

  locationMatch::parameters&
  locationMatch::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
    bidirectional = other.bidirectional;
    threshold     = other.threshold;
    matchType     = other.matchType;
    bucketSize    = other.bucketSize;

    return *this;
  }

  // alias for copy method
  locationMatch::parameters&
  locationMatch::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& locationMatch::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  locationMatch::parameters*
  locationMatch::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  locationMatch::parameters*
  locationMatch::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool locationMatch::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"bidirectional",bidirectional);
      lti::write(handler,"threshold",threshold);
      lti::write(handler,"matchType",matchType);
      lti::write(handler,"bucketSize",bucketSize);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool locationMatch::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"bidirectional",bidirectional);
      lti::read(handler,"threshold",threshold);
      lti::read(handler,"matchType",matchType);
      lti::read(handler,"bucketSize",bucketSize);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // locationMatch
  // --------------------------------------------------

  // default constructor
  locationMatch::locationMatch()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  locationMatch::locationMatch(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  locationMatch::locationMatch(const locationMatch& other)
    : functor() {
    copy(other);
  }

  // destructor
  locationMatch::~locationMatch() {
  }

  // copy member
  locationMatch& 
  locationMatch::copy(const locationMatch& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  locationMatch&
  locationMatch::operator=(const locationMatch& other) {
    return (copy(other));
  }

  // class name
  const std::string& locationMatch::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  locationMatch* locationMatch::clone() const {
    return new locationMatch(*this);
  }

  // create a new instance
  locationMatch* locationMatch::newInstance() const {
    return new locationMatch();
  }

  // return parameters
  const locationMatch::parameters&
  locationMatch::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  // On place apply for type list<location>!
  bool locationMatch::apply(const list<location>& locsA,
                            const std::list<dvector>& vctsA,
                            const list<location>& locsB,
                            const std::list<dvector>& vctsB,
                            std::vector<location>& matchedLocsA,
                            std::vector<dvector>& descA,
                            std::vector<location>& matchedLocsB,
                            std::vector<dvector>& descB) {
    
    const parameters& par = getParameters();
    switch (par.matchType) {
      case LinearSearch:
        return linearSearch(locsA,vctsA,
                            locsB,vctsB,
                            matchedLocsA,descA,
                            matchedLocsB,descB);
        break;
      case KdTreeBased:
        return kdtree(locsA,vctsA,
                      locsB,vctsB,
                      matchedLocsA,descA,
                      matchedLocsB,descB);
        break;
      default:
        setStatusString("Unknown matching algorithm");
        return false;
    }
  }

  template<class T,class L>
  void locationMatch::convert(const L& theList,
                              std::vector<T>& theVct) const {

    typename L::const_iterator it;
    typename std::vector<T>::iterator vit;
    const typename L::const_iterator eit = theList.end();

    theVct.resize(theList.size());

    for (it=theList.begin(),vit=theVct.begin();it!=eit;++it,++vit) {
      (*vit)=(*it);
    }
    
  }

  bool locationMatch::linearSearch(const list<location>& locsA,
                                   const std::list<dvector>& vctsA,
                                   const list<location>& locsB,
                                   const std::list<dvector>& vctsB,
                                   std::vector<location>& matchedLocsA,
                                   std::vector<dvector>& descA,
                                   std::vector<location>& matchedLocsB,
                                   std::vector<dvector>& descB) {

    const parameters& par = getParameters();

    const double thresh = sqr(par.threshold);

    // vectorize
    std::vector<location> vlocsA,vlocsB;
    std::vector<dvector> vvctA,vvctB;

    convert(locsA,vlocsA);
    convert(locsB,vlocsB);
    convert(vctsA,vvctA);
    convert(vctsB,vvctB);

    // indices
    ivector va(static_cast<const int>(vlocsA.size()),-1);
    ivector vb(static_cast<const int>(vlocsB.size()),-1);

    dvector dva(va.size(),std::numeric_limits<double>::max());
    dvector dvb(vb.size(),std::numeric_limits<double>::max());

    euclidianSqrDistantor< dvector > sqrDist;

    for (int i=0;i<va.size();++i) {
      const dvector& avct = vvctA[i];
      int minIdx=-1;
      for (int j=0;j<vb.size();++j) {
        const dvector& bvct = vvctB[j];
        double d = sqrDist(avct,bvct);
        if ( d < dvb.at(j) ) {
          dvb.at(j) = d;
          minIdx    = j;
        }
      }
      
      // at this point the closest element to vct i is minIdx
      if ( (minIdx>=0) &&
           (dvb.at(minIdx) <= thresh) &&
           (dvb.at(minIdx) <= dva.at(i)) ) {
        va.at(i)      = minIdx;
        vb.at(minIdx) = i;
        dva.at(i)     = dvb.at(minIdx);
      }
    }

    matchedLocsA.clear();
    matchedLocsB.clear();
    
    matchedLocsA.reserve(vlocsA.size());
    matchedLocsB.reserve(vlocsB.size());

    descA.clear();
    descA.reserve(vlocsA.size());

    descB.clear();
    descB.reserve(vlocsB.size());

    if (par.bidirectional) {
      // check that the opposite direction
      
      for (int j=0;j<vb.size();++j) {
        const dvector& bvct = vvctB[j];
        int minIdx=-1;
        double minDist = std::numeric_limits<double>::max();
        for (int i=0;i<va.size();++i) {
          const dvector& avct = vvctA[i];
          double d = sqrDist(avct,bvct);
          if ( d < minDist ) {
            minDist = d;
            minIdx  = i;
          }
        }

        if (j == va.at(minIdx)) {
          int a = minIdx;
          int b = j;

          matchedLocsA.push_back( vlocsA[a] );
          descA.push_back( vvctA[a] );

          matchedLocsB.push_back( vlocsB[b] );
          descB.push_back( vvctB[b] );          
        }
      }
    } else {
      for (int i=0;i<va.size();++i) {
        if (va.at(i) >= 0) {
          int a = i;
          int b = va.at(i);

          matchedLocsA.push_back( vlocsA[a] );
          descA.push_back( vvctA[a] );

          matchedLocsB.push_back( vlocsB[b] );
          descB.push_back( vvctB[b] );          
        }
      }
    }

    return true;
  }

  bool locationMatch::kdtree(const list<location>& locsA,
                             const std::list<dvector>& vctsA,
                             const list<location>& locsB,
                             const std::list<dvector>& vctsB,
                             std::vector<location>& matchedLocsA,
                             std::vector<dvector>& descA,
                             std::vector<location>& matchedLocsB,
                             std::vector<dvector>& descB) {

    // vectorize
    std::vector<location> vlocsA,vlocsB;
    std::vector<dvector> vvctA,vvctB;

    convert(locsA,vlocsA);
    convert(locsB,vlocsB);
    convert(vctsA,vvctA);
    convert(vctsB,vvctB);

    matchedLocsA.clear();
    matchedLocsB.clear();
    
    matchedLocsA.reserve(vlocsA.size());
    matchedLocsB.reserve(vlocsB.size());

    descA.clear();
    descA.reserve(vlocsA.size());

    descB.clear();
    descB.reserve(vlocsB.size());

    // create kd-trees for both descriptor lists
    typedef kdTree<dvector,int> tree_type;
    typedef tree_type::element element_type;

    tree_type treeA,treeB;

    unsigned int u;
    for (u=0;u<vvctA.size();++u) {
      treeA.add(vvctA[u],int(u));
    }
    
    for (u=0;u<vvctB.size();++u) {
      treeB.add(vvctB[u],int(u));
    }

    const parameters& par = getParameters();
    const double thresh = sqr(par.threshold);

    // create both trees
    treeA.build(par.bucketSize);
    treeB.build(par.bucketSize);

    // now search for the closest match 
    if (par.bidirectional) {

      double dist;
      element_type* ePtr;
      element_type* ePtr2;
      // bidirectional search
      for (u=0;u<vvctA.size();++u) {
        if ( treeB.searchNearest(vvctA[u],ePtr,dist) &&
             (dist<=thresh) && 
             // we found a candidate.
             // since this is the bidirectional method, the
             // location match is kept if and only if it is also the
             // nearest location in the other direction
             (treeA.searchNearest(ePtr->point,ePtr2,dist)) &&
             (ePtr2->data == static_cast<int>(u)) ) {

          matchedLocsA.push_back( vlocsA[u] );
          descA.push_back( vvctA[u] );

          matchedLocsB.push_back( vlocsB[ePtr->data] );
          descB.push_back( ePtr->point );
        }
      }
      
    } else {

      double dist;
      element_type* ePtr;
      // unidirectional search
      for (u=0;u<vvctA.size();++u) {
        if ( treeB.searchNearest(vvctA[u],ePtr,dist) &&
             (dist<=thresh) ) {

          // we found a candidate
          matchedLocsA.push_back( vlocsA[u] );
          descA.push_back( vvctA[u] );

          matchedLocsB.push_back( vlocsB[ePtr->data] );
          descB.push_back( ePtr->point );
        }
      }
    }

    return true;
  }
  
  /**
   * Read a surfLocalDescriptor::eOrientationMode value
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            locationMatch::eMatchType& data) {
    std::string str;
    
    if (handler.read(str)) {
      if (str.find("Tree") != std::string::npos) {
        data = locationMatch::KdTreeBased;
      } else if (str.find("Linear")) {
        data = locationMatch::LinearSearch;
      }
      return true;
    }

    return false;
  }

  /**
   * Write a surfLocalDescriptor::eOrientationMode value
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const locationMatch::eMatchType& data) {
    bool b = false;
    switch(data) {
      case locationMatch::KdTreeBased:
        b = handler.write("KdTreeBased");
        break;
      case locationMatch::LinearSearch:
        b = handler.write("LinearSearch");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined eLocationSelectionMode");
        break;
    }

    return b;
  }


}

