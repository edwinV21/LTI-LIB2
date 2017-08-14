/*
 * Copyright (C) 1998-2006
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
 * \file   ltiLabelAdjacencyMap.cpp
 * \author Pablo Alvarado
 * \date   18.11.2002
 * 
 * $Id: ltiLabelAdjacencyMap.cpp,v 1.11 2014-10-21 14:19:39 alvarado Exp $
 */

#include "ltiLabelAdjacencyMap.h"
#include "ltiColors.h"
#include "ltiUsePalette.h"
#include <limits>

namespace lti {
  
  // --------------------------------------------------
  // labelAdjacencyMap::parameters
  // --------------------------------------------------

  static const rgbaPixel palData[] = {lti::Black,
                                      lti::Red,       lti::Green, lti::Blue,
                                      lti::Yellow,    lti::Cyan,  lti::Magenta,
                                      lti::DarkOrange,lti::DarkGreen,
                                      lti::DarkCyan,  lti::Violet,
                                      lti::Grey75};

  /*
   * Defaut color palette
   */
  const palette labelAdjacencyMap::defaultPalette(12,palData);

  // default constructor
  labelAdjacencyMap::parameters::parameters()
    : functor::parameters() {
    minColors = bool(false);
    thePalette.copy(defaultPalette);
    neighborhood = int(8);
  }

  // copy constructor
  labelAdjacencyMap::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  labelAdjacencyMap::parameters::~parameters() {
  }

  // get type name
  const std::string& labelAdjacencyMap::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  labelAdjacencyMap::parameters&
  labelAdjacencyMap::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    minColors = other.minColors;
    thePalette.copy(other.thePalette);
    neighborhood = other.neighborhood;

