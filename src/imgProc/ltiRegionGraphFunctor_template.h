/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiRegionGraphFunctor_template.h
 *         Contains the class lti::regionGraphFunctor used in
 *         graph representations of images
 * 
 * \author Pablo Alvarado
 * \date   25.10.2003
 *
 * revisions ..: $Id: ltiRegionGraphFunctor_template.h,v 1.2 2012-09-16 04:58:33 alvarado Exp $
 */


#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include <iostream>
#include "ltiLispStreamHandler.h"

namespace std {
  inline ostream& operator<<(ostream& out,lti::ioObject& obj) {
    lti::lispStreamHandler lsh(out);
    obj.write(lsh);
    return out;
  }
}

#endif

namespace lti {
  // --------------------------------------------------
  // regionGraphFunctor::parameters
  // --------------------------------------------------

  /*
   * Default constructor
   */
  template<class G>
  regionGraphFunctor<G>::parameters::parameters(const eMergeMode& mode,
                                                const weight_type& threshold) 
        : functor::parameters() {
    mergeMode = mode;
    mergeThreshold = threshold;
    minRegionNumber = 1;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  template<class G>
  regionGraphFunctor<G>::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }
      
  /*
   * Destructor
   */
  template<class G>
  regionGraphFunctor<G>::parameters::~parameters() {
  }

  /*
   * returns name of this type
   */
  template<class G>
  const std::string& regionGraphFunctor<G>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy the contents of a parameters object.
   *
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  template<class G>
  typename regionGraphFunctor<G>::parameters& 
  regionGraphFunctor<G>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    mergeMode = other.mergeMode;
    mergeThreshold = other.mergeThreshold;
    minRegionNumber = other.minRegionNumber;
    
