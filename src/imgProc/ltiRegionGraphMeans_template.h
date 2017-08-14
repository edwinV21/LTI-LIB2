/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
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

/*
 * \file   ltiRegionGraphMeans_template.h
 *         Graph merging class according to the mean value of image regions.
 * \author Pablo Alvarado
 * \date   25.10.2003
 *
 * revisions ..: $Id: ltiRegionGraphMeans_template.h,v 1.3 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiSplitImageToRGB.h"

namespace lti {

  // --------------------------------------------------
  // regionGraphMeansNode
  // --------------------------------------------------
  template<class T> 
  regionGraphMeansNode<T>::regionGraphMeansNode() 
    : sumOfElements_(T()),n_(0) {    
  }

  template<class T> 
  regionGraphMeansNode<T>::~regionGraphMeansNode() {
  }

  template<class T> 
  regionGraphMeansNode<T>::regionGraphMeansNode(
                                       const regionGraphMeansNode<T>& other) 
    : ioObject(),sumOfElements_(other.sumOfElements_),n_(other.n_) {
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::operator+=(const regionGraphMeansNode<T>& other) {
    n_+=other.n_;
    sumOfElements_+=other.sumOfElements_;
    return *this;
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::operator=(const regionGraphMeansNode<T>& other) {
    return copy(other);
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::copy(const regionGraphMeansNode<T>& other) {
    n_=other.n_;
    sumOfElements_=other.sumOfElements_;
    return *this;
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::consider(const T& elem) {
    ++n_;
    sumOfElements_+=elem;
    return *this;
  }

  template<class T> 
  T regionGraphMeansNode<T>::computeMean() const {
    return sumOfElements_/static_cast<float>(n_);
  }

  template<class T> 
  int regionGraphMeansNode<T>::size() const {
    return n_;
  }

  // IO methods

  template<class T>
  bool regionGraphMeansNode<T>::read(ioHandler& handler,const bool complete) {
    if (complete) {
      handler.readBegin();
    }
    
    lti::read(handler,"sum",sumOfElements_);
    lti::read(handler,"n",n_);

    if (complete) {
      handler.readEnd();
    }
    return true;
  }

  template<class T> 
  bool regionGraphMeansNode<T>::write(ioHandler& handler,
                                      const bool complete) const {
    if (complete) {
      handler.writeBegin();
    }
    
    lti::write(handler,"sum",sumOfElements_);
    lti::write(handler,"n",n_);

    if (complete) {
      handler.writeEnd();
    }

    return true;
  }

  template<class T> 
  regionGraphMeansNode<T>* regionGraphMeansNode<T>::clone() const {
    return new regionGraphMeansNode<T>(*this);
  }

  template<class T> 
  regionGraphMeansNode<T>* regionGraphMeansNode<T>::newInstance() const {
    return new regionGraphMeansNode<T>();
  }

  template<class T> 
  const std::string& regionGraphMeansNode<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // --------------------------------------------------
  // regionGraphColor
  // --------------------------------------------------
  template<class F>
  regionGraphColor<F>::regionGraphColor() 
    : parent() {
  }

  template<class F>
  regionGraphColor<F>::regionGraphColor(const parameters& par) 
    : parent(par) {
  }

  template<class F>
  regionGraphColor<F>::regionGraphColor(const regionGraphColor& other)
    : parent() {
    this->copy(other);
  }

  template<class F>
  regionGraphColor<F>::~regionGraphColor() {
  }

  // returns the name of this type
  template<class F>
  const std::string& regionGraphColor<F>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template<class F>
  regionGraphColor<F>* regionGraphColor<F>::clone() const {
    return new regionGraphColor<F>(*this);
  }

  // clone member
  template<class F>
  regionGraphColor<F>* regionGraphColor<F>::newInstance() const {
    return new regionGraphColor<F>();
  }


  template<class F>
  bool regionGraphColor<F>::apply(const matrix<int>& regions,
                                  const image& image,
                                  const int minLabel,
                                  graph_type& graph) {
    
    if (regions.size() != image.size()) {
      this->setStatusString("image and labeled mask have different sizes");
      return false;
    }

    splitImageToRGB splitter;
    splitter.apply(image,c1,c2,c3); // c1,c2,c3 are attributes of the class
    
    return apply(regions,minLabel,graph);
  }

  template<class F>
  bool regionGraphColor<F>::generate(const matrix<int>& regions,
                                     const image& image,
                                     const int minLabel,
                                     graph_type& graph) {
    return apply(regions,image,minLabel,graph);
  }
  
  template<class F>
  bool regionGraphColor<F>::apply(const matrix<int>& regions,
                                  const channel& sc1,
                                  const channel& sc2,
                                  const channel& sc3,
                                  const int minLabel,
                                  graph_type& graph) {
    
    if ( (regions.size() != sc1.size()) ||
         (sc1.size() != sc2.size()) ||
         (sc2.size() != sc3.size()) ) {
      this->setStatusString("image and labeled mask have different sizes");
      return false;
    }

    // c1,c2,c3 are attributes of the class
    c1.copy(sc1);
    c2.copy(sc2);
    c3.copy(sc3);
    
    return apply(regions,minLabel,graph);
  }

  template<class F>
  bool regionGraphColor<F>::generate(const matrix<int>& regions,
                                     const channel& sc1,
                                     const channel& sc2,
                                     const channel& sc3,
                                     const int minLabel,
                                     graph_type& graph) {
    return apply(regions,sc1,sc2,sc3,minLabel,graph);
  }

  /*
   * Alias for merge()
   */
  template<class F>
  bool regionGraphColor<F>::apply(const weight_type& threshold,
				  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(threshold,graph,equivalences);
  }
  
  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphColor<F>::apply(graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(graph,equivalences);
  }
  
  /*
   * Alias for merge()
   */
  template<class F>
  bool regionGraphColor<F>::apply(const weight_type& threshold,
                                  const int minLabel,
				  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(threshold,minLabel,graph,equivalences);
  }
  
  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphColor<F>::apply(const int minLabel,
                                  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(minLabel,graph,equivalences);
  }


  /*
   * Alias for generate()
   */
  template<class F>
  bool regionGraphColor<F>::apply(const matrix<int>& regions,
				  const int minLabel,
				  graph_type& graph) {
    return parent::apply(regions,minLabel,graph);
  }


  // -------------------------------------------------------------------
  // Reimplementable methods
  // -------------------------------------------------------------------

  template<class F>
  bool 
  regionGraphColor<F>::checkInternalData(const ipoint& regionsSize,
                                         const int ) const {
    return ( (c1.size() == regionsSize) &&
             (c2.size() == regionsSize) &&
             (c3.size() == regionsSize) );
  }

  template<class F>
  bool 
  regionGraphColor<F>::considerForEdgeData(const ipoint& ,
                                           const ipoint& ,
                                           edge_data_type& edgeData) {
    edgeData++;
    return true;
  }

  template<class F>
  bool 
  regionGraphColor<F>::considerForNodeData(const ipoint& p1,
                                           const int ,
                                           node_type& nodeData) {
    nodeData.consider(rgbPixel<float>(c1.at(p1),c2.at(p1),c3.at(p1)));
    return true;
  }


  // --------------------------------------------------
  // regionGraphGray
  // --------------------------------------------------
  template<class F>
  regionGraphGray<F>::regionGraphGray() 
    : parent() {
  }

  template<class F>
  regionGraphGray<F>::regionGraphGray(const parameters& par) 
    : parent(par) {
  }

  template<class F>
  regionGraphGray<F>::regionGraphGray(const regionGraphGray& other)
    : parent() {
    this->copy(other);
  }

  template<class F>
  regionGraphGray<F>::~regionGraphGray() {
  }

  // returns the name of this type
  template<class F>
  const std::string& regionGraphGray<F>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template<class F>
  regionGraphGray<F>* regionGraphGray<F>::clone() const {
    return new regionGraphGray<F>(*this);
  }

  template<class F>
  regionGraphGray<F>* regionGraphGray<F>::newInstance() const {
    return new regionGraphGray<F>();
  }

  template<class F>
  bool regionGraphGray<F>::apply(const matrix<int>& regions,
                                  const channel& chnl,
                                  const int minLabel,
                                  graph_type& graph) {
    
    if (regions.size() != chnl.size()) {
      this->setStatusString("image and labeled mask have different sizes");
      return false;
    }

    c1.copy(chnl);

    return parent::apply(regions,minLabel,graph);
  }

  /*
   * Alias for merge()
   */
  template<class F>
  bool regionGraphGray<F>::apply(const weight_type& threshold,
				 graph_type& graph,
				 ivector& equivalences) {
    return parent::apply(threshold,graph,equivalences);
  }

  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphGray<F>::apply(graph_type& graph,
				 ivector& equivalences) {
    return parent::apply(graph,equivalences);
  }
  
  template<class F>
  bool regionGraphGray<F>::apply(const weight_type& threshold,
                                 const int minLabel,
                                 graph_type& graph,
                                 ivector& equivalences) {
    return parent::apply(threshold,minLabel,graph,equivalences);
  }
  
  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphGray<F>::apply(const int minLabel,
                                  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(minLabel,graph,equivalences);
  }

  /*
   * Alias for generate()
   */
  template<class F>
  bool regionGraphGray<F>::apply(const matrix<int>& regions,
                                 const int minLabel,
                                 graph_type& graph) {
    return parent::apply(regions,minLabel,graph);
  }

  // -------------------------------------------------------------------
  // Reimplementable methods
  // -------------------------------------------------------------------

  template<class F>
  bool 
  regionGraphGray<F>::checkInternalData(const ipoint& regionsSize,
                                        const int ) const {
    return (c1.size() == regionsSize);
  }

  template<class F>
  bool 
  regionGraphGray<F>::considerForEdgeData(const ipoint& ,
                                          const ipoint& ,
                                          edge_data_type& edgeData) {
    edgeData++;
    return true;
  }

  template<class F>
  bool 
  regionGraphGray<F>::considerForNodeData(const ipoint& p1,
                                          const int ,
                                          node_type& nodeData) {
    nodeData.consider(c1.at(p1));
    return true;
  }



}
#include "ltiUndebug.h"