    return *this;
  }

  // alias for copy member
  labelAdjacencyMap::parameters&
    labelAdjacencyMap::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  labelAdjacencyMap::parameters* labelAdjacencyMap::parameters::clone() const {
    return new parameters(*this);
  }

  labelAdjacencyMap::parameters* 
  labelAdjacencyMap::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool labelAdjacencyMap::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"minColors",minColors);
      lti::write(handler,"thePalette",thePalette);
      lti::write(handler,"neighborhood",neighborhood);
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
  bool labelAdjacencyMap::parameters::read(ioHandler& handler,
                                           const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"minColors",minColors);
      lti::read(handler,"thePalette",thePalette);
      lti::read(handler,"neighborhood",neighborhood);
    }

    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // labelAdjacencyMap::graph
  // --------------------------------------------------

  labelAdjacencyMap::graph::graph() {
  }

  void labelAdjacencyMap::graph::acc(const int row,const int col) {
    edges_type& r = nodes_[row];
    r[col]++;
  }

  void labelAdjacencyMap::graph::clear() {
    nodes_.clear();
  }

  int labelAdjacencyMap::graph::findMaxId() const {
    if (nodes_.empty()) {
      return std::numeric_limits<int>::min();
    } else {
      return (*nodes_.rbegin()).first;
    }
  }

  int labelAdjacencyMap::graph::findMinId() const {
    if (nodes_.empty()) {
      return std::numeric_limits<int>::max();
    } else {
      return (*nodes_.begin()).first;
    }
  }
  
  void 
  labelAdjacencyMap::graph::findMinMaxIds(int& minId,int& maxId) const {

    if (nodes_.empty()) {
      minId = std::numeric_limits<int>::max();
      maxId = std::numeric_limits<int>::min();
    } else {
      minId = (*nodes_.begin()).first;
      maxId = (*nodes_.rbegin()).first;
    }
  }

  // --------------------------------------------------
  // labelAdjacencyMap
  // --------------------------------------------------

  // default constructor
  labelAdjacencyMap::labelAdjacencyMap()
    : functor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  labelAdjacencyMap::labelAdjacencyMap(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  labelAdjacencyMap::labelAdjacencyMap(const labelAdjacencyMap& other)
    : functor()  {
    copy(other);
  }

  // destructor
  labelAdjacencyMap::~labelAdjacencyMap() {
  }

  // returns the name of this type
  const std::string& labelAdjacencyMap::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  labelAdjacencyMap&
  labelAdjacencyMap::copy(const labelAdjacencyMap& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  labelAdjacencyMap&
  labelAdjacencyMap::operator=(const labelAdjacencyMap& other) {
    return (copy(other));
  }


  // clone member
  labelAdjacencyMap* labelAdjacencyMap::clone() const {
    return new labelAdjacencyMap(*this);
  }

  labelAdjacencyMap* labelAdjacencyMap::newInstance() const {
    return new labelAdjacencyMap;
  }

  // return parameters
  const labelAdjacencyMap::parameters&
    labelAdjacencyMap::getParameters() const {
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

  // Private class here and not inner class of the functor due to
  // MS-VC++ bugs (otherwise the implementation should be in the
  // header).
  template<class T>
  class adjacencyHelper {
  public:
    /**
     * compute the adjacency for a 4 neighborhood
     */
    bool adjacency4(const matrix<T>& src,
                    labelAdjacencyMap::graph& adjMap) {

      const int lastx = src.lastColumn();
      const int lasty = src.lastRow();

      adjMap.clear();
      
      int x,y;
      for (y=0;y<lasty;++y) {
        for (x=0;x<lastx;++x) {
          
          const T a = src.at(y,x);
          const T b = src.at(y,x+1);
          const T c = src.at(y+1,x);
          
          if (a != b) {
            adjMap.acc(a,b);
            adjMap.acc(b,a);
          }
          
          if (a != c) {
            adjMap.acc(a,c);
            adjMap.acc(c,a);
          }
        }
      }

      return true;
    }

    /**
     * compute the adjacency for an 8 neighborhood
     */
    bool adjacency8(const matrix<T>& src,
                    labelAdjacencyMap::graph& adjMap) {

      const int lastx = src.lastColumn();
      const int lasty = src.lastRow();

      adjMap.clear();
      
      int x,y;
      for (y=0;y<lasty;++y) {
        for (x=0;x<lastx;++x) {
          const T a = src.at(y,x);
          const T b = src.at(y,x+1);
          const T c = src.at(y+1,x);
          const T d = src.at(y+1,x+1);
          
          if (a != b) {
            adjMap.acc(a,b);
            adjMap.acc(b,a);
          }
          
          if (a != c) {
            adjMap.acc(a,c);
            adjMap.acc(c,a);
          }
          
          if (a != d) {
            adjMap.acc(a,d);
            adjMap.acc(d,a);
          }
          
          if (b != c) {
            adjMap.acc(b,c);
            adjMap.acc(c,b);
          }
        }
      }

      return true;
    }
  };

    // On copy apply for type channel8!
  bool 
  labelAdjacencyMap::apply(const matrix<ubyte>& src,image& dest) const {
    graph adj;
    usePalette colorizer;
    if (adjacency(src,adj)) {
      palette pal;
      if (computePalette(adj,pal)) {
	return colorizer.apply(src,pal,dest);
      } else {
	const parameters& par = getParameters();
	rgbaPixel color(0,0,0);
	if (!par.thePalette.empty()) {
	  color = par.thePalette[0];
	}
	dest.assign(src.size(),color);
	return true;
      }
    }

    return false;
  }

  // On copy apply for type matrix<int>!
  bool labelAdjacencyMap::apply(const matrix<int>& src,
                                image& dest) const {
    graph adj;
    usePalette colorizer;
    if (adjacency(src,adj)) {
      palette pal;
      if (computePalette(adj,pal)) {
	return colorizer.apply(src,pal,dest);
      } else {
	const parameters& par = getParameters();
	rgbaPixel color(0,0,0);
	if (!par.thePalette.empty()) {
	  color = par.thePalette[0];
	}
	// there was only one label present
	dest.assign(src.size(),color);
	return true;	
      }
    }

    return false;
  }

  // On copy apply for type matrix<int>!
  bool labelAdjacencyMap::adjacency(const matrix<int>& src,
                                    graph& dest) const {

    adjacencyHelper<int> helper;
    bool result(false);
    const parameters& par = getParameters();
    if (par.neighborhood == 4) {
      result = helper.adjacency4(src,dest);
    } else {
      result = helper.adjacency8(src,dest);
    }

    if (!result) {
      setStatusString("Invalid labeled mask: negative labels");
      return false;
    }

    return true;
  }

  // On copy apply for type matrix<int>!
  bool 
  labelAdjacencyMap::adjacency(const matrix<ubyte>& src,
                               graph& dest) const {

    adjacencyHelper<ubyte> helper;
    const parameters& par = getParameters();
    if (par.neighborhood == 4) {
      return helper.adjacency4(src,dest);
    } else {
      return helper.adjacency8(src,dest);
    }
    return false;
  }

  bool labelAdjacencyMap::computeMinPalette(const graph& adj,
                                            ivector& apal) const {

    int minLabel,last;
    adj.findMinMaxIds(minLabel,last);

    if (last<minLabel) {
      // there was only one single label
      apal.assign(1,0);
      return false;
    }

    last++; // last label + 1 is the size of the required LUT
    
    if (minLabel < 0) {
      // labels < 0 cannot be used as index for LUT!
      setStatusString("Labels with ids < 0 cannot be used!");
      return false;
    }

    const parameters& par = getParameters();
    const int palsize = par.thePalette.size();

    // apal contains for each region label the index of entry in the
    // parameters's palette .
    apal.resize(last,-1,Init); // -1 means, entry not assigned yet

    // tpal is a flag array that indicates which entries of the parameters'
    // palette are being used by the neighbors of the current region
    ivector tpal(palsize,-1);

    int k;
    graph::nodes_type::const_iterator i,ei;
    graph::edges_type::const_iterator j,l;

    // access to the adjacency matrix
    const graph::nodes_type& rows = adj.nodes_;
    for (i=rows.begin(),ei=rows.end();i!=ei;++i) { // for each label i 
      // mark all palette entries already used by the neighbors of
      // this label
      tpal.fill(-1);
      const graph::edges_type& nodes = (*i).second;

      for (j=nodes.begin(),l=nodes.end();j!=l;++j) { // for all neighbors of i
        if (apal.at((*j).first) >= 0) { // if that neighbor already assigned 
          tpal.at(apal.at((*j).first)) = +1; // mark that palette entry as used
        }
      }

      // choose the first not used palette entry
      k=0;
      while (k<palsize && tpal.at(k)>=0) {
        ++k;
      };

      // assign the first found label
      apal.at((*i).first) = k % palsize;
    }

    return true;
  }

  bool labelAdjacencyMap::computeMinPalette(const graph& adj,
                                            palette& pal) const {
    ivector apal;

    if (computeMinPalette(adj,apal)) {
      int k;
      const int last = apal.size();
      const parameters& par = getParameters();
      pal.resize(last,Black,Init);
      for (k=0;k<last;++k) {
        pal.at(k) = par.thePalette.at(max(0,apal.at(k)));
      }
      return true;
    }
    return false;
  }

  bool labelAdjacencyMap::computeMaxPalette(const graph& adj,
                                            ivector& apal) const {
    // access to the adjacency matrix
    const graph::nodes_type& rows = adj.nodes_;
    
    int minLabel,last;
    adj.findMinMaxIds(minLabel,last);

    if (last<minLabel) {
      // there was only one single label
      apal.assign(1,0);
      return false;
    }

    last++; // last label + 1 is the size of the required LUT
    
    if (minLabel < 0) {
      // labels < 0 cannot be used as index for LUT!
      setStatusString("Labels with ids < 0 cannot be used!");
      return false;
    }

    const parameters& par = getParameters();
    const int palsize = par.thePalette.size();

    // apal contains for each region label the index of entry in the
    // parameters's palette .
    apal.resize(last,-1,Init); // -1 means, entry not assigned yet

    // tpal is a flag array that indicates which entries of the parameters'
    // palette are being used by the neighbors of the current region
    ivector tpal(palsize,-1);

    graph::nodes_type::const_iterator i,ei;
    graph::edges_type::const_iterator j,l;

    int jj,k(0);
    for (i=rows.begin(),ei=rows.end();i!=ei;++i) { // for each label i 
      // mark all palette entries already used by the neighbors of
      // this label
      tpal.fill(-1);
      const graph::edges_type& nodes = (*i).second;

      for (j=nodes.begin(),l=nodes.end();j!=l;++j) { // for all neighbors of i
        if (apal.at((*j).first) >= 0) { // if that neighbor already assigned 
          tpal.at(apal.at((*j).first)) = +1; // mark that palette entry as used
        }
      }

      // choose the first not used palette entry
      jj=0;
      while (jj<palsize && tpal.at((k+jj)%palsize)>=0) {
        ++jj;
      };
      // assign the first found label
      apal.at((*i).first) = (k+jj) % palsize;

      k++;
    }

    return true;
  }

  bool labelAdjacencyMap::computeMaxPalette(const graph& adj,
                                            palette& pal) const {

    ivector apal;
    
    if (computeMaxPalette(adj,apal)) {
      int k;
      const int last = apal.size();
      const parameters& par = getParameters();
      pal.resize(last,Black,Init);
      for (k=0;k<last;++k) {
        pal.at(k) = par.thePalette.at(max(0,apal.at(k)));
      }
      return true;
    }
    return false;
  }

  bool labelAdjacencyMap::computePalette(const graph& adj,
                                         palette& pal) const {

    const parameters& par = getParameters();
    if (par.minColors) {
      return computeMinPalette(adj,pal);
    } else {
      return computeMaxPalette(adj,pal);
    }
    return false;
  }  

  bool labelAdjacencyMap::computePalette(const graph& adj,
                                               ivector& lut) const {

    const parameters& par = getParameters();
    if (par.minColors) {
      return computeMinPalette(adj,lut);
    } else {
      return computeMaxPalette(adj,lut);
    }
    return false;
  }
}