    return *this;
  }

  /*
   * Copy the contents of a parameters object.
   *
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  template<class G>
  typename regionGraphFunctor<G>::parameters&
  regionGraphFunctor<G>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * Returns a pointer to a clone of the parameters
   */
  template<class G>
  typename regionGraphFunctor<G>::parameters* 
  regionGraphFunctor<G>::parameters::clone() const {
    return new parameters(*this);
  }

  template<class G>
  typename regionGraphFunctor<G>::parameters* 
  regionGraphFunctor<G>::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template<class G>
  bool regionGraphFunctor<G>::parameters::write(ioHandler& handler,
                                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      switch(mergeMode) {
        case Optimal:
          lti::write(handler,"mergeMode","Optimal");
          break;
        case Fast:
          lti::write(handler,"mergeMode","Fast");
          break;
        default:
          lti::write(handler,"mergeMode","Optimal");
      }
      
      lti::write(handler,"mergeThreshold",mergeThreshold);
      lti::write(handler,"minRegionNumber",minRegionNumber);
    }
    
    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }
  
  /**
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template<class G>
  bool regionGraphFunctor<G>::parameters::read(ioHandler& handler,
                                               const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      std::string str;
      lti::read(handler,"mergeMode",str);
      if (str == "Optimal") {
        mergeMode = Optimal;
      } else {
        mergeMode = Fast;
      }
      
      lti::read(handler,"mergeThreshold",mergeThreshold);
      lti::read(handler,"minRegionNumber",minRegionNumber);
    }
    
    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  
  // --------------------------------------------------
  // regionGraphFunctor
  // --------------------------------------------------

  // default constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const bool initParams)
    : functor() {
    if (initParams) {
      // create an instance of the parameters with the default values
      parameters defaultParameters;
      // set the default parameters
      setParameters(defaultParameters);
    }
  }

  // default constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const eMergeMode& mode,
                                            const weight_type& threshold)
    : functor() {
    // create an instance of the parameters with the default values
    parameters defaultParameters(mode,threshold);
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const regionGraphFunctor<G>& other)
    : functor() {
    copy(other);
  }

  // destructor
  template<class G>
  regionGraphFunctor<G>::~regionGraphFunctor() {
  }

  // returns the name of this type
  template<class G>
  const std::string& regionGraphFunctor<G>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template<class G>
  regionGraphFunctor<G>&
  regionGraphFunctor<G>::copy(const regionGraphFunctor<G>& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class G>
  regionGraphFunctor<G>&
  regionGraphFunctor<G>::operator=(const regionGraphFunctor<G>& other) {
    return (copy(other));
  }

  // clone member
  template<class G>
  regionGraphFunctor<G>* regionGraphFunctor<G>::clone() const {
    return new regionGraphFunctor<G>(*this);
  }

  // clone member
  template<class G>
  regionGraphFunctor<G>* regionGraphFunctor<G>::newInstance() const {
    return new regionGraphFunctor<G>();
  }


  // return parameters
  template<class G>
  const typename regionGraphFunctor<G>::parameters&
  regionGraphFunctor<G>::getParameters() const {
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

  // On place apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const matrix<int>& regions,
                                    const int minLabel,
                                    graph_type& graph)  {

    return generate(regions,minLabel,graph);
  }

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const weight_type& threshold,
                                    graph_type& graph,
                                    ivector& equivalences)  {

    return merge(threshold,graph,equivalences);
  }

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(graph_type& graph,
                                    ivector& equivalences)  {

    return merge(getParameters().mergeThreshold,graph,equivalences);
  }

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const weight_type& threshold,
                                    const int minLabel,
                                    graph_type& graph,
                                    ivector& equivalences)  {

    return merge(threshold,minLabel,graph,equivalences);
  }

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const int minLabel,
                                    graph_type& graph,
                                    ivector& equivalences)  {

    return merge(getParameters().mergeThreshold,minLabel,graph,equivalences);
  }

  // -------------------------------------------------------------------
  // Reimplementable methods
  // -------------------------------------------------------------------

  template<class G>
  bool 
  regionGraphFunctor<G>::checkInternalData(const ipoint& ,
                                           const int ) const {
    // default implementation does nothing:
    // internal data (which is nonexisten) has always the proper size!
    return true;
  }

  template<class G>
  bool 
  regionGraphFunctor<G>::considerForEdgeData(const ipoint& ,
                                             const ipoint& ,
                                             edge_data_type& ) {
    // default implementation does nothing:
    // internal data (which is nonexistent) has always the proper size!
    return true;
  }

  template<class G>
  bool 
  regionGraphFunctor<G>::considerForNodeData(const ipoint& ,
                                             const int ,
                                             node_type& ) {
    // default implementation does nothing:
    // internal data (which is nonexistent) has always the proper size!
    return true;
  }

  template<class G>
  bool 
  regionGraphFunctor<G>::prepareEdgeAndNodeData(graph_type& ) {
    return true;
  }

  // -------------------------------------------------------------------
  // The functionality methods
  // -------------------------------------------------------------------

  template<class G>
  bool regionGraphFunctor<G>::generate(const matrix<int>& regions,
                                       const int minLabel,
                                       graph_type& graph) {

    // clear the graph
    graph.clear();

    const int lastLabel = regions.findMaximum();

    if (!checkInternalData(regions.size(),lastLabel)) {
      setStatusString("Internal data check failed");
      return false;
    }

    // set the number of nodes of the graph
    graph.resize(lastLabel+1);

    return generateWorker(regions,minLabel,graph);
  }

  template<class G>
  bool regionGraphFunctor<G>::generate(const matrix<int>& regions,
                                       const int minLabel,
                                       const std::vector<node_type>& data,
                                       graph_type& graph) {

    // clear the graph
    graph.clear();

    const int lastLabel = regions.findMaximum();

    if (!checkInternalData(regions.size(),lastLabel)) {
      setStatusString("Internal data check failed");
      return false;
    }

    graph.resize(lastLabel+1);

    int i;
    const int s = min(static_cast<int>(data.size()),lastLabel+1);
    
    for (i=0;i<s;++i) {
      graph.getNodeData(i) = data[i];
    }

    return generateWorker(regions,minLabel,graph);
  }


  template<class G>
  bool regionGraphFunctor<G>::generateWorker(const matrix<int>& regions,
                                             const int minLabel,
                                             graph_type& graph) {

    // work mostly with the down diagonal adjacency matrix, so that
    // the first edge iterators of each row with higher probability 
    // point to a valid edge.


    int l1,l2,a,b;
    ipoint p;

    // create the huge adjacency matrix 
    for (p.y=0;p.y<regions.lastRow();++p.y) {
      for (p.x=0;p.x<regions.lastColumn();++p.x) {
        // update node's data
        a=regions.at(p);
        considerForNodeData(p,a,graph.getNodeData(a));

        if (a != (b=regions.at(p.y,p.x+1))) {
          minmax(a,b,l1,l2);
          if (l2 >= minLabel) {
            considerForEdgeData(p,ipoint(p.x+1,p.y),
                                graph.forceTopologicalEdge(l2,l1));
          }
        }

        if (a != (b=regions.at(p.y+1,p.x))) {
          minmax(a,b,l1,l2);
          if (l2 >= minLabel) {
            considerForEdgeData(p,ipoint(p.x,p.y+1),
                                graph.forceTopologicalEdge(l2,l1));
          }
        }
      }

      // last column
      a=regions.at(p);
      considerForNodeData(p,a,graph.getNodeData(a));

      if (a != (b=regions.at(p.y+1,p.x))) {
        minmax(a,b,l1,l2);
        if (l2 >= minLabel) {
            considerForEdgeData(p,ipoint(p.x,p.y+1),
                                graph.forceTopologicalEdge(l2,l1));
        }
      }
    }

    // last row
    for (p.x=0;p.x<regions.lastColumn();++p.x) {
      a=regions.at(p);
      considerForNodeData(p,a,graph.getNodeData(a));

      if (a != (b=regions.at(p.y,p.x+1))) {
        minmax(a,b,l1,l2);
        if (l2 >= minLabel) {
          considerForEdgeData(p,ipoint(p.x+1,p.y),
                              graph.forceTopologicalEdge(l2,l1));
        }
      } 
    }

    // last pixel
    a=regions.at(p);
    considerForNodeData(p,a,graph.getNodeData(a));

    // ------------------------------------------------------------------------
    
    // The adjacency matrix is ready with all preliminary node and edge
    // data.
    // Before we continue computing the edge weights, a few pre-computations
    // can be necessary:
    if (!prepareEdgeAndNodeData(graph)) {
      return false;
    }

    // ------------------------------------------------------------------------
    // save first the data in temporary vectors and construct the
    // whole queue at once, which is much much faster than inserting
    // each element one after another.

    if (edge_traits::Symmetric) {
      mirrorEdgeData(graph);
    }

    return graph.recomputeAllWeights();
  }
  
  // mirrorEdgeData()
  template<class G>
  bool regionGraphFunctor<G>::mirrorEdgeData(graph_type& graph) const {
    typename graph_type::const_iterator nit,enit;
    typename graph_type::edge_iterator it,eit;
    typename graph_type::node_pair p;
    // for all nodes
    for (nit=graph.begin(),enit=graph.end();nit!=enit;++nit) {
      for (it=graph.begin(nit.id()),eit=graph.end(nit.id());it!=eit;++it) {
        p = graph.getEdge(it);
        if (p.second < p.first) {
          graph.getEdgeData(p.second,p.first) = *it;
        } else {
          break; // nothing more to do for this node
        }
      }
    }

    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::affinityMatrix(const graph_type& graph,
                                             matrix<weight_type>& affinity,
                                             const weight_type noEdgeValue
                                             ) const {

    typename graph_type::const_iterator nit,enit;
    typename graph_type::const_edge_iterator it,eit;
    typename graph_type::node_pair p;

    const int size = graph.lastValidId()+1;
    affinity.assign(size,size,noEdgeValue); // init with zero
    weight_type w;
    
    // for all nodes
    for (nit=graph.begin(),enit=graph.end();nit!=enit;++nit) {
      // for all edges
      for (it=graph.begin(nit.id()),eit=graph.end(nit.id());it!=eit;++it) {
        p = graph.getEdge(it);   // get pair of nodes
        if (!edge_traits::Symmetric ||  (p.second < p.first)) {
          // still edges with data:
          w = graph.getEdgeWeight(it);
          affinity.at(p.first,p.second) = w;
          if (edge_traits::Symmetric) {
            affinity.at(p.second,p.first) = w;
          }
        } else {
          // end of relevant data
          break; 
        }
      }
    }
    
    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::merge(const weight_type& threshold,
                                    graph_type& graph,
                                    ivector& equivalences) const {
    const int lastLabel = graph.lastValidId();
    const parameters& par = getParameters();
    equivalences.assign(lastLabel+1,lastLabel+1);
    weight_type w;
    node_pair p;
    typename graph_type::id_type surviver;

    // this takes O(n) time!
    int numNodes = graph.totalAdjacentNodes();

    if (par.mergeMode == Fast) {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) &&
             (numNodes > par.minRegionNumber)) {
        surviver = graph.topologicalMerge(p);
        numNodes--;
        if (surviver == p.first) {
          // p.first kept
          equivalences.at(p.second)=p.first;
        } else {
          // p.second kept
          equivalences.at(p.first)=p.second;
        }      
      }
    } else {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) &&
             (numNodes > par.minRegionNumber)) {
        surviver = graph.mergeNodes(p);
        numNodes--;
        if (surviver == p.first) {
          // p.first kept
          equivalences.at(p.second)=p.first;
        } else {
          // p.second kept
          equivalences.at(p.first)=p.second;
        }
      }
    }

    // fix the equivalences vector
    int i,j;
    for (i=0;i<equivalences.size();++i) {
      j=i;
      while (equivalences.at(j)<j) {
        j=equivalences.at(j);
      }
      equivalences.at(i)=j;
    }

    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::merge(const weight_type& threshold,
                                    const int minLabel,
                                    graph_type& graph,
                                    ivector& equivalences) const {

    const int lastLabel = graph.lastValidId();
    const parameters& par = getParameters();
    int i,j;
    weight_type w;
    node_pair p;
    typename graph_type::id_type surviver;

    // this takes O(n) time!
    int numNodes = graph.totalAdjacentNodes();

    equivalences.allocate(lastLabel+1);
    for (i=0;i<equivalences.size();++i) {
      equivalences.at(i) = i;
    }

    if (par.mergeMode == Fast) {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) && 
             (numNodes > par.minRegionNumber)) {
        if (max(equivalences.at(p.first),
                equivalences.at(p.second)) >= minLabel) {
          _lti_debug("Merging " << p.first << " and " << p.second << 
                     " (dist= " << w << " )" << std::endl);
          // if at least one label is not in the protected node heap merge
          // the nodes
          surviver = graph.topologicalMerge(p);
          numNodes--;
        
          if (surviver == p.first) {
            // p.first kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.second) = surviver;
          } else {
            // p.second kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.first)=surviver;
          }
        } else {
          _lti_debug("Removing edge between " << p.first << " and " << 
                     p.second << " (dist= " << w << " )" << std::endl);
          // it seems that this edge would merge protected nodes: remove the 
          // edge
          graph.removeEdge(p);
        }
      }
    } else {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) && 
             (numNodes > par.minRegionNumber)) {
        if (max(equivalences.at(p.first),
                equivalences.at(p.second)) >= minLabel) {
          _lti_debug("Merging " << p.first << " and " << p.second << 
                     " (dist= " << w << " )" << std::endl);
          // if at least one label is not in the protected node heap merge
          // the nodes
          surviver = graph.mergeNodes(p);
          numNodes--;
        
          if (surviver == p.first) {
            // p.first kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.second) = surviver;
          } else {
            // p.second kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.first)=surviver;
          }
        } else {
          _lti_debug("Removing edge between " << p.first << " and " << 
                     p.second << " (dist= " << w << " )" << std::endl);
          // it seems that this edge would merge protected nodes: remove the 
          // edge
          graph.removeEdge(p);
        }
      }
    }

    // fix the equivalences vector
    for (i=0;i<equivalences.size();++i) {
      j=i;
      while (equivalences.at(j)<j) {
        j=equivalences.at(j);
      }
      equivalences.at(i)=j;
    }

    return true;
  }


  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                             const imatrix& regions,
                                                   imatrix& newRegions,
                                             const bool compact) const {

    if (regions.empty()) {
      setStatusString("Labeled mask was empty.");
      return false;
    }

    ivector labels;
    const ivector* theLabels = 0;
    if (compact) {
      compactLabels(equivalences,labels);
      theLabels = &labels;
    } else {
      theLabels = &equivalences;
    }


    newRegions.allocate(regions.size());
    int y,x;
    for (y=0;y<regions.rows();++y) {
      for (x=0;x<regions.columns();++x) {
        newRegions.at(y,x) = theLabels->at(regions.at(y,x));
      }
    }

    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                                   imatrix& regions,
                                                   ivector& regionSizes,
                                             const bool compact) const {
    imatrix tmp;
    if (reassignLabels(equivalences,regions,tmp,regionSizes,compact)) {
      tmp.detach(regions);
      return true;
    }
    return false;
  }
  
  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                             const imatrix& regions,
                                                   imatrix& newRegions,
                                                   ivector& regionSizes,
                                             const bool compact) const {

    if (regions.empty()) {
      setStatusString("Labeled mask was empty.");
      return false;
    }

    int size;
    ivector labels;
    const ivector* theLabels = 0;
    if (compact) {
      size = compactLabels(equivalences,labels);
      theLabels = &labels;
      
    } else {
      theLabels = &equivalences;
      size = equivalences.size();
    }

    regionSizes.assign(size,0);

    newRegions.allocate(regions.size());
    int y,x,i;
    for (y=0;y<regions.rows();++y) {
      for (x=0;x<regions.columns();++x) {
        i=theLabels->at(regions.at(y,x));
        newRegions.at(y,x) = i;
        regionSizes.at(i)++;
      }
    }

    return true;
  }


  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                                   imatrix& regions,
                                             const bool compact) const {
    
    imatrix tmp;
    if (reassignLabels(equivalences,regions,tmp,compact)) {
      tmp.detach(regions);
      return true;
    }
    
    return false;
  }

  template<class G>
  int regionGraphFunctor<G>::compactLabels(const ivector& equivalences,
                                           ivector& newEquivalences) const {
    
    // ------------------------------------
    // use only as many labels as necessary
    // ------------------------------------
    ivector usedLabels(equivalences.size(),-1);
    int i;

    // which labels are used?
    for (i=0;i<equivalences.size();++i) {
      usedLabels.at(equivalences.at(i))=1;
    }

    // reassign labels
    int l1=0;
    for (i=0;i<usedLabels.size();++i) {
      if (usedLabels.at(i)>0) {
        usedLabels.at(i)=l1;
        l1++;
      }
    }
    
    newEquivalences.allocate(equivalences.size());
    

    // now equivalences must be updated
    for (i=0;i<equivalences.size();++i) {
      newEquivalences.at(i)=usedLabels.at(equivalences.at(i));
    }
    
    return l1;
 }
}

#include "ltiUndebug.h"
